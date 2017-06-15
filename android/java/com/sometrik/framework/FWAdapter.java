package com.sometrik.framework;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import android.graphics.Color;
import android.graphics.Typeface;
import android.text.TextUtils;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ExpandableListAdapter;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;

public class FWAdapter extends ArrayAdapter<View> implements ExpandableListAdapter {

  enum AdapterDataType { SHEET, DATA }
  
  private List<View> viewList;
//  private ArrayList<String> sheetList;
  private HashMap<Integer, AdapterData> dataList;
  private AdapterData columnData;
  private FrameWork frame;
  private int addedHeaders = 0;
  private LinearLayout.LayoutParams listItemParams;
  private boolean sheetsEnabled = false;
  
  public FWAdapter(FrameWork frame, List<View> viewList) {
    super(frame, 0, viewList);
    this.frame = frame;
    dataList = new HashMap<Integer, AdapterData>();
    columnData = new AdapterData(new ArrayList<String>());
    listItemParams = new LinearLayout.LayoutParams(0, LinearLayout.LayoutParams.MATCH_PARENT);
    listItemParams.weight = 1;
    listItemParams.leftMargin = 50;
  }

  public ArrayList<String> getDataRow(int row, int sheet) {
    if (sheet == 0) {
      AdapterData sheetData = dataList.get(sheet);
      if (sheetData == null || sheetData.type == AdapterDataType.DATA) {
	AdapterData data = dataList.get(row);
	if (data == null) {
	  Log.d("adapter", "no row found");
	  return null;
	} else {
	  Log.d("adapter", "row found");
	  return data.stringList;
	}
      }
      if (sheetData.getChildren().size() != 0) {
	AdapterData data = sheetData.getChild(row);
	if (data == null) {
	  Log.d("adapter", "no row found");
	  return null;
	} else {
	  Log.d("adapter", "row found");
	  return data.stringList;
	}
      } else {
	  Log.d("adapter", "no children - no row found");
	return null;
      }
      
    } else {
      AdapterData sheetData = dataList.get(sheet);
	  Log.d("adapter", "sheetData size: " + dataList.size());
      if (sheetData == null){
	  Log.d("adapter", "sheetData null");
	return null;
      }
      if (sheetData.getChildren().size() != 0) {
	AdapterData data = sheetData.getChild(row);
	if (data == null) {
	  Log.d("adapter", "no row found");
	  return null;
	} else {
	  Log.d("adapter", "row found");
	  return data.stringList;
	}
      } else {
	  Log.d("adapter", "no children - no row found");
	return null;
      }
    }
  }

  public void addItem(int row, int sheet, ArrayList<String> cellItems) {
    Log.d("adapter", "adding new dataList to row: " + row + " sheet: " + sheet);
    if (!sheetsEnabled) {
      dataList.put(row, new AdapterData(cellItems));
      return;
    } else {
      Log.d("adapter",  "datalist size: " + dataList.size());
      AdapterData sheetData = dataList.get(sheet);
      if (sheetData == null){
	Log.d("adapter", "no sheet found");
	return;
      }
      sheetData.addChild(row, new AdapterData(cellItems));
    }
  }
  
  public void addColumn(String columnText){
    columnData.addString(columnText);
  }
  
  public void addSheet(String sheetName){
    int size = dataList.size();
    ArrayList<String> sheet = new ArrayList<String>();
    sheet.add(sheetName);
    Log.d("adapter", "putting sheet to " + (size));
    sheetsEnabled = true;
    dataList.put(size, new AdapterData(sheet, AdapterDataType.SHEET));
  }

  @Override
  public int getCount() {
    Log.d("adapter", "getCount");
    int counter = 0;
    for (HashMap.Entry<Integer, AdapterData> pair : dataList.entrySet()) {
      if (pair.getValue().type == AdapterDataType.SHEET && pair.getValue().getChildren().size() == 0) {
	continue;
      } else {
	counter++;
      }
    }
    if (columnData.getSize() != 0) {
      return counter + 1;
    } else {
      return counter;
    }
  }

  @Override
  public long getItemId(int arg0) {
    Log.d("adapter", "getItemId");
    return 0;
  }
  
  @Override
  public void clear(){
    Log.d("adapter", "clear");
    Iterator<Entry<Integer, AdapterData>> iterator = dataList.entrySet().iterator();
    while (iterator.hasNext()) {
      Map.Entry<Integer, AdapterData> entry = iterator.next();
      if (entry.getValue().type == AdapterDataType.DATA) {
	iterator.remove();
      }
    }
  }

