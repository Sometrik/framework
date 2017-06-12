package com.sometrik.framework;

import android.view.View;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.RelativeLayout.LayoutParams;

public class SliderButton extends Button {

  
  SliderButton topButton;
  SliderButton bottomButton;
  boolean hideExtraButtons = true;
  
  FWList list;
  SliderLayout sliderLayout;
  
  FrameWork frame;
  boolean onTop = true;
  SliderButton base;
  
  public SliderButton(final FrameWork frame, final SliderLayout sliderLayout, boolean hideExtraButtons) {
    super(frame);
    this.frame = frame;
    this.sliderLayout = sliderLayout;
    this.hideExtraButtons = hideExtraButtons;
    base = this;
    
    setOnClickListener(new Button.OnClickListener(){
      @Override
      public void onClick(View arg0) {

	onTop = true;
	if (topButton != null) {
	  topButton.raiseAbove();
	}
	if (bottomButton != null) {
	  bottomButton.collapseBelow();
	}
	setPosition();
	list.setViewVisibility(true);
      }
    });
    
  }
  
  public void setPosition(){
    int[] oldCoordinates = new int[2];
    getLocationOnScreen(oldCoordinates);
    if (onTop) {
      if (topButton != null) {
	RelativeLayout.LayoutParams buttonParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
	buttonParams.addRule(RelativeLayout.BELOW, topButton.getId());
	setLayoutParams(buttonParams);
	
	//TODO animation
//	int[] coordinates = new int[2];
//	getLocationOnScreen(coordinates);
//	System.out.println("oldY: " + oldCoordinates[1] + " Y: " + coordinates[1]);
//	//set position TranslateAnimation(float fromXDelta, float toXDelta, float fromYDelta, float toYDelta
//	final Animation animation = new TranslateAnimation(0,0,0,y - oldY);
//	// set Animation for 5 sec
//	animation.setDuration(1000);
//	//for button stops in the new position.
//	animation.setFillAfter(true);
//	startAnimation(animation);
      } else {
	RelativeLayout.LayoutParams buttonParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
	buttonParams.addRule(RelativeLayout.ALIGN_PARENT_TOP);
	setLayoutParams(buttonParams);
      }
    } else {
      if (bottomButton != null && !hideExtraButtons) {
	RelativeLayout.LayoutParams buttonParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
	buttonParams.addRule(RelativeLayout.ABOVE, bottomButton.getId());
	setLayoutParams(buttonParams);
      } else {
	RelativeLayout.LayoutParams buttonParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
	buttonParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
	setLayoutParams(buttonParams);
      }
    }
  }
  
  public void setList(FWList list){
    this.list = list;
  }
  
  public FWList getList(){
    return list;
  }

  public void setTopButton(SliderButton topButton) {
    this.topButton = topButton;
  }

  public void setBottomButton(SliderButton bottomButton) {
    this.bottomButton = bottomButton;
    RelativeLayout.LayoutParams listParams = (LayoutParams) list.getLayoutParams();
    listParams.addRule(RelativeLayout.ABOVE, bottomButton.getId());
    list.setLayoutParams(listParams);
    
  }
  
  public void collapseBelow(){
    onTop = false;
    list.setViewVisibility(false);
    if (bottomButton != null) {
      if (!hideExtraButtons) {
	bottomButton.collapseBelow();
      } else {
	bottomButton.hideBelow();
      }
    }
    this.setVisibility(VISIBLE);
    setPosition();
  }
  
  public void raiseAbove() {
    onTop = true;
    list.setViewVisibility(false);
    if (topButton != null) {
      if (!hideExtraButtons) {
	topButton.raiseAbove();
      } else {
	topButton.hideAbove();
      }
    }
    this.setVisibility(VISIBLE);
    setPosition();
  }
  
  public void hideBelow() {
    onTop = false;
    list.setViewVisibility(false);
    if (bottomButton != null) {
      bottomButton.hideBelow();
    }
    setPosition();
    this.setVisibility(GONE);
  }
  
  public void hideAbove(){
    onTop = true;
    list.setViewVisibility(false);
    if (topButton != null){
      topButton.hideAbove();
    }
    setPosition();
    this.setVisibility(GONE);
  }
}
