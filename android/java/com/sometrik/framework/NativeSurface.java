package com.sometrik.framework;

import android.content.Context;
import android.view.SurfaceView;
import android.view.View;

public class NativeSurface extends SurfaceView implements NativeCommandHandler {
  
  private Context context;
  private FrameWork framework;
  
  public NativeSurface(Context context, FrameWork framework) {
    super(context);
    this.context = context;
    this.framework = framework;
    System.out.println("Native Surface constructor complete");
  }
  
  @Override
  public void showView() {
    FrameWork frame = (FrameWork)context;
    frame.setContentView(this);
    FrameWork.currentView = getId();
  }

  @Override
  protected void onSizeChanged(int w, int h, int oldw, int oldh) {
    super.onSizeChanged(w, h, oldw, oldh);
    System.out.println("NativeSurface onSizeChanged called");
    
    float width = w;
    float height = h;
    framework.onResize(System.currentTimeMillis() / 1000, width, height, getId());
    
  }

  @Override
  public int getElementId() {
    return getId();
  }

  @Override
  public void addChild(View view) {
    System.out.println("Native Surface couldn't handle addChild");
    //TODO
  }
  @Override
  public void addOption(int position, String text) {
    System.out.println("Native Surface couldn't handle command");
  }

}
