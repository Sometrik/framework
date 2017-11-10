package com.sometrik.framework;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.TranslateAnimation;
import android.widget.FrameLayout;

public class FWFrameView extends FrameLayout implements NativeCommandHandler {

  FrameWork frame;
  private String title;
  private Animation enterAnimation = null;
  private int enterAnimationOtherViewFromX = 0;
  private Animation returnAnimation = null;
  private int returnAnimationOtherViewFromX = 0;
  private ViewStyleManager normalStyle, activeStyle, currentStyle;
  
  public FWFrameView(FrameWork frameWork, String title) {
    super(frameWork);
    this.frame = frameWork;
    this.title = title;
    
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
  }
  
  public FWFrameView(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;
    // LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
    // this.setLayoutParams(params);
    // this.setFillViewport(true);
    
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
  }
  
  @Override
  public void onVisibilityChanged(View changedView, int visibility) {
    switch (visibility) {
    case VISIBLE:
      frame.visibilityChangedEvent(getId(), true);
      break;
    case GONE:
    case INVISIBLE: 
      frame.visibilityChangedEvent(getId(), false);
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
  public void setValue(String v) { }

  @Override
  public void setValue(int v) {
    System.out.println("FWFrameView setValue " + v);
    if (v == 1){
      if (enterAnimation != null) {
	frame.setCurrentView(this, true, enterAnimation, enterAnimationOtherViewFromX);
      } else {
	frame.setCurrentView(this, true, title);
      }
    } else if (v == 2) {
      if (enterAnimation != null) {
	frame.setCurrentView(this, false, enterAnimation, enterAnimationOtherViewFromX);
      } else {
	frame.setCurrentView(this, false, title);
      }
    } else if (v == 3) {
      if (returnAnimation != null) {
	System.out.println("FWFrameView return animation");
	frame.setCurrentView(this, false, returnAnimation, returnAnimationOtherViewFromX);
      } else {
	System.out.println("FWFrameView no animation");
	frame.setCurrentView(this, false, title);
      }
    }
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
    
    System.out.println("FWFrameView style " + key + " " + value);
    
    if (returnAnimation == null) {
      
    } else {
      System.out.println("FWFrameView got animation");
    }
    
    if (key.equals("enter-animation")) {
      if (value.equals("from-right")) {
	enterAnimationOtherViewFromX = 2000;
	TranslateAnimation r;
	r = new TranslateAnimation(0, -2000, 0, 0);
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
    } else if (key.equals("return-animation")) {
      if (value.equals("from-left")) {
	returnAnimationOtherViewFromX = -2000;
	TranslateAnimation r;
	r = new TranslateAnimation(0, 2000, 0, 0);
	r.setDuration(100);
	returnAnimation = r;
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
  }
  
  @Override
  public void setBitmap(Bitmap bitmap) { }

  @Override
  public void reshape(int size) {
  }

  @Override
  public void deinitialize() {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addImageUrl(String url, int width, int height) {
    // TODO Auto-generated method stub
    
  }
}
