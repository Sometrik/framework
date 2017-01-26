package com.sometrik.framework;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import com.android.trivialdrivesample.util.IabHelper;
import com.android.trivialdrivesample.util.IabHelper.IabAsyncInProgressException;
import com.android.trivialdrivesample.util.IabResult;
import com.android.trivialdrivesample.util.Inventory;
import com.android.trivialdrivesample.util.Purchase;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.text.Editable;
import android.text.Html;
import android.text.InputType;
import android.text.TextWatcher;
import android.text.method.LinkMovementMethod;
import android.text.method.ScrollingMovementMethod;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.PopupMenu;
import android.widget.PopupMenu.OnMenuItemClickListener;
import android.widget.ScrollView;
import android.widget.Switch;
import android.widget.TableLayout;
import android.widget.TextView;

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
  

  private final int FLAG_PADDING_LEFT = 1;
  private final int FLAG_PADDING_RIGHT = 2;
  private final int FLAG_PADDING_TOP = 4;
  private final int FLAG_PADDING_BOTTOM = 8;
  private final int FLAG_PASSWORD = 16;
  private final int FLAG_NUMERIC = 32;
  private final int FLAG_HYPERLINK = 64;
  private final int FLAG_USE_PURCHASES_API = 128;
 
  public enum CommandType {
    CREATE_PLATFORM,
    CREATE_APPLICATION,
    CREATE_BASICVIEW,
    CREATE_FORMVIEW,
    CREATE_OPENGL_VIEW,
    CREATE_TEXTFIELD, // For viewing single value
    CREATE_TEXTVIEW, // For viewing multiline text
    CREATE_LISTVIEW, // For viewing lists
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
    SHOW_MESSAGE_DIALOG,
    SHOW_INPUT_DIALOG,
    SHOW_ACTION_SHEET,
    LAUNCH_BROWSER,
    POST_NOTIFICATION,
    HISTORY_GO_BACK,
    HISTORY_GO_FORWARD,
    SET_INT_VALUE, // Sets value of radio groups, checkboxes and pickers
    SET_TEXT_VALUE, // Sets value of textfields, labels and images
    SET_LABEL, // Sets label for buttons and checkboxes
    SET_ENABLED,
    SET_STYLE,
    SET_ERROR,
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
  
  public NativeCommand(FrameWork frame, int messageTypeId, int internalId, int childInternalId, int value, byte[] textValue, byte[] textValue2, int flags){
    this.frame = frame;
    command = CommandType.values()[messageTypeId];
    this.internalId = internalId;
    this.childInternalId = childInternalId;
    this.value = value;
    this.flags = flags;

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
      FWScrollView scrollView = new FWScrollView(frame);
      scrollView.setId(getChildInternalId());
      FrameWork.addToViewList(scrollView);
      
      if (view == null){
	System.out.println("view was null");
	if (frame.getCurrentViewId() == 0){
	  scrollView.setValue(1);
	}
      } else {
	view.addChild(scrollView);
      }
      break;
    case CREATE_BASICVIEW:
    case CREATE_LINEAR_LAYOUT:
      FWLayout layout = createLinearLayout();
      view.addChild(layout);
      break;
      
    case CREATE_AUTO_COLUMN_LAYOUT:
      final FWGridView autoLayout = new FWGridView(frame);
      autoLayout.setId(getChildInternalId());
      autoLayout.setColumnWidth(200);
      autoLayout.setNumColumns(GridView.AUTO_FIT);
      autoLayout.setVerticalSpacing(0);
      autoLayout.setHorizontalSpacing(0);
      autoLayout.setStretchMode(GridView.STRETCH_COLUMN_WIDTH);
      final FWAdapter adapteri = new FWAdapter(frame, new ArrayList<View>());
      autoLayout.post(new Runnable() {
	    public void run() {
	      autoLayout.setAdapter(adapteri);
	      autoLayout.setupAdapter(adapteri);
	    }
	});
//      autoLayout.setAdapter(adapteri);
//      FlowLayout autoLayout = new FlowLayout(frame, null);
//      autoLayout.setId(getChildInternalId());
      FrameWork.addToViewList(autoLayout);
      view.addChild(autoLayout);
      break;
    case CREATE_TABLE_LAYOUT:
      FWTable table = createTableLayout();
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

    case CREATE_CHECKBOX:
      FWCheckBox checkBox = new FWCheckBox(frame);
      checkBox.setId(childInternalId);
      if (textValue != "") {
	checkBox.setText(textValue);
      }
      checkBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
	@Override
	public void onCheckedChanged(CompoundButton box, boolean isChecked) {
	  frame.intChangedEvent(System.currentTimeMillis() / 1000.0, childInternalId, isChecked ? 1 : 0);	  
	}
      });
      FrameWork.addToViewList(checkBox);
      view.addChild(checkBox);
      break;
    case CREATE_OPENGL_VIEW:
      NativeSurface surface = frame.createNativeOpenGLView(childInternalId);
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
    case CREATE_TEXT:
      FWTextView textView = createTextView();
      view.addChild(textView);
      break;

    case CREATE_IMAGEVIEW:
      ImageView imageView = createImageView();
      view.addChild(imageView);
      break;
      
    case ADD_OPTION:
      // Forward Command to FWPicker
      view.addOption(getValue(), getTextValue());
      break;
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
    case SET_ENABLED:
      view.setViewEnabled(value != 0);
      break;
    case SET_STYLE:
      view.setStyle(textValue, textValue2);
      break;
    case SET_ERROR:
      view.setError(value != 0, textValue);
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
      frame.getPreferencesEditor().putString(textValue, textValue2);
      frame.getPreferencesEditor().apply();
      break;
    case DELETE_ELEMENT:
      FrameWork.views.remove(childInternalId);
      ((ViewGroup) view).removeViewAt(childInternalId);
      break;
    case BUY_PRODUCT:
      try {
	launchPurchase(textValue);
      } catch (IabAsyncInProgressException e) {
	e.printStackTrace();
	System.out.println("Error on launchPurchase with message: " + e.getMessage());
      }
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
  
  private FWTable createTableLayout(){
    FWTable table = new FWTable(frame);
    table.setId(getChildInternalId());
    table.setColumnCount(value);
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
	frame.intChangedEvent(System.currentTimeMillis() / 1000.0, buttonView.getId(), isChecked ? 1 : 0);
      }
    });
    FrameWork.addToViewList(click);
    return click;
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
    LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
    params.weight = 1.0f;
    params.gravity = Gravity.FILL;
    layout.setBaselineAligned(false);
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
   
    button.setOnClickListener(new OnClickListener() {
      @Override
      public void onClick(View arg0) {
	System.out.println("Java: my button was clicked with id " + getChildInternalId());
	frame.intChangedEvent(System.currentTimeMillis() / 1000.0, getChildInternalId(), 1);
      }
    });
    FrameWork.addToViewList(button);
    return button;
  }
  
  private FWEditText createEditText(){
    final FWEditText editText = new FWEditText(frame);
    editText.setId(getChildInternalId());
    editText.setText(getTextValue());
    editText.setMinimumWidth(120000 / (int) frame.getScreenWidth());
    if (isSet(FLAG_PASSWORD) && isSet(FLAG_NUMERIC)){
      editText.setInputType(InputType.TYPE_NUMBER_VARIATION_PASSWORD);
    } else if (isSet(FLAG_PASSWORD)) {
      editText.setInputType(InputType.TYPE_TEXT_VARIATION_PASSWORD);
    } else if (isSet(FLAG_NUMERIC)){
      editText.setInputType(InputType.TYPE_CLASS_NUMBER);
    }
    editText.addTextChangedListener(new TextWatcher() {
      public void afterTextChanged(Editable editable) {
	frame.textChangedEvent(System.currentTimeMillis() / 1000.0, getChildInternalId(), editable.toString());
      }
      public void beforeTextChanged(CharSequence s, int start, int count, int after) {}
      public void onTextChanged(CharSequence s, int start, int before, int count) {}
   });
    FrameWork.addToViewList(editText);
    return editText;
  }
  

  private FWEditText createBigEditText() {
    final FWEditText editText = new FWEditText(frame);
    editText.setId(getChildInternalId());
    editText.setText(getTextValue());
    // editText.setMinimumWidth(120000 / (int) frame.getScreenWidth());
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

  private FWTextView createTextView() {
    FWTextView textView = new FWTextView(frame);
    textView.setId(getChildInternalId());
    
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

  private void launchPurchase(final String productId) throws IabAsyncInProgressException {
    // Sku = product id from google account
    frame.getPurchaseHelper().launchPurchaseFlow(frame, productId, IabHelper.ITEM_TYPE_INAPP, null, 1, new IabHelper.OnIabPurchaseFinishedListener() {

      @Override
      public void onIabPurchaseFinished(IabResult result, Purchase info) {
	if (result.isSuccess()) {
	  System.out.println("Purchase of product id " + productId + " completed");
	  FrameWork.onPurchaseEvent(info.getPurchaseTime() / 1000.0, info.getSku(), true);
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
    System.out.println("getting purchase history. Purchase list size: " + purchaseList.size());
    for (Purchase purchase : inventory.getAllPurchases()) {
      FrameWork.onPurchaseEvent(purchase.getPurchaseTime() / 1000.0, purchase.getSku(), false);
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
