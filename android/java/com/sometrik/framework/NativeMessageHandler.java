package com.sometrik.framework;

public interface NativeMessageHandler {
  public void handleCommand(NativeCommand command);
  public void showView();
  
  public int getElementId();
}
