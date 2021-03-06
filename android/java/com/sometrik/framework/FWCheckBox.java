package com.sometrik.framework;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap;
import android.view.View;
import android.widget.CheckBox;

public class FWCheckBox extends CheckBox implements NativeCommandHandler {

  FrameWork frame;
  
  public FWCheckBox(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;
  }

  @Override
  public void addChild(View view) {
    System.out.println("FWCheckBox couldn't handle command");
  }
  
  @Override
  public void addOption(int optionId, String text) {
    System.out.println("FWCheckBox couldn't handle command");
  }

  @Override
  public void setValue(String v) {
    setText(v);
  }

  @Override
  public void setValue(int v) {
    if (v > 0){
      setChecked(true);
    } else {
      setChecked(false);
    }
  }

  @Override
  public void setStyle(Selector selector, String key, String value) { }

  @Override
  public void applyStyles() { }
  
  @Override
  public void setError(boolean hasError, String errorText) { }

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
    System.out.println("FWCheckBox couldn't handle command");
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
