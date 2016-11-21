package com.sometrik.framework;

import java.util.ArrayList;

import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
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
    baseLayout.setOrientation(LinearLayout.VERTICAL);
    baseLayout.setId(0);
    viewId = id;
    FrameWork frame = (FrameWork)context;
    mainHandler = frame.mainHandler;
    
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
  
  @Override
  public void showView(){
    FrameWork frame = (FrameWork) context;
    frame.setContentView(baseLayout);
    FrameWork.currentView = viewId;
  }

  @Override
  public void handleCommand(NativeCommand command) {
    
    switch (command.getCommand()){
    case CREATE_LINEAR_LAYOUT:
      System.out.println("Formview creating layout");
      FWLayout layout = new FWLayout(context);
      layout.setId(command.getChildInternalId());
      FrameWork.addToViewList(command.getChildInternalId(), layout);
      if (command.getValue() == 2){
	layout.setOrientation(LinearLayout.HORIZONTAL);
      } else if (command.getValue() == 1){
	layout.setOrientation(LinearLayout.VERTICAL);
      }
      baseLayout.addView(layout);
      break;

    case CREATE_TEXTLABEL:
      TextView textView = new TextView(context);
      textView.setId(command.getChildInternalId());
      textView.setText(command.getTextValue());
      baseLayout.addView(textView);
      break;

    case CREATE_BUTTON:
      Button button = new Button(context);
      button.setId(command.getChildInternalId());
      button.setText(command.getTextValue());
      baseLayout.addView(button);
      break;
      
    case CREATE_PICKER:
      FWPicker picker = new FWPicker(context);
      picker.setId(command.getChildInternalId()); 
      FrameWork.addToViewList(command.getChildInternalId(), picker);
      baseLayout.addView(picker);
      break;

    case CREATE_IMAGE_ELEMENT:
      ImageView imageView = new ImageView(context);
      imageView.setId(command.getChildInternalId());
      //Missing image set
//      imageView.setImageBitmap();
      break;
      
    default:
      System.out.println("Unhandled case");
      break;

    }
  }

  @Override
  public int getElementId() {
    return viewId;
  }
}
