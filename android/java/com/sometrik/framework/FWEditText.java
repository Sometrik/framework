package com.sometrik.framework;

import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

import android.content.Context;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.EditText;

public class FWEditText extends EditText implements NativeCommandHandler {

  private FrameWork frame;
  private Date lastTypeTime;
  
  public FWEditText(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;
    
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
  public void addData(int rowNumber, int columnNumber, String text) {
    System.out.println("FWEditText couldn't handle command");
  }

}
