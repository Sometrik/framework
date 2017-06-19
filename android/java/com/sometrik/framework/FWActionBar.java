package com.sometrik.framework;

import java.util.ArrayList;

import android.app.ActionBar;
import android.view.MenuItem;
import android.view.View;

public class FWActionBar implements NativeCommandHandler {
  
  int id;
  ActionBar actionBar;
  ArrayList<ActionBarItem> itemList;
  

  public FWActionBar(FrameWork frame, String title, int id){
    actionBar = frame.getActionBar();
    actionBar.setDisplayShowTitleEnabled(true);
    actionBar.setTitle(title);
    actionBar.show();
    itemList = new ArrayList<ActionBarItem>();
    this.id = id;
  }
  
  public ArrayList<ActionBarItem> getItemList(){
    return itemList;
  }
  
  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addChild(View view) {
    System.out.println("Command couldn't be handled by FWActionBar");
  }

  @Override
  public void addOption(int optionId, String text) {
    itemList.add(new ActionBarItem(optionId, text, "default"));
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    System.out.println("Command couldn't be handled by FWActionBar");
  }

  @Override
  public void setValue(String v) {
    actionBar.setTitle(v);
  }

  @Override
  public void setValue(int v) {
    if (v == 0){
      actionBar.hide();
    } else if (v == 1){
      actionBar.show();
    }
  }

  @Override
  public void setViewEnabled(Boolean enabled) {
    if (!enabled){
      actionBar.hide();
    } else {
      actionBar.show();
    }
  }

  @Override
  public void setViewVisibility(boolean visible) {
    if (!visible){
      actionBar.hide();
    } else {
      actionBar.show();
    }
  }

  @Override
  public void setStyle(String key, String value) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setError(boolean hasError, String errorText) {
    System.out.println("Command couldn't be handled by FWActionBar");
  }

  @Override
  public int getElementId() {
    return id;
  }

  
  public class ActionBarItem {

    int id;
    String name;
    String pictureAsset;

    public ActionBarItem(int id, String name, String pictureAsset) {
      this.id = id;
      this.name = name;
      this.pictureAsset = pictureAsset;
    }
  }


  @Override
  public void clear() {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void flush() {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addColumn(String text, int columnType) {
    // TODO Auto-generated method stub
    
  }
}
