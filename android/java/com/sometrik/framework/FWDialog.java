package com.sometrik.framework;

import android.app.Dialog;
import android.view.View;
import android.view.ViewGroup;

public class FWDialog extends Dialog implements NativeCommandHandler{

  FrameWork frame;
  ViewGroup baseView;
  int id;
  
  public FWDialog(FrameWork frame, int id) {
    super(frame);
    this.frame = frame;
    this.id = id;
  }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
  }

  @Override
  public void addChild(View view) {
    if (baseView == null){
      baseView = (ViewGroup) view;
      setContentView(view);
    } else {
      baseView.addView(view);
    }
  }

  @Override
  public void addOption(int optionId, String text) {
    System.out.println("FWDialog couldn't handle the command");
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    System.out.println("FWDialog couldn't handle the command");
  }

  @Override
  public void setValue(String v) {
    setTitle(v);
  }

  @Override
  public void setValue(int v) {
    if (v > 0){
      show();
    } else {
      dismiss();
    }
  }

  @Override
  public void setViewEnabled(Boolean enabled) {
    if (enabled){
      show();
    } else {
      dismiss();
    }
  }

  @Override
  public void setViewVisibility(boolean visible) {
    if (visible){
      show();
    } else {
      dismiss();
    }
  }

  @Override
  public void setStyle(String key, String value) {
    System.out.println("FWDialog couldn't handle the command");
  }

  @Override
  public void setError(boolean hasError, String errorText) {
    System.out.println("FWDialog couldn't handle the command");
  }

  @Override
  public void clear() {
    dismiss();
  }

  @Override
  public int getElementId() {
    return id;
  }

}
