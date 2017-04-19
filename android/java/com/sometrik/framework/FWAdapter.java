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
  private ArrayList<Integer> sectionHeaderRows;
  private FrameWork frame;
  private int addedHeaders = 0;
  private LinearLayout.LayoutParams listItemParams;
  
  public FWAdapter(Context context, List<View> viewList) {
    super(context, 0, viewList);
    this.frame = (FrameWork) context;
    dataList = new ArrayList<AdapterData>();
    columnData = new AdapterData(new ArrayList<String>());
    listItemParams = new LinearLayout.LayoutParams(0, LinearLayout.LayoutParams.WRAP_CONTENT);
    listItemParams.weight = 1;
    listItemParams.leftMargin = 2;
    sectionHeaderRows = new ArrayList<Integer>();
  }

  public ArrayList<String> getDataRow(int rowId) {
    if (dataList.size() >= rowId) {
      AdapterData data = dataList.get(rowId - 1);
      if (data.dataType.equals("section")){
	return null;
      }
      return data.stringList;
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
    sectionHeaderRows.add(row);
    dataList.add(new AdapterData(sectionHeader));
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
	  if (position - 1 == sectionRow - 1) {
	    Log.d("adapter", "sectionRow found");
//	    String sectionText = sectionHeaders.get(sectionRow);
		data = dataList.get(position - 1);
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
	data = dataList.get(position - 1);
//	if (dataList.size() == position){
//	    addedHeaders = 0;
//	}
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
}


