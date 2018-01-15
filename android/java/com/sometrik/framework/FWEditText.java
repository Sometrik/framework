package com.sometrik.framework;

import java.util.Date;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap;
import android.text.InputFilter;
import android.view.MotionEvent;
import android.view.View;
import android.widget.EditText;
import android.widget.LinearLayout;

public class FWEditText extends EditText implements NativeCommandHandler {

  private FrameWork frame;
  private Date lastTypeTime;
  ViewStyleManager normalStyle, activeStyle, currentStyle;
  
  public FWEditText(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;

    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
    
    final FWEditText editText = this;
    
    setOnTouchListener(new OnTouchListener() {
      @Override
      public boolean onTouch(View v, MotionEvent event) {
	if (event.getAction() == MotionEvent.ACTION_DOWN) {
	  editText.currentStyle = editText.activeStyle;
	  editText.currentStyle.apply(editText);
	} else if (event.getAction() == MotionEvent.ACTION_UP) {
	  editText.currentStyle = editText.normalStyle;
	  editText.currentStyle.apply(editText);
	}
	return false;
      }
    });
    
    LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
    setLayoutParams(params);
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
    setSelection(getText().length());
  }

  @Override
  public void setValue(int v) {
    if (v > 0) {
      this.requestFocus();
      frame.setSoftKeyboardShow(this, true);
    } else {
      frame.setSoftKeyboardShow(this, false);
    }
  }

  @Override
  public void setStyle(Selector selector, String key, String value) {
    
    if (key.equals("max-length")) {
      InputFilter[] filters = new InputFilter[1];
      filters[0] = new InputFilter.LengthFilter(Integer.parseInt(value)); //Filter to 10 characters
      setFilters(filters);
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