  @Override
  public View getView(int position, View convertView, ViewGroup parent) {

    Log.d("adapter", "position: " + position);
    LinearLayout layout = new LinearLayout(frame);
    AdapterData data;
    if (position == 0) {
      data = columnData;

      for (int i = 0; i < data.getSize(); i++) {
	TextView txtFirst = new TextView(frame);
	txtFirst.setLayoutParams(listItemParams);
	txtFirst.setTypeface(null, Typeface.BOLD);
	txtFirst.setFocusable(false);
	txtFirst.setFocusableInTouchMode(false);
	txtFirst.setClickable(false);
	layout.addView(txtFirst);
	txtFirst.setText(data.getData(i));
      }

      return layout;

    } else {
      Log.d("adapter", "dataKList.size: " + dataList.size());
      Log.d("adapter", "trying to get from index: " + (position));
      data = dataList.get(position);
    }

    if (data == null) {
      Log.d("adapter", "no data on position " + position);
      return layout;
    }

    for (int i = 0; i < data.getSize(); i++) {
      TextView txtFirst = new TextView(frame);
      txtFirst.setLayoutParams(listItemParams);
      txtFirst.setFocusable(false);
      txtFirst.setClickable(false);
      txtFirst.setFocusableInTouchMode(false);
      layout.addView(txtFirst);
      txtFirst.setText(data.getData(i));
    }

    return layout;
  }
  
  @Override
  public Object getChild(int groupPosition, int childPosition) {
    Log.d("adapter", "getChild");
    return null;
  }
  
  @Override
  public boolean isEnabled(int position) {
    return true;
  }

  @Override
  public long getChildId(int groupPosition, int childPosition) {
    Log.d("adapter", "getChildId");
    return 0;
  }

  @Override
  public View getChildView(int groupPosition, int childPosition, boolean isLastChild, View convertView, ViewGroup parent) {
    Log.d("adapter", "GetChildView position: " + groupPosition + " childPosition: " + childPosition);
    if (columnData.getSize() != 0){
      groupPosition--;
      Log.d("adapter", "GetChildView (changed) position: " + groupPosition + " childPosition: " + childPosition);
    }
    AdapterData sheetData = dataList.get(groupPosition);
    ArrayList<AdapterData> children = sheetData.getChildren();
    ScrollView scrollView = new ScrollView(frame);
    LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
    params.gravity = Gravity.FILL;
    scrollView.setLayoutParams(params);
    LinearLayout expandLayout = new LinearLayout(frame);
    expandLayout.setOrientation(LinearLayout.VERTICAL);
    expandLayout.setLayoutParams(params);
    scrollView.addView(expandLayout);


    Log.d("adapter", "Children size on getChildView: " + children.size());
    for (int i = 0; i < children.size(); i++) {
      AdapterData childData = sheetData.getChild(i);
      LinearLayout layout = new LinearLayout(frame);
      layout.setBackgroundColor(Color.parseColor("#d1d1d1"));
      for (int i2 = 0; i2 < childData.getSize(); i2++) {
	TextView txtFirst = new TextView(frame);
	txtFirst.setLayoutParams(listItemParams);
	txtFirst.setFocusable(false);
	txtFirst.setClickable(false);
	txtFirst.setSingleLine();
	txtFirst.setEllipsize(TextUtils.TruncateAt.END);
	txtFirst.setFocusableInTouchMode(false);
	layout.addView(txtFirst);
	txtFirst.setText(childData.getData(i2));
      }
      expandLayout.addView(layout);
    }

    return scrollView;
  }

  @Override
  public int getChildrenCount(int groupPosition) {
    Log.d("adapter", "getChildrenCount " + groupPosition);
    if (groupPosition == 0 || !sheetsEnabled){
      return 0;
    }
    Log.d("adapter", "getChildrenCount (changed) " + (groupPosition - 1));
    
    AdapterData sheetData = dataList.get(groupPosition - 1);
    if (sheetData == null){
      Log.d("adapter", "sheetData null");
      return 0;
    }
    if (sheetData.getChildren().size() > 0){
      Log.d("adapter", "children found");
      return 1;
    } else {
      Log.d("adapter", "no children found");
      return 0;
    }
  }

  @Override
  public long getCombinedChildId(long groupId, long childId) {
    Log.d("adapter", "getCombinedChildId");
    return 0;
  }

  @Override
  public long getCombinedGroupId(long groupId) {
    Log.d("adapter", "getCombinedGroupId");
    return 0;
  }

