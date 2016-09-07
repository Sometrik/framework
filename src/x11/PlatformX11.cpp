#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>
#include <cassert>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include <FWContextBase.h>
#include <FWPlatformBase.h>
#include <CurlClient.h>

#include <ContextCairo.h>

#include <iostream>

using namespace std;

bool XNextEventTimed(Display * dsp, XEvent * event_return, struct timeval * tv) {
  // optimization

  if (tv == NULL) {
    XNextEvent(dsp, event_return);
    return True;
  }

  // the real deal

  if (XPending(dsp) == 0) {
    int fd = ConnectionNumber(dsp);
    fd_set readset;
    FD_ZERO(&readset);
    FD_SET(fd, &readset);
    if (select(fd+1, &readset, NULL, NULL, tv) == 0) {
      return false;
    } else {
      XNextEvent(dsp, event_return);
      return true;
    }
  } else {
    XNextEvent(dsp, event_return);
    return true;
  }
}

class PlatformX11 : public FWPlatformBase {
public:
  PlatformX11() : FWPlatformBase(1.0f,
				 // "#version 300 es",
				 "#version 100",
				 true) { }

  double getTime() const override {
    struct timeval tv;
    struct timezone tz;
    int r = gettimeofday(&tv, &tz);
    double t = 0;
    if (r == 0) {
      t = (double)tv.tv_sec + tv.tv_usec / 1000000.0;
    }
    return t;
  }
  
  void handleEvent(XEvent & xev) {
    // cerr << "received event " << int(xev.type) << endl;
    KeySym key;
    char text;
  
    switch (xev.type) {
    case KeyPress:
      if (XLookupString(&xev.xkey, &text, 1, &key, 0) == 1) {
	getApplication().onKeyPress(text, getTime(), 0, 0);
      }
      break;
    case DestroyNotify:
      userInterrupt = true;
      break;
    case MotionNotify:
      // getApplication().onMouseMove(xev.xmotion.x, xev.xmotion.y);
      if (button_pressed) {
	getApplication().touchesMoved(xev.xmotion.x, xev.xmotion.y, getTime(), 0);
      }
      mouse_x = xev.xmotion.x;
      mouse_y = xev.xmotion.y;
      break;
    case ButtonPress:
      // getApplication().onMouseDown(xev.xbutton.button, 0, 0);
      getApplication().touchesBegin(mouse_x, mouse_y, getTime(), 0);
      button_pressed = true;
      break;
    case ButtonRelease:
      // getApplication().onMouseUp(xev.xbutton.button, 0, 0);
      getApplication().touchesEnded(mouse_x, mouse_y, getTime(), 0);
      button_pressed = false;
      break;
    case ConfigureNotify:
      {
	XConfigureEvent xce = xev.xconfigure;
	display_width = xce.width;
	display_height = xce.height;	
      }
      break;
    }
  }

  void sendEvents() {
    if (display_width != getApplication().getActualWidth() ||
	display_height != getApplication().getActualHeight()) {
      getApplication().onResize(display_width, display_height, display_width, display_height);
    }
  }
  
  void showMessageBox(const string&, const string&) {

  }
  void postNotification(const string&, const string&) {
    
  }
  string getLocalFilename(const char * fn, FileType type) {
    string s = "android_projects/assets/";
    return s + fn;
  }
  
  std::shared_ptr<HTTPClientFactory> createHTTPClientFactory() const {
    return std::make_shared<CurlClientFactory>();
  }
  
  int showActionSheet(const FWRect&, const FWActionSheet&) {
    return 0;
  }

  bool readEvents() {
    XEvent xev;

    while ( XPending(x_display) ) {
      XNextEvent(x_display, &xev);
      handleEvent(xev);
    }
    if (!userInterrupt) {
      sendEvents();
    }

    if (!userInterrupt) {
      timeval tv = { 0, 1000000 / 50 };
      if (XNextEventTimed(x_display, &xev, &tv)) {
	handleEvent(xev);
	sendEvents();
      }
    }

    return userInterrupt;
  }

