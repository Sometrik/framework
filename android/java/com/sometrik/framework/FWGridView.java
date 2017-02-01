package com.sometrik.framework;

import java.util.ArrayList;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.GridView;
import android.widget.LinearLayout;

public class FWGridView extends GridView implements NativeCommandHandler {
  
  private FrameWork frame;
  private FWAdapter adapter;
  private ArrayList<View> preCreationViewList;
  private boolean adapterSetup = false;
  private ViewGroup.LayoutParams childParams;
  
  public FWGridView(FrameWork frame) {
    super(frame);
    this.frame = frame;
    GridView.LayoutParams params = new GridView.LayoutParams(GridView.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
    this.setLayoutParams(params);
    preCreationViewList = new ArrayList<View>();
    
  }
  
  public void setupAdapter(FWAdapter adapter){
    for (View view : preCreationViewList){
      adapter.addItem(view);
    }
  }

  @Override
  public void addChild(View view) {
    FWAdapter adapter = (FWAdapter) getAdapter();
    if (adapter == null){
      System.out.println("adapter null");
      preCreationViewList.add(view);
    } else {
      if (adapterSetup){
	setupAdapter(adapter);
	adapterSetup = true;
      }
      adapter.addItem(view);
      setAdapter(adapter);
    }
  }
  
  @Override
  public void addOption(int optionId, String text) {
    System.out.println("FWGridView couldn't handle command");
  }

  @Override
  public void setValue(String v) {
    System.out.println("FWGridView couldn't handle command");
  }

  @Override
  public void setValue(int v) {
    if (v == 1){
      frame.setCurrentView(this, true);
    } else if (v == 2){
      frame.setCurrentView(this, false);
    }
  }

  @Override
  public void setViewEnabled(Boolean enabled) {
    System.out.println("FWGridView couldn't handle command");
  }

  @Override
  public void setStyle(String key, String value) {
    System.out.println("FWGridView couldn't handle command");
  }

  @Override
  public void setError(boolean hasError, String errorText) {
    System.out.println("FWGridView couldn't handle command");
  }

  @Override
  public int getElementId() {
    return getId();
  }
  
  

}
