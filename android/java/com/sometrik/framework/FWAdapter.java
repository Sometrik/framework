package com.sometrik.framework;

import java.util.ArrayList;
import java.util.List;

import android.content.Context;
import android.util.DisplayMetrics;
import android.util.TypedValue;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsListView;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.GridView;
import android.widget.LinearLayout;

public class FWAdapter extends ArrayAdapter<View> {

  private List<View> viewList;
  
  public FWAdapter(Context context, List<View> viewList) {
    super(context, 0, viewList);
    this.viewList = viewList;
  }
  
  public void addItem(View view){
    viewList.add(view);
  }

  @Override
  public int getCount() {
	System.out.println("getCount called with count of: " + viewList.size());
    return viewList.size();
  }

  @Override
  public long getItemId(int arg0) {
    return 0;
  }

  @Override
  public View getView(int position, View convertView, ViewGroup parent) {

	System.out.println("Abina: size: " + viewList.size() + " position: " + position);
    View view;
    if (convertView == null) {  
      System.out.println("creating new abin");
      DisplayMetrics displaymetrics = new DisplayMetrics();
      int dp = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 35, displaymetrics );
      AbsListView.LayoutParams params = new AbsListView.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, 130);
	view = viewList.get(position);
	view.setLayoutParams(params);
    } else {
	System.out.println("recycle abin");
	view = convertView;
    }
    return view;
  }
  
}
