package com.sometrik.framework;

import java.io.IOException;
import java.io.InputStream;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.Gravity;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;

public class FWImageView extends ImageView implements NativeCommandHandler {

  FrameWork frame;
  
  public FWImageView(FrameWork frame) {
    super(frame);
    this.frame = frame;
    LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
    this.setLayoutParams(params);
    this.setScaleType(ScaleType.FIT_CENTER);
  }
  
  void setImageFromAssets(String filename) {
    try {
      AssetManager mgr = frame.getAssets();
      InputStream stream = mgr.open(filename);
      if (stream != null) {
	Bitmap bitmap = BitmapFactory.decodeStream(stream);
	setImageBitmap(bitmap);
	bitmap = null;
      }
      stream.close();
    } catch (IOException e) {
      e.printStackTrace();
    }
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
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addColumn(String text, int columnType) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setValue(String v) {
    
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
	final float scale = getContext().getResources().getDisplayMetrics().density;
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
      } else {
	int pixels = (int) (Integer.parseInt(value) * scale + 0.5f);
	params.width = pixels;
      }
      setLayoutParams(params);
    } else if (key.equals("height")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      if (value.equals("wrap-content")) {
	params.height = LinearLayout.LayoutParams.WRAP_CONTENT;
      } else if (value.equals("match-parent")) {
	params.height = LinearLayout.LayoutParams.MATCH_PARENT;
      } else {
	int pixels = (int) (Integer.parseInt(value) * scale + 0.5f);
	params.height = pixels;
      }
      setLayoutParams(params);
    } else if (key.equals("weight")) {
      LinearLayout.LayoutParams params = (LayoutParams) getLayoutParams();
      params.weight = Integer.parseInt(value);
      setLayoutParams(params);
    } else if (key.equals("padding-top")) {
      setPadding(getPaddingLeft(), (int) (Integer.parseInt(value) * scale + 0.5f), getPaddingRight(), getPaddingBottom());
    } else if (key.equals("padding-bottom")) {
      setPadding(getPaddingLeft(), getPaddingTop(), getPaddingRight(), (int) (Integer.parseInt(value) * scale + 0.5f));
    } else if (key.equals("padding-left")) {
      setPadding((int) (Integer.parseInt(value) * scale + 0.5f), getPaddingTop(), getPaddingRight(), getPaddingBottom());
    } else if (key.equals("padding-right")) {
      setPadding(getPaddingLeft(), getPaddingTop(), (int) (Integer.parseInt(value) * scale + 0.5f), getPaddingBottom());
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
    this.setImage(bytes);
  }

  @Override
  public void reshape(int size) {
    // TODO Auto-generated method stub
    
  }

}
