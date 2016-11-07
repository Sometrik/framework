package com.sometrik.framework;

import android.content.Context;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

public class FWLayout extends LinearLayout implements NativeMessageHandler{
  
  private Context context;
  
  public FWLayout(Context context) {
    super(context);
  }

  @Override
  public void handleMessage(NativeMessage message) {
    
    switch(message.getMessage()){
    case CREATE_BUTTON:
      Button button = new Button(context);
      button.setId(message.getChildInternalId());
      this.addView(button);
      break;

    case CREATE_PICKER:
      break;

    case CREATE_LINEAR_LAYOUT:
      LinearLayout layout = new LinearLayout(context);
      layout.setId(message.getChildInternalId());
      this.addView(layout);
      break;

    case CREATE_OPENGL_VIEW:
      break;

    case CREATE_TEXTFIELD:
      TextView textView = new TextView(context);
      textView.setId(message.getChildInternalId());
      this.addView(textView);
      break;

    case CREATE_TEXTLABEL:
      break;

    case SET_ATTRIBUTE:
      break;
      
    default:
      break;
    }
  }

}
