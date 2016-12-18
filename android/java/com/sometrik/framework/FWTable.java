package com.sometrik.framework;

import android.content.Context;
import android.view.View;
import android.widget.TableLayout;

public class FWTable extends TableLayout implements NativeCommandHandler {
  
  public FWTable(Context context) {
    super(context);
  }



@Override
public void addChild(View view) {
  System.out.println("FWTable couldn't handle addChild command");
}

@Override
public void showView() {
  System.out.println("FWTable couldn't handle showView command");
}

@Override
public void addOption(int position, String text) {
}

@Override
public int getElementId() {
  return getId();
}



}
