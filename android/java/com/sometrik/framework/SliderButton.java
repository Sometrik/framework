package com.sometrik.framework;

import android.graphics.Canvas;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.TranslateAnimation;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.RelativeLayout.LayoutParams;

public class SliderButton extends Button {

  SliderButton topButton;
  SliderButton bottomButton;
  boolean hideExtraButtons = true;
  
  //in millis
  static int transitionTime = 500;

  NativeCommandHandler child;
  SliderLayout sliderLayout;
  Position position;
  int positionId;
  
  FrameWork frame;
  boolean onTop = true;
  SliderButton base;

  enum Position {
    BOTTOM, MIDDLE, TOP, BOTTOMHIDDEN, TOPHIDDEN
  }

  public SliderButton(final FrameWork frame, final SliderLayout sliderLayout, boolean hideExtraButtons, final int positionId) {
    super(frame);
    this.frame = frame;
    this.sliderLayout = sliderLayout;
    this.hideExtraButtons = hideExtraButtons;
    this.positionId = positionId;
    base = this;
    setBackground(frame.getResources().getDrawable(android.R.drawable.dialog_holo_light_frame));

    setOnClickListener(new Button.OnClickListener() {
      @Override
      public void onClick(View arg0) {
	System.out.println("onclick on " + positionId + " activeId: " + sliderLayout.getActiveButton());
	if (sliderLayout.getActiveButton() != positionId) {
	  if (position == Position.BOTTOM) {
	    System.out.println("position: BOTTOM");
	    topButton.raiseAbove();
	    if (bottomButton != null) {
	      bottomButton.raiseAbove();
	    }
	  } else if (position == Position.TOP) {
	    System.out.println("position: TOP");
	    if (topButton != null) {
	      topButton.collapseBelow();
	    }
	    bottomButton.collapseBelow();
	  }
	  sliderLayout.setActiveButton(positionId);
	  moveToMiddle();
	  child.setViewVisibility(true);
	  onTop = true;

	  frame.intChangedEvent(System.currentTimeMillis() / 1000.0, sliderLayout.getId(), -1, positionId);

	}
      }
    });
  }

  public void setInitialPosition() {
    System.out.println("setInitialPosition " + positionId + " position: " + String.valueOf(position));
    this.setVisibility(VISIBLE);
    if (position == Position.TOP) {
      RelativeLayout.LayoutParams buttonParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
      buttonParams.addRule(RelativeLayout.ALIGN_PARENT_TOP);
      setLayoutParams(buttonParams);
    } else if (position == Position.MIDDLE) {
      if (topButton == null){
	RelativeLayout.LayoutParams buttonParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
	buttonParams.addRule(RelativeLayout.ALIGN_PARENT_TOP);
	setLayoutParams(buttonParams);
	child.setViewVisibility(true);
      } else {
	RelativeLayout.LayoutParams buttonParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
	buttonParams.addRule(RelativeLayout.BELOW, topButton.getId());
	setLayoutParams(buttonParams);
	child.setViewVisibility(true);
      }
    } else if (position == Position.BOTTOM) {
      RelativeLayout.LayoutParams buttonParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
      buttonParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
      setLayoutParams(buttonParams);
    } else if (position == Position.BOTTOMHIDDEN || position == Position.TOPHIDDEN) {
      setVisibility(GONE);
    }
    System.out.println("setInitialPosition end " + positionId);
  }

  public void setList(NativeCommandHandler child) {
    this.child = child;
    if (position == Position.MIDDLE){
      child.setViewVisibility(true);
    }
  }

  public NativeCommandHandler getList() {
    return child;
  }

  public void setTopButton(SliderButton topButton) {
    this.topButton = topButton;
  }

  public void setBottomButton(SliderButton bottomButton) {
    this.bottomButton = bottomButton;
    View childView = (View) child;
    RelativeLayout.LayoutParams listParams = (LayoutParams) childView.getLayoutParams();
    listParams.addRule(RelativeLayout.ABOVE, bottomButton.getId());
    childView.setLayoutParams(listParams);

  }

  public void moveToTop() {

    if (topButton != null) {
      topButton.hideAbove();
    } else {
      position = Position.TOP;
      return;
    }
    position = Position.TOP;

    final float yDelta = sliderLayout.getHeight() - (SliderLayout.buttonSize);
    System.out.println("BaseHeight: " + SliderLayout.buttonSize);
    final Animation animation = new TranslateAnimation(0, 0, 0, -getHeight());
    animation.setDuration(transitionTime);
    startAnimation(animation);
    animation.setAnimationListener(new Animation.AnimationListener() {

      @Override
      public void onAnimationEnd(Animation arg0) {
	
	//Fix flickering
	TranslateAnimation anim = new TranslateAnimation(0.0f, 0.0f, 0.0f, 0.0f);
        anim.setDuration(1);
        base.startAnimation(anim);
	
	RelativeLayout.LayoutParams buttonParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
	buttonParams.addRule(RelativeLayout.ALIGN_PARENT_TOP);
	setLayoutParams(buttonParams);
      }

      @Override
      public void onAnimationRepeat(Animation arg0) {
      }

      @Override
      public void onAnimationStart(Animation arg0) {
      }
    });
  }

