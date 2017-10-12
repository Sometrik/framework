package com.sometrik.framework;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Iterator;

import com.sometrik.framework.NativeCommand.Selector;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.BitmapDrawable;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.ImageView.ScaleType;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

public class FWSimpleList extends LinearLayout implements NativeCommandHandler {

  private FrameWork frame;
  private LinearLayout.LayoutParams defaultListParams;
  private ArrayList<Sheet> sheets = new ArrayList<Sheet>();
  private ArrayList<String> sheetMemory = new ArrayList<String>();
  private String sheeticon_right = "";
  
    public FWSimpleList(FrameWork frame) {
      super(frame);
      this.frame = frame;
      defaultListParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
      // defaultListParams.setMargins(0, 10, 0, 10);
      setOrientation(LinearLayout.VERTICAL);
//      setDividerDrawable(frame.getResources().getDrawable(android.R.drawable.divider_horizontal_bright));
    }
    
    @Override
    public void onScreenOrientationChange(boolean isLandscape) {
      // TODO Auto-generated method stub
      
    }

    @Override
    public void addChild(final View view) {
      view.setLayoutParams(defaultListParams);
      final int sheetNumber = sheets.size();
      view.setOnClickListener(new OnClickListener() {
	  @Override
	  public void onClick(View v) {
	    System.out.println("Click java sheet: " + sheetNumber);
	    frame.intChangedEvent(System.currentTimeMillis() / 1000.0, view.getId(), 1, sheetNumber);
	  }
      });
      Sheet sheet = new Sheet((ViewGroup)view);
      sheets.add(sheet);
      if (sheetMemory.size() >= sheets.size()) {
        Log.d("adapter", "setting sheetMemory");
        sheet.name.setText(sheetMemory.get(sheets.size() - 1));
      } else {
        sheet.name.setText("TITLE");
      }
      
      if (sheeticon_right != "") {
	System.out.println("setting sheeticon_right from memory");
	sheet.setRightIcon(sheeticon_right);
      } else {
	System.out.println("no sheeticon_right in memory");
      }

      addView(sheet.layout);
    }
    
    private ArrayList<Sheet> getEnabledSheets() {
      ArrayList<Sheet> enabledSheets = new ArrayList<Sheet>();
      for (Sheet sheet : sheets) {
        if (!sheet.disabled) {
  	enabledSheets.add(sheet);
        }
      }
      return enabledSheets;
    }
    
    private ArrayList<Sheet> getDisabledSheets(){
      ArrayList<Sheet> disabledSheets = new ArrayList<Sheet>();
      for (Sheet sheet : sheets) {
        if (sheet.disabled) {
  	disabledSheets.add(sheet);
        }
      }
      return disabledSheets;
    }

    @Override
    public void addOption(int optionId, String text) {
      if (optionId < sheets.size()) {
        sheets.get(optionId).name.setText(text);;
      }
      sheetMemory.add(optionId, text);
    }

    @Override
    public void addColumn(String text, int columnType) {

    }

    @Override
    public void addData(String text, int row, int column, int sheet) {
      // TODO Auto-generated method stub
      
    }

    @Override
    public void setValue(String v) {

    }

    @Override
    public void setImage(byte[] bytes, int width, int height, Config config) {
      // TODO Auto-generated method stub
      
    }

    @Override
    public void setValue(int v) {
//    if (v == 1) {
//      frame.setCurrentView(this, true);
//    } else if (v == 2) {
//      frame.setCurrentView(this, false);
//    }
  }

    @Override
    public void reshape(int value, int size) {
    if (value < sheets.size()) {
      ViewGroup layout = sheets.get(value).layout;

      // add one because title layout won't be resized
      size++;
      if (layout.getChildCount() == size) {
	return;
      }
      ArrayList<View> viewsToBeRemoved = new ArrayList<View>();
      for (int i = size; i < layout.getChildCount(); i++) {
	View view = layout.getChildAt(i);
	viewsToBeRemoved.add(view);
      }

      Iterator<View> i = viewsToBeRemoved.iterator();
      while (i.hasNext()) {
	View v = i.next();
	if (v instanceof FWImageView) {
	  Bitmap drawable = ((BitmapDrawable) v.getBackground()).getBitmap();
	  if (drawable != null) {
	    drawable.recycle();
	  }
	}
	frame.removeViewFromList(v.getId());
	layout.removeView(v);
      }
    } else {

      ViewGroup layout = sheets.get(value).layout;
      for (int i = 0; i < layout.getChildCount(); i++) {

      }
      return;
    }
  }

    @Override
  public void reshape(int size) {
    System.out.println("reshape_table simple " + size);
    ArrayList<Sheet> enabledSheets = getEnabledSheets();
    System.out.println("reshape_table simple " + size + " " + enabledSheets.size());
    if (size == enabledSheets.size()) {
      return;
    }
    if (size < enabledSheets.size()) {
      for (int i = size; i < enabledSheets.size(); i++) {
	System.out.println("removing sheet " + i);
	sheets.get(i).disable();
      }
    }
    if (size > enabledSheets.size()) {
      ArrayList<Sheet> disabledSheets = getDisabledSheets();
      System.out.println("reshape_table simple disabled: " + disabledSheets.size());
      int difference = size - enabledSheets.size();
      for (int i = 0; i < difference; i++) {
	Sheet disabledSheet = disabledSheets.get(i);
	disabledSheet.enable();
      }
    }
    invalidate();
  }    

