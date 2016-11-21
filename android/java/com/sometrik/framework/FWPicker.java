package com.sometrik.framework;

import java.util.ArrayList;
import java.util.TreeMap;

import android.content.Context;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

public class FWPicker extends Spinner implements NativeMessageHandler {
  
  ArrayAdapter<String> adapter;
  ArrayList<Integer> numberList;
  TreeMap<Integer, String> valueMap;

  public FWPicker(Context context) {
    super(context);
    adapter = new ArrayAdapter<String>(context, android.R.layout.simple_spinner_item);
    valueMap = new TreeMap<Integer, String>();
    numberList = new ArrayList<Integer>();
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
