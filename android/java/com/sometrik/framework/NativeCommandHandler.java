package com.sometrik.framework;

import android.view.View;

public interface NativeCommandHandler {
  public void handleCommand(NativeCommand command);
  public void addChild(View view);
  public void showView();
  public void addOption(int position, String text);
  
  public int getElementId();
}
