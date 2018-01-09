package com.sometrik.framework;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap;
import android.text.Html;
import android.text.Layout;
import android.text.Spannable;
import android.text.method.LinkMovementMethod;
import android.text.style.URLSpan;
import android.text.util.Linkify;
import android.text.util.Linkify.TransformFilter;
import android.util.Patterns;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;

public class FWTextView extends TextView implements NativeCommandHandler {

  private FrameWork frame;
  private ViewStyleManager normalStyle, activeStyle, currentStyle, linkStyle;
  private ArrayList<String> linkList;
  private boolean autolink;

  public FWTextView(final FrameWork frame, boolean autolink) {
    super(frame);
    this.frame = frame;
    this.autolink = autolink;
    linkList = new ArrayList<String>();

    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
    this.linkStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
    if (autolink) {
      this.setLinksClickable(false);
//      this.setMovementMethod(new TextViewLinkHandler() {
//	@Override
//	public void onLinkClick(String url) {
//	  System.out.println("link clicked " + url);
//	  frame.launchBrowser(url);
//	}
//      });
//      this.setAutoLinkMask(15);
      this.setFocusable(true);
      

      TransformFilter transformFilter = new TransformFilter() {
        public final String transformUrl(final Matcher match, String url) {
  	return url;
        }
      };
     
      Pattern urlPattern = Patterns.WEB_URL;
      Linkify.addLinks(this, urlPattern, null, null, transformFilter);
    }
    

    this.setClickable(false);
    this.setLongClickable(false);
  }
  
  
  private void setToLink(String text) {
    String textViewText = (String) getText();

    Pattern pattern = Pattern.compile(text);
    List<String> matches = new ArrayList<String>();
    Matcher m = pattern.matcher(getText());
    while (m.find()) {
      String s = m.group(1);
      matches.add(s);
    }
    for (String match : matches) {
      String newText = textViewText.replaceAll(match, "<a href=\"" + match + "\">" + match + "</a>");
      setText(Html.fromHtml(newText));
    }
    
    this.setLinksClickable(false);
    this.setMovementMethod(new TextViewLinkHandler() {
	@Override
	public void onLinkClick(String url) {
	System.out.println("link clicked " + url);
	byte[] b = url.getBytes(frame.getCharset());
	frame.sendNativeValueEvent(getElementId(), b);
//	  frame.launchBrowser(url);
	}
    });

    this.setFocusable(true);
    this.setClickable(false);
    this.setLongClickable(false);
  }
  
  public void addLink(String text) {
    linkList.add(text);
    setToLink(text);
  }

  public abstract class TextViewLinkHandler extends LinkMovementMethod {

    public boolean onTouchEvent(TextView widget, Spannable buffer, MotionEvent event) {
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
	if (event.getAction() == MotionEvent.ACTION_UP) {
	  onLinkClick(link[0].getURL());
	}
	return true;
      } else {
	return false;
      }
    }

    abstract public void onLinkClick(String url);
  }

  @Override
  public void addChild(View view) {
  }

  @Override
  public void addOption(int optionId, String text) {
    
  }

  @Override
  public void setValue(String v) {
    setText(v);

    if (!linkList.isEmpty()) {
      for (String link : linkList) {
        setToLink(link);
      }
    }

    if (autolink) {
      TransformFilter transformFilter = new TransformFilter() {
        public final String transformUrl(final Matcher match, String url) {
  	return url;
        }
      };

	//TODO remove http
      Pattern urlPattern = Patterns.WEB_URL;
      Linkify.addLinks(this, urlPattern, null, null, transformFilter);
    }
  }

  @Override
  public void setValue(int v) {
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
  public void setError(boolean hasError, String errorText) {
  }

  @Override
  public int getElementId() {
    return getId();
  }

  @Override
  public void onScreenOrientationChange(boolean isLandscape) {
  }

  @Override
  public void addData(String text, int row, int column, int sheet) {
  }

  @Override
  public void setViewVisibility(boolean visibility) {
    if (visibility) {
      this.setVisibility(VISIBLE);
    } else {
      this.setVisibility(INVISIBLE);
    }
  }

  @Override
  public void clear() {
  }

  @Override
  public void flush() {
  }

  @Override
  public void addColumn(String text, int columnType) {
  }

  @Override
  public void reshape(int value, int size) {
  }

  @Override
  public void setBitmap(Bitmap bitmap) {
  }

  @Override
  public void reshape(int size) {
  }

  @Override
  public void deinitialize() {
  }

  @Override
  public void addImageUrl(String url, int width, int height) {
  }
}
