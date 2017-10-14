package com.sometrik.framework;

import java.io.IOException;
import java.io.InputStream;

import com.sometrik.framework.NativeCommand.Selector;

import android.content.res.AssetManager;
import android.content.res.ColorStateList;
import android.graphics.Bitmap.Config;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.GradientDrawable;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.RotateAnimation;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;

public class FWButton extends Button implements NativeCommandHandler {
  private FrameWork frame;
  private Animation animation = null;
  private ViewStyleManager normalStyle, activeStyle, selectedStyle;
  private ViewStyleManager currentStyle;
  private boolean isSelected = false;
      
  public FWButton(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;

    BitmapCache bitmapCache = frameWork.bitmapCache;
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(bitmapCache, scale, false);
    this.selectedStyle = new ViewStyleManager(bitmapCache, scale, false);
    
    final FWButton button = this;
    
    setOnClickListener(new OnClickListener() {
      @Override
      public void onClick(View arg0) {
	if (!FrameWork.transitionAnimation) {
	  frame.intChangedEvent(System.currentTimeMillis() / 1000.0, getElementId(), 1, 0);
	  if (animation != null) button.startAnimation(animation);
	}
      }
    });
    
    setOnTouchListener(new OnTouchListener() {
      @Override
      public boolean onTouch(View v, MotionEvent event) {
	if (event.getAction() == MotionEvent.ACTION_DOWN) {
	  button.currentStyle = button.activeStyle;
	  button.currentStyle.apply(button);
	} else if (event.getAction() == MotionEvent.ACTION_UP) {
	  button.currentStyle = isSelected ? button.selectedStyle : button.normalStyle;
	  button.currentStyle.apply(button);
	}
	return false;
      }
    });
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
  }
  
  @Override
  public void setValue(int v) {
    boolean b = v != 0;
    if (b != isSelected) {  
      isSelected = b;
      if (isSelected) currentStyle = selectedStyle;
      else currentStyle = normalStyle;
      currentStyle.apply(his);
    }
  }

  @Override
  public int getElementId() {
    return getId();
  }

  @Override
  public void setStyle(Selector selector, String key, String value) {
    if (selector == Selector.NORMAL) {
      normalStyle.setStyle(key, value);
      if (normalStyle == currentStyle) normalStyle.apply(this);
    } else if (selector == Selector.ACTIVE) {
      activeStyle.setStyle(key, value);      
      if (activeStyle == currentStyle) activeStyle.apply(this);
    } else if (selector == Selector.SELECTED) {
      selectedStyle.setStyle(key, value);      
      if (selectedStyle == currentStyle) selectedStyle.apply(this);
    }
    
    if (key.equals("animation")) {
      if (value.equals("rotate")) {
	RotateAnimation r = new RotateAnimation(-5f, 5f,50,50); 
	r.setDuration(100);
	r.setRepeatCount(10);
	r.setRepeatMode(RotateAnimation.REVERSE);
	animation = r;
      }
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
  public void setImage(byte[] bytes, int width, int height, Config config) {
    // TODO Auto-generated method stub
    
  }


  @Override
  public void reshape(int size) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void deinitialize() {
    // TODO Auto-generated method stub
    
  }
}
