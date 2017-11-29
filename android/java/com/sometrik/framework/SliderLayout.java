package com.sometrik.framework;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Timer;
import java.util.TimerTask;

import com.sometrik.framework.NativeCommand.Selector;
import com.sometrik.framework.SliderButton.Position;

import android.graphics.Bitmap;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.FrameLayout;
import android.widget.RelativeLayout;

public class SliderLayout extends RelativeLayout implements NativeCommandHandler {

  FrameWork frame;
  ArrayList<SliderButton> buttonList;
  HashMap<Integer, String> labelList;
  int activeButton = 0;
  boolean topVisible = false;
  int nextId = 777775;
  public static int buttonSize = 0;
  int tableSize = 0;
  boolean usesLists = true;
  private int bottomLayoutId = 0;
  private int topLayoutId = 0;
  
  public SliderLayout(FrameWork frame) {
    super(frame);

    buttonList = new ArrayList<SliderButton>();
    labelList = new HashMap<Integer, String>();
    this.frame = frame; 
  }
  
  public void setAllButtonsDisabled(){
    for (SliderButton button : buttonList){
      button.disableOnClick = true;
      button.child.setViewVisibility(false);
      // button.child.setViewEnabled(false);
    }
    
    Timer timer = new Timer();
    TimerTask task = new TimerTask(){
      @Override
      public void run() {
	for (SliderButton button : buttonList) {
	  button.disableOnClick = false;
	  // button.child.setViewEnabled(true);
	}
      }
    };
    timer.schedule(task, SliderButton.transitionTime);
  }
  
  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addChild(View view) {
    usesLists = false;
    final int positionId = buttonList.size();
    SliderButton button = new SliderButton(frame, this, true, positionId);
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
      tableSize++;
      addView(button);
    }
    
    RelativeLayout.LayoutParams listParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT);
    listParams.addRule(RelativeLayout.BELOW, button.getId());
    if (bottomLayoutId != 0) {
      listParams.addRule(RelativeLayout.ABOVE, bottomLayoutId);
    }
    FWScrollLayout scrollView = new FWScrollLayout(frame, 0);
    scrollView.setViewVisibility(false);
    scrollView.setLayoutParams(listParams);
    scrollView.addChild(view);
    if (view instanceof FWLayout) {
      FWLayout layout = (FWLayout) view;
      
      FrameLayout.LayoutParams fwLayoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.MATCH_PARENT, FrameLayout.LayoutParams.WRAP_CONTENT);
      layout.setLayoutParams(fwLayoutParams);
    }
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
    if (optionId < buttonList.size()){
      SliderButton button = buttonList.get(optionId);
      button.setText(text);
    }
    labelList.put(optionId, text);
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    if (sheet < buttonList.size()) {
      SliderButton button = buttonList.get(sheet);
      button.getList().addData(text, row, column, 0);
    } else {
      setValue(labelList.get(sheet));
      addData(text,row,column,sheet);
    }
  }

  @Override
  public void setValue(String v) {

    if (usesLists) {
      final int sheet = buttonList.size();
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
      tableSize++;
      addView(button);
      FWList list = new FWList(frame, new FWAdapter2(frame, null));
      list.setOnItemClickListener(new OnItemClickListener() {

        @Override
        public void onItemClick(AdapterView<?> arg0, View arg1, int groupPosition, long id) {
  	System.out.println("row clicked. Sending intChangedEvent of " + (groupPosition - 1));
  	  frame.sendNativeValueEvent(getId(), (groupPosition - 1), sheet);
        }
      });
      
      RelativeLayout.LayoutParams listParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT);
      listParams.addRule(RelativeLayout.BELOW, button.getId());
      list.setLayoutParams(listParams);
      list.setViewVisibility(false);
      button.setList(list);
      addView(list);
      button.setText(v);
      labelList.put(buttonList.size() - 1, v);
      button.setInitialPosition();
    } else {
      //TODO
    }
  }
  
  public void setBottomLayout(int viewId) {
    bottomLayoutId = viewId;
  }
  
  public void setTopLayout(int viewId) {
    topLayoutId = viewId;
  }

  @Override
  public void setValue(int v) {
    if (buttonList.size() == 0) {
      activeButton = v;
      return;
    }
    if (activeButton == v) {
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
  public void setViewVisibility(boolean visible) {
    this.setVisibility(visible ? VISIBLE : GONE);
  }

  @Override
  public void setStyle(Selector selector, String key, String value) {
    // TODO Auto-generated method stub
  }
  
  @Override
  public void applyStyles() { }

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

  @Override
  public void addColumn(String text, int columnType) {
    for (SliderButton button : buttonList){
      button.getList().addColumn(text, columnType);
    }
  }

  @Override
  public void reshape(int value, int size) {
    // RESHAPE_SHEET
    if (value < buttonList.size()) {
      SliderButton button = buttonList.get(value);
      button.getList().reshape(0, size);
    } else {
      System.out.println("Error reshaping list. index of " + value + " is too big");
    }
  }


  @Override
  public void reshape(int size) {
    // RESHAPE_TABLE

      if (tableSize == size) {
	return;
      }
      int removeCounter = 0;
      if (size <= buttonList.size()) {
	for (int i = 0; i < buttonList.size(); i++) {
	  if (i >= size) {
	    SliderButton button = buttonList.get(i);
	    System.out.println("removing button " + i);
	    removeView(button);
	    View list = (View) button.getList();
	    removeCounter++;
	    removeView(list);
	  } else if (i > tableSize) {
	    System.out.println("adding back in button");
	    setValue(labelList.get(i));
	  }
	}
      } else if (size > buttonList.size()) {
	for (int i = buttonList.size(); i < size; i++) {
	  if (usesLists) {
	    setValue("");
	  } else {
	    addChild(new FWLayout(frame, 0));
	  }
	}
      }
      for (int i = 0; i < removeCounter; i++) {
	System.out.println("removing from buttonList");
	buttonList.remove(buttonList.size() - 1);
      }

      tableSize = size;
      this.invalidate();
  }
  
  @Override
  public void setBitmap(Bitmap bitmap) { }

  @Override
  public void deinitialize() { }

  @Override
  public void addImageUrl(String url, int width, int height) {
    // TODO Auto-generated method stub
    
  }
}
