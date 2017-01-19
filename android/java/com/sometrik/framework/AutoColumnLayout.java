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
  private GridLayout layout;
  GridLayout.LayoutParams gridParam;
  LinearLayout.LayoutParams childParams;
  
  public AutoColumnLayout(FrameWork frameWork){
    super(frameWork);
    frame = frameWork;
    viewList = new ArrayList<View>();
    setOrientation(LinearLayout.VERTICAL);
    layout = new GridLayout(frame);

    childParams = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
    childParams.weight = 1.0f;
    childParams.gravity = Gravity.FILL;

    layout.setColumnCount(2);
    layout.setRowCount(5);
    GridLayout.LayoutParams param = new GridLayout.LayoutParams();
    param.height = LayoutParams.WRAP_CONTENT;
    param.width = LayoutParams.MATCH_PARENT;
    layout.setLayoutParams(param);
    addView(layout);
  }
  
  public void addChild(View view){
    layout.setLayoutParams(childParams);
    layout.addView(view);
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
//      buildAutoLayout(checkForBigView());
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
