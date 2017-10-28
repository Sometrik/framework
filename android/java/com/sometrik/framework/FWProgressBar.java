package com.sometrik.framework;

import com.sometrik.framework.NativeCommand.Selector;

import android.view.View;
import android.widget.ProgressBar;

public class FWProgressBar extends ProgressBar implements NativeCommandHandler {
  private FrameWork frame;
  private ViewStyleManager normalStyle, activeStyle, selectedStyle;
  private ViewStyleManager currentStyle;
  
  public FWProgressBar(FrameWork frame) {
    super(frame);
    this.frame = frame;
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
  }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) { }

  @Override
  public void addChild(View view) { }

  @Override
  public void addOption(int optionId, String text) { }

  @Override
  public void addColumn(String text, int columnType) { }

  @Override
  public void addData(String text, int row, int column, int sheet) { }

  @Override
  public void setValue(String v) { }

  @Override
  public void setImage(byte[] bytes, int width, int height, int internalFormat) { }

  @Override
  public void setValue(int v) { }

  @Override
  public void reshape(int value, int size) { }

  @Override
  public void reshape(int size) { }

  @Override
  public void setViewVisibility(boolean visible) {
    if (visible) {
      this.setVisibility(VISIBLE);
    } else {
      this.setVisibility(GONE);
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
    } else if (selector == Selector.SELECTED) {
      selectedStyle.setStyle(key, value);      
      if (selectedStyle == currentStyle) selectedStyle.apply(this);
    }
  }

  @Override
  public void setError(boolean hasError, String errorText) { }
  
  @Override
  public void clear() { }

  @Override
  public void flush() { }

  @Override
  public void deinitialize() { }

  @Override
  public int getElementId() {
    return getId();
  }

  @Override
  public void addImageUrl(String url, int width, int height) {
    // TODO Auto-generated method stub
    
  }

}
