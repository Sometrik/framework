package com.sometrik.framework;

import com.sometrik.framework.NativeCommand.Selector;

import android.app.Dialog;
import android.content.DialogInterface;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.text.Layout;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.TextView;

public class FWDialog extends Dialog implements NativeCommandHandler {

  private FrameWork frame;
  private LinearLayout baseView;
  private int id;
  private ViewStyleManager normalStyle, activeStyle, currentStyle;
  private FWTextView titleView;
  
  public FWDialog(final FrameWork frame, final int id) {
    super(frame);
    requestWindowFeature(Window.FEATURE_NO_TITLE);
    this.frame = frame;
    this.id = id;
    baseView = new LinearLayout(frame);
    baseView.setOrientation(LinearLayout.VERTICAL);
    setContentView(baseView);
    LayoutParams params = getWindow().getAttributes();
    params.height = LayoutParams.WRAP_CONTENT;
    params.width = LayoutParams.MATCH_PARENT;
    getWindow().setAttributes((android.view.WindowManager.LayoutParams) params);

    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
    
    
    titleView = new FWTextView(frame, false);
    titleView.setStyle(Selector.NORMAL, "background-color", "#ffffff");
    titleView.setStyle(Selector.NORMAL, "width", "match-parent");
    titleView.setStyle(Selector.NORMAL, "gravity", "center-horizontal");
    titleView.setStyle(Selector.NORMAL, "height", "wrap-content");
    titleView.setStyle(Selector.NORMAL, "font-size", "24");
    titleView.setStyle(Selector.NORMAL, "padding-top", "12");
    titleView.setStyle(Selector.NORMAL, "padding-bottom", "12");
    titleView.setStyle(Selector.NORMAL, "font-weight", "bold");
    titleView.setStyle(Selector.NORMAL, "padding-left", "14");
    titleView.setStyle(Selector.NORMAL, "color", "#c1272d");
    baseView.addView(titleView);

    LinearLayout dividerView = new LinearLayout(frame);
    LinearLayout.LayoutParams dividerParams = new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, 5);
    dividerView.setBackgroundColor(Color.parseColor("#c1272d"));
    dividerView.setLayoutParams(dividerParams);
    baseView.addView(dividerView);
    
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
    titleView.setText(v);
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
