package com.sometrik.framework;

import android.content.Context;
import android.view.SurfaceView;

public class NativeSurface extends SurfaceView implements NativeMessageHandler {
  
  private Context context;
  
  public NativeSurface(Context context) {
    super(context);
    this.context = context;
  }

  @Override
  public void handleMessage(NativeMessage message) {
    
    switch (message.getMessage()){
    case SHOW_VIEW:
      showView();
      break;
    default:
      System.out.println("Message couldn't been handled by NativeSurface");
      break;
    }
  }
  

  @Override
  public void showView() {
    FrameWork frame = (FrameWork)context;
    frame.setContentView(this);
    FrameWork.currentView = getId();
  }

  @Override
  public int getElementId() {
    return getId();
  }

}
