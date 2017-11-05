package com.sometrik.framework;

import com.sometrik.framework.NativeCommand.Selector;

import android.content.Context;
import android.view.View;
import android.widget.Switch;

public class FWSwitch extends Switch implements NativeCommandHandler {

  FrameWork frame;
  
  public FWSwitch(FrameWork frame) {
    super(frame);
    this.frame = frame;
  }

  @Override
  public void addChild(View view) {
    System.out.println("FWSwitch coulnd't handle Command");
  }

  @Override
  public void addOption(int optionId, String text) {
    System.out.println("FWSwitch coulnd't handle Command");
  }

  @Override
  public void setValue(String v) {
    setText(v);
  }

  @Override
  public void setValue(int v) {
    if (v > 0){
      this.setSelected(true);
    } else {
      this.setSelected(false);
    }
  }

  @Override
  public void setStyle(Selector selector, String key, String value) {
    System.out.println("FWSwitch coulnd't handle Command");
  }

  @Override
  public void setError(boolean hasError, String errorText) {
    System.out.println("FWSwitch coulnd't handle Command");
  }

  @Override
  public int getElementId() {
    return getId();
  }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    System.out.println("FWSwitch coulnd't handle Command");
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
  public void setBitmap(Bitmap bitmap) { }

  @Override
  public void reshape(int size) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void deinitialize() {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addImageUrl(String url, int width, int height) {
    // TODO Auto-generated method stub
    
  }
}
