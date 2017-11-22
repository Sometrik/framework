package com.sometrik.framework;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap;
import android.view.MotionEvent;
import android.view.View;
import android.widget.FrameLayout;

public class FWFrameLayout extends FrameLayout implements NativeCommandHandler {
  
  private FrameWork frame;
  private ViewStyleManager normalStyle, activeStyle, currentStyle;
  
  public FWFrameLayout(FrameWork frameWork, int id, boolean is_clickable) {
    super(frameWork);
    this.frame = frameWork;
    this.setId(id);
    
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);

    setClickable(is_clickable);
    setFocusable(is_clickable);
      
    if (is_clickable) {      
      final FWFrameLayout layout = this;
    
      setOnTouchListener(new OnTouchListener() {
      @Override
      public boolean onTouch(View v, MotionEvent event) {
	if (event.getAction() == MotionEvent.ACTION_DOWN || event.getAction() == MotionEvent.ACTION_POINTER_DOWN || event.getAction() == MotionEvent.ACTION_HOVER_ENTER) {
	  layout.currentStyle = layout.activeStyle;
	  layout.currentStyle.apply(layout);
	} else if (event.getAction() == MotionEvent.ACTION_UP || event.getAction() == MotionEvent.ACTION_POINTER_UP ||
	    	event.getAction() == MotionEvent.ACTION_HOVER_EXIT || event.getAction() == MotionEvent.ACTION_OUTSIDE ||
	    	event.getAction() == MotionEvent.ACTION_CANCEL) {
	  layout.currentStyle = layout.normalStyle;
	  layout.currentStyle.apply(layout);
	}
	return false;
      }
    });
    }
  }
  
  @Override
  public boolean performClick() {
    frame.sendNativeValueEvent(getElementId(), 0, 0);
    return super.performClick();
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
  public void setBitmap(Bitmap bitmap) { }

  @Override
  public void reshape(int size) { }

  @Override
  public void deinitialize() { }

  @Override
  public void addImageUrl(String url, int width, int height) { }
}
