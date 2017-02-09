package com.sometrik.framework;

import android.content.Context;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.view.animation.RotateAnimation;
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
  public void addOption(int optionId, String text) {
    System.out.println("FWButton couldn't handle command");
  }

  @Override
  public void setValue(String v) {
    setText(v);
    //FIXME Debug animation
    RotateAnimation r = new RotateAnimation(-5f, 5f,50,50); 
    r.setDuration(100);
    r.setRepeatCount(10);
    r.setRepeatMode(RotateAnimation.REVERSE);
    startAnimation(r);
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
  public void setViewEnabled(Boolean enabled) {
    setEnabled(true);
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
  public void addData(int rowNumber, int columnNumber, String text) {
    System.out.println("FWButton couldn't handle command");
  }

  @Override
  public void setViewVisibility(boolean visibility) {
    if (visibility){
      this.setVisibility(VISIBLE);
    } else {
      this.setVisibility(INVISIBLE);
    }
  }
}
