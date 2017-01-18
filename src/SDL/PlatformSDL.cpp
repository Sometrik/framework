#include <FWPlatform.h>
#include <CurlClient.h>
#include <Logger.h>
#include <ContextCairo.h>
#include <SDLSoundCanvas.h>
#include <Command.h>
#include <FWApplication.h>

#include <OpenGLInitEvent.h>
#include <TouchEvent.h>
#include <DrawEvent.h>
#include <SysEvent.h>
#include <UpdateEvent.h>
#include <ResizeEvent.h>
#include <TimerEvent.h>

#include <SDL/SDL.h>

#include <cstdio>
#include <cstdlib>

using namespace std;

static Uint32 timer_callback(Uint32 interval, void *param);

class Timer {
public:
  Timer(FWPlatform * _platform, int _id, int _interval)
    : platform(_platform), id(_id), interval(_interval) { }

  int getId() const { return id; }
  FWPlatform * getPlatform() { return platform; }
      
  void create() {
    SDL_AddTimer(interval, timer_callback, this);
  }

private:
  FWPlatform * platform;
  int id, interval;
};

static Uint32 timer_callback(Uint32 interval, void *param) {
  Timer * timer = (Timer *)timer;
  FWPlatform * platform = timer->getPlatform();
  TimerEvent ev(platform->getTime(), timer->getId());
  platform->pushEvent(ev);
  return interval;
}

class PlatformSDL : public FWPlatform {
public:
  PlatformSDL() : FWPlatform(1.0f) {
    CurlClientFactory::globalInit();
  }

  void pushEvent(const Event & ev) override {
    SDL_Event user_event;
    user_event.type = SDL_USEREVENT;
    user_event.user.code = 1;
    user_event.user.data1 = ev.dup();
    user_event.user.data2 = NULL;
    SDL_PushEvent(&user_event);
  }
  
  string getLocalFilename(const char * fn, FileType type) override {
    string s = "assets/";
    return s + fn;
  }
  
  std::shared_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    return std::make_shared<CurlClientFactory>();
  }
    
  std::shared_ptr<canvas::ContextFactory> createContextFactory() const override {
    return std::shared_ptr<canvas::ContextFactory>(new canvas::CairoContextFactory);
  }

  void sendCommand2(const Command & command) override {
    switch (command.getType()) {
    // case Command::SET_CAPTION:
    //   SDL_WM_SetCaption(command.getTextValue().c_str(),
    // 			command.getTextValue().c_str());
    //   break;
    case Command::SHOW_MESSAGE_DIALOG:
#if 0
      SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_INFORMATION,
				title.c_str(),
				message.c_str(),
				NULL);
#endif
      break;
    case Command::CREATE_TIMER:
      {
	auto timer = std::make_shared<Timer>(this, command.getChildInternalId(), command.getValue());
	timer->create();
	timers[timer->getId()] = timer;
      }
      break;
    default:
      break;
    }
  }

#ifdef HAS_SOUNDCANVAS
  std::shared_ptr<SoundCanvas> createSoundCanvas() const override {
    return std::make_shared<SDLSoundCanvas>();
  }
