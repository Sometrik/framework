package com.sometrik.framework;

import java.util.ArrayList;
import java.util.List;

import com.sometrik.vapu.R;

import android.content.Context;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

public class FWAdapter extends ArrayAdapter<View> {

  private List<View> viewList;
  private ArrayList<AdapterData> dataList;
  private FrameWork frame;
  
  public FWAdapter(Context context, List<View> viewList) {
    super(context, 0, viewList);
    this.frame = (FrameWork)context;
//    this.viewList = viewList;
    this.dataList = new ArrayList<AdapterData>();
  }

  public ArrayList<String> getDataRow(int rowId) {
    if (dataList.size() >= rowId + 1) {
      return dataList.get(rowId).dataList;
    }
    Log.d("adapter", "no row found");
    return null;
  }
  
  public void addItem(View view){
//    viewList.add(view);
  }
  public void addItem(ArrayList<String> cellItems){
    dataList.add(new AdapterData(cellItems));
  }

  @Override
  public int getCount() {
    return dataList.size();
  }

  @Override
  public long getItemId(int arg0) {
    return 0;
  }

  @Override
  public View getView(int position, View convertView, ViewGroup parent) {
    
	
	
	LayoutInflater inflater = frame.getLayoutInflater();
	convertView=inflater.inflate(R.layout.multicolumn_data, null);
	
	
	TextView txtFirst=(TextView) convertView.findViewById(R.id.name);
	TextView txtSecond=(TextView) convertView.findViewById(R.id.gender);
	TextView txtThird=(TextView) convertView.findViewById(R.id.age);
	TextView txtFourth=(TextView) convertView.findViewById(R.id.status);
	
	AdapterData data = dataList.get(position);
	txtFirst.setText(data.getData(0));
	txtSecond.setText(data.getData(1));
	txtThird.setText(data.getData(2));
	txtFourth.setText(data.getData(3));
	
//    DisplayMetrics displaymetrics = new DisplayMetrics();
////    int dp = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 130, displaymetrics);
//    AbsListView.LayoutParams params = new AbsListView.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, 130);
//    View view = viewList.get(position);
//    view.setLayoutParams(params);
    return convertView;
  }
  
  public class AdapterData {

    private ArrayList<String> dataList;

    public AdapterData(ArrayList<String> dataList) {
      this.dataList = dataList;
    }

    public String getData(int position) {

      if (position < dataList.size()) {
	return dataList.get(position);
      } else {
	return dataList.get(0);
      }
    }

  }
}


