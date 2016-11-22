package com.sometrik.framework;

import android.content.Context;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.Spinner;
import android.widget.TextView;

public class NativeCommand {

  private int internalId = 0;
  private int childInternalId = 0;
  private int value = 0;
  private String textValue = "";
  private String textValue2 = "";
  private CommandType command;
  private String key;
  private FrameWork frame;
  
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
  
  public NativeCommand(CommandType command, int internalId, int childInternalId, String textValue, String textValue2){
    this.command = command;
    this.internalId = internalId;
    this.childInternalId = childInternalId;
    this.textValue = textValue;
    this.textValue2 = textValue2;
  }
  public NativeCommand(int messageTypeId, int internalId, int childInternalId, int value, String textValue, String textValue2){
    command = CommandType.values()[messageTypeId];
    this.internalId = internalId;
    this.childInternalId = childInternalId;
    this.value = value;
    this.textValue = textValue;
    this.textValue2 = textValue2;
  }
  
  public void apply(NativeMessageHandler view, Context context) {
    frame = (FrameWork)context;

    switch (command) {

    case CREATE_FORMVIEW:
      System.out.println("creating formView " + getChildInternalId());
      createFormView(context);
      break;
      
    case CREATE_LINEAR_LAYOUT:
      System.out.println("FWLayout " + getInternalId() + " creating layout " + getChildInternalId());
      FWLayout layout = new FWLayout(context);
      layout.setId(getChildInternalId());
      FrameWork.addToViewList(layout);
      if (getValue() == 2) {
	layout.setOrientation(LinearLayout.HORIZONTAL);
      } else if (getValue() == 1) {
	layout.setOrientation(LinearLayout.VERTICAL);
      }
      view.addChild(layout);
      break;

    case CREATE_BUTTON:
      Button button = createButton(context);
      view.addChild(button);
      break;

    case CREATE_PICKER:
      FWPicker picker = createSpinner(context);
      view.addChild(picker);
      break;

    case CREATE_OPENGL_VIEW:
      break;

    case CREATE_TEXTFIELD:
      EditText editText = createEditText(context);
      view.addChild(editText);
      break;

    case CREATE_TEXTLABEL:
      TextView textView = createTextView(context);
      view.addChild(textView);
      break;

    case SET_ATTRIBUTE:
      break;
    case CREATE_IMAGE_ELEMENT:
      ImageView imageView = new ImageView(context);
      imageView.setId(getChildInternalId());
      // Missing image set
      // imageView.setImageBitmap();
      break;
      
    case SHOW_VIEW:
      view.showView();
      break;
    case ADD_OPTION:
      //Forward Command to FWPicker
      view.handleCommand(this);
    case POST_NOTIFICATION:
      // TODO
      break;
    case CREATE_APPLICATION:
      frame.setAppId(getInternalId());
      break;
    case SET_CAPTION:
      frame.setTitle(getTextValue());
      break;
    case LAUNCH_BROWSER:
      // TODO
      break;
    default:
      System.out.println("Message couldn't be handled");
      break;
    }
  }

  private Button createButton(Context context) {
    Button button = new Button(context);
    button.setId(getInternalId());
    button.setText(getTextValue());
    final FrameWork frame = (FrameWork) context;

    button.setOnClickListener(new OnClickListener() {
      @Override
      public void onClick(View arg0) {
	frame.buttonClicked(getInternalId());
      }
    });
    return button;
  }
  
  private EditText createEditText(Context context){
    final EditText editText = new EditText(context);
    editText.setId(getChildInternalId());
    editText.setText(getTextValue());
    final FrameWork frame = (FrameWork) context;
    editText.setMinimumWidth(400000 / (int)frame.getScreenWidth());
    editText.addTextChangedListener(new TextWatcher() {
      public void afterTextChanged(Editable editable) {

	  frame.textChangedEvent(getChildInternalId(), editable.toString());
      }
      public void beforeTextChanged(CharSequence s, int start, int count, int after) {}
      public void onTextChanged(CharSequence s, int start, int before, int count) {}
   });
    return editText;
  }
  
  private FWPicker createSpinner(Context context){
    FWPicker picker = new FWPicker(context);
    picker.setId(getChildInternalId());
    FrameWork.views.put(getChildInternalId(), picker);
    
    return picker;
  }

  private TextView createTextView(Context context) {
    TextView textView = new TextView(context);
    textView.setId(getChildInternalId());
    textView.setText(getTextValue());
    return textView;
  }
  
  private void createFormView(Context context){
    FWLayout layout = new FWLayout(context);
    layout.setId(getChildInternalId());
    FrameWork.views.put(getChildInternalId(), layout);
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
