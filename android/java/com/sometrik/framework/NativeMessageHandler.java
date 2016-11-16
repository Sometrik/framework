package com.sometrik.framework;

public interface NativeMessageHandler {
  public void handleMessage(NativeMessage message);
  public void showView();
  
  public int getElementId();
}
