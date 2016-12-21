package com.sometrik.framework;

import android.content.Context;
import android.view.View;
import android.widget.TableLayout;
import android.widget.TableRow;

public class FWTable extends TableLayout implements NativeCommandHandler {
  
  Context context;
  int columnCount = 1;
  
  public FWTable(Context context) {
    super(context);
    this.context = context;
  }
  
  private void addRow(){
    TableRow row = new TableRow(context);
    //TODO
  }
  
  public void setColumnCount(int columnCount) { this.columnCount = columnCount; };



@Override
public void addChild(View view) {
  addRow();
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
