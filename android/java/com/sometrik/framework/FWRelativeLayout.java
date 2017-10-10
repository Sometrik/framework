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

  
  public FWRelativeLayout(FrameWork frame) {
    super(frame);
    this.frame = frame;
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
    for (int i = 0; i < getChildCount(); i++) {
      if (optionId == getChildAt(i).getId()) {
	System.out.println("relative: child found");
	View view = getChildAt(i);
	LayoutParams params = (LayoutParams) view.getLayoutParams();
	if (text.equals("parent-left")) {
	  params.addRule(RelativeLayout.ALIGN_PARENT_LEFT);
	}
      }
    }
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
  public void setViewEnabled(Boolean enabled) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setViewVisibility(boolean visible) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setStyle(Selector selector, String key, String value) {
    if (key.equals("gravity")) {
      LinearLayout.LayoutParams params = (LinearLayout.LayoutParams) getLayoutParams();
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
      if (getParent() instanceof ScrollView) {
	FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
	this.setLayoutParams(params);
      } else {
	      LinearLayout.LayoutParams params = (LinearLayout.LayoutParams) getLayoutParams();
	      if (value.equals("wrap-content")) {
		params.width = LinearLayout.LayoutParams.WRAP_CONTENT;
	      } else if (value.equals("match-parent")) {
		params.width = LinearLayout.LayoutParams.MATCH_PARENT;
	      } else {
		final float scale = getContext().getResources().getDisplayMetrics().density;
		int pixels = (int) (Integer.parseInt(value) * scale + 0.5f);
		params.width = pixels;
	      }
	      setLayoutParams(params);
      }
    } else if (key.equals("height")) {
      if (getParent() instanceof ScrollView) {
	FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
	this.setLayoutParams(params);
      } else {
	LinearLayout.LayoutParams params = (LinearLayout.LayoutParams) getLayoutParams();
	if (value.equals("wrap-content")) {
	  params.height = LinearLayout.LayoutParams.WRAP_CONTENT;
	} else if (value.equals("match-parent")) {
	  params.height = LinearLayout.LayoutParams.MATCH_PARENT;
	} else {

	  final float scale = getContext().getResources().getDisplayMetrics().density;
	  int pixels = (int) (Integer.parseInt(value) * scale + 0.5f);
	  params.height = pixels;
	}
	setLayoutParams(params);
      }
    } else if (key.equals("weight")) {
      LinearLayout.LayoutParams params = (LinearLayout.LayoutParams) getLayoutParams();
      params.weight = Integer.parseInt(value);
      setLayoutParams(params);
    } else if (key.equals("padding-top")) {
      setPadding(getPaddingLeft(), Integer.parseInt(value), getPaddingRight(), getPaddingBottom());
    } else if (key.equals("padding-bottom")) {
      setPadding(getPaddingLeft(), getPaddingTop(), getPaddingRight(), Integer.parseInt(value));
    } else if (key.equals("padding-left")) {
      setPadding(Integer.parseInt(value), getPaddingTop(), getPaddingRight(), getPaddingBottom());
    } else if (key.equals("padding-right")) {
      setPadding(getPaddingLeft(), getPaddingTop(), Integer.parseInt(value), getPaddingBottom());
    } else if (key.equals("frame")) {
      if (value.equals("light")) {
	this.setBackgroundDrawable(frame.getResources().getDrawable(android.R.drawable.dialog_holo_light_frame));
//	this.setDividerDrawable(frame.getResources().getDrawable(android.R.drawable.divider_horizontal_bright));
      } else if (value.equals("dark")) {
	this.setBackgroundDrawable(frame.getResources().getDrawable(android.R.drawable.alert_light_frame));
      }
    } else if (key.equals("background-color")) {
      this.setBackgroundColor(Color.parseColor(value));
    } else if (key.equals("margin-right")) {
      LinearLayout.LayoutParams params = (LinearLayout.LayoutParams) getLayoutParams();
      params.rightMargin = Integer.parseInt(value);
      setLayoutParams(params);
    } else if (key.equals("margin-left")) {
      LinearLayout.LayoutParams params = (LinearLayout.LayoutParams) getLayoutParams();
      params.leftMargin = Integer.parseInt(value);
      setLayoutParams(params);
    } else if (key.equals("margin-top")) {
      LinearLayout.LayoutParams params = (LinearLayout.LayoutParams) getLayoutParams();
      params.topMargin = Integer.parseInt(value);
      setLayoutParams(params);
    } else if (key.equals("margin-bottom")) {
      LinearLayout.LayoutParams params = (LinearLayout.LayoutParams) getLayoutParams();
      params.bottomMargin = Integer.parseInt(value);
      setLayoutParams(params);
    } else if (key.equals("shadow")) {
      setElevation(Integer.parseInt(value));
    } else if (key.equals("border")) {
      if (value.equals("none")) {
	setBackgroundResource(0);
      } else {
	GradientDrawable gd = createBackground();
	gd.setStroke(1, Color.parseColor(value));
      }
    } else if (key.equals("border-radius")) {
      final float scale = getContext().getResources().getDisplayMetrics().density;
      int pixels = (int) (Integer.parseInt(value) * scale + 0.5f);
      GradientDrawable gd = createBackground();
      gd.setCornerRadius(pixels);      
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

}
