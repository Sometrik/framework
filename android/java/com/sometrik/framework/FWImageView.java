package com.sometrik.framework;

import java.net.URI;
import java.net.URISyntaxException;
import java.util.TreeSet;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap;
import android.view.View;
import android.widget.ImageView;

public class FWImageView extends ImageView implements NativeCommandHandler {
  final static int RGB565 = 3;
  final static int RGBA4 = 4;
  final static int RGBA8 = 5;
  final static int RGB8 = 6;
  
  FrameWork frame;
  ViewStyleManager normalStyle, activeStyle, currentStyle;
  Bitmap ownedBitmap = null;
  boolean imageRequestSent = false;
  int currentWidth = 0, currentHeight = 0;
  TreeSet<ImageData> images = new TreeSet<ImageData>();
  
  public FWImageView(final FrameWork frame, int id) {
    super(frame);
    this.frame = frame;
    this.setId(id);
    this.setScaleType(ScaleType.CENTER_CROP); // needed for parallax scrolling
    this.setClipToOutline(true);
    this.setAdjustViewBounds(true);
    this.setClickable(false);
        
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);    
  }
  
  @Override
  public void addImageUrl(String url, int width, int height) {
    String protocol = getProtocol(url); 
    if (protocol.equals("http") || protocol.equals("https")) {
      images.add(new ImageData(width, height, url));
    } else {
      setImageFromAssets(url);
    }
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
    images.clear();
    String protocol = getProtocol(v);
    if (protocol.equals("http") || protocol.equals("https")) {
      addImageUrl(v, 0, 0);
      if (currentWidth != 0 && currentHeight != 0) requestImage();
    } else {
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
  public void clear() {
    deinitialize();
    setImageBitmap(null);
    images.clear();
  }

  @Override
  public void flush() { }

  @Override
  public int getElementId() {
    return getId();
  }
  
  @Override
  public void setBitmap(Bitmap bitmap) {
    releaseBitmap();  
    ownedBitmap = bitmap;
    setImageBitmap(ownedBitmap);
  }

  @Override
  public void reshape(int size) { }

  private void releaseBitmap() {
    if (ownedBitmap != null) {
      ownedBitmap.recycle();
      ownedBitmap = null;
    }
  }
  
  private void cancelImageRequest() {
    if (imageRequestSent) {
      frame.cancelImageRequest(getElementId());
    }
  }
  
  @Override
  public void deinitialize() {
    releaseBitmap();
    cancelImageRequest();    
  }

  @Override
  protected void onSizeChanged(int w, int h, int oldw, int oldh) {
    cancelImageRequest();
    currentWidth = w;
    currentHeight = h;
    requestImage();
  }
  
  protected void requestImage() {
    if (currentWidth > 0) {
	final float scale = getContext().getResources().getDisplayMetrics().density;
      if (!images.isEmpty()) {
	ImageData img = getSuitable(currentWidth);
	System.out.println("Sending image request for " + img.url);
	imageRequestSent = true;
	ImageData smallest = images.first();
	if (false && smallest.width < img.width) {
	  frame.sendURLImageRequest(getElementId(), smallest.url, (int)(currentWidth / scale), 0, 0);
	}
	if (true) {
	  frame.sendURLImageRequest(getElementId(), img.url, (int)(currentWidth / scale), 0, RGB565);
	}
      } else {
	frame.sendImageRequest(getElementId(), (int)(currentWidth / scale), (int)(currentHeight / scale), RGB565);
      }
    } else {
      System.out.println("Unable to sent image request");
    }
  }
  
  private ImageData getSuitable(int target_width) {
    for (ImageData d : images) {
      if (4 * d.width >= 3 * target_width) { // select image if it's at least 0.75 * target_width
	return d;
      }
    }
    if (!images.isEmpty()) {
      return images.last();
    } else {
      return null;
    }
  }
  
  private class ImageData implements Comparable<ImageData> {
    public int width;
    public int height;
    public String url;
    
    public ImageData(int width, int height, String url) {
      this.width = width;
      this.height = height;
      this.url = url;
    }

    @Override
    public int compareTo(ImageData other) {
      return Integer.compare(width, other.width);
    }
  }
}