  private void moveToMiddle() {
    if (topButton != null) {
      if (position == Position.BOTTOM) {
	final float yDelta = sliderLayout.getHeight() - (2 * base.getHeight());
//	final Animation animation = new TranslateAnimation(0, 0, sliderLayout.getHeight() - base.getHeight(), -base.getHeight());
	final Animation animation = new TranslateAnimation(0, 0, 0, yDelta * -1);
	// set Animation for 5 sec
	animation.setDuration(transitionTime);
	// for button stops in the new position.
	// animation.setFillAfter(true);
	
	startAnimation(animation);
	animation.setAnimationListener(new Animation.AnimationListener() {

	  @Override
	  public void onAnimationEnd(Animation arg0) {

	    // Fix flickering
	    TranslateAnimation anim = new TranslateAnimation(0.0f, 0.0f, 0.0f, 0.0f);
	    anim.setDuration(1);
	    base.startAnimation(anim);


	    RelativeLayout.LayoutParams buttonParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
	    buttonParams.addRule(RelativeLayout.BELOW, topButton.getId());
	    setLayoutParams(buttonParams);
	    position = Position.MIDDLE;
	  }

	  @Override
	  public void onAnimationRepeat(Animation arg0) {
	  }

	  @Override
	  public void onAnimationStart(Animation arg0) {
	  }
	});
      } else if (position == Position.TOP) {
	final float yDelta = sliderLayout.getHeight() - (2 * base.getHeight());
	final Animation animation = new TranslateAnimation(0, 0, 0, base.getHeight());
	// set Animation for 5 sec
	animation.setDuration(transitionTime);
	// for button stops in the new position.
	// animation.setFillAfter(true);
	startAnimation(animation);
	animation.setAnimationListener(new Animation.AnimationListener() {

	  @Override
	  public void onAnimationEnd(Animation arg0) {

	    // Fix flickering
	    TranslateAnimation anim = new TranslateAnimation(0.0f, 0.0f, 0.0f, 0.0f);
	    anim.setDuration(1);
	    base.startAnimation(anim);

	    RelativeLayout.LayoutParams buttonParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
	    buttonParams.addRule(RelativeLayout.BELOW, topButton.getId());
	    setLayoutParams(buttonParams);
	    position = Position.MIDDLE;
	  }

	  @Override
	  public void onAnimationRepeat(Animation arg0) {
	  }

	  @Override
	  public void onAnimationStart(Animation arg0) {
	  }
	});
      }

    } else {
      position = Position.MIDDLE;
    }
  }

  @Override
  protected void onDraw(Canvas canvas) {
    super.onDraw(canvas);
    if (getHeight() != 0) {
      SliderLayout.buttonSize = getHeight();
    }

  }

  private void moveToBottomFromHidden() {
    position = Position.BOTTOM;

    RelativeLayout.LayoutParams buttonParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
    setLayoutParams(buttonParams);
    final float yDelta = sliderLayout.getHeight() - (SliderLayout.buttonSize);
    System.out.println("BaseHeight: " + SliderLayout.buttonSize);
    final Animation animation = new TranslateAnimation(0, 0, sliderLayout.getHeight(), yDelta);
    animation.setDuration(transitionTime);
    startAnimation(animation);
    animation.setAnimationListener(new Animation.AnimationListener() {

      @Override
      public void onAnimationEnd(Animation arg0) {

	// Fix flickering
	TranslateAnimation anim = new TranslateAnimation(0.0f, 0.0f, 0.0f, 0.0f);
	anim.setDuration(1);
	base.startAnimation(anim);

	RelativeLayout.LayoutParams buttonParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
	buttonParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
	setLayoutParams(buttonParams);
      }

      @Override
      public void onAnimationRepeat(Animation arg0) {
      }

      @Override
      public void onAnimationStart(Animation arg0) {
      }
    });
  }

