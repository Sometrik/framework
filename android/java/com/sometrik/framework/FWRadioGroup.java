package com.sometrik.framework;

import android.content.Context;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.RadioButton;
import android.widget.RadioGroup;

public class FWRadioGroup extends RadioGroup implements NativeCommandHandler {
  
  private FrameWork frame;
  
  public FWRadioGroup(FrameWork frameWork){
    super(frameWork);
    this.frame = frameWork;

    setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener(){
      @Override
      public void onCheckedChanged(RadioGroup group, int checkedId) {
	frame.intChangedEvent(System.currentTimeMillis() / 1000.0, checkedId, 1, 0);
      }
    });
  }

  @Override
  public void addChild(View view) {
    System.out.println("FWRadioGroup couldn't handle command");
  }
  
  @Override
  public void addOption(int optionId, String text) {
    RadioButton button = new RadioButton(frame);
    button.setId(optionId);
    button.setText(text);
    addView(button);
  }

  @Override
  public void setValue(String v) {
    System.out.println("FWRadioGroup couldn't handle command");
  }

  @Override
  public void setValue(int v) {
    System.out.println("FWRadioGroup couldn't handle command");
  }

  @Override
  public int getElementId() {
    return getId();
  }

  @Override
  public void setViewEnabled(Boolean enabled) {
    setEnabled(enabled);
  }

  @Override
  public void setStyle(String key, String value) { }

  @Override
  public void setError(boolean hasError, String errorText) { }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    System.out.println("FWRadioGroup couldn't handle command");
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
}
