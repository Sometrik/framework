package com.sometrik.framework;

import java.io.IOException;
import java.io.InputStream;

import android.content.res.AssetManager;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.ScaleDrawable;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.animation.RotateAnimation;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;

public class FWButton extends Button implements NativeCommandHandler {

  FrameWork frame;
  BitmapDrawable leftDraw;
  BitmapDrawable rightDraw;
  BitmapDrawable bottomDraw;
  BitmapDrawable topDraw;
  
  public FWButton(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;
    this.setBackground(frame.getResources().getDrawable(android.R.drawable.dialog_holo_light_frame));
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
    System.out.println("Button style " + key + " " + value);
    if (key.equals("font-size")){
      if (value.equals("small")){
	this.setTextSize(9);
      } else if (value.equals("medium")){
	this.setTextSize(12);
      } else if (value.equals("large")){
	this.setTextSize(15);
      } else {
	setTextSize(Integer.parseInt(value));
      }
    } else if (key.equals("gravity")) {
      Log.d("button", "setting gravity: ");
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
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
      if (value.equals("wrap-content")) {
	params.width = LinearLayout.LayoutParams.WRAP_CONTENT;
      } else if (value.equals("match-parent")) {
	params.width = LinearLayout.LayoutParams.MATCH_PARENT;
      }
      setLayoutParams(params);
    } else if (key.equals("height")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      if (value.equals("wrap-content")) {
	params.height = LinearLayout.LayoutParams.WRAP_CONTENT;
      } else if (value.equals("match-parent")) {
	params.height = LinearLayout.LayoutParams.MATCH_PARENT;
      }
      setLayoutParams(params);
    } else if (key.equals("weight")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      params.weight = Integer.parseInt(value);
      setLayoutParams(params);
      System.out.println("button weight: " + params.weight);
    } else if (key.equals("pressed")) {
      if (value.equals("true") || value.equals("1")) {
	this.setPressed(true);
	this.setTextColor(Color.RED);
      } else {
	this.setTextColor(Color.BLACK);
	this.setPressed(false);
      }
    } else if (key.equals("icon-left") || key.equals("icon-right") || key.equals("icon-top") || key.equals("icon-bottom")){
      AssetManager mgr = frame.getAssets();
      try {
        InputStream stream = mgr.open(value);
        BitmapDrawable draw = new BitmapDrawable(stream);
        this.setGravity(Gravity.CENTER);
      if (key.equals("icon-right")){
        rightDraw = draw;
      } else if (key.equals("icon-top")){
        topDraw = draw;
      } else if (key.equals("icon-bottom")){
        bottomDraw = draw;
      } else if (key.equals("icon-left")){
	leftDraw = draw;
      }
      this.setCompoundDrawablesWithIntrinsicBounds(leftDraw, topDraw, rightDraw, bottomDraw);
      } catch (IOException e) {
        System.out.println("no picture found: " + value);
        e.printStackTrace();
      }
    } else if (key.equals("borderless")) {
      setBackgroundResource(0);
    } else if (key.equals("single-line")) {
      this.setSingleLine();
    } else if (key.equals("text-color")) {
      setTextColor(Color.parseColor(value));
    } else if (key.equals("color")) {
      setBackgroundColor(Color.parseColor(value));
    } else if (key.equals("padding-top")) {
      setPadding(getPaddingLeft(), Integer.parseInt(value), getPaddingRight(), getPaddingBottom());
    } else if (key.equals("padding-bottom")) {
      setPadding(getPaddingLeft(), getPaddingTop(), getPaddingRight(), Integer.parseInt(value));
    } else if (key.equals("padding-left")) {
      setPadding(Integer.parseInt(value), getPaddingTop(), getPaddingRight(), getPaddingBottom());
    } else if (key.equals("padding-right")) {
      setPadding(getPaddingLeft(), getPaddingTop(), Integer.parseInt(value), getPaddingBottom());
    } else if (key.equals("margin-right")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      params.rightMargin = Integer.parseInt(value);
      setLayoutParams(params);
    } else if (key.equals("margin-left")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      params.leftMargin = Integer.parseInt(value);
      setLayoutParams(params);
    } else if (key.equals("margin-top")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      params.topMargin = Integer.parseInt(value);
      setLayoutParams(params);
    } else if (key.equals("margin-bottom")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      params.bottomMargin = Integer.parseInt(value);
      setLayoutParams(params);
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


  @Override
  public void setImage(byte[] bytes, int width, int height) {
    // TODO Auto-generated method stub
    
  }


  @Override
  public void reshape(int size) {
    // TODO Auto-generated method stub
    
  }
}
