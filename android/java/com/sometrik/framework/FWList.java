package com.sometrik.framework;

import java.util.ArrayList;

import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ExpandableListAdapter;
import android.widget.ExpandableListView;
import android.widget.ListView;

public class FWList extends ExpandableListView implements NativeCommandHandler{

  private FrameWork frame;
  private FWAdapter adapter;
  
  public FWList(final FrameWork frame, FWAdapter adapter) {
    super(frame);
    this.frame = frame;
    this.adapter = adapter;
    this.setAdapter((ExpandableListAdapter)adapter);
    setOnItemClickListener(new OnItemClickListener(){

      @Override
      public void onItemClick(AdapterView<?> parent, View view, int position, long rowId) {
	System.out.println("itemClick detected " + position);
	frame.intChangedEvent(System.currentTimeMillis() / 1000.0, getElementId(), position);
      }
      
    });
  }
  
  @Override
  public void addData(String text, int row, int column, int sheet){
    Log.d("FWList", "adding data for row " + row + " column " + column);
    ArrayList<String> dataRow = adapter.getDataRow(row);
    if (dataRow != null){
      Log.d("FWList", "row found adding data");
      dataRow.add(column, text);
    } else {
      Log.d("FWList", "creating new row");
      dataRow = new ArrayList<String>();
      dataRow.add(text);
      adapter.addItem(row, dataRow);
    }
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
    if (optionId == 0){
      adapter.addColumn(text);
    } else {
      Log.d("FWList", "adding header to row " + optionId);
      adapter.addSectionHeader(optionId, text);
    }
  }

  @Override
  public void setValue(String v) {
  }

  @Override
  public void setValue(int v) {
    if (v == 0){
      adapter.clear();
    }
  }

  @Override
  public void setViewEnabled(Boolean enabled) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setStyle(String key, String value) {
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
    if (visibility){
      this.setVisibility(VISIBLE);
    } else {
      this.setVisibility(INVISIBLE);
    }
  }
}
