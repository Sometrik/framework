package com.sometrik.framework;

import java.util.ArrayList;

import android.content.Context;
import android.util.Log;
import android.view.View;
import android.widget.ListView;

public class FWList extends ListView implements NativeCommandHandler{

  private FrameWork frame;
  private FWAdapter adapter;
  
  public FWList(FrameWork frame, FWAdapter adapter) {
    super(frame);
    this.frame = frame;
    this.adapter = adapter;
    setAdapter(adapter);
  }
  
  public void addData(int row, int column, String text){
    Log.d("FWList", "adding data for row " + row + " column " + column);
    ArrayList<String> dataRow = adapter.getDataRow(row);
    if (dataRow != null){
      Log.d("FWList", "row found adding data");
      dataRow.add(column, text);
    } else {
      Log.d("FWList", "creating new row");
      dataRow = new ArrayList<String>();
      dataRow.add(text);
      adapter.addItem(dataRow);
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
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setValue(String v) {
    // TODO Auto-generated method stub
    
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

}