    @Override
    public void setViewVisibility(boolean visible) {
      if (visible) {
	this.setVisibility(VISIBLE);
      } else {
	this.setVisibility(GONE);
      }
    }

  @Override
  public void setStyle(Selector selector, String key, String value) {
    System.out.println("SimpleListView style " + key + " " + value);
    if (key.equals("divider")) {
      if (value.equals("middle")) {
	// this.setShowDividers(LinearLayout.SHOW_DIVIDER_MIDDLE);
      } else if (value.equals("end")) {
	// this.setShowDividers(LinearLayout.SHOW_DIVIDER_END);
      } else if (value.equals("beginning")) {
	// this.setShowDividers(LinearLayout.SHOW_DIVIDER_BEGINNING);
      }
    } else if (key.equals("background-color")) {
      this.setBackgroundColor(Color.parseColor(value));
    } else if (key.equals("width")) {
      FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) getLayoutParams();
      if (value.equals("wrap-content")) {
	params.width = LinearLayout.LayoutParams.WRAP_CONTENT;
      } else if (value.equals("match-parent")) {
	params.width = LinearLayout.LayoutParams.MATCH_PARENT;
      } else {
	final float scale = getContext().getResources().getDisplayMetrics().density;
	int pixels = (int) (Integer.parseInt(value) * scale + 0.5f);
	params.width = pixels;
      }
      setLayoutParams(params);
    } else if (key.equals("height")) {
      FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) getLayoutParams();
      if (value.equals("wrap-content")) {
	params.height = LinearLayout.LayoutParams.WRAP_CONTENT;
      } else if (value.equals("match-parent")) {
	params.height = LinearLayout.LayoutParams.MATCH_PARENT;
      } else {

	final float scale = getContext().getResources().getDisplayMetrics().density;
	int pixels = (int) (Integer.parseInt(value) * scale + 0.5f);
	params.height = pixels;
      }
      setLayoutParams(params);
    } else if (key.equals("sheeticon-right")) {
      System.out.println("setting right icon");
	sheeticon_right = value;
      for (Sheet sheet : sheets) {
	sheet.setRightIcon(value);
      }
    }
  }

    @Override
    public void setError(boolean hasError, String errorText) {
      // TODO Auto-generated method stub
      
    }

    @Override
    public void clear() {
      reshape(0);
    }

    @Override
    public void flush() {
      // TODO Auto-generated method stub
      
    }

    @Override
    public int getElementId() {
      return getId();
    }
    
    private class Sheet  {
      private ViewGroup layout;
      private LinearLayout sheetLayout;
      private FWTextView name;
      private int restrictedSize = 1;
      private boolean disabled = false;
      private ImageView rightIconView;
      
      private void disable() {
	disabled = true;
	layout.setVisibility(GONE);
      }
      private void enable() {
	disabled = false;
	layout.setVisibility(VISIBLE);
      }
      private Sheet(ViewGroup view) {
	sheetLayout = new LinearLayout(frame);
	sheetLayout.setLayoutParams(new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
	sheetLayout.setOrientation(LinearLayout.HORIZONTAL);
	sheetLayout.setFocusable(false);
	
        name = new FWTextView(frame);
	LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
	params.weight = 1;
        name.setTextSize(24);
        name.setTypeface(null, Typeface.BOLD);
        name.setLayoutParams(params);
	final float scale = getContext().getResources().getDisplayMetrics().density;
	int pixels = (int) (41 * scale + 0.5f);
	
        name.setHeight(pixels);
        sheetLayout.addView(name);
        layout = view;
        view.addView(sheetLayout);
    }
      

    private void setRightIconText(RelativeLayout layout, ImageView iconView, String text) {
      FWTextView textView = new FWTextView(frame);
      RelativeLayout.LayoutParams listParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT);
//      listParams.addRule(RelativeLayout.LEFT_OF, iconView.getId());
      textView.setLayoutParams(listParams);
      textView.setTextSize(13);
      textView.setId(845848);
      textView.setText("MORE");
      textView.setTextColor(Color.parseColor("#c1272d"));

	final float scale = getContext().getResources().getDisplayMetrics().density;
	int topPadding = (int) (10 * scale + 0.5f);
	int rightPadding = (int) (38 * scale + 0.5f);
	
      textView.setGravity(Gravity.RIGHT);
      textView.setPadding(0, topPadding, rightPadding, 0);
      layout.addView(textView);
    }

    private void setRightIcon(String assetFilename) {

      System.out.println("setting right icon");
      if (rightIconView == null) {
	rightIconView = new ImageView(frame);
//	LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
	rightIconView.setScaleType(ScaleType.FIT_END);
	rightIconView.setId(99944);

	RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT);
	rightIconView.setLayoutParams(params);

	RelativeLayout iconLayout = new RelativeLayout(frame);
	LinearLayout.LayoutParams parentParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
	parentParams.weight = 1;
	iconLayout.setLayoutParams(parentParams);
	iconLayout.setGravity(Gravity.RIGHT);
	setRightIconText(iconLayout, rightIconView, "");
	
	iconLayout.addView(rightIconView);
	sheetLayout.addView(iconLayout);
	
      }
      try {
	AssetManager mgr = frame.getAssets();
	InputStream stream = mgr.open(assetFilename);
	if (stream != null) {
	  Bitmap bitmap = BitmapFactory.decodeStream(stream);
	  rightIconView.setImageBitmap(bitmap);
	  bitmap = null;
	}
	stream.close();
      } catch (IOException e) {
	e.printStackTrace();
      }
    }
  }

  }

