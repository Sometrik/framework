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

  enum AdapterDataType {
    SHEET, DATA
  }

  private List<View> viewList;
  private ArrayList<HashMap> sheetList;
  private HashMap<Integer, AdapterData> dataList;
//  private AdapterData columnData;
  private FrameWork frame;
  private LinearLayout.LayoutParams listItemParams;
  private boolean sheetsEnabled = false;

  public FWAdapter(FrameWork frame, List<View> viewList) {
    super(frame, 0, viewList);
    this.frame = frame;
    dataList = new HashMap<Integer, AdapterData>();
    sheetList = new ArrayList<HashMap>();
    sheetList.add(dataList);
//    columnData = new AdapterData(new ArrayList<String>());
    listItemParams = new LinearLayout.LayoutParams(0, LinearLayout.LayoutParams.MATCH_PARENT);
    listItemParams.weight = 1;
    listItemParams.leftMargin = 50;
  }

  public ArrayList<String> getDataRow(int row, int sheet) {
    if (sheet == 0) {
      HashMap<Integer, AdapterData> sheetData = sheetList.get(0);
//      if (sheetData == null || sheetData.type == AdapterDataType.DATA) {
	AdapterData data = sheetData.get(row);
	if (data == null) {
	  Log.d("adapter", "no row found");
	  return null;
	} else {
	  Log.d("adapter", "row found");
	  return data.stringList;
	}
	
//      if (sheetData.getChildren().size() != 0) {
//	AdapterData data = sheetData.getChild(row);
//	if (data == null) {
//	  Log.d("adapter", "no row found");
//	  return null;
//	} else {
//	  Log.d("adapter", "row found");
//	  return data.stringList;
//	}
//      } else {
//	Log.d("adapter", "no children - no row found");
      // return null;
      // }

    } else {
      // AdapterData sheetData = dataList.get(sheet);
      HashMap<Integer, AdapterData> sheetData = sheetList.get(sheet);
      // Log.d("adapter", "sheetData size: " + dataList.size());
      if (sheetData == null) {
	Log.d("adapter", "sheetData null");
	return null;
      }
      AdapterData data = sheetData.get(row);
      if (data == null) {
	Log.d("adapter", "no row found");
	return null;
      } else {
	Log.d("adapter", "row found");
	return data.stringList;
      }
    }
  }

  public void addItem(int row, int sheet, ArrayList<String> cellItems) {
    Log.d("adapter", "adding new dataList to row: " + row + " sheet: " + sheet);
    HashMap<Integer, AdapterData> sheetData = sheetList.get(sheet);
    
    AdapterData data = new AdapterData(cellItems);
    sheetData.put(row, data);
    
//    if (!sheetsEnabled) {
//      dataList.put(row, new AdapterData(cellItems));
//      return;
//    } else {
//      Log.d("adapter", "datalist size: " + dataList.size());
//      AdapterData sheetData = dataList.get(sheet);
//      if (sheetData == null) {
//	Log.d("adapter", "no sheet found");
//	return;
//      }
//      sheetData.addChild(row, new AdapterData(cellItems));
//    }
  }
  
  public void addSheet(String sheetName) {
    HashMap<Integer, AdapterData> newList = new HashMap<Integer, AdapterData>();
    sheetList.add(newList);
  }

  public void reshape(int sheet, int size) {
    if (size == dataList.size()){
      return;
    }
    if (size < dataList.size()) {
      // Currently removes rows from the end
      ArrayList<Integer> keyList = new ArrayList<Integer>();
      for (Entry<Integer, AdapterData> entry : dataList.entrySet()) {
	keyList.add(entry.getKey());
      }

      for (int i = 0; i < size; i++) {
	int key = keyList.get(keyList.size() - 1);
	dataList.remove(key);
      }
    } else {
      int difference = size - dataList.size();
      System.out.println("adding " + difference + " new rows");
      for (int i = 0; i < difference; i++) {
	ArrayList<String> newSheet = new ArrayList<String>();
	dataList.put(dataList.size(), new AdapterData(newSheet, AdapterDataType.SHEET));
      }
    }
    notifyDataSetChanged();
  }

  @Override
  public int getCount() {
    System.out.println("adapter getCount " + sheetList.size());
    return sheetList.size();
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
  public long getItemId(int arg0) {
    Log.d("adapter", "getItemId");
    return 0;
  }

  @Override
  public void clear() {
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
    if (convertView != null) {
      System.out.println("Data already set");
      return convertView;
    }
    Log.d("adapter", "position: " + position);
    LinearLayout layout = new LinearLayout(frame);
    AdapterData data = dataList.get(position);
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
//    Log.d("adapter", "GetChildView position: " + groupPosition + " childPosition: " + childPosition);
//    if (columnData.getSize() != 0) {
//      groupPosition--;
//      Log.d("adapter", "GetChildView (changed) position: " + groupPosition + " childPosition: " + childPosition);
//    }
//    HashMap<Integer, AdapterData> sheetData = sheetList.get(groupPosition);
//    ArrayList<String> data = sheetData.get(childPosition).getList();
//    
//    LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
//    params.gravity = Gravity.FILL;
//    LinearLayout expandLayout = new LinearLayout(frame);
//    expandLayout.setOrientation(LinearLayout.HORIZONTAL);
//    expandLayout.setLayoutParams(params);
//
//    Log.d("adapter", "Children size on getChildView: " + data.size());;
//    for (int i = 0; i < data.size(); i++) {
//      TextView txtFirst = new TextView(frame);
//      txtFirst.setLayoutParams(listItemParams);
//      txtFirst.setFocusable(false);
//      txtFirst.setClickable(false);
//      txtFirst.setSingleLine();
//      txtFirst.setEllipsize(TextUtils.TruncateAt.END);
//      txtFirst.setFocusableInTouchMode(false);
//      txtFirst.setText(data.get(i));
//      expandLayout.addView(txtFirst);
//      }
//
    return convertView;
  }

  @Override
  public int getChildrenCount(int groupPosition) {
    Log.d("adapter", "getChildrenCount " + groupPosition);
    return 0;
//    HashMap<Integer, AdapterData> sheetData = sheetList.get(groupPosition);
//    return sheetData.size();
    
//    if (groupPosition == 0 || !sheetsEnabled) {
//      return 0;
//    }
//    Log.d("adapter", "getChildrenCount (changed) " + (groupPosition - 1));
//
//    AdapterData sheetData = dataList.get(groupPosition - 1);
//    if (sheetData == null) {
//      Log.d("adapter", "sheetData null");
//      return 0;
//    }
//    if (sheetData.getChildren().size() > 0) {
//      Log.d("adapter", "children found");
//      return 1;
//    } else {
//      Log.d("adapter", "no children found");
//      return 0;
//    }
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
    return dataList.size();
  }

  @Override
  public long getGroupId(int groupPosition) {
    Log.d("adapter", "getGroupId");
    return 0;
  }

  @Override
  public View getGroupView(int position, boolean arg1, View convertView, ViewGroup arg3) {
    Log.d("adapter", "GetGroupView position: " + position);
    if (convertView != null) {
      System.out.println("Data already set");
      return convertView;
    }
    LinearLayout layout = new LinearLayout(frame);
    AdapterData data;

      Log.d("adapter", "dataKList.size: " + dataList.size());
      Log.d("adapter", "trying to get from index: " + (position));
      data = dataList.get(position);
      
      for (int i = 0; i < data.getSize(); i++) {
	Log.d("adapter", "looping throud data " + i);
	TextView txtFirst = new TextView(frame);
	txtFirst.setFocusable(false);
	txtFirst.setFocusableInTouchMode(false);
	txtFirst.setClickable(false);
	// txtFirst.setTextAppearance(frame,
	// android.R.style.TextAppearance_DeviceDefault_Small);
	txtFirst.setTextSize(9);
	txtFirst.setLayoutParams(listItemParams);
	layout.addView(txtFirst);
	txtFirst.setText(data.getData(i));
      }

      layout.setPadding(0, 20, 0, 20);
      return layout;
    
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
    private AdapterDataType type = AdapterDataType.DATA;

    public AdapterData(ArrayList<String> stringList) {
      this.stringList = stringList;
    }

    public AdapterData(ArrayList<String> stringList, AdapterDataType type) {
      this.type = type;
      this.stringList = stringList;
    }

    public void addString(String text) {
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

    public ArrayList<String> getList() {
      return stringList;
    }
  }
}
