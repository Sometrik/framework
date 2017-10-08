package com.sometrik.framework;

import android.graphics.Bitmap.Config;
import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.text.TextUtils.TruncateAt;
import android.view.Gravity;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;
import android.widget.TextView;

public class FWTextView extends TextView implements NativeCommandHandler {

  private FrameWork frame;
  private GradientDrawable currentBackground = null;
  
  public FWTextView(FrameWork frame) {
    super(frame);
    this.frame = frame;
//    Color color = new Color();
    this.setBackground(null);
  }

  private GradientDrawable createBackground() {
    if (currentBackground == null) {
      currentBackground = new GradientDrawable();
      setBackground(currentBackground);
    }
    currentBackground.setColor(Color.parseColor("#ffffff")); // Changes this drawable to use a single color instead of a gradient
    return currentBackground;
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
  public void setStyle(String key, String value) {
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
    } else if (key.equals("padding-top")) {
      setPadding(getPaddingLeft(), Integer.parseInt(value), getPaddingRight(), getPaddingBottom());
    } else if (key.equals("padding-bottom")) {
      setPadding(getPaddingLeft(), getPaddingTop(), getPaddingRight(), Integer.parseInt(value));
    } else if (key.equals("padding-left")) {
      setPadding(Integer.parseInt(value), getPaddingTop(), getPaddingRight(), getPaddingBottom());
    } else if (key.equals("padding-right")) {
      setPadding(getPaddingLeft(), getPaddingTop(), Integer.parseInt(value), getPaddingBottom());
    } else if (key.equals("margin-right")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      params.rightMargin = Integer.parseInt(value);
      setLayoutParams(params);
    } else if (key.equals("margin-left")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      params.leftMargin = Integer.parseInt(value);
      setLayoutParams(params);
    } else if (key.equals("margin-top")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      params.topMargin = Integer.parseInt(value);
      setLayoutParams(params);
    } else if (key.equals("margin-bottom")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      params.bottomMargin = Integer.parseInt(value);
      setLayoutParams(params);
    } else if (key.equals("width")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      if (value.equals("wrap-content")) {
	params.width = LinearLayout.LayoutParams.WRAP_CONTENT;
      } else if (value.equals("match-parent")) {
	params.width = LinearLayout.LayoutParams.MATCH_PARENT;
      } else {
	final float scale = getContext().getResources().getDisplayMetrics().density;
	int pixels = (int) (Integer.parseInt(value) * scale + 0.5f);
	params.width = pixels;
      }
      setLayoutParams(params);
    } else if (key.equals("height")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      if (value.equals("wrap-content")) {
	params.height = LinearLayout.LayoutParams.WRAP_CONTENT;
      } else if (value.equals("match-parent")) {
	params.height = LinearLayout.LayoutParams.MATCH_PARENT;
      } else {

	final float scale = getContext().getResources().getDisplayMetrics().density;
	int pixels = (int) (Integer.parseInt(value) * scale + 0.5f);
	params.height = pixels;
      }
      setLayoutParams(params);
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
    } else if (key.equals("border")) {
      if (value.equals("none")) {
	setBackgroundResource(0);
      } else {
	GradientDrawable gd = createBackground();
	gd.setStroke(1, Color.parseColor(value));
      }
    } else if (key.equals("border-radius")) {
      final float scale = getContext().getResources().getDisplayMetrics().density;
      int pixels = (int) (Integer.parseInt(value) * scale + 0.5f);
      GradientDrawable gd = createBackground();
      gd.setCornerRadius(pixels);      
    } else if (key.equals("weight")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      params.weight = Integer.parseInt(value);
      setLayoutParams(params);
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
    } else if (key.equals("color")) {
      this.setTextColor(Color.parseColor(value));
    } else if (key.equals("gravity")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      if (value.equals("bottom")) {
	setGravity(Gravity.BOTTOM);
      } else if (value.equals("top")) {
	setGravity(Gravity.TOP);
      } else if (value.equals("left")) {
	setGravity(Gravity.LEFT);
      } else if (value.equals("right")) {
	setGravity(Gravity.RIGHT);
      } else if (value.equals("center")) {
	setGravity(Gravity.CENTER);
      } else if (value.equals("center-vertical")) {
	setGravity(Gravity.CENTER_VERTICAL);
      } else if (value.equals("center-horizontal")) {
	setGravity(Gravity.CENTER_HORIZONTAL);
      }
      setLayoutParams(params);
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
