package com.sometrik.framework;

import java.util.ArrayList;

import android.content.Context;
import android.content.res.Configuration;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.GridLayout;
import android.widget.GridLayout.Spec;
import android.widget.LinearLayout;

public class AutoColumnLayout extends LinearLayout implements NativeCommandHandler {
  
  //For small view AutoLayout puts views vertically
  //For big view Autolayout groups 2 views next to each other
  
  private ArrayList<View> viewList;
  private FrameWork frame;
  private int maxColumns;
  private LayoutParams params;
  
  public AutoColumnLayout(FrameWork frameWork){
    super(frameWork);
    frame = frameWork;
    viewList = new ArrayList<View>();
    setOrientation(LinearLayout.VERTICAL);
    
    
  }
  
  private void buildAutoLayout(){
    
    ArrayList<LinearLayout> rowList = new ArrayList<LinearLayout>();
    LinearLayout row = new LinearLayout(frame);
    rowList.add(row);
    int columnCounter = 0;
    
    while (true) { 
      LinearLayout column = new LinearLayout(frame);
      fillColumn(column);
      row.addView(column);
      columnCounter++;
      if (viewList.size() == 0){
	break;
      } else if (columnCounter >= maxColumns){
	row = new LinearLayout(frame);
	rowList.add(row);
      }
    }
    
    for (LinearLayout rowFromList : rowList){
      addView(rowFromList);
    }
  }
  
  private Boolean fillColumn(LinearLayout column) {
    int counter = 0;
    while (true) {
      column.addView(viewList.get(counter));
      counter ++;
      if (counter >= 3 || viewList.size() - counter == 0 ){
	for (int i = 0; i < counter; i++){
	  viewList.remove(0);
	}
	break;
      }
    }
    if (counter >= 3) {
      return true;
    } else {
      return false;
    }
  }

  public void addChild(View view){
    viewList.add(view);
  }

  private boolean checkForBigView(){
    //TODO Truly detect screen size
   int orientation =  frame.getResources().getConfiguration().orientation;
   if (orientation == Configuration.ORIENTATION_PORTRAIT){
     return true;
   } else {
     return false;
   }
  }

  @Override
  public void removeChild(int id) {
    for (int i = 0; i < getChildCount(); i++) {
      View v = getChildAt(i);
      if (v.getId() == id) {
	removeView(v);
	System.out.println("Child " + id + " removed succesfully");
	return;
      }
    }
    System.out.println("Child " + id + " wasn't found and wasn't removed");
  }

  @Override
  public void addOption(int optionId, String text) {
    System.out.println("AutoColumnLayout couldn't handle command");
  }

  @Override
  public void setValue(String v) {
    System.out.println("AutoColumnLayout couldn't handle command");
  }

  @Override
  public void setValue(int v) {
    if (v > 0){
      buildAutoLayout();
      frame.setContentView(this);
    }
  }

  @Override
  public int getElementId() {
    return getId();
  }

  @Override
  public void setEnabled(Boolean enabled) {
    System.out.println("AutoColumnLayout couldn't handle command");
  }
  
  @Override
  public void setStyle(String key, String value) { }

  @Override
  public void setError(boolean hasError, String errorText) { }
}
