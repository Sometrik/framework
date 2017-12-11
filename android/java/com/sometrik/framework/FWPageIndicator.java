package com.sometrik.framework;

import java.util.ArrayList;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.drawable.ShapeDrawable;
import android.graphics.drawable.shapes.OvalShape;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.LinearLayout;

public class FWPageIndicator extends LinearLayout implements NativeCommandHandler {

  private FrameWork frame;
  private ArrayList<FrameLayout> indicatorList;
  
  
  public FWPageIndicator(FrameWork frame, int pageCount) {
    super(frame);
    this.frame = frame;
    indicatorList = new ArrayList<FrameLayout>();
    
    for (int i = 0; i < pageCount; i++) {
      FWFrameLayout shapeLayout = new FWFrameLayout(frame, 0);
      shapeLayout.setStyle(Selector.values()[0], "width", "10");
      shapeLayout.setStyle(Selector.values()[0], "height", "10");
      ShapeDrawable oval = new ShapeDrawable(new OvalShape());
      oval.setIntrinsicHeight(10);
      oval.setIntrinsicWidth(10);
      oval.getPaint().setColor(Color.parseColor("#c1272d"));
      shapeLayout.setBackgroundDrawable(oval);
      indicatorList.add(shapeLayout);
      addView(shapeLayout);
    }
  }
  
  public void setIndicator(int position) {
    
    
  }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addChild(View view) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addOption(int optionId, String text) {
    // TODO Auto-generated method stub
    
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
  public void setBitmap(Bitmap bitmap) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addImageUrl(String url, int width, int height) {
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
  public void setViewVisibility(boolean visible) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setStyle(Selector selector, String key, String value) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void applyStyles() {
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
  public void deinitialize() {
    // TODO Auto-generated method stub
    
  }

  @Override
  public int getElementId() {
    // TODO Auto-generated method stub
    return 0;
  }
}