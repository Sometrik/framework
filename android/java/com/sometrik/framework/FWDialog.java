package com.sometrik.framework;

import android.app.Dialog;
import android.content.DialogInterface;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.widget.LinearLayout;
import android.widget.ScrollView;

public class FWDialog extends Dialog implements NativeCommandHandler{

  FrameWork frame;
  ViewGroup baseView;
  ScrollView scrollView;
  int id;
  
  public FWDialog(final FrameWork frame, final int id) {
    super(frame);
    this.frame = frame;
    this.id = id;
    scrollView = new ScrollView(frame);
    setContentView(scrollView);
    LayoutParams params = getWindow().getAttributes();
    params.height = LayoutParams.WRAP_CONTENT;
    params.width = LayoutParams.MATCH_PARENT;
    getWindow().setAttributes((android.view.WindowManager.LayoutParams) params);

    this.setOnCancelListener(new OnCancelListener(){
      @Override
      public void onCancel(DialogInterface arg0) {
	frame.endModal(System.currentTimeMillis() / 1000.0, id, null);
      }      
    });
    // this.setOnDismissListener(new OnDismissListener(){
    //   @Override
    //   public void onDismiss(DialogInterface arg0) {
    // 	  frame.endModal(System.currentTimeMillis() / 1000.0, id, null);
    //   }
    // });
  }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    
  }

  @Override
  public void addChild(View view) {
    if (baseView == null){
      baseView = (ViewGroup) view;
      scrollView.addView(baseView);
    } else {
      baseView.addView(view);
    }
  }

  @Override
  public void addOption(int optionId, String text) {
    System.out.println("FWDialog couldn't handle the command");
    switch (optionId){
    case 0:
      break;
    case 1:
      break;
    }
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    System.out.println("FWDialog couldn't handle the command");
  }

  @Override
  public void setValue(String v) {
    setTitle(v);
  }

  @Override
  public void setValue(int v) {
    if (v > 0){
      show();
    } else {
      dismiss();
    }
  }

  @Override
  public void setViewEnabled(Boolean enabled) {
    if (enabled){
      show();
    } else {
      dismiss();
    }
  }

  @Override
  public void setViewVisibility(boolean visible) {
    if (visible){
      show();
    } else {
      dismiss();
    }
  }

  @Override
  public void setStyle(String key, String value) {
    if (key.equals("width")) {
      ViewGroup.LayoutParams params = getWindow().getAttributes();
      if (value.equals("wrap-content")) {
	params.width = LinearLayout.LayoutParams.WRAP_CONTENT;
      } else if (value.equals("match-parent")) {
	params.width = LinearLayout.LayoutParams.MATCH_PARENT;
      }
      getWindow().setAttributes((android.view.WindowManager.LayoutParams) params);
    } else if (key.equals("height")) {
      ViewGroup.LayoutParams params = getWindow().getAttributes();
      if (value.equals("wrap-content")) {
	params.height = LinearLayout.LayoutParams.WRAP_CONTENT;
      } else if (value.equals("match-parent")) {
	params.height = LinearLayout.LayoutParams.MATCH_PARENT;
      }
      getWindow().setAttributes((android.view.WindowManager.LayoutParams) params);
    }
  }

  @Override
  public void setError(boolean hasError, String errorText) {
    System.out.println("FWDialog couldn't handle the command");
  }

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
  public void setImage(byte[] bytes) {
    // TODO Auto-generated method stub
    
  }

}
