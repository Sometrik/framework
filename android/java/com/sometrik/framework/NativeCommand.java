package com.sometrik.framework;

import java.nio.charset.Charset;
import java.util.ArrayList;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.content.SharedPreferences;
import android.text.Editable;
import android.text.InputType;
import android.text.TextWatcher;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.PopupMenu;
import android.widget.PopupMenu.OnMenuItemClickListener;
import android.widget.ScrollView;
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
  private ArrayList<PopupMenu> menuList = new ArrayList<PopupMenu>();
  
  public enum CommandType {
    CREATE_PLATFORM,
    CREATE_APPLICATION,
    CREATE_FORMVIEW,
    CREATE_NATIVE_OPENGL_VIEW,
    CREATE_GRIDVIEW,
    CREATE_BUTTON,
    CREATE_SWITCH,
    CREATE_PICKER, // called Spinner in Android
    CREATE_LINEAR_LAYOUT,
    CREATE_TABLE_LAYOUT,
    CREATE_TEXTFIELD,
    CREATE_TEXTLABEL,
    CREATE_DIALOG, // For future
    CREATE_IMAGE_ELEMENT,
    CREATE_ACTION_SHEET,
    CREATE_CHECKBOX,
    CREATE_RADIO_GROUP,
    CREATE_SEPARATOR,
    CREATE_SLIDER,
    SET_USER_ID,
    DELETE_ELEMENT,
    SHOW_VIEW,
    REQUEST_REDRAW,
    SHOW_MESSAGE_DIALOG,
    SHOW_INPUT_DIALOG,
    SHOW_ACTION_SHEET,
    LAUNCH_BROWSER,
    POST_NOTIFICATION,
    SET_VALUE, //Sets value of textfields.
    SET_LABEL, //this sets label for buttons and labels.
    SET_ATTRIBUTE,
    SET_CAPTION,
    UPDATE_PREFERENCE,
    ADD_OPTION,
    QUIT_APP,

    // Timers
    CREATE_TIMER,
    
    // In-app purchases
    LIST_PRODUCTS,
    BUY_PRODUCT,
    LIST_PURCHASES,
    CONSUME_PURCHASE
  }
  
  public NativeCommand(FrameWork frame, CommandType command, int internalId, int childInternalId, byte[] textValue, byte[] textValue2){
    this.frame = frame;
    this.command = command;
    this.internalId = internalId;
    this.childInternalId = childInternalId;

    if (textValue != null) {
	this.textValue = new String(textValue, Charset.forName("UTF-8"));
    }
    if (textValue2 != null) {
	this.textValue2 = new String(textValue2, Charset.forName("UTF-8"));
    }
  }
  public NativeCommand(FrameWork frame, int messageTypeId, int internalId, int childInternalId, int value, byte[] textValue, byte[] textValue2){
    this.frame = frame;
    command = CommandType.values()[messageTypeId];
    this.internalId = internalId;
    this.childInternalId = childInternalId;
    this.value = value;

    if (textValue != null) {
	this.textValue = new String(textValue, Charset.forName("UTF-8"));
    }
    if (textValue2 != null) {
	this.textValue2 = new String(textValue2, Charset.forName("UTF-8"));
    }
  }
  
  public void apply(NativeCommandHandler view) {

    System.out.println("Processing message " + command + " id: " + internalId + " Child id: " + getChildInternalId());

    switch (command) {

    case CREATE_FORMVIEW:
      System.out.println("creating formView " + getChildInternalId());
      createFormView();
      break;
      
    case CREATE_LINEAR_LAYOUT:
      FWLayout layout = createLinearLayout();
      view.addChild(layout);
      break;

    case CREATE_BUTTON:
      Button button = createButton();
      view.addChild(button);
      break;

    case CREATE_PICKER:
      FWPicker picker = createSpinner();
      view.addChild(picker);
      break;

    case CREATE_NATIVE_OPENGL_VIEW:
      NativeSurface surface = frame.createNativeOpenGLView(childInternalId);
      surface.showView();
      break;

    case CREATE_TEXTFIELD:
      EditText editText = createEditText();
      view.addChild(editText);
      break;

    case CREATE_TEXTLABEL:
      TextView textView = createTextView();
      view.addChild(textView);
      break;

    case SET_ATTRIBUTE:
      break;
    case CREATE_IMAGE_ELEMENT:
      ImageView imageView = new ImageView(frame);
      imageView.setId(getChildInternalId());
      // Missing image set
      // imageView.setImageBitmap();
      break;
      
    case SHOW_VIEW:
    	frame.disableDraw();
      view.showView();
      break;
    case ADD_OPTION:
      //Forward Command to FWPicker
      view.addOption(getValue(), getTextValue());
      break;
    case POST_NOTIFICATION:
      frame.createNotification(getTextValue(), getTextValue2());
      break;
    case CREATE_APPLICATION:
      frame.setAppId(getInternalId());
      break;
    case SET_CAPTION:
      frame.setTitle(getTextValue());
      break;
    case LAUNCH_BROWSER:
      frame.launchBrowser(getTextValue());
      break;
    case SHOW_MESSAGE_DIALOG:
      showMessageDialog(textValue, textValue2);
      break;
    case SHOW_INPUT_DIALOG:
      showInputDialog(textValue, textValue2);
      break;
    case CREATE_ACTION_SHEET:
      createActionSheet();
      break;
    case QUIT_APP:
      // TODO
      frame.finish();
      break;
    case UPDATE_PREFERENCE:
    	//Now stores String value to string key.
      SharedPreferences prefs = frame.getSharedPreferences("com.example.framework", Context.MODE_PRIVATE);
      SharedPreferences.Editor editor = prefs.edit();
      editor.putString(textValue, textValue2);
      editor.apply();
      break;
    default:
      System.out.println("Message couldn't be handled");
      break;
    }
  }
  
  
  private void createActionSheet(){
    PopupMenu menu = new PopupMenu(frame, null);
    menu.setOnMenuItemClickListener(new OnMenuItemClickListener(){
      @Override
      public boolean onMenuItemClick(MenuItem item) {
	
	return false;
      }
    });
    menuList.add(menu);
  }
  
  private void addOption(int menuId, int optionId, String optionText){
    PopupMenu menu = menuList.get(menuId);
    menu.getMenu().add(Menu.NONE, optionId, Menu.NONE, optionText);
  }
  
  private FWLayout createLinearLayout(){
    FWLayout layout = new FWLayout(frame);
    layout.setId(getChildInternalId());
    FrameWork.addToViewList(layout);
    if (getValue() == 2) {
	layout.setOrientation(LinearLayout.HORIZONTAL);
    } else if (getValue() == 1) {
	layout.setOrientation(LinearLayout.VERTICAL);
    }
    return layout;
  }

  private Button createButton() {
    Button button = new Button(frame);
    button.setId(getInternalId());
    button.setText(getTextValue());

    button.setOnClickListener(new OnClickListener() {
      @Override
      public void onClick(View arg0) {
	frame.buttonClicked(System.currentTimeMillis() / 1000.0, getChildInternalId());
      }
    });
    return button;
  }
  
  private EditText createEditText(){
    final EditText editText = new EditText(frame);
    editText.setId(getChildInternalId());
    editText.setText(getTextValue());
    editText.setMinimumWidth(400000 / (int)frame.getScreenWidth());
    editText.addTextChangedListener(new TextWatcher() {
      public void afterTextChanged(Editable editable) {

	  frame.textChangedEvent(System.currentTimeMillis() / 1000.0, getChildInternalId(), editable.toString());
      }
      public void beforeTextChanged(CharSequence s, int start, int count, int after) {}
      public void onTextChanged(CharSequence s, int start, int before, int count) {}
   });
    return editText;
  }
  
  private FWPicker createSpinner(){
    FWPicker picker = new FWPicker(frame);
    picker.setId(getChildInternalId());
    FrameWork.views.put(getChildInternalId(), picker);
    
    return picker;
  }

  private TextView createTextView() {
    TextView textView = new TextView(frame);
    textView.setId(getChildInternalId());
    textView.setText(getTextValue());
    return textView;
  }
  
  private void createFormView(){
    FWLayout layout = new FWLayout(frame);
    layout.setId(getChildInternalId());
    FrameWork.views.put(getChildInternalId(), layout);
    ScrollView scrollView = new ScrollView(frame);
    scrollView.addView(layout);
  }

 // Create dialog with user text input
  private void showInputDialog(String title, String message) {
    System.out.println("Creating input dialog");

    AlertDialog.Builder builder;
    builder = new AlertDialog.Builder(frame);

    // Building an alert
    builder.setTitle(title);
    builder.setMessage(message);
    builder.setCancelable(true);

    final EditText input = new EditText(frame);
    input.setInputType(InputType.TYPE_CLASS_TEXT);
    builder.setView(input);
    
    builder.setOnCancelListener(new OnCancelListener(){

      @Override
      public void onCancel(DialogInterface arg0) {
	frame.endModal(System.currentTimeMillis() / 1000.0, 0, null);
      }
    });

    // Negative button listener
    builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
      public void onClick(DialogInterface dialog, int id) {
	frame.endModal(System.currentTimeMillis() / 1000.0, 0, null);
	dialog.cancel();
      }
    });

    // Positive button listener
    builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
      public void onClick(DialogInterface dialog, int id) {
	String inputText = String.valueOf(input.getText());
	byte[] b = inputText.getBytes(Charset.forName("UTF-8"));
	frame.endModal(System.currentTimeMillis() / 1000.0, 1, b);
	dialog.cancel();
      }
    });

    // Create and show the alert
    AlertDialog alert = builder.create();
    alert.show();
  }

  // create Message dialog
  private void showMessageDialog(String title, String message) {

    System.out.println("creating message dialog");

    AlertDialog.Builder builder;
    builder = new AlertDialog.Builder(frame);

    // Building an alert
    builder.setTitle(title);
    builder.setMessage(message);
    builder.setCancelable(true);
    
    builder.setOnCancelListener(new OnCancelListener(){

      @Override
      public void onCancel(DialogInterface arg0) {
	frame.endModal(System.currentTimeMillis() / 1000.0, 0, null);
      }
      
    });

    // Positive button listener
    builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
      public void onClick(DialogInterface dialog, int id) {
	frame.endModal(System.currentTimeMillis() / 1000.0, 1, null);
	dialog.dismiss();
      }
    });

    // Create and show the alert
    AlertDialog alert = builder.create();
    alert.show();

    System.out.println("message dialog created");
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
