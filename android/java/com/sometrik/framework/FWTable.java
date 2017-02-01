package com.sometrik.framework;

import java.util.ArrayList;

import android.content.Context;
import android.util.Log;
import android.view.View;
import android.view.ViewParent;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;

public class FWTable extends TableLayout implements NativeCommandHandler {
  
  FrameWork frame;
  int columnCount = 4;
  ArrayList<TableRow> rowList;
  ArrayList<View> dataList;
  
  public FWTable(FrameWork frameWork) {
    super(frameWork);
    frame = frameWork;
    rowList = new ArrayList<TableRow>();
    TableRow firstRow = new TableRow(frame);
    rowList.add(firstRow);
    addView(firstRow);
    dataList = new ArrayList<View>();
  }
  
  private TableRow getCurrentRow(){
    if ((dataList.size() + 1) / columnCount > rowList.size()) {
      Log.d("table", "New Row");
      TableRow row = new TableRow(frame);
      rowList.add(row);
      addView(row);
      return row;
    } else {
      return rowList.get(rowList.size() - 1);
    }
  }
  
  public void setColumnCount(int columnCount) { this.columnCount = columnCount; };


  @Override
  public void addChild(View view) {
    dataList.add(view);
    TableRow row = getCurrentRow();
    row.addView(view);
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
  public void setValue(String v) {
    System.out.println("FWTable couldn't handle command");
  }

  @Override
  public void setValue(int v) {
    System.out.println("FWTable couldn't handle command");
  }

  @Override
  public void setViewEnabled(Boolean enabled) {
    System.out.println("FWTable couldn't handle command");
  }

  @Override
  public void setStyle(String key, String value) { }

  @Override
  public void setError(boolean hasError, String errorText) { }
}
