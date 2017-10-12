package com.sometrik.framework;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

import com.sometrik.framework.NativeCommand.Selector;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnTouchListener;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

public class FWImageView extends ImageView implements NativeCommandHandler {

  FrameWork frame;
  ViewStyleManager normalStyle, activeStyle, currentStyle;
  
  public FWImageView(final FrameWork frame) {
    super(frame);
    this.frame = frame;
    LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
    this.setLayoutParams(params);
    this.setScaleType(ScaleType.FIT_CENTER);
    this.setClipToOutline(true);
    
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(scale, true);
    this.activeStyle = new ViewStyleManager(scale, false);
    
    final FWImageView image = this;

    setOnClickListener(new OnClickListener() {
      @Override
      public void onClick(View arg0) {
	if (!FrameWork.transitionAnimation) {
	  frame.intChangedEvent(System.currentTimeMillis() / 1000.0, getElementId(), 1, 0);
	}
      }
    });
    
    setOnTouchListener(new OnTouchListener() {
      @Override
      public boolean onTouch(View v, MotionEvent event) {
	if (event.getAction() == MotionEvent.ACTION_DOWN) {
	  image.currentStyle = image.activeStyle;
	  image.currentStyle.apply(image);
	} else if (event.getAction() == MotionEvent.ACTION_UP) {
	  image.currentStyle = image.normalStyle;
	  image.currentStyle.apply(image);
	}
	return false;
      }
    });
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
    if (selector == Selector.NORMAL) {
      normalStyle.setStyle(key, value);
      if (normalStyle == currentStyle) normalStyle.apply(this);
    } else if (selector == Selector.ACTIVE) {
      activeStyle.setStyle(key, value);      
      if (activeStyle == currentStyle) activeStyle.apply(this);
    }
    
    if (key.equals("scale")) {
      if (value.equals("fit-start")) {
	setScaleType(ScaleType.FIT_START);
      } else if (value.equals("fit-end")) {
	setScaleType(ScaleType.FIT_END);
      } else if (value.equals("fit-center")) {
	setScaleType(ScaleType.FIT_CENTER);
      } else if (value.equals("center")) {
	setScaleType(ScaleType.CENTER);
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
