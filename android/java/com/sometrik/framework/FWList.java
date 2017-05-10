package com.sometrik.framework;

import java.util.ArrayList;

import android.R.color;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ExpandableListAdapter;
import android.widget.ExpandableListView;
import android.widget.ListView;

public class FWList extends ExpandableListView implements NativeCommandHandler{

  private FrameWork frame;
  private FWAdapter adapter;
  
  public FWList(final FrameWork frame, final FWAdapter adapter) {
    super(frame);
    this.frame = frame;
    this.adapter = adapter;
    this.setAdapter((ExpandableListAdapter)adapter);
    this.setGroupIndicator(null);
//  this.setGroupIndicator(frame.getResources().getDrawable(android.R.drawable.ic_menu_more));
    setOnGroupClickListener(new OnGroupClickListener(){
      @Override
      public boolean onGroupClick(ExpandableListView parent, View v, int groupPosition, long id) {
	System.out.println("groupClick detected " + groupPosition);
	if (groupPosition == 0) {
	  System.out.println("column title clicked. ignoring");
	} else {
	  System.out.println("row clicked. Sending intChangedEvent of " + (groupPosition - 1));
	  frame.intChangedEvent(System.currentTimeMillis() / 1000.0, getElementId(), groupPosition - 1);
  	}
	return false;
      }
    });
    setOnChildClickListener(new OnChildClickListener(){
      @Override
      public boolean onChildClick(ExpandableListView parent, View v, int groupPosition, int childPosition, long id) {
	System.out.println("childClick detected group: " + groupPosition + " child: " + childPosition);
	return false;
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
    if (optionId == 0){
      adapter.addColumn(text);
    } else {
      Log.d("FWList", "adding header to row " + optionId);
      adapter.addSectionHeader(optionId, text);
    }
    adapter.notifyDataSetChanged();
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
