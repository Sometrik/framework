package com.sometrik.framework;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import com.android.trivialdrivesample.util.IabHelper;
import com.android.trivialdrivesample.util.IabHelper.IabAsyncInProgressException;
import com.android.trivialdrivesample.util.IabResult;
import com.android.trivialdrivesample.util.Inventory;
import com.android.trivialdrivesample.util.Purchase;

import android.app.ActionBar;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Typeface;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.text.Editable;
import android.text.Html;
import android.text.InputType;
import android.text.TextWatcher;
import android.text.method.LinkMovementMethod;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.Gravity;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.Window;
import android.view.inputmethod.EditorInfo;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.PopupMenu;
import android.widget.PopupMenu.OnMenuItemClickListener;
import android.widget.PopupWindow;

public class NativeCommand {

  private int internalId = 0;
  private int childInternalId = 0;
  private int value = 0;
  private int flags = 0;
  private String textValue = "";
  private String textValue2 = "";
  private CommandType command;
  private String key;
  private FrameWork frame;
  private ArrayList<PopupMenu> menuList = new ArrayList<PopupMenu>();
  private int rowNumber = -1;
  private int columnNumber = -1;
  private int sheet = 0;
  

  private final int FLAG_PADDING_LEFT = 1;
  private final int FLAG_PADDING_RIGHT = 2;
  private final int FLAG_PADDING_TOP = 4;
  private final int FLAG_PADDING_BOTTOM = 8;
  private final int FLAG_PASSWORD = 16;
  private final int FLAG_NUMERIC = 32;
  private final int FLAG_HYPERLINK = 64;
  private final int FLAG_USE_PURCHASES_API = 128;
  private final int FLAG_SLIDERVIEW = 256;
 
  public enum CommandType {
    CREATE_PLATFORM,
    CREATE_APPLICATION,
    CREATE_BASICVIEW,
    CREATE_FORMVIEW,
    CREATE_NAVIGATIONVIEW,
    CREATE_OPENGL_VIEW,
    CREATE_TEXTFIELD, // For viewing single value
    CREATE_TEXTVIEW, // For viewing multiline text
    CREATE_LISTVIEW, // For viewing lists
    CREATE_SIMPLELISTVIEW,
    CREATE_GRIDVIEW, // For viewing tables
    CREATE_BUTTON,
    CREATE_SWITCH,
    CREATE_PICKER, // called Spinner in Android
    CREATE_LINEAR_LAYOUT,
    CREATE_TABLE_LAYOUT,
    CREATE_AUTO_COLUMN_LAYOUT,
    CREATE_HEADING_TEXT,
    CREATE_TEXT,
    CREATE_DIALOG, // For future
    CREATE_IMAGEVIEW,
    CREATE_ACTION_SHEET,
    CREATE_CHECKBOX,
    CREATE_RADIO_GROUP,
    CREATE_SEPARATOR,
    CREATE_SLIDER,
    CREATE_ACTIONBAR,
    DELETE_ELEMENT,
    END_MODAL,
    SHOW_DIALOG,
    SHOW_MESSAGE_DIALOG,
    SHOW_INPUT_DIALOG,
    SHOW_ACTION_SHEET,
    LAUNCH_BROWSER,
    POST_NOTIFICATION,
    HISTORY_GO_BACK,
    HISTORY_GO_FORWARD,
    CLEAR, // Clears the contents of GridView
    SET_INT_VALUE, // Sets value of radio groups, checkboxes and pickers
    SET_TEXT_VALUE, // Sets value of textfields, labels and images
    SET_INT_DATA,
    SET_TEXT_DATA, // Sets the cell value of GridView
    SET_LABEL, // Sets label for buttons and checkboxes
    SET_ENABLED,
    SET_READONLY,
    SET_VISIBILITY,
    SET_SHAPE, // Specifies the number of rows and columns in a GridView
    SET_STYLE,
    SET_ERROR,
    FLUSH_VIEW, // Flushes GridView content
    UPDATE_PREFERENCE,
    ADD_OPTION,
    ADD_SHEET,
    ADD_COLUMN,
    RESHAPE_SHEET,
    QUIT_APP,

    // Timers
    CREATE_TIMER,
    
    // In-app purchases
    LIST_PRODUCTS,
    BUY_PRODUCT,
    LIST_PURCHASES,
    CONSUME_PURCHASE
  }

