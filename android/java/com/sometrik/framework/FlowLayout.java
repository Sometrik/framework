package com.sometrik.framework;

import com.sometrik.framework.NativeCommand.Selector;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup;

public class FlowLayout extends ViewGroup implements NativeCommandHandler {
  private int mHorizontalSpacing;
  private int mVerticalSpacing;
  private Paint mPaint;
  private FrameWork frame;

  public FlowLayout(Context context, AttributeSet attrs) {
    super(context, attrs);
    frame = (FrameWork)context;

//    TypedArray a = context.obtainStyledAttributes(attrs, R.styleable.FlowLayout);
    try {
//      mHorizontalSpacing = a.getDimensionPixelSize(R.styleable.FlowLayout_horizontalSpacing, 0);
//      mVerticalSpacing = a.getDimensionPixelSize(R.styleable.FlowLayout_verticalSpacing, 0);
    } finally {
//      a.recycle();
    }

    mPaint = new Paint();
    mPaint.setAntiAlias(true);
    mPaint.setColor(0xffff0000);
    mPaint.setStrokeWidth(2.0f);
  }

  @Override
  protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
    int widthSize = MeasureSpec.getSize(widthMeasureSpec) - getPaddingRight();
    int widthMode = MeasureSpec.getMode(widthMeasureSpec);

    boolean growHeight = widthMode != MeasureSpec.UNSPECIFIED;

    int width = 0;
    int height = getPaddingTop();

    int currentWidth = getPaddingLeft();
    int currentHeight = 0;

    boolean breakLine = false;
    boolean newLine = false;
    int spacing = 0;

    final int count = getChildCount();
    for (int i = 0; i < count; i++) {
      View child = getChildAt(i);
      measureChild(child, widthMeasureSpec, heightMeasureSpec);

      LayoutParams lp = (LayoutParams) child.getLayoutParams();
      spacing = mHorizontalSpacing;
      if (lp.horizontalSpacing >= 0) {
	spacing = lp.horizontalSpacing;
      }

      if (growHeight && (breakLine || currentWidth + child.getMeasuredWidth() > widthSize)) {
	height += currentHeight + mVerticalSpacing;
	currentHeight = 0;
	width = Math.max(width, currentWidth - spacing);
	currentWidth = getPaddingLeft();
	newLine = true;
      } else {
	newLine = false;
      }

      lp.x = currentWidth;
      lp.y = height;

      currentWidth += child.getMeasuredWidth() + spacing;
      currentHeight = Math.max(currentHeight, child.getMeasuredHeight());

      breakLine = lp.breakLine;
    }

    if (!newLine) {
      height += currentHeight;
      width = Math.max(width, currentWidth - spacing);
    }

    width += getPaddingRight();
    height += getPaddingBottom();

    setMeasuredDimension(resolveSize(width, widthMeasureSpec), resolveSize(height, heightMeasureSpec));
  }

  @Override
  protected void onLayout(boolean changed, int l, int t, int r, int b) {
    final int count = getChildCount();
    for (int i = 0; i < count; i++) {
      View child = getChildAt(i);
      LayoutParams lp = (LayoutParams) child.getLayoutParams();
      child.layout(lp.x, lp.y, lp.x + child.getMeasuredWidth(), lp.y + child.getMeasuredHeight());
    }
  }

  @Override
  protected boolean drawChild(Canvas canvas, View child, long drawingTime) {
    boolean more = super.drawChild(canvas, child, drawingTime);
    LayoutParams lp = (LayoutParams) child.getLayoutParams();
    if (lp.horizontalSpacing > 0) {
      float x = child.getRight();
      float y = child.getTop() + child.getHeight() / 2.0f;
      canvas.drawLine(x, y - 4.0f, x, y + 4.0f, mPaint);
      canvas.drawLine(x, y, x + lp.horizontalSpacing, y, mPaint);
      canvas.drawLine(x + lp.horizontalSpacing, y - 4.0f, x + lp.horizontalSpacing, y + 4.0f, mPaint);
    }
    if (lp.breakLine) {
      float x = child.getRight();
      float y = child.getTop() + child.getHeight() / 2.0f;
      canvas.drawLine(x, y, x, y + 6.0f, mPaint);
      canvas.drawLine(x, y + 6.0f, x + 6.0f, y + 6.0f, mPaint);
    }
    return more;
  }

  @Override
  protected boolean checkLayoutParams(ViewGroup.LayoutParams p) {
    return p instanceof LayoutParams;
  }

  @Override
  protected LayoutParams generateDefaultLayoutParams() {
    return new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
  }

  @Override
  public LayoutParams generateLayoutParams(AttributeSet attrs) {
    return new LayoutParams(getContext(), attrs);
  }

  @Override
  protected LayoutParams generateLayoutParams(ViewGroup.LayoutParams p) {
    return new LayoutParams(p.width, p.height);
  }

  public static class LayoutParams extends ViewGroup.LayoutParams {
    int x;
    int y;

    public int horizontalSpacing;
    public boolean breakLine;

    public LayoutParams(Context context, AttributeSet attrs) {
      super(context, attrs);
//      TypedArray a = context.obtainStyledAttributes(attrs, R.styleable.FlowLayout_LayoutParams);
      try {
//	horizontalSpacing = a.getDimensionPixelSize(R.styleable.FlowLayout_LayoutParams_layout_horizontalSpacing, -1);
//	breakLine = a.getBoolean(R.styleable.FlowLayout_LayoutParams_layout_breakLine, false);
      } finally {
//	a.recycle();
      }
    }

    public LayoutParams(int w, int h) {
      super(w, h);
    }
  }

  @Override
  public void addChild(View view) {
    addView(view);
  }

  @Override
  public void addOption(int optionId, String text) { }

  @Override
  public void setValue(String v) { }

  @Override
  public void setValue(int v) {
    if (v == 1){
      frame.setCurrentView(this);
    } else if (v == 2) {
      frame.setCurrentView(this);
    }
    
  }

  @Override
  public void setStyle(Selector selector, String key, String value) { }

  @Override
  public void setError(boolean hasError, String errorText) { }

  @Override
  public void applyStyles() { }
  
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
    // TODO Auto-generated method stub
    
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
    // TODO Auto-generated method stub
    
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
  public void setBitmap(Bitmap bitmap) { }

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
