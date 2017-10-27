package com.sometrik.framework;

import java.util.ArrayList;

import com.sometrik.framework.NativeCommand.Selector;

import android.app.ActionBar;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.view.Gravity;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ImageButton;
import android.widget.ImageView.ScaleType;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class FWActionBar implements NativeCommandHandler {
  
  private int id;
  private ActionBar actionBar;
  private ArrayList<ActionBarItem> itemList;
  private FrameWork frame;
  private TextView titleView;
  private TextView subtitleView;
  private ImageButton drawerButton;
  private RelativeLayout mainLayout;
  private FWLayout alternativeButtonLayout;
  ViewStyleManager normalStyle, activeStyle, currentStyle;

  public FWActionBar(final FrameWork frame, String title, int id){
    this.frame = frame;
    actionBar = frame.getActionBar();
//    actionBar.setDisplayShowTitleEnabled(true);
//    actionBar.setTitle(title);

    actionBar.setDisplayShowHomeEnabled(false);
    actionBar.setDisplayHomeAsUpEnabled(false);
    actionBar.setDisplayUseLogoEnabled(false);
    actionBar.setDisplayShowTitleEnabled(false);
    actionBar.setDisplayShowCustomEnabled(true);
    
    mainLayout = new RelativeLayout(frame);
//    mainLayout.setOrientation(LinearLayout.HORIZONTAL);
    mainLayout.setBackgroundColor(Color.parseColor("#ffffff"));
    LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
    params.weight = 1;
    mainLayout.setLayoutParams(params);

    final float scale = frame.getResources().getDisplayMetrics().density;
    drawerButton = new ImageButton(frame);
    LinearLayout.LayoutParams drawerButtonParams = new LinearLayout.LayoutParams((int)(45 * scale), LayoutParams.MATCH_PARENT);
    drawerButton.setLayoutParams(drawerButtonParams);
    Bitmap bitmap = frame.bitmapCache.loadBitmap("icons_hamburger-menu.png");
    
    if (bitmap != null) {
      Drawable draw = new BitmapDrawable(bitmap);
      
      drawerButton.setImageDrawable(draw);
      drawerButton.setScaleType(ScaleType.FIT_CENTER);
      drawerButton.setBackgroundColor(Color.parseColor("#ffffff"));
      drawerButton.setOnClickListener(new OnClickListener() {
	@Override
	public void onClick(View v) {
	  System.out.println("drawerButton click");
	  NativeCommandHandler handler = frame.views.get(frame.getCurrentDrawerViewId());
	  System.out.println("handler: " + handler.getElementId());
	  if (handler instanceof View) {
	    if (((View) handler).getVisibility() == View.VISIBLE) {
	      handler.setViewVisibility(false);
	    } else {
	      handler.setViewVisibility(true);
	    }
	  }
	}
      });
      mainLayout.addView(drawerButton);
    }
    
    titleView = new TextView(frame);
    titleView.setText(title);
    titleView.setLayoutParams(params);
    titleView.setGravity(Gravity.CENTER);
    titleView.setTextSize(14);
    titleView.setTextColor(Color.parseColor("#5d6468"));
    

    subtitleView = new TextView(frame);
    subtitleView.setText("subtitle");
    subtitleView.setLayoutParams(params);
    subtitleView.setGravity(Gravity.CENTER);
    subtitleView.setVisibility(TextView.GONE);
    subtitleView.setTextSize(10);
    subtitleView.setTextColor(Color.parseColor("#b7c4cd"));
    

    LinearLayout titleHolderLayout = new LinearLayout(frame);
    titleHolderLayout.setOrientation(LinearLayout.VERTICAL);
    titleHolderLayout.setLayoutParams(params);
    
    titleHolderLayout.addView(titleView);
    titleHolderLayout.addView(subtitleView);
    
    int pixels = (int) (58 * scale + 0.5f);
//    int pixels = drawerButton.getWidth();
//    titleView.setPadding(titleView.getPaddingLeft(), titleView.getPaddingRight(), pixels, titleView.getPaddingBottom());
//    subtitleView.setPadding(subtitleView.getPaddingLeft(), subtitleView.getPaddingRight(), pixels, subtitleView.getPaddingBottom());
//    titleView.setGravity(Gravity.CENTER);
    mainLayout.addView(titleHolderLayout);
    
    
    actionBar.show();
    actionBar.setCustomView(mainLayout);
    
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
    
    itemList = new ArrayList<ActionBarItem>();
    this.id = id;
  }

  
  void changeButton(final View view) {
    
    clear();

    drawerButton.setVisibility(ImageButton.GONE);
    alternativeButtonLayout = new FWLayout(frame);
    LinearLayout.LayoutParams params2 = new LinearLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.MATCH_PARENT);
    alternativeButtonLayout.setLayoutParams(params2);
    alternativeButtonLayout.addView(view);
    alternativeButtonLayout.setGravity(Gravity.CENTER_HORIZONTAL);
    mainLayout.addView(alternativeButtonLayout, 0);

    alternativeButtonLayout.setOnClickListener(new OnClickListener() {
      @Override
      public void onClick(View arg0) {
	System.out.println("actionbar: view clicked " + view.getId());
	frame.intChangedEvent(view.getId(), 1, 1);
      }
    });
  }
  
  public void setTitle(String title) {
    titleView.setText(title);
  }
  
  public ArrayList<ActionBarItem> getItemList(){
    return null;
  }
  
  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addChild(View view) {
    System.out.println("Command couldn't be handled by FWActionBar");
    changeButton(view);
  }

  @Override
  public void addOption(int optionId, String text) {
    itemList.add(new ActionBarItem(optionId, text, "default"));
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    System.out.println("Command couldn't be handled by FWActionBar");
  }

  @Override
  public void setValue(String v) {
    setTitle(v);
  }

  @Override
  public void setValue(int v) {
    if (v == 0){
      actionBar.hide();
    } else if (v == 1){
      actionBar.show();
    }
  }

  @Override
  public void setViewVisibility(boolean visible) {
    if (!visible) {
      actionBar.hide();
    } else {
      actionBar.show();
    }
  }

  @Override
  public void setStyle(Selector selector, String key, String value) {
    if (selector == Selector.NORMAL) {
      normalStyle.setStyle(key, value);
      // if (normalStyle == currentStyle) normalStyle.apply(this);
    } else if (selector == Selector.ACTIVE) {
      activeStyle.setStyle(key, value);      
      // if (activeStyle == currentStyle) activeStyle.apply(this);
    }
    
    if (key.equals("subtitle")) {
      subtitleView.setText(value);
      if (value.isEmpty()) {
	subtitleView.setVisibility(TextView.GONE);
      } else if (subtitleView.getVisibility() == TextView.GONE) {
	subtitleView.setVisibility(TextView.VISIBLE);
      }
    } else if (key.equals("height")) {

      final float scale = frame.getResources().getDisplayMetrics().density;
      LinearLayout.LayoutParams params = (LayoutParams) mainLayout.getLayoutParams();
      if (value.equals("wrap-content")) {
	params.height = LinearLayout.LayoutParams.WRAP_CONTENT;
      } else if (value.equals("match-parent")) {
	params.height = LinearLayout.LayoutParams.MATCH_PARENT;
      } else {
	int pixels = (int) (Integer.parseInt(value) * scale + 0.5f);
	params.height = pixels;
      }
      mainLayout.setLayoutParams(params);
    }
  }

  @Override
  public void setError(boolean hasError, String errorText) {
    System.out.println("Command couldn't be handled by FWActionBar");
  }

  @Override
  public int getElementId() {
    return id;
  }

  
  public class ActionBarItem {

    int id;
    String name;
    String pictureAsset;

    public ActionBarItem(int id, String name, String pictureAsset) {
      this.id = id;
      this.name = name;
      this.pictureAsset = pictureAsset;
    }
  }

  @Override
  public void clear() {
    if (alternativeButtonLayout != null) {
     mainLayout.removeView(alternativeButtonLayout);
     drawerButton.setVisibility(ImageButton.VISIBLE);
     alternativeButtonLayout = null;
    }
  }

  @Override
  public void flush() {
    // TODO Auto-generated method stub
    
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
  public void setImage(byte[] bytes, int width, int height, int internalFormat) { }

  @Override
  public void reshape(int size) {
    // TODO Auto-generated method stub
    
  }


  @Override
  public void deinitialize() {
    // TODO Auto-generated method stub
    
  }
}
