package com.sometrik.framework;

import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import com.android.trivialdrivesample.util.IabHelper;
import com.android.trivialdrivesample.util.IabHelper.IabAsyncInProgressException;
import com.android.trivialdrivesample.util.IabResult;
import com.android.trivialdrivesample.util.Inventory;
import com.android.trivialdrivesample.util.Purchase;

import android.support.v4.app.ShareCompat;
import android.text.Editable;
import android.text.Html;
import android.text.InputType;
import android.text.TextWatcher;
import android.text.method.LinkMovementMethod;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewParent;
import android.view.inputmethod.EditorInfo;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.CompoundButton;
import android.widget.LinearLayout;
import android.widget.PopupMenu;
import android.widget.PopupMenu.OnMenuItemClickListener;
import android.widget.Toast;

public class NativeCommand {

  private int internalId = 0;
  private int childInternalId = 0;
  private int value = 0;
  private int flags = 0;
  private byte[] byteArray;
  private byte[] byteArray2;
  private CommandType command;
  private String key;
  private FrameWork frame;
  private ArrayList<PopupMenu> menuList = new ArrayList<PopupMenu>();
  private int rowNumber = -1;
  private int columnNumber = -1;
  private int sheet = 0;
  private int width = 0;
  private int height = 0;
  
  private final int FLAG_PASSWORD = 16;
  private final int FLAG_NUMERIC = 32;
  private final int FLAG_USE_PURCHASES_API = 128;
  private final int FLAG_SLIDERVIEW = 256;
  private final int FLAG_STICKY_HEADER = 512;
  
  public enum Selector {
    NORMAL,
    ACTIVE,
    HOVER,
    SELECTED,
    DISABLED,
    LINK
  };
 
  public enum CommandType {
    CREATE_APPLICATION,
    CREATE_FRAMEVIEW,
    CREATE_NAVIGATIONVIEW,
    CREATE_OPENGL_VIEW,
    CREATE_TEXTFIELD, // For viewing single value
    CREATE_TEXTVIEW, // For viewing multiline text
    CREATE_LISTVIEW, // For viewing lists
    CREATE_GRIDVIEW, // For viewing tables
    CREATE_BUTTON,
    CREATE_SWITCH,
    CREATE_PICKER, // called Spinner in Android
    CREATE_LINEAR_LAYOUT,
    CREATE_FRAME_LAYOUT,
    CREATE_EVENT_LAYOUT,
    CREATE_SCROLL_LAYOUT,
    CREATE_TABLE_LAYOUT,
    CREATE_AUTO_COLUMN_LAYOUT,
    CREATE_TEXT,
    CREATE_LINK,
    CREATE_DIALOG,
    CREATE_IMAGEVIEW,
    CREATE_ACTION_SHEET,
    CREATE_CHECKBOX,
    CREATE_RADIO_GROUP,
    CREATE_SEPARATOR,
    CREATE_SLIDER,
    CREATE_ACTIONBAR,
    CREATE_NAVIGATIONBAR,
    CREATE_PROGRESSBAR,
    CREATE_TOAST,
    CREATE_NOTIFICATION,
    DELETE_ELEMENT,
    REMOVE_CHILD,
    REORDER_CHILD,
    LAUNCH_BROWSER,
    HISTORY_GO_BACK,
    HISTORY_GO_FORWARD,
    CLEAR,
    SET_INT_VALUE, // Sets value of radio groups, checkboxes and pickers
    SET_TEXT_VALUE, // Sets value of textfields, labels and images
    SET_INT_DATA,
    SET_TEXT_DATA,
    SET_VISIBILITY,
    SET_SHAPE, // Specifies the number of rows and columns in a GridView
    SET_STYLE,
    SET_ERROR,
    ADD_IMAGE_URL,
    FLUSH_VIEW,
    UPDATE_PREFERENCE,
    DELETE_PREFERENCE,
    COMMIT_PREFERENCES,
    ADD_OPTION,
    ADD_SHEET,
    ADD_COLUMN,
    RESHAPE_TABLE,
    RESHAPE_SHEET,	
    QUIT_APP,

