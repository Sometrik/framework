package com.sometrik.framework;

import java.util.Timer;
import java.util.TimerTask;

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
  
  @Override
  public void showView() {
    FrameWork frame = framework;
    frame.setContentView(this);
    drawTimer = new DrawTimer(getId());
    FrameWork.currentView = getId();
    timer.scheduleAtFixedRate(drawTimer, UPDATE_FREQUENCY, UPDATE_FREQUENCY);
  }
  
  class DrawTimer extends TimerTask {

    int viewId;
   
    public DrawTimer(int viewId){
      this.viewId = viewId;
    }

    @Override
    public void run() {
      System.out.println("onUpdate");
      FrameWork.onUpdate(System.currentTimeMillis() / 1000.0, viewId);
    }
  }

  @Override
  protected void onSizeChanged(int w, int h, int oldw, int oldh) {
    super.onSizeChanged(w, h, oldw, oldh);
    System.out.println("NativeSurface onSizeChanged called");
    
    float width = w;
    float height = h;
    FrameWork.onResize(System.currentTimeMillis() / 1000.0, width, height, getId());
    
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
