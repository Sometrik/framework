package com.sometrik.framework;

import java.util.ArrayList;
import java.util.Iterator;

import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.BitmapDrawable;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.LinearLayout;

public class FWSimpleList extends LinearLayout implements NativeCommandHandler {

  private FrameWork frame;
  private FrameLayout.LayoutParams defaultListParams;
  private ArrayList<Sheet> sheets = new ArrayList<Sheet>();
  private ArrayList<String> sheetMemory = new ArrayList<String>();
  private int tableSize = 1;
  
    public FWSimpleList(FrameWork frame) {
      super(frame);
      this.frame = frame;
      defaultListParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.MATCH_PARENT, FrameLayout.LayoutParams.MATCH_PARENT);
      defaultListParams.setMargins(0, 10, 0, 10);
      setOrientation(LinearLayout.VERTICAL);
      setDividerDrawable(frame.getResources().getDrawable(android.R.drawable.divider_horizontal_bright));

      this.setBackgroundColor(Color.rgb(255, 255, 255));
    }
    
    @Override
    public void onScreenOrientationChange(boolean isLandscape) {
      // TODO Auto-generated method stub
      
    }

    @Override
    public void addChild(View view) {
      view.setLayoutParams(defaultListParams);
      Sheet sheet = new Sheet((ViewGroup)view);
      sheets.add(sheet);
      if (sheetMemory.size() >= sheets.size()) {
        Log.d("adapter", "setting sheetMemory");
        sheet.name.setText(sheetMemory.get(sheets.size() - 1));
      } else {
        sheet.name.setText("TITLE");
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
//      System.out.println("setting sheet " + sheetPosition + " " + title + " " + sheets.size());
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
    public void setImage(byte[] bytes) {
      // TODO Auto-generated method stub
      
    }

    @Override
    public void setValue(int v) {
      // TODO Auto-generated method stub
      
    }

    @Override
    public void reshape(int value, int size) {
      if (value < sheets.size()) {
        ViewGroup layout = sheets.get(value).layout;

        //add one because title layout won't be resized
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
//      invalidate();
    }

    @Override
    public void reshape(int size) {
      this.tableSize = size;
      ArrayList<Sheet> enabledSheets = getEnabledSheets();
      if (size == enabledSheets.size()) {
        return;
      }
      if (size < enabledSheets.size()) {
        for (int i = size; i < enabledSheets.size(); i++) {
  	System.out.println("removing sheet " + i);
  	sheets.get(i).disable();
//  	sheets.get(i).layout.setVisibility(GONE);
//  	removeView(sheets.get(i).layout);
        }
      }
      if (size > enabledSheets.size()) {
        ArrayList<Sheet> disabledSheets = getDisabledSheets();
        int difference = size - enabledSheets.size();
        for (int i = 0; i < difference; i++) {
  	Sheet disabledSheet = disabledSheets.get(i);
  	disabledSheet.enable();
//  	disabledSheet.disabled = false;
//  	sheets.get(i).layout.setVisibility(VISIBLE);
//  	addView(disabledSheet.layout);
  	
        }
      }
//      updateLayout();
    invalidate();
    }
    
//    private void updateLayout() {
//      this.removeAllViews();
//      
//      ArrayList<Sheet> enabledSheets = getEnabledSheets();
//      for (Sheet sheet : enabledSheets) {
//	addView(sheet.layout);
//      }
//      invalidate();
//    }

    @Override
    public void setViewEnabled(Boolean enabled) {
      // TODO Auto-generated method stub
      
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
  public void setStyle(String key, String value) {
    if (key.equals("divider")) {
      if (value.equals("middle")) {
	this.setShowDividers(LinearLayout.SHOW_DIVIDER_MIDDLE);
      } else if (value.equals("end")) {
	this.setShowDividers(LinearLayout.SHOW_DIVIDER_END);
      } else if (value.equals("beginning")) {
	this.setShowDividers(LinearLayout.SHOW_DIVIDER_BEGINNING);
      }
    } else if (key.equals("color")) {
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
      private FWTextView name;
      private int restrictedSize = 1;
      private boolean disabled = false;
      
      private void disable() {
	disabled = true;
//	layout.setVisibility(GONE);
      }
      private void enable() {
	disabled = false;
//	layout.setVisibility(VISIBLE);
//	for (int i = 0; i < layout.getChildCount(); i++) {
//	  layout.getChildAt(i).setVisibility(VISIBLE);;
//	}
      }
      private Sheet(ViewGroup view) {
        name = new FWTextView(frame);
//        name.setBackgroundDrawable(frame.getResources().getDrawable(android.R.drawable.dialog_holo_light_frame));
        name.setTextSize(24);
        name.setTypeface(null, Typeface.BOLD);
        name.setLayoutParams(defaultListParams);
        

	final float scale = getContext().getResources().getDisplayMetrics().density;
	int pixels = (int) (41 * scale + 0.5f);
	
        name.setHeight(pixels);
//        name.setBackgroundDrawable(backgroundColor);
        layout = view;
        view.addView(name);
      }
    }

  }

