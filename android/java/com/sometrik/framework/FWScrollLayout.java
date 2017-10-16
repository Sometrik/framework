package com.sometrik.framework;

import com.sometrik.framework.NativeCommand.Selector;

import android.content.res.Resources;
import android.graphics.Color;
import android.graphics.Bitmap.Config;
import android.util.TypedValue;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.TranslateAnimation;
import android.widget.LinearLayout;
import android.widget.ScrollView;

public class FWScrollLayout extends ScrollView implements NativeCommandHandler {
  FrameWork frame;
  private ViewStyleManager normalStyle, activeStyle, currentStyle;
    
  public FWScrollLayout(FrameWork frameWork, int id) {
    super(frameWork);
    
    this.setId(id);
    this.frame = frameWork;
    LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
    this.setLayoutParams(params);
    // this.setFillViewport(true);
    
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
  }
  
  @Override
  public void addChild(View view) {
    if (this.getChildCount() > 0) {
      System.out.println("FWScrollLayout already has a child");
    } else {
      addView(view);
    }
  }
  
  @Override
  public void addOption(int optionId, String text) {
  }

  @Override
  public void setValue(String v) {
  }

  @Override
  public void setValue(int v) { }

  @Override
  public void setStyle(Selector selector, String key, String value) {
    if (selector == Selector.NORMAL) {
      normalStyle.setStyle(key, value);
      if (normalStyle == currentStyle) normalStyle.apply(this);
    } else if (selector == Selector.ACTIVE) {
      activeStyle.setStyle(key, value);      
      if (activeStyle == currentStyle) activeStyle.apply(this);
    }
  }

  @Override
  public void setError(boolean hasError, String errorText) { }

  @Override
  public int getElementId() {
    return getId();
  }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    invalidate();
  }

  @Override
  public void addData(String text, int row, int column, int sheet) { }

  @Override
  public void setViewVisibility(boolean visibility) {
    if (visibility){
      this.setVisibility(VISIBLE);
    } else {
      this.setVisibility(INVISIBLE);
    }
  }

  @Override
  public void clear() { }

  @Override
  public void flush() { }

  @Override
  public void addColumn(String text, int columnType) { }
  
  @Override
  public void reshape(int value, int size) { }

  @Override
  public void setImage(byte[] bytes, int width, int height, Config config) { }

  @Override
  public void reshape(int size) { }

  @Override
  public void deinitialize() { }
}