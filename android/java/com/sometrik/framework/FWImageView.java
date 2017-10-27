package com.sometrik.framework;

import java.net.URI;
import java.net.URISyntaxException;
import java.nio.ByteBuffer;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;

public class FWImageView extends ImageView implements NativeCommandHandler {
  FrameWork frame;
  ViewStyleManager normalStyle, activeStyle, currentStyle;
  Bitmap ownedBitmap = null;
  boolean imageRequestSent = false;
  String currentUrl = null;
  int currentWidth = 0, currentHeight = 0;
  
  public FWImageView(final FrameWork frame, int id, String imageUrl) {
    super(frame);
    this.frame = frame;
    this.setId(id);
    this.setScaleType(ScaleType.CENTER_CROP); // needed for parallax scrolling
    this.setClipToOutline(true);
    this.setAdjustViewBounds(true);
    
    if (imageUrl != null) {
      String protocol = getProtocol(imageUrl); 
      if (protocol.equals("http") || protocol.equals("https")) {
	currentUrl = imageUrl;
      } else {
	setImageFromAssets(imageUrl);
      }
    }
        
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
    
    final FWImageView image = this;

    setOnClickListener(new OnClickListener() {
      @Override
      public void onClick(View arg0) {
	if (!FrameWork.transitionAnimation) {
	  frame.intChangedEvent(getElementId(), 0, 0);
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
      String scheme = uri.getScheme();
      return scheme != null ? scheme : "asset";
    } catch (URISyntaxException e) {
      return "asset";
    }
  }
    
  private void setImageFromAssets(String filename) {
    deinitialize();
    Bitmap bitmap = frame.bitmapCache.loadBitmap(filename);
    setImageBitmap(bitmap);
    invalidate();
  }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) { }

  @Override
  public void addChild(View view) { }

  @Override
  public void addOption(int optionId, String text) { }

  @Override
  public void addColumn(String text, int columnType) { }

  @Override
  public void addData(String text, int row, int column, int sheet) { }

  @Override
  public void setValue(String v) {
    deinitialize();
    String protocol = getProtocol(v);
    if (protocol.equals("http") || protocol.equals("https")) {
      currentUrl = v;
      if (currentWidth != 0 && currentHeight != 0) requestImage();
    } else {
      currentUrl = null;
      setImageFromAssets(v);
    }
  }

  @Override
  public void setValue(int v) { }

  @Override
  public void reshape(int value, int size) { }

  @Override
  public void setViewVisibility(boolean visible) { }

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
  public void setError(boolean hasError, String errorText) { }

  @Override
  public void clear() { }

  @Override
  public void flush() { }

  @Override
  public int getElementId() {
    return getId();
  }
  
  private Bitmap createBitmap(int width, int height, int internalFormat) {
    switch (internalFormat) {
    case 3: return Bitmap.createBitmap(width, height, Bitmap.Config.RGB_565);
    case 4: return Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_4444);
    case 5: case 6: return Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
    }
    System.out.println("ERROR: unable to display format " + internalFormat);
    return null;
  }

  @Override
  public void setImage(byte[] bytes, int width, int height, int internalFormat) {
    deinitialize();
    
    ownedBitmap = createBitmap(width, height, internalFormat);
    if (ownedBitmap != null) {
      ByteBuffer buffer = ByteBuffer.wrap(bytes);
      ownedBitmap.copyPixelsFromBuffer(buffer);
    }
    this.setImageBitmap(ownedBitmap);
    
    invalidate();
    
    activeStyle.apply(this);
  }

  @Override
  public void reshape(int size) { }

  @Override
  public void deinitialize() {
    if (ownedBitmap != null) {
      ownedBitmap.recycle();
      ownedBitmap = null;
    } else if (imageRequestSent) {
      frame.cancelImageRequest(getElementId());
    }
  }

  @Override
  protected void onSizeChanged(int w, int h, int oldw, int oldh) {
    currentWidth = w;
    currentHeight = h;
    if (currentUrl != null) requestImage();
  }
  
  protected void requestImage() {
    if (currentWidth > 0) {
      System.out.println("Sending image request for " + currentUrl);
      imageRequestSent = true;
      final float scale = getContext().getResources().getDisplayMetrics().density;
      frame.sendImageRequest(getElementId(), currentUrl, (int)(currentWidth / scale), 0);
    } else {
      System.out.println("Unable to sent image request for " + currentUrl);
    }
  }
}
