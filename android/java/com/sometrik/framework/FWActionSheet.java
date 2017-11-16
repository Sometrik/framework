package com.sometrik.framework;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap;
import android.view.Menu;
import android.view.View;
import android.widget.PopupMenu;

public class FWActionSheet extends PopupMenu implements NativeCommandHandler {

  private FrameWork frame;
  private int id;
  private boolean hasEntries = false;
  
  public FWActionSheet(FrameWork frame, View anchor, int id) {
    super(frame, anchor);
    this.id = id;
   
    // Add dummy entry, or the menu will not have any width
    getMenu().add("No menu entries");
    show();
  }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) { }

  @Override
  public void addChild(View view) { }

  @Override
  public void addOption(int optionId, String text) {
    if (!hasEntries) {
      getMenu().clear();
      hasEntries = true;      
    }
    System.out.println("ActionSheet adding option " + optionId + " " + text);
    getMenu().add(Menu.NONE, optionId, Menu.NONE, text);
  }

  @Override
  public void addColumn(String text, int columnType) { }

  @Override
  public void addData(String text, int row, int column, int sheet) { }

  @Override
  public void setValue(String v) { }

  @Override
  public void setBitmap(Bitmap bitmap) { }

  @Override
  public void addImageUrl(String url, int width, int height) { }

  @Override
  public void setValue(int v) { }

  @Override
  public void reshape(int value, int size) { }

  @Override
  public void reshape(int size) { }

  @Override
  public void setViewVisibility(boolean visible) { }

  @Override
  public void setStyle(Selector selector, String key, String value) { }

  @Override
  public void setError(boolean hasError, String errorText) { }

  @Override
  public void clear() { }

  @Override
  public void flush() { }

  @Override
  public void deinitialize() {
    dismiss();
  }

  @Override
  public int getElementId() {
    return id;
  }

}
