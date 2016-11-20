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
  boolean is_initialized = false;

  public native void nativeOnDraw();

  public native void Draw();

  public native void onResize(float xSize, float ySize);

  public native void onInitElement(double timestamp, int viewId);

  public native boolean onUpdate(double timestamp, int viewId);

  public MyGLRenderer(Context context, float x, float y) {

    frame = (FrameWork) context;
    startTime = System.currentTimeMillis();
    assetManager = context.getAssets();
    this.xSize = x;
    this.ySize = y;
  }

  public void onDrawFrame(GL10 unused) {
      // Calls onDraw in AndroidPlatform
    if (!is_initialized) {
      onInitElement(System.currentTimeMillis() / 1000.0, frame.currentView);
      is_initialized = true;
    }
    onUpdate((double) System.currentTimeMillis() / 1000.0, frame.currentView);
    nativeOnDraw();
  }

  public void onSurfaceChanged(GL10 unused, int width, int height) {
    System.out.println("(Renderer) onSurfaceChanged called");
//    GLES20.glViewport(0, 0, width, height);

//    onResize(width, height);
  }

  @Override
  public void onSurfaceCreated(GL10 gl, javax.microedition.khronos.egl.EGLConfig config) {
    System.out.println("(Renderer)onSurfaceCreated(override) called");

    renderer = this;

  }

}
