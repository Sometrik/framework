package com.sometrik.framework;

import android.graphics.Color;
import android.graphics.drawable.GradientDrawable;
import android.graphics.drawable.ShapeDrawable;
import android.graphics.drawable.shapes.RoundRectShape;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.ScrollView;
import android.widget.TextView;

class ViewStyleManager {
  private float displayScale = 1.0f;
  
  private boolean isPaddingSet = false;
  private int paddingLeft = 0, paddingTop = 0, paddingRight = 0, paddingBottom = 0;
  private Integer marginLeft = null, marginTop = null;
  private Integer marginRight = null, marginBottom = null;
  private Integer width = null, height = null;
  private Float opacity = null;
  private Integer leftPosition = null, topPosition = null;
  private Integer rightPosition = null, bottomPosition = null;
  private int minWidth = 0, minHeight = 0;
  private String title = null;
  private Integer weight = null;
  private Integer backgroundColor = null;
  private Integer color = null;
  private Integer gravity = null;
  private Float zoom = null;
  private Integer shadow = null;
  private float[] borderRadius = null;
  private Integer borderWidth = null, borderColor = null;
  
  public ViewStyleManager(float displayScale, boolean isDefault) {
    this.displayScale = displayScale;
    if (isDefault) setDefaults();
  }
  
  public void setDefaults() {
    zoom = new Float(1.0);
    opacity = new Float(1.0);
    shadow = new Integer(0);
    color = new Integer(Color.parseColor("#000000"));
    backgroundColor = new Integer(0);
  }
    
  public void setStyle(String key, String value) {
    if (key.equals("padding-top")) {
      isPaddingSet = true;
      paddingTop = Integer.parseInt(value);
    } else if (key.equals("padding-bottom")) {
      isPaddingSet = true;
      paddingBottom = Integer.parseInt(value);
    } else if (key.equals("padding-left")) {
      isPaddingSet = true;
      paddingLeft = Integer.parseInt(value);
    } else if (key.equals("padding-right")) {
      isPaddingSet = true;
      paddingRight = Integer.parseInt(value);
    } else if (key.equals("margin")) {
      marginRight = marginLeft = marginTop = marginBottom = new Integer(value);
    } else if (key.equals("margin-right")) {
      marginRight = new Integer(value);
    } else if (key.equals("margin-left")) {
      marginLeft = new Integer(value);
    } else if (key.equals("margin-top")) {
      marginTop = new Integer(value);      
    } else if (key.equals("margin-bottom")) {
      marginBottom = new Integer(value);      
    } else if (key.equals("weight")) {
      weight = new Integer(value);
    } else if (key.equals("opacity")) {
      opacity = new Float(value);
    } else if (key.equals("text-shadow")) {

    } else if (key.equals("box-shadow")) {

    } else if (key.equals("shadow")) {
      shadow = new Integer(value);
    } else if (key.equals("left")) {
      leftPosition = new Integer(value);
    } else if (key.equals("top")) {
      topPosition = new Integer(value);
    } else if (key.equals("right")) {
      rightPosition = new Integer(value);
    } else if (key.equals("bottom")) {
      bottomPosition = new Integer(value);
    } else if (key.equals("min-width")) {
      minWidth = Integer.parseInt(value);
    } else if (key.equals("min-height")) {
      minHeight = Integer.parseInt(value);
    } else if (key.equals("title")) {
      title = value;
    } else if (key.equals("width")) {
      if (value.equals("wrap-content")) {
	width = new Integer(LayoutParams.WRAP_CONTENT);
      } else if (value.equals("match-parent")) {
	width = new Integer(LayoutParams.MATCH_PARENT);
      } else {
	width = new Integer(value);
      }
    } else if (key.equals("height")) {
      if (value.equals("wrap-content")) {
	height = new Integer(LayoutParams.WRAP_CONTENT);
      } else if (value.equals("match-parent")) {
	height = new Integer(LayoutParams.MATCH_PARENT);
      } else {
	height = new Integer(value);
      }
    } else if (key.equals("background-color")) {
      backgroundColor = new Integer(Color.parseColor(value));
    }  else if (key.equals("color")) {
      color = new Integer(Color.parseColor(value));
    } else if (key.equals("gravity")) {
      if (value.equals("bottom")) {
	gravity = new Integer(Gravity.BOTTOM);
      } else if (value.equals("top")) {
	gravity = new Integer(Gravity.TOP);
      } else if (value.equals("left")) {
	gravity = new Integer(Gravity.LEFT);
      } else if (value.equals("right")) {
	gravity = new Integer(Gravity.RIGHT);
      } else if (value.equals("center")) {
	gravity = new Integer(Gravity.CENTER);
      } else if (value.equals("center-vertical")) {
	gravity = new Integer(Gravity.CENTER_VERTICAL);
      } else if (value.equals("center-horizontal")) {
	gravity = new Integer(Gravity.CENTER_HORIZONTAL);
      }
    } else if (key.equals("zoom")) {
      if (value.equals("inherit")) {
	zoom = null;
      } else {
	zoom = new Float(value);
      }
    } else if (key.equals("border")) {
      if (value.equals("none")) {
	borderWidth = new Integer(0);
      } else {
	borderWidth = new Integer(1);
	borderColor = new Integer(Color.parseColor(value));
      }      
    } else if (key.equals("border-radius")) {
      String[] values = value.split(" ");
      borderRadius = new float[4];
      float prev = 0.0f;
      for (int i = 0; i < 4; i++) {
	if (i < values.length) prev = Float.valueOf(values[i].trim());
	borderRadius[i] = prev;
      }
    } 
  }

