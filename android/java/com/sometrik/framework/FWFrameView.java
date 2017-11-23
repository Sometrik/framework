package com.sometrik.framework;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.TranslateAnimation;
import android.widget.FrameLayout;

public class FWFrameView extends FrameLayout implements NativeCommandHandler {

  FrameWork frame;
  private Animation enterAnimation = null;
  private int enterAnimationOtherViewFromX = 0;
  private Animation returnAnimation = null;
  private int returnAnimationOtherViewFromX = 0;
  private ViewStyleManager normalStyle, activeStyle, currentStyle;
 
  public FWFrameView(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;
    
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
  }
  
  @Override
  public boolean performClick() {
    frame.sendNativeValueEvent(getElementId(), 0, 0);
    return super.performClick();
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
    frame.hideSoftKeyboard();

    if (v == 1) {
      if (enterAnimation != null) {
	frame.setCurrentView(this, true, enterAnimation, enterAnimationOtherViewFromX);
      } else {
	frame.setCurrentView(this, true);
      }
    } else if (v == 2) {
      if (enterAnimation != null) {
	frame.setCurrentView(this, false, enterAnimation, enterAnimationOtherViewFromX);
      } else {
	frame.setCurrentView(this, false);
      }
    } else if (v == 3) {
      if (returnAnimation != null) {
	frame.setCurrentView(this, false, returnAnimation, returnAnimationOtherViewFromX);
      } else {
	frame.setCurrentView(this, false);
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
        
    if (returnAnimation == null) {
      
    } else {
    
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
	      frame.setCurrentView(holderView, true);
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
	      frame.setCurrentView(holderView, true);
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
    if (visibility) {
      frame.hideSoftKeyboard();
 
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
  public void setBitmap(Bitmap bitmap) { }

  @Override
  public void reshape(int size) { }

  @Override
  public void deinitialize() { }

  @Override
  public void addImageUrl(String url, int width, int height) { }
}
