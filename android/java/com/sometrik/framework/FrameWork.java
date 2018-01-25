package com.sometrik.framework;

import java.io.ByteArrayOutputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;
import java.util.Map.Entry;

import com.android.trivialdrivesample.util.IabException;
import com.android.trivialdrivesample.util.IabHelper;
import com.android.trivialdrivesample.util.IabHelper.IabAsyncInProgressException;
import com.android.trivialdrivesample.util.Inventory;
import com.sometrik.framework.FWActionBar.ActionBarItem;

import android.accounts.Account;
import android.accounts.AccountManager;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ApplicationInfo;
import android.content.pm.ConfigurationInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.provider.MediaStore;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.TranslateAnimation;
import android.view.inputmethod.InputMethodManager;
import android.widget.FrameLayout;

public class FrameWork extends Activity {

  private SharedPreferences prefs;
  private SharedPreferences.Editor editor;
  private FrameWork frameWork;
  private double updateTimer = 0;
  private IabHelper purchaseHelper;
  private static final int RESULT_SETTINGS = 1;
  private static final int RESULT_CHOOSE_FROM_GALLERY = 2;
  private int galleryChooseElementId = 0;
  private Inventory inventory;
  private DisplayMetrics displayMetrics;
  private View currentlyShowingView;
  private Charset utf8_charset;
  private FWDrawerLayout drawerLayout;
  private int currentDrawerViewId;
  
  private boolean drawMode = false;
  
  public FWActionBar actionBar = null;
  
  private Locale defaultLocale;
  private float screenHeight;
  private float screenWidth;
  public Handler mainHandler;
  private Intent dialogIntent;
  private AlertDialog.Builder builder;
  private AlertDialog alert;
  private float windowYcoords;
  public HashMap<Integer, NativeCommandHandler> views = new HashMap<Integer, NativeCommandHandler>();
  private int appId = 0;
  private int currentView = 0;
  public static boolean transitionAnimation = false;
  public BitmapCache bitmapCache;
  private boolean setCurrentViewDisabled = false;

  private native void textChangedEvent(int id, byte[] textValue);
  public native void intChangedEvent(int id, int changedInt, int changedInt2);
  public native void imageUploadEvent(int id, byte[] image);
  public native void visibilityChangedEvent(int id, boolean visible);
  public native void keyPressed(int keyId, int viewId);
  public native void touchEvent(int viewId, int mode, int fingerIndex, double timestamp, float x, float y);
  public native void flushTouchEvent(double timestamp, int viewId, int mode);
  public native void onInit(AssetManager assetManager, int xSize, int ySize, float displayScale, String email, String language, String country, String version);
  public native void nativeSetSurface(Surface surface, int surfaceId, int gl_version, int width, int height);
  public native void nativeSurfaceDestroyed(int surfaceId, int gl_version);
  public native void nativeOnResume(int appId);
  public native void nativeOnPause(int appId);
  public native void nativeOnStop(int appId);
  public native void nativeOnStart(int appId);
  public native void nativeOnDestroy(int appId);
  public native void nativeAddPreference(String key, String value);
  public native void nativeScrollChanged(int viewId, int scrollPos, int scrollRem, int height);
  public native void sendURLImageRequest(int viewId, String url, int width, int height, int internalFormat);
  public native void sendImageRequest(int viewId, int width, int height, int internalFormat);
  public native void cancelImageRequest(int viewId);
  public native void showNativeDebug();
  private native void setNativeActiveView(int activeView, boolean recordHistory);
  private native void languageChanged(int appId, String language);
  private native void memoryWarning(int appId);
  public static native void onPurchaseEvent(int applicationId, String orderId, boolean newPurchase, double purchaseTime);
  public static native void onResize(float width, float height, int viewId);
  public static native void onUpdate(double timestamp, int viewId);
  public static native void timerEvent(int viewId, int timerId);
  
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
//      this.setBackground(frame.getResources().getDrawable(android.R.drawable.dialog_holo_light_frame));
//      this.setTheme(android.R.style.Theme_Holo);
      this.setTheme(android.R.style.Theme_Holo_Light);
    }
    this.setTheme(android.R.style.Theme_Holo_Light);
    super.onCreate(savedInstanceState);
    
    utf8_charset = Charset.forName("UTF-8");
    defaultLocale = Locale.getDefault();
    System.out.println("Users preferred locale: " + defaultLocale.getCountry() + " Language: " + defaultLocale.getLanguage());
    

    prefs = getSharedPreferences("app", Context.MODE_PRIVATE);
    editor = prefs.edit();
    
    // ActionBar is hidden by default with this
    getActionBar().hide();
    getActionBar().setDisplayHomeAsUpEnabled(true);
