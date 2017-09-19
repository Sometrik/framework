package com.sometrik.framework;

import android.R;
import android.graphics.Bitmap.Config;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.PopupWindow;

public class FWPopupView extends PopupWindow implements NativeCommandHandler {

  FrameWork frame;
  int id;
  
  public FWPopupView(FrameWork frame, int id) {
    super(frame);
    this.frame = frame;
    this.id = id;
    setWidth(ViewGroup.LayoutParams.WRAP_CONTENT);
    setHeight(ViewGroup.LayoutParams.WRAP_CONTENT);
    
    //Default: Set background color as light rather than dark
    this.setBackgroundDrawable(frame.getResources().getDrawable(R.drawable.screen_background_light));
  }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    
  }

  @Override
  public void addChild(View view) {
    System.out.println("setting new contentView for popupView");
    this.setContentView(view);
    //Some default padding
    view.setPadding(20, 20, 20, 20);
  }

  @Override
  public void addOption(int optionId, String text) {
    System.out.println("command couldn't be handled by popupView");
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    System.out.println("command couldn't be handled by popupView");
  }

  @Override
  public void setValue(String v) {
    System.out.println("command couldn't be handled by popupView");
  }

  @Override
  public void setValue(int v) {
    if (v == 1) {
      System.out.println("Trying to show FWPopupView on parentViewId " + frame.getCurrentViewId());
      View view = (View)frame.views.get(frame.getCurrentViewId());
      if (view == null) {
	System.out.println("view was null");
	return;
      }
      showAtLocation(view, Gravity.CENTER, 0, 0);
    } else if (v <= 0) {
      dismiss();
    }
  }

  @Override
  public void setViewEnabled(Boolean enabled) {
    if (enabled){
      showAtLocation((View)frame.views.get(frame.getCurrentViewId()), Gravity.CENTER, 0, 0);
    } else {
      dismiss();
    }
  }

  @Override
    public void setViewVisibility(boolean visible) {
    if (visible){
      showAtLocation((View)frame.views.get(frame.getCurrentViewId()), Gravity.CENTER, 0, 0);
    } else {
      dismiss();
    }
  }

  @Override
  public void setStyle(String key, String value) {
    System.out.println("command couldn't be handled by popupView");
  }

  @Override
  public void setError(boolean hasError, String errorText) {
    System.out.println("command couldn't be handled by popupView");
  }

  @Override
  public int getElementId() {
    return id;
  }

  @Override
  public void clear() {
    System.out.println("couldn't handle command");
  }

  @Override
  public void flush() {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addColumn(String text, int columnType) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void reshape(int value, int size) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setImage(byte[] bytes, int width, int height, Config config) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void reshape(int size) {
    // TODO Auto-generated method stub
    
  }
  
}
