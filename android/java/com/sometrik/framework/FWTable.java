package com.sometrik.framework;

import java.util.ArrayList;

import android.content.Context;
import android.view.View;
import android.view.ViewParent;
import android.widget.TableLayout;
import android.widget.TableRow;

public class FWTable extends TableLayout implements NativeCommandHandler {
  
  FrameWork frame;
  int columnCount = 1;
  ArrayList<TableRow> rowList;
  ArrayList<View> dataList;
  
  public FWTable(Context context) {
    super(context);
    context = (FrameWork)context;
    rowList = new ArrayList<TableRow>();
    rowList.add(new TableRow(context));
    dataList = new ArrayList<View>();
  }
  
  private TableRow getCurrentRow(){
    if (dataList.size() / columnCount > rowList.size()){
      TableRow row = new TableRow(frame);
      rowList.add(row);
      return row;
    } else {
      TableRow row = rowList.get(rowList.size() - 1);
      return new TableRow(frame);
    }
  }
  
  public void setColumnCount(int columnCount) { this.columnCount = columnCount; };


  @Override
  public void addChild(View view) {
    TableRow row = getCurrentRow();
    row.addView(view);
    dataList.add(view);
  }

  @Override
  public void showView() {
    frame.setCurrentView(this);
  }

@Override
public void addOption(int position, String text) {
}

@Override
public int getElementId() {
  return getId();
}

@Override
public void removeChild(int id) {
  // TODO Auto-generated method stub
  
}



}
