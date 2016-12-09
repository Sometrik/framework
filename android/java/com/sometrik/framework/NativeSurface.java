package com.sometrik.framework;

import android.content.Context;
import android.view.SurfaceView;
import android.view.View;

public class NativeSurface extends SurfaceView implements NativeCommandHandler {
  
  private FrameWork framework;
  //draw and update call frequency in millis
  private static final int UPDATE_FREQUENCY = 16;
  
  public NativeSurface(FrameWork framework) {
    super(framework);
    this.framework = framework;
    System.out.println("Native Surface constructor complete");
  }
  
  @Override
  public void showView() {
    FrameWork frame = framework;
    frame.setContentView(this);
    FrameWork.currentView = getId();
    framework.setToDraw(new drawRunnable(getId()));
  }
  

  
  class drawRunnable implements Runnable {
  	
  	int viewId;
  	public drawRunnable(int viewId){
  		this.viewId = viewId;
  	}
  	
		@Override
		public void run() {

			double timestamp = System.currentTimeMillis() / 1000.0;
			framework.nativeOnDraw(timestamp, viewId);
			framework.nativeOnUpdate(timestamp, viewId);
			if (framework.getDrawMode()){
				framework.mainHandler.postDelayed(this, UPDATE_FREQUENCY);
			}
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
