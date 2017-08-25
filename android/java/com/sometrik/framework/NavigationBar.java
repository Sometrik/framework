package com.sometrik.framework;

import android.graphics.Color;
import android.util.DisplayMetrics;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;

public class NavigationBar extends FrameLayout implements NativeCommandHandler {
  
  FrameWork frame;
  LinearLayout baseLayout;
  LinearLayout.LayoutParams childParams;
  int displayScale = 1;
  
  public NavigationBar(FrameWork frame) {
    super(frame);
    this.frame = frame;
//    this.setBackground(frame.getResources().getDrawable(android.R.drawable.dialog_holo_light_frame));
    setBackgroundColor(Color.parseColor("#ffffff"));
    baseLayout = new LinearLayout(frame);
    baseLayout.setOrientation(LinearLayout.HORIZONTAL);
    LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
    baseLayout.setLayoutParams(params);
    setLayoutParams(params);
    DisplayMetrics metrics = frame.setupDisplayMetrics();
    displayScale = (int) metrics.scaledDensity;
    childParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, 60 * displayScale);
    childParams.weight = 1;
    
    
    addView(baseLayout);
  }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addChild(View view) {
    System.out.println("addChild on NavigationBar " + view.getId());
    final int buttonId = view.getId();
    view.setLayoutParams(childParams);
    view.setOnClickListener(new OnClickListener() {
      @Override
      public void onClick(View v) {
	System.out.println("Navigation OnClick: " + buttonId);
	frame.intChangedEvent(System.currentTimeMillis() / 1000.0, getId(), 1, buttonId);
      }
    });
    baseLayout.addView(view);
  }

  @Override
  public void addOption(int optionId, String text) {
    System.out.println("navigationBar couldn't handle command");
  }

  @Override
  public void addColumn(String text, int columnType) {
    
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
  public void setValue(int v) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void reshape(int value, int size) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setViewEnabled(Boolean enabled) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setViewVisibility(boolean visible) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setStyle(String key, String value) {
    if (key.equals("width")) {
      LinearLayout.LayoutParams params = (android.widget.LinearLayout.LayoutParams) getLayoutParams();
      if (value.equals("wrap-content")) {
	params.width = LinearLayout.LayoutParams.WRAP_CONTENT;
      } else if (value.equals("match-parent")) {
	params.width = LinearLayout.LayoutParams.MATCH_PARENT;
      } else {
	final float scale = getContext().getResources().getDisplayMetrics().density;
	int pixels = (int) (Integer.parseInt(value) * scale + 0.5f);
	params.width = pixels;
      }
      setLayoutParams(params);
    } else if (key.equals("height")) {
      LinearLayout.LayoutParams params = (android.widget.LinearLayout.LayoutParams) getLayoutParams();
      if (value.equals("wrap-content")) {
	params.height = LinearLayout.LayoutParams.WRAP_CONTENT;
      } else if (value.equals("match-parent")) {
	params.height = LinearLayout.LayoutParams.MATCH_PARENT;
      } else {

	final float scale = getContext().getResources().getDisplayMetrics().density;
	int pixels = (int) (Integer.parseInt(value) * scale + 0.5f);
	params.height = pixels;
      }
      setLayoutParams(params);
    }
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
  public int getElementId() {
    return getId();
  }

  @Override
  public void setImage(byte[] bytes) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void reshape(int size) {
    // TODO Auto-generated method stub
    
  }

}
