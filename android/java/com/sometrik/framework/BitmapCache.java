package com.sometrik.framework;

import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

class BitmapCache {
  private AssetManager assetManager;
  private float displayScale;
  private HashMap<String, Bitmap> bitmaps = new HashMap<String, Bitmap>();

  public BitmapCache(AssetManager assetManager, float displayScale) {
    this.assetManager = assetManager;
    this.displayScale = displayScale;
  }
    
  public Bitmap loadBitmap(String filename) {
    Bitmap bitmap = bitmaps.get(filename);
    if (bitmap == null) {
      try {       
	InputStream stream = assetManager.open(filename);
	if (stream != null) {
	  bitmap = BitmapFactory.decodeStream(stream);
	  bitmaps.put(filename, bitmap);
	  stream.close();
	}
      } catch (IOException e) {
	e.printStackTrace();
      }      
    }
    return bitmap;
  }
};
