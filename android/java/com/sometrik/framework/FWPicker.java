package com.sometrik.framework;

import java.util.Map.Entry;
import java.util.TreeMap;

import android.content.Context;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

public class FWPicker extends Spinner implements NativeCommandHandler {
  
  private Context context;
  private ArrayAdapter<String> adapter;
  private TreeMap<Integer, String> valueMap;
  private final int id;
  
  private native void pickerOptionSelected(int id, int position);

  public FWPicker(Context context) {
    super(context);
    adapter = new ArrayAdapter<String>(context, android.R.layout.simple_spinner_item);
    valueMap = new TreeMap<Integer, String>();
    id = getId();
    this.context = context;
    
    setOnItemSelectedListener(new OnItemSelectedListener() {

      @Override
      public void onItemSelected(AdapterView<?> arg0, View arg1, int position, long arg3) {
	pickerOptionSelected(id, position);
      }

      @Override
      public void onNothingSelected(AdapterView<?> arg0) {
      }
    });
  }

  public void handleCommand(NativeCommand command) {
  }

  @Override
  public void showView() {
    System.out.println("Picker couldn't handle showView");
  }

  @Override
  public int getElementId() {
    return getId();
  }

  @Override
  public void addChild(View view) {
    System.out.println("Picker couldn't handle addChild");
    //TODO
  }

  @Override
  public void addOption(int position, String text) {
    adapter = new ArrayAdapter<String>(context, android.R.layout.simple_spinner_item);
    valueMap.put(position, text);
    for(Entry<Integer, String> entry : valueMap.entrySet()) {
	adapter.add(entry.getValue());
    }
    setAdapter(adapter);
  }
}
