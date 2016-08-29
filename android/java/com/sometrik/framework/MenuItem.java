package com.sometrik.framework;

import android.content.Context;
import android.graphics.drawable.Drawable;

public class MenuItem {

  int mode;
  int id;
  String name;
  private String media;
  private Drawable icon;

  public MenuItem(int mode, int id, String name) {

    this.mode = mode;
    this.id = id;
    this.name = name;
    media = "nothing";
  }

  public MenuItem(Context context, int id, String name, String media) {

    this.id = id;
    this.name = name;
    this.media = media;
    mode = 0;
    if (media.equals("facebook")) {
      int iconId = context.getResources().getIdentifier("facebook_icon", "drawable", context.getPackageName());
      System.out.println("yarr");
      icon = context.getResources().getDrawable(iconId);
      System.out.println("yarr");
    } else if (media.equals("twitter")) {
      int iconId = context.getResources().getIdentifier("twitter_icon", "drawable", context.getPackageName());
      icon = context.getResources().getDrawable(iconId);
    } else if (media.equals("instagram")) {
      int iconId = context.getResources().getIdentifier("instagram_icon", "drawable", context.getPackageName());
      icon = context.getResources().getDrawable(iconId);
    }
    System.out.println("yarr");
  }

  public int getMode() {
    return mode;
  }

  public int getId() {
    return id;
  }

  public String getName() {
    return name;
  }

  public String getMedia() {
    return media;
  }

  public void setMedia(String media) {
    this.media = media;
  }

  public Drawable getIcon() {
    return icon;
  }

  public void setIcon(Drawable icon) {
    this.icon = icon;
  }

}