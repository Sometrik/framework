package com.sometrik.framework;

import android.content.Context;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

public class FWPicker extends Spinner implements NativeMessageHandler {
  
  ArrayAdapter<String> adapter;

  public FWPicker(Context context) {
    super(context);
  adapter = new ArrayAdapter<String>(context, android.R.layout.simple_spinner_item);
  }

  @Override
  public void handleMessage(NativeMessage message) {
    

    switch(message.getMessage()){
    case ADD_OPTION:
      adapter.add(message.getTextValue());
      setAdapter(adapter);
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
