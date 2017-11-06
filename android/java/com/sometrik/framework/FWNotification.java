package com.sometrik.framework;

import com.sometrik.framework.NativeCommand.Selector;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.graphics.Bitmap;
import android.view.View;

public class FWNotification extends Notification.Builder implements NativeCommandHandler {

  private int id;
  private FrameWork frame;

  public FWNotification(FrameWork frame, String title, String text) {
    super(frame);
    this.frame = frame;
    Intent intent = new Intent(frame, FrameWork.class);
    PendingIntent pIntent = PendingIntent.getActivity(frame, (int) System.currentTimeMillis(), intent, 0);
    setContentTitle(title);
    setContentText(text);
    setContentIntent(pIntent);
    setSmallIcon(android.R.drawable.arrow_down_float);
    
    //Attempt at getting icon from application
//    List<PackageInfo> packs = frame.getPackageManager().getInstalledPackages(0);
//
//    System.out.println("packs size: " + packs.size());
//
//    for (PackageInfo p : packs) {
//      System.out.println("packs 1: " + p.applicationInfo.icon);
//      if (p.applicationInfo.icon != 0) {
//	setSmallIcon(p.applicationInfo.icon);
//	break;
//      }
//    }
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
  public void setBitmap(Bitmap bitmap) { }

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
      notificationManager.notify(234235, notif);
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

  @Override
  public void addImageUrl(String url, int width, int height) {
    // TODO Auto-generated method stub
    
  }

}
