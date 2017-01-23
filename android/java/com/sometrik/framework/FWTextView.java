package com.sometrik.framework;

import android.content.Context;
import android.view.View;
import android.widget.TextView;

public class FWTextView extends TextView implements NativeCommandHandler {

  private FrameWork frame;
  
  public FWTextView(FrameWork frame) {
    super(frame);
    this.frame = frame;
  }

  @Override
  public void addChild(View view) {
    System.out.println("FWTextView couldn't handle command");
  }

  @Override
  public void removeChild(int id) {
    System.out.println("FWTextView couldn't handle command");
  }

  @Override
  public void addOption(int optionId, String text) {
    System.out.println("FWTextView couldn't handle command");
  }

  @Override
  public void setValue(String v) {
    setText(v);
  }

  @Override
  public void setValue(int v) {
    System.out.println("FWTextView couldn't handle command");
  }

  @Override
  public void setViewEnabled(Boolean enabled) {
    setEnabled(enabled);
  }

  @Override
  public void setStyle(String key, String value) {
    // TODO
  }

  @Override
  public void setError(boolean hasError, String errorText) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public int getElementId() {
    return getId();
  }

}
