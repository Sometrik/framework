package com.sometrik.framework;

public interface NativeMessageHandler {
  public void handleMessage(NativeMessage message);
  
  public int getId();
}
