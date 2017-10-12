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

public class FWScrollView extends ScrollView implements NativeCommandHandler {

  FrameWork frame;
  private String title;
  private int maxHeight = 0;
  private Animation enterAnimation = null;
  
  public FWScrollView(FrameWork frameWork, String title) {
    super(frameWork);
    this.frame = frameWork;
    this.title = title;
    LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
    this.setLayoutParams(params);
    this.setFillViewport(true);
  }
  
  public FWScrollView(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;
    LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
    this.setLayoutParams(params);
    this.setFillViewport(true);
  }
  
  @Override
  public void onVisibilityChanged(View changedView, int visibility) {
    switch (visibility) {
    case VISIBLE:
      frame.visibilityChangedEvent(System.currentTimeMillis() / 1000.0, getId(), true);
      break;
    case GONE:
    case INVISIBLE: 
      frame.visibilityChangedEvent(System.currentTimeMillis() / 1000.0, getId(), false);
    break;
    }
  }

  @Override
  public void addChild(View view) {
    if (this.getChildCount() > 0) {
      System.out.println("ScrollView already has a child");
    } else {
      addView(view);
    }
  }
  
  @Override
  public void addOption(int optionId, String text) {
    System.out.println("FWScrollView couldn't handle command");
  }

  @Override
  public void setValue(String v) {
    System.out.println("FWScrollView couldn't handle command");
  }

  @Override
  public void setValue(int v) {
    if (v == 1){
      if (enterAnimation != null) {
	System.out.println("animation commencing");
	frame.setCurrentView(this, true, enterAnimation);
      } else {
	frame.setCurrentView(this, true, title);
      }
    } else if (v == 2){
      frame.setCurrentView(this, false, title);
    }
  }

  @Override
  public void setStyle(Selector selector, String key, String value) {
    if (key.equals("background-color")) {
      this.setBackgroundColor(Color.parseColor(value));
    } else if (key.equals("animation")) {
      if (value.equals("from-left")) {
	TranslateAnimation r;
	// if (recordHistory) {
	// r = new TranslateAnimation(0, -2000, 0, 0);
	// } else {
	r = new TranslateAnimation(0, -1000, 0, 0);
	// }
	r.setDuration(100);
	enterAnimation = r;
	final View holderView = this;
	r.setAnimationListener(new Animation.AnimationListener() {
	  @Override
	  public void onAnimationStart(Animation animation) {
	  }

	  @Override
	  public void onAnimationEnd(Animation animation) {
	      frame.setCurrentView(holderView, true, title);
	  }

	  @Override
	  public void onAnimationRepeat(Animation arg0) {
	  }
	});
      }
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
  public void addData(String text, int row, int column, int sheet) {
    System.out.println("FWScrollView couldn't handle command");
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
  protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
    if (maxHeight != 0) {
      heightMeasureSpec = MeasureSpec.makeMeasureSpec(dpToPx(getResources(),maxHeight), MeasureSpec.AT_MOST);
      super.onMeasure(widthMeasureSpec, heightMeasureSpec);
    } else {
      super.onMeasure(widthMeasureSpec, heightMeasureSpec);
    }
  }

  private int dpToPx(Resources res, int dp) {
    return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, dp, res.getDisplayMetrics());
  }

  @Override
  public void reshape(int value, int size) {
    if (getChildCount() > 0) {
      View view = getChildAt(0);
      if (view instanceof NativeCommandHandler) {
	NativeCommandHandler handler = (NativeCommandHandler)view;
	handler.reshape(value, size);
      }
    }
  }
  
  public void setMaxHeight(int maxHeight) {
    this.maxHeight = maxHeight;
  }

  @Override
  public void setImage(byte[] bytes, int width, int height, Config config) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void reshape(int size) {
    if (getChildCount() > 0) {
      View view = getChildAt(0);
      if (view instanceof NativeCommandHandler) {
	NativeCommandHandler handler = (NativeCommandHandler)view;
	handler.reshape(size);
      }
    }
  }

  @Override
  public void deinitialize() {
    // TODO Auto-generated method stub
    
  }
}
