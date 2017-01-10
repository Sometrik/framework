package com.sometrik.framework;

import android.view.View;

public interface NativeCommandHandler {
  public void addChild(View view);
  public void removeChild(int id);
  public void showView();
  public void addOption(int optionId, String text);
  
  public int getElementId();
}
