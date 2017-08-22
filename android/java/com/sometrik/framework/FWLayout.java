package com.sometrik.framework;

import java.util.ArrayList;
import java.util.Iterator;

import android.view.Gravity;
import android.view.View;
import android.widget.AdapterView;
import android.widget.LinearLayout;

public class FWLayout extends LinearLayout implements NativeCommandHandler {
  
  private FrameWork frame;
  private boolean childListeners = false;
  private ChildClickListener hostListener;
  
  public FWLayout(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;
    setDividerDrawable(frame.getResources().getDrawable(android.R.drawable.divider_horizontal_bright));
  }

  @Override
  public int getElementId() {
    return getId();
  }
 
  @Override
  public void addChild(View view) {
    System.out.println("addChild on FWLayout " + view.getId());
    addView(view);
    
    if (childListeners) {
      if (view instanceof AdapterView) {
	return;
      } else {
	System.out.println("set listener to " + view.getId());
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
	System.out.println("set listener to " + view.getId());
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
  public void setViewEnabled(Boolean enabled) {
    System.out.println("FWLayout couldn't handle command");
  }
  
  @Override
  public void setStyle(String key, String value) {
    if (key.equals("gravity")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      if (value.equals("bottom")) {
	params.gravity = Gravity.BOTTOM;
      } else if (value.equals("top")) {
	params.gravity = Gravity.TOP;
      } else if (value.equals("left")) {
	params.gravity = Gravity.LEFT;
      } else if (value.equals("right")) {
	params.gravity = Gravity.RIGHT;
      }
      setLayoutParams(params);
    } else if (key.equals("width")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      if (value.equals("wrap-content")) {
	params.width = LinearLayout.LayoutParams.WRAP_CONTENT;
      } else if (value.equals("match-parent")) {
	params.width = LinearLayout.LayoutParams.MATCH_PARENT;
      }
      setLayoutParams(params);
    } else if (key.equals("height")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      if (value.equals("wrap-content")) {
	params.height = LinearLayout.LayoutParams.WRAP_CONTENT;
      } else if (value.equals("match-parent")) {
	params.height = LinearLayout.LayoutParams.MATCH_PARENT;
      }
      setLayoutParams(params);
    } else if (key.equals("weight")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      params.weight = Integer.parseInt(value);
      setLayoutParams(params);
    } else if (key.equals("padding-top")) {
      setPadding(getPaddingLeft(), Integer.parseInt(value), getPaddingRight(), getPaddingBottom());
    } else if (key.equals("padding-bottom")) {
      setPadding(getPaddingLeft(), getPaddingTop(), getPaddingRight(), Integer.parseInt(value));
    } else if (key.equals("padding-left")) {
      setPadding(Integer.parseInt(value), getPaddingTop(), getPaddingRight(), getPaddingBottom());
    } else if (key.equals("padding-right")) {
      setPadding(getPaddingLeft(), getPaddingTop(), Integer.parseInt(value), getPaddingBottom());
    } else if (key.equals("frame")) {
      if (value.equals("light")) {
	this.setBackgroundDrawable(frame.getResources().getDrawable(android.R.drawable.dialog_holo_light_frame));
//	this.setDividerDrawable(frame.getResources().getDrawable(android.R.drawable.divider_horizontal_bright));
      } else if (value.equals("dark")) {
	this.setBackgroundDrawable(frame.getResources().getDrawable(android.R.drawable.alert_light_frame));
      }
    } else if (key.equals("divider")) {
      if (value.equals("middle")) {
	this.setShowDividers(LinearLayout.SHOW_DIVIDER_MIDDLE);
      } else if (value.equals("end")) {
	this.setShowDividers(LinearLayout.SHOW_DIVIDER_END);
      } else if (value.equals("beginning")) {
	this.setShowDividers(LinearLayout.SHOW_DIVIDER_BEGINNING);
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
    System.out.println("FWLayout reshape: " + size + " current: " + getChildCount());
//    
    if (size > getChildCount()) {
      System.out.println("FWLayout did nothing");
    }
    
    ArrayList<View> viewsToBeRemoved = new ArrayList<View>();
    for (int i = size; i < getChildCount(); i++) {
      System.out.println("addView to reshape");
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
  public void setImage(byte[] bytes) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void reshape(int size) {
    System.out.println("FWLayout reshape: " + size + " current: " + getChildCount());
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
}
