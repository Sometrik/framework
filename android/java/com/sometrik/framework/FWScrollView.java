package com.sometrik.framework;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.ScrollView;

public class FWScrollView extends ScrollView implements NativeCommandHandler {

  FrameWork frame;
  private String title;
  
  public FWScrollView(FrameWork frameWork, String title) {
    super(frameWork);
    this.frame = frameWork;
    this.title = title;
    LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
    this.setLayoutParams(params);
    this.setFillViewport(true);
    if (frame.getActionBar() != null){
      frame.getActionBar().setTitle(title);
    }
  }

  @Override
  public void addChild(View view) {
    addView(view);
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
    if (v == 1){
      frame.setCurrentView(this, true, title);
    } else if (v == 2){
      frame.setCurrentView(this, false, title);
    }
  }

  @Override
  public void setViewEnabled(Boolean enabled) {
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

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    invalidate();
  }

  @Override
  public void addData(int rowNumber, int columnNumber, String text) {
    System.out.println("FWScrollView couldn't handle command");
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
