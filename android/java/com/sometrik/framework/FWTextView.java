package com.sometrik.framework;

import android.graphics.Bitmap.Config;

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
  ViewStyleManager normalStyle, activeStyle, currentStyle;
  
  public FWTextView(FrameWork frame) {
    super(frame);
    this.frame = frame;
//    Color color = new Color();
    this.setBackground(null);
    
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(scale, true);
    this.activeStyle = new ViewStyleManager(scale, false);
    
    final FWTextView textView = this;

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
  public void setViewEnabled(Boolean enabled) {
    setEnabled(enabled);
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

    if (key.equals("font-size")) {
      if (value.equals("small")) {
	this.setTextSize(9);
      } else if (value.equals("medium")) {
	this.setTextSize(12);
      } else if (value.equals("large")) {
	this.setTextSize(15);
      } else {
	setTextSize(Integer.parseInt(value));
      }
    } else if (key.equals("text-overflow")) {
      if (value.equals("ellipsis")) {
	setEllipsize(TruncateAt.END);
      }   
    } else if (key.equals("font-weight")) {
      if (value.equals("bold")) {
        setTypeface(null, Typeface.BOLD);
      } else if (value.equals("normal")) {
	setTypeface(null, Typeface.NORMAL);
      }
    } else if (key.equals("font-style")) {
      if (value.equals("italic") || value.equals("oblique")) {
        setTypeface(null, Typeface.ITALIC);
      } else if (value.equals("normal")) {
	setTypeface(null, Typeface.NORMAL);
      }
    } else if (key.equals("text-align")) {
      if (value.equals("left")) {
	setTextAlignment(TEXT_ALIGNMENT_TEXT_START);
      } else if (value.equals("center")) {
	setTextAlignment(TEXT_ALIGNMENT_CENTER);
      } else if (value.equals("right")) {
	setTextAlignment(TEXT_ALIGNMENT_TEXT_END);
      }
    } else if (key.equals("white-space")) {
      boolean single = false;
      if (value.equals("nowrap")) single = true;
      setSingleLine(single);
    } else if (key.equals("max-lines")) {
      setMaxLines(Integer.parseInt(value));
    }
  }

  @Override
  public void setError(boolean hasError, String errorText) {
    //TODO
    System.out.println("FWTextView couldn't handle command");
  }

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
  public void setImage(byte[] bytes, int width, int height, Config config) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void reshape(int size) {
    // TODO Auto-generated method stub
    
  }
}
