package com.sometrik.framework;

import android.graphics.drawable.Drawable;

public class OptionsItem {

  int id;
  String name;
  int icon;

  public OptionsItem(int id, String name) {

    this.id = id;
    this.name = name;
  }

  public OptionsItem(int id, String name, int icon) {

    this.id = id;
    this.name = name;
    this.icon = icon;
  }

  public int getId() {
    return id;
  }

  public String getName() {
    return name;
  }

}
