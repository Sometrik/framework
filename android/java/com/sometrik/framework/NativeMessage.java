package com.sometrik.framework;

public class NativeMessage {

  private int internalId = 0;
  private int childInternalId = 0;
  private String textValue = "";
  private String textValue2 = "";
  private MessageType message;
  private String key;
  
  public enum MessageType{
    CREATE_APPLICATION,
    SHOW_VIEW,
    CREATE_FORMVIEW,
    CREATE_BUTTON,
    CREATE_PICKER, // called Spinner in Android
    CREATE_LINEAR_LAYOUT,
    CREATE_OPENGL_VIEW,
    CREATE_NATIVE_OPENGL_VIEW,
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

    // Timers
    CREATE_TIMER,
    DELETE_TIMER,
    
    // In-app purchases
    LIST_PRODUCTS,
    BUY_PRODUCT,
    LIST_PURCHASES,
    CONSUME_PURCHASE
  }
  
  NativeMessage(MessageType message, int internalId, int childInternalId, String textValue, String textValue2){
    this.message = message;
    this.internalId = internalId;
    this.childInternalId = childInternalId;
    this.textValue = textValue;
    this.textValue2 = textValue2;
  }
  NativeMessage(int messageTypeId, int internalId, int childInternalId, String textValue, String textValue2){
    message = MessageType.values()[messageTypeId];
    this.internalId = internalId;
    this.childInternalId = childInternalId;
    this.textValue = textValue;
    this.textValue2 = textValue2;
  }
  
  public String getKey() {
    return key;
  }
  public void setKey(String key) {
    this.key = key;
  }
  public int getInternalId() {
    return internalId;
  }
  public int getChildInternalId() {
    return childInternalId;
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
