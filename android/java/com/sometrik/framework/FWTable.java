package com.sometrik.framework;

import java.util.ArrayList;

import android.content.Context;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewParent;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;

public class FWTable extends TableLayout implements NativeCommandHandler {
  
  FrameWork frame;
  int columnCount = 4;
  ArrayList<TableRow> rowList;
  ArrayList<View> dataList;
  ViewGroup.LayoutParams params;
  boolean autoSize = false;
  
  public FWTable(FrameWork frameWork) {
    super(frameWork);
    frame = frameWork;
    params = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
    setLayoutParams(params);
    rowList = new ArrayList<TableRow>();
    TableRow firstRow = new TableRow(frame);
    firstRow.setLayoutParams(params);
    rowList.add(firstRow);
    addView(firstRow);
    dataList = new ArrayList<View>();
  }

  private TableRow getCurrentRow(View view) {
    if (autoSize) {
      // Check that row wouldn't go over the screen
      TableRow row = rowList.get(rowList.size() - 1);
      Log.d("table", "Row Size Check. Before: " + frame.measureViewWidth(row));
      Log.d("table", "Row Size Check. New: " + frame.measureViewWidth(view));
      Log.d("table", "Row Size Check. Screen Size: " + frame.getScreenWidth());
      if (frame.measureViewWidth(row) + frame.measureViewWidth(view) > (frame.getScreenWidth())) {
	Log.d("table", "Too big yo");
	return createNewRow();
      } else {
	return rowList.get(rowList.size() - 1);
      }
    } else {
      if ((dataList.size() + 1) / columnCount > rowList.size()) {
	return createNewRow();
      } else {
	return rowList.get(rowList.size() - 1);
      }
    }
  }
  
  private TableRow createNewRow() {
    Log.d("table", "New Row");
    TableRow row = new TableRow(frame);
    row.setLayoutParams(params);
    rowList.add(row);
    addView(row);
    return row;
  }
  
  public void resizeAutoSize(){
    if (autoSize){
      Log.d("table", "Resizing autoSize table");
      
      for (TableRow row : rowList){
	row.removeAllViews();
      }
      this.removeAllViews();
      rowList = new ArrayList<TableRow>();
      TableRow firstRow = new TableRow(frame);
      firstRow.setLayoutParams(params);
      rowList.add(firstRow);
      for (View view : dataList) {
	TableRow row = getCurrentRow(view);
	row.addView(view);
      }
    }
  }
  
  public void setAutoSize(boolean enabled){
    autoSize = enabled;
  }
  
  public void setColumnCount(int columnCount) { this.columnCount = columnCount; };

 

  @Override
  public void addChild(View view) {
    dataList.add(view);
    TableRow row = getCurrentRow(view);
    row.addView(view);
    Log.d("table", "Ellipsis Full Row: " + frame.measureViewWidth(row));
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

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    resizeAutoSize();
  }
}
