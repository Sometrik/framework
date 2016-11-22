package com.sometrik.framework;

import android.view.View;

public interface NativeMessageHandler {
  public void handleCommand(NativeCommand command);
  public void addChild(View view);
  public void showView();
  
  public int getElementId();
}
