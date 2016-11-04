package com.sometrik.framework;

public class Message {
  
  private int elementId = 0;
  private int parentElementId = 0;
  private String textValue = "";
  private String textValue2 = "";
  private MessageType message;
  private String key;
  
  public enum MessageType{
    SHOW_VIEW,
    CREATE_FORMVIEW,
    CREATE_BUTTON,
    CREATE_PICKER, // called Spinner in Android
    CREATE_LINEAR_LAYOUT,
    CREATE_OPENGL_VIEW,
    CREATE_TEXTFIELD,
    CREATE_TEXTLABEL,
    CREATE_DIALOG, //For future
    CREATE_CANVAS,
    SHOW_MESSAGE_DIALOG,
    SHOW_INPUT_DIALOG,
    LAUNCH_BROWSER,
    POST_NOTIFICATION,
    SET_VALUE, //Sets value of textfields.
    SET_LABEL, //this sets label for buttons and labels.
    SET_ATTRIBUTE,
    SET_CAPTION,
    ADD_OPTION,
    QUIT_APP,

    // In-app purchases
    LIST_PRODUCTS,
    BUY_PRODUCT,
    LIST_PURCHASES,
    CONSUME_PURCHASE
  }
  
  Message(MessageType message, String textValue){
    this.message = message;
    this.textValue = textValue;
  }
  Message(MessageType message, int elementId, int parentElementId){
    this.message = message;
    this.elementId = elementId;
    this.parentElementId = parentElementId;
  }
  Message(MessageType message, String textValue, String textValue2){
    this.message = message;
    this.textValue = textValue;
    this.textValue2 = textValue2;
  }
  
  public String getKey() {
    return key;
  }
  public void setKey(String key) {
    this.key = key;
  }
  public int getElementId() {
    return elementId;
  }
  public int getParentElementId() {
    return parentElementId;
  }
  public String getTextValue() {
    return textValue;
  }
  public String getTextValue2() {
    return textValue2;
  }
  public MessageType getMessage() {
    return message;
  }
}
