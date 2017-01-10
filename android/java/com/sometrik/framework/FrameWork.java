package com.sometrik.framework;

import java.util.HashMap;

import com.android.trivialdrivesample.util.IabException;
import com.android.trivialdrivesample.util.IabHelper;
import com.android.trivialdrivesample.util.IabHelper.IabAsyncInProgressException;
import com.android.trivialdrivesample.util.IabResult;
import com.android.trivialdrivesample.util.Inventory;

import android.R.bool;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.AlertDialog;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ConfigurationInfo;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.PointF;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.preference.PreferenceManager;
import android.util.DisplayMetrics;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.Window;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

public class FrameWork extends Activity implements NativeCommandHandler {

  private RelativeLayout mainView;
  private SharedPreferences prefs;
  private SharedPreferences.Editor editor;
  private FrameWork frameWork;
  private double updateTimer = 0;
  private IabHelper purchaseHelper;
  private static final int RESULT_SETTINGS = 1;
  private Inventory inventory;
  
  private boolean drawMode = false;

  private Settings settings;

  private float screenHeight;
  private float screenWidth;
  public Handler mainHandler;
  private Intent dialogIntent;
  private Bitmap picture;
  private AlertDialog.Builder builder;
  private AlertDialog alert;
  private float windowYcoords;
  public static HashMap<Integer, NativeCommandHandler> views = new HashMap<Integer, NativeCommandHandler>();
  private int appId = 0;
  private int currentView = 0;

