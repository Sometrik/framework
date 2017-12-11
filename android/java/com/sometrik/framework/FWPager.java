package com.sometrik.framework;

import java.util.ArrayList;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap;
import android.support.v4.view.PagerAdapter;
import android.support.v4.view.ViewPager;
import android.view.View;
import android.view.ViewGroup;

public class FWPager extends ViewPager implements NativeCommandHandler {

  private FrameWork frame;
  private FWPagerAdapter adapter;
  ViewStyleManager normalStyle, activeStyle, currentStyle, linkStyle;
  
  public FWPager(final FrameWork frame) {
    super(frame);
    this.frame = frame;

    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
    this.linkStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
    
    adapter = new FWPagerAdapter();
    setAdapter(adapter);
    setOnPageChangeListener(new ViewPager.SimpleOnPageChangeListener() {
      @Override
      public void onPageSelected(int position) {
	System.out.println("Pager change page " + getElementId());
	frame.sendNativeValueEvent(getElementId(), position, 0);
      }
    });

  }
  
  @Override
  public void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
    super.onMeasure(widthMeasureSpec, heightMeasureSpec);
    View currentView = adapter.currentItem;

    if (currentView != null) {
        currentView.measure(widthMeasureSpec, heightMeasureSpec);
        super.onMeasure(widthMeasureSpec, View.MeasureSpec.makeMeasureSpec(currentView.getMeasuredHeight(), View.MeasureSpec.EXACTLY));
        return;
    }
    super.onMeasure(widthMeasureSpec, heightMeasureSpec);
  }
  
  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub
    
  }
  @Override
  public void addChild(View view) {
    adapter.addToList(view);
	adapter.notifyDataSetChanged();
	System.out.println("Pager add page");
//    addView(view);s
  }
  @Override
  public void addOption(int optionId, String text) {
    // TODO Auto-generated method stub
    
  }
  @Override
  public void addColumn(String text, int columnType) {
    // TODO Auto-generated method stub
    
  }
  @Override
  public void addData(String text, int row, int column, int sheet) {
    // TODO Auto-generated method stub
    
  }
  @Override
  public void setValue(String v) {
    // TODO Auto-generated method stub
    
  }
  @Override
  public void setBitmap(Bitmap bitmap) {
    // TODO Auto-generated method stub
    
  }
  @Override
  public void addImageUrl(String url, int width, int height) {
    // TODO Auto-generated method stub
    
  }
  @Override
  public void setValue(int v) {
    setCurrentItem(v);
  }
  @Override
  public void reshape(int value, int size) {
    // TODO Auto-generated method stub
    
  }
  @Override
  public void reshape(int size) {
    // TODO Auto-generated method stub
    
  }
  @Override
  public void setViewVisibility(boolean visible) {
    // TODO Auto-generated method stub
    
  }
  @Override
  public void setStyle(Selector selector, String key, String value) {
    
    if (key.equals("page-margin")) {
      this.setPageMargin(Integer.parseInt(value));
      return;
    }
    
    if (selector == Selector.NORMAL) {
      normalStyle.setStyle(key, value);
    } else if (selector == Selector.ACTIVE) {
      activeStyle.setStyle(key, value);      
    } else if (selector == Selector.LINK) {
      linkStyle.setStyle(key, value);
    }
  }
  @Override
  public void applyStyles() {
    currentStyle.apply(this);  
    linkStyle.applyLinkColor(this);
  }
  @Override
  public void setError(boolean hasError, String errorText) {
    // TODO Auto-generated method stub
    
  }
  @Override
  public void clear() {
    // TODO Auto-generated method stub
    
  }
  @Override
  public void flush() {
    // TODO Auto-generated method stub
    
  }
  @Override
  public void deinitialize() {
    // TODO Auto-generated method stub
    
  }
  @Override
  public int getElementId() {
    return getId();
  }
  
  private class FWPagerAdapter extends PagerAdapter {

  private ArrayList<View> viewList;
  private View currentItem;
  
    public FWPagerAdapter() {
      viewList = new ArrayList<View>();
    }
    
    public void addToList(View view) {
      viewList.add(view);
      notifyDataSetChanged();
    }

    @Override
    public int getCount() {
      return viewList.size();
    }
    

    @Override 
    public void setPrimaryItem(ViewGroup container, int position, Object object ) {
      super.setPrimaryItem(container, position, object);
      currentItem = (View)object;
    }

    @Override
    public void startUpdate(ViewGroup container) {
      super.startUpdate(container);
    }
    
    @Override
    public Object instantiateItem(ViewGroup container, int position) {
      container.addView(viewList.get(position));
      return viewList.get(position);
    }
    
    @Override
    public void destroyItem(ViewGroup collection, int position, Object view) {
        collection.removeView((View) view);
    }

    @Override
    public boolean isViewFromObject(View view, Object object) {
      return view == object;
    }
  }
}
