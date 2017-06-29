package com.sometrik.framework;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;

import android.graphics.BitmapFactory;
import android.util.DisplayMetrics;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.ImageView.ScaleType;
import android.widget.LinearLayout;

public class NavigationBar extends FrameLayout implements NativeCommandHandler {
  
  FrameWork frame;
  LinearLayout baseLayout;
  LinearLayout.LayoutParams childParams;
  ArrayList<ImageView> childList;
  int displayScale = 1;
  
  public NavigationBar(FrameWork frame) {
    super(frame);

    this.frame = frame;
    this.setBackground(frame.getResources().getDrawable(android.R.drawable.dialog_holo_light_frame));
    childList = new ArrayList<ImageView>();
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
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addOption(int optionId, String text) {

    for (ImageView view : childList) {
      if (optionId == view.getId()) {
	try {
	  InputStream stream = frame.getAssets().open(text);
	  view.setImageBitmap(BitmapFactory.decodeStream(stream));
	} catch (IOException e) {
	  e.printStackTrace();
	}
      }
    }
  }

  @Override
  public void addColumn(String text, int columnType) {
    final int buttonId = columnType;
    ImageView view = new ImageView(frame);
    view.setLayoutParams(childParams);
    view.setId(columnType);
    InputStream stream;
    view.setScaleType(ScaleType.CENTER_INSIDE);
    try {
      stream = frame.getAssets().open(text);
      view.setImageBitmap(BitmapFactory.decodeStream(stream));
    } catch (IOException e) {
      e.printStackTrace();
    }
    
    view.setOnClickListener(new OnClickListener(){
      @Override
      public void onClick(View v) {
	System.out.println("Navigation OnClick: " + buttonId);
	frame.intChangedEvent(System.currentTimeMillis() / 1000.0, getId(), 1, buttonId);
      }
    });

    childList.add(view);
    baseLayout.addView(view);
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
    // TODO Auto-generated method stub
    
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

}
