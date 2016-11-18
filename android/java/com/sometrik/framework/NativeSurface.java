package com.sometrik.framework;

import android.content.Context;
import android.view.SurfaceView;

public class NativeSurface extends SurfaceView {
  
  private Context context;
  
  public NativeSurface(Context context) {
    super(context);
    this.context = context;
  }

  private int id;


  public int getId() {
    return id;
  }

  public void setId(int id) {
    this.id = id;
  }

}
