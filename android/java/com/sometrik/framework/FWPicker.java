package com.sometrik.framework;

import java.util.ArrayList;

import com.sometrik.framework.NativeCommand.Selector;

import android.content.Context;
import android.graphics.Bitmap;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

public class FWPicker extends Spinner implements NativeCommandHandler { 
  private FrameWork frame;
  private CustomPickerAdapter adapter;
  private boolean ignoreNextSelection = true;
  ViewStyleManager normalStyle, activeStyle, currentStyle;

  public FWPicker(FrameWork frameWork) {
    super(frameWork);
    this.frame = frameWork;
    adapter = new CustomPickerAdapter(frame, android.R.layout.simple_spinner_item);
    setAdapter(adapter);
    
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
    
    setOnItemSelectedListener(new OnItemSelectedListener() {

      @Override
      public void onItemSelected(AdapterView<?> view, View arg1, int position, long itemId) {
	System.out.println("FWPicker onItemSelected " + position);
	if (ignoreNextSelection) {
	  ignoreNextSelection = false;
	  System.out.println("FWPicker not sending event");
	  return;
	} else {
	  frame.sendNativeValueEvent(getId(), position, 0);
	}
      }

      @Override
      public void onNothingSelected(AdapterView<?> arg0) {
	  System.out.println("FWPicker nothing selected");
      }
    });
  }
  
  @Override
  public boolean performClick() {
    adapter.notifyDataSetChanged();
    return super.performClick();
  }

  @Override
  public int getElementId() {
    return getId();
  }

  @Override
  public void addChild(View view) {
    System.out.println("FWPicker addChild");
    if (view instanceof FWLayout) {
      System.out.println("FWPicker is FWLayout " + ((FWLayout)view).getChildCount());
    }
    adapter.add(view);
    adapter.viewList.add(view);
    adapter.notifyDataSetChanged();
  }

  @Override
  public void addOption(int optionId, String text) {

  }

  @Override
  public void setValue(String v) {

  }

  @Override
  public void setValue(int v) {
    ignoreNextSelection = true;
    setSelection(v);
  }

  @Override
  public void setStyle(Selector selector, String key, String value) {
    if (selector == Selector.NORMAL) {
      normalStyle.setStyle(key, value);
    } else if (selector == Selector.ACTIVE) {
      activeStyle.setStyle(key, value);
    }
  }
  
  @Override
  public void applyStyles() {
    currentStyle.apply(this);  
    }

  @Override
  public void setError(boolean hasError, String errorText) { }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    System.out.println("Picker couldn't handle command");
  }
  
  @Override
  public void setViewVisibility(boolean visibility) {
    if (visibility){
      this.setVisibility(VISIBLE);
    } else {
      this.setVisibility(INVISIBLE);
    }
  }

  @Override
  public void clear() {
    System.out.println("FWPicker clear");
    adapter.viewList.clear();
    adapter.clear();
    adapter.notifyDataSetChanged();
  }

  @Override
  public void flush() {
  }

  @Override
  public void addColumn(String text, int columnType) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void reshape(int value, int size) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setBitmap(Bitmap bitmap) { }

  @Override
  public void reshape(int size) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void deinitialize() {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addImageUrl(String url, int width, int height) {
    // TODO Auto-generated method stub
    
  }
  
  private class CustomPickerAdapter extends ArrayAdapter<View> {

    private ArrayList<View> viewList;
    public CustomPickerAdapter(Context context, int resource) {
      super(context, resource);
      viewList = new ArrayList<View>();
    }
    
    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
      View view = new View(frame);
      view = viewList.get(position);
      return view;
//      return getDropDownView(position, convertView, parent);
//	System.out.println("FWPicker getView " + position);
//      convertView = viewList.get(position);
//      if (convertView instanceof FWLayout) {
//	System.out.println("FWPicker is FWLayout in getView " + convertView.getId() + " "+ ((FWLayout) convertView).getChildCount());
//      }
//      return convertView;
    }
    
    @Override
    public int getViewTypeCount() {
      return 1;
    }
    
    @Override
    public int getCount() {
      return viewList.size();
    }
    
    @Override
    public View getDropDownView(int position, View convertView, ViewGroup parent) {
	System.out.println("FWPicker getDropDown " + position);
      View view = viewList.get(position);
      if (view instanceof FWLayout) {
	System.out.println("FWPicker is FWLayout in getDropDown " + view.getId() + " "+ ((FWLayout) view).getChildCount());
      }
      return view;
    }
    
  }
}