    // Timers
    CREATE_TIMER,
    
    // In-app purchases
    LIST_PRODUCTS,
    BUY_PRODUCT,
    LIST_PURCHASES,
    CONSUME_PURCHASE,

    SHARE_LINK
  }

  public NativeCommand(FrameWork frame, int messageTypeId, int internalId, int childInternalId, int value, byte[] textValue, byte[] textValue2, int flags, int row, int column, int sheet, int width, int height) {

    this.frame = frame;
    command = CommandType.values()[messageTypeId];
    this.internalId = internalId;
    this.childInternalId = childInternalId;
    this.value = value;
    this.flags = flags;
    this.rowNumber = row;
    this.columnNumber = column;
    this.sheet = sheet;
    this.width = width;
    this.height = height;
    byteArray = textValue;
    byteArray2 = textValue2;
  }
  
  public NativeCommand(FrameWork frame, int messageTypeId, int internalId, int childInternalId, int value, byte[] textValue, byte[] textValue2, int flags) {
    this.frame = frame;
    command = CommandType.values()[messageTypeId];
    this.internalId = internalId;
    this.childInternalId = childInternalId;
    this.value = value;
    this.flags = flags;
    byteArray = textValue;
    byteArray2 = textValue2;
  }

  private String getTextValueAsString() {
    if (byteArray != null) {
      return new String(byteArray, frame.getCharset());
    } else {
      return "";
    }
  }

  private String getTextValue2AsString() {
    if (byteArray2 != null) {
      return new String(byteArray2, frame.getCharset());
    } else {
      return "";
    }
  }
  