#endif
    
  std::string getBundleFilename(const char * filename) override {
    string s = "assets/";
    return s + filename;
  }

  void run() {
    SDL_Event event;

    cerr << "starting runloop\n";
    
    while ( true ) {
      bool redraw_screen = false;
      /* Grab all the events off the queue. */
      while( SDL_PollEvent( &event ) ) {
	switch( event.type ) {
	case SDL_KEYDOWN:
	  /* Handle key presses. */
	  // handle_key_down( &event.key.keysym );
	  break;
	case SDL_MOUSEBUTTONDOWN:
	  {
	    TouchEvent ev(getTime(), TouchEvent::ACTION_DOWN, mouse_x, mouse_y, 0);
	    postEvent(getActiveViewId(), ev);
	    if (ev.isRedrawNeeded()) redraw_screen = true;

	    TouchEvent ev2(getTime(), TouchEvent::ACTION_DOWN, true);
	    postEvent(getActiveViewId(), ev2);
	    if (ev2.isRedrawNeeded()) redraw_screen = true;

	    button_pressed = true;
	  }
	  break;
	case SDL_MOUSEBUTTONUP:
	  {
	    TouchEvent ev(getTime(), TouchEvent::ACTION_UP, mouse_x, mouse_y, 0);
	    postEvent(getActiveViewId(), ev);
	    if (ev.isRedrawNeeded()) redraw_screen = true;

	    TouchEvent ev2(getTime(), TouchEvent::ACTION_UP, true);
	    postEvent(getActiveViewId(), ev2);
	    if (ev2.isRedrawNeeded()) redraw_screen = true;

	    button_pressed = false;
	  }
	  break;
	case SDL_MOUSEMOTION:
	  if (button_pressed) {
	    TouchEvent ev(getTime(), TouchEvent::ACTION_DRAG, event.motion.x, event.motion.y, 0);
	    postEvent(getActiveViewId(), ev);
	    if (ev.isRedrawNeeded()) redraw_screen = true;

	    TouchEvent ev2(getTime(), TouchEvent::ACTION_DRAG, true);
	    postEvent(getActiveViewId(), ev2);
	    if (ev2.isRedrawNeeded()) redraw_screen = true;
	  }
	  mouse_x = event.motion.x;
	  mouse_y = event.motion.y;
	  break;
	case SDL_VIDEORESIZE:
	  {
	    int w = event.resize.w, h = event.resize.h;
	    cerr << "resized (" << w << " " << h << ")\n";
	    setDisplayWidth(w);
	    setDisplayHeight(h);
	    ResizeEvent ev(getTime(), w, h, w, h);
	    postEvent(getActiveViewId(), ev);
	    if (ev.isRedrawNeeded()) redraw_screen = true;
	  }
	  break;
	case SDL_USEREVENT:
	  {
	    Event * ev = (Event*)event.user.data1;
	    assert(ev);
	    std::shared_ptr<Event> ptr(ev);
	    postEvent(getActiveViewId(), *ptr);
	    if (ptr->isRedrawNeeded()) redraw_screen = true;
	  }
	  break;
	case SDL_QUIT:
	  return;
	}
      }

      if (!is_initialized) {
	OpenGLInitEvent ev(getTime(), 0x30000, true);
	postEvent(getActiveViewId(), ev);
	is_initialized = true;
	redraw_screen = true;	
      }
      
      UpdateEvent ev0(getTime());
      postEvent(getActiveViewId(), ev0);
      if (ev0.isRedrawNeeded()) redraw_screen = true;
      
      if (redraw_screen) {
	DrawEvent ev(getTime());
	postEvent(getActiveViewId(), ev);
	SDL_GL_SwapBuffers();
      }
    }
  }

  std::string showTextEntryDialog(const std::string & title, const std::string & message) override { return ""; }

private:
  bool button_pressed = false;
  int mouse_x = 0, mouse_y = 0;
  bool is_initialized = false;
  map<int, shared_ptr<Timer> > timers;
};

#if 0
static void handle_key_down(SDL_keysym* keysym) {

    /* 
     * We're only interested if 'Esc' has
     * been presssed.
     *
     * EXERCISE: 
     * Handle the arrow keys and have that change the
     * viewing position/angle.
     */
    switch( keysym->sym ) {
    case SDLK_ESCAPE:
        quit_tutorial( 0 );
        break;
    case SDLK_SPACE:
        should_rotate = !should_rotate;
        break;
    default:
        break;
    }

}
#endif

extern FWApplication * applicationMain();

int main(int argc, char *argv[]) {
  if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER ) < 0 ) {
    /* Failed, exit. */
    fprintf( stderr, "Video initialization failed: %s\n",
             SDL_GetError( ) );
    SDL_Quit( );
    exit(1);
  }

  const SDL_VideoInfo * info = SDL_GetVideoInfo( );

  if (!info) {
    /* This should probably never happen. */
    fprintf( stderr, "Video query failed: %s\n", SDL_GetError() );
    SDL_Quit();
    exit(1);
  }

  int width = 800, height = 600;
  int bpp = info->vfmt->BitsPerPixel;

  SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  
  int flags = SDL_OPENGL | SDL_RESIZABLE; // | SDL_FULLSCREEN;

  if (SDL_SetVideoMode( width, height, bpp, flags ) == 0) {
    fprintf( stderr, "Video mode set failed: %s\n",
             SDL_GetError( ) );
    SDL_Quit();
    exit(1);
  }

  FWApplication * application = applicationMain();

  PlatformSDL platform;  
  platform.setDisplayWidth(width);
  platform.setDisplayHeight(height);
  platform.addChild(std::shared_ptr<Element>(application));
    
  platform.run();
  
  SysEvent ev(platform.getTime(), SysEvent::DESTROY);
  platform.postEvent(application->getInternalId(), ev);
  SDL_Quit();
  return 0;
}
