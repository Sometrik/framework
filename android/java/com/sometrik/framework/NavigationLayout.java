package com.sometrik.framework;

import android.graphics.Bitmap.Config;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.support.v4.widget.DrawerLayout;
import android.view.Gravity;
import android.view.View;
import android.widget.LinearLayout;

public class NavigationLayout extends LinearLayout implements NativeCommandHandler {

  FrameWork frame;
  ViewStyleManager normalStyle, activeStyle, currentStyle;

  public NavigationLayout(FrameWork frame) {
    super(frame);
    this.frame = frame;

    DrawerLayout.LayoutParams lp = new DrawerLayout.LayoutParams(LinearLayout.LayoutParams.FILL_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
    lp.gravity = Gravity.START;
    setLayoutParams(lp);
    setBackgroundColor(Color.parseColor("#dddbd6"));
    // setBackground(frame.getResources().getDrawable(android.R.drawable.dialog_holo_light_frame));
    ColorDrawable cd = new ColorDrawable();
    cd.setColor(Color.WHITE);
    setBackgroundDrawable(cd);
    
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(scale, true);
    this.activeStyle = new ViewStyleManager(scale, false);
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
  public void addOption(int optionId, String text) {
    // TODO Auto-generated method stub

  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    // TODO Auto-generated method stub

  }

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
    System.out.println("Drawer visibility");
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
    
    if (key.equals("add-weight")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      params.weight += 1;
      setLayoutParams(params);
    }
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
  public int getElementId() {
    return getId();
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