  public void apply(NativeCommandHandler view) {
    // System.out.println("Processing message " + command + " id: " + internalId + " Child id: " + getChildInternalId());
    
    switch (command) {
      
    case CREATE_FRAMEVIEW: {
      FWFrameView frameView = new FWFrameView(frame, getTextValueAsString());
      frameView.setId(getChildInternalId());
      frame.addToViewList(frameView);

      if (view == null){
	System.out.println("view was null");
	if (frame.getCurrentViewId() == 0){
	  frameView.setValue(2);
	}
      } else {
	view.addChild(frameView);
      }
    }
     break;
     
    case CREATE_NAVIGATIONVIEW:
      NavigationLayout navigationLayout = new NavigationLayout(frame, getChildInternalId());
      frame.addToViewList(navigationLayout);
      if (frame.getCurrentDrawerViewId() == 0){
	System.out.println("setting current navigationDrawer to " + getChildInternalId());
	frame.setCurrentDrawerViewId(getChildInternalId());
      }
      break;
	
    case CREATE_LINEAR_LAYOUT: {
      FWLayout layout = createLinearLayout(getValue());
      if (view != null) {
	view.addChild(layout);
      }
      break;
    }

    case CREATE_FRAME_LAYOUT: {
      FWFrameLayout layout = new FWFrameLayout(frame, getChildInternalId(), false);
      frame.addToViewList(layout);
      if (view != null) {
	view.addChild(layout);
      }
    }
      break;
    case CREATE_EVENT_LAYOUT: {
      FWFrameLayout layout = new FWFrameLayout(frame, getChildInternalId(), true);
      frame.addToViewList(layout);
      if (view != null) {
	view.addChild(layout);
      }
    }
      break;
    case CREATE_SCROLL_LAYOUT:
      if (view != null) {
	FWScrollLayout scrollLayout = new FWScrollLayout(frame, getChildInternalId());
	frame.addToViewList(scrollLayout);
	view.addChild(scrollLayout);
      }
    break;
    case CREATE_AUTO_COLUMN_LAYOUT:
      if (view != null) {
	FWAuto auto  = new FWAuto(frame);
	auto.setId(getChildInternalId());
	frame.addToViewList(auto);
	view.addChild(auto);
      }
      break;
    case CREATE_TABLE_LAYOUT:
      if (view != null) {
	FWTable table = createTableLayout(false);
	view.addChild(table);
      }
      break;

    case CREATE_BUTTON:
      if (view != null) {
	FWButton button = createButton();
	view.addChild(button);
      }
      break;

    case CREATE_PICKER:
      if (view != null) {
	FWPicker picker = createSpinner();
	view.addChild(picker);
      }
      break;
      
    case CREATE_SWITCH:
      if (view != null) {
	FWSwitch click = createSwitch();
	view.addChild(click);
      }
      break;
      
    case CLEAR:
      if (view != null) {
	view.clear();
      }
      break;
      
    case CREATE_GRIDVIEW:
      if (view != null) {
	// TODO
	// Fix from being debug status
	// FWLayout debugList = createDebugResultsScreen();
	FWList debugList = new FWList(frame, new FWAdapter2(frame, null));
	debugList.setId(childInternalId);
	frame.addToViewList(debugList);
	view.addChild(debugList);
      }
      break;
      
    case CREATE_LISTVIEW:
      if (view != null) {
	if (isSet(FLAG_SLIDERVIEW)) {
	  // Real slider stuff
	  SliderLayout slider = new SliderLayout(frame);
	  slider.setId(childInternalId);
	  frame.addToViewList(slider);
	  view.addChild(slider);
	} else {
	  FWList listView = new FWList(frame, new FWAdapter2(frame, null));
	  LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
	  params.weight = 1.0f;
	  // params.gravity = Gravity.TOP;
	  listView.setLayoutParams(params);
	  listView.setId(childInternalId);
	  listView.setOnItemClickListener(new OnItemClickListener() {
	    @Override
	    public void onItemClick(AdapterView<?> arg0, View arg1, int groupPosition, long id) {
	      System.out.println("row clicked. Sending intChangedEvent of " + (groupPosition - 1));
	      frame.sendNativeValueEvent(childInternalId, (groupPosition - 1), 0);
	    }
	  });
	  frame.addToViewList(listView);
	  view.addChild(listView);
	}
      }
      break;
      
    case CREATE_TIMER: {
      	Timer timer = new Timer();
      	timer.scheduleAtFixedRate((new TimerTask() {
      	  @Override
      	  public void run() {
      	    frame.timerEvent(internalId, childInternalId);
      	  }
      	}), 1000, 1000);
    }
      break;
    case CREATE_CHECKBOX:
      if (view != null) {
	FWCheckBox checkBox = createCheckBox();
	frame.addToViewList(checkBox);
	view.addChild(checkBox);
      }
      break;
    case CREATE_OPENGL_VIEW:
      frame.createNativeOpenGLView(childInternalId);
      break;

    case CREATE_TEXTVIEW:
      if (view != null) {
	FWEditText editTextView = createBigEditText();
	view.addChild(editTextView);
      }
      break;
      
    case CREATE_TEXTFIELD:
      if (view != null) {
	FWEditText editText = createEditText();
	view.addChild(editText);
      }
      break;
      
    case CREATE_RADIO_GROUP:
      if (view != null) {
	FWRadioGroup radioGroup = new FWRadioGroup(frame);
	radioGroup.setId(childInternalId);
	frame.addToViewList(radioGroup);
	view.addChild(radioGroup);
      }
      break;
      
    case CREATE_TEXT:
      if (view != null) {
	FWTextView textView = new FWTextView(frame, getValue() != 0);
	textView.setId(getChildInternalId());
	textView.setText(getTextValueAsString());
	frame.addToViewList(textView);
	view.addChild(textView);
      }
      break;

    case CREATE_LINK:
      if (view != null) {
	FWTextView textView = new FWTextView(frame, false);
	textView.setId(getChildInternalId());
	textView.setMovementMethod(LinkMovementMethod.getInstance());
	String text = "<a href='" + getTextValue2AsString() + "'>" + getTextValueAsString() + "</a>";
	textView.setText(Html.fromHtml(text));
	frame.addToViewList(textView);
	view.addChild(textView);
      }
      break;

    case CREATE_IMAGEVIEW:
      if (view != null) {
	FWImageView imageView = new FWImageView(frame, childInternalId);
	String imgFile = getTextValueAsString();
	if (!imgFile.isEmpty()) {
	  imageView.addImageUrl(imgFile, width, height);
	}
	frame.addToViewList(imageView);
	view.addChild(imageView);
      }
      break;
      
    case CREATE_TOAST: {
      Toast toast = Toast.makeText(frame, getTextValueAsString(), getValue() != 0 ? getValue() : Toast.LENGTH_SHORT);
      toast.show();
    }
      break;
      
    case CREATE_NOTIFICATION: {
      FWNotification notif = new FWNotification(frame, getTextValueAsString(), getTextValue2AsString());
      notif.setId(childInternalId);
      frame.addToViewList(notif);
      break;
    }
    
    case ADD_OPTION:
      if (view != null) {
	view.addOption(getValue(), getTextValueAsString());
      }
      break;
      
    case ADD_COLUMN:
      if (view != null) {
	view.addColumn(getTextValueAsString(), getValue());
      }
      break;
      
    case ADD_SHEET:
      if (view != null) {
	System.out.println("add_sheet: " + getTextValueAsString() + " " + rowNumber + " " + columnNumber + " " + sheet);
	view.setValue(getTextValueAsString());
      }
      break;
      
    case CREATE_APPLICATION: {
      frame.setAppId(getChildInternalId());
      if (isSet(FLAG_USE_PURCHASES_API)) {
	System.out.println("Initializing purchaseHelper");
	frame.initializePurchaseHelper(getTextValue2AsString(), new IabHelper.OnIabSetupFinishedListener() {

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
    }
      break;
    case SET_INT_VALUE:
      if (view != null) {
	view.setValue(getValue());
      }
      break;
    case SET_TEXT_VALUE:
      if (view != null) {
	view.setValue(getTextValueAsString());
      }
      break;
    case SET_TEXT_DATA:
      if (view != null) {
	System.out.println("set_text_data: " + getTextValueAsString() + " " + rowNumber + " " + columnNumber + " " + sheet);
	view.addData(getTextValueAsString(), rowNumber, columnNumber, sheet);
      }
      break;
    case SET_VISIBILITY:
      if (view != null) {
	view.setViewVisibility(value != 0);
      }
      break;    
    case SET_STYLE:
      if (view != null) {
	view.setStyle(Selector.values()[value], getTextValueAsString(), getTextValue2AsString());
      }
      break;
    case SET_ERROR:
      if (view != null) {
	view.setError(value != 0, getTextValueAsString());
      }
      break;
    case ADD_IMAGE_URL:
      if (view != null) {
	view.addImageUrl(getTextValueAsString(), width, height);
      }
      break;
    case LAUNCH_BROWSER:
      frame.launchBrowser(getTextValueAsString());
      break;
    case CREATE_DIALOG:
      FWDialog dialog = new FWDialog(frame, childInternalId);
      frame.addToViewList(dialog);
      break;
    case CREATE_ACTION_SHEET:
      if (view instanceof View) {
	FWActionSheet sheet = new FWActionSheet(frame, (View)view, childInternalId);
	frame.addToViewList(sheet);
      }
      break;
    case CREATE_ACTIONBAR: {
      FWActionBar ab = new FWActionBar(frame, childInternalId);
      frame.actionBar = ab;
      frame.addToViewList(ab);
      break;
    }
    case CREATE_NAVIGATIONBAR: {
      FWFrameLayout bar = new FWFrameLayout(frame, getChildInternalId());      
      view.addChild(bar);
      frame.addToViewList(bar);
      break;
    }
    case CREATE_PROGRESSBAR:
      if (view != null) {
      FWProgressBar bar = new FWProgressBar(frame);
      bar.setId(childInternalId);
      view.addChild(bar);
      frame.addToViewList(bar);
      }
      break;
    case FLUSH_VIEW:
      if (view != null) {
	view.flush();
      }
      break;
    case QUIT_APP:
      // TODO
      frame.finish();
      break;
    case UPDATE_PREFERENCE: // Now stores String value to string key
      System.out.println("UPDATE_PREFERENCE: " + getTextValueAsString() + " " + getTextValue2AsString());
      frame.getPreferencesEditor().putString(getTextValueAsString(), getTextValue2AsString());
      break;
    case DELETE_PREFERENCE:
      frame.getPreferencesEditor().remove(getTextValueAsString());
      break;
    case COMMIT_PREFERENCES:
      frame.getPreferencesEditor().apply();
      break;
    case DELETE_ELEMENT:
      if (view != null) {
	deleteElement(view);
      }
      break;
    case REMOVE_CHILD:
      if (view != null) {
	removeChild(view, childInternalId);
      }
      break;
    case REORDER_CHILD:
      if (view != null) {
	reorderChild(view, childInternalId, getValue());
      }
      break;
    case BUY_PRODUCT:
      try {
	launchPurchase(getTextValueAsString());
      } catch (IabAsyncInProgressException e) {
	e.printStackTrace();
	System.out.println("Error on launchPurchase with message: " + e.getMessage());
      }
      break;
    case RESHAPE_SHEET:
      if (view != null) {
	view.reshape(sheet, value);
      }
      break;
    case RESHAPE_TABLE:
      if (view != null) {
	System.out.println("reshape table: " + value);
	view.reshape(value);
      }
      break;
    case SHARE_LINK: {
      ShareCompat.IntentBuilder.from(frame)
      .setType("text/plain")
      .setChooserTitle("Share link")
      .setText(getTextValueAsString())
      .startChooser();
      break;
    }
    default:
      System.out.println("Command couldn't be handled "  + command + " id: " + internalId + " Child id: " + getChildInternalId());
      break;
    }
  }
  
  private void createActionSheet(View anchor) {
    PopupMenu menu = new PopupMenu(frame, anchor);
    menu.setOnMenuItemClickListener(new OnMenuItemClickListener(){
      @Override
      public boolean onMenuItemClick(MenuItem item) {
	
	return false;
      }
    });
    menuList.add(menu);
    menu.show();
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
    frame.addToViewList(table);
    return table;
  }
  
  private FWSwitch createSwitch() {
    String s1 = getTextValueAsString();
    String s2 = getTextValue2AsString();
    FWSwitch click = new FWSwitch(frame);
    click.setId(childInternalId);
    if (!s1.isEmpty()) {
      click.setTextOn(s1);
    }
    if (!s2.isEmpty()) {
      click.setTextOff(s2);
    }
    click.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
      @Override
      public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
	frame.sendNativeValueEvent(buttonView.getId(), isChecked ? 1 : 0, 0);
      }
    });
    frame.addToViewList(click);
    return click;
  }
  
