package com.sometrik.framework;

import com.sometrik.framework.NativeCommand.Selector;

import android.R;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.graphics.Bitmap.Config;
import android.view.View;

public class FWNotification extends Notification.Builder implements NativeCommandHandler {

  private int id;
  private FrameWork frame;
  
  private static final int iconId = com.sometrik.lister.R.drawable.chaff_logo;
  
  public FWNotification(FrameWork frame, String title, String text) {
    super(frame);
    this.frame = frame;
    Intent intent = new Intent(frame, FrameWork.class);
    PendingIntent pIntent = PendingIntent.getActivity(frame, (int) System.currentTimeMillis(), intent, 0);

    setContentTitle(title);
    setContentText(text);
    setContentIntent(pIntent);
    setSmallIcon(iconId);
    setAutoCancel(true);
  }
  
  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addChild(View view) {

  }

  @Override
  public void addOption(int optionId, String text) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addColumn(String text, int columnType) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setValue(String v) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setImage(byte[] bytes, int width, int height, Config config) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setValue(int v) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void reshape(int value, int size) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void reshape(int size) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setViewVisibility(boolean visible) {
    if (visible) {
      Notification notif = getNotification();
      NotificationManager notificationManager = (NotificationManager) frame.getSystemService(FrameWork.NOTIFICATION_SERVICE);
      notificationManager.notify(4535312, notif);
    }
  }

  @Override
  public void setStyle(Selector selector, String key, String value) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setError(boolean hasError, String errorText) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void clear() {
    // TODO Auto-generated method stub
    
  }
  
  public void setId(int id) {
    this.id = id;
  }

  @Override
  public void flush() {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void deinitialize() {
    // TODO Auto-generated method stub
    
  }

  @Override
  public int getElementId() {
    return id;
  }

}
