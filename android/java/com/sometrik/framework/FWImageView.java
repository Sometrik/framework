package com.sometrik.framework;

import java.net.URI;
import java.net.URISyntaxException;
import java.nio.ByteBuffer;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;

public class FWImageView extends ImageView implements NativeCommandHandler {
  FrameWork frame;
  ViewStyleManager normalStyle, activeStyle, currentStyle;
  Bitmap ownedBitmap = null;
  
  public FWImageView(final FrameWork frame) {
    super(frame);
    this.frame = frame;
    // LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
    // this.setLayoutParams(params);
    // this.setScaleType(ScaleType.FIT_CENTER);
    this.setScaleType(ScaleType.CENTER_CROP); // needed for parallax scrolling
    this.setClipToOutline(true);
    this.setAdjustViewBounds(true);
        
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
    
    final FWImageView image = this;

    setOnClickListener(new OnClickListener() {
      @Override
      public void onClick(View arg0) {
	if (!FrameWork.transitionAnimation) {
	  frame.intChangedEvent(System.currentTimeMillis() / 1000.0, getElementId(), 0, 0);
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

  private String getProtocol(String filename) {
    try {
      URI uri = new URI(filename);
      return uri.getScheme();
    } catch (URISyntaxException e) {
      return "asset";
    }
  }
    
  public void setImageFromAssets(String filename) {
    deinitialize();
    Bitmap bitmap = frame.bitmapCache.loadBitmap(filename);
    setImageBitmap(bitmap);
    invalidate();
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
    deinitialize();
    if (getProtocol(v) != "http") {
      setImageFromAssets(v);
    }
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
    deinitialize();
    
    ownedBitmap = Bitmap.createBitmap(width, height, config);
    ByteBuffer buffer = ByteBuffer.wrap(bytes);
    ownedBitmap.copyPixelsFromBuffer(buffer);
    this.setImageBitmap(ownedBitmap);
    
    invalidate();
    
    activeStyle.apply(this);
  }

  @Override
  public void reshape(int size) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void deinitialize() {
    if (ownedBitmap != null) {
      ownedBitmap.recycle();
      ownedBitmap = null;
    }
  }

}
