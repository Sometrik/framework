package com.sometrik.framework;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;

import android.app.ActionBar;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.view.View;

public class FWActionBar implements NativeCommandHandler {
  
  int id;
  ActionBar actionBar;
  ArrayList<ActionBarItem> itemList;
  FrameWork frame;
  

  public FWActionBar(FrameWork frame, String title, int id){
    this.frame = frame;
    actionBar = frame.getActionBar();
    actionBar.setDisplayShowTitleEnabled(true);
    actionBar.setTitle(title);
    actionBar.show();
    actionBar.setDisplayUseLogoEnabled(false);
    ColorDrawable cd = new ColorDrawable();
    cd.setColor(Color.WHITE);
    actionBar.setBackgroundDrawable(cd);
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
    if (key.equals("icon")){
      try {
	InputStream stream = frame.getAssets().open(value);
	Bitmap b = BitmapFactory.decodeStream(stream);
	Drawable d = new BitmapDrawable(b);
	actionBar.setIcon(d);
      } catch (IOException e) {
	e.printStackTrace();
      }
    }
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

  @Override
  public void reshape(int value, int size) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setImage(byte[] bytes) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void reshape(int size) {
    // TODO Auto-generated method stub
    
  }
}
