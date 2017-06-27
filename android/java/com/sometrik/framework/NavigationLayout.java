package com.sometrik.framework;

import android.graphics.Color;
import android.support.v4.widget.DrawerLayout;
import android.view.Gravity;
import android.view.View;
import android.widget.LinearLayout;

public class NavigationLayout extends LinearLayout implements NativeCommandHandler {

  FrameWork frame;
  
  public NavigationLayout(FrameWork frame) {
    super(frame);
    this.frame = frame;

    DrawerLayout.LayoutParams lp = new DrawerLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
    lp.gravity = Gravity.START;
    setLayoutParams(lp);
    setBackgroundColor(Color.parseColor("#dddbd6"));
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
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setValue(int v) {
    if (v == 0){
      frame.setCurrentDrawerViewId(0);
    } else {
      frame.setCurrentDrawerViewId(getId());
    }
  }

  @Override
  public void setViewEnabled(Boolean enabled) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setViewVisibility(boolean visible) {
    System.out.println("Drawer visibility");
    frame.getDrawerLayout().setDrawerVisibility(this, visible);
  }

  @Override
  public void setStyle(String key, String value) {

    if (key.equals("gravity")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      if (value.equals("bottom")) {
	params.gravity = Gravity.BOTTOM;
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
    } else if (key.equals("add_weight")) {
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

}
