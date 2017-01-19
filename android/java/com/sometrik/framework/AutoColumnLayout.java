package com.sometrik.framework;

import java.util.ArrayList;

import android.content.Context;
import android.content.res.Configuration;
import android.view.View;
import android.widget.GridLayout;
import android.widget.GridLayout.Spec;
import android.widget.LinearLayout;

public class AutoColumnLayout extends LinearLayout implements NativeCommandHandler {
  
  //For small view AutoLayout puts views vertically
  //For big view Autolayout groups 2 views next to each other
  
  private ArrayList<View> viewList;
  private Context context;
  private FrameWork frame;
  
  public AutoColumnLayout(FrameWork frameWork){
    super(frameWork);
    frame = frameWork;
    viewList = new ArrayList<View>();
    setOrientation(LinearLayout.VERTICAL);
  }
  
  public void addChild(View view){
    viewList.add(view);
  }
  
  public void showView(){
    buildAutoLayout(checkForBigView());
    frame.setContentView(this);
  }
  
  private void buildAutoLayout(Boolean bigView) {
    
    GridLayout layout = new GridLayout(context);

    Spec rowSpan = GridLayout.spec(GridLayout.UNDEFINED, 2);
    Spec colspan = GridLayout.spec(GridLayout.UNDEFINED, 3);
    GridLayout.LayoutParams gridParam = new GridLayout.LayoutParams(rowSpan, colspan);

    for (int i = 0; i < viewList.size(); i++){
      layout.addView(viewList.get(i), gridParam);
    }
    
//    if (bigView) {
//      for (int i = 0; i < viewList.size(); i++) {
//	LinearLayout layout = new LinearLayout(context);
//	layout.setOrientation(LinearLayout.HORIZONTAL);
//	layout.addView(viewList.get(i));
//
//	if (viewList.size() - 1 > i) {
//	  layout.addView(viewList.get(i + 1));
//	  i++;
//	}
//	addView(layout);
//      }
//
//    } else {
//      for (View view : viewList) {
//	addView(view);
//      }
//    }
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
      frame.setCurrentView(this);
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
  

}
