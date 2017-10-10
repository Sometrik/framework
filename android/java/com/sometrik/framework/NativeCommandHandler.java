package com.sometrik.framework;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.view.View;

public interface NativeCommandHandler {
  public void onScreenOrientationChange(boolean isLandscape);
  public void addChild(View view);
  public void addOption(int optionId, String text);
  public void addColumn(String text, int columnType);
  public void addData(String text, int row, int column, int sheet);
  public void setValue(String v);
  public void setImage(byte[] bytes, int width, int height, Config config);
  public void setValue(int v);
  public void reshape(int value, int size);
  public void reshape(int size);
  public void setViewEnabled(Boolean enabled);
  public void setViewVisibility(boolean visible);
  public void setStyle(Selector selector, String key, String value);
  public void setError(boolean hasError, String errorText);
  public void clear();
  public void flush();
  
  public int getElementId();
}
