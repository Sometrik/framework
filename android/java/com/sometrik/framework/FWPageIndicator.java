package com.sometrik.framework;

import java.util.ArrayList;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.drawable.ShapeDrawable;
import android.graphics.drawable.shapes.OvalShape;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.LinearLayout;

public class FWPageIndicator extends LinearLayout implements NativeCommandHandler {

  private FrameWork frame;
  private ArrayList<BallIndicator> indicatorList;
  private ViewStyleManager normalStyle, activeStyle, currentStyle;
  private int activeBallColor;
  private int ballColor;
  
  
  public FWPageIndicator(FrameWork frame, int pageCount) {
    super(frame);
    this.frame = frame;
    indicatorList = new ArrayList<BallIndicator>();
    
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
    ballColor = Color.parseColor("#c1272d");
    activeBallColor = Color.parseColor("#c1272d");
    
    for (int i = 0; i < pageCount; i++) {
      addBallIndicator();
    }
    
    if (pageCount > 0) {
      setValue(0);
    }
  }
  
  	protected class BallIndicator extends FWFrameLayout {
	  
	  protected ShapeDrawable oval;
	  private int activeBallColor;
	  private int ballColor;
	  private boolean active;

	  protected BallIndicator(FrameWork frameWork, int id, int ballColor, int activeBallColor) {
	    super(frameWork, id);
	      this.ballColor = ballColor;
	      this.activeBallColor = activeBallColor;

	      oval = new ShapeDrawable(new OvalShape());
	      oval.setIntrinsicHeight(10);
	      oval.setIntrinsicWidth(10);
	      oval.getPaint().setColor(ballColor);
	      setBackgroundDrawable(oval);
	      active = false;
	  }
	  
	  private void setActive(boolean active) {
	    this.active = active;
	    oval.getPaint().setColor(active ? activeBallColor : ballColor);
	    System.out.println("pageControl ball color " + (active ? activeBallColor : ballColor));
	    setBackgroundDrawable(oval);
	    invalidate();
	  }
	  
	  private void setBallColor(int color) {
	    ballColor = color;
	    if (!active) {
	      oval.getPaint().setColor(color);
	    }
	  }
	  private void setActiveBallColor(int color) {
	    activeBallColor = color;
	    if (active) {
	      oval.getPaint().setColor(color);
	    }
	  }
  	}
  	
  	
  void addBallIndicator() {
    BallIndicator shapeLayout = new BallIndicator(frame, 0, ballColor, activeBallColor);
    indicatorList.add(shapeLayout);
    addView(shapeLayout);
    shapeLayout.setStyle(Selector.NORMAL, "margin-left", "1");
    shapeLayout.setStyle(Selector.NORMAL, "margin-right", "1");
    shapeLayout.applyStyles();
  }
  
  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addChild(View view) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addOption(int optionId, String text) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addColumn(String text, int columnType) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setValue(String v) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setBitmap(Bitmap bitmap) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addImageUrl(String url, int width, int height) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setValue(int v) {
    for (int i = 0; i < indicatorList.size(); i++) {
      BallIndicator indicator = indicatorList.get(i);
      indicator.setActive(i == v);
    }
    invalidate();
  }

  @Override
  public void reshape(int value, int size) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void reshape(int size) {
    System.out.println("java pageControl reshape " + size);
//    if (size > 20) {
//      size = 20;
//    }
    if (size < indicatorList.size()) {
      int listSize = indicatorList.size();
      for (int i = size; i < listSize; i++) {
	removeView(indicatorList.get(indicatorList.size() - 1));
	indicatorList.remove(indicatorList.size() - 1);
      }
    } else if (size > indicatorList.size()) {
      System.out.println("java pageControl reshape adding");
      int listSize = indicatorList.size();
      for (int i = listSize; i < size; i++) {
	addBallIndicator();
      }
    }
    invalidate();
  }

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
    
    if (key.equals("color")) {
      ballColor = Color.parseColor(value);
      for (BallIndicator indicator : indicatorList) {
	indicator.setBallColor(ballColor);
      }
      return;
    } else if (key.equals("active-color")) {
      activeBallColor = Color.parseColor(value);
      for (BallIndicator indicator : indicatorList) {
	indicator.setActiveBallColor(activeBallColor);
      }
      return;
    }
    if (selector == Selector.NORMAL) {
      normalStyle.setStyle(key, value);
    } else if (selector == Selector.ACTIVE) {
      activeStyle.setStyle(key, value);      
    }
  }

  @Override
  public void applyStyles() {
    currentStyle.apply(this);
  }

  @Override
  public void setError(boolean hasError, String errorText) {
    // TODO Auto-generated method stub
    
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
  public void deinitialize() {
    // TODO Auto-generated method stub
    
  }

  @Override
  public int getElementId() {
    return getId();
  }
}