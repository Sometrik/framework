package com.sometrik.framework;

class BitmapCache {
  private AssetManager assetManager;
  private float displayScale;
  private HashMap<String, Bitmap> bitmaps = new HashMap<>();

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
	  bitmaps.set(filename, bitmap);
	  stream.close();
	}
      } catch (IOException e) {
	e.printStackTrace();
      }      
    }
    return bitmap;
  }
};
