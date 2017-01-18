package com.sometrik.framework;

import java.util.ArrayList;
import java.util.Map.Entry;
import java.util.TreeMap;

import android.content.Context;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

public class FWPicker extends Spinner implements NativeCommandHandler {
  
  private Context context;
  private FrameWork frame;
  private ArrayAdapter<String> adapter;
  private ArrayList<Integer> idList;
  private final int id;
  

  public FWPicker(Context context) {
    super(context);
    adapter = new ArrayAdapter<String>(context, android.R.layout.simple_spinner_item);
    idList = new ArrayList<Integer>();
    id = getId();
    this.context = context;
    this.frame = (FrameWork)context;
    
    setOnItemSelectedListener(new OnItemSelectedListener() {

      @Override
      public void onItemSelected(AdapterView<?> view, View arg1, int position, long itemId) {
	frame.intChangedEvent(System.currentTimeMillis() / 1000.0, getId(), idList.get(position));
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
  public void addOption(int optionId, String text) {
    idList.add(optionId);
    adapter.add(text);
    setAdapter(adapter);
  }

  @Override
  public void removeChild(int id) {
    System.out.println("Picker couldn't handle remove element");
    // TODO Auto-generated method stub
  }

  @Override
  public void setValue(String v) {

  }

  @Override
  public void setValue(int v) {

  }

  @Override
  public void setEnabled(Boolean enabled) {
    setEnabled(enabled);
  }
}
