package com.sometrik.framework;

import android.content.Context;
import android.view.View;
import android.widget.Switch;

public class FWSwitch extends Switch implements NativeCommandHandler {

  FrameWork frame;
  
  public FWSwitch(FrameWork frame) {
    super(frame);
    this.frame = frame;
  }

  @Override
  public void addChild(View view) {
    System.out.println("FWSwitch coulnd't handle Command");
  }

  @Override
  public void addOption(int optionId, String text) {
    System.out.println("FWSwitch coulnd't handle Command");
  }

  @Override
  public void setValue(String v) {
    setText(v);
  }

  @Override
  public void setValue(int v) {
    if (v > 0){
      this.setSelected(true);
    } else {
      this.setSelected(false);
    }
  }

  @Override
  public void setViewEnabled(Boolean enabled) {
    setEnabled(enabled);
  }

  @Override
  public void setStyle(String key, String value) {
    System.out.println("FWSwitch coulnd't handle Command");
  }

  @Override
  public void setError(boolean hasError, String errorText) {
    System.out.println("FWSwitch coulnd't handle Command");
  }

  @Override
  public int getElementId() {
    return getId();
  }

}
