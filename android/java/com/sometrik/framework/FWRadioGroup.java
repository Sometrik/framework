package com.sometrik.framework;

import android.content.Context;
import android.view.View;
import android.widget.RadioButton;
import android.widget.RadioGroup;

public class FWRadioGroup extends RadioGroup implements NativeCommandHandler {
  
  private Context context;
  
  public FWRadioGroup(Context context){
    super(context);
    this.context = context;
  }

  @Override
  public void addChild(View view) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void removeChild(int id) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void showView() {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addOption(int optionId, String text) {
    RadioButton button = new RadioButton(context);
    button.setId(optionId);
    button.setText(text);
    addView(button);
  }

  @Override
  public void setValue(String v) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setValue(int v) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public int getElementId() {
    return getId();
  }

}
