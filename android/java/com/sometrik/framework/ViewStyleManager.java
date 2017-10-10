package com.sometrik.framework;

class ViewStyleManager {
  private float displayScale = 1.0f;

  private boolean isPaddingSet = false;
  private int paddingLeft = 0, paddingTop = 0, paddingRight = 0, paddingBottom = 0;
  private Integer marginLeft = null, marginTop = null;
  private Integer marginRight = null, marginBottom = null;
  private Integer width = null, height = null;
  private Float alpha = null;
  private Integer leftPosition = null, topPosition = null;
  private Integer rightPosition = null, bottomPosition = null;
  private int minWidth = 0, minHeight = 0;
  private String title = null;
  private Integer weight = null;

  public ViewStyleManager(float displayScale) {
    this.displayScale = displayScale;
  }
    
  public void setStyle(Selector selector, String key, String value) {
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
      alpha = new Float(value);
    } else if (key.equals("text-shadow")) {

    } else if (key.equals("box-shadow")) {

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
	width = new Integer(LinearLayout.LayoutParams.WRAP_CONTENT);
      } else if (value.equals("match-parent")) {
	width = new Integer(LinearLayout.LayoutParams.MATCH_PARENT);
      } else {
	width = new Integer(value);
      }
    } else if (key.equals("height")) {
      if (value.equals("wrap-content")) {
	height = new Integer(LinearLayout.LayoutParams.WRAP_CONTENT);
      } else if (value.equals("match-parent")) {
	height = new Integer(LinearLayout.LayoutParams.MATCH_PARENT);
      } else {
	height = new Integer(value);
      }
    } else if (key.equals("gravity")) {
      
    }
  }

  public void apply(View view) {
    if (alpha != null) view.setAlpha(alpha);
    if (title != null) view.setTooltipText(title);

    // Scaled parameters
    if (isPaddingSet) {
      view.setPadding(applyScale(paddingLeft),
		      applyScale(paddingTop),
		      applyScale(paddingRight),
		      applyScale(paddingBottom));
    }
    if (positionLeft != null) view.setLeft(applyScale(leftPosition));
    if (positionRight != null) view.setRight(applyScale(rightPosition));
    if (positionTop != null) view.setTop(applyScale(topPosition));
    if (positionBottom != null) view.setBottom(applyScale(bottomPosition));
    if (minWidth > 0) view.setMinimumWidth(applyScale(minWidth));
    if (minHeight > 0) view.setMinimumHeight(applyScale(minHeight));

    // Layout parameters
    if (weight != null || width != null || height != null ||
	marginRight != null || marginLeft != null ||
	marginTop != null || marginBottom != null) {
      LinearLayout.LayoutParams params = (LayoutParams)getLayoutParams();
      if (weight != null) params.weight = weight;
      if (marginRight != null) params.rightMargin = applyScale(marginRight);
      if (marginLeft != null) params.leftMargin = applyScale(marginLeft);
      if (marginTop != null) params.rightMargin = applyScale(marginTop);
      if (marginBottom != null) params.bottomMargin = applyScale(marginBottom);
      if (width != null) params.width = applyScale(width);
      if (height != null) params.height = applyScale(height);
      setLayoutParams(params);
    }
  }

  protected int applyScale(int v) {
    return (int)(v * scale + 0.5f);
  }

  protected float applyScale(float v) {
    return v * scale;
  }

}
