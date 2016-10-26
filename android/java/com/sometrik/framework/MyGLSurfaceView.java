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
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.media.SoundPool;
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
    
    // Create an OpenGL ES 2.0 context
    setEGLContextClientVersion(2);

    // Set the Renderer for drawing on the GLSurfaceView
    setRenderer(renderer);
    setRenderMode(RENDERMODE_CONTINUOUSLY);

    
    System.out.println("SurfaceCreated end");

  }

  protected void onSizeChanged(int w, int h, int oldw, int oldh) {
    super.onSizeChanged(w, h, oldw, oldh);

    System.out.println("MyView onsizechanged called");
    setWillNotDraw(false);
  }

  @Override
  protected void onDraw(Canvas canvas) {
    super.onDraw(canvas);
  }


}
