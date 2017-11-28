package com.sometrik.framework;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;

import com.sometrik.framework.NativeCommand.Selector;

import android.app.ActionBar;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.view.Gravity;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
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
  
  private static boolean debugClickEnabled = true;
  private int debugClickCounter = 0;

  public FWActionBar(final FrameWork frame, int id){
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
    Bitmap bitmap = frame.bitmapCache.loadBitmap("chaff_actionbar.png");
    
    if (bitmap != null) {
      Drawable draw = new BitmapDrawable(bitmap);
      
      drawerButton.setImageDrawable(draw);
      drawerButton.setScaleType(ScaleType.FIT_CENTER);
      drawerButton.setBackgroundColor(Color.parseColor("#ffffff"));
      drawerButton.setOnClickListener(new OnClickListener() {
	@Override
	public void onClick(View v) {
	  if (debugClickEnabled) {
	    debugClickCounter = 0;
	  }
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
    
    if (debugClickEnabled) {
      mainLayout.setOnClickListener(new OnClickListener() {
	@Override
	public void onClick(View arg0) {
	  debugClickCounter++;
	  System.out.println("actionBar debugClick: " + debugClickCounter);
	  if (debugClickCounter > 4) {
	    System.out.println("actionBar debugClick: showing debug");
	    frame.showNativeDebug();
	    debugClickCounter = 0;
	  }
	}
      });
    }
    
//    titleView = new TextView(frame);
//    titleView.setText(title);
//    titleView.setLayoutParams(params);
//    titleView.setGravity(Gravity.CENTER);
//    titleView.setTextSize(14);
//    titleView.setTextColor(Color.parseColor("#5d6468"));
//    
//
//    subtitleView = new TextView(frame);
//    subtitleView.setText("subtitle");
//    subtitleView.setLayoutParams(params);
//    subtitleView.setGravity(Gravity.CENTER);
//    subtitleView.setVisibility(TextView.GONE);
//    subtitleView.setTextSize(10);
//    subtitleView.setTextColor(Color.parseColor("#b7c4cd"));
    

//    LinearLayout titleHolderLayout = new LinearLayout(frame);
//    titleHolderLayout.setOrientation(LinearLayout.VERTICAL);
//    titleHolderLayout.setLayoutParams(params);
//    
//    titleHolderLayout.addView(titleView);
//    titleHolderLayout.addView(subtitleView);
    
    int pixels = (int) (58 * scale + 0.5f);
//    int pixels = drawerButton.getWidth();
//    titleView.setPadding(titleView.getPaddingLeft(), titleView.getPaddingRight(), pixels, titleView.getPaddingBottom());
//    subtitleView.setPadding(subtitleView.getPaddingLeft(), subtitleView.getPaddingRight(), pixels, subtitleView.getPaddingBottom());
//    titleView.setGravity(Gravity.CENTER);
//    mainLayout.addView(titleHolderLayout);
    
    
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
    alternativeButtonLayout = new FWLayout(frame, 0);
    LinearLayout.LayoutParams params2 = new LinearLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.MATCH_PARENT);
    alternativeButtonLayout.setLayoutParams(params2);
    alternativeButtonLayout.addView(view);
    alternativeButtonLayout.setGravity(Gravity.CENTER_HORIZONTAL);
    mainLayout.addView(alternativeButtonLayout, 0);

    alternativeButtonLayout.setOnClickListener(new OnClickListener() {
      @Override
      public void onClick(View arg0) {
	System.out.println("actionbar: view clicked " + view.getId());
	frame.sendNativeValueEvent(view.getId(), 1, 1);
      }
    });
  }
  
  public void setTitle(String title) {
//    titleView.setText(title);
  }
  
  public ArrayList<ActionBarItem> getItemList(){
    return itemList;
  }
  
  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void addChild(View view) {
//    if (view instanceof TextView) {
//      ((TextView) view).setGravity(Gravity.CENTER);
//      ((TextView) view).setTextAlignment(TextView.TEXT_ALIGNMENT_CENTER);
//    } else if (view instanceof LinearLayout) {
//      ((LinearLayout) view).setGravity(Gravity.CENTER);
//    }
    mainLayout.addView(view);
  }

  @Override
  public void addOption(int optionId, String text) {
    itemList.add(new ActionBarItem(optionId, text));
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
    System.out.println("Command couldn't be handled by FWActionBar");
  }

  @Override
  public void setValue(String v) {
//    setTitle(v);
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
    if (key.equals("drawer-button")) {
      if (value.equals("hidden")) {
	drawerButton.setVisibility(Button.GONE);
      } else if (value.equals("visible")) {
	drawerButton.setVisibility(Button.VISIBLE);
      }
    }
    if (key.equals("height")) {
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
  public void setError(boolean hasError, String errorText) { }

  @Override
  public int getElementId() {
    return id;
  }

  
  public class ActionBarItem {

    int id;
    String pictureAsset;
    BitmapDrawable picture;

    public ActionBarItem(int id, String pictureAsset) {
      this.id = id;
      this.pictureAsset = pictureAsset;

      try {
	InputStream stream = frame.getAssets().open(pictureAsset);
	if (stream != null) {
	  Bitmap bitmap = BitmapFactory.decodeStream(stream);
	  picture = new BitmapDrawable(frame.getResources(), bitmap);
	  stream.close();
	}
      } catch (IOException e) {
	e.printStackTrace();
      }

    }
  }

  @Override
  public void clear() {
    if (alternativeButtonLayout != null) {
     mainLayout.removeView(alternativeButtonLayout);
     drawerButton.setVisibility(ImageButton.VISIBLE);
     alternativeButtonLayout = null;
    }
    mainLayout.removeAllViews();
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
}
