package com.sometrik.framework;

import android.content.Context;
import android.view.View;
import android.view.ViewParent;
import android.widget.LinearLayout;

public class FWLayout extends LinearLayout implements NativeCommandHandler {
  
  private FrameWork frame;
  
  public FWLayout(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;
  }

  @Override
  public int getElementId() {
    return getId();
  }
 
  @Override
  public void addChild(View view) {
    addView(view);
  }

  @Override
  public void addOption(int optionId, String text) {
    System.out.println("Native Surface couldn't handle command");
  }

  @Override
  public void setValue(String v) {
    System.out.println("FWLayout couldn't handle command");
  }

  @Override
  public void setValue(int v) {
    if (v == 1){
      frame.setCurrentView(this, true);
    } else if (v == 2){
      frame.setCurrentView(this, false);
    }
  }

  @Override
  public void setViewEnabled(Boolean enabled) {
    System.out.println("FWLayout couldn't handle command");
  }

  @Override
  public void setStyle(String key, String value) { }

  @Override
  public void setError(boolean hasError, String errorText) { }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub
    
  }
}
