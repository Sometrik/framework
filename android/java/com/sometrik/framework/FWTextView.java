package com.sometrik.framework;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap;
import android.view.View;
import android.widget.TextView;

public class FWTextView extends TextView implements NativeCommandHandler {

  private FrameWork frame;
  ViewStyleManager normalStyle, activeStyle, currentStyle, linkStyle;
  
  public FWTextView(final FrameWork frame, boolean autolink) {
    super(frame);
    this.frame = frame;
    
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
    this.linkStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
    this.setClickable(false);
    this.setFocusable(false);

    if (autolink) {
      this.setLinksClickable(true);
      this.setAutoLinkMask(15);
    }
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
  public void setValue(int v) { }

  @Override
  public void setStyle(Selector selector, String key, String value) {
    if (selector == Selector.NORMAL) {
      normalStyle.setStyle(key, value);
      if (normalStyle == currentStyle) normalStyle.apply(this);
    } else if (selector == Selector.ACTIVE) {
      activeStyle.setStyle(key, value);      
      if (activeStyle == currentStyle) activeStyle.apply(this);
    } else if (selector == Selector.LINK) {
      linkStyle.setStyle(key, value);
      linkStyle.applyLinkColor(this);
    }

    if (key.equals("max-lines")) {
      setMaxLines(Integer.parseInt(value));
    }
  }

  @Override
  public void setError(boolean hasError, String errorText) { }

  @Override
  public int getElementId() {
    return getId();
  }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) { }

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
