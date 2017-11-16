package com.sometrik.framework;

import com.sometrik.framework.NativeCommand.Selector;

import android.app.Dialog;
import android.content.DialogInterface;
import android.graphics.Bitmap;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.widget.FrameLayout;

public class FWDialog extends Dialog implements NativeCommandHandler {

  FrameWork frame;
  ViewGroup baseView;
  int id;
  ViewStyleManager normalStyle, activeStyle, currentStyle;
  
  public FWDialog(final FrameWork frame, final int id) {
    super(frame);
    requestWindowFeature(Window.FEATURE_NO_TITLE);
    this.frame = frame;
    this.id = id;
    baseView = new FrameLayout(frame);
    setContentView(baseView);
    LayoutParams params = getWindow().getAttributes();
    params.height = LayoutParams.WRAP_CONTENT;
    params.width = LayoutParams.MATCH_PARENT;
    getWindow().setAttributes((android.view.WindowManager.LayoutParams) params);

    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
    
    this.setOnCancelListener(new OnCancelListener(){
      @Override
      public void onCancel(DialogInterface arg0) {
	frame.intChangedEvent(id, 0, 0);
      }      
    });
    
    show();
  }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    
  }

  @Override
  public void addChild(View view) {
    baseView.addView(view);
  }

  @Override
  public void addOption(int optionId, String text) {
    System.out.println("FWDialog couldn't handle command");
    }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    System.out.println("FWDialog couldn't handle command");
    }

  @Override
  public void setValue(String v) {
    System.out.println("FWDialog couldn't handle command");
  }

  @Override
  public void setValue(int v) { }

  @Override
  public void setViewVisibility(boolean visible) {
    if (visible) {
      show();
    } else {
      dismiss();
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
  }

  @Override
  public void setError(boolean hasError, String errorText) { }

  @Override
  public void clear() {
    dismiss();
  }

  @Override
  public int getElementId() {
    return id;
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
    dismiss();
  }

  @Override
  public void addImageUrl(String url, int width, int height) {
    // TODO Auto-generated method stub
    
  }

}
