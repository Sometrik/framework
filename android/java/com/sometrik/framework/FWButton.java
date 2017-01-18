package com.sometrik.framework;

import android.content.Context;
import android.view.View;
import android.widget.Button;

public class FWButton extends Button implements NativeCommandHandler {

  FrameWork frame;
  
  public FWButton(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;
    
  }


  @Override
  public void addChild(View view) {
    System.out.println("FWButton couldn't handle command");
  }
  
  @Override
  public void removeChild(int id) {
    System.out.println("FWButton couldn't handle command");
  }

  @Override
  public void showView() {
    System.out.println("FWButton couldn't handle command");
  }

  @Override
  public void addOption(int optionId, String text) {
    System.out.println("FWButton couldn't handle command");
  }

  @Override
  public void setValue(String v) {
    setText(v);
  }
  
  @Override
  public void setValue(int v) {
    System.out.println("FWButton couldn't handle command");
  }

  @Override
  public int getElementId() {
    return getId();
  }

  @Override
  public void setEnabled(Boolean enabled) {
    System.out.println("FWButton couldn't handle command");
  }

}
