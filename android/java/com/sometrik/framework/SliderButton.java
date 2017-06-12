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

  FWList list;
  SliderLayout sliderLayout;
  Position position;

  FrameWork frame;
  boolean onTop = true;
  SliderButton base;

  enum Position {
    BOTTOM, MIDDLE, TOP, BOTTOMHIDDEN, TOPHIDDEN
  }

  public SliderButton(final FrameWork frame, final SliderLayout sliderLayout, boolean hideExtraButtons) {
    super(frame);
    this.frame = frame;
    this.sliderLayout = sliderLayout;
    this.hideExtraButtons = hideExtraButtons;
    base = this;

    setOnClickListener(new Button.OnClickListener() {
      @Override
      public void onClick(View arg0) {
	System.out.println("onclick on " + getId() + " activeId: " + sliderLayout.getActiveButton());
	if (sliderLayout.getActiveButton() != getId()) {
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
	  sliderLayout.setActiveButton(getId());
	  moveToMiddle();
//	  RelativeLayout.LayoutParams listParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
//	  listParams.addRule(RelativeLayout.BELOW, getId());
//	  listParams.topMargin = 10;
	  list.setViewVisibility(true);
//	  list.setLayoutParams(listParams);
	  onTop = true;

	}
      }
    });
  }

  public void setInitialPosition() {
    if (position == Position.MIDDLE) {
      RelativeLayout.LayoutParams buttonParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
      buttonParams.addRule(RelativeLayout.ALIGN_PARENT_TOP);
      setLayoutParams(buttonParams);
    } else if (position == Position.BOTTOM) {
      RelativeLayout.LayoutParams buttonParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
      buttonParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
      setLayoutParams(buttonParams);
    } else if (position == Position.BOTTOMHIDDEN || position == Position.TOPHIDDEN) {
      setVisibility(GONE);
    }
  }

  public void setList(FWList list) {
    this.list = list;
    if (position == Position.MIDDLE){
      list.setViewVisibility(true);
    }
  }

  public FWList getList() {
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
    animation.setDuration(1000);
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
	final Animation animation = new TranslateAnimation(0, 0, 0, yDelta * -1);
	// set Animation for 5 sec
	animation.setDuration(1000);
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
	animation.setDuration(1000);
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
      System.out.println("Setting buttonSize as " + getHeight());
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
    animation.setDuration(1000);
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
    animation.setDuration(1000);
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
    animation.setDuration(1000);
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
    list.setViewVisibility(false);
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
    list.setViewVisibility(false);
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
    list.setViewVisibility(false);
    if (bottomButton != null) {
      bottomButton.hideBelow();
    }

    System.out.println("BaseHeight: " + SliderLayout.buttonSize);
    final Animation animation = new TranslateAnimation(0, 0, 0, getHeight());
    animation.setDuration(1000);
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
    list.setViewVisibility(false);
    if (topButton != null) {
      topButton.hideAbove();
    }

    System.out.println("BaseHeight: " + SliderLayout.buttonSize);
    final Animation animation = new TranslateAnimation(0, 0, 0, -getHeight());
    animation.setDuration(1000);
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
