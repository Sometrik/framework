package com.sometrik.framework;

import android.graphics.Typeface;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.RotateAnimation;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;

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
    setEnabled(enabled);
  }

  @Override
  public void setStyle(String key, String value) {
    if (key.equals("font-size")){
      if (value.equals("small")){
	this.setTextAppearance(frame, android.R.style.TextAppearance_DeviceDefault_Small);
      } else if (value.equals("medium")){
	this.setTextAppearance(frame, android.R.style.TextAppearance_DeviceDefault_Medium);
      } else if (value.equals("large")){
	this.setTextAppearance(frame, android.R.style.TextAppearance_DeviceDefault_Large);
      }
    } else if (key.equals("gravity")) {
      Log.d("button", "setting gravity: ");
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      params.weight = 1;
      if (value.equals("bottom")) {
	params.gravity = Gravity.BOTTOM;
	      Log.d("button", " to bottom");
      } else if (value.equals("top")) {
	params.gravity = Gravity.TOP;
      } else if (value.equals("left")) {
	params.gravity = Gravity.LEFT;
      } else if (value.equals("right")) {
	params.gravity = Gravity.RIGHT;
      }
      setLayoutParams(params);
    } else if (key.equals("width")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      if (value.equals("wrap_content")) {
	params.width = LinearLayout.LayoutParams.WRAP_CONTENT;
      } else if (value.equals("match_parent")) {
	params.width = LinearLayout.LayoutParams.MATCH_PARENT;
      }
      setLayoutParams(params);
    } else if (key.equals("height")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      if (value.equals("wrap_content")) {
	params.height = LinearLayout.LayoutParams.WRAP_CONTENT;
      } else if (value.equals("match_parent")) {
	params.height = LinearLayout.LayoutParams.MATCH_PARENT;
      }
      setLayoutParams(params);
    } else if (key.equals("add_weight")){
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      params.weight += 1;
      setLayoutParams(params);
      System.out.println("button weight: " + params.weight);
    }
  }

  @Override
  public void setError(boolean hasError, String errorText) { }


  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub
    
  }


  @Override
  public void addData(String text, int row, int column, int sheet) {
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


  @Override
  public void clear() {
    System.out.println("couldn't handle command");
  }
}
