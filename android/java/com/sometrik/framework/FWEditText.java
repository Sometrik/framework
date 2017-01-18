package com.sometrik.framework;

import android.content.Context;
import android.view.View;
import android.widget.EditText;

public class FWEditText extends EditText implements NativeCommandHandler {

  private FrameWork frame;
  
  public FWEditText(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;
  }

  @Override
  public void addChild(View view) {
    System.out.println("FWEditText couldn't handle command");
  }

  @Override
  public void removeChild(int id) {
    System.out.println("FWEditText couldn't handle command");
  }

  @Override
  public void showView() {
    System.out.println("FWEditText couldn't handle command");
  }

  @Override
  public void addOption(int optionId, String text) {
    System.out.println("FWEditText couldn't handle command");
  }

  @Override
  public void setValue(String v) {
    setText(v);
  }

  @Override
  public void setValue(int v) {
    System.out.println("FWEditText couldn't handle command");
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
