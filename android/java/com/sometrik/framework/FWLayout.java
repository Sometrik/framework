package com.sometrik.framework;

import android.content.Context;
import android.view.View;
import android.view.ViewParent;
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
    ViewParent vp = getParent();
    if (vp == null){
      frame.setContentView(this);
    } else {
      frame.setContentView((View)vp);
    }
    FrameWork.currentView = getId();
  }
 
  @Override
  public void addChild(View view) {
    addView((View)view);
  }

  @Override
  public void addOption(int position, String text) {
    System.out.println("Native Surface couldn't handle command");
  }

}
