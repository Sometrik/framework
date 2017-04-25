package com.sometrik.framework;

import android.view.View;

public interface NativeCommandHandler {
  public void onScreenOrientationChange(boolean isLandscape);
  public void addChild(View view);
  public void addOption(int optionId, String text);
  public void addData(String text, int row, int column, int sheet);
  public void setValue(String v);
  public void setValue(int v);
  public void setViewEnabled(Boolean enabled);
  public void setViewVisibility(boolean visible);
  public void setStyle(String key, String value);
  public void setError(boolean hasError, String errorText);
  
  public int getElementId();
}
