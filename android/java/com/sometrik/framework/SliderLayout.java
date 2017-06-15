package com.sometrik.framework;

import java.util.ArrayList;
import java.util.HashMap;

import com.sometrik.framework.SliderButton.Position;

import android.view.View;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

public class SliderLayout extends RelativeLayout implements NativeCommandHandler {

  FrameWork frame;
  ArrayList<SliderButton> buttonList;
  HashMap<Integer, String> labelList;
  int activeButton = 0;
  int nextId = 777775;
  public static int buttonSize = 0;
  boolean usesLists = true;
  
  public SliderLayout(FrameWork frame) {
    super(frame);

    buttonList = new ArrayList<SliderButton>();
    labelList = new HashMap<Integer, String>();
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
    usesLists = false;
    SliderButton button = new SliderButton(frame, this, true, buttonList.size());
    button.setId(nextId);
    nextId++;
    if (activeButton == 0){
      if (buttonList.size() != 0){
        SliderButton topButton = buttonList.get(buttonList.size() - 1);
        topButton.setBottomButton(button);
        button.setTopButton(topButton);
      } else {
        button.setPosition(Position.MIDDLE);
      }
      if (buttonList.size() == 1){
        button.setPosition(Position.BOTTOM);
      } else if (buttonList.size() > 1){
        button.setPosition(Position.BOTTOMHIDDEN);
      }
      buttonList.add(button);
      addView(button);
    }
    
    RelativeLayout.LayoutParams listParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, 0);
    listParams.addRule(RelativeLayout.BELOW, button.getId());
    FWScrollView scrollView = new FWScrollView(frame);
    scrollView.setViewVisibility(false);
    scrollView.setLayoutParams(listParams);
    scrollView.addChild(view);
    addView(scrollView);
    button.setList(scrollView);
    String label = labelList.get(buttonList.size() - 1);
    if (label != ""){
      button.setText(label);
    } else {
      button.setText("Label");
    }
    button.setInitialPosition();
  }

  @Override
  public void addOption(int optionId, String text) {
    System.out.println("Slider addOption: " + optionId + " " + text + " " + buttonList.size());
    if (optionId < buttonList.size()){
      SliderButton button = buttonList.get(optionId);
      button.setText(text);
    }
    labelList.put(optionId, text);
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    SliderButton button = buttonList.get(sheet);
    button.getList().addData(text, row, column, 0);
  }

  @Override
  public void setValue(String v) {

    if (usesLists) {
      SliderButton button = new SliderButton(frame, this, true, buttonList.size());
      button.setId(nextId);
      nextId++;
      if (buttonList.size() != 0) {
	SliderButton topButton = buttonList.get(buttonList.size() - 1);
	topButton.setBottomButton(button);
	button.setTopButton(topButton);
      } else {
	button.setPosition(Position.MIDDLE);
      }
      if (buttonList.size() == 1) {
	button.setPosition(Position.BOTTOM);
      } else if (buttonList.size() > 1) {
	button.setPosition(Position.BOTTOMHIDDEN);
      }
      buttonList.add(button);
      addView(button);
      FWList list = new FWList(frame, new FWAdapter(frame, null));
      RelativeLayout.LayoutParams listParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
      listParams.addRule(RelativeLayout.BELOW, button.getId());
      list.setLayoutParams(listParams);
      list.setViewVisibility(false);
      button.setList(list);
      addView(list);
      button.setText(v);
      button.setInitialPosition();
    } else {
      
    }
  }

  @Override
  public void setValue(int v) {
    System.out.println("setValue "  + v + " activeButton " + activeButton + " buttonList.size " + buttonList.size() + " " + getId());
    if (buttonList.size() == 0){
      activeButton = v;
      return;
    }
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
  
  @Override
  public void setViewEnabled(Boolean enabled) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setViewVisibility(boolean visible) {
    if (visible){
      this.setVisibility(VISIBLE);
    } else{
      this.setVisibility(GONE);
    }
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
//    for (SliderButton button : buttonList){
//      NativeCommandHandler child = button.getList();
//      child.clear();
//    }
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

  @Override
  public void flush() {
    // TODO Auto-generated method stub
    
  }

}
