package com.sometrik.framework;

import java.util.ArrayList;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

public class FormView {

  private LinearLayout baseLayout;
  private Context context;
  private ArrayList<ViewGroup> elementList;
  private ArrayList<TextView> TextElementList;
  
  public FormView(Context context) {
    this.context = context;
    baseLayout = new LinearLayout(context);
    baseLayout.setOrientation(0);
    baseLayout.setId(0);
  }
  
  //1 = vertical, 0 = horizontal
  public void changeBaseLayoutOrientation(int orientation){
    baseLayout.setOrientation(orientation);
  }
  
  
  public void createButton(int id){
    Button button = new Button(context);
    button.setId(id);
    TextElementList.add(button);
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
}
