package com.sometrik.framework;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Color;
import android.graphics.Bitmap.Config;
import android.graphics.drawable.GradientDrawable;
import android.view.Gravity;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.ScrollView;

public class FWRelativeLayout extends RelativeLayout implements NativeCommandHandler {

  private FrameWork frame;
  private GradientDrawable currentBackground = null;
  private ViewStyleManager normalStyle, activeStyle, currentStyle;
  
  public FWRelativeLayout(FrameWork frame) {
    super(frame);
    this.frame = frame;
    
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
  }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addChild(View view) {
    addView(view);
  }

  @Override
  public void addOption(int optionId, String text) {
//    for (int i = 0; i < getChildCount(); i++) {
//      if (optionId == getChildAt(i).getId()) {
//	System.out.println("relative: child found");
//	View view = getChildAt(i);
//	LayoutParams params = (LayoutParams) view.getLayoutParams();
//	if (text.equals("parent-left")) {
//	  params.addRule(RelativeLayout.ALIGN_PARENT_LEFT);
//	}
//      }
//    }
  }

  @Override
  public void addColumn(String text, int columnType) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setValue(String v) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setImage(byte[] bytes, int width, int height, Config config) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setValue(int v) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void reshape(int value, int size) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void reshape(int size) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setViewVisibility(boolean visible) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setStyle(Selector selector, String key, String value) {
    if (selector == Selector.NORMAL) {
      normalStyle.setStyle(key, value);
      if (normalStyle == currentStyle) normalStyle.apply(this);
    } else if (selector == Selector.ACTIVE) {
      activeStyle.setStyle(key, value);      
      if (activeStyle == currentStyle) activeStyle.apply(this);
    }
    
    if (key.equals("frame")) {
      if (value.equals("light")) {
	this.setBackgroundDrawable(frame.getResources().getDrawable(android.R.drawable.dialog_holo_light_frame));
//	this.setDividerDrawable(frame.getResources().getDrawable(android.R.drawable.divider_horizontal_bright));
      } else if (value.equals("dark")) {
	this.setBackgroundDrawable(frame.getResources().getDrawable(android.R.drawable.alert_light_frame));
      }
    }
  }
  
  private GradientDrawable createBackground() {
    if (currentBackground == null) {
      currentBackground = new GradientDrawable();
      setBackground(currentBackground);
    }
    currentBackground.setColor(Color.parseColor("#ffffff")); // Changes this drawable to use a single color instead of a gradient
    return currentBackground;
  }  

  @Override
  public void setError(boolean hasError, String errorText) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void clear() {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void flush() {
    // TODO Auto-generated method stub
    
  }

  @Override
  public int getElementId() {
    return getId();
  }

  @Override
  public void deinitialize() {
    // TODO Auto-generated method stub
    
  }

}
