package com.sometrik.framework;

import java.util.Timer;
import java.util.TimerTask;

import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

public class NativeSurface extends SurfaceView implements NativeCommandHandler {
  
  private FrameWork framework;
  private Timer timer;
  private DrawTimer drawTimer;
  //draw and update call frequency in millis
  private static final int UPDATE_FREQUENCY = 16;
  
  public NativeSurface(FrameWork framework) {
    super(framework);
    this.framework = framework;
    timer = new Timer();
    System.out.println("Native Surface constructor complete");
  }
  
  class DrawTimer extends TimerTask {

    int viewId;
   
    public DrawTimer(int viewId){
      this.viewId = viewId;
    }

    @Override
    public void run() {
      FrameWork.onUpdate(System.currentTimeMillis() / 1000.0, viewId);
    }
  }

  @Override
  protected void onSizeChanged(int w, int h, int oldw, int oldh) {
//    super.onSizeChanged(w, h, oldw, oldh);
    System.out.println("NativeSurface onSizeChanged called");
    
    float width = w;
    float height = h;
//    FrameWork.onResize(System.currentTimeMillis() / 1000.0, width, height, getId());
    
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
  public void addOption(int optionId, String text) {
    System.out.println("Native Surface couldn't handle command");
  }
  
  @Override
  public void onWindowVisibilityChanged (int visibility){
    super.onWindowVisibilityChanged(visibility);

    timer.cancel();
    drawTimer = new DrawTimer(getId());
    timer = new Timer();
    
    switch (visibility){
    case GONE:
      System.out.println("NativeSurface visibility changed to GONE");
      break;
    case INVISIBLE:
      System.out.println("NativeSurface visibility changed to INVISIBLE");
      break;
    case VISIBLE:
      System.out.println("NativeSurface visibility changed to VISIBLE");
      timer.scheduleAtFixedRate(drawTimer, UPDATE_FREQUENCY, UPDATE_FREQUENCY);
      break;
    }
  }

  @Override
  public void setValue(String v) {
    framework.setCurrentView(this, true);
    drawTimer = new DrawTimer(getId());
    timer.scheduleAtFixedRate(drawTimer, UPDATE_FREQUENCY, UPDATE_FREQUENCY);
  }

  @Override
  public void setValue(int v) {
    if (v == 1){
      framework.setCurrentView(this, true);
      drawTimer = new DrawTimer(getId());
      timer.scheduleAtFixedRate(drawTimer, UPDATE_FREQUENCY, UPDATE_FREQUENCY);
    } else if (v == 2) {
      framework.setCurrentView(this, false);
    }
  }

  @Override
  public void setViewEnabled(Boolean enabled) {
    System.out.println("FrameWork couldn't handle command");
  }

  @Override
  public void setStyle(String key, String value) { }

  @Override
  public void setError(boolean hasError, String errorText) { }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    System.out.println("Native Surface couldn't handle command");
  }

  @Override
  public void setViewVisibility(boolean visibility) {
    if (visibility){
      this.setVisibility(VISIBLE);
    } else {
      this.setVisibility(INVISIBLE);
    }
  }

  @Override
  public void clear() {
    System.out.println("Native Surface couldn't handle command");
  }

  @Override
  public void flush() {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addColumn(String text, int columnType) {
    // TODO Auto-generated method stub
    
  }
}
