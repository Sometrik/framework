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
  
  public FWTable(FrameWork frameWork) {
    super(frameWork);
    frame = frameWork;
    rowList = new ArrayList<TableRow>();
    rowList.add(new TableRow(frame));
    dataList = new ArrayList<View>();
  }
  
  private TableRow getCurrentRow(){
    if (dataList.size() / columnCount > rowList.size()){
      TableRow row = new TableRow(frame);
      rowList.add(row);
      return row;
    } else {
      return rowList.get(rowList.size() - 1);
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
  public void addOption(int optionId, String text) {
    System.out.println("FWTable couldn't handle command");
  }

  @Override
  public int getElementId() {
    return getId();
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
  public void setValue(String v) {
    System.out.println("FWTable couldn't handle command");
  }

  @Override
  public void setValue(int v) {
    System.out.println("FWTable couldn't handle command");
  }

  @Override
  public void setEnabled(Boolean enabled) {
    System.out.println("FWTable couldn't handle command");
  }
}
