package com.sometrik.framework;

import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.view.View;

public class FWDrawerLayout extends DrawerLayout {
  
  FrameWork frame;

  public FWDrawerLayout(final FrameWork frame) {
    super(frame);
    this.frame = frame;
    this.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
    final DrawerLayout drawer = new DrawerLayout(frame);
    ActionBarDrawerToggle mDrawerToggle = new ActionBarDrawerToggle(frame, drawer,
            0, 0) {

        /** Called when a drawer has settled in a completely closed state. */
        public void onDrawerClosed(View view) {
            super.onDrawerClosed(view);
            System.out.println("drawerClosed");
            frame.invalidateOptionsMenu(); // creates call to onPrepareOptionsMenu()
            frame.sendNativeValueEvent(frame.getCurrentDrawerViewId(), 0, 0);
        }

        /** Called when a drawer has settled in a completely open state. */
        public void onDrawerOpened(View drawerView) {
            super.onDrawerOpened(drawerView);
            System.out.println("drawerOpened");
            frame.invalidateOptionsMenu(); // creates call to onPrepareOptionsMenu()
            frame.sendNativeValueEvent(frame.getCurrentDrawerViewId(), 1, 0);
        }
    };
    mDrawerToggle.setDrawerIndicatorEnabled(false);
//    scrollView.setBackground(frame.getResources().getDrawable(android.R.drawable.screen_background_light));
//    mDrawerToggle.setHomeAsUpIndicator(android.R.drawable.ic_menu_agenda);
    setDrawerListener(mDrawerToggle);
  }
  
  public void setDrawerVisibility(View view, boolean visible){
    if (!visible){
      this.closeDrawer(view);
    } else {
      this.openDrawer(view);
    }
  }

  @Override
  protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
      widthMeasureSpec = MeasureSpec.makeMeasureSpec(
              MeasureSpec.getSize(widthMeasureSpec), MeasureSpec.EXACTLY);
      heightMeasureSpec = MeasureSpec.makeMeasureSpec(
              MeasureSpec.getSize(heightMeasureSpec), MeasureSpec.EXACTLY);
      super.onMeasure(widthMeasureSpec, heightMeasureSpec);
  }
}

