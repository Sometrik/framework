package com.sometrik.framework;

import java.util.ArrayList;
import java.util.HashMap;

import com.sometrik.framework.FWList.ColumnType;
import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap.Config;
import android.graphics.Typeface;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;

public class FWList extends ListView implements NativeCommandHandler {

  enum ColumnType {
    TEXT, NUMERIC, TIMESTAMP, ICON
  };

  private FrameWork frame;
  private FWAdapter2 adapter;
  private ArrayList<String> headers;
  private View currentHeaderView;
  private HashMap<Integer, ColumnType> columnTypes;
  private FWLayout stickyHeader;

  public static int defaultColumnWeight = 3;
  public static int timestampColumnWeight = 1;
  public static int iconColumnWeight = 3;
  public static int numericColumnWeight = 1;
  

  public FWList(final FrameWork frame, final FWAdapter2 adapter) {
    super(frame);
    this.frame = frame;
    this.adapter = adapter;
    this.setAdapter( adapter);
    this.setAnimation(null);
    columnTypes = new HashMap<Integer, ColumnType>();
    headers = new ArrayList<String>();
    System.out.println("FWLIST constructor");
    this.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    // System.out.println("FWList adding data for row " + row + " column " +
    // column + " sheet " + sheet);
    ArrayList<String> dataRow = adapter.getDataRow(row, sheet);
    if (dataRow != null) {
      // System.out.println("FWList row found adding data");
      while (dataRow.size() < column) { // Add empty cells if needed
	dataRow.add(dataRow.size(), "");
      }
      if (dataRow.size() > column) {
	// System.out.println("FWList replacing column " + column);
	dataRow.remove(column);
      }
      dataRow.add(column, text);
    } else {
      // System.out.println("FWList creating new row");
      dataRow = new ArrayList<String>();
      dataRow.add(text);
      adapter.addItem(row, sheet, dataRow);
    }
    adapter.notifyDataSetChanged();
  }
  
  public void addStickyHeader(FWLayout stickyHeader){
    this.stickyHeader = stickyHeader;
  }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub

  }

  @Override
  public void addChild(View view) {
    // TODO Auto-generated method stub

  }

  @Override
  public void addOption(int optionId, String text) {
    System.out.println("FWlist couldn't handle commnad addOption");
  }

  @Override
  public void setValue(String v) {
//    adapter.addSheet(v);
    adapter.notifyDataSetChanged();
  }

  @Override
  public void setValue(int v) {
//    this.expandGroup(v + 1);
  }

  @Override
  public void setStyle(Selector selector, String key, String value) {
    // TODO Auto-generated method stub

  }

  @Override
  public void setError(boolean hasError, String errorText) {
    // TODO Auto-generated method stub

  }

  @Override
  public int getElementId() {
    return getId();
  }

  @Override
  public void setViewVisibility(boolean visibility) {
    if (visibility) {
      this.setVisibility(VISIBLE);
    } else {
      this.setVisibility(INVISIBLE);
    }
  }

  @Override
  public void clear() {
    adapter.clear();
    adapter.notifyDataSetChanged();
  }

  @Override
  public void flush() {
    System.out.println("FWList flush");
    adapter.notifyDataSetChanged();
    invalidate();
  }

  @Override
  public void addColumn(String text, int columnType) {
    System.out.println("AddOption " + columnType + " text");
    ColumnType type = ColumnType.values()[columnType - 1];
    System.out.println("columnType int: " + ColumnType.values()[columnType - 1]);
    headers.add(text);
    adapter.notifyDataSetChanged();

    adapter.setColumnType(type, headers.size() - 1);
    View headerView = createHeaderView();
    if (getHeaderViewsCount() > 0){
      this.removeHeaderView(currentHeaderView);
    }
    currentHeaderView = headerView;
    addHeaderView(headerView);
  }
  
  private View createHeaderView(){

    LinearLayout layout = new LinearLayout(frame);
    layout.setOrientation(LinearLayout.HORIZONTAL);
    layout.setPadding(0, 20, 0, 20);

    System.out.println(getId() + " headers size: " + headers.size());
    for (int i = 0; i < headers.size(); i++) {
      System.out.println(getId() + " add header: " + headers.get(i));
      TextView txtFirst = new TextView(frame);
      txtFirst.setFocusable(false);
      txtFirst.setFocusableInTouchMode(false);
      txtFirst.setClickable(false);
      txtFirst.setTextSize(10);
      txtFirst.setLayoutParams(adapter.getColumnParameters(i));
      txtFirst.setSingleLine();
      txtFirst.setText(headers.get(i));
      txtFirst.setTypeface(null, Typeface.BOLD);
      layout.addView(txtFirst);
    }
    
    return layout;
  }

  @Override
  public void reshape(int value, int size) {
    adapter.reshape(value, size);
    adapter.notifyDataSetChanged();
  }

  @Override
  public void setImage(byte[] bytes, int width, int height, Config config) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void reshape(int size) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void deinitialize() {
    // TODO Auto-generated method stub
    
  }
}