//    getActionBar().setHomeButtonEnabled(true);
    
    // Init NavigationBar
    drawerLayout = new FWDrawerLayout(this);
    currentDrawerViewId = 0;
    
    // Init for screen settings
    setupDisplayMetrics();

    bitmapCache = new BitmapCache(getAssets(), displayMetrics.scaledDensity);

    final FrameWork framework = this;
    
    //this mainHandler is used to process messages that come from native application.
    mainHandler = new Handler() {

      public void handleMessage(Message msg) {
	if (msg.what == 1) {
	  try {
	    HashMap<Integer, NativeCommandHandler> changedViews = null;
	    NativeCommandTransaction transaction = (NativeCommandTransaction) msg.obj;
	    for (NativeCommand command : transaction.getCommands()) {
	      NativeCommandHandler view = framework.views.get(command.getInternalId()); 
	      command.apply(view);
	      if (command.getCommand() == NativeCommand.CommandType.SET_STYLE) {
		 if (changedViews == null) changedViews = new HashMap<Integer, NativeCommandHandler>();
		 changedViews.put(command.getInternalId(), view);
	      }
	    }
	    if (changedViews != null) {
	      for (HashMap.Entry<Integer, NativeCommandHandler> entry : changedViews.entrySet()) {
		NativeCommandHandler view = entry.getValue();
		view.applyStyles();
	      }
	    }
	  } catch (Throwable t) {
	    t.printStackTrace();
	  }
	} else if (msg.what == 2) {
	  try {
	    NativeCommandHandler view = framework.views.get(msg.arg1);
	    if (view != null) {
	      view.setBitmap((Bitmap)msg.obj);
	    }
	  } catch (Throwable t) {
	    t.printStackTrace();
	  }
	}
      }

    };
    System.out.println("initing native on onCreate");
    initNative();
  }

  //PurchaseHelper is used to process in-app purchases
  public Boolean initializePurchaseHelper(String key, IabHelper.OnIabSetupFinishedListener listener) {
    // Get PurchaseHelper. Requires App public key
    purchaseHelper = new IabHelper(this, key);
    purchaseHelper.startSetup(listener);
    return true;
  }
  
  private boolean hasAccountPermission() {
    return false;
  }

  /**
   * Retrieves Google Account Email. Requires Account permissions to be added to
   * manifest and approved by user.
   * @return Email address of the first google account.
   */
  private String getUserGoogleAccountEmail() {
    if (hasAccountPermission()) {
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
      if (gmail.isEmpty()) {
	Log.d("accountFinder", "No user Google Account found");
      }
      return gmail;
    } else {
      return "";
    }
  }

