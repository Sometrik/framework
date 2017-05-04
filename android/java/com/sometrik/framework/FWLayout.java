package com.sometrik.framework;

import android.content.Context;
import android.view.Gravity;
import android.view.View;
import android.view.ViewParent;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;

public class FWLayout extends LinearLayout implements NativeCommandHandler {
  
  private FrameWork frame;
  
  public FWLayout(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;
  }

  @Override
  public int getElementId() {
    return getId();
  }
 
  @Override
  public void addChild(View view) {
    addView(view);
  }

  @Override
  public void addOption(int optionId, String text) {
    System.out.println("FWLayout couldn't handle command");
  }

  @Override
  public void setValue(String v) {
    System.out.println("FWLayout couldn't handle command");
  }

  @Override
  public void setValue(int v) {
    if (v == 1){
      frame.setCurrentView(this, true);
    } else if (v == 2){
      frame.setCurrentView(this, false);
    }
  }

  @Override
  public void setViewEnabled(Boolean enabled) {
    System.out.println("FWLayout couldn't handle command");
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
  public void setError(boolean hasError, String errorText) { }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    invalidate();
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    System.out.println("FWLayout couldn't handle command");
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
