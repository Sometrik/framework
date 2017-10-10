package com.sometrik.framework;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

import com.sometrik.framework.NativeCommand.Selector;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;

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
  public void setStyle(Selector selector, String key, String value) {
    System.out.println("ImageView style " + key + " " + value);
    final float scale = getContext().getResources().getDisplayMetrics().density;
    if (key.equals("gravity")) {
      if (getParent() instanceof LinearLayout) {
	LinearLayout.LayoutParams params = (LinearLayout.LayoutParams) getLayoutParams();
	if (value.equals("bottom")) {
	  params.gravity = Gravity.BOTTOM;
	} else if (value.equals("top")) {
	  params.gravity = Gravity.TOP;
	} else if (value.equals("left")) {
	  params.gravity = Gravity.LEFT;
	} else if (value.equals("right")) {
	  params.gravity = Gravity.RIGHT;
	} else if (value.equals("center")) {
	  params.gravity = Gravity.CENTER;
	} else if (value.equals("center-horizontal")) {
	  params.gravity = Gravity.CENTER_HORIZONTAL;
	} else if (value.equals("center-vertical")) {
	  params.gravity = Gravity.CENTER_VERTICAL;
	}
	setLayoutParams(params);
      } else {
	System.out.println("Gravity cannot be assigned to this FWImageView");
      }
    } else if (key.equals("width")) {
      ViewGroup.LayoutParams params = getLayoutParams();
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
      ViewGroup.LayoutParams params = getLayoutParams();
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
      if (getParent() instanceof LinearLayout) {
	LinearLayout.LayoutParams params = (LinearLayout.LayoutParams) getLayoutParams();
	params.weight = Integer.parseInt(value);
	setLayoutParams(params);
      } else {
	System.out.println("Weight cannot be assigned to this FWImageView");
      }
    } else if (key.equals("padding-top")) {
      setPadding(getPaddingLeft(), (int) (Integer.parseInt(value) * scale + 0.5f), getPaddingRight(), getPaddingBottom());
    } else if (key.equals("padding-bottom")) {
      setPadding(getPaddingLeft(), getPaddingTop(), getPaddingRight(), (int) (Integer.parseInt(value) * scale + 0.5f));
    } else if (key.equals("padding-left")) {
      setPadding((int) (Integer.parseInt(value) * scale + 0.5f), getPaddingTop(), getPaddingRight(), getPaddingBottom());
    } else if (key.equals("padding-right")) {
      setPadding(getPaddingLeft(), getPaddingTop(), (int) (Integer.parseInt(value) * scale + 0.5f), getPaddingBottom());
    } else if (key.equals("scale")) {
      if (value.equals("start")) {
	setScaleType(ScaleType.FIT_START);
      } else if (value.equals("end")) {
	setScaleType(ScaleType.FIT_END);
      } else if (value.equals("center")) {
	setScaleType(ScaleType.FIT_CENTER);
      }
    } else if (key.equals("margin-right")) {
      if (getParent() instanceof LinearLayout) {
	LinearLayout.LayoutParams params = (LinearLayout.LayoutParams) getLayoutParams();
	params.rightMargin = Integer.parseInt(value);
	setLayoutParams(params);
      }
    } else if (key.equals("margin-left")) {
      if (getParent() instanceof LinearLayout) {
	LinearLayout.LayoutParams params = (LinearLayout.LayoutParams) getLayoutParams();
	params.leftMargin = Integer.parseInt(value);
	setLayoutParams(params);
      }
    } else if (key.equals("margin-top")) {
      if (getParent() instanceof LinearLayout) {
	LinearLayout.LayoutParams params = (LinearLayout.LayoutParams) getLayoutParams();
	params.topMargin = Integer.parseInt(value);
	setLayoutParams(params);
      }
    } else if (key.equals("margin-bottom")) {
      if (getParent() instanceof LinearLayout) {
	LinearLayout.LayoutParams params = (LinearLayout.LayoutParams) getLayoutParams();
	params.bottomMargin = Integer.parseInt(value);
	setLayoutParams(params);
      }
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
  public void setImage(byte[] bytes, int width, int height, Bitmap.Config config) {
    System.out.println("SetImage " + bytes.length + " " + getWidth());
//    Bitmap map = BitmapFactory.decodeByteArray(bytes, 0, bytes.length);
//    BitmapFactory.decodeStream(bytes);
//    BitmapFactory.de
//    InputStream myInputStream = new ByteArrayInputStream(bytes); 
//
//  Bitmap map = BitmapFactory.decodeStream(myInputStream);
//    if (map == null) {
//      System.out.println("shieet");
//    } else {
//      System.out.println("mappi: " + map.getHeight());
//    } 

	final float scale = getContext().getResources().getDisplayMetrics().density;
//      System.out.println("picture scale: " + scale + " " + width + " " + height);
//    Bitmap bmp = Bitmap.createBitmap((int)scale * width, (int)scale * height, config);
    Bitmap bmp = Bitmap.createBitmap(width, height, config);
//  Bitmap bmp = Bitmap.createBitmap((int[])bytes, 32, 32, Bitmap.Config.ARGB_8888)
    ByteBuffer buffer = ByteBuffer.wrap(bytes);
    bmp.copyPixelsFromBuffer(buffer);
//    Bitmap map = Bitmap.create
    this.setImageBitmap(bmp);
//    map = null;
    invalidate();
  }

  @Override
  public void reshape(int size) {
    // TODO Auto-generated method stub
    
  }

}
