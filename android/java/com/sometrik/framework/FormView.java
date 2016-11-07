package com.sometrik.framework;

import java.util.ArrayList;

import android.content.Context;
import android.content.DialogInterface;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

public class FormView implements NativeMessageHandler {

  private LinearLayout baseLayout;
  private Context context;
  private ArrayList<ViewGroup> elementList;
  private ArrayList<TextView> TextElementList;
  private int viewId;
  private Handler mainHandler;
  
  public FormView(int id, Context context) {
    this.context = context;
    baseLayout = new LinearLayout(context);
    baseLayout.setOrientation(0);
    baseLayout.setId(0);
    viewId = id;
    FrameWork frame = (FrameWork)context;
    mainHandler = frame.mainHandler;
    
    EditText usernameEdit = new EditText(context);
    usernameEdit.setText("Username");
    baseLayout.addView(usernameEdit);
    EditText passwordEdit = new EditText(context);
    passwordEdit.setText("Password");
    baseLayout.addView(passwordEdit);
    
    Button submitButton = new Button(context);
    submitButton.setText("Submit");
    baseLayout.addView(submitButton);
    
  }
  
  //1 = vertical, 0 = horizontal
  public void changeBaseLayoutOrientation(int orientation){
    baseLayout.setOrientation(orientation);
  }
  
  public void createButton(final int id){
    Button button = new Button(context);
    button.setId(id);
    TextElementList.add(button);
    
    button.setOnClickListener(new Button.OnClickListener() {
      @Override
      public void onClick(View arg0) {
	Message msg = Message.obtain(mainHandler, 9, id);
	msg.sendToTarget();
      }
    });
  }
  
  public void createLayout(int id, int targetId, int orientation){
    LinearLayout linearLayout = new LinearLayout(context);
    linearLayout.setOrientation(orientation);
    elementList.add(linearLayout);
    getElementById(targetId).addView(linearLayout);
  }
  
  public void createEditText(int id, int targetId){
    EditText editText = new EditText(context);
    editText.setId(id);
    TextElementList.add(editText);
    getElementById(targetId).addView(editText);
  }
  
  public void createTextView(int id, int targetId){
    TextView textView = new TextView(context);
    textView.setId(id);
    TextElementList.add(textView);
    getElementById(targetId).addView(textView);
  }
  
  public void setTextElementText(int id, String text){
    for (TextView textView : TextElementList){
      if (textView.getId() == id){
	textView.setText(text);
      }
    }
  }
  
  public void showView(){
    FrameWork frame = (FrameWork)context;
    frame.setContentView(baseLayout);
  }
  
  private ViewGroup getElementById(int id){
    for (ViewGroup element : elementList){
      if (element.getId() == id){
	return element;
      }
    }
    return baseLayout;
  }
  
  public int getViewId(){
    return viewId;
  }

  @Override
  public void handleMessage(NativeMessage message) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public int getId() {
    return viewId;
  }
}
