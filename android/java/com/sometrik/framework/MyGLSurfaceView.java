package com.sometrik.framework;


import android.content.Context;
import android.content.SharedPreferences;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Path;
import android.opengl.GLSurfaceView;
import android.os.Handler;

public class MyGLSurfaceView extends GLSurfaceView implements NativeMessageHandler {

  private Context context;
  public Handler sHandler;
  private MyGLRenderer renderer;
  private FrameWork frame;
  private AssetManager assetManager;
  private Bitmap nativeCanvasBitmap;
  private Bitmap image1;
  private Paint mPaint;
  private Path mPath;
  private SharedPreferences prefs;
  private SharedPreferences.Editor editor;
  private int viewId;
  
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
  
  public int getViewId(){
    return viewId;
  }

  @Override
  public void handleMessage(NativeMessage message) {
    // TODO Auto-generated method stub
    
  }


}
