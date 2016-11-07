package com.sometrik.framework;

import android.content.Context;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.Spinner;
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
      Spinner spinner = new Spinner(context);
      spinner.setId(message.getChildInternalId());
      this.addView(spinner);
      break;

    case CREATE_LINEAR_LAYOUT:
      LinearLayout layout = new LinearLayout(context);
      layout.setId(message.getChildInternalId());
      this.addView(layout);
      break;

    case CREATE_OPENGL_VIEW:
      break;

    case CREATE_TEXTFIELD:
      EditText editText = new EditText(context);
      editText.setId(message.getChildInternalId());
      this.addView(editText);
      break;

    case CREATE_TEXTLABEL:
      TextView textView = new TextView(context);
      textView.setId(message.getChildInternalId());
      this.addView(textView);
      break;

    case SET_ATTRIBUTE:
      break;
      
    default:
      System.out.println("Message couldn't be handled by Element");
      break;
    }
  }

}
