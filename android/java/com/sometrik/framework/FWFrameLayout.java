package com.sometrik.framework;

import com.sometrik.framework.NativeCommand.Selector;

import android.view.MotionEvent;
import android.view.View;
import android.widget.FrameLayout;

public class FWFrameLayout extends FrameLayout implements NativeCommandHandler {
  private FrameWork frame;
  private ViewStyleManager normalStyle, activeStyle, currentStyle;
  
  public FWFrameLayout(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;
    
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);

    final FWFrameLayout layout = this;
    
    setOnTouchListener(new OnTouchListener() {
      @Override
      public boolean onTouch(View v, MotionEvent event) {
	if (event.getAction() == MotionEvent.ACTION_DOWN) {
	  layout.currentStyle = layout.activeStyle;
	  layout.currentStyle.apply(layout);
	} else { // if (event.getAction() == MotionEvent.ACTION_UP)
	  layout.currentStyle = layout.normalStyle;
	  layout.currentStyle.apply(layout);
	}
	return false;
      }
    });
  }
  
  @Override
  public int getElementId() {
    return getId();
  }
 
  @Override
  public void addChild(final View view) {
    addView(view);
  }  
 
  @Override
  public void addOption(int optionId, String text) { }

  @Override
  public void setValue(String v) { }

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
      this.setVisibility(GONE);
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
  public void setImage(byte[] bytes, int width, int height, int internalFormat) { }

  @Override
  public void reshape(int size) { }

  @Override
  public void deinitialize() { }
}
