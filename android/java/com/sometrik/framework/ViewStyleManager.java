package com.sometrik.framework;

import android.app.Dialog;
import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.graphics.drawable.ShapeDrawable;
import android.graphics.drawable.shapes.RoundRectShape;
import android.text.TextUtils.TruncateAt;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.ScrollView;
import android.widget.TextView;

class ViewStyleManager {
  private enum WhiteSpace { NORMAL, NOWRAP };
  private enum HorizontalAlignment { LEFT, CENTER, RIGHT };
  private enum TextOverflow { CLIP, ELLIPSIS };
  private enum FontStyle { NORMAL, ITALIC, OBLIQUE };
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
  private Integer fontSize = null;
  private WhiteSpace whiteSpace = null;
  private HorizontalAlignment textAlign = null;
  private TextOverflow textOverflow = null;
  private Integer fontWeight = null;
  private FontStyle fontStyle = null;
  private String fontFamily = null;
  private String hint = null;
  
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
    fontWeight = new Integer(400);
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
    } else if (key.equals("font-size")) {
      if (value.equals("small")){
	fontSize = new Integer(9);
      } else if (value.equals("medium")){
	fontSize = new Integer(12);
      } else if (value.equals("large")){
	fontSize = new Integer(15);
      } else {
	fontSize = new Integer(value);
      }
    } else if (key.equals("white-space")) {
      if (value.equals("normal")) whiteSpace = WhiteSpace.NORMAL;
      else if (value.equals("nowrap")) whiteSpace = WhiteSpace.NOWRAP;
    } else if (key.equals("text-overflow")) {
      if (value.equals("ellipsis")) {
	textOverflow = TextOverflow.ELLIPSIS;
      }
    } else if (key.equals("font-weight")) {
      if (value.equals("normal")) {
	fontWeight = new Integer(400);
      } else if (value.equals("bold")) {
	fontWeight = new Integer(700);
      } else {
	fontWeight = new Integer(value);
      }
    } else if (key.equals("font-style")) {
      if (value.equals("italic")) {
	fontStyle = FontStyle.ITALIC;
      } else if (value.equals("oblique")) {
	fontStyle = FontStyle.OBLIQUE;
      } else {
	fontStyle = FontStyle.NORMAL;
      }
    } else if (key.equals("text-align")) {
      if (value.equals("left")) {
	textAlign = HorizontalAlignment.LEFT;
      } else if (value.equals("center")) {
	textAlign = HorizontalAlignment.CENTER;
      } else if (value.equals("right")) {
	textAlign = HorizontalAlignment.RIGHT;
      }
    } else if (key.equals("font-family")) {
      fontFamily = value;
    } else if (key.equals("hint")) {
      hint = value;
    }
  }

  public void apply(Dialog dialog) {
    if (width != null || height != null) {
      ViewGroup.LayoutParams params = dialog.getWindow().getAttributes();
      if (width != null) params.width = applyScale(width);
      if (height != null) params.height = applyScale(height);
      dialog.getWindow().setAttributes((android.view.WindowManager.LayoutParams) params);
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
	if (borderRadius != null) {
	  gd.setCornerRadius(2); // Might be necessary for zero radiuses to work
	  gd.setCornerRadii(expandRadii(borderRadius));
	}
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
      	if (marginTop != null) params.topMargin = applyScale(marginTop);
      	if (marginBottom != null) params.bottomMargin = applyScale(marginBottom);
      	if (width != null) params.width = applyScale(width);
      	if (height != null) params.height = applyScale(height);
      	if (gravity != null) params.gravity = gravity;
      	view.setLayoutParams(params);
      } else if (view.getParent() instanceof FrameLayout) {
	FrameLayout.LayoutParams params = (FrameLayout.LayoutParams)view.getLayoutParams();
      	if (marginRight != null) params.rightMargin = applyScale(marginRight);
      	if (marginLeft != null) params.leftMargin = applyScale(marginLeft);
      	if (marginTop != null) params.topMargin = applyScale(marginTop);
      	if (marginBottom != null) params.bottomMargin = applyScale(marginBottom);
      	if (width != null) params.width = applyScale(width);
      	if (height != null) params.height = applyScale(height);
      	if (gravity != null) params.gravity = gravity;
      	view.setLayoutParams(params);	
      } else if (view.getParent() instanceof RelativeLayout) {
	RelativeLayout.LayoutParams params = (RelativeLayout.LayoutParams)view.getLayoutParams();
      	if (marginRight != null) params.rightMargin = applyScale(marginRight);
      	if (marginLeft != null) params.leftMargin = applyScale(marginLeft);
      	if (marginTop != null) params.topMargin = applyScale(marginTop);
      	if (marginBottom != null) params.bottomMargin = applyScale(marginBottom);
      	if (width != null) params.width = applyScale(width);
      	if (height != null) params.height = applyScale(height);
      	view.setLayoutParams(params);	
      } else {
	System.out.println("this style cannot be applied to view that doesn't have valid layout as parent");
      }
    }
    
    if (view instanceof EditText) {
      EditText editText = (EditText) view;
      if (whiteSpace != null) {
	switch (whiteSpace) {
	case NORMAL:
	  editText.setSingleLine(false);
//	  editText.setInputType(InputType.TYPE_TEXT_FLAG_MULTI_LINE);
	  break;
	case NOWRAP:
	  editText.setSingleLine(true);
	  break;
	}
      }
    }

    if (view instanceof TextView) { // also Buttons
      TextView textView = (TextView)view;

      if (color != null) textView.setTextColor(color);
      if (fontSize != null) textView.setTextSize(fontSize);
      if (whiteSpace != null) {
	switch (whiteSpace) {
	case NORMAL:
	  textView.setSingleLine(false);
	  break;
	case NOWRAP:
	  textView.setSingleLine(true);
	  break;
	}
      }
      if (textAlign != null) {
	switch (textAlign) {
	case LEFT:
	  textView.setTextAlignment(TextView.TEXT_ALIGNMENT_TEXT_START);
	  break;
	case CENTER:
	  textView.setTextAlignment(TextView.TEXT_ALIGNMENT_CENTER);
	  break;
	case RIGHT:
  	  textView.setTextAlignment(TextView.TEXT_ALIGNMENT_TEXT_END);
  	  break;
	}
      }
      if (textOverflow != null) {
	switch (textOverflow) {
	case CLIP:
	  textView.setEllipsize(null);
	  break;
	case ELLIPSIS:
	  textView.setEllipsize(TruncateAt.END);
	  break;
	} 
      }
      if (fontFamily != null || fontWeight != null || fontStyle != null) {
	int flags = 0;
	if (fontWeight != null && fontWeight > 550) flags |= Typeface.BOLD;
	if (fontStyle != null && (fontStyle == FontStyle.ITALIC || fontStyle == FontStyle.OBLIQUE)) flags |= Typeface.ITALIC;
	if (fontFamily != null) {
	  textView.setTypeface(Typeface.create(fontFamily, flags), flags);
	} else {
	  textView.setTypeface(null, flags);  
	}
      }
      if (hint != null) textView.setHint(hint);
    }
  }

  public void applyLinkColor(View view) {
    if (view instanceof TextView) { // also Buttons
      TextView textView = (TextView)view;

      if (color != null) textView.setLinkTextColor(color);
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
