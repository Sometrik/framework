package com.sometrik.framework;

import javax.microedition.khronos.opengles.GL10;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.content.res.AssetManager;
import android.opengl.EGLConfig;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.DisplayMetrics;

public class MyGLRenderer implements GLSurfaceView.Renderer {

  private MyGLRenderer renderer;
  public Handler mHandler;
  AssetManager assetManager;
  MyGLSurfaceView GLView;
  FrameWork frame;
  Context context;
  float xSize;
  float ySize;
  long endTime;
  long startTime;

  public native void nativeOnDraw();

  public native void onInit(AssetManager assetManager, MyGLSurfaceView view, float xSize, float ySize, float displayScale, Boolean hasEs3);

  public native void Draw();

  public native void onResize(float xSize, float ySize);

  public native void onTouchesBegin(int fingerIndex);

  public native void onTouchesEnded(int fingerIndex);

  public native void onTouchesMoved(int fingerIndex);

  public MyGLRenderer(Context context, float x, float y) {

    frame = (FrameWork) context;
    startTime = System.currentTimeMillis();
    assetManager = context.getAssets();
    this.xSize = x;
    this.ySize = y;
  }

  public void onDrawFrame(GL10 unused) {
      // Calls onDraw in AndroidPlatform
      nativeOnDraw();
  }

  public void onSurfaceChanged(GL10 unused, int width, int height) {
    System.out.println("(Renderer) onSurfaceChanged called");
    GLES20.glViewport(0, 0, width, height);

    onResize(width, height);
  }

  @Override
  public void onSurfaceCreated(GL10 gl, javax.microedition.khronos.egl.EGLConfig config) {
    System.out.println("(Renderer)onSurfaceCreated(override) called");

    renderer = this;
    System.out.println("surface check: " + frame.getSurfaceView());

    DisplayMetrics displayMetrics = frame.getDisplayMetrics();
    System.out.println("Display scale: " + displayMetrics.scaledDensity);
    final ActivityManager activityManager = (ActivityManager) frame.getSystemService(Context.ACTIVITY_SERVICE);
    final ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
    Boolean hasEs3;
    if (configurationInfo.reqGlEsVersion >= 0x30000){
      hasEs3 = true;
    } else if (configurationInfo.reqGlEsVersion >= 0x20000) {
      hasEs3 = false;
    } else {
      hasEs3 = false;
      System.out.println("openGLES 2 isn't supported");
    }
    // Calls onInit in AndroidPlatform
    onInit(assetManager, frame.getSurfaceView(), xSize, ySize, displayMetrics.scaledDensity, hasEs3);

    xSize = displayMetrics.widthPixels / displayMetrics.scaledDensity;
    ySize = displayMetrics.heightPixels / displayMetrics.scaledDensity;

  }

}
