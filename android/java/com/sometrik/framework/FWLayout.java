package com.sometrik.framework;

import java.util.ArrayList;
import java.util.Iterator;

import com.sometrik.framework.NativeCommand.Selector;

import android.view.MotionEvent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.LinearLayout;

public class FWLayout extends LinearLayout implements NativeCommandHandler {
  
  private FrameWork frame;
  private boolean childListeners = false;
  private ChildClickListener hostListener;
  private ViewStyleManager normalStyle, activeStyle, currentStyle;
  
  public FWLayout(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;
//    setDividerDrawable(frame.getResources().getDrawable(android.R.drawable.divider_horizontal_bright));
//    setDividerDrawable(frame.getResources().getDrawable(android.R.drawable.divider_horizontal_textfield));
    
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);

    final FWLayout layout = this;
    
    setOnClickListener(new OnClickListener() {
      @Override
      public void onClick(View v) {
	frame.intChangedEvent(getElementId(), 0, 0);
      }
    });
    
    setOnTouchListener(new OnTouchListener() {
      @Override
      public boolean onTouch(View v, MotionEvent event) {
	if (event.getAction() == MotionEvent.ACTION_DOWN) {
	  layout.currentStyle = layout.activeStyle;
	  layout.currentStyle.apply(layout);
	} else { // if (event.getAction() == MotionEvent.ACTION_UP)
	  layout.currentStyle = layout.normalStyle;
	  layout.currentStyle.apply(layout);
	}
	return false;
      }
    });
  }
  
  @Override
  public int getElementId() {
    return getId();
  }
 
  @Override
  public void addChild(final View view) {
    addView(view);
    
    if (childListeners) {
      if (view instanceof AdapterView) {
	return;
      } else {
	view.setOnClickListener(new OnClickListener() {
	  @Override
	  public void onClick(View v) {
  	    System.out.println("child click " + v.getId());
  	    int index = getChildIndex(v.getId());
  	    hostListener.onClick(index, v.getId());
	  }
	});
      }
    } 
    else if (view instanceof FWLayout){
      view.setOnClickListener(new OnClickListener() {
	  @Override
	  public void onClick(View v) {
	    frame.intChangedEvent(getElementId(), 0, 1);
	  }
      });
    }
  }
  
  public interface ChildClickListener {
    
    public void onClick(int childIndex, int childId);
  }

  private int getChildIndex(int id) {
    for (int i = 0; i < getChildCount(); i++) {
      if (id == getChildAt(i).getId()) {
	return i;
      }
    }
    return 0;
  }
  
  @Override
  public void addOption(int optionId, String text) {
    System.out.println("FWLayout couldn't handle command");
  }

  @Override
  public void setValue(String v) {
    System.out.println("FWLayout couldn't handle command");
  }

  @Override
  public void setValue(int v) {
    if (v == 1){
      frame.setCurrentView(this, true);
    } else if (v == 2){
      frame.setCurrentView(this, false);
    }
  }
  
  public void setChildListeners(final ChildClickListener listener) {
    this.hostListener = listener;
    childListeners = true;
    for (int i = 0; i < getChildCount(); i++) {
      View view = getChildAt(i);
      if (view instanceof AdapterView) {
	return;
      } else {
	view.setOnClickListener(new OnClickListener() {
	  @Override
	  public void onClick(View v) {
  	    System.out.println("child click " + v.getId());
  	    int index = getChildIndex(v.getId());
  	    hostListener.onClick(index, v.getId());
	  }
	});
      }
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
    }
    
    if (key.equals("frame")) {
      if (value.equals("light")) {
	this.setBackgroundDrawable(frame.getResources().getDrawable(android.R.drawable.dialog_holo_light_frame));
//	this.setDividerDrawable(frame.getResources().getDrawable(android.R.drawable.divider_horizontal_bright));
      } else if (value.equals("dark")) {
	this.setBackgroundDrawable(frame.getResources().getDrawable(android.R.drawable.alert_light_frame));
      }
    } else if (key.equals("divider")) {
      if (value.equals("middle")) {
	// this.setShowDividers(LinearLayout.SHOW_DIVIDER_MIDDLE);
      } else if (value.equals("end")) {
	// this.setShowDividers(LinearLayout.SHOW_DIVIDER_END);
      } else if (value.equals("beginning")) {
	// this.setShowDividers(LinearLayout.SHOW_DIVIDER_BEGINNING);
      }
    }
  }

  @Override
  public void setError(boolean hasError, String errorText) { }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    invalidate();
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    System.out.println("FWLayout couldn't handle command");
  }

  @Override
  public void setViewVisibility(boolean visibility) {
    if (visibility){
      this.setVisibility(VISIBLE);
    } else {
      this.setVisibility(GONE);
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
    if (size > getChildCount()) {
      System.out.println("FWLayout did nothing");
    }
    
    ArrayList<View> viewsToBeRemoved = new ArrayList<View>();
    for (int i = size; i < getChildCount(); i++) {
      View view = getChildAt(i);
      viewsToBeRemoved.add(view);
    }

    Iterator<View> i = viewsToBeRemoved.iterator();
    while (i.hasNext()) {
      View v = i.next();
      frame.removeViewFromList(v.getId());
      removeView(v);
    }
  }

  @Override
  public void setImage(byte[] bytes, int width, int height, int internalFormat) { }

  @Override
  public void reshape(int size) {
    ArrayList<View> viewsToBeRemoved = new ArrayList<View>();
    for (int i = size; i < getChildCount(); i++) {
      View view = getChildAt(i);
      viewsToBeRemoved.add(view);
    }

    Iterator<View> i = viewsToBeRemoved.iterator();
    while (i.hasNext()) {
      View v = i.next(); 
      removeView(v);
   }
  }

  @Override
  public void deinitialize() {
    // TODO Auto-generated method stub
    
  }
}
