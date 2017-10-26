package com.sometrik.framework;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap.Config;
import android.graphics.Color;
import android.support.v4.widget.DrawerLayout;
import android.view.Gravity;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.LinearLayout;

public class NavigationLayout extends FrameLayout implements NativeCommandHandler {

  FrameWork frame;
  ViewStyleManager normalStyle, activeStyle, currentStyle;
  LinearLayout base;

  public NavigationLayout(FrameWork frame, int id) {
    super(frame);
    this.frame = frame;
    this.setId(id);;
    
    DrawerLayout.LayoutParams lp = new DrawerLayout.LayoutParams(LinearLayout.LayoutParams.FILL_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
    lp.gravity = Gravity.START;
    setLayoutParams(lp);
    
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
  }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    invalidate();
  }

  @Override
  public void onVisibilityChanged(View changedView, int visibility) {
    switch (visibility) {
    case VISIBLE:
      System.out.println("DRAWER java: visible");
      frame.visibilityChangedEvent(System.currentTimeMillis() / 1000.0, getId(), true);
      break;
    case GONE:
    case INVISIBLE:
      System.out.println("DRAWER java: invisible");
      frame.visibilityChangedEvent(System.currentTimeMillis() / 1000.0, getId(), false);
      break;
    }
  }

  @Override
  public void addChild(View view) {
    addView(view);
  }

  @Override
  public void addOption(int optionId, String text) { }

  @Override
  public void addData(String text, int row, int column, int sheet) { }

  @Override
  public void setValue(String v) {
    if (v.equals("toggle")) {
      if (getVisibility() == VISIBLE) {
	frame.getDrawerLayout().setDrawerVisibility(this, false);
      } else {
	frame.getDrawerLayout().setDrawerVisibility(this, true);
      }
    }
  }

  @Override
  public void setValue(int v) {
    if (v == 0) {
      frame.setCurrentDrawerViewId(0);
    } else {
      frame.setCurrentDrawerViewId(getId());
    }
  }

  @Override
  public void setViewVisibility(boolean visible) {
    frame.getDrawerLayout().setDrawerVisibility(this, visible);
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
  }

  @Override
  public void setError(boolean hasError, String errorText) { }

  @Override
  public void clear() { }

  @Override
  public int getElementId() {
    return getId();
  }

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
