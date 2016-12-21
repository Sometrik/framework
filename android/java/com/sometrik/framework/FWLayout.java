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
    System.out.println("showing " + getId());
    if (vp == null){
      frame.setCurrentView(this);
    } else {
      frame.setCurrentView(this);
    }
  }
 
  @Override
  public void addChild(View view) {
    addView((View)view);
  }

  @Override
  public void addOption(int position, String text) {
    System.out.println("Native Surface couldn't handle command");
  }

  @Override
  public void removeChild(int id) {
    for (int i = 0; i < getChildCount(); i++) {
      View v = getChildAt(i);
      if (v.getId() == id) {
	removeView(v);
	System.out.println("Child " + id + " removed succesfully");
	return;
      }
    }
    System.out.println("Child " + id + " wasn't found and wasn't removed");
  }
}
