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
  
  public FWScrollView(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;
    LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
    this.setLayoutParams(params);
    this.setFillViewport(true);
  }
  
  @Override
  public void onVisibilityChanged(View changedView, int visibility) {
    switch (visibility) {
    case VISIBLE:
      frame.visibilityChangedEvent(System.currentTimeMillis() / 1000.0, getId(), true);
      break;
    case GONE:
    case INVISIBLE: 
      frame.visibilityChangedEvent(System.currentTimeMillis() / 1000.0, getId(), false);
    break;
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
  public void addData(String text, int row, int column, int sheet) {
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
  public void setImage(byte[] bytes) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void reshape(int size) {
    // TODO Auto-generated method stub
    
  }
}
