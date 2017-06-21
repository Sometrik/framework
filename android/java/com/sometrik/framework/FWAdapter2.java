package com.sometrik.framework;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.LinearLayout;
import android.widget.TextView;

public class FWAdapter2 extends ArrayAdapter<View> {

  enum AdapterDataType {
    SHEET, DATA
  }

  private List<View> viewList;
  private HashMap<Integer, AdapterData> dataList;
//  private AdapterData columnData;
  private FrameWork frame;
  private LinearLayout.LayoutParams listItemParams;
  private boolean sheetsEnabled = false;

  public FWAdapter2(FrameWork frame, List<View> viewList) {
    super(frame, 0, viewList);
    this.frame = frame;
    dataList = new HashMap<Integer, AdapterData>();
//    columnData = new AdapterData(new ArrayList<String>());
    listItemParams = new LinearLayout.LayoutParams(0, LinearLayout.LayoutParams.MATCH_PARENT);
    listItemParams.weight = 1;
    listItemParams.leftMargin = 50;
  }

  public ArrayList<String> getDataRow(int row, int sheet) {
    // if (sheetData == null || sheetData.type == AdapterDataType.DATA) {
    AdapterData data = dataList.get(row);
    if (data == null) {
      Log.d("adapter", "no row found");
      return null;
    } else {
      Log.d("adapter", "row found");
      return data.stringList;
    }
  }

  public void addItem(int row, int sheet, ArrayList<String> cellItems) {
    Log.d("adapter", "adding new dataList to row: " + row + " sheet: " + sheet);
    
    AdapterData data = new AdapterData(cellItems);
    dataList.put(row, data);
    
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
    System.out.println("adapter getCount " + dataList.size());
    return dataList.size();
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
    System.out.println("adapter position: " + position);
    if (convertView == null) {
      System.out.println("Data already set");

      AdapterData data = dataList.get(position);
      if (data == null) {
        
        Log.d("adapter", "no data on position " + position);
        return convertView;
      }
      LinearLayout layout = new LinearLayout(frame);
      layout.setOrientation(LinearLayout.HORIZONTAL);
      layout.setPadding(0, 20, 0, 20);
      ViewHolder holder = new ViewHolder(data, layout);
      convertView = holder.rowLayout;
      convertView.setTag(holder);
      
    } else {
      AdapterData data = dataList.get(position);
      if (data == null) {
        Log.d("adapter", "no data on position " + position);
        return convertView;
      }
      ViewHolder holder = (ViewHolder) convertView.getTag();
      holder.update(data);
    }
    return convertView;

  }

  @Override
  public boolean isEnabled(int position) {
    return true;
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
  
  public class ViewHolder {
    
    AdapterData data;
    LinearLayout rowLayout;
    ArrayList<TextView> viewList;
    int row;
    
    public ViewHolder(AdapterData data, LinearLayout rowLayout){
      this.data = data;
      viewList = new ArrayList<TextView>();
      this.rowLayout = rowLayout;
      
      for (int i = 0; i < data.getSize(); i++){
	TextView textView = new TextView(frame);
	textView.setFocusable(false);
	textView.setFocusableInTouchMode(false);
	textView.setClickable(false);
	// txtFirst.setTextAppearance(frame,
	// android.R.style.TextAppearance_DeviceDefault_Small);
	textView.setTextSize(9);
	textView.setLayoutParams(listItemParams);
	textView.setText(data.getData(i));
	viewList.add(textView);
	rowLayout.addView(textView);
      }
    }
    
    public void update(AdapterData data){


      for (int i = viewList.size(); i < data.getSize(); i++){
	TextView textView = new TextView(frame);
	textView.setFocusable(false);
	textView.setFocusableInTouchMode(false);
	textView.setClickable(false);
	// txtFirst.setTextAppearance(frame,
	// android.R.style.TextAppearance_DeviceDefault_Small);
	textView.setTextSize(9);
	textView.setLayoutParams(listItemParams);
	textView.setText(data.getData(i));
	textView.setSingleLine();
	viewList.add(textView);
	rowLayout.addView(textView);
      }
      
      for (int i = 0; i < viewList.size(); i++){
	TextView view = viewList.get(i);
	view.setText(data.getData(i));
      }
    }
    
  }
}