  private void deleteElement(NativeCommandHandler handler) {
    frame.removeViewFromList(handler.getElementId());    

    if (handler instanceof View) {
      View view = (View)handler;
      ViewParent parent = view.getParent();
      if (parent instanceof ViewGroup) {
	((ViewGroup) parent).removeView(view);
      }
    }
  }
  
  private void removeChild(NativeCommandHandler parent, int childId) {
    if (parent instanceof ViewGroup) {
      ViewGroup group = (ViewGroup) parent;
      int childCount = group.getChildCount();
      for (int i = 0; i < childCount; i++) {
	View view = group.getChildAt(i);
	if (view.getId() == childId) {
	  ((ViewGroup) parent).removeViewAt(i);
	  break;
	}
      }
    } else {
      System.out.println("Deletion parent was not an instance of ViewGroup");
    }
  }
  
  private void reorderChild(NativeCommandHandler parent, int childId, int newPosition) {
    if (parent instanceof ViewGroup) {
      ViewGroup group = (ViewGroup) parent;
      int childCount = group.getChildCount();
      for (int i = 0; i < childCount; i++) {
	View view = group.getChildAt(i);
	if (view.getId() == childId && i != newPosition) {
	  ((ViewGroup) parent).removeViewAt(i);
	  if (newPosition < group.getChildCount()) {
	    ((ViewGroup) parent).addView(view, newPosition);
	  } else {	    
	    ((ViewGroup) parent).addView(view);
	  }
	  break;
	}
      }
    } else {
      System.out.println("Reordering parent was not an instance of ViewGroup");
    }
  }
    
