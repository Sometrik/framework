package com.sometrik.framework;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;

public class NativeLooper extends Thread {
  
  public native void test();
  public Handler nHandler;
  
  public NativeLooper(){
    System.out.println("Looper created");
  }

  public void run() {
      Looper.prepare();

      nHandler = new Handler() {
          public void handleMessage(Message msg) {
            System.out.println("message for you sir");
            test();
          }
      };
  }
  
  public void loop(){
    Looper.loop();
  }
  
  public void print(){
    System.out.println("NativeLooper reporting");
  }
  
  public Handler getHandler(){
    return nHandler;
  }
}