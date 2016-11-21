package com.sometrik.framework;

import java.util.ArrayList;
import java.util.TreeMap;

import android.content.Context;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

public class FWPicker extends Spinner implements NativeMessageHandler {
  
  private ArrayAdapter<String> adapter;
  private ArrayList<Integer> numberList;
  private TreeMap<Integer, String> valueMap;
  
  private native void pickerOptionSelected(int id);

  public FWPicker(Context context) {
    super(context);
    adapter = new ArrayAdapter<String>(context, android.R.layout.simple_spinner_item);
    valueMap = new TreeMap<Integer, String>();
    numberList = new ArrayList<Integer>();
    
    setOnItemSelectedListener(new OnItemSelectedListener() {

      @Override
      public void onItemSelected(AdapterView<?> arg0, View arg1, int position, long arg3) {
	pickerOptionSelected(position);
      }

      @Override
      public void onNothingSelected(AdapterView<?> arg0) {
      }
    });
  }

  @Override
  public void handleMessage(NativeMessage message) {
    
    switch (message.getMessage()) {
    case ADD_OPTION:
      valueMap.put(message.getValue(), message.getTextValue());
      numberList.add(message.getValue());
      for (int i = 0; i < numberList.size(); i++) {
	adapter.add(valueMap.get(numberList.get(i)));
      }
      setAdapter(adapter);
      break;
    default:
      System.out.println("Message couldn't be handled by Picker");
      break;
    }
  }

  @Override
  public void showView() {
    
  }

  @Override
  public int getElementId() {
    return getId();
  }

}
