package com.sometrik.framework;

import java.net.MalformedURLException;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.sometrik.framework.NativeCommand.Selector;

import android.graphics.Bitmap;
import android.text.Html;
import android.text.Layout;
import android.text.Spannable;
import android.text.SpannableString;
import android.text.TextPaint;
import android.text.method.LinkMovementMethod;
import android.text.style.URLSpan;
import android.util.Patterns;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;

public class FWTextView extends TextView implements NativeCommandHandler {

  private FrameWork frame;
  private ViewStyleManager normalStyle, activeStyle, currentStyle, linkStyle;
  private ArrayList<String> linkList;
  private boolean autolink;
  private boolean linkOptionsSet = false;

  public FWTextView(final FrameWork frame, boolean autolink) {
    super(frame);
    this.frame = frame;
    this.autolink = autolink;
    linkList = new ArrayList<String>();

    final float scale = getContext().getResources().getDisplayMetrics().density;
    this.normalStyle = currentStyle = new ViewStyleManager(frame.bitmapCache, scale, true);
    this.activeStyle = new ViewStyleManager(frame.bitmapCache, scale, false);
    this.linkStyle = new ViewStyleManager(frame.bitmapCache, scale, false);

    this.setClickable(false);
    this.setLongClickable(false);
  }
  
  private void stripUnderlines() {
    Spannable s = new SpannableString(getText());
    URLSpan[] spans = s.getSpans(0, s.length(), URLSpan.class);
    for (URLSpan span : spans) {
      int start = s.getSpanStart(span);
      int end = s.getSpanEnd(span);
      s.removeSpan(span);
      span = new URLSpanNoUnderline(span.getURL());
      s.setSpan(span, start, end, 0);
    }
    setText(s);
  }
  
  private class URLSpanNoUnderline extends URLSpan {
    public URLSpanNoUnderline(String url) {
      super(url);
    }

    @Override
    public void updateDrawState(TextPaint ds) {
      super.updateDrawState(ds);
      ds.setUnderlineText(false);
    }
  }

  private String cleanUpURLToShow(String link) {
    try {
      URL url = new URL(link);
      System.out.println("cleanUpURL host: " + url.getHost() + " path: " + url.getPath());
      return (url.getHost() + url.getPath()).toString();
    } catch (MalformedURLException e) {
      e.printStackTrace();
      return link;
    }
  }
  
  private void setToLink(String text) {
    String textViewText = getText().toString();

    System.out.println("setToLink " + text);
    
    Pattern pattern = Pattern.compile(text);
    List<String> matches = new ArrayList<String>();
    Matcher m = pattern.matcher(getText());
    while (m.find()) {
      if (m.groupCount() > 0) {
	String s = m.group(1);
	System.out.println("setToLink match " + s);
	boolean alreadyAdded = false;
	for (String match : matches) {
	  if (match.equals(s)) {
	    System.out.println("match already added " + s);
	    alreadyAdded = true;
	    break;
	  }
	}
	if (!alreadyAdded) {
	  matches.add(s);
	}
      }
    }
    String newText = textViewText;
    for (String match : matches) {
      newText = newText.replaceAll("(" + match + ")" + "(?!\\w)", "<a href=\"" + match + "\">" + match + "</a>");
      System.out.println("setToLink new text " + newText);
    }
    
    if (autolink) {
      
      Pattern urlPattern = Patterns.WEB_URL;
//      String regex = "(@)?(href=')?(HREF=')?(HREF=\")?(href=\")?(http://)?[a-zA-Z_0-9\\-]+(\\.\\w[a-zA-Z_0-9\\-]+)+(/[#&\\n\\-=?\\+\\%/\\.\\w]+)?";
//      String regex = "\\b(https?|ftp|file)://[-a-zA-Z0-9+&@#/%?=~_|!:,.;]*[-a-zA-Z0-9+&@#/%=~_|]";
//      Pattern urlPattern = Pattern.compile(regex);
      
      List<String> autoLinkMatches = new ArrayList<String>();
      Matcher m2 = urlPattern.matcher(getText());
      while (m2.find()) {
	if (m2.groupCount() > 0) {
	  String s = m2.group(1);
//	  try {
//	    URL urlCheck = new URL(s);
//	  } catch (MalformedURLException e) {
//	    e.printStackTrace();
//	    continue;
//	  }
	  autoLinkMatches.add(s);
	}
      }
      for (String match : autoLinkMatches) {
        newText = newText.replaceAll(match, "<a href=\"" + match + "\">" + cleanUpURLToShow(match) + "</a>");
        System.out.println("setToLink (a) new text " + newText);
      }
    }
    

    if (!newText.equals(textViewText)) {
      try {
      setText(Html.fromHtml(newText));
      } catch(Exception e) {
	e.printStackTrace();
	System.out.println("Exception setToLink");
      }
    }
    
    stripUnderlines();
    
    if (!linkOptionsSet) {

      this.setLinksClickable(false);
      this.setMovementMethod(new TextViewLinkHandler() {
  	@Override
  	public void onLinkClick(String url) {

    	System.out.println("link clicked " + url);
  	try {
  	  URL u = new URL(url);
  	  u.toURI();
  	  frame.launchBrowser(url);
  	  return;
  	} catch (URISyntaxException e) {
  	  e.printStackTrace();
  	} catch (MalformedURLException e) {
  	  e.printStackTrace();
  	}

  	byte[] b = url.getBytes(frame.getCharset());
  	frame.sendNativeValueEvent(getElementId(), b);
  	}
      });

      this.setFocusable(true);
      this.setClickable(false);
      this.setLongClickable(false);
      linkOptionsSet = true;
    }
  }
  
  public void addLink(String text) {
    for (String link : linkList) {
      if (link.equals(text)) {
	return;
      }
    }
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
	System.out.println("link movement clicked " + link);
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
