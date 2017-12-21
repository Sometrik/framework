package com.sometrik.framework;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap;
import android.text.Layout;
import android.text.Spannable;
import android.text.method.LinkMovementMethod;
import android.text.style.URLSpan;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;

public class FWTextView extends TextView implements NativeCommandHandler {

  private FrameWork frame;
  ViewStyleManager normalStyle, activeStyle, currentStyle, linkStyle;
  
  public FWTextView(final FrameWork frame, boolean autolink) {
    super(frame);
    this.frame = frame;
    
    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
    this.linkStyle = new ViewStyleManager(frame.bitmapCache, scale, false);

    if (autolink) {
      this.setLinksClickable(false);
      this.setMovementMethod(new TextViewLinkHandler() {
	@Override
	public void onLinkClick(String url) {
	  System.out.println("link clicked");
	  frame.launchBrowser(url);
	}
      });
      this.setAutoLinkMask(15);
    }

    this.setClickable(false);
    this.setLongClickable(false);
    this.setFocusable(false);
  }
  
  public abstract class TextViewLinkHandler extends LinkMovementMethod {

    public boolean onTouchEvent(TextView widget, Spannable buffer, MotionEvent event) {
      if (event.getAction() != MotionEvent.ACTION_UP)
	return false;
        

        int x = (int) event.getX();
        int y = (int) event.getY();

        x -= widget.getTotalPaddingLeft();
        y -= widget.getTotalPaddingTop();

        x += widget.getScrollX();
        y += widget.getScrollY();

        Layout layout = widget.getLayout();
        int line = layout.getLineForVertical(y);
        int off = layout.getOffsetForHorizontal(line, x);

        URLSpan[] link = buffer.getSpans(off, off, URLSpan.class);
        if (link.length != 0) {
            onLinkClick(link[0].getURL());
            return true;
        }
        return false;
    }

    abstract public void onLinkClick(String url);
}
    
  @Override
  public void addChild(View view) { }
  
  @Override
  public void addOption(int optionId, String text) { }

  @Override
  public void setValue(String v) {
    setText(v);
  }

  @Override
  public void setValue(int v) { }

  @Override
  public void setStyle(Selector selector, String key, String value) {
    if (selector == Selector.NORMAL) {
      normalStyle.setStyle(key, value);
    } else if (selector == Selector.ACTIVE) {
      activeStyle.setStyle(key, value);      
    } else if (selector == Selector.LINK) {
      linkStyle.setStyle(key, value);
    }

    if (key.equals("max-lines")) {
      setMaxLines(Integer.parseInt(value));
    }
  }

  @Override
  public void applyStyles() {
    currentStyle.apply(this);  
    linkStyle.applyLinkColor(this);
  }
  
  @Override
  public void setError(boolean hasError, String errorText) { }

  @Override
  public int getElementId() {
    return getId();
  }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) { }

  @Override
  public void addData(String text, int row, int column, int sheet) { }

  @Override
  public void setViewVisibility(boolean visibility) {
    if (visibility){
      this.setVisibility(VISIBLE);
    } else {
      this.setVisibility(INVISIBLE);
    }
  }

  @Override
  public void clear() { }

  @Override
  public void flush() { }

  @Override
  public void addColumn(String text, int columnType) { }

  @Override
  public void reshape(int value, int size) { }

  @Override
  public void setBitmap(Bitmap bitmap) { }

  @Override
  public void reshape(int size) { }

  @Override
  public void deinitialize() { }

  @Override
  public void addImageUrl(String url, int width, int height) { }
}
