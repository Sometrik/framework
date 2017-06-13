package com.sometrik.framework;

import java.util.ArrayList;

import com.sometrik.framework.SliderButton.Position;

import android.view.View;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

public class SliderLayout extends RelativeLayout implements NativeCommandHandler {

  FrameWork frame;
  ArrayList<SliderButton> buttonList;
  int activeButton = 0;
  int nextId = 77777773;
  public static int buttonSize = 0;
  
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
    SliderButton button = new SliderButton(frame, this, true);
    button.setId(nextId);
    nextId++;
    if (buttonList.size() != 0){
      SliderButton topButton = buttonList.get(buttonList.size() - 1);
      topButton.setBottomButton(button);
      button.setTopButton(topButton);
    } else {
      button.setPosition(Position.MIDDLE);
      button.setInitialPosition();
    }
    if (buttonList.size() == 1){
      button.setPosition(Position.BOTTOM);
      button.setInitialPosition();
    } else if (buttonList.size() > 1){
      button.setPosition(Position.BOTTOMHIDDEN);
      button.setInitialPosition();
    }
    buttonList.add(button);
    addView(button);
    
    FWList list = new FWList(frame, new FWAdapter(frame, null));
    RelativeLayout.LayoutParams listParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
    listParams.addRule(RelativeLayout.BELOW, button.getId());
    list.setLayoutParams(listParams);
    list.setViewVisibility(false);
    button.setText(v);
    button.setList(list);
    addView(list);
  }

  @Override
  public void setValue(int v) {
    
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

  public int getActiveButton() {
    return activeButton;
  }

  public void setActiveButton(int activeButton) {
    this.activeButton = activeButton;
  }

}
