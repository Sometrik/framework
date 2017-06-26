package com.sometrik.framework;

import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

import android.content.Context;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;

public class FWEditText extends EditText implements NativeCommandHandler {

  private FrameWork frame;
  private Date lastTypeTime;
  
  public FWEditText(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;

    LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
    setLayoutParams(params);
  }
  
  public void addDelayedChangeListener(final int viewId){
    
    addTextChangedListener(new TextWatcher() {
      public void afterTextChanged(Editable editable) {  }

      public void beforeTextChanged(CharSequence s, int start, int count, int after) {
	lastTypeTime = new Date();
      }

      @Override
      public void onTextChanged(final CharSequence text, int arg1, int arg2, int arg3) {

	System.out.println("BigEditText TextChanged");
	// dispatch after done typing (1 sec after)
	Timer t = new Timer();
	TimerTask tt = new TimerTask() {
	  @Override
	  public void run() {
	    Date myRunTime = new Date();
	    System.out.println("BigEditText TextChanged run!");

	    if ((lastTypeTime.getTime() + 2000) <= myRunTime.getTime()) {
	      post(new Runnable() {
		@Override
		public void run() {
		  System.out.println("typing finished. Sending update");
		  System.out.println("update: " + text.toString());
//		  frame.textChangedEvent(System.currentTimeMillis() / 1000.0, viewId, text);
		}
	      });
	    } else {
	      System.out.println("Canceled");
	    }
	  }
	};
	t.schedule(tt, 2000);
      }
    });
  }

  @Override
  public void onVisibilityChanged(View changedView, int visibility) {
    switch (visibility) {
    case VISIBLE:
      frame.visibilityChangedEvent(System.currentTimeMillis() / 1000.0, getId(), true);
      break;
    case GONE:
    case INVISIBLE: 
      frame.visibilityChangedEvent(System.currentTimeMillis() / 1000.0, getId(), false);
    break;
    }
  }
  
  @Override
  public void addChild(View view) {
    System.out.println("FWEditText couldn't handle command");
  }
  
  @Override
  public void addOption(int optionId, String text) {
    System.out.println("FWEditText couldn't handle command");
  }

  @Override
  public void setValue(String v) {
    setText(v);
  }

  @Override
  public void setValue(int v) {
    System.out.println("FWEditText couldn't handle command");
  }

  @Override
  public void setViewEnabled(Boolean enabled) {
    setEnabled(enabled);
  }

  @Override
  public void setStyle(String key, String value) {
    if (key.equals("add-weight")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      params.weight += 1;
      setLayoutParams(params);
      System.out.println("edittext weight: " + params.weight);
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
    } else if (key.equals("gravity")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      params.weight = 1;
      if (value.equals("bottom")) {
	params.gravity = Gravity.BOTTOM;
      } else if (value.equals("top")) {
	params.gravity = Gravity.TOP;
      } else if (value.equals("left")) {
	params.gravity = Gravity.LEFT;
      } else if (value.equals("right")) {
	params.gravity = Gravity.RIGHT;
      }
    }
  }

  @Override
  public void setError(boolean hasError, String errorText) {
    setError(hasError ? errorText : null);
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
    System.out.println("FWEditText couldn't handle command");
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
    System.out.println("Clear on EditText");
    this.setText("");
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
}
