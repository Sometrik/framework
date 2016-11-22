package com.sometrik.framework;

import android.content.Context;
import android.view.SurfaceView;
import android.view.View;

public class NativeSurface extends SurfaceView implements NativeCommandHandler {
  
  private Context context;
  
  public NativeSurface(Context context) {
    super(context);
    this.context = context;
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

  @Override
  public void addChild(View view) {
    System.out.println("Native Surface couldn't handle addChild");
    //TODO
  }

  @Override
  public void handleCommand(NativeCommand command) {
    System.out.println("Native Surface couldn't handle command");
    //Possible commands that have to be handled
  }

  @Override
  public void addOption() {
    System.out.println("Native Surface couldn't handle command");
  }

}
