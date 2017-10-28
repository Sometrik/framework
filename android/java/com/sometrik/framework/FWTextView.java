package com.sometrik.framework;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.text.TextUtils.TruncateAt;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;
import android.widget.TextView;

public class FWTextView extends TextView implements NativeCommandHandler {

  private FrameWork frame;
  ViewStyleManager normalStyle, activeStyle, currentStyle, linkStyle;
  
  public FWTextView(final FrameWork frame, boolean autolink) {
    super(frame);
    this.frame = frame;
    this.setBackground(null);
    
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
    this.linkStyle = new ViewStyleManager(frame.bitmapCache, scale, false);

    if (autolink) {
      this.setLinksClickable(true);
      this.setAutoLinkMask(15);
    }
    
    final FWTextView textView = this;
    
    setOnClickListener(new OnClickListener() {
      @Override
      public void onClick(View v) {
	System.out.println("OnClick TextView");
	frame.intChangedEvent(getElementId(), 0, 0);
      }
    });
    
    setOnTouchListener(new OnTouchListener() {
      @Override
      public boolean onTouch(View v, MotionEvent event) {
	if (event.getAction() == MotionEvent.ACTION_DOWN) {
	  textView.currentStyle = textView.activeStyle;
	  textView.currentStyle.apply(textView);
	} else if (event.getAction() == MotionEvent.ACTION_UP) {
	  textView.currentStyle = textView.normalStyle;
	  textView.currentStyle.apply(textView);
	}
	return false;
      }
    });
  }
    
  @Override
  public void addChild(View view) {
    System.out.println("FWTextView couldn't handle command");
  }
  
  @Override
  public void addOption(int optionId, String text) {
    System.out.println("FWTextView couldn't handle command");
  }

  @Override
  public void setValue(String v) {
    setText(v);
  }

  @Override
  public void setValue(int v) {
    System.out.println("FWTextView couldn't handle command");
  }

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
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    System.out.println("FWTextView couldn't handle command");
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
  public void reshape(int value, int size) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setImage(byte[] bytes, int width, int height, int internalFormat) { }

  @Override
  public void reshape(int size) {
    // TODO Auto-generated method stub
    
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