  @Override
  public Object getGroup(int groupPosition) {
    Log.d("adapter", "getGroup");
    
    return null;
  }

  @Override
  public int getGroupCount() {
    if (columnData.getSize() != 0) {
      return dataList.size() + 1;
    } else {
      return dataList.size();
    }
//    int counter = 0;
//    for (HashMap.Entry<Integer, AdapterData> pair : dataList.entrySet()) {
//      if (pair.getValue().type == AdapterDataType.SHEET && pair.getValue().getChildren().size() == 0) {
//	continue;
//      } else {
//	counter++;
//      }
//    }
//    if (columnData.getSize() != 0) {
//      return counter + 1;
//    } else {
//      return counter;
//    }
  }

  @Override
  public long getGroupId(int groupPosition) {
    Log.d("adapter", "getGroupId");
    return 0;
  }

  @Override
  public View getGroupView(int position, boolean arg1, View arg2, ViewGroup arg3) {
    Log.d("adapter", "GetGroupView position: " + position);
    LinearLayout layout = new LinearLayout(frame);
    AdapterData data;
    if (position == 0) {
      data = columnData;

      for (int i = 0; i < data.getSize(); i++) {
	TextView txtFirst = new TextView(frame);
	txtFirst.setLayoutParams(listItemParams);
	txtFirst.setTypeface(null, Typeface.BOLD);
	txtFirst.setTextAppearance(frame, android.R.style.TextAppearance_DeviceDefault_Small);
	txtFirst.setFocusable(false);
	txtFirst.setFocusableInTouchMode(false);
	txtFirst.setClickable(false);
	txtFirst.setPadding(0, 20, 0, 20);
	layout.addView(txtFirst);
	// layout.setBackgroundColor(0xFF777777);
	txtFirst.setText(data.getData(i));
      }

      return layout;

    } else {

      if (columnData.getSize() != 0) {
	position--;
      }

      Log.d("adapter", "dataKList.size: " + dataList.size());
      Log.d("adapter", "trying to get from index: " + (position));
      data = dataList.get(position);

//      if (data.type == AdapterDataType.SHEET && data.getChildren().size() == 0) {
//	layout.setVisibility(LinearLayout.INVISIBLE);
//	layout.setLayoutParams(new AbsListView.LayoutParams(-1, -2));
//	return layout;
//      }
	
      for (int i = 0; i < data.getSize(); i++) {
	Log.d("adapter", "looping throud data " + i);
	TextView txtFirst = new TextView(frame);
	txtFirst.setFocusable(false);
	txtFirst.setFocusableInTouchMode(false);
	txtFirst.setClickable(false);
	txtFirst.setTextAppearance(frame, android.R.style.TextAppearance_DeviceDefault_Small);
	txtFirst.setLayoutParams(listItemParams);
	layout.addView(txtFirst);
	txtFirst.setText(data.getData(i));
      }


      return layout;
    }
  }

  @Override
  public boolean isChildSelectable(int groupPosition, int childPosition) {
    return true;
  }

  @Override
  public void onGroupCollapsed(int groupPosition) {
    Log.d("adapter", "onGroupCollapsed");
    
  }

  @Override
  public void onGroupExpanded(int groupPosition) {
    Log.d("adapter", "onGroupExpanded");
    
  }
  
 
  public class AdapterData {
    
    private ArrayList<String> stringList;
    private boolean columnData = false;
    private ArrayList<AdapterData> children;
    private AdapterDataType type = AdapterDataType.DATA;
    
    public AdapterData(ArrayList<String> stringList) {
      this.stringList = stringList;
      children = new ArrayList<AdapterData>();
    }
    public AdapterData(ArrayList<String> stringList, AdapterDataType type) {
      this.type = type;
      this.stringList = stringList;
      children = new ArrayList<AdapterData>();
    }
    
    public void addString(String text){
      stringList.add(text);
    }

    public int getSize() {
      return stringList.size();
    }
    
    public void addChild(int row, AdapterData data){
      children.add(row, data);
    }

    public String getData(int position) {
      if (position < stringList.size()) {
	return stringList.get(position);
      } else {
	return stringList.get(0);
      }
    }

    public AdapterData getChild(int row) {
      System.out.println("getChild on AdpaterData");
      if (row >= children.size()) {
	System.out.println("no children");
	return null;
      }
      return children.get(row);
    }
    
    public ArrayList<AdapterData> getChildren() {
      return children;
    }
    
    public ArrayList<String> getList(){
      return stringList;
    }

  }

}


