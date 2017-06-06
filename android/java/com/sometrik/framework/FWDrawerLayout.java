package com.sometrik.framework;

import android.graphics.Color;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.TextView;

public class FWDrawerLayout extends DrawerLayout {
  
  FrameWork frame;

  public FWDrawerLayout(final FrameWork frame) {
    super(frame);
    this.frame = frame;
    
    final DrawerLayout drawer = new DrawerLayout(frame);
    System.out.println("here jnot");
    
    DrawerLayout.LayoutParams lp = new DrawerLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
    lp.gravity = Gravity.START;

    FrameLayout drawFrame = new FrameLayout(frame);
    drawFrame.setLayoutParams(lp);
    drawFrame.setBackgroundColor(Color.parseColor("#dddbd6"));
    LinearLayout drawerPanel = new LinearLayout(frame);
    drawerPanel.setGravity(Gravity.START);
    drawerPanel.setClickable(true);
    drawerPanel.setOrientation(LinearLayout.VERTICAL);
    
    ActionBarDrawerToggle mDrawerToggle = new ActionBarDrawerToggle(frame, drawer,
            0, 0) {

        /** Called when a drawer has settled in a completely closed state. */
        public void onDrawerClosed(View view) {
            super.onDrawerClosed(view);
            System.out.println("drawerClosed");
            frame.invalidateOptionsMenu(); // creates call to onPrepareOptionsMenu()
        }

        /** Called when a drawer has settled in a completely open state. */
        public void onDrawerOpened(View drawerView) {
            super.onDrawerOpened(drawerView);
            System.out.println("drawerOpened");
            frame.invalidateOptionsMenu(); // creates call to onPrepareOptionsMenu()
        }
    };
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

