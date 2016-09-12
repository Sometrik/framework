package com.sometrik.framework;

import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.concurrent.TimeUnit;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.net.Uri;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.os.Message;
import android.text.format.Time;

public class MyGLSurfaceView extends GLSurfaceView {

  public Settings settings;
  private Context context;
  public Handler sHandler;
  private MyGLRenderer renderer;
  FrameWork frame;
  AssetManager assetManager;
  Bitmap nativeCanvasBitmap;
  Bitmap image1;
  Paint mPaint;
  Path mPath;
  SharedPreferences prefs;
  SharedPreferences.Editor editor;
  
  public MyGLSurfaceView(Context context, MyGLRenderer renderer) {
    super(context);
    this.context = context;
    this.renderer = renderer;
    frame = (FrameWork) context;
    
    prefs = frame.getSharedPreferences("com.example.framework", Context.MODE_PRIVATE);
    editor = prefs.edit();

    assetManager = context.getAssets();
    System.out.println("assetmanager: " + String.valueOf(assetManager));

    // FormatDateDebug
//    formatDateTime(Calendar.getInstance().getTimeInMillis());

    // Create an OpenGL ES 2.0 context
    setEGLContextClientVersion(2);

    // Set the Renderer for drawing on the GLSurfaceView
    setRenderer(renderer);
    setRenderMode(RENDERMODE_CONTINUOUSLY);

    // Piirtoa jni debuggia varten
    mPaint = new Paint();
    mPaint.setAntiAlias(true);
    mPaint.setColor(Color.BLUE);
    mPaint.setStyle(Paint.Style.STROKE);
    mPaint.setStrokeJoin(Paint.Join.ROUND);
    mPaint.setStrokeWidth(3f);
    mPaint.setTextSize(40f);
    
    sHandler = new Handler() {
      @Override
      public void handleMessage(Message msg) {

	System.out.println("Surface message received: " + String.valueOf(msg));
	int mode = 0;
	float x = 0;
	float y = 0;
	int fingerIndex = 0;
	long time = 0;
	OptionsItem[] optionsList = null;

	switch (msg.what) {
	// 1- TouchEvent
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
	// 22 - Menu Button Pressed jni return
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

	// Launch Browser
	case 3:
	  System.out.println("handler case 3");
	  frame.mainHandler.sendMessage(Message.obtain(null, 3, msg.obj));
	  break;
	// Post notification
	case 4: 
	  System.out.println("handler case 4");
	  frame.mainHandler.sendMessage(Message.obtain(null, 4, msg.obj));
	  break;
	// Show Message Dialog
	case 5:
	  System.out.println("handler case 5");
	  frame.mainHandler.sendMessage(Message.obtain(null, 5, msg.obj));
	  break;
	}

      }

    };



    sHandler.postDelayed(onDrawRunnable, 500);
    sHandler.postDelayed(onUpdateRunnable, 100);
    System.out.println("SurfaceCreated end");

  }
  
  Runnable onUpdateRunnable = new Runnable() {
    @Override
    public void run() {
      onUpdate((double)System.currentTimeMillis());
      sHandler.postDelayed(onUpdateRunnable, 100);
    }
  };


  Runnable onDrawRunnable = new Runnable() {
    @Override
    public void run() {

//       draw(canvas);
       requestRender();
       invalidate();
      // onInit(assetManager);
      // if (update()){
      // requestRender();
      // System.out.println("Update True");
      // }else{
      // System.out.println("Update False");
      // }

      sHandler.postDelayed(onDrawRunnable, 50);
    }
  };

  protected void onSizeChanged(int w, int h, int oldw, int oldh) {
    super.onSizeChanged(w, h, oldw, oldh);

    System.out.println("MyView onsizechanged called");
    setWillNotDraw(false);
  }

  @Override
  protected void onDraw(Canvas canvas) {
    super.onDraw(canvas);

    //debug for androidplatform
    if (nativeCanvasBitmap != null) {
      System.out.println("nativeCanvas being drawn " + nativeCanvasBitmap);
      canvas.drawBitmap(nativeCanvasBitmap, 0, 0, mPaint);
    }

  }

  private void createOptionsDialog(final int[] idArray, String[] names) {

    AlertDialog.Builder builder = new AlertDialog.Builder(context);
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

    settings = new Settings(context);
    // Settings täytyy tehdä uusiksi tässä, jotta lista ei pysy samana
    settingsCreator(settings, id);

    // Animaatio kokeiluja

    // FragmentTransaction ft =
    // frame.getFragmentManager().beginTransaction();
    // ft.setCustomAnimations(R.anim.fade_in, R.anim.fade_in);
    //
    // ft.replace(android.R.id.content, settings).addToBackStack("main")
    // .commit();

    frame.getFragmentManager().beginTransaction().replace(android.R.id.content, settings).addToBackStack("main").commit();

  }
  