  public native void endModal(double timestamp, int value, byte[] textValue);
  public native void buttonClicked(double timestamp, int id);
  public native void textChangedEvent(double timestamp, int id, String text);
  public native void menuPressed(double timestamp, int viewId);
  public native void keyPressed(double timestamp, int keyId, int viewId);
  public native void touchEvent(int viewId, int mode, int fingerIndex, long time, float x, float y);
  public native void onInit(AssetManager assetManager, float xSize, float ySize, float displayScale);
  public native void nativeSetSurface(Surface surface, int surfaceId, int gl_version);
  public native void nativeSurfaceDestroyed(double timestamp, int surfaceId, int gl_version);
  public native void nativeOnResume(double timestamp, int appId);
  public native void nativeOnPause(double timestamp, int appId);
  public native void nativeOnStop(double timestamp, int appId);
  public native void nativeOnStart(double timestamp, int appId);
  public native void nativeOnDestroy(double timestamp, int appId);
  public static native void onPurchaseEvent(double purchaseTime, String orderId, Boolean newPurchase);
  public static native void onResize(double timestamp, float width, float height, int viewId);
  public static native void onUpdate(double timestamp, int viewId);

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);

    // You can disable status bar with this
    this.requestWindowFeature(Window.FEATURE_NO_TITLE);
    
    LinearLayout linear = new LinearLayout(this);
    linear.setId(-1);

    // Init for screen settings
    getDisplayMetrics();

    // Set up classes
    settings = new Settings(this);

    mainHandler = new Handler() {

      public void handleMessage(Message msg) {

	NativeCommand command = (NativeCommand) msg.obj;
	command.apply(FrameWork.views.get(command.getInternalId()));

      }

    };

    initNative();
  }

  public Boolean initializePurchaseHelper(String key, IabHelper.OnIabSetupFinishedListener listener) {
    // Get PurchaseHelper. Requires App public key
    purchaseHelper = new IabHelper(this, key);
    purchaseHelper.startSetup(listener);
    return true;
  }

  public Inventory getPurchaseHelperInventory() {
    System.out.println("about to query purchaseHelper inventory");
    try {
      inventory = purchaseHelper.queryInventory();
      return inventory;
    } catch (IabException e) {
      System.out.println("Exception getting inventory with message: " + e.getMessage());
      e.printStackTrace();
    }
    return null;
  }

  private void initNative() {    
    DisplayMetrics displayMetrics = getDisplayMetrics();
    System.out.println("Display scale: " + displayMetrics.scaledDensity);
    float xSize = displayMetrics.widthPixels / displayMetrics.scaledDensity;
    float ySize = displayMetrics.heightPixels / displayMetrics.scaledDensity;
    onInit(getAssets(), xSize, ySize, displayMetrics.scaledDensity);
  }

  // Get screen settings
  public DisplayMetrics getDisplayMetrics() {
    DisplayMetrics displaymetrics = new DisplayMetrics();
    getWindowManager().getDefaultDisplay().getMetrics(displaymetrics);
    screenHeight = displaymetrics.heightPixels;
    screenWidth = displaymetrics.widthPixels;
    return displaymetrics;
  }
  
  public void setSharedPreferences(String textValue){
    prefs = getSharedPreferences(textValue, Context.MODE_PRIVATE);
    editor = prefs.edit();
  }
  
  public SharedPreferences.Editor getPreferencesEditor(){ return editor; }

  public static void addToViewList(NativeCommandHandler view){
    System.out.println(view.getElementId() + " added to view list");
    views.put(view.getElementId(), view);
  }

  public void launchBrowser(String url) {
    Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
    startActivity(browserIntent);
  }

  public void setCurrentView(View view) {
    currentView = view.getId();
    setContentView(view);
  }
  
  private void createFormView(int id){
    FWLayout layout = new FWLayout(this);
    layout.setId(id);
    views.put(id, layout);
  }

  public NativeSurface createNativeOpenGLView(final int id) {
    final ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
    final ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
    final int gl_version = configurationInfo.reqGlEsVersion;    
    System.out.println("about to create native surface. gl_version: " + gl_version);
    NativeSurface surfaceView = new NativeSurface(this);
    surfaceView.setId(id);
    surfaceView.setLayoutParams(new FrameLayout.LayoutParams((int)screenWidth, (int)screenHeight));
    surfaceView.setOnTouchListener(new MyOnTouchListener(this, id));
    SurfaceHolder holder = surfaceView.getHolder();
    holder.setFixedSize((int)screenWidth, (int)screenHeight);
	holder.addCallback(new Callback() {
      public void surfaceDestroyed(SurfaceHolder holder) {
	System.out.println("surfaceDestroyed");
      	nativeSurfaceDestroyed(System.currentTimeMillis() / 1000.0, id, gl_version);
      }

      public void surfaceCreated(SurfaceHolder holder) { }

      public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
	System.out.println("setting native surface");
	nativeSetSurface(holder.getSurface(), id, gl_version);
	System.out.println("native surface has been set");
      }
    });

    System.out.println("...");
    views.put(id, surfaceView);
    if (currentView == 0){
      System.out.println("no current view set. showing created surfaceView");
      surfaceView.showView();
    }
    System.out.println("native surface created");
    return surfaceView;
  }

 // TODO: Add icon and sound
  public void createNotification(String title, String text) {

    System.out.println("Creating notification");

    Intent intent = new Intent(this, FrameWork.class);
    PendingIntent pIntent = PendingIntent.getActivity(this, (int) System.currentTimeMillis(), intent, 0);

    Notification.Builder builder = new Notification.Builder(this);

    builder.setContentTitle(title);
    builder.setContentText(text);
//    builder.setSmallIcon(R.drawable.picture);
    builder.setContentIntent(pIntent);
    builder.setAutoCancel(true);

    Notification notif = builder.build();
    NotificationManager notificationManager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
    notificationManager.notify(0, notif);

  }

  //Code to show user preferences on screen. Might be useful later
  private void showUserSettings() {
//    setContentView(R.layout.activity_main);
    System.out.println("showSettings called");
    SharedPreferences sharedPrefs = PreferenceManager.getDefaultSharedPreferences(this);
    StringBuilder builder = new StringBuilder();
    builder.append("\n Username: " + sharedPrefs.getString("prefUsername", "NULL"));
    builder.append("\n Send report:" + sharedPrefs.getBoolean("prefSendReport", false));
    builder.append("\n Sync Frequency: " + sharedPrefs.getString("prefSyncFrequency", "NULL"));
//    TextView settingsTextView = (TextView) findViewById(R.id.textUserSettings);
//    settingsTextView.setText(builder.toString());
  }

  private static PointF touchScreenStartPtArr[] = new PointF[10];

  //Screen touchevent listener. Will send information to MyGLSurfaceView messagehandler
  private class MyOnTouchListener implements OnTouchListener {

    FrameWork frameWork;
    int viewId;

    public MyOnTouchListener(FrameWork frameWork, int viewId) {
      this.frameWork = frameWork;
      this.viewId = viewId;
    }

    public void onClick(View v) {
      System.out.println("Click happened");
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
      // On touchesBegin(), touchesEnded(), touchesMoved(), Different
      // fingers (pointerId)
      Message msg;
      int[] intArray;

      int action = event.getAction() & MotionEvent.ACTION_MASK;
      int pointerIndex = (event.getAction() & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
      int fingerId = event.getPointerId(pointerIndex);

      switch (action) {

      //Touch event of screen touch-down for the first finger
      case MotionEvent.ACTION_DOWN:
	
//	System.out.println("Liike alkoi: " + event.getX() + " " + event.getY() + " - id: " + event.getActionIndex() + " time: " + System.currentTimeMillis());
	touchEvent(viewId, 1, event.getActionIndex(), System.currentTimeMillis(), (int) event.getX(), (int) (event.getRawY() + windowYcoords));

	break;
	//Touch event of screen touch-down after the first touch
      case MotionEvent.ACTION_POINTER_DOWN:
//	System.out.println("Liike alkoi: " + event.getX() + " " + event.getY() + " - id: " + event.getActionIndex());
	touchEvent(viewId, 1, event.getActionIndex(), System.currentTimeMillis(), (int) event.getX(), (int) (event.getRawY() + windowYcoords));
	break;

	//Touch event of finger moving
      case MotionEvent.ACTION_MOVE:

	int pointerCount = event.getPointerCount();
	for (int i = 0; i < pointerCount; i++) {
	  pointerIndex = i;
	  int pointerId = event.getPointerId(pointerIndex);

	     System.out.println("finger move. FingerId: " + pointerId);
	      touchEvent(viewId, 2, pointerId, System.currentTimeMillis(), (int) event.getX(), (int) (event.getRawY() + windowYcoords));
	}
	break;
	//touch event of first finger being removed from the screen
      case MotionEvent.ACTION_UP:
	//touch event of fingers other than the first leaving the screen
      case MotionEvent.ACTION_POINTER_UP:
	  touchEvent(viewId, 3, event.getActionIndex(), System.currentTimeMillis(), (int) event.getX(), (int) (event.getRawY() + windowYcoords));
	break;
      }
      return true;
    }
  }

  @Override
  public boolean onCreateOptionsMenu(Menu menu) {

    System.out.println("onCreateOptionsMenu");
//    menuPressed(System.currentTimeMillis() / 1000.0);
    // getMenuInflater().inflate(R.menu.settings, menu);
    return true;
  }

  @Override
  public boolean onKeyDown(int keycode, KeyEvent e) {
    switch (keycode) {
    case KeyEvent.KEYCODE_MENU:
      System.out.println("KeyEvent. KeyCode: " + keycode + " ViewId: " + findViewById(android.R.id.content).getRootView().getId());
      menuPressed(System.currentTimeMillis() / 1000.0, currentView);
      return true;
    default:
      System.out.println("KeyEvent. KeyCode: " + keycode + " ViewId: " + findViewById(android.R.id.content).getRootView().getId());
      keyPressed(System.currentTimeMillis() / 1000.0, e.getKeyCode(), currentView);
      break;
    }

    return super.onKeyDown(keycode, e);
  }
  

  private void createOptionsDialog(final int[] idArray, String[] names) {

    AlertDialog.Builder builder = new AlertDialog.Builder(this);
    builder.setTitle("Options Menu");

    builder.setItems(names, new DialogInterface.OnClickListener() {
      public void onClick(DialogInterface dialog, int item) {

	System.out.println("item selected: " + item);
	System.out.println("item id: " + idArray[item]);

	optionSelected(idArray[item]);

      }
    });

    AlertDialog alert = builder.create();
    alert.show();
  }
  
  //Called after option was selected from ActionSheet. Currently creates settings view
  private void optionSelected(int id) {

  }

  //Listener for built in menu options. Propably removable
  @Override
  public boolean onOptionsItemSelected(MenuItem item) {
    switch (item.getItemId()) {

//    case R.id.menu_settings:
//
//      getFragmentManager().beginTransaction().replace(android.R.id.content, new MyPreferenceFragment()).commit();
//
//      break;
//
//    case 1:
//      startActivity(new Intent(this, Settings.class));
//      break;

    }

    return true;
  }
  
  public float getScreenWidth(){
    return screenWidth;
  }
  
  public void setAppId(int id){
    this.appId = id;
  }
  
  public boolean getDrawMode(){ return drawMode; }
  
  public void disableDraw(){ drawMode = false; }

  // returns database path
  public String getDBPath(String dbName) {
    System.out.println("getting DBPath _ db: " + dbName + " Path: " + String.valueOf(getDatabasePath(dbName)));
    return String.valueOf(getDatabasePath(dbName));
  }

  public static void sendMessage(FrameWork frameWork, NativeCommand command) {
    Message msg = Message.obtain(null, 1, command);
    frameWork.mainHandler.sendMessage(msg);
  }
  
  public void addToPrefs(String key, String value){
    editor.putString(key, value);
    editor.apply();
  }
  
  public String getFromPrefs(String key){
    return prefs.getString(key, "");
  }
    
  @Override
  protected void onActivityResult(int requestCode, int resultCode, Intent data) {
    super.onActivityResult(requestCode, resultCode, data);

    switch (requestCode) {
    case RESULT_SETTINGS:
      // showUserSettings();
      break;

    }
  }
  
  @Override
  public void onConfigurationChanged(Configuration newConfig) {

    super.onConfigurationChanged(newConfig);

    if (newConfig.orientation == Configuration.ORIENTATION_PORTRAIT) {
      System.out.println("Orientation conf portrait");
    } else if (newConfig.orientation == Configuration.ORIENTATION_LANDSCAPE) {
      System.out.println("Orientation conf landscape");
    }
  }
  
  @Override
  public void onSaveInstanceState(Bundle savedInstanceState) {
    // Save the user's current game state
    // savedInstanceState.putInt(STATE_SCORE, mCurrentScore);
    // savedInstanceState.putInt(STATE_LEVEL, mCurrentLevel);

    // Always call the superclass so it can save the view hierarchy state
    super.onSaveInstanceState(savedInstanceState);
  }

  public void onRestoreInstanceState(Bundle savedInstanceState) {
    // Always call the superclass so it can restore the view hierarchy
    super.onRestoreInstanceState(savedInstanceState);

    // Restore state members from saved instance
    // mCurrentScore = savedInstanceState.getInt(STATE_SCORE);
    // mCurrentLevel = savedInstanceState.getInt(STATE_LEVEL);
  }
  
  
  @Override 
  public void onResume(){
    super.onResume();
    nativeOnResume(System.currentTimeMillis() / 1000.0, appId);
  }
  @Override 
  public void onPause(){
    super.onPause();
    nativeOnPause(System.currentTimeMillis() / 1000.0, appId);
  }
  @Override 
  public void onStop(){
    super.onStop();
    nativeOnStop(System.currentTimeMillis() / 1000.0, appId);
  }
  @Override 
  public void onStart(){
    super.onStart();
    nativeOnStart(System.currentTimeMillis() / 1000.0, appId);
  }
  @Override 
  public void onDestroy(){
    super.onDestroy();
    if (purchaseHelper != null){
    	try {
	  purchaseHelper.dispose();
	} catch (IabAsyncInProgressException e) {
	  e.printStackTrace();
	  System.out.println("Error in disposing purchaseHelper with message: " + e.getMessage());
	}
    }
  	purchaseHelper = null;
    nativeOnDestroy(System.currentTimeMillis() / 1000.0, appId);
  }
  
  public IabHelper getPurchaseHelper(){
  	return purchaseHelper;
  }


  //Load JNI. Framework references to make file.
  static {
    System.loadLibrary("framework");
  }

  @Override
  public void showView() {
  }
  @Override
  public int getElementId() {
    return appId;
  }
  @Override
  public void addChild(View view) {
    System.out.println("FrameWork couldn't handle addChild");
  }
  @Override
  public void addOption(int optionId, String text) {
    System.out.println("FrameWork couldn't handle addOption");
  }
  @Override
  public void removeChild(int id) {
    // TODO Auto-generated method stub
    
  }

}
