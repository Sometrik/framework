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
//    ViewPager.LayoutParams params = new ViewPager.LayoutParams();
//    params.width = ViewPager.LayoutParams.MATCH_PARENT;
//    params.height = ViewPager.LayoutParams.WRAP_CONTENT;
    setAdapter(adapter);
//    setLayoutParams(params);
    setOnPageChangeListener(new ViewPager.SimpleOnPageChangeListener() {
      @Override
      public void onPageSelected(int position) {
	System.out.println("Pager change page " + getElementId());
	frame.sendNativeValueEvent(getElementId(), position, 0);
      }
    });

  }
  
  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub
    
  }
  @Override
  public void addChild(View view) {
    adapter.addToList(view);
	adapter.notifyDataSetChanged();
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
  
//  private class FWPagerAdapter extends FragmentPagerAdapter  {
//    
//    private ArrayList<View> viewList;
//    
//    public FWPagerAdapter(android.support.v4.app.FragmentManager fm) {
//      super(fm);
//      viewList = new ArrayList<View>();
//    }
//
//    @Override
//    public Fragment getItem(int position) {
//      View view = viewList.get(position);
//      FWObjectFragment fragment = new FWObjectFragment(view);
//      return fragment;
//    }
//
//    @Override
//    public int getCount() {
//      return viewList.size();
//    }
//  }
//  
//  private class FWObjectFragment extends Fragment {
//    public final String ARG_OBJECT = "object";
//    
//    private View rootView;
//    public FWObjectFragment(View rootView) {
//      this.rootView = rootView;
//    }
//
//    @Override
//    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
////        // The last two arguments ensure LayoutParams are inflated
////        // properly.
////        View rootView = inflater.inflate(
////                R.layout.fragment_collection_object, container, false);
////        Bundle args = getArguments();
////        ((TextView) rootView.findViewById(android.R.id.text1)).setText(
////                Integer.toString(args.getInt(ARG_OBJECT)));
//        return rootView;
//    }
//}

}