  public void apply(View view) {
    if (opacity != null) view.setAlpha(opacity);
    if (zoom != null) {
      view.setScaleX(zoom);
      view.setScaleY(zoom);
    }
    if (shadow != null) view.setElevation(shadow);
//    if (title != null) view.setTooltipText(title);

    // Scaled parameters
    if (isPaddingSet) {
      view.setPadding(applyScale(paddingLeft),
		      applyScale(paddingTop),
		      applyScale(paddingRight),
		      applyScale(paddingBottom));
    }
    if (leftPosition != null) view.setLeft(applyScale(leftPosition));
    if (rightPosition != null) view.setRight(applyScale(rightPosition));
    if (topPosition != null) view.setTop(applyScale(topPosition));
    if (bottomPosition != null) view.setBottom(applyScale(bottomPosition));
    if (minWidth > 0) view.setMinimumWidth(applyScale(minWidth));
    if (minHeight > 0) view.setMinimumHeight(applyScale(minHeight));
    if ((borderColor != null && borderWidth != null) || borderRadius != null) {
      view.setBackgroundResource(0);
      if (borderRadius != null &&
	  (backgroundColor == null || backgroundColor == 0) &&
	  (borderWidth == null || borderWidth == 0)) {
     	if (backgroundColor != null) view.setBackgroundColor(backgroundColor);
	RoundRectShape shape = new RoundRectShape(expandRadii(borderRadius), null, null); 
     	ShapeDrawable sd = new ShapeDrawable(shape);
     	view.setBackground(sd);
      } else {
	GradientDrawable gd = new GradientDrawable();
	if (backgroundColor != null) gd.setColor(backgroundColor);
	if (borderRadius != null) gd.setCornerRadii(expandRadii(borderRadius));
	if (borderColor != null && borderWidth != null) {
	  gd.setStroke(borderWidth, borderColor);
	}
	view.setBackground(gd);
      }
    } else if (backgroundColor != null) {
      view.setBackgroundColor(backgroundColor);
    }
    
    // Layout parameters
    if (weight != null || width != null || height != null ||
	marginRight != null || marginLeft != null ||
	marginTop != null || marginBottom != null ||
	gravity != null) {
      
      if (view.getParent() instanceof ScrollView) { // stupid hack
	FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
	view.setLayoutParams(params);
      } else if (view.getParent() instanceof LinearLayout) {
	LinearLayout.LayoutParams params = (LinearLayout.LayoutParams)view.getLayoutParams();
      	if (weight != null) params.weight = weight;
      	if (marginRight != null) params.rightMargin = applyScale(marginRight);
      	if (marginLeft != null) params.leftMargin = applyScale(marginLeft);
      	if (marginTop != null) params.rightMargin = applyScale(marginTop);
      	if (marginBottom != null) params.bottomMargin = applyScale(marginBottom);
      	if (width != null) params.width = applyScale(width);
      	if (height != null) params.height = applyScale(height);
      	if (gravity != null) params.gravity = gravity;
      	view.setLayoutParams(params);
      } else if (view.getParent() instanceof FrameLayout) {
	FrameLayout.LayoutParams params = (FrameLayout.LayoutParams)view.getLayoutParams();
      	if (marginRight != null) params.rightMargin = applyScale(marginRight);
      	if (marginLeft != null) params.leftMargin = applyScale(marginLeft);
      	if (marginTop != null) params.rightMargin = applyScale(marginTop);
      	if (marginBottom != null) params.bottomMargin = applyScale(marginBottom);
      	if (width != null) params.width = applyScale(width);
      	if (height != null) params.height = applyScale(height);
      	if (gravity != null) params.gravity = gravity;
      	view.setLayoutParams(params);	
      } else if (view.getParent() instanceof RelativeLayout) {
	RelativeLayout.LayoutParams params = (RelativeLayout.LayoutParams)view.getLayoutParams();
      	if (marginRight != null) params.rightMargin = applyScale(marginRight);
      	if (marginLeft != null) params.leftMargin = applyScale(marginLeft);
      	if (marginTop != null) params.rightMargin = applyScale(marginTop);
      	if (marginBottom != null) params.bottomMargin = applyScale(marginBottom);
      	if (width != null) params.width = applyScale(width);
      	if (height != null) params.height = applyScale(height);
      	view.setLayoutParams(params);	
      } else {
	System.out.println("this style cannot be applied to view that doesn't have valid layout as parent");
      }
    }

    if (view instanceof TextView) { // also Buttons
      TextView textView = (TextView)view;

      if (color != null) textView.setTextColor(color);
    }
  }

  protected int applyScale(int v) {
    return (int)(v * displayScale + 0.5f);
  }

  protected float applyScale(float v) {
    return v * displayScale;
  }

  protected float[] expandRadii(float[] input) {
    float[] r = new float[8];
    for (int i = 0; i < 4; i++) {
      r[2 * i] = r[2 * i + 1] = applyScale(borderRadius[i]);
    }
    return r;
  }
}
