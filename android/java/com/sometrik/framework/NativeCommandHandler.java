package com.sometrik.framework;

import android.view.View;

public interface NativeCommandHandler {
  public void addChild(View view);
  public void removeChild(int id);
  public void addOption(int optionId, String text);
  public void setValue(String v);
  public void setValue(int v);
  public void setEnabled(Boolean enabled);
  public void setStyle(String key, String value);
  public void setError(String error);
  
  public int getElementId();
}
