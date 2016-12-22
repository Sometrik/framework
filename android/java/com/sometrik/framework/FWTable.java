package com.sometrik.framework;

import java.util.ArrayList;

import android.content.Context;
import android.view.View;
import android.widget.TableLayout;
import android.widget.TableRow;

public class FWTable extends TableLayout implements NativeCommandHandler {
  
  Context context;
  int columnCount = 1;
  int rowCounter = 0;
  ArrayList<TableRow> rowList;
  ArrayList<View> dataList;
  
  public FWTable(Context context) {
    super(context);
    this.context = context;
    rowList = new ArrayList<TableRow>();
    dataList = new ArrayList<View>();
  }
  
  private TableRow getCurrentRow(){
    if (rowCounter > columnCount){
      TableRow row = new TableRow(context);
      rowList.add(row);
      rowCounter = 0;
      return row;
    } else {
      TableRow row = rowList.get(rowList.size() - 1);
      return new TableRow(context);
    }
  }
  
  public void setColumnCount(int columnCount) { this.columnCount = columnCount; };



@Override
public void addChild(View view) {
//  addRow();
  System.out.println("FWTable couldn't handle addChild command");
}

@Override
public void showView() {
  System.out.println("FWTable couldn't handle showView command");
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