  private void moveToTopFromHidden() {
    position = Position.TOP;

    RelativeLayout.LayoutParams buttonParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
    setLayoutParams(buttonParams);
    final float yDelta = sliderLayout.getHeight() - (SliderLayout.buttonSize);
    final Animation animation = new TranslateAnimation(0, 0, -SliderLayout.buttonSize, 0);
    animation.setDuration(transitionTime);
    startAnimation(animation);
    animation.setAnimationListener(new Animation.AnimationListener() {

      @Override
      public void onAnimationEnd(Animation arg0) {

	// Fix flickering
	TranslateAnimation anim = new TranslateAnimation(0.0f, 0.0f, 0.0f, 0.0f);
	anim.setDuration(1);
	base.startAnimation(anim);

	RelativeLayout.LayoutParams buttonParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
	buttonParams.addRule(RelativeLayout.ALIGN_PARENT_TOP);
	setLayoutParams(buttonParams);
      }

      @Override
      public void onAnimationRepeat(Animation arg0) {
      }

      @Override
      public void onAnimationStart(Animation arg0) {
      }
    });
  }

  private void moveToBottomFromMiddle() {
    System.out.println("biz " + getId() + " moving to BOTTOM");
    if (bottomButton != null) {
      bottomButton.hideBelow();
    }

    position = Position.BOTTOM;

    final float yDelta = sliderLayout.getHeight() - (2 * base.getHeight());
    final Animation animation = new TranslateAnimation(0, 0, 0, yDelta * 1);
    // set Animation for 5 sec
    animation.setDuration(transitionTime);
    // for button stops in the new position.
    // animation.setFillAfter(true);
    startAnimation(animation);
    animation.setAnimationListener(new Animation.AnimationListener() {

      @Override
      public void onAnimationEnd(Animation arg0) {

	// Fix flickering
	TranslateAnimation anim = new TranslateAnimation(0.0f, 0.0f, 0.0f, 0.0f);
	anim.setDuration(1);
	base.startAnimation(anim);

	RelativeLayout.LayoutParams buttonParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
	buttonParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
	setLayoutParams(buttonParams);
      }

      @Override
      public void onAnimationRepeat(Animation arg0) {
      }

      @Override
      public void onAnimationStart(Animation arg0) {
      }
    });
  }

  public void raiseAbove() {
    onTop = true;
    child.setViewVisibility(false);
    this.setVisibility(VISIBLE);
    if (position == Position.BOTTOMHIDDEN) {
      moveToBottomFromHidden();
    } else if (position == Position.MIDDLE) {
      moveToTop();
    } else if (position == Position.TOP) {
      if (topButton != null) {
	hideAbove();
      }
    }
  }

  public void collapseBelow() {
    System.out.println("Collapse below on " + getId() + " position: " + position);
    onTop = false;
    child.setViewVisibility(false);
    this.setVisibility(VISIBLE);

    if (position == Position.MIDDLE) {
      moveToBottomFromMiddle();
    } else if (position == Position.TOPHIDDEN) {
      moveToTopFromHidden();
    } else if (position == Position.BOTTOM) {
      hideBelow();
    }
  }

  public void hideBelow() {
    System.out.println("biz " + getId() + " hidden below");
    onTop = false;
    child.setViewVisibility(false);
    if (bottomButton != null) {
      bottomButton.hideBelow();
    }

    System.out.println("BaseHeight: " + SliderLayout.buttonSize);
    final Animation animation = new TranslateAnimation(0, 0, 0, getHeight());
    animation.setDuration(transitionTime);
    startAnimation(animation);
    animation.setAnimationListener(new Animation.AnimationListener() {

      @Override
      public void onAnimationEnd(Animation arg0) {
	base.setVisibility(GONE);
      }

      @Override
      public void onAnimationRepeat(Animation arg0) {
      }

      @Override
      public void onAnimationStart(Animation arg0) {
      }
    });

    this.setVisibility(GONE);
    position = Position.BOTTOMHIDDEN;
  }

  public void hideAbove() {
    onTop = true;
    child.setViewVisibility(false);
    if (topButton != null) {
      topButton.hideAbove();
    }

    System.out.println("BaseHeight: " + SliderLayout.buttonSize);
    final Animation animation = new TranslateAnimation(0, 0, 0, -getHeight());
    animation.setDuration(transitionTime);
    startAnimation(animation);
    animation.setAnimationListener(new Animation.AnimationListener() {

      @Override
      public void onAnimationEnd(Animation arg0) {
	base.setVisibility(GONE);
      }

      @Override
      public void onAnimationRepeat(Animation arg0) {
      }

      @Override
      public void onAnimationStart(Animation arg0) {
      }
    });
    position = Position.TOPHIDDEN;
  }

  public Position getPosition() {
    return position;
  }

  public void setPosition(Position position) {
    this.position = position;
  }
}
