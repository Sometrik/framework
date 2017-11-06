package com.sometrik.framework;

import java.util.ArrayList;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

public class FWPicker extends Spinner implements NativeCommandHandler { 
  private FrameWork frame;
  private ArrayAdapter<String> adapter;
  private ArrayList<Integer> idList;

  public FWPicker(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;
    adapter = new ArrayAdapter<String>(frame, android.R.layout.simple_spinner_item);
    idList = new ArrayList<Integer>();
    
    setOnItemSelectedListener(new OnItemSelectedListener() {

      @Override
      public void onItemSelected(AdapterView<?> view, View arg1, int position, long itemId) {
	frame.intChangedEvent(getId(), idList.get(position), 0);
      }

      @Override
      public void onNothingSelected(AdapterView<?> arg0) {
      }
    });
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
  public void setValue(String v) {

  }

  @Override
  public void setValue(int v) {

  }

  @Override
  public void setStyle(Selector selector, String key, String value) { }

  @Override
  public void setError(boolean hasError, String errorText) { }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    System.out.println("Picker couldn't handle command");
  }
  
  @Override
  public void setViewVisibility(boolean visibility) {
    if (visibility){
      this.setVisibility(VISIBLE);
    } else {
      this.setVisibility(INVISIBLE);
    }
  }

  @Override
  public void clear() {
    System.out.println("couldn't handle command");
  }

  @Override
  public void flush() {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addColumn(String text, int columnType) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void reshape(int value, int size) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setBitmap(Bitmap bitmap) { }

  @Override
  public void reshape(int size) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void deinitialize() {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addImageUrl(String url, int width, int height) {
    // TODO Auto-generated method stub
    
  }
}
