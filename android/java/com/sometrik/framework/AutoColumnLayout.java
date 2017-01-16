package com.sometrik.framework;

import java.util.ArrayList;

import android.content.Context;
import android.content.res.Configuration;
import android.view.View;
import android.widget.LinearLayout;

public class AutoColumnLayout extends LinearLayout {
  
  //For small view AutoLayout puts views vertically
  //For big view Autolayout groups 2 views next to each other
  
  private ArrayList<View> viewList;
  private Context context;
  private FrameWork frame;
  
  public AutoColumnLayout(Context context){
    super(context);
    this.context = context;
    frame = (FrameWork)context;
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
    
    if (bigView) {
      for (int i = 0; i < viewList.size(); i++) {
	LinearLayout layout = new LinearLayout(context);
	layout.setOrientation(LinearLayout.HORIZONTAL);
	layout.addView(viewList.get(i));

	if (viewList.size() - 1 > i) {
	  layout.addView(viewList.get(i + 1));
	  i++;
	}
	addView(layout);
      }

    } else {
      for (View view : viewList) {
	addView(view);
      }
    }
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
  

}
