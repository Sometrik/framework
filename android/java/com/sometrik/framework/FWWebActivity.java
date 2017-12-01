package com.sometrik.framework;

import android.app.Activity;
import android.os.Bundle;
import android.support.v7.widget.Toolbar.LayoutParams;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.FrameLayout;

public class FWWebActivity extends Activity {
  
  public static final String URL_EXTRA = "url";
  
  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    String url = getIntent().getStringExtra(URL_EXTRA);
    
    FrameLayout layout = new FrameLayout(this);
    layout.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
    setContentView(layout);
    
    WebView webView = new WebView(this);
    webView.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
//    webView.setWebViewClient(new MyWebViewClient());
    webView.setWebViewClient(new WebViewClient());
    webView.getSettings().setJavaScriptEnabled(true);
    layout.addView(webView);
    webView.loadUrl(url);
    
  }
  
  // override default behaviour of the browser
  private class MyWebViewClient extends WebViewClient {
    @Override
    public boolean shouldOverrideUrlLoading(WebView view, String url) {
      view.loadUrl(url);
      return true;
    }
  }
  
}
