package com.sometrik.framework;

import android.content.Context;
import android.view.Gravity;
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
    System.out.println("FWLayout couldn't handle command");
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
  public void setStyle(String key, String value) {
    if (key.equals("gravity")){
	LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      if (value.equals("bottom")){
	params.gravity = Gravity.BOTTOM;
      } else if (value.equals("top")){
	params.gravity = Gravity.TOP;
      } else if (value.equals("left")){
	params.gravity = Gravity.LEFT;
      } else if (value.equals("right")){
	params.gravity = Gravity.RIGHT;
      }
      setLayoutParams(params);
    }
  }

  @Override
  public void setError(boolean hasError, String errorText) { }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    invalidate();
  }

  @Override
  public void addData(int rowNumber, int columnNumber, String text) {
    System.out.println("FWLayout couldn't handle command");
  }

  @Override
  public void setViewVisibility(boolean visibility) {
    if (visibility){
      this.setVisibility(VISIBLE);
    } else {
      this.setVisibility(INVISIBLE);
    }
  }
}
