package com.sometrik.framework;

import java.util.ArrayList;

import com.sometrik.framework.NativeCommand.CommandType;

import android.R.color;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ExpandableListAdapter;
import android.widget.ExpandableListView;
import android.widget.ListView;

public class FWList extends ExpandableListView implements NativeCommandHandler{
  
  enum ColumnType { TEXT, NUMERIC, TIMESTAMP, ICON };

  private FrameWork frame;
  private FWAdapter adapter;
  
  public FWList(final FrameWork frame, final FWAdapter adapter) {
    super(frame);
    this.frame = frame;
    this.adapter = adapter;
    this.setAdapter((ExpandableListAdapter)adapter);
    this.setGroupIndicator(null);
    this.setAnimation(null);
    System.out.println("FWLIST constructor");
    this.setDescendantFocusability(ViewGroup.FOCUS_BLOCK_DESCENDANTS);
//  this.setGroupIndicator(frame.getResources().getDrawable(android.R.drawable.ic_menu_more));
    setOnGroupClickListener(new OnGroupClickListener(){
      @Override
      public boolean onGroupClick(ExpandableListView parent, View v, int groupPosition, long id) {
	System.out.println("groupClick detected " + groupPosition);
	if (groupPosition == 0) {
	  System.out.println("column title clicked. ignoring");
	} else {
	  System.out.println("row clicked. Sending intChangedEvent of " + (groupPosition - 1));
	  frame.intChangedEvent(System.currentTimeMillis() / 1000.0, getElementId(), 0, groupPosition - 1);
  	}
	return true;
      }
    });
    setOnChildClickListener(new OnChildClickListener(){
      @Override
      public boolean onChildClick(ExpandableListView parent, View v, int groupPosition, int childPosition, long id) {
	System.out.println("child clicked. Sending intChangedEvent of " + (groupPosition - 1) + " " + (childPosition - 1));
	frame.intChangedEvent(System.currentTimeMillis() / 1000.0, getElementId(), childPosition - 1, groupPosition - 1);
	return true;
      }
    });

    //This collapses all other than the chosen group
    setOnGroupExpandListener(new OnGroupExpandListener() {
      @Override
      public void onGroupExpand(int groupPosition) {
	for (int i = 0; i < adapter.getGroupCount(); i++) {
	  if (i != groupPosition) {
	    if (isGroupExpanded(i)) {
	      collapseGroup(i);
	    }
	  }
	}
	Log.d("adapterExpansion", "expansionStuff done");
      }
    });
  }

  @Override
  public void addData(String text, int row, int column, int sheet){
    Log.d("FWList", "adding data for row " + row + " column " + column + " sheet " + sheet);
    ArrayList<String> dataRow = adapter.getDataRow(row, sheet);
    if (dataRow != null){
      Log.d("FWList", "row found adding data");
      while (dataRow.size() < column) { // Add empty cells if needed
	dataRow.add(dataRow.size(), "");
      }
      if (dataRow.size() > column) {
	Log.d("FWList", "replacing column " + column);
	dataRow.remove(column);
      }
      dataRow.add(column, text);
    } else {
      Log.d("FWList", "creating new row");
      dataRow = new ArrayList<String>();
      dataRow.add(text);
      adapter.addItem(row, sheet, dataRow);
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
    System.out.println("FWlist couldn't handle commnad addOption");
  }

  @Override
  public void setValue(String v) {
    adapter.addSheet(v);
  }

  @Override
  public void setValue(int v) {
    this.expandGroup(v + 1);
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

  @Override
  public void clear() {
    adapter.clear();
  }

  @Override
  public void flush() {
    adapter.notifyDataSetChanged();
    invalidate();
  }

  @Override
  public void addColumn(String text, int columnType) {
    System.out.println("AddOption " + columnType + " text");
    ColumnType type = ColumnType.values()[columnType - 1];
    System.out.println("columnType int: " + ColumnType.values()[columnType - 1]);
    adapter.addColumn(text);
    System.out.println("AddOption " + columnType + " text");
    adapter.notifyDataSetChanged();
    System.out.println("AddOption " + columnType + " text");
  }
}
