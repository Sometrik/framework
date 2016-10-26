package com.sometrik.framework;

import java.util.ArrayList;

import com.example.bombdefender.R;
import com.sometrik.framework.Settings.MyPreferenceFragment;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.PointF;
import android.media.SoundPool;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.preference.PreferenceManager;
import android.text.InputType;
import android.util.DisplayMetrics;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.Window;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TableRow;
import android.widget.TextView;

public class FrameWork extends Activity {

  // BUG ontouchesevent UP_action tulee my�h�ss�, kun moveja spammataan //ehk�

  // action Sheet n�kym� tiettyyn kohtaan
  // Setting objektit (ESActionSheet) C - puolella, muista, ett� menee my�s ios

  MyGLSurfaceView mGLView;
  RelativeLayout mainView;
  SharedPreferences prefs;
  SharedPreferences.Editor editor;

  private static final int RESULT_SETTINGS = 1;

  Settings settings;

  float screenHeight;
  float screenWidth;
  public Handler mainHandler;
  Intent dialogIntent;
  Bitmap picture;
  AlertDialog.Builder builder;
  AlertDialog alert;
  float windowYcoords;
  ArrayList<FormView> viewList;

  private MyGLRenderer renderer;

  public native void NativeOnTouch();

  public native int GetInt(float x, float y);

  public native String getText();

  public native void okPressed(String text);
  
  public native void settingsCreator(Settings settings, int id);

  public native void menuPressed();
  
  public native void touchEvent(int mode, int fingerIndex, long time, float x, float y);

  public native void onTouchesBegin(int fingerIndex);

  public native void onTouchesEnded(int fingerIndex);

