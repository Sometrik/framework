package com.sometrik.framework;

import android.content.Context;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

public class FWLayout extends LinearLayout implements NativeMessageHandler{
  
  private Context context;
  private FrameWork frame;
  
  public FWLayout(Context context) {
    super(context);
    this.context = context;
    this.frame = (FrameWork)context;
    list();
  }

  @Override
  public int getElementId() {
    return getId();
  }
  
  @Override
  public void showView(){
    frame.setContentView(this);
    FrameWork.currentView = getId();
  }
  
  private void list(){
    FrameWork.addToViewList(this);
  }
  
  private void createButton(final int id, String text){
    Button button = new Button(context);
    button.setId(id);
    button.setText(text);
   
    button.setOnClickListener(new OnClickListener(){
      @Override
      public void onClick(View arg0) {
	frame.buttonClicked(id);
      }
    });
    this.addView(button);
  }
  
  private void createEditText(final int id, String text){
    System.out.println("FWLayout " + this.getId() + " creating textfield");
    final EditText editText = new EditText(context);
    editText.setId(id);
    editText.setText(text);
    editText.setMinimumWidth(400000 / (int)frame.getScreenWidth());
    editText.addTextChangedListener(new TextWatcher() {

      public void afterTextChanged(Editable editable) {

	  frame.textChangedEvent(id, editable.toString());
      }

      public void beforeTextChanged(CharSequence s, int start, int count, int after) {}

      public void onTextChanged(CharSequence s, int start, int before, int count) {}
   });
    this.addView(editText);
  }

  @Override
  public void handleMessage(NativeMessage message) {
    System.out.println("Message FWLayout " + this.getId());
    
    switch(message.getMessage()){
    case CREATE_BUTTON:
      createButton(message.getChildInternalId(), message.getTextValue());
      break;

    case CREATE_PICKER:
      FWPicker picker = new FWPicker(context);
      picker.setId(message.getChildInternalId());
      FrameWork.addToViewList(picker);
      this.addView(picker);
      break;

    case CREATE_LINEAR_LAYOUT:
      System.out.println("FWLayout " + this.getId() + " creating layout");
      FWLayout layout = new FWLayout(context);
      layout.setId(message.getChildInternalId());
      if (message.getValue() == 2){
	layout.setOrientation(LinearLayout.HORIZONTAL);
      } else if (message.getValue() == 1){
	layout.setOrientation(LinearLayout.VERTICAL);
      }
      this.addView(layout);
      break;

    case CREATE_OPENGL_VIEW:
      break;

    case CREATE_TEXTFIELD:
      createEditText(message.getChildInternalId(), message.getTextValue());
      break;

    case CREATE_TEXTLABEL:
      System.out.println("FWLayout " + this.getId() + " creating textlabel");
      TextView textView = new TextView(context);
      textView.setId(message.getChildInternalId());
      textView.setText(message.getTextValue());
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
