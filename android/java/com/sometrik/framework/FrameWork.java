package com.sometrik.framework;

import java.nio.charset.Charset;
import java.util.HashMap;
import java.util.Locale;

import com.android.trivialdrivesample.util.IabException;
import com.android.trivialdrivesample.util.IabHelper;
import com.android.trivialdrivesample.util.IabHelper.IabAsyncInProgressException;
import com.android.trivialdrivesample.util.Inventory;

import android.accounts.Account;
import android.accounts.AccountManager;
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
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.animation.Animation;
import android.view.animation.TranslateAnimation;
import android.view.inputmethod.InputMethodManager;
import android.widget.FrameLayout;
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
  private DisplayMetrics displayMetrics;
  private View currentlyShowingView;
  private Charset utf8_charset;
  
  private boolean drawMode = false;
  
  private Locale defaultLocale;
  
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
  public static boolean transitionAnimation = false;

  public native void endModal(double timestamp, int value, byte[] textValue);
  public native void textChangedEvent(double timestamp, int id, byte[] textValue);
  public native void intChangedEvent(double timestamp, int id, int changedInt);
  public native void keyPressed(double timestamp, int keyId, int viewId);
  public native void touchEvent(int viewId, int mode, int fingerIndex, long time, float x, float y);
  public native void onInit(AssetManager assetManager, float xSize, float ySize, float displayScale, String email, String language, String country);
  public native void nativeSetSurface(Surface surface, int surfaceId, int gl_version);
  public native void nativeSurfaceDestroyed(double timestamp, int surfaceId, int gl_version);
  public native void nativeOnResume(double timestamp, int appId);
  public native void nativeOnPause(double timestamp, int appId);
  public native void nativeOnStop(double timestamp, int appId);
  public native void nativeOnStart(double timestamp, int appId);
  public native void nativeOnDestroy(double timestamp, int appId);
  private native void setNativeActiveView(double timestamp, int activeView, boolean recordHistory);
  private native void languageChanged(double timestamp, int appId, String language);
  private native void memoryWarning(double timestamp, int appId);
  public static native void onPurchaseEvent(double timestamp, int applicationId, String orderId, boolean newPurchase, double purchaseTime);
  public static native void onResize(double timestamp, float width, float height, int viewId);
  public static native void onUpdate(double timestamp, int viewId);
  public static native void timerEvent(double timestamp, int viewId, int timerId);
  
  @Override
  protected void onCreate(Bundle savedInstanceState) {
    System.out.println("onCreate called");
    // Set default theme for the app. Commented default themes are dark versions
    if (Build.VERSION.SDK_INT <= 10) {
      // this.setTheme(android.R.style.Theme);
      this.setTheme(android.R.style.Theme_Light);
    } else if (Build.VERSION.SDK_INT >= 21) {
      // this.setTheme(android.R.style.Theme_Material);
      this.setTheme(android.R.style.Theme_Material_Light);
    } else {
      // this.setTheme(android.R.style.Theme_DeviceDefault);
      // this.setTheme(android.R.style.Theme_Holo);
      this.setTheme(android.R.style.Theme_Holo_Light);
    }
    super.onCreate(savedInstanceState);
    
    utf8_charset = Charset.forName("UTF-8");
    defaultLocale = Locale.getDefault();
    System.out.println("Users preferred locale: " + defaultLocale.getCountry() + " Language: " + defaultLocale.getLanguage());
    
    // You can disable status bar with this
//    this.requestWindowFeature(Window.FEATURE_NO_TITLE);
    
    // Init for screen settings
    setupDisplayMetrics();

    mainHandler = new Handler() {

      public void handleMessage(Message msg) {

	NativeCommand command = (NativeCommand) msg.obj;
	command.apply(FrameWork.views.get(command.getInternalId()));

      }

    };
    System.out.println("initing native on onCreate");
    initNative();
  }

  public Boolean initializePurchaseHelper(String key, IabHelper.OnIabSetupFinishedListener listener) {
    // Get PurchaseHelper. Requires App public key
    purchaseHelper = new IabHelper(this, key);
    purchaseHelper.startSetup(listener);
    return true;
  }
  
  private String getUserGoogleAccountEmail() {
    Log.d("accountFinder", "Checking for user Google Account");
    AccountManager manager = AccountManager.get(this);
    Account[] accounts = manager.getAccounts();
    String gmail = "";
    for (Account account : accounts) {
      if (account.type.equalsIgnoreCase("com.google")) {
	gmail = account.name;
	Log.d("accountFinder", "Google Account found: " + gmail);
	break;
      }
    }
    if (gmail == "") {
      Log.d("accountFinder", "No user Google Account found");
    }
    return gmail;
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
    System.out.println("Display scale: " + displayMetrics.scaledDensity);
    float xSize = displayMetrics.widthPixels / displayMetrics.scaledDensity;
    float ySize = displayMetrics.heightPixels / displayMetrics.scaledDensity;
    onInit(getAssets(), xSize, ySize, displayMetrics.scaledDensity, getUserGoogleAccountEmail(), defaultLocale.getLanguage(), defaultLocale.getCountry());
  }

  // Get screen settings
  public DisplayMetrics setupDisplayMetrics() {
    displayMetrics = new DisplayMetrics();
    getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
    screenHeight = displayMetrics.heightPixels;
    screenWidth = displayMetrics.widthPixels;
    return displayMetrics;
  }

  public void setSharedPreferences(String textValue) {
    prefs = getSharedPreferences(textValue, Context.MODE_PRIVATE);
    editor = prefs.edit();
  }

  public SharedPreferences.Editor getPreferencesEditor() { return editor; }

  public static void addToViewList(NativeCommandHandler view) {
    System.out.println(view.getElementId() + " added to view list");
    views.put(view.getElementId(), view);
  }

  public void launchBrowser(String url) {
    Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
    startActivity(browserIntent);
  }
  
  public void removeViewFromList(int viewId){
    FrameWork.views.remove(viewId);
  }

  public void setCurrentView(final View view, final boolean recordHistory) {
    if (currentView != 0) {

      View focusedView = this.getCurrentFocus();
      if (focusedView != null) {
	InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
	imm.hideSoftInputFromWindow(focusedView.getWindowToken(), 0);
      }
      
      TranslateAnimation r;
      if (recordHistory) {
	r = new TranslateAnimation(0, -2000, 0, 0);
      } else {
	r = new TranslateAnimation(0, 2000, 0, 0);
      }
      r.setDuration(200);
      r.setAnimationListener(new Animation.AnimationListener() {
	@Override
	public void onAnimationStart(Animation animation) {
	 transitionAnimation = true;
	}

	@Override
	public void onAnimationEnd(Animation animation) {
	  currentView = view.getId();
	  currentlyShowingView = view;
	  setContentView(view);
	  TranslateAnimation q;
	  if (recordHistory) {
	    q = new TranslateAnimation(2000, 0, 0, 0);
	  } else {
	    q = new TranslateAnimation(-2000, 0, 0, 0);
	  }
	  setNativeActiveView(System.currentTimeMillis() / 1000.0, view.getId(), recordHistory);
	  q.setAnimationListener(new Animation.AnimationListener() {
	    @Override
	    public void onAnimationEnd(Animation animation) { transitionAnimation = false; }
	    @Override
	    public void onAnimationRepeat(Animation animation) {  }
	    @Override
	    public void onAnimationStart(Animation animation) { }
	  });
	  q.setDuration(200);
	  view.startAnimation(q);
	}

	@Override
	public void onAnimationRepeat(Animation animation) {
	}
      });
      View sadas = (View) FrameWork.views.get(currentView);
      sadas.startAnimation(r);
    } else {
      currentView = view.getId();
      currentlyShowingView = view;
      setContentView(view);
      setNativeActiveView(System.currentTimeMillis() / 1000.0, view.getId(), false);
    }
  }
  
  public void setCurrentView(final View view, final boolean recordHistory, String title) {
    getActionBar().setTitle(title);
    setCurrentView(view, recordHistory);
  }
  
  public int getCurrentViewId() { return currentView; }

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
      //Set value shows view
      surfaceView.setValue(1);
    }
    System.out.println("native surface created");
    return surfaceView;
  }
  
  public void setActionBarTitle(String title){

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
    return true;
  }

  @Override
  public boolean onKeyDown(int keycode, KeyEvent e) {
    System.out.println("KeyEvent. KeyCode: " + keycode + " ViewId: " + findViewById(android.R.id.content).getRootView().getId());
    if (!transitionAnimation){
      keyPressed(System.currentTimeMillis() / 1000.0, e.getKeyCode(), currentView);
    }
    return true;
  }

  private void createOptionsDialog(final int[] idArray, String[] names) {

    AlertDialog.Builder builder = new AlertDialog.Builder(this);
    builder.setTitle("Options Menu");

    builder.setItems(names, new DialogInterface.OnClickListener() {
      public void onClick(DialogInterface dialog, int item) {

	System.out.println("item selected: " + item);
	System.out.println("item id: " + idArray[item]);
//	optionSelected(idArray[item]);

      }
    });

    AlertDialog alert = builder.create();
    alert.show();
  }
  
  public float getScreenWidth(){
    return screenWidth;
  }
  
  public void setAppId(int id){ this.appId = id; }
  public int getAppId(){ return appId; }

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

    Locale locale = getResources().getConfiguration().locale;
    if (locale.getLanguage() != defaultLocale.getLanguage()) {
      System.out.println("Language change spotted");
      System.out.println("Previous locale: " + defaultLocale.getCountry() + " Language: " + defaultLocale.getLanguage());
      System.out.println("New locale: " + locale.getCountry() + " Language: " + locale.getLanguage());
      languageChanged(System.currentTimeMillis() / 1000.0, appId, locale.getLanguage());
      defaultLocale = locale;
    }

    // super.onConfigurationChanged(newConfig);
    System.out.println("onConfigChange");
    boolean isLandscape = false;
    if (newConfig.orientation == Configuration.ORIENTATION_PORTRAIT) {
      System.out.println("Orientation conf portrait");
      isLandscape = false;
      onResize(System.currentTimeMillis() / 1000.0, screenHeight, screenHeight, currentView);
    } else if (newConfig.orientation == Configuration.ORIENTATION_LANDSCAPE) {
      System.out.println("Orientation conf landscape");
      isLandscape = true;
      onResize(System.currentTimeMillis() / 1000.0, screenHeight, screenHeight, currentView);
    }
    displayMetrics = setupDisplayMetrics();
    super.onConfigurationChanged(newConfig);
    
    for (NativeCommandHandler handler : views.values()){
      handler.onScreenOrientationChange(isLandscape);
    }
  }
  
  public int measureViewLength(View view){
    int widthMeasureSpec = View.MeasureSpec.makeMeasureSpec((int) screenWidth, View.MeasureSpec.AT_MOST);
    int heightMeasureSpec = View.MeasureSpec.makeMeasureSpec(0, View.MeasureSpec.UNSPECIFIED);
    view.measure(widthMeasureSpec, heightMeasureSpec);
    return view.getMeasuredHeight();
  }
  
  public int measureViewWidth(View view){
    int widthMeasureSpec = View.MeasureSpec.makeMeasureSpec((int) screenWidth, View.MeasureSpec.AT_MOST);
    int heightMeasureSpec = View.MeasureSpec.makeMeasureSpec(0, View.MeasureSpec.UNSPECIFIED);
    view.measure(widthMeasureSpec, heightMeasureSpec);
    return view.getMeasuredWidth();
  }

  
  @Override
  public void onSaveInstanceState(Bundle savedInstanceState) {
    System.out.println("onSaveInstanceState");
    super.onSaveInstanceState(savedInstanceState);
  }

  public void onRestoreInstanceState(Bundle savedInstanceState) {
    System.out.println("onRestoreInstanceState");
    super.onRestoreInstanceState(savedInstanceState);
  }
  
  
  @Override 
  public void onResume(){
    Log.d("Framework", "onResume");
    super.onResume();
    nativeOnResume(System.currentTimeMillis() / 1000.0, appId);
  }
  @Override 
  public void onPause(){
    Log.d("Framework", "onPause");
    super.onPause();
    nativeOnPause(System.currentTimeMillis() / 1000.0, appId);
  }
  @Override 
  public void onStop(){
    Log.d("Framework", "onStop");
    super.onStop();
    nativeOnStop(System.currentTimeMillis() / 1000.0, appId);
  }
  @Override 
  public void onStart(){
    Log.d("Framework", "onStart");
    super.onStart();
    nativeOnStart(System.currentTimeMillis() / 1000.0, appId);
  }
  @Override
  public void onLowMemory(){
    Log.d("Framework", "Low Memory Detected");
    super.onLowMemory();
    memoryWarning(System.currentTimeMillis() / 1000.0, appId);
  }

  @Override
  public void onDestroy() {
    System.out.println("onDestroy called");

    // It's important to destroy native before the activity, since
    // native stuff may wish to use Framework functionality in their
    // destructors

    nativeOnDestroy(System.currentTimeMillis() / 1000.0, appId);
    if (purchaseHelper != null) {
      try {
	purchaseHelper.dispose();
      } catch (IabAsyncInProgressException e) {
	e.printStackTrace();
	System.out.println("Error in disposing purchaseHelper with message: " + e.getMessage());
      }
    }
    purchaseHelper = null;
    super.onDestroy();
    System.exit(0);
  }

  public IabHelper getPurchaseHelper() {
    return purchaseHelper;
  }

  //Load JNI. Framework references to make file.
  static {
    System.out.println("Loading native library");
    System.loadLibrary("framework");
    System.out.println("native library loaded");
  }
  
  public static void handleNativeException(Throwable error){
    System.out.println("handling error");
    error.printStackTrace();
    System.out.println("error cause: " + error.getCause());
  }
  public Charset getCharset(){
    return utf8_charset;
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
  public void setValue(String v) {
    System.out.println("FrameWork couldn't handle addOption"); 
    }

  @Override
  public void setValue(int v) { 
    System.out.println("FrameWork couldn't handle addOption");
    }
  @Override
  public void setViewEnabled(Boolean enabled) {
    System.out.println("FrameWork couldn't handle addOption");
  }
  @Override
  public void setStyle(String key, String value) { }

  @Override
  public void setError(boolean hasError, String errorText) { }
  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub
    
  }
  @Override
  public void addData(int rowNumber, int columnNumber, String text) {
    // TODO Auto-generated method stub
    
  }
  
  @Override
  public void setViewVisibility(boolean visibility) {

  }
}