  //Creates ActionSheet. Called from JNI message
  public static void createOptionsFromJNI(MyGLSurfaceView view, int message, int[] idArray, String[] nameArray) {
      System.out.println("yayyy" + view);

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

  //DEBUG. Replaces current onDraw canvas with canvas from JNI
  public void setNativeCanvas(Bitmap bitmap) {
    System.out.println("native canvas bitmap given to framework: " + bitmap);
    nativeCanvasBitmap = bitmap;
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

  public String formatDateTime(long time) {

    System.out.println("formatDateTime: " + time);

    SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd-HH:mm:ss");

    Calendar c = Calendar.getInstance();
    c.setTimeInMillis(time);
    Date date = null;
    try {
      date = format.parse(format.format(c.getTime()));
    } catch (java.text.ParseException e) {

      e.printStackTrace();
    }

    java.text.DateFormat dateFormat = android.text.format.DateFormat.getDateFormat(context);
    java.text.DateFormat timeFormat = android.text.format.DateFormat.getTimeFormat(context);

//    System.out.println("unformat: " + date);
//    System.out.println("format: " + dateFormat.format(date) + " " + timeFormat.format(date));

    return dateFormat.format(date) + timeFormat.format(date);
  }
  
  public static double getTime(){
    Time t = new Time();
    t.setToNow();
//    System.out.println("time time time = " + t);
//    System.out.println("time time time = " + t.toMillis(false));
//    System.out.println("time time time = " + (double)t.toMillis(false));
    double timeSeconds = (double)t.toMillis(false) / 1000.0;
    return timeSeconds;
  }

  private String formatDate(long time) {

    // 13.10
    // formatDateTime -- FormatTimeStamp
    // formatDate Palauttaa lokaalin mukaisen pvm ja kellonajat
    // Ottaa ajan unix aikana, eli sekuntteina (laskettuna 1.1.1970 GMT
    // ajasta lähtien)
    System.out.println("formatDateTime: " + time);

    SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd");

    Calendar c = Calendar.getInstance();
    c.setTimeInMillis(time);
    Date date = null;
    try {
      date = format.parse(format.format(c.getTime()));
    } catch (java.text.ParseException e) {

      e.printStackTrace();
    }

    java.text.DateFormat dateFormat = android.text.format.DateFormat.getDateFormat(context);

    System.out.println("format: " + dateFormat.format(date));

    return dateFormat.format(date);

  }
  
  //adds key and value to preferences. (Small database)
  public void addToPrefs(String key, String value){
    System.out.println("adding to prefs... key: " + key + " value: " + value);
    editor.putString(key, value);
    editor.apply();
  }
  
  //gets value from preferences
  public String getFromPrefs(String key){
    String value = prefs.getString(key, "");
    System.out.println("getting from prefs... key: " + key + " value: " + value);
    return value;
  }
  
  //returns database path
  public String getDBPath(String dbName){
    System.out.println("getting DBPath _ db: " + dbName + " Path: " + String.valueOf(frame.getDatabasePath(dbName)));
    return String.valueOf(frame.getDatabasePath(dbName));
  }
  
  //returns android resource file path
  public String getResourcePath(String fileName){
    System.out.println("getting fileName path _ file name: " + fileName + " Path: " + "android.resource://com.sometrik.framework/test/" + fileName);
      Uri resourcePath = Uri.parse("android.resource://com.sometrik.framework/test/" + fileName);
      return resourcePath.toString();
  }

  // Ehkä myös formatValue, joka ottaa desimaaliluvun ja esittää sen joko
  // desimaalipisteellä tai pilkulla.
  private String formatValue(double value) {
    NumberFormat formatter = new DecimalFormat("#0.00");
    return formatter.format(value);
  }


  public native void createSound();

  public native void settingsCreator(Settings settings, int id);

  public native void menuPressed();

  public native void nativeOnDraw();

  public native boolean onUpdate(double timestamp);

  public native void touchEvent(int mode, int fingerIndex, long time, float x, float y);

  public native void onTouchesBegin(int fingerIndex);

  public native void onTouchesEnded(int fingerIndex);

  public native void onTouchesMoved(int fingerIndex);

}
