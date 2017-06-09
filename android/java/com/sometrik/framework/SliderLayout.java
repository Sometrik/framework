package com.sometrik.framework;

import java.util.ArrayList;

import android.view.View;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

public class SliderLayout extends RelativeLayout implements NativeCommandHandler {

  FrameWork frame;
  ArrayList<SliderButton> buttonList;
  int nextId = 77777773;
  
  public SliderLayout(FrameWork frame) {
    super(frame);

    buttonList = new ArrayList<SliderButton>();
    this.frame = frame;

    LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
    setLayoutParams(params);
 
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
  public void addData(String text, int row, int column, int sheet) {
    SliderButton button = buttonList.get(sheet);
    button.getList().addData(text, row, column, 0);
  }

  @Override
  public void setValue(String v) {
    System.out.println("setValue");
    SliderButton button = new SliderButton(frame, this);
    button.setId(nextId);
    nextId++;
    if (buttonList.size() != 0){
      SliderButton topButton = buttonList.get(buttonList.size() - 1);
      topButton.setBottomButton(button);
      button.setTopButton(topButton);
    }
    buttonList.add(button);
    addView(button);
    button.setPosition();
    
    FWList list = new FWList(frame, new FWAdapter(frame, null));
    RelativeLayout.LayoutParams listParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
    listParams.addRule(RelativeLayout.BELOW, button.getId());
    listParams.topMargin = 10;
    list.setLayoutParams(listParams);
    list.setViewVisibility(false);
    button.setText(v);
    button.setList(list);
    addView(list);
    System.out.println("setValue end");
  }

  @Override
  public void setValue(int v) {
    if (v > 0){
      frame.setCurrentView(this, false);
    }
  }

  @Override
  public void setViewEnabled(Boolean enabled) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setViewVisibility(boolean visible) {
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
  public void clear() {
    // TODO Auto-generated method stub
    
  }

  @Override
  public int getElementId() {
    return getId();
  }

}
