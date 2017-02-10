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
import android.widget.LinearLayout;
import android.widget.TextView;

public class FWAdapter extends ArrayAdapter<View> {

  private List<View> viewList;
  private ArrayList<AdapterData> dataList;
  private FrameWork frame;
  private LinearLayout.LayoutParams listItemParams;
  
  public FWAdapter(Context context, List<View> viewList) {
    super(context, 0, viewList);
    this.frame = (FrameWork) context;
    dataList = new ArrayList<AdapterData>();
    listItemParams = new LinearLayout.LayoutParams(0, LinearLayout.LayoutParams.WRAP_CONTENT);
    listItemParams.weight = 1;
    listItemParams.leftMargin = 2;
  }

  public ArrayList<String> getDataRow(int rowId) {
    if (dataList.size() >= rowId + 1) {
      return dataList.get(rowId).stringList;
    }
    Log.d("adapter", "no row found");
    return null;
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
  public void clear(){
    dataList = new ArrayList<AdapterData>();
  }

  @Override
  public View getView(int position, View convertView, ViewGroup parent) {

    LinearLayout layout = new LinearLayout(frame);

    AdapterData data = dataList.get(position);
    
    for (int i = 0; i < data.getSize(); i++){
      TextView txtFirst = new TextView(frame);
      txtFirst.setLayoutParams(listItemParams);
      layout.addView(txtFirst);
      txtFirst.setText(data.getData(i));
    }

    return layout;
  }
  
  public class AdapterData {

    private ArrayList<String> stringList;

    public AdapterData(ArrayList<String> stringList) {
      this.stringList = stringList;
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

  }
}


