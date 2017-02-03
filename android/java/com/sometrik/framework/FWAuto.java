package com.sometrik.framework;

import java.util.ArrayList;

import android.content.Context;
import android.util.Log;
import android.view.View;
import android.widget.LinearLayout;

public class FWAuto extends LinearLayout implements NativeCommandHandler {
  
  private FrameWork frame;
  private ArrayList<View> viewList;
  private ArrayList<LinearLayout> layoutList;
  private LinearLayout.LayoutParams params;

  public FWAuto(FrameWork frame) {
    super(frame);
    this.frame = frame;
    viewList = new ArrayList<View>();
    layoutList = new ArrayList<LinearLayout>();
    params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
    setLayoutParams(params);
    setOrientation(LinearLayout.HORIZONTAL);
  }
  
  
  private void buildAuto(int columns){
    
    ArrayList<View> copyViewList = new ArrayList<View>();
    for (View view : viewList){
      copyViewList.add(view);
    }
    layoutList = new ArrayList<LinearLayout>();

    for (int i = 1; i <= columns; i++) {
      LinearLayout newLayout = new LinearLayout(frame);
      newLayout.setOrientation(LinearLayout.VERTICAL);
      newLayout.setLayoutParams(params);
      layoutList.add(newLayout);
      for (int viewI = 0; viewI < viewList.size() / columns; viewI++) {
	newLayout.addView(copyViewList.get(0));
	copyViewList.remove(0);
      }
      addView(newLayout);
    }
  }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub

  }
  
  private void resetLayout(){
    for (LinearLayout layout : layoutList) {
      Log.d("auto", "Removing layout");
      layout.removeAllViews();
      removeView(layout);
    }
  }
  
  private boolean canFitColumns(){
    int width = 0;
    for (LinearLayout layout : layoutList){
      width += frame.measureViewWidth(layout);
      Log.d("autoMeasure", "thisWidth +: " + width);
    }
//    int width = frame.measureViewWidth(this);
    Log.d("autoMeasure", "thisWidth final: " + width);
    Log.d("autoMeasure", "screenWidth " + frame.getScreenWidth());
    if (width >= frame.getScreenWidth() || width == 0){
      return false;
    } else {
      return true;
    }
  }
  
  private int measureColumnCount(){
    
    int columns = 1;
    while (true){
      buildAuto(columns + 1);
      if (!canFitColumns()){
	Log.d("auto", "cant fit anymore. Returning " + columns);
	break;
      } else {
	resetLayout();
	columns++;
      }
    }

    resetLayout();
    return columns;
  }

  @Override
  public void addChild(View view) {
    resetLayout();
    viewList.add(view);
    int columns = measureColumnCount();
    buildAuto(columns);
  }

  @Override
  public void addOption(int optionId, String text) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setValue(String v) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setValue(int v) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setViewEnabled(Boolean enabled) {
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
  public int getElementId() {
    return getId();
  }

}
