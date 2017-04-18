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
import android.widget.LinearLayout;
import android.widget.TextView;

public class FWAdapter extends ArrayAdapter<View> {

  private List<View> viewList;
  private ArrayList<AdapterData> dataList;
  private AdapterData columnData;
  private HashMap<Integer, String> sectionHeaders;
  private ArrayList<Integer> sectionHeaderRows;
  private FrameWork frame;
  private LinearLayout.LayoutParams listItemParams;
  
  public FWAdapter(Context context, List<View> viewList) {
    super(context, 0, viewList);
    this.frame = (FrameWork) context;
    dataList = new ArrayList<AdapterData>();
    columnData = new AdapterData(new ArrayList<String>());
    listItemParams = new LinearLayout.LayoutParams(0, LinearLayout.LayoutParams.WRAP_CONTENT);
    listItemParams.weight = 1;
    listItemParams.leftMargin = 2;
    sectionHeaders = new HashMap<Integer, String>();
    sectionHeaderRows = new ArrayList<Integer>();
  }

  public ArrayList<String> getDataRow(int rowId) {
    if (dataList.size() >= rowId) {
//      AdapterData data = dataList.get(rowId - 1);
//      if (data.dataType.equals("section")){
//	getDataRow(rowId + 1);
//      }
      return dataList.get(rowId - 1).stringList;
    }
    Log.d("adapter", "no row found");
    return null;
  }
  
  public void addItem(ArrayList<String> cellItems){
    dataList.add(new AdapterData(cellItems));
  }
  
  public void addColumn(String columnText){
    columnData.addString(columnText);
  }
  
  public void addSectionHeader(int row, String sectionHeader){
    sectionHeaders.put(row, sectionHeader);
    sectionHeaderRows.add(row);
//    ArrayList<String> emptyList = new ArrayList<String>();
//    emptyList.add("nothing");
//    dataList.add(new AdapterData("sectionHeader"));
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
    dataList = new ArrayList<AdapterData>();
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
	Log.d("adapter", "trying to get from index: " + (position - 1));
	for (Integer sectionRow : sectionHeaderRows) {
	  if (position - 1 == sectionRow) {
	    Log.d("adapter", "sectionRow found");
	    String sectionText = sectionHeaders.get(sectionRow);
	    TextView section = new TextView(frame);
	    section.setLayoutParams(listItemParams);
	    section.setTypeface(null, Typeface.BOLD);
	    layout.addView(section);
	    section.setText(sectionText);
	    return layout;
	  }
	}
	data = dataList.get(position - 1);
      } else {
	data = dataList.get(position);
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
}


