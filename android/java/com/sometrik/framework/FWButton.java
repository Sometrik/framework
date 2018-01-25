package com.sometrik.framework;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;

public class FWButton extends Button implements NativeCommandHandler {
  private FrameWork frame;
  private ViewStyleManager normalStyle, activeStyle, selectedStyle;
  private ViewStyleManager currentStyle;
  private boolean isSelected = false;
      
  public FWButton(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;

    BitmapCache bitmapCache = frameWork.bitmapCache;
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(bitmapCache, scale, false);
    this.selectedStyle = new ViewStyleManager(bitmapCache, scale, false);
    
    setMinHeight(0);
    setMinimumHeight(0);
    setMinWidth(0);
    setMinimumWidth(0);
    
    final FWButton button = this;
        
    setOnTouchListener(new OnTouchListener() {
      @Override
      public boolean onTouch(View v, MotionEvent event) {
	if (button.activeStyle.isModified()) {
	  if (event.getAction() == MotionEvent.ACTION_DOWN) {
	    button.currentStyle = button.activeStyle;
	    button.currentStyle.apply(button, true);
	  } else if (event.getAction() == MotionEvent.ACTION_UP || event.getAction() == MotionEvent.ACTION_POINTER_UP
	    || event.getAction() == MotionEvent.ACTION_CANCEL || event.getAction() == MotionEvent.ACTION_OUTSIDE) {
	    button.currentStyle = isSelected ? button.selectedStyle : button.normalStyle;
	    button.currentStyle.apply(button, true);
	  }
	}
	return false;
      }
    });
  }
  

  @Override
  public boolean performClick() {
    requestFocus();
    System.out.println("FWButton " + getElementId() + " click ");
    frame.sendNativeValueEvent(getElementId(), 0, 0);
//    frame.hideSoftKeyboard();
    return super.performClick();
  }
  
  @Override
  public void addChild(View view) { }
  
  @Override
  public void addOption(int optionId, String text) { }

  @Override
  public void setValue(String v) {
    setText(v);
  }
  
  @Override
  public void setValue(int v) {
    boolean b = v != 0;
    if (b != isSelected) {  
      isSelected = b;
      if (isSelected) currentStyle = selectedStyle;
      else currentStyle = normalStyle;
      currentStyle.apply(this);
    }
  }

  @Override
  public int getElementId() {
    return getId();
  }

  @Override
  public void setStyle(Selector selector, String key, String value) {
    if (selector == Selector.NORMAL) {
      normalStyle.setStyle(key, value);
    } else if (selector == Selector.ACTIVE) {
      activeStyle.setStyle(key, value);      
    } else if (selector == Selector.SELECTED) {
      selectedStyle.setStyle(key, value);      
    }
  }
  
  @Override
  public void applyStyles() {
    currentStyle.apply(this);    
  }

  @Override
  public void setError(boolean hasError, String errorText) { }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) { }

  @Override
  public void addData(String text, int row, int column, int sheet) { }

  @Override
  public void setViewVisibility(boolean visibility) {
    if (visibility) {
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
