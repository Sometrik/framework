package com.sometrik.framework;

import java.util.ArrayList;
import java.util.HashMap;

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
import android.text.InputType;
import android.util.DisplayMetrics;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.Window;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

public class FrameWork extends Activity implements SurfaceHolder.Callback, NativeMessageHandler {

  private MyGLSurfaceView mGLView;
  private RelativeLayout mainView;
  private SharedPreferences prefs;
  private SharedPreferences.Editor editor;
  private FrameWork frameWork;

  private static final int RESULT_SETTINGS = 1;

  private Settings settings;

  private Boolean firstViewSet = false;
  private float screenHeight;
  private float screenWidth;
  public Handler mainHandler;
  private Intent dialogIntent;
  private Bitmap picture;
  private AlertDialog.Builder builder;
  private AlertDialog alert;
  private float windowYcoords;
  public static HashMap<Integer, NativeMessageHandler> views = new HashMap<Integer, NativeMessageHandler>();
  public static int currentView;
  private MyGLRenderer renderer;
  private int appId = 0;

  public native void NativeOnTouch();
  public native int GetInt(float x, float y);
  public native String getText();
  public native void okPressed(String text);
  public native void buttonClicked(int id);
  public native void textChangedEvent(int id, String text);
  public native void settingsCreator(Settings settings, int id);
  public native void menuPressed();
  public native void touchEvent(int mode, int fingerIndex, long time, float x, float y);
  public native void onInit(AssetManager assetManager, float xSize, float ySize, float displayScale, Boolean hasEs3);
  public native void nativeSetSurface(Surface surface, int surfaceId);
  public native void nativeOnResume(int appId);
  public native void nativeOnPause(int appId);
  public native void nativeOnStop(int appId);
  public native void nativeOnRestart(int appId);


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

    // Get preferences (simple key-value database)
    prefs = this.getSharedPreferences("com.example.Work", Context.MODE_PRIVATE);
    editor = prefs.edit();

