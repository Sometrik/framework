package com.sometrik.framework;

import java.lang.ref.WeakReference;

import com.sometrik.framework.NativeCommand.Selector;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.Point;
import android.graphics.Rect;
import android.graphics.drawable.BitmapDrawable;
import android.view.Choreographer;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;

public class BlurLayout extends FrameLayout implements NativeCommandHandler {
  private FrameWork frame;
  private ViewStyleManager normalStyle, activeStyle, currentStyle;
  
  private float downscaleFactor = 0.25f;
  private int blurRadius = 12;
  private int FPS = 60;
  private Bitmap.Config blurConfig = Bitmap.Config.ARGB_4444;
  
  private WeakReference<View> activityView;

  public BlurLayout(FrameWork frameWork, int id) {
    super(frameWork);
    
    this.frame = frameWork;
    this.setId(id);
    
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
    
    BlurKit.init(getContext());

    if (FPS > 0) {
      Choreographer.getInstance().postFrameCallback(invalidationLoop);
    }
  }
  
  private Choreographer.FrameCallback invalidationLoop = new Choreographer.FrameCallback() {
    @Override
    public void doFrame(long frameTimeNanos) {
      invalidate();
      Choreographer.getInstance().postFrameCallbackDelayed(this, 1000 / FPS);
    }
  };

  @Override
  public void invalidate() {
    super.invalidate();
    Bitmap bitmap = blur();
    if (bitmap != null) {
      setBackground(new BitmapDrawable(bitmap));
    }
  }

  private Bitmap blur() {
    if (getContext() == null) {
      return null;
    }

    // Check the reference to the parent view.
    // If not available, attempt to make it.
    if (activityView == null || activityView.get() == null) {
      activityView = new WeakReference<View>(getActivityView());
      if (activityView.get() == null) {
	return null;
      }
    }

    // Calculate the relative point to the parent view.
    Point pointRelativeToActivityView = getPositionInScreen();

    // Set alpha to 0 before creating the parent view bitmap.
    // The blur view shouldn't be visible in the created bitmap.
    setAlpha(0);

    // Screen sizes for bound checks
    int screenWidth = activityView.get().getWidth();
    int screenHeight = activityView.get().getHeight();

    // The final dimensions of the blurred bitmap.
    int width = (int) (getWidth() * downscaleFactor);
    int height = (int) (getHeight() * downscaleFactor);

    // The X/Y position of where to crop the bitmap.
    int x = (int) (pointRelativeToActivityView.x * downscaleFactor);
    int y = (int) (pointRelativeToActivityView.y * downscaleFactor);

    // Padding to add to crop pre-blur.
    // Blurring straight to edges has side-effects so padding is added.
    int padding = getWidth() > getHeight() ? getWidth() / 8 : getHeight() / 8;

    // Calculate padding independently for each side, checking edges.
    int leftOffset = -padding;
    leftOffset = x + leftOffset >= 0 ? leftOffset : 0;

    int rightOffset = padding;
    rightOffset = x + getWidth() + rightOffset <= screenWidth ? rightOffset : screenWidth - getWidth() - x;

    int topOffset = -padding;
    topOffset = y + topOffset >= 0 ? topOffset : 0;

    int bottomOffset = padding;
    bottomOffset = y + height + bottomOffset <= screenHeight ? bottomOffset : 0;

    // Create parent view bitmap, cropped to the BlurLayout area with above padding.
    Bitmap bitmap;
    try {
      bitmap = getDownscaledBitmapForView(
                    activityView.get(),
                    new Rect(
                            pointRelativeToActivityView.x + leftOffset,
                            pointRelativeToActivityView.y + topOffset,
                            pointRelativeToActivityView.x + getWidth() + Math.abs(leftOffset) + rightOffset,
                            pointRelativeToActivityView.y + getHeight() + Math.abs(topOffset) + bottomOffset
                    ),
                    downscaleFactor
            );
    } catch (NullPointerException e) {
      return null;
    }

    // Blur the bitmap.
    bitmap = BlurKit.getInstance().blur(bitmap, blurRadius);

    // Crop the bitmap again to remove the padding.
    bitmap = Bitmap.createBitmap(
                bitmap,
                (int) (Math.abs(leftOffset) * downscaleFactor),
                (int) (Math.abs(topOffset) * downscaleFactor),
                width,
                height
        );

    // Make self visible again.
    setAlpha(1);

    // Set background as blurred bitmap.
    return bitmap;
  }

  /**
   * Casts context to Activity and attempts to create a view reference using the window decor view.
   * @return View reference for whole activity.
   */
  private View getActivityView() {
    Activity activity;
    try {
      activity = (Activity) getContext();
    } catch (ClassCastException e) {
      return null;
    }

    return activity.getWindow().getDecorView().findViewById(android.R.id.content);
  }

  /**
   * Returns the position in screen. Left abstract to allow for specific implementations such as
   * caching behavior.
   */
  private Point getPositionInScreen() {
    return getPositionInScreen(this);
  }

  /**
   * Finds the Point of the parent view, and offsets result by self getX() and getY().
   * @return Point determining position of the passed in view inside all of its ViewParents.
   */
  private Point getPositionInScreen(View view) {
    if (getParent() == null) {
      return new Point();
    }

    ViewGroup parent;
    try {
      parent = (ViewGroup) view.getParent();
    } catch (Exception e) {
      return new Point();
    }

    if (parent == null) {
      return new Point();
    }

    Point point = getPositionInScreen(parent);
    point.offset((int) view.getX(), (int) view.getY());
    return point;
  }

  /**
   * Users a View reference to create a bitmap, and downscales it using the passed in factor.
   * Uses a Rect to crop the view into the bitmap.
   * @return Bitmap made from view, downscaled by downscaleFactor.
   * @throws NullPointerException
   */
  private Bitmap getDownscaledBitmapForView(View view, Rect crop, float downscaleFactor) throws NullPointerException {
    View screenView = view.getRootView();

    int width = (int) (crop.width() * downscaleFactor);
    int height = (int) (crop.height() * downscaleFactor);

    if (screenView.getWidth() <= 0 || screenView.getHeight() <= 0 || width <= 0 || height <= 0) {
      throw new NullPointerException();
    }

    float dx = -crop.left * downscaleFactor;
    float dy = -crop.top * downscaleFactor;

    Bitmap bitmap = Bitmap.createBitmap(width, height, blurConfig);
    Canvas canvas = new Canvas(bitmap);
    Matrix matrix = new Matrix();
    matrix.preScale(downscaleFactor, downscaleFactor);
    matrix.postTranslate(dx, dy);
    canvas.setMatrix(matrix);
    screenView.draw(canvas);

    return bitmap;
  }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
    invalidate();
  }

  @Override
  public void addChild(View view) {
    addView(view);
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
  public void setBitmap(Bitmap bitmap) { }

  @Override
  public void addImageUrl(String url, int width, int height) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void setValue(int v) {
    // TODO Auto-generated method stub
    
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
    if (visible) {
      frame.hideSoftKeyboard();
      this.setVisibility(VISIBLE);
    } else {
      this.setVisibility(GONE);
    }    
  }

  @Override
  public void setStyle(Selector selector, String key, String value) {
    if (selector == Selector.NORMAL) {
      normalStyle.setStyle(key, value);
      if (normalStyle == currentStyle) normalStyle.apply(this);
    } else if (selector == Selector.ACTIVE) {
      activeStyle.setStyle(key, value);      
      if (activeStyle == currentStyle) activeStyle.apply(this);
    }
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

}