/**
 * Retrives history of purchases made by the user.
 * @return Inventory object containing purchases.
 */
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

  /**
   * Sends device and asset information to native application. Should be called at the beginning of the program.
   */
  private void initNative() {
    System.out.println("Display scale: density = " + displayMetrics.scaledDensity + ", dpi = " + displayMetrics.densityDpi);
    int xSize = displayMetrics.widthPixels;
    int ySize = displayMetrics.heightPixels;
    initNativePreferences();

    String versionName = null;
    try {
      versionName = getPackageManager().getPackageInfo(getPackageName(), 0).versionName;
    } catch (NameNotFoundException e) {
      System.out.println("versionName not found");
      e.printStackTrace();
    }
    
    onInit(getAssets(), xSize, ySize, displayMetrics.scaledDensity, getUserGoogleAccountEmail(), defaultLocale.getLanguage(), defaultLocale.getCountry(), versionName);
  }
  
  /**
   * Retrieves all saved prefre
   */
  public void initNativePreferences() {
    Map<String, String> allPrefs = (Map<String, String>) prefs.getAll();
    if (allPrefs == null || allPrefs.size() == 0) {
      System.out.println("no preferences found");
    }
    for (Entry<String, String> entry : allPrefs.entrySet()) {
      System.out.println("found pref - key: " + entry.getKey() + " value: " + entry.getValue());
      nativeAddPreference(entry.getKey(), entry.getValue());
    }
  }
  
  public static String getApplicationName(Context context) {
    ApplicationInfo applicationInfo = context.getApplicationInfo();
    int stringId = applicationInfo.labelRes;
    return stringId == 0 ? applicationInfo.nonLocalizedLabel.toString() : context.getString(stringId);
  }

  /**
   * Sets FrameWork screen metric variables
   */
  public DisplayMetrics setupDisplayMetrics() {
    displayMetrics = new DisplayMetrics();
    Display display = ((WindowManager) getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
    display.getMetrics(displayMetrics);
    screenWidth = displayMetrics.widthPixels;
    screenHeight = displayMetrics.heightPixels;

    Resources resources = getResources();
    int resourceId = resources.getIdentifier( "status_bar_height", "dimen", "android" );
    int statusHeight = ( resourceId > 0 ) ? resources.getDimensionPixelSize( resourceId ) : 0;
    System.out.println("statusHeight: " + statusHeight);
    screenHeight -= statusHeight;

    return displayMetrics;
  }

  public SharedPreferences.Editor getPreferencesEditor() { return editor; }

  /**
   * Adds NativeCommandHandler to viewlist so they can be sent commands by the native application
   * @param view
   */
  public void addToViewList(NativeCommandHandler view) {
    views.put(view.getElementId(), view);
  }

  /**
   * Launches FWWebActivity to display in-app browser
   * @param url
   */
  public void launchBrowser(String url) {
    Intent intent = new Intent(this, FWWebActivity.class);
    intent.putExtra(FWWebActivity.URL_EXTRA, url);
    startActivity(intent);
  }
  
  /**
   * Removies view from viewlist
   * @param viewId id of the NativeCommandHandler
   */
  public void removeViewFromList(int viewId) {
    NativeCommandHandler view = views.get(viewId);
    if (view != null) view.deinitialize();
    views.remove(viewId);
  }
  
  /**
   * Sets android keyboard open or closed
   * @param view that is currently active
   * @param enabled if false hides keyboard
   */
  public void setSoftKeyboardShow(View view, boolean enabled) {
    InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
    if (enabled) {
      imm.showSoftInput(view, InputMethodManager.SHOW_IMPLICIT);
    } else {
      imm.hideSoftInputFromWindow(view.getApplicationWindowToken(), 0);
    }
  }
  
  /**
   * Sends value event to native application. Reduces the change of events being sent while doing transitionAnimations
   * @param elementId Id of the element the event will be sent to
   * @param value 
   * @param value2
   */
  public void sendNativeValueEvent(int elementId, int value, int value2) {
    if (!transitionAnimation) {
      System.out.println("click while not transitioning");
      intChangedEvent(elementId, value, value2);
    } else {
      System.out.println("click while transitioning");
    }
  }

  /**
   * Sends value event to native application. Reduces the change of events being sent while doing transitionAnimations
   * @param elementId Id of the element the event will be sent to
   * @param value
   */
  public void sendNativeValueEvent(int elementId, byte[] value) {
    if (!transitionAnimation) {
      textChangedEvent(elementId, value);
    }
  }

  /**
   * Displays given view. If there is a navigation drawer the view will be attached to it.
   * Sends information about the active view to native application. is able to do simple transition animations.
   * @param view
   * @param animation not used
   * @param newViewAnimationFromX sets where the animation will begin. Used to set on which side tranlate animations will come from
   */
  public void setCurrentView(final View view, Animation animation, final int newViewAnimationFromX) {
    //Prevents new view being shown while doing transitionAnimation
    if (transitionAnimation) {
      return;
    }

    //Disables transitionAnimation after the animation is complete
    Runnable runnable = new Runnable() {
      @Override
      public void run() {
	transitionAnimation = false;
      }
    };
    mainHandler.postDelayed(runnable, 300);

    transitionAnimation = true;
    currentView = view.getId();
    currentlyShowingView = view;

    //Remove views from drawerLayout so that the new view can be inserted into drawerLayout
    if (drawerLayout.getChildCount() > 0) {
      drawerLayout.removeAllViews();
    }

    //Insert new view into drawerLayout if it exists
    if (currentDrawerViewId != 0) {
      NativeCommandHandler drawerView = views.get(currentDrawerViewId);
      if (drawerView != null) {
	drawerLayout.addView(view);
	drawerLayout.addView((View) drawerView);
	setContentView(drawerLayout);
      } else {
	setContentView(view);
      }
    } else {
      setContentView(view);
    }

    TranslateAnimation q;
    q = new TranslateAnimation(newViewAnimationFromX, 0, 0, 0);
    //send info of the new active view to native application
    setNativeActiveView(view.getId(), false);
    q.setDuration(200);
    view.startAnimation(q);
  }

  /**
   * Displays given view. If there is a navigation drawer the view will be attached to it.
   * Sends information about the active view to native application.
   * @param view new view to be displayed.
   */
  public void setCurrentView(final View view) {
    if (currentView == view.getId()){
      System.out.println("view already set");
      return;
    }
      currentView = view.getId();
      currentlyShowingView = view;

    if (drawerLayout.getChildCount() > 0) {
      drawerLayout.removeAllViews();
    }

    System.out.println("currentDrawerViewId: " + currentDrawerViewId);
    if (currentDrawerViewId != 0) {
      NativeCommandHandler drawerView = views.get(currentDrawerViewId);
      if (drawerView != null) {
	drawerLayout.addView(view);
	drawerLayout.addView((View) drawerView);
	setContentView(drawerLayout);
      } else {
	System.out.println("no navigation drawer element found on id: " + currentDrawerViewId);
	setContentView(view);
      }
    } else {
      setContentView(view);
    }
     
    setNativeActiveView(view.getId(), false);
  }
  
  public int getCurrentViewId() { return currentView; }

  /**
   * Creates NativeSurface that will be set on native application
   * @param id that will be set on this native surface
   * @return
   */
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
	holder.addCallback(new Callback() {
      public void surfaceDestroyed(SurfaceHolder holder) {
	System.out.println("surfaceDestroyed");
	nativeSurfaceDestroyed(id, gl_version);
      }

      public void surfaceCreated(SurfaceHolder holder) {
	setupDisplayMetrics();
	System.out.println("surfaceCreated. Width: " + screenWidth + " height: " + screenHeight + " id: " + id);
	nativeSetSurface(holder.getSurface(), id, gl_version, (int)screenWidth, (int)screenHeight);
	}

      public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
	System.out.println("nativeSurfaceChanged. Width: " + width + " height: " + height + " id: " + id);
	onResize(width, height, id);
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
  
  /**
   * Screen touchevent listener. Will send information to MyGLSurfaceView messagehandler
   *
   */
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

      int action = event.getAction() & MotionEvent.ACTION_MASK;
      int pointerIndex = (event.getAction() & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
      int fingerId = event.getPointerId(pointerIndex);

      switch (action) {

      //Touch event of screen touch-down for the first finger
      case MotionEvent.ACTION_DOWN:
	//Touch event of screen touch-down after the first touch
      case MotionEvent.ACTION_POINTER_DOWN:
	System.out.println("Liike alkoi: " + event.getX(event.getActionIndex()) + " " + event.getY(event.getActionIndex()) + " - id: " + fingerId);
	touchEvent(viewId, 1, fingerId, System.currentTimeMillis() / 1000.0, (int) event.getX(event.getActionIndex()), (int) (event.getY(event.getActionIndex())));
	flushTouchEvent(System.currentTimeMillis() / 1000.0, viewId, 1);
	break;

	//Touch event of finger moving
      case MotionEvent.ACTION_MOVE:
	int pointerCount = event.getPointerCount();
	for (int i = 0; i < pointerCount; i++) {
	  // System.out.println("Liike: " + event.getX(i) + " " + event.getY(i) + " - id: " + i);
	  touchEvent(viewId, 2, i, System.currentTimeMillis() / 1000.0, (int) event.getX(i), (int) (event.getY(i)));
	}
	flushTouchEvent(System.currentTimeMillis() / 1000.0, viewId, 2);
	break;
      // touch event of first finger being removed from the screen
      case MotionEvent.ACTION_UP:
	// touch event of fingers other than the first leaving the screen
      case MotionEvent.ACTION_POINTER_UP:
	System.out.println("Liike loppui POINTER_UP: " + event.getX(event.getActionIndex()) + " " + event.getY(event.getActionIndex()) + " - id: " + fingerId);
	  touchEvent(viewId, 3, fingerId, System.currentTimeMillis() / 1000.0, (int) event.getX(event.getActionIndex()), (int) (event.getY(event.getActionIndex())));
  	  flushTouchEvent(System.currentTimeMillis() / 1000.0, viewId, 3);
	break;
      }
      return true;
    }
  }

  @Override
  public boolean onCreateOptionsMenu(Menu menu) {
    if (actionBar != null) {
      ArrayList<ActionBarItem> itemList = actionBar.getItemList();
      for (ActionBarItem item : itemList) {
	System.out.println("onCreateOptionsMenu add new Item " + item.id);
	MenuItem newItem = menu.add(0, item.id, 0, "item");
	newItem.setIcon(item.picture).setShowAsAction(MenuItem.SHOW_AS_ACTION_IF_ROOM);
      }
    }
    System.out.println("onCreateOptionsMenu");
    return true;
  }

  @Override
  public boolean onOptionsItemSelected(MenuItem item) {
    System.out.println("onOptionsItemSelected Item " + actionBar.getElementId() + " " + item.getItemId() + " " + item.getGroupId());
    intChangedEvent(actionBar.getElementId(), item.getItemId(), item.getItemId());
    if (item.getItemId() == 16908332){
      if (drawerLayout.isDrawerOpen(Gravity.LEFT)){
	drawerLayout.closeDrawer(Gravity.LEFT);
      } else {
	drawerLayout.openDrawer(Gravity.LEFT);
      }
    }
    return super.onOptionsItemSelected(item);
  }

  @Override
  public boolean onKeyDown(int keycode, KeyEvent e) {
    System.out.println("KeyEvent. KeyCode: " + keycode + " ViewId: " + findViewById(android.R.id.content).getRootView().getId());
//    if (e.getAction() == KeyEvent.ACTION_MULTIPLE) {
//      System.out.println("KeyEvent ACTION_MULTIPLE. KeyCode: " + keycode + " ViewId: " + findViewById(android.R.id.content).getRootView().getId());
//      return true;
//    }
    if (!transitionAnimation) {
      if (keycode == KeyEvent.KEYCODE_BACK && drawerLayout != null) {
	if (drawerLayout.isDrawerOpen(Gravity.LEFT)) {
	  drawerLayout.closeDrawer(Gravity.LEFT);
	  return true;
	}
      }
      keyPressed(e.getKeyCode(), currentView);
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
  public float getScreenHeight(){
    return screenHeight;
  }
  
  public void setAppId(int id) { this.appId = id; }
  public int getAppId() { return appId; }

  // returns database path
  public String getDBPath(String dbName) {
    System.out.println("getting DBPath _ db: " + dbName + " Path: " + String.valueOf(getDatabasePath(dbName)));
    return String.valueOf(getDatabasePath(dbName));
  }
			       
  public static void sendTransaction(FrameWork frameWork, NativeCommandTransaction commandTransaction) {
    Message msg = Message.obtain(null, 1, commandTransaction);
    frameWork.mainHandler.sendMessage(msg);
  }

  public static void sendBitmap(FrameWork frameWork, int internalId, Bitmap bitmap) {
    Message msg = Message.obtain(null, 2, internalId, 0, bitmap);
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
    
    case RESULT_CHOOSE_FROM_GALLERY:
      if (resultCode == RESULT_OK) {
	System.out.println("RESULT OK");
	 Uri selectedImg = data.getData();
	System.out.println("Selected image: " + selectedImg.toString());
	 try {
	  Bitmap bitmap = MediaStore.Images.Media.getBitmap(this.getContentResolver(), selectedImg);
	  ByteArrayOutputStream stream = new ByteArrayOutputStream();
	  bitmap.compress(Bitmap.CompressFormat.PNG, 100, stream);
	  byte[] byteArray = stream.toByteArray();
	  System.out.println("image uploaded " + galleryChooseElementId);
//	  this.nativev(galleryChooseElementId, byteArray);
	  imageUploadEvent(galleryChooseElementId, byteArray);
	} catch (FileNotFoundException e) {
	  e.printStackTrace();
	  System.out.println("error uploading image");
	} catch (IOException e) {
	  e.printStackTrace();
	  System.out.println("error uploading image");
	}
	//TODO send to native
      } else {
	System.out.println("Error receiving image from gallery");
      }
      break;
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
      languageChanged(appId, locale.getLanguage());
      defaultLocale = locale;
    }

    super.onConfigurationChanged(newConfig);
    displayMetrics = setupDisplayMetrics();
    System.out.println("onConfigChange");
    boolean isLandscape = false;
    if (newConfig.orientation == Configuration.ORIENTATION_PORTRAIT) {
      System.out.println("Orientation conf portrait");
      isLandscape = false;
      onResize(screenWidth, screenHeight, currentView);
      System.out.println("Orientation conf portrait. SWidth: " + screenWidth + " SHeight: " + screenHeight + " currentView: " + currentView);
    } else if (newConfig.orientation == Configuration.ORIENTATION_LANDSCAPE) {
      System.out.println("Orientation conf landscape");
      isLandscape = true;
      onResize(screenWidth, screenHeight, currentView);
      System.out.println("Orientation conf landscape. SWidth: " + screenWidth + " SHeight: " + screenHeight + " currentView: " + currentView);
    }
    
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
  
  /**
   * Moves application to activity where user can choose a image from the device's gallery.
   * Sets id for ImageUploadEvent to be sent to.
   * @param nativeId Id of a element where imageUploadEvent will be sent
   */
  public void selectFromGallery(int nativeId) {
    Intent intent = new Intent();  
    intent.setType("image/*");
    System.out.println("image upload putting intent Id: " + nativeId);
    intent.putExtra(MediaStore.EXTRA_OUTPUT, nativeId);
    intent.setAction(Intent.ACTION_GET_CONTENT);
    Intent returnIntent = Intent.createChooser(intent, "Choose Picture");
    galleryChooseElementId = nativeId;
    returnIntent.putExtra(MediaStore.EXTRA_OUTPUT, nativeId);
    startActivityForResult(returnIntent, RESULT_CHOOSE_FROM_GALLERY);
  }

  
  @Override
  public void onSaveInstanceState(Bundle savedInstanceState) {
    System.out.println("onSaveInstanceState");
    super.onSaveInstanceState(savedInstanceState);
  }

  @Override
  public void onRestoreInstanceState(Bundle savedInstanceState) {
    System.out.println("onRestoreInstanceState");
    super.onRestoreInstanceState(savedInstanceState);
  }
  
  
  @Override 
  public void onResume() {
    super.onResume();
    nativeOnResume(appId);
  }
  @Override 
  public void onPause() {
    super.onPause();
    nativeOnPause(appId);
  }
  @Override 
  public void onStop() {
    super.onStop();
    nativeOnStop(appId);
  }
  @Override 
  public void onStart() {
    super.onStart();
    nativeOnStart(appId);
  }
  @Override
  public void onLowMemory() {
    super.onLowMemory();
    System.out.println("FRAMEWORK MEMORY WARNING");
    memoryWarning(appId);
  }

  @Override
  public void onDestroy() {
    System.out.println("onDestroy called");

    // It's important to destroy native before the activity, since
    // native stuff may wish to use Framework functionality in their
    // destructors

    nativeOnDestroy(appId);
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

  // Load JNI. Framework references to make file.
  static {
    System.out.println("Loading native library");
    System.loadLibrary("framework");
    System.out.println("native library loaded");
  }

  public static void handleNativeException(Throwable error) {
    System.out.println("handling error");
    error.printStackTrace();
    System.out.println("error cause: " + error.getCause());
  }

  public Charset getCharset() {
    return utf8_charset;
  }
  public int getCurrentDrawerViewId() {
    return currentDrawerViewId;
  }
  public void setCurrentDrawerViewId(int currentDrawerViewId) {
    this.currentDrawerViewId = currentDrawerViewId;
  }
  public FWDrawerLayout getDrawerLayout() {
    return drawerLayout;
  }
}
