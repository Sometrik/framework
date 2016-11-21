package com.sometrik.framework;

public class NativeCommand {

  private int internalId = 0;
  private int childInternalId = 0;
  private int value = 0;
  private String textValue = "";
  private String textValue2 = "";
  private CommandType command;
  private String key;
  
  public enum CommandType{
    CREATE_APPLICATION,
    SHOW_VIEW,
    SHOW,
    CREATE_FORMVIEW,
    CREATE_BUTTON,
    CREATE_PICKER, // called Spinner in Android
    CREATE_LINEAR_LAYOUT,
    CREATE_OPENGL_VIEW,
    CREATE_NATIVE_OPENGL_VIEW,
    CREATE_TEXTFIELD,
    CREATE_TEXTLABEL,
    CREATE_DIALOG, // For future
    CREATE_IMAGE_ELEMENT,
    CREATE_ACTION_SHEET,
    REQUEST_REDRAW,
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
  
  NativeCommand(CommandType command, int internalId, int childInternalId, String textValue, String textValue2){
    this.command = command;
    this.internalId = internalId;
    this.childInternalId = childInternalId;
    this.textValue = textValue;
    this.textValue2 = textValue2;
  }
  NativeCommand(int messageTypeId, int internalId, int childInternalId, int value, String textValue, String textValue2){
    command = CommandType.values()[messageTypeId];
    this.internalId = internalId;
    this.childInternalId = childInternalId;
    this.value = value;
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
  public CommandType getCommand() {
    return command;
  }
  public int getValue() {
    return value;
  }
}
