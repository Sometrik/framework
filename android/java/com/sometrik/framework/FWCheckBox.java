package com.sometrik.framework;

import android.content.Context;
import android.text.TextUtils.TruncateAt;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsListView;
import android.widget.CheckBox;

public class FWCheckBox extends CheckBox implements NativeCommandHandler {

  FrameWork frame;
  
  public FWCheckBox(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;
//    this.setMaxLines(1);
//    this.setEllipsize(TruncateAt.MARQUEE);
//    this.setHorizontallyScrolling(true);
    
    ViewGroup.LayoutParams params = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
    this.setLayoutParams(params);
  }

  @Override
  public void addChild(View view) {
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
  public void setViewEnabled(Boolean enabled) {
    setEnabled(enabled);
  }

  @Override
  public void setStyle(String key, String value) { }

  @Override
  public void setError(boolean hasError, String errorText) { }

  @Override
  public int getElementId() {
    return getId();
  }
}
