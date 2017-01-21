package com.sometrik.framework;

import android.content.Context;
import android.view.View;
import android.widget.ScrollView;

public class FWScrollView extends ScrollView implements NativeCommandHandler {

  FrameWork frame;
  
  public FWScrollView(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;
  }

  @Override
  public void addChild(View view) {
    System.out.println("Stack overflow?");
    addView(view);
    System.out.println("Stack overflow");
  }

  @Override
  public void removeChild(int id) {
    View view = getChildAt(0);
    if (view != null){
      if (view.getId() == id){
        this.removeChild(0);
      }
    }
  }
  
  @Override
  public void addOption(int optionId, String text) {
    System.out.println("FWScrollView couldn't handle command");
  }

  @Override
  public void setValue(String v) {
    System.out.println("FWScrollView couldn't handle command");
  }

  @Override
  public void setValue(int v) {
    if (v > 0){
      frame.setCurrentView(this);
    }
  }

  @Override
  public void setEnabled(Boolean enabled) {
    System.out.println("FWScrollView couldn't handle command");
  }

  @Override
  public void setStyle(String key, String value) { }

  @Override
  public void setError(boolean hasError, String errorText) { }

  @Override
  public int getElementId() {
    return getId();
  }
}