    initNative();
  }

  @Override
  public void handleMessage(NativeMessage message) {
    System.out.println("id: " + message.getInternalId() + " MessageType: " + String.valueOf(message.getMessage()));

    switch (message.getMessage()) {
    case SHOW_VIEW:
      NativeMessageHandler formViewToShow = views.get((message.getInternalId()));
      if (formViewToShow != null) {
	formViewToShow.showView();
      } else {
	System.out.println("Message not handled");
      }
      break;
    case CREATE_APPLICATION:
      appId = message.getInternalId();
      break;
    case SET_CAPTION:
      setTitle(message.getTextValue());
      break;
    case CREATE_FORMVIEW:
      System.out.println("creating formView " + message.getChildInternalId());
      createFormView(message.getChildInternalId());
      break;
    case CREATE_OPENGL_VIEW:
      createOpenGLView(message.getChildInternalId());
      break;
    case CREATE_NATIVE_OPENGL_VIEW:
      createNativeOpenGLView(message.getChildInternalId());
      break;
    // Create notification
    case POST_NOTIFICATION:
      createNotification("", "");
      break;
    // Open Browser
    case LAUNCH_BROWSER:
      launchBrowser("");
      break;
    default:
      NativeMessageHandler handlerView = views.get((message.getInternalId()));
      if (handlerView != null) {
	handlerView.handleMessage(message);
      } else {
	System.out.println("Message not handled");
      }
      break;
    }
  }

  private void initNative(){
    
    DisplayMetrics displayMetrics = getDisplayMetrics();
    System.out.println("Display scale: " + displayMetrics.scaledDensity);
    final ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
    final ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
    Boolean hasEs3;
    if (configurationInfo.reqGlEsVersion >= 0x30000){
      hasEs3 = true;
    } else if (configurationInfo.reqGlEsVersion >= 0x20000) {
      System.out.println("openGLES 3 isn't supported");
      hasEs3 = false;
    } else {
      hasEs3 = false;
      System.out.println("openGLES 2 isn't supported");
    }
    float xSize = displayMetrics.widthPixels / displayMetrics.scaledDensity;
    float ySize = displayMetrics.heightPixels / displayMetrics.scaledDensity;
    onInit(getAssets(), xSize, ySize, displayMetrics.scaledDensity, hasEs3);
  }

  // Get screen settings
  public DisplayMetrics getDisplayMetrics() {
    DisplayMetrics displaymetrics = new DisplayMetrics();
    getWindowManager().getDefaultDisplay().getMetrics(displaymetrics);
    screenHeight = displaymetrics.heightPixels;
    screenWidth = displaymetrics.widthPixels;
    return displaymetrics;
  }
  
  public static void addToViewList(int id, NativeMessageHandler view){
    System.out.println(view.getElementId() + " added to view list");
    views.put(id, view);
  }

  public MyGLSurfaceView getSurfaceView() {
    return mGLView;
  }

  public void launchBrowser(String url) {
    Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
    startActivity(browserIntent);
  }
  
  private void createFormView(int id){
    FWLayout layout = new FWLayout(this);
    layout.setId(id);
    views.put(id, layout);
  }
  private void createOpenGLView(int id){
    MyGLRenderer renderer = new MyGLRenderer(this, screenWidth, screenHeight);
    MyGLSurfaceView mGLView = new MyGLSurfaceView(this, renderer, id);
    mGLView.setOnTouchListener(new MyOnTouchListener(this));
    mGLView.setWillNotDraw(false);
    mGLView.getHolder().addCallback(this);
    views.put(id, mGLView);
    if (!firstViewSet) {
      setContentView(mGLView);
      currentView = id;
      firstViewSet = true;
    }
  }
  
  private void createNativeOpenGLView(int id){
    NativeSurface surfaceView = new NativeSurface(this);
    surfaceView.setId(id);
    surfaceView.setOnTouchListener(new MyOnTouchListener(this));
    surfaceView.getHolder().addCallback(this);
    views.put(id, surfaceView);
    if (!firstViewSet) {
      setContentView(surfaceView);
      currentView = id;
      firstViewSet = true;
    }
  }

  // Lisää kuvan antaminen // Aika // Ääni
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

  // Create dialog with user text input
  private void createInputDialog(String title, String message) {

    System.out.println("Creating input dialog");

    AlertDialog.Builder builder;
    builder = new AlertDialog.Builder(this);

    // Building an alert
    builder.setTitle(title);
    builder.setMessage(message);
    builder.setCancelable(false);

    final EditText input = new EditText(this);
    input.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_PASSWORD);

    builder.setView(input);

    // Negative button listener
    builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
      public void onClick(DialogInterface dialog, int id) {
	dialog.cancel();

      }
    });

    // Positive button listener
    builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
      public void onClick(DialogInterface dialog, int id) {

	String inputText = String.valueOf(input.getText());
	// editor.putString("username", "teksti" );
	okPressed(inputText);
	dialog.cancel();

      }
    });

    // Create and show the alert
    alert = builder.create();
    alert.show();

  }

  // create Message dialog
  private void showMessageDialog(String title, String message) {

    System.out.println("creating message dialog");

    AlertDialog.Builder builder;
    builder = new AlertDialog.Builder(this);

    // Building an alert
    builder.setTitle(title);
    builder.setMessage(message);
    builder.setCancelable(false);

    // Negative button listener
    builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
      public void onClick(DialogInterface dialog, int id) {
	dialog.cancel();

      }
    });

    // Positive button listener
    builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
      public void onClick(DialogInterface dialog, int id) {

	dialog.cancel();
      }
    });

    // Create and show the alert
    alert = builder.create();
    alert.show();

    System.out.println("message dialog created");

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

    public MyOnTouchListener(FrameWork frameWork) {
      this.frameWork = frameWork;

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
	touchEvent(1, event.getActionIndex(), (int) System.currentTimeMillis(), (int) event.getX(), (int) (screenHeight - event.getRawY() - windowYcoords));

	break;
	//Touch event of screen touch-down after the first touch
      case MotionEvent.ACTION_POINTER_DOWN:
//	System.out.println("Liike alkoi: " + event.getX() + " " + event.getY() + " - id: " + event.getActionIndex());
	touchEvent(1, event.getActionIndex(), (int) System.currentTimeMillis(), (int) event.getX(), (int) (screenHeight - event.getRawY() - windowYcoords));
	break;

	//Touch event of finger moving
      case MotionEvent.ACTION_MOVE:

	int pointerCount = event.getPointerCount();
	for (int i = 0; i < pointerCount; i++) {
	  pointerIndex = i;
	  int pointerId = event.getPointerId(pointerIndex);

	  if (pointerId == 0) {
	    // System.out.println("fingerOne move: " + event.getX(pointerIndex) + event.getY(pointerIndex));
		touchEvent(2, 0, (int) System.currentTimeMillis(), (int) event.getX(), (int) (screenHeight - event.getRawY() - windowYcoords));
	  }
	  if (pointerId == 1) {
	    // System.out.println("fingerTwo move: " + event.getX(pointerIndex) + event.getY(pointerIndex));
		touchEvent(2, 1, (int) System.currentTimeMillis(), (int) event.getX(), (int) (screenHeight - event.getRawY() - windowYcoords));
	  }
	  if (pointerId == 2) {
//	     System.out.println("fingerThree move: " + event.getX(pointerIndex) + event.getY(pointerIndex));
		touchEvent(2, 2, (int) System.currentTimeMillis(), (int) event.getX(), (int) (screenHeight - event.getRawY() - windowYcoords));
	  }
	  if (pointerId == 3) {
	    // System.out.println("fingerFour move: " + event.getX(pointerIndex) + event.getY(pointerIndex));
		touchEvent(2, 3, (int) System.currentTimeMillis(), (int) event.getX(), (int) (screenHeight - event.getRawY() - windowYcoords));
	  }
	  if (pointerId == 4) {
	    // System.out.println("fingerFive move: " + event.getX(pointerIndex) + event.getY(pointerIndex));
		touchEvent(2, 4, (int) System.currentTimeMillis(), (int) event.getX(), (int) (screenHeight - event.getRawY() - windowYcoords));
	  }
	}
	// System.out.println("Liikettä: " + event.getX() + " " +
	// event.getY() + " - id: " + event.getActionIndex());
	break;
	//touch event of first finger being removed from the screen
      case MotionEvent.ACTION_UP:
	//touch event of fingers other than the first leaving the screen
      case MotionEvent.ACTION_POINTER_UP:
	touchEvent(3, event.getActionIndex(), (int) System.currentTimeMillis(), (int) event.getX(), (int) (screenHeight - event.getRawY() - windowYcoords));
	break;
      }
      return true;
    }
  }

  //Build in Methods of Menu Creating. Propably removable
  @Override
  public boolean onCreateOptionsMenu(Menu menu) {

    System.out.println("onCreateOptionsMenu");
    // getMenuInflater().inflate(R.menu.settings, menu);
    return true;
  }

  @Override
  public boolean onKeyDown(int keycode, KeyEvent e) {
    switch (keycode) {
    case KeyEvent.KEYCODE_MENU:

      System.out.println("KeyEvent");
      mGLView.sHandler.sendEmptyMessage(2);
      return true;
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

    settings = new Settings(this);
    // Settings täytyy tehdä uusiksi tässä, jotta lista ei pysy samana
    settingsCreator(settings, id);
    

    getFragmentManager().beginTransaction().replace(android.R.id.content, settings).addToBackStack("main").commit();
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

  // returns database path
  public String getDBPath(String dbName) {
    System.out.println("getting DBPath _ db: " + dbName + " Path: " + String.valueOf(getDatabasePath(dbName)));
    return String.valueOf(getDatabasePath(dbName));
  }

  public static void sendMessage(FrameWork frameWork, NativeMessage message) {
    Message msg = Message.obtain(null, 1, message);
    frameWork.mainHandler.sendMessage(msg);
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
    nativeOnResume(appId);
  }
  @Override 
  public void onPause(){
    super.onPause();
    nativeOnPause(appId);
  }
  @Override 
  public void onStop(){
    super.onStop();
    nativeOnStop(appId);
  }
  @Override 
  public void onRestart(){
    super.onRestart();
    nativeOnRestart(appId);
  }
  

  @Override
  public void surfaceChanged(SurfaceHolder holder, int arg1, int arg2, int arg3) {
    nativeSetSurface(holder.getSurface(), currentView);
  }

  @Override
  public void surfaceCreated(SurfaceHolder holder) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void surfaceDestroyed(SurfaceHolder holder) {
    // Empty
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
    return 0;
  }

}
