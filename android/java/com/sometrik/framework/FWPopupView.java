package com.sometrik.framework;

import android.view.Gravity;
import android.view.View;
import android.widget.PopupWindow;

public class FWPopupView extends PopupWindow implements NativeCommandHandler {

  FrameWork frame;
  int parentViewId;
  int id;
  
  public FWPopupView(FrameWork frame, int parentViewId, int id){
    this.frame = frame;
    this.parentViewId = parentViewId;
    this.id = id;
  }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    
  }

  @Override
  public void addChild(View view) {
    System.out.println("setting new contentView for popupView");
    this.setContentView(view);
  }

  @Override
  public void addOption(int optionId, String text) {
    System.out.println("command couldn't be handled by popupView");
  }

  @Override
  public void addData(int rowNumber, int columnNumber, String text) {
    System.out.println("command couldn't be handled by popupView");
  }

  @Override
  public void setValue(String v) {
    System.out.println("command couldn't be handled by popupView");
  }

  @Override
  public void setValue(int v) {
    if (v == 1) {
      System.out.println("Trying to show FWPopupView on parentViewId " + parentViewId);
      View view = (View)frame.views.get(parentViewId);
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
      showAtLocation((View)frame.views.get(parentViewId), Gravity.CENTER, 0, 0);
    } else {
      dismiss();
    }
  }

  @Override
    public void setViewVisibility(boolean visible) {
    if (visible){
      showAtLocation((View)frame.views.get(parentViewId), Gravity.CENTER, 0, 0);
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
  
}
