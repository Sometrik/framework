package com.sometrik.framework;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.content.Context;
import android.graphics.Typeface;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ExpandableListAdapter;
import android.widget.LinearLayout;
import android.widget.TextView;

public class FWAdapter extends ArrayAdapter<View> implements ExpandableListAdapter {

  private List<View> viewList;
//  private ArrayList<AdapterData> dataList;
  private HashMap<Integer, AdapterData> dataList;
  private AdapterData columnData;
  private ArrayList<Integer> sectionHeaderRows;
  private FrameWork frame;
  private int addedHeaders = 0;
  private LinearLayout.LayoutParams listItemParams;
  
  public FWAdapter(Context context, List<View> viewList) {
    super(context, 0, viewList);
    this.frame = (FrameWork) context;
//    dataList = new ArrayList<AdapterData>();
    dataList = new HashMap<Integer, AdapterData>();
    columnData = new AdapterData(new ArrayList<String>());
    listItemParams = new LinearLayout.LayoutParams(0, LinearLayout.LayoutParams.WRAP_CONTENT);
    listItemParams.weight = 1;
    listItemParams.leftMargin = 2;
    sectionHeaderRows = new ArrayList<Integer>();
  }

  public ArrayList<String> getDataRow(int rowId) {
    // if (dataList.size() >= rowId) {
    AdapterData data = dataList.get(rowId);
    if (data == null) {
      Log.d("adapter", "no row found");
      return null;
    }
    if (data.dataType.equals("section")) {
      return null;
    }
    return data.stringList;
    // }
  }
  
  public void addItem(int row, ArrayList<String> cellItems){
    Log.d("adapter",  "adding new dataList to row " + row);
    dataList.put(row, new AdapterData(cellItems));
  }
  
  public void addColumn(String columnText){
    columnData.addString(columnText);
  }
  
  public void addSectionHeader(int row, String sectionHeader){
    sectionHeaderRows.add(row);
    dataList.put(row, new AdapterData(sectionHeader));
    System.out.println("section added. Datalist size: " + dataList.size());
  }

  @Override
  public int getCount() {
    if (columnData.getSize() != 0){
      return dataList.size() + 1;
    }
    return dataList.size();
  }

  @Override
  public long getItemId(int arg0) {
    return 0;
  }
  
  @Override
  public void clear(){
    dataList = new HashMap<Integer, AdapterData>();
  }

  @Override
  public View getView(int position, View convertView, ViewGroup parent) {

    Log.d("adapter", "position: " + position);
    LinearLayout layout = new LinearLayout(frame);
    AdapterData data;
    if (position == 0){
      data = columnData;

      for (int i = 0; i < data.getSize(); i++) {
	TextView txtFirst = new TextView(frame);
	txtFirst.setLayoutParams(listItemParams);
	txtFirst.setTypeface(null, Typeface.BOLD);
	layout.addView(txtFirst);
	txtFirst.setText(data.getData(i));
      }

      return layout;

    } else {
      Log.d("adapter", "dataKList.size: " + dataList.size());
      if (columnData.getSize() != 0) {
	Log.d("adapter", "trying to get from index: " + (position));
	for (Integer sectionRow : sectionHeaderRows) {
	  if (position == sectionRow) {
	    Log.d("adapter", "sectionRow found");
//	    String sectionText = sectionHeaders.get(sectionRow);
		data = dataList.get(position);
		String sectionText = data.getData(0);
		Log.d("adapter", "section text: " + sectionText);
	    TextView section = new TextView(frame);
	    section.setLayoutParams(listItemParams);
	    section.setTypeface(null, Typeface.BOLD);
	    layout.addView(section);
	    section.setText(sectionText);
//	    addedHeaders++;
	    return layout;
	  }
	}
	data = dataList.get(position);
//	if (dataList.size() == position){
//	    addedHeaders = 0;
//	}
      } else {
	data = dataList.get(position);
      }
      
      if (data == null) {
	Log.d("adapter", "no data on position " + position);
      }

      for (int i = 0; i < data.getSize(); i++) {
	    Log.d("adapter", "looping throud data " + i);
	TextView txtFirst = new TextView(frame);
	txtFirst.setLayoutParams(listItemParams);
	layout.addView(txtFirst);
	txtFirst.setText(data.getData(i));
      }

      return layout;
    }
    
  }
  
  public class AdapterData {

    private ArrayList<String> stringList;
    private boolean columnData = false;
    private String dataType = "data";
    
    public AdapterData(ArrayList<String> stringList) {
      this.stringList = stringList;
    }
    public AdapterData(String sectionTitle) {
      dataType = "section";
      stringList = new ArrayList<String>();
      stringList.add(sectionTitle);
    }
    
    public void addString(String text){
      stringList.add(text);
    }

    public int getSize() {
      return stringList.size();
    }

    public String getData(int position) {
      if (position < stringList.size()) {
	return stringList.get(position);
      } else {
	return stringList.get(0);
      }
    }
    
    public ArrayList<String> getList(){
      return stringList;
    }

  }

  @Override
  public Object getChild(int groupPosition, int childPosition) {
    // TODO Auto-generated method stub
    return null;
  }

  @Override
  public long getChildId(int groupPosition, int childPosition) {
    // TODO Auto-generated method stub
    return 0;
  }

  @Override
  public View getChildView(int groupPosition, int childPosition, boolean isLastChild, View convertView, ViewGroup parent) {
    // TODO Auto-generated method stub
    return null;
  }

  @Override
  public int getChildrenCount(int groupPosition) {
    // TODO Auto-generated method stub
    return 0;
  }

  @Override
  public long getCombinedChildId(long groupId, long childId) {
    // TODO Auto-generated method stub
    return 0;
  }

  @Override
  public long getCombinedGroupId(long arg0) {
    // TODO Auto-generated method stub
    return 0;
  }

  @Override
  public Object getGroup(int arg0) {
    // TODO Auto-generated method stub
    return null;
  }

  @Override
  public int getGroupCount() {
    // TODO Auto-generated method stub
    return 0;
  }

  @Override
  public long getGroupId(int arg0) {
    // TODO Auto-generated method stub
    return 0;
  }

  @Override
  public View getGroupView(int arg0, boolean arg1, View arg2, ViewGroup arg3) {
    // TODO Auto-generated method stub
    return null;
  }

  @Override
  public boolean isChildSelectable(int arg0, int arg1) {
    // TODO Auto-generated method stub
    return false;
  }

  @Override
  public void onGroupCollapsed(int arg0) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void onGroupExpanded(int arg0) {
    // TODO Auto-generated method stub
    
  }
}