  public NativeCommand(FrameWork frame, int messageTypeId, int internalId, int childInternalId, int value, byte[] textValue, byte[] textValue2, int flags, int row, int column, int sheet){

    this.frame = frame;
    command = CommandType.values()[messageTypeId];
    this.internalId = internalId;
    this.childInternalId = childInternalId;
    this.value = value;
    this.flags = flags;
    this.rowNumber = row;
    this.columnNumber = column;
    this.sheet = sheet;

    if (textValue != null) {
	this.textValue = new String(textValue, frame.getCharset());
    }
    if (textValue2 != null) {
	this.textValue2 = new String(textValue2, frame.getCharset());
    }
  }
  
  public NativeCommand(FrameWork frame, int messageTypeId, int internalId, int childInternalId, int value, byte[] textValue, byte[] textValue2, int flags){
    this.frame = frame;
    command = CommandType.values()[messageTypeId];
    this.internalId = internalId;
    this.childInternalId = childInternalId;
    this.value = value;
    this.flags = flags;

    if (textValue != null) {
	this.textValue = new String(textValue, frame.getCharset());
    }
    if (textValue2 != null) {
	this.textValue2 = new String(textValue2, frame.getCharset());
    }
  }
  
  public void apply(NativeCommandHandler view) {

    System.out.println("Processing message " + command + " id: " + internalId + " Child id: " + getChildInternalId());
    
    switch (command) {

    case CREATE_FORMVIEW:
      FWScrollView scrollView = new FWScrollView(frame, textValue);
      scrollView.setId(getChildInternalId());
      scrollView.setPadding(10, 10, 10, 10);
      FrameWork.addToViewList(scrollView);
      
      if (view == null){
	System.out.println("view was null");
	if (frame.getCurrentViewId() == 0){
	  scrollView.setValue(2);
	}
      } else {
	view.addChild(scrollView);
      }
      break;
    case CREATE_NAVIGATIONVIEW:
      NavigationLayout navigationLayout = new NavigationLayout(frame);
      navigationLayout.setId(getChildInternalId());
      FrameWork.addToViewList(navigationLayout);
      if (frame.getCurrentDrawerViewId() == 0){
	System.out.println("setting current navigationDrawer to " + getChildInternalId());
	frame.setCurrentDrawerViewId(getChildInternalId());
      }
      break;
    case CREATE_BASICVIEW:
    case CREATE_LINEAR_LAYOUT:
      FWLayout layout = createLinearLayout();
      view.addChild(layout);
      break;
      
    case CREATE_AUTO_COLUMN_LAYOUT:{
      FWAuto auto  = new FWAuto(frame);
      auto.setId(getChildInternalId());
      FrameWork.addToViewList(auto);
      view.addChild(auto);
    }
      break;
    case CREATE_TABLE_LAYOUT:
      FWTable table = createTableLayout(false);
      view.addChild(table);
      break;

    case CREATE_BUTTON:
      FWButton button = createButton();
      view.addChild(button);
      break;

    case CREATE_PICKER:
      FWPicker picker = createSpinner();
      view.addChild(picker);
      break;
      
    case CREATE_SWITCH:
      FWSwitch click = createSwitch();
      view.addChild(click);
      break;
      
    case CLEAR:
      view.clear();
      break;
      
    case CREATE_GRIDVIEW:
      //TODO
      //Fix from being debug status
//      FWLayout debugList = createDebugResultsScreen();
      FWList debugList = new FWList(frame, new FWAdapter2(frame, null));
      debugList.setId(childInternalId);
      FrameWork.addToViewList(debugList);
      view.addChild(debugList);
      break;
    case CREATE_LISTVIEW:
    case CREATE_SIMPLELISTVIEW:
      if (isSet(FLAG_SLIDERVIEW)) {
	SliderLayout slider = new SliderLayout(frame);
	slider.setId(childInternalId);
	FrameWork.addToViewList(slider);
	view.addChild(slider);
      } else {
	FWList listView = new FWList(frame, new FWAdapter2(frame, null));
	LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
	params.weight = 1.0f;
	// params.gravity = Gravity.TOP;
	listView.setLayoutParams(params);
	listView.setId(childInternalId);
	FrameWork.addToViewList(listView);
	view.addChild(listView);
      }
      break;
      
    case CREATE_TIMER:
      Timer timer = new Timer();
      timer.schedule((new TimerTask(){
	@Override
	public void run() {
	  FrameWork.timerEvent(System.currentTimeMillis() / 1000, internalId, childInternalId);
	}
      }), value, value);
      break;
    case CREATE_CHECKBOX:
      FWCheckBox checkBox = createCheckBox();
      FrameWork.addToViewList(checkBox);
      view.addChild(checkBox);
      break;
    case CREATE_OPENGL_VIEW:
      frame.createNativeOpenGLView(childInternalId);
      break;

    case CREATE_TEXTVIEW:
      FWEditText editTextView = createBigEditText();
      view.addChild(editTextView);
      break;
      
    case CREATE_TEXTFIELD:
      FWEditText editText = createEditText();
      view.addChild(editText);
      break;
      
    case CREATE_RADIO_GROUP:
      FWRadioGroup radioGroup = new FWRadioGroup(frame);
      radioGroup.setId(childInternalId);
      break;
      
    case CREATE_HEADING_TEXT:
      FWTextView headingText = createTextView(true);
      view.addChild(headingText);
      break;
    case CREATE_TEXT:
      FWTextView textView = createTextView(false);
      view.addChild(textView);
      break;

    case CREATE_IMAGEVIEW:
      ImageView imageView = createImageView();
      view.addChild(imageView);
      break;
      
    case ADD_OPTION:
      view.addOption(getValue(), getTextValue());
      break;
    case ADD_COLUMN:
      view.addColumn(getTextValue(), getValue());
      break;
    case ADD_SHEET:
      System.out.println("add_sheet: " + textValue + " " + rowNumber + " " + columnNumber + " " + sheet);
      view.setValue(textValue);
    case POST_NOTIFICATION:
      frame.createNotification(getTextValue(), getTextValue2());
      break;
    case CREATE_APPLICATION:
      frame.setAppId(getInternalId());
      frame.setSharedPreferences(textValue);
      if (isSet(FLAG_USE_PURCHASES_API)) {
	System.out.println("Initializing purchaseHelper");
	frame.initializePurchaseHelper(textValue2, new IabHelper.OnIabSetupFinishedListener() {

	  @Override
	  public void onIabSetupFinished(IabResult result) {
	    if (result.isSuccess()) {
	      System.out.println("PurchaseHelper successfully setup");
	      sendInventory(frame.getPurchaseHelperInventory());
	    } else {
	      System.out.println("PurchaseHelper failed to setup");
	    }
	  }
	});
      }
      break;
    case SET_INT_VALUE:
      view.setValue(getValue());
      break;
    case SET_TEXT_VALUE:
      view.setValue(textValue);
      break;
    case SET_TEXT_DATA:
      System.out.println("set_text_data: " + textValue + " " + rowNumber + " " + columnNumber + " " + sheet);
      view.addData(textValue, rowNumber, columnNumber, sheet);
      break;
    case SET_VISIBILITY:
      if (value == 0){
	view.setViewVisibility(false);
      } else {
	view.setViewVisibility(true);
      }
      break;
    case SET_ENABLED:
      view.setViewEnabled(value != 0);
      break;
    case SET_STYLE:
      view.setStyle(textValue, textValue2);
      break;
    case SET_LABEL:
      frame.actionBar.setValue(textValue);
      break;
    case SET_ERROR:
      view.setError(value != 0, textValue);
      break;
    case LAUNCH_BROWSER:
      frame.launchBrowser(getTextValue());
      break;
    case END_MODAL:
      view.setValue(0);
      break;
    case SHOW_DIALOG:
      view.setValue(1);
      break;
    case SHOW_MESSAGE_DIALOG:
      showMessageDialog(textValue, textValue2);
      break;
    case SHOW_INPUT_DIALOG:
      showInputDialog(textValue, textValue2);
      break;
    case CREATE_DIALOG:
      FWDialog dialog = new FWDialog(frame, childInternalId);
      dialog.setTitle(textValue);
      FrameWork.addToViewList(dialog);
      break;
    case CREATE_ACTION_SHEET:
      createActionSheet();
      break;
    case CREATE_ACTIONBAR:
      FWActionBar ab = new FWActionBar(frame, textValue, childInternalId);
      frame.actionBar = ab;
      FrameWork.addToViewList(ab);
      break;
    case FLUSH_VIEW:
      view.flush();
      break;
    case QUIT_APP:
      // TODO
      frame.finish();
      break;
    case UPDATE_PREFERENCE:
    	//Now stores String value to string key.
      frame.getPreferencesEditor().putString(textValue, textValue2);
      frame.getPreferencesEditor().apply();
      break;
    case DELETE_ELEMENT:
      deleteElement(view, childInternalId);
      break;
    case BUY_PRODUCT:
      try {
	launchPurchase(textValue);
      } catch (IabAsyncInProgressException e) {
	e.printStackTrace();
	System.out.println("Error on launchPurchase with message: " + e.getMessage());
      }
      break;
    case RESHAPE_SHEET:
      view.reshape(sheet, value);
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
  
  private FWTable createTableLayout(boolean autoSize){
    FWTable table = new FWTable(frame);
    table.setId(getChildInternalId());
    if (autoSize){
      table.setAutoSize(true);
    } else {
      Log.d("table", "ALERT " + value);
      table.setColumnCount(value);
    }
    table.setStretchAllColumns(true);
    table.setShrinkAllColumns(true);
    FrameWork.addToViewList(table);
    return table;
  }
  
  private FWSwitch createSwitch() {
    FWSwitch click = new FWSwitch(frame);
    click.setId(childInternalId);
    if (textValue != "") {
      click.setTextOn(textValue);
    }
    if (textValue2 != "") {
      click.setTextOff(textValue2);
    }
    click.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
      @Override
      public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
	frame.intChangedEvent(System.currentTimeMillis() / 1000.0, buttonView.getId(), isChecked ? 1 : 0, 0);
      }
    });
    FrameWork.addToViewList(click);
    return click;
  }
  
  private void deleteElement(NativeCommandHandler parent, int childId) {
    FrameWork.views.remove(childInternalId);
    if (parent instanceof ViewGroup) {
      ViewGroup group = (ViewGroup) parent;
      int childCount = group.getChildCount();
      for (int i = 0; i < childCount; i++) {
	View view = group.getChildAt(i);
	if (view.getId() == childInternalId) {
	  ((ViewGroup) parent).removeViewAt(i);
	  break;
	}
      }
    } else {
      System.out.println("Deletion parent was not an instance of ViewGroup");
    }
  }
  
  private ImageView createImageView() {
    ImageView imageView = new ImageView(frame);
    imageView.setId(childInternalId);
    try {
      InputStream is = frame.getAssets().open(textValue);
      Bitmap bitmap = BitmapFactory.decodeStream(is);
      imageView.setImageBitmap(bitmap);
      return imageView;
    } catch (IOException e) {
      e.printStackTrace();
      System.out.println("error loading asset file to imageView");
      System.exit(1);
    }
    return null;
  }
  
  private FWLayout createLinearLayout() {
    FWLayout layout = new FWLayout(frame);
    LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
    params.weight = 1.0f;
//    params.gravity = Gravity.FILL;
//    layout.setBaselineAligned(false);
    layout.setLayoutParams(params);
    layout.setId(getChildInternalId());
    FrameWork.addToViewList(layout);
    if (getValue() == 2) {
	layout.setOrientation(LinearLayout.HORIZONTAL);
    } else {
	layout.setOrientation(LinearLayout.VERTICAL);
    }
    return layout;
  }

  private FWButton createButton() {
    FWButton button = new FWButton(frame);
    button.setId(getChildInternalId());
    button.setText(getTextValue());
    LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
//    params.weight = 1;
//    params.gravity = Gravity.BOTTOM;
    button.setLayoutParams(params);

    button.setOnClickListener(new OnClickListener() {
      @Override
      public void onClick(View arg0) {
	System.out.println("Java: my button was clicked with id " + getChildInternalId());
	if (!FrameWork.transitionAnimation) {
	  frame.intChangedEvent(System.currentTimeMillis() / 1000.0, getChildInternalId(), 1, 0);
	}
      }
    });
    FrameWork.addToViewList(button);
    return button;
  }
  
  private FWEditText createEditText(){
    
    final FWEditText editText = new FWEditText(frame);
    editText.setId(getChildInternalId());
    editText.setText(getTextValue());
//    editText.setMinWidth(80);
    editText.setSingleLine();
    editText.setImeOptions(EditorInfo.IME_ACTION_DONE);
    
    if (isSet(FLAG_PASSWORD) && isSet(FLAG_NUMERIC)){
      editText.setInputType(InputType.TYPE_NUMBER_VARIATION_PASSWORD);
    } else if (isSet(FLAG_PASSWORD)) {
      editText.setInputType(InputType.TYPE_TEXT_VARIATION_PASSWORD);
    } else if (isSet(FLAG_NUMERIC)){
      editText.setInputType(InputType.TYPE_CLASS_NUMBER);
    }
    editText.addTextChangedListener(new TextWatcher() {
      public void afterTextChanged(Editable editable) {
	String inputText = editable.toString();
	byte[] b = inputText.getBytes(frame.getCharset());
	frame.textChangedEvent(System.currentTimeMillis() / 1000.0, getChildInternalId(), b);
      }
      public void beforeTextChanged(CharSequence s, int start, int count, int after) {}
      public void onTextChanged(CharSequence s, int start, int before, int count) {}
   });
    FrameWork.addToViewList(editText);
    return editText;
  }
  

  private FWEditText createBigEditText() {
    final FWEditText editText = new FWEditText(frame);
    LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
    editText.setMinLines(4);
    editText.setLayoutParams(params);
    editText.setId(getChildInternalId());
    editText.setText(getTextValue());
    editText.setVerticalScrollBarEnabled(true);
    editText.setMovementMethod(new ScrollingMovementMethod());
    editText.addDelayedChangeListener(getChildInternalId());
    FrameWork.addToViewList(editText);
    return editText;
  }

  private FWPicker createSpinner(){
    FWPicker picker = new FWPicker(frame);
    picker.setId(getChildInternalId());
    FrameWork.addToViewList(picker);
    
    return picker;
  }
  
  private FWCheckBox createCheckBox() {
    FWCheckBox checkBox = new FWCheckBox(frame);
    checkBox.setPadding(0, 0, 10, 0);
    checkBox.setId(childInternalId);
    if (textValue != "") {
      checkBox.setText(textValue);
    }
    checkBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
      @Override
      public void onCheckedChanged(CompoundButton box, boolean isChecked) {
	frame.intChangedEvent(System.currentTimeMillis() / 1000.0, childInternalId, isChecked ? 1 : 0, 0);
      }
    });
    return checkBox;
  }

  private FWTextView createTextView(boolean bolded) {
    FWTextView textView = new FWTextView(frame);
    textView.setId(getChildInternalId());
    if (bolded) {
      textView.setTypeface(null, Typeface.BOLD);
    }
    if (isSet(FLAG_HYPERLINK)) {
      textView.setMovementMethod(LinkMovementMethod.getInstance());
      String text = "<a href='" + textValue2 + "'>" + textValue + "</a>";
      textView.setText(Html.fromHtml(text));
    } else {
      textView.setText(textValue);
    }

    FrameWork.addToViewList(textView);
    return textView;
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
	dialog.dismiss();
      }
    });

    // Positive button listener
    builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
      public void onClick(DialogInterface dialog, int id) {
	String inputText = String.valueOf(input.getText());
	byte[] b = inputText.getBytes(frame.getCharset());
	frame.endModal(System.currentTimeMillis() / 1000.0, 1, b);
	dialog.dismiss();
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
  
  private void launchPurchase(final String productId) throws IabAsyncInProgressException {
    // Sku = product id from google account
    frame.getPurchaseHelper().launchPurchaseFlow(frame, productId, IabHelper.ITEM_TYPE_INAPP, null, 1, new IabHelper.OnIabPurchaseFinishedListener() {

      @Override
      public void onIabPurchaseFinished(IabResult result, Purchase info) {
	if (result.isSuccess()) {
	  System.out.println("Purchase of product id " + productId + " completed");
	  FrameWork.onPurchaseEvent(System.currentTimeMillis() / 1000.0, frame.getAppId(), info.getSku(), true, info.getPurchaseTime() / 1000.0);
	  // TODO
	} else {
	  System.out.println("Purchase of product id " + productId + " failed");
	  // TODO
	}
      }

    }, "");
  }

  private void sendInventory(Inventory inventory) {
    List<Purchase> purchaseList = inventory.getAllPurchases();
    System.out.println("getting purchase history. Purchase child size: " + purchaseList.size());
    for (Purchase purchase : inventory.getAllPurchases()) {
      FrameWork.onPurchaseEvent(System.currentTimeMillis() / 1000.0, frame.getAppId(), purchase.getSku(), false, purchase.getPurchaseTime() / 1000.0);
    }
  }
  
  private Boolean isSet(int flag) {
    return (flags & flag) != 0;
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
