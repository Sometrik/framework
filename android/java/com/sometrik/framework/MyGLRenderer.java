package com.sometrik.framework;

import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.EGLConfig;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;

public class MyGLRenderer implements GLSurfaceView.Renderer {

  private MyGLRenderer renderer;
  public Handler mHandler;
  AssetManager assetManager;
  MyGLSurfaceView GLView;
  FrameWork frame;
  Context context;
  float x;
  float y;
  long endTime;
  long startTime;

  public native void nativeOnDraw();

  public native void onInit(AssetManager assetManager, MyGLSurfaceView view);

  public native void Draw();

  public native void onResize(float x, float y);

  public native void onTouchesBegin(int fingerIndex);

  public native void onTouchesEnded(int fingerIndex);

  public native void onTouchesMoved(int fingerIndex);

  public MyGLRenderer(Context context, float x, float y) {

    frame = (FrameWork) context;
    startTime = System.currentTimeMillis();
    assetManager = context.getAssets();
    this.x = x;
    this.y = y;
  }

  public void onDrawFrame(GL10 unused) {

    // FPS cap kokeiluja
    endTime = System.currentTimeMillis();
    if (endTime > startTime + 33) {
      try {
	Thread.sleep(33);
      } catch (InterruptedException e) {
	e.printStackTrace();
      }

      // System.out.println("onDrawFrame called");
      startTime = System.currentTimeMillis();

      //Calls onDraw in AndroidPlatform
      nativeOnDraw();
    }

  }

  public void onSurfaceChanged(GL10 unused, int width, int height) {
    GLES20.glViewport(0, 0, width, height);
  }

  @Override
  public void onSurfaceCreated(GL10 gl, javax.microedition.khronos.egl.EGLConfig config) {
    System.out.println("(Renderer)onSurfaceCreated(override) called");

    renderer = this;
    System.out.println("surface check: " + frame.getSurfaceView());
    
    //Calls onInit in AndroidPlatform
    onInit(assetManager, frame.getSurfaceView());
    onResize(x, y);

  }

  // Wanhaa
  public static int loadShader(int type, String shaderCode) {

    // create a vertex shader type (GLES20.GL_VERTEX_SHADER)
    // or a fragment shader type (GLES20.GL_FRAGMENT_SHADER)
    int shader = GLES20.glCreateShader(type);

    // add the source code to the shader and compile it
    GLES20.glShaderSource(shader, shaderCode);
    GLES20.glCompileShader(shader);

    return shader;
  }

}
