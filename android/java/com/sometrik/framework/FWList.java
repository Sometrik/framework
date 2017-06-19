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
  private int ownerId = 0;
  
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
	  if (ownerId == 0) {
	    frame.intChangedEvent(System.currentTimeMillis() / 1000.0, getElementId(), groupPosition - 1, 0);
	  } else {
	    System.out.println("ownerId " + ownerId);
	    frame.intChangedEvent(System.currentTimeMillis() / 1000.0, ownerId, groupPosition - 1, 0);
	  }
  	}
	return true;
      }
    });
    setOnChildClickListener(new OnChildClickListener(){
      @Override
      public boolean onChildClick(ExpandableListView parent, View v, int groupPosition, int childPosition, long id) {
	System.out.println("child clicked. Sending intChangedEvent of " + (groupPosition - 1) + " " + (childPosition - 1));
	if (ownerId == 0) {
	  frame.intChangedEvent(System.currentTimeMillis() / 1000.0, getElementId(), groupPosition - 1, childPosition - 1);
	} else {
	  frame.intChangedEvent(System.currentTimeMillis() / 1000.0, ownerId, groupPosition - 1, childPosition - 1);
	}
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
  
  public void setOwnerId(int ownerId){ this.ownerId = ownerId; }

  @Override
  public void addData(String text, int row, int column, int sheet){
    // System.out.println("FWList adding data for row " + row + " column " + column + " sheet " + sheet);
    ArrayList<String> dataRow = adapter.getDataRow(row, sheet);
    if (dataRow != null){
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
  
  @Override
  public void onVisibilityChanged(View changedView, int visibility) {
    switch (visibility) {
    case VISIBLE:
      frame.visibilityChangedEvent(System.currentTimeMillis() / 1000.0, getId(), true);
      break;
    case GONE:
    case INVISIBLE: 
      frame.visibilityChangedEvent(System.currentTimeMillis() / 1000.0, getId(), false);
    break;
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
    adapter.notifyDataSetChanged();
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
    adapter.addColumn(text);
    System.out.println("AddOption " + columnType + " text");
    adapter.notifyDataSetChanged();
    System.out.println("AddOption " + columnType + " text");
  }

  @Override
  public void reshape(int value, int size) {
    adapter.reshape(value, size);
    adapter.notifyDataSetChanged();
  }
}