  private FWLayout createLinearLayout(int direction) {
    FWLayout layout = new FWLayout(frame, getChildInternalId());
    frame.addToViewList(layout);
    if (direction == 2) {
      layout.setOrientation(LinearLayout.HORIZONTAL);
    } else {
      layout.setOrientation(LinearLayout.VERTICAL);
    }
    return layout;
  }

  private FWButton createButton() {
    FWButton button = new FWButton(frame);
    button.setId(getChildInternalId());
    button.setText(getTextValueAsString());
    LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
//    params.weight = 1;
//    params.gravity = Gravity.BOTTOM;
    button.setLayoutParams(params);
    frame.addToViewList(button);
    return button;
  }
  
  private FWEditText createEditText() {
    
    final FWEditText editText = new FWEditText(frame);
    editText.setId(getChildInternalId());
    editText.setText(getTextValueAsString());
//    editText.setMinWidth(80);
    editText.setSingleLine();
    editText.setImeOptions(EditorInfo.IME_ACTION_DONE);
    
    if (isSet(FLAG_PASSWORD) && isSet(FLAG_NUMERIC)) {
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
	frame.sendNativeValueEvent(getChildInternalId(), b);
      }
      public void beforeTextChanged(CharSequence s, int start, int count, int after) {}
      public void onTextChanged(CharSequence s, int start, int before, int count) {}
   });
    frame.addToViewList(editText);
    return editText;
  }
  

  private FWEditText createBigEditText() {
    final FWEditText editText = new FWEditText(frame);
    LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
    editText.setMinLines(4);
    editText.setLayoutParams(params);
    editText.setId(getChildInternalId());
    editText.setText(getTextValueAsString());
    editText.setVerticalScrollBarEnabled(true);
    editText.setMovementMethod(new ScrollingMovementMethod());
    editText.addDelayedChangeListener(getChildInternalId());
    frame.addToViewList(editText);
    return editText;
  }

  private FWPicker createSpinner(){
    FWPicker picker = new FWPicker(frame);
    picker.setId(getChildInternalId());
    frame.addToViewList(picker);
    
    return picker;
  }
  
  private FWCheckBox createCheckBox() {
    String s1 = getTextValueAsString();
    FWCheckBox checkBox = new FWCheckBox(frame);
    checkBox.setPadding(0, 0, 10, 0);
    checkBox.setId(childInternalId);
    if (!s1.isEmpty()) {
      checkBox.setText(s1);
    }
    checkBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
      @Override
      public void onCheckedChanged(CompoundButton box, boolean isChecked) {
	frame.sendNativeValueEvent(childInternalId, isChecked ? 1 : 0, 0);
      }
    });
    return checkBox;
  }
  
  private void launchPurchase(final String productId) throws IabAsyncInProgressException {
    // Sku = product id from google account
    frame.getPurchaseHelper().launchPurchaseFlow(frame, productId, IabHelper.ITEM_TYPE_INAPP, null, 1, new IabHelper.OnIabPurchaseFinishedListener() {

      @Override
      public void onIabPurchaseFinished(IabResult result, Purchase info) {
	if (result.isSuccess()) {
	  System.out.println("Purchase of product id " + productId + " completed");
	  frame.onPurchaseEvent(frame.getAppId(), info.getSku(), true, info.getPurchaseTime() / 1000.0);
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
      frame.onPurchaseEvent(frame.getAppId(), purchase.getSku(), false, purchase.getPurchaseTime() / 1000.0);
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
  public CommandType getCommand() {
    return command;
  }
  public int getValue() {
    return value;
  }
}
