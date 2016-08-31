package com.sometrik.framework;

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
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.EditText;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class FrameWork extends Activity {

  // BUG ontouchesevent UP_action tulee myöhässä, kun moveja spammataan //ehkä

  // action Sheet näkymä tiettyyn kohtaan
  // Setting objektit (ESActionSheet) C - puolella, muista, että menee myös ios

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

  private MyGLRenderer renderer;

  public native void NativeOnTouch();

  public native int GetInt(float x, float y);

  public native String getText();

  public native void okPressed(String text);

  public native void nativeOnDraw();

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);

    // create message handler for framework. Messages come from MyGLSurfaceView
    mainHandler = new Handler() {
      @Override
      public void handleMessage(Message msg) {

	System.out.println("main message received: " + String.valueOf(msg.what));

	switch (msg.what) {
	case 1:
	  // createMessageDialog("1", "1");
	  break;
	case 2:
	  // createMessageDialog("2", "2");
	  break;
	case 3:
	  launchBrowser((String) msg.obj);
	  break;
	case 4:
	  String[] notificationArray = (String[]) msg.obj;
	  createNotification(notificationArray[0], notificationArray[1]);
	  break;
	// show message dialog
	case 5:
	  String[] dialogArray = (String[]) msg.obj;
	  showMessageDialog(dialogArray[0], dialogArray[1]);
	  break;
	}

      }
    };

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

  // Lisää kuvan antaminen // Aika // Ääni
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
	intArray[4] = (int) (screenHeight - event.getY());

	msg = Message.obtain(null, 1, intArray);
	mGLView.sHandler.sendMessage(msg);

	break;
	//Touch event of screen touch-down after the first touch
      case MotionEvent.ACTION_POINTER_DOWN:
	System.out.println("Liike alkoi: " + event.getX() + " " + event.getY() + " - id: " + event.getActionIndex());

	intArray = new int[5];
	intArray[0] = 1;
	intArray[1] = event.getActionIndex();
	intArray[2] = (int) System.currentTimeMillis();
	intArray[3] = (int) event.getX();
	intArray[4] = (int) (screenHeight - event.getY());
	msg = Message.obtain(null, 1, intArray);
	mGLView.sHandler.sendMessage(msg);
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
	    intArray[4] = (int) (screenHeight - event.getY());
	    msg = Message.obtain(null, 1, intArray);
	    mGLView.sHandler.sendMessage(msg);
	    // mGLView.sHandler.sendMessage(msg);

	  }
	  if (pointerId == 1) {
	    // System.out.println("fingerTwo move: " +
	    // event.getX(pointerIndex) + event.getY(pointerIndex));

	    intArray = new int[5];
	    intArray[0] = 3;
	    intArray[1] = 1;
	    intArray[2] = (int) System.currentTimeMillis();
	    intArray[3] = (int) event.getX();
	    intArray[4] = (int) (screenHeight - event.getY());
	    msg = Message.obtain(null, 1, intArray);
	    mGLView.sHandler.sendMessage(msg);
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
	    intArray[4] = (int) (screenHeight - event.getY());
	    msg = Message.obtain(null, 1, intArray);
	    mGLView.sHandler.sendMessage(msg);
	  }
	  if (pointerId == 3) {
	    // System.out.println("fingerFour move: " +
	    // event.getX(pointerIndex) + event.getY(pointerIndex));

	    intArray = new int[5];
	    intArray[0] = 3;
	    intArray[1] = 3;
	    intArray[2] = (int) System.currentTimeMillis();
	    intArray[3] = (int) event.getX();
	    intArray[4] = (int) (screenHeight - event.getY());
	    msg = Message.obtain(null, 1, intArray);
	    mGLView.sHandler.sendMessage(msg);
	  }
	  if (pointerId == 4) {
	    // System.out.println("fingerFive move: " +
	    // event.getX(pointerIndex) + event.getY(pointerIndex));

	    intArray = new int[5];
	    intArray[0] = 3;
	    intArray[1] = 4;
	    intArray[2] = (int) System.currentTimeMillis();
	    intArray[3] = (int) event.getX();
	    intArray[4] = (int) (screenHeight - event.getY());
	    msg = Message.obtain(null, 1, intArray);
	    mGLView.sHandler.sendMessage(msg);
	  }

	}
	// System.out.println("Liikettä: " + event.getX() + " " +
	// event.getY() + " - id: " + event.getActionIndex());

	break;

	//touch event of first finger being removed from the screen
      case MotionEvent.ACTION_UP:

	intArray = new int[5];
	intArray[0] = 2;
	intArray[1] = event.getActionIndex();
	intArray[2] = (int) System.currentTimeMillis();
	intArray[3] = (int) event.getX();
	intArray[4] = (int) (screenHeight - event.getY());
	msg = Message.obtain(null, 1, intArray);
	mGLView.sHandler.sendMessage(msg);

	// print (debug)
	// System.out.println( "Liike loppui: " + event.getX() + " " +
	// event.getY() + " - id: " +
	// event.getPointerId(event.getActionIndex()));
	break;

	//touch event of fingers other than the first leaving the screen
      case MotionEvent.ACTION_POINTER_UP:

	intArray = new int[5];
	intArray[0] = 2;
	intArray[1] = event.getActionIndex();
	intArray[2] = (int) System.currentTimeMillis();
	intArray[3] = (int) event.getX();
	intArray[4] = (int) (screenHeight - event.getY());
	msg = Message.obtain(null, 1, intArray);
	mGLView.sHandler.sendMessage(msg);

	// print (debug)
	// System.out.println("Liike loppui: " + event.getX() + " " +
	// event.getY() + " - id: " +
	// event.getPointerId(event.getActionIndex()));
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