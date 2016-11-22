package com.sometrik.framework;

import android.content.Context;
import android.view.View;
import android.widget.LinearLayout;

public class FWLayout extends LinearLayout implements NativeCommandHandler {
  
  private Context context;
  private FrameWork frame;
  
  public FWLayout(Context context) {
    super(context);
    this.context = context;
    this.frame = (FrameWork)context;
  }

  @Override
  public int getElementId() {
    return getId();
  }
  
  @Override
  public void showView(){
    frame.setContentView(this);
    FrameWork.currentView = getId();
  }
 
  @Override
  public void addChild(View view) {
    addView((View)view);
  }

  @Override
  public void handleCommand(NativeCommand command) {
    System.out.println("Layout couldn't handle command");
    //Possible commands that have to be handled
  }

  @Override
  public void addOption(int position, String text) {
    System.out.println("Native Surface couldn't handle command");
  }

}
