package com.sometrik.framework;

import android.graphics.SurfaceTexture;
import android.view.Surface;

public class NativeSurface extends Surface{
  
  private int id;

  public NativeSurface(SurfaceTexture surfaceTexture) {
    super(surfaceTexture);
  }

  public int getId() {
    return id;
  }

  public void setId(int id) {
    this.id = id;
  }

}
