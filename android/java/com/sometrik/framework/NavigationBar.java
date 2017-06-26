package com.sometrik.framework;

import java.io.IOException;
import java.io.InputStream;

import android.content.Context;
import android.graphics.BitmapFactory;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ImageView.ScaleType;

public class NavigationBar extends FrameLayout implements NativeCommandHandler {
  
  FrameWork frame;
  LinearLayout baseLayout;
  LinearLayout.LayoutParams childParams;
  
  public NavigationBar(FrameWork frame) {
    super(frame);

    this.frame = frame;
    this.setBackground(frame.getResources().getDrawable(android.R.drawable.dialog_holo_light_frame));
    baseLayout = new LinearLayout(frame);
    baseLayout.setOrientation(LinearLayout.HORIZONTAL);
    LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
    baseLayout.setLayoutParams(params);
    setLayoutParams(params);

    childParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, 40);
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
    final int buttonId = optionId;
    ImageView view = new ImageView(frame);
    view.setLayoutParams(childParams);
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
      }
    });

    baseLayout.addView(view);
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

}
