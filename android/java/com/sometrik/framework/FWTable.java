package com.sometrik.framework;

import java.util.ArrayList;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap;
import android.util.Log;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.TableLayout;
import android.widget.TableRow;

public class FWTable extends TableLayout implements NativeCommandHandler {
  
  FrameWork frame;
  int columnCount = 4;
  ArrayList<TableRow> rowList;
  ArrayList<View> dataList;
  LinearLayout.LayoutParams params;
  TableRow.LayoutParams rowChildParams;
  boolean autoSize = false;
  
  public FWTable(FrameWork frameWork) {
    super(frameWork);
    frame = frameWork;
    rowChildParams = new TableRow.LayoutParams(TableRow.LayoutParams.MATCH_PARENT, TableRow.LayoutParams.WRAP_CONTENT);
    params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
    params.weight = 1;
    setLayoutParams(params);
    rowList = new ArrayList<TableRow>();
    TableRow firstRow = new TableRow(frame);
    firstRow.setLayoutParams(rowChildParams);
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
    } else if (dataList.size() != 0){
      Log.d("table", "Alert. mathi : " + ((dataList.size()) / columnCount));
      double mathi = (double)(dataList.size()) / (double)columnCount;
      Log.d("table", "Alert. Datalist size: " + (dataList.size() ));
      Log.d("table", "Alert. BLaag : " + ((dataList.size()) / columnCount));
      Log.d("table", "Alert. rowList size: " + rowList.size());
      Log.d("table", "Alert. mathi: " + mathi);
      if (mathi > rowList.size()) {
	return createNewRow();
      } else {
	return rowList.get(rowList.size() - 1);
      }
    } else {
	return rowList.get(rowList.size() - 1);
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
  
  public void resizeAutoSize() {
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
    view.setLayoutParams(rowChildParams);
    dataList.add(view);
    TableRow row = getCurrentRow(view);
    row.addView(view);
  }
  
  @Override
  public void addOption(int optionId, String text) { }

  @Override
  public int getElementId() {
    return getId();
  }

  @Override
  public void setValue(String v) { }

  @Override
  public void setValue(int v) { }

  @Override
  public void setStyle(Selector selector, String key, String value) { }

  @Override
  public void applyStyles() { }
  @Override
  public void setError(boolean hasError, String errorText) { }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    resizeAutoSize();
  }

  @Override
  public void addData(String text, int row, int column, int sheet) { }

  @Override
  public void setViewVisibility(boolean visibility) {
    if (visibility){
      this.setVisibility(VISIBLE);
    } else {
      this.setVisibility(INVISIBLE);
    }
  }

  @Override
  public void clear() { }

  @Override
  public void flush() { }

  @Override
  public void addColumn(String text, int columnType) { }

  @Override
  public void reshape(int value, int size) { }

  @Override
  public void setBitmap(Bitmap bitmap) { }

  @Override
  public void reshape(int size) { }

  @Override
  public void deinitialize() { }

  @Override
  public void addImageUrl(String url, int width, int height) { }
}
