package com.sometrik.framework;

import android.content.Context;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.Spinner;
import android.widget.TextView;

public class FWButton extends Button implements NativeMessageHandler {

  private Context context;
  
  public FWButton(Context context) {
    super(context);
    this.context = context;
    list();
  }
  
  private void list(){
    FrameWork frameWork = (FrameWork)context;
    frameWork.addToViewList(this);
  }

  @Override
  public void handleMessage(NativeMessage message) {

    switch(message.getMessage()){

    case SET_ATTRIBUTE:
      break;
      
    default:
      System.out.println("Message couldn't be handled by Element");
      break;
    }
    
  }

}