  public native void onTouchesMoved(int fingerIndex);


  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);

    // You can disable status bar with this
    this.requestWindowFeature(Window.FEATURE_NO_TITLE);
    

    // Init for screen settings
    getDisplayMetrics();

    // Set up classes
    settings = new Settings(this);
    renderer = new MyGLRenderer(this, screenWidth, screenHeight);
    mGLView = new MyGLSurfaceView(this, renderer);

    // Get preferences (simple key-value database)
    prefs = this.getSharedPreferences("com.example.Work", Context.MODE_PRIVATE);
    editor = prefs.edit();

    // Create listener for screen touches and make MyGlSurfaceView the active
    // view
    mGLView.setOnTouchListener(new MyOnTouchListener(this));
    mGLView.setWillNotDraw(false);
    setContentView(mGLView);
    
    int[] coords = new int[2];
    mGLView.getLocationInWindow(coords);
    windowYcoords = coords[0];
    
    // create message handler for framework. Messages come from MyGLSurfaceView
    mainHandler = new Handler() {
      @Override
      public void handleMessage(Message msg) {

	System.out.println("main message received: " + String.valueOf(msg.what));

	System.out.println("Surface message received: " + String.valueOf(msg));
	int mode = 0;
	float x = 0;
	float y = 0;
	int fingerIndex = 0;
	long time = 0;
	OptionsItem[] optionsList = null;

	switch (msg.what) {
	// Touchevents
	case 1:

	  int[] intArray = (int[]) msg.obj;
	  mode = intArray[0];
	  fingerIndex = intArray[1];
	  time = intArray[2];
	  x = (float) intArray[3];
	  y = (float) intArray[4];
	  System.out.println("MESSAGE RECEIVED " + String.valueOf(mode) + " " + String.valueOf(fingerIndex) + " " + String.valueOf(x) + " " + String.valueOf(y));

	  touchEvent(mode, fingerIndex, time, x, y);
	  System.out.println("shandler case 1");

	  break;
	// 2- Menu Button Pressed
	case 2:
	  System.out.println("shandler case 2");
	  menuPressed();
	  System.out.println("shandler case 2");
	  break;
	case 22:
	  System.out.println("shandler case 22");
	  optionsList = (OptionsItem[]) msg.obj;
	  String[] names = new String[optionsList.length];
	  int[] idArray = new int[optionsList.length];

	  for (int i = 0; i < optionsList.length; i++) {
	    names[i] = optionsList[i].getName();
	    idArray[i] = optionsList[i].getId();
	  }
	  System.out.println("MenuPressed() called in jni");

	  createOptionsDialog(idArray, names);
	  break;
	// Launch browser
	case 3:
	  launchBrowser((String) msg.obj);
	  break;
	// Create notification
	case 4:
	  String[] notificationArray = (String[]) msg.obj;
	  createNotification(notificationArray[0], notificationArray[1]);
	  break;
	// show message dialog
	case 5:
	  String[] dialogArray = (String[]) msg.obj;
	  showMessageDialog(dialogArray[0], dialogArray[1]);
	  break;
	// Show OpenGLView
	case 6:
	  setContentView(mGLView);
	  break;
	  // Show formView
	case 7:
	  showFormView((int)msg.obj);
	  break;
	  // create formView
	case 8:
	  createFormView((int)msg.obj);
	  break;
	  // FormView button was clicked
	case 9:
	  int buttonId = (int)msg.obj;
	  //Call native function for this here
	}
      }
    };
  }

  // Get screen settings
  public DisplayMetrics getDisplayMetrics() {
    DisplayMetrics displaymetrics = new DisplayMetrics();
    getWindowManager().getDefaultDisplay().getMetrics(displaymetrics);
    screenHeight = displaymetrics.heightPixels;
    screenWidth = displaymetrics.widthPixels;
    return displaymetrics;
  }

  public MyGLSurfaceView getSurfaceView() {
    return mGLView;
  }

  public void launchBrowser(String url) {
    Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
    startActivity(browserIntent);
  }
  
  private void createFormView(int id){
    viewList.add(new FormView(id, this));
  }
  
  private void showFormView(int id){
    for (FormView view : viewList){
      if (view.getViewId() == id){
	view.showView();
      }
    }
  }

  // Lis�� kuvan antaminen // Aika // ��ni
  public void createNotification(String title, String text) {

    System.out.println("Creating notification");

    Intent intent = new Intent(this, FrameWork.class);
    PendingIntent pIntent = PendingIntent.getActivity(this, (int) System.currentTimeMillis(), intent, 0);

    Notification.Builder builder = new Notification.Builder(this);

    builder.setContentTitle(title);
    builder.setContentText(text);
    builder.setSmallIcon(R.drawable.picture);
    builder.setContentIntent(pIntent);
    builder.setAutoCancel(true);

    Notification notif = builder.build();
    NotificationManager notificationManager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
    notificationManager.notify(0, notif);

  }

  // Create dialog with user text input
  public void createInputDialog(String title, String message) {

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
  public void showMessageDialog(String title, String message) {

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
    setContentView(R.layout.activity_main);
    System.out.println("showSettings called");
    SharedPreferences sharedPrefs = PreferenceManager.getDefaultSharedPreferences(this);
    StringBuilder builder = new StringBuilder();
    builder.append("\n Username: " + sharedPrefs.getString("prefUsername", "NULL"));
    builder.append("\n Send report:" + sharedPrefs.getBoolean("prefSendReport", false));
    builder.append("\n Sync Frequency: " + sharedPrefs.getString("prefSyncFrequency", "NULL"));
    TextView settingsTextView = (TextView) findViewById(R.id.textUserSettings);
    settingsTextView.setText(builder.toString());
  }

  private static PointF touchScreenStartPtArr[] = new PointF[10];

  //Screen touchevent listener. Will send information to MyGLSurfaceView messagehandler
  public class MyOnTouchListener implements OnTouchListener {

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
	
	System.out.println("Liike alkoi: " + event.getX() + " " + event.getY() + " - id: " + event.getActionIndex() + " time: " + System.currentTimeMillis());
	intArray = new int[5];
	intArray[0] = 1;
	intArray[1] = event.getActionIndex();
	intArray[2] = (int) System.currentTimeMillis();
	intArray[3] = (int) event.getX();
	intArray[4] = (int) (screenHeight - event.getRawY() - windowYcoords);

	msg = Message.obtain(null, 1, intArray);
	mainHandler.sendMessage(msg);

	break;
	//Touch event of screen touch-down after the first touch
      case MotionEvent.ACTION_POINTER_DOWN:
	System.out.println("Liike alkoi: " + event.getX() + " " + event.getY() + " - id: " + event.getActionIndex());

	intArray = new int[5];
	intArray[0] = 1;
	intArray[1] = event.getActionIndex();
	intArray[2] = (int) System.currentTimeMillis();
	intArray[3] = (int) event.getX();
	intArray[4] = (int) (screenHeight - event.getRawY() - windowYcoords);
	msg = Message.obtain(null, 1, intArray);
	mainHandler.sendMessage(msg);

	break;

	//Touch event of finger moving
      case MotionEvent.ACTION_MOVE:

	int pointerCount = event.getPointerCount();
	for (int i = 0; i < pointerCount; i++) {
	  pointerIndex = i;
	  int pointerId = event.getPointerId(pointerIndex);

	  if (pointerId == 0) {

	    // System.out.println("fingerOne move: " +
	    // event.getX(pointerIndex) + event.getY(pointerIndex));

	    intArray = new int[5];
	    intArray[0] = 3;
	    intArray[1] = 0;
	    intArray[2] = (int) System.currentTimeMillis();
	    intArray[3] = (int) event.getX();
	    intArray[4] = (int) (screenHeight - event.getRawY() - windowYcoords);
	    msg = Message.obtain(null, 1, intArray);
	    mainHandler.sendMessage(msg);

	  }
	  if (pointerId == 1) {
	    // System.out.println("fingerTwo move: " +
	    // event.getX(pointerIndex) + event.getY(pointerIndex));

	    intArray = new int[5];
	    intArray[0] = 3;
	    intArray[1] = 1;
	    intArray[2] = (int) System.currentTimeMillis();
	    intArray[3] = (int) event.getX();
	    intArray[4] = (int) (screenHeight - event.getRawY() - windowYcoords);
	    msg = Message.obtain(null, 1, intArray);
	    mainHandler.sendMessage(msg);
	  }
	  if (pointerId == 2) {
	    // System.out.println("fingerThree move: " +
	    // event.getX(pointerIndex) +
	    // event.getY(pointerIndex));'

	    intArray = new int[5];
	    intArray[0] = 3;
	    intArray[1] = 2;
	    intArray[2] = (int) System.currentTimeMillis();
	    intArray[3] = (int) event.getX();
	    intArray[4] = (int) (screenHeight - event.getRawY() - windowYcoords);
	    msg = Message.obtain(null, 1, intArray);
	    mainHandler.sendMessage(msg);
	  }
	  if (pointerId == 3) {
	    // System.out.println("fingerFour move: " +
	    // event.getX(pointerIndex) + event.getY(pointerIndex));

	    intArray = new int[5];
	    intArray[0] = 3;
	    intArray[1] = 3;
	    intArray[2] = (int) System.currentTimeMillis();
	    intArray[3] = (int) event.getX();
	    intArray[4] = (int) (screenHeight - event.getRawY() - windowYcoords);
	    msg = Message.obtain(null, 1, intArray);
	    mainHandler.sendMessage(msg);
	  }
	  if (pointerId == 4) {
	    // System.out.println("fingerFive move: " +
	    // event.getX(pointerIndex) + event.getY(pointerIndex));

	    intArray = new int[5];
	    intArray[0] = 3;
	    intArray[1] = 4;
	    intArray[2] = (int) System.currentTimeMillis();
	    intArray[3] = (int) event.getX();
	    intArray[4] = (int) (screenHeight - event.getRawY() - windowYcoords);
	    msg = Message.obtain(null, 1, intArray);
	    mainHandler.sendMessage(msg);
	  }

	}
	// System.out.println("Liikett�: " + event.getX() + " " +
	// event.getY() + " - id: " + event.getActionIndex());

	break;

	//touch event of first finger being removed from the screen
      case MotionEvent.ACTION_UP:

	intArray = new int[5];
	intArray[0] = 2;
	intArray[1] = event.getActionIndex();
	intArray[2] = (int) System.currentTimeMillis();
	intArray[3] = (int) event.getX();
	intArray[4] = (int) (screenHeight - event.getRawY() - windowYcoords);
	msg = Message.obtain(null, 1, intArray);
	mainHandler.sendMessage(msg);
	break;

	//touch event of fingers other than the first leaving the screen
      case MotionEvent.ACTION_POINTER_UP:

	intArray = new int[5];
	intArray[0] = 2;
	intArray[1] = event.getActionIndex();
	intArray[2] = (int) System.currentTimeMillis();
	intArray[3] = (int) event.getX();
	intArray[4] = (int) (screenHeight - event.getRawY() - windowYcoords);
	msg = Message.obtain(null, 1, intArray);
	mainHandler.sendMessage(msg);
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
    // Settings t�ytyy tehd� uusiksi t�ss�, jotta lista ei pysy samana
    settingsCreator(settings, id);
    

    getFragmentManager().beginTransaction().replace(android.R.id.content, settings).addToBackStack("main").commit();
  }

  //Listener for built in menu options. Propably removable
  @Override
  public boolean onOptionsItemSelected(MenuItem item) {
    switch (item.getItemId()) {

    case R.id.menu_settings:

      getFragmentManager().beginTransaction().replace(android.R.id.content, new MyPreferenceFragment()).commit();

      break;

    case 1:
      startActivity(new Intent(this, Settings.class));
      break;

    }

    return true;
  }
  
  
  //Creates ActionSheet. Called from JNI message
  public static void createOptionsFromJNI(MyGLSurfaceView view, int message, int[] idArray, String[] nameArray) {

    OptionsItem[] optionsList = new OptionsItem[nameArray.length];
    for (int i = 0; i < optionsList.length; i++) {
      System.out.println("createOptionsFromJni " + "i: " + i + "  intarray length: " + nameArray.length);
      optionsList[i] = new OptionsItem(idArray[i], nameArray[i]);
    }

    System.out.println("messageposter array: " + view);
    System.out.println("messageposter array: " + nameArray);
    Message msg = Message.obtain(null, message, optionsList);
    view.sHandler.sendMessage(msg);

  }

  // returns database path
  public String getDBPath(String dbName) {
    System.out.println("getting DBPath _ db: " + dbName + " Path: " + String.valueOf(getDatabasePath(dbName)));
    return String.valueOf(getDatabasePath(dbName));
  }

  //Creates message that is sent to MyGLSurface handler. Called from JNI
  public static void LeaveMessageToSurface(MyGLSurfaceView view, int messageCode) {
    view.sHandler.sendEmptyMessage(messageCode);
  }
  public static void LeaveMessageToSurface(MyGLSurfaceView view, int messageCode, String text) {
    Message msg = Message.obtain(null, messageCode, text);
    view.sHandler.sendMessage(msg);
  }
  public static void LeaveMessageToSurface(MyGLSurfaceView view, int messageCode, String title, String text) {
    String[] stringArray = {title, text};
    Message msg = Message.obtain(null, messageCode, stringArray);
    view.sHandler.sendMessage(msg);
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

  //Load JNI. Framework references to make file.
  static {
    System.loadLibrary("framework");
  }

}