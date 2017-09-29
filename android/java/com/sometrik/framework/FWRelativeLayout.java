package com.sometrik.framework;

import android.content.Context;
import android.graphics.Bitmap.Config;
import android.view.View;
import android.widget.RelativeLayout;

public class FWRelativeLayout extends RelativeLayout implements NativeCommandHandler {

  private FrameWork frame;
  
  public FWRelativeLayout(FrameWork frame) {
    super(frame);
    this.frame = frame;
  }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addChild(View view) {
    addView(view);
  }

  @Override
  public void addOption(int optionId, String text) {
    for (int i = 0; i < getChildCount(); i++) {
      if (optionId == getChildAt(i).getId()) {
	System.out.println("relative: child found");
	View view = getChildAt(i);
	LayoutParams params = (LayoutParams) view.getLayoutParams();
	if (text.equals("parent-left")) {
	  params.addRule(RelativeLayout.ALIGN_PARENT_LEFT);
	}
      }
    }
  }

  @Override
  public void addColumn(String text, int columnType) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setValue(String v) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setImage(byte[] bytes, int width, int height, Config config) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setValue(int v) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void reshape(int value, int size) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void reshape(int size) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setViewEnabled(Boolean enabled) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setViewVisibility(boolean visible) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setStyle(String key, String value) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setError(boolean hasError, String errorText) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void clear() {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void flush() {
    // TODO Auto-generated method stub
    
  }

  @Override
  public int getElementId() {
    return getId();
  }

}
