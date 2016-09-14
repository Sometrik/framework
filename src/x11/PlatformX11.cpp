
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
#include <EventLoop.h>
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
    // cerr << "nothing pending, fd = " << fd << "\n";
    fd_set readset;
    FD_ZERO(&readset);
    FD_SET(fd, &readset);
    int r = select(fd+1, &readset, NULL, NULL, tv);
    if (r == -1) {
      cerr << "select failed\n";
      return false;
    } else if (r == 0) {
      cerr << "nothing to select\n";    
      return false;
    } else {
      // cerr << "getting event (r = " << r << ")\n";
      if (XPending(dsp)) {
	XNextEvent(dsp, event_return);
	return true;
      } else {
	return false;
      }
    }
  } else {
    // cerr << "pending\n";
    
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

#if 0
    XSelectInput(x_display, win, 
		 ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask |
		 ButtonPressMask | ButtonReleaseMask  | StructureNotifyMask 
		 );
#endif
    
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

    XFlush(x_display);

    return true;
  }

  std::shared_ptr<canvas::ContextFactory> createContextFactory() const {
    return std::shared_ptr<canvas::ContextFactory>(new canvas::CairoContextFactory);
  }

  std::shared_ptr<EventLoop> createEventLoop() override;
  
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
    string s = "assets/";
    return s + filename;
  }

  void storeValue(const std::string & key, const std::string & value) {
    
  }

  std::string loadValue(const std::string & key) {
    return "";
  }

  void swapBuffers() {
    eglSwapBuffers(eglDisplay, eglSurface);
  }

private:
  // X11 related local variables
  Display * x_display = NULL;
};


class PlatformX11;

class EventLoopX11 : public EventLoop {
public:
  EventLoopX11( Display * _x_display,
		PlatformX11 * _platform,
		FWContextBase * _application
		)
    : EventLoop(_application), x_display(_x_display), platform(_platform) { }

  void readEvents() {
    XEvent xev;

    // cerr << "read pending\n";
    
    while ( XPending(x_display) ) {
      XNextEvent(x_display, &xev);
      handleEvent(xev);
    }
    if (doKeepRunning()) {
      sendEvents();
    }

    if (doKeepRunning()) {
      // cerr << "read all\n";
    
      timeval tv = { 1, 0 }; // 1000000 / 50 };
      if (XNextEventTimed(x_display, &xev, &tv)) {
	handleEvent(xev);
	sendEvents();
      }
    }
  }

  void run() override {
    struct timeval t1, t2;
    struct timezone tz;
    float deltatime;
    
    gettimeofday ( &t1 , &tz );
    
    while (doKeepRunning()) {
      readEvents();
      getApplication()->loadEvents();

#if 0
      gettimeofday(&t2, &tz);
      deltatime = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
      t1 = t2;
#endif
      
      if (getApplication()->onUpdate(getPlatform()->getTime())) {
	getApplication()->onDraw();
	platform->swapBuffers();
      }
    }
  }

protected:
  void sendEvents() {
    if (display_width && display_height &&
	(display_width != getApplication()->getActualWidth() ||
	 display_height != getApplication()->getActualHeight())) {
      getApplication()->onResize(display_width, display_height, display_width, display_height);
    }
  }

  void handleEvent(XEvent & xev) {
    KeySym key;
    char text;
  
    switch (xev.type) {
    case KeyPress:
      if (XLookupString(&xev.xkey, &text, 1, &key, 0) == 1) {
	getApplication()->onKeyPress(text, getPlatform()->getTime(), 0, 0);
      }
      break;
    case DestroyNotify:
      stop();
      break;
    case MotionNotify:
      // getApplication().onMouseMove(xev.xmotion.x, xev.xmotion.y);
      if (button_pressed) {
	getApplication()->touchesMoved(xev.xmotion.x, xev.xmotion.y, getPlatform()->getTime(), 0);
      }
      mouse_x = xev.xmotion.x;
      mouse_y = xev.xmotion.y;
      break;
    case ButtonPress:
      // getApplication().onMouseDown(xev.xbutton.button, 0, 0);
      getApplication()->touchesBegin(mouse_x, mouse_y, getPlatform()->getTime(), 0);
      button_pressed = true;
      break;
    case ButtonRelease:
      // getApplication().onMouseUp(xev.xbutton.button, 0, 0);
      getApplication()->touchesEnded(mouse_x, mouse_y, getPlatform()->getTime(), 0);
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

  PlatformX11 * getPlatform() { return platform; }
  
private:
  Display * x_display;
  PlatformX11 * platform;
  int display_width = 0, display_height = 0;
  bool button_pressed = false;
  int mouse_x = 0, mouse_y = 0;
};

std::shared_ptr<EventLoop>
PlatformX11::createEventLoop() {
  return std::make_shared<EventLoopX11>(x_display, this, &(getApplication()));
}

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

  auto eventloop = platform.createEventLoop();
  eventloop->run();
  
  platform.getApplication().onShutdown();
  return 0;
}