  bool createWindow(FWContextBase * context, const char* title) override {
    Window root;
    XSetWindowAttributes swa;
    XSetWindowAttributes  xattr;
    Atom wm_state;
    XWMHints hints;
    XEvent xev;
    EGLConfig ecfg;
    EGLint num_config;
    Window win;

    /*
     * X11 native display initialization
     */

    x_display = XOpenDisplay(NULL);
    if (x_display == NULL) {
      return false;
    }

    root = DefaultRootWindow(x_display);

    // esContext->getActualWidth(), esContext->getActualHeight()
    int width = 800;
    int height = 600;
    
    swa.event_mask = ExposureMask | PointerMotionMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask;
    win = XCreateWindow(
               x_display, root,
               0, 0, width, height, 0,
               CopyFromParent, InputOutput,
               CopyFromParent, CWEventMask,
               &swa );

    xattr.override_redirect = 0;
    XChangeWindowAttributes ( x_display, win, CWOverrideRedirect, &xattr );

    // XSelectInput?
    
    hints.input = 1;
    hints.flags = InputHint;
    XSetWMHints(x_display, win, &hints);

    // make the window visible on the screen
    XMapWindow (x_display, win);
    XStoreName (x_display, win, title);

    // get identifiers for the provided atom name strings
    wm_state = XInternAtom (x_display, "_NET_WM_STATE", 0);

    memset ( &xev, 0, sizeof(xev) );
    xev.type                 = ClientMessage;
    xev.xclient.window       = win;
    xev.xclient.message_type = wm_state;
    xev.xclient.format       = 32;
    xev.xclient.data.l[0]    = 1;
    xev.xclient.data.l[1]    = 0;
    XSendEvent (
       x_display,
       DefaultRootWindow ( x_display ),
       0,
       SubstructureNotifyMask,
       &xev );

    eglNativeWindow = (EGLNativeWindowType) win;
    eglNativeDisplay = (EGLNativeDisplayType) x_display;

    cerr << "native display = " << x_display << endl;
    
    return true;
  }

  std::shared_ptr<canvas::ContextFactory> createContextFactory() const {
    return std::shared_ptr<canvas::ContextFactory>(new canvas::CairoContextFactory);
  }

  void startEventLoop() {
    struct timeval t1, t2;
    struct timezone tz;
    float deltatime;
    
    gettimeofday ( &t1 , &tz );
    
    while (readEvents() == false) {
      getApplication().loadEvents();

      gettimeofday(&t2, &tz);
      deltatime = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
      t1 = t2;
      
      if (getApplication().onUpdate(deltatime)) {
	getApplication().onDraw();
	eglSwapBuffers(eglDisplay, eglSurface);
      }
    }
  }

  void showMessageBox(const std::string & message) {
    
  }

  std::string showTextEntryDialog(const std::string & message) {
    return "";
  }

  void postNotification(const std::string & message) {

  }

  void launchBrowser(const std::string & input_url) {

  }

  std::string getBundleFilename(const char * filename) {
    return filename;
  }

  void storeValue(const std::string & key, const std::string & value) {
    
  }

  std::string loadValue(const std::string & key) {
    return "";
  }

private:
  // X11 related local variables
  Display * x_display = NULL;
  bool userInterrupt = false;
  int mouse_x = 0, mouse_y = 0;
  bool button_pressed = false;
};

///
//  Global extern.  The application must declare this function
//  that runs the application.
//
extern void applicationMain(FWPlatformBase * platform);

///
//  main()
//
//      Main entrypoint for application
//
int main(int argc, char *argv[]) {
  PlatformX11 platform;
  cerr << "starting\n";
  applicationMain(&platform);
  // platform.createWindow(&(platform.getApplication()), "App");
  platform.createContext(&(platform.getApplication()), "App", 800, 600);
  platform.getApplication().onCmdLine(argc, argv);
  platform.getApplication().Init();
  platform.startEventLoop();
  platform.getApplication().onShutdown();
  return 0;
}
