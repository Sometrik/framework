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
  int nextId = 777775;
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
    SliderButton button = new SliderButton(frame, this, true, buttonList.size());
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
    System.out.println("setValue "  + v + " activeButton " + activeButton);
    if (activeButton == v){
      System.out.println("button already active");
      return;
    }
    activeButton = v;
    if (v < buttonList.size()) {
      for (int i = 0; i < buttonList.size(); i++) {
	System.out.println("button loop " + i);
	SliderButton button = buttonList.get(i);
	if (i == v) {
	  button.setPosition(Position.MIDDLE);
	} else if (i == (v - 1)) {
	  button.setPosition(Position.TOP);
	} else if (i == (v + 1)) {
	  button.setPosition(Position.BOTTOM);
	} else if (i < (v - 1)) {
	  button.setPosition(Position.TOPHIDDEN);
	} else if (i > (v + 1)) {
	  button.setPosition(Position.BOTTOMHIDDEN);
	}
      }
      
      for (SliderButton button : buttonList){
	button.setInitialPosition();
      }
    }
  }
  
//  @Override
//  public void onVisibilityChanged(View changedView, int visibility){
//    switch (visibility){
//    case VISIBLE:
//      
//      break;
//    }
//  }
  

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
    for (SliderButton button : buttonList){
      FWList list = button.getList();
      list.clear();
    }
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
