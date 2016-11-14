package com.sometrik.framework;

public class EditTextEvent {
  
  private int id;
  private String text;
  
  public EditTextEvent(int id, String text){
    this.id = id;
    this.text = text;
  }
  
  public int getId() {
    return id;
  }
  public String getText() {
    return text;
  }
}
