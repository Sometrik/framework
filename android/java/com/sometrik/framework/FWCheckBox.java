package com.sometrik.framework;

import android.content.Context;
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
  public void removeChild(int id) {
    System.out.println("FWCheckBox couldn't handle command");
  }

  @Override
  public void showView() {
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
  public void setEnabled(Boolean enabled) {
    setEnabled(enabled);
  }

  @Override
  public int getElementId() {
    return getId();
  }

}
