#import "OpenGLView.h"
#import "InputDialog.h"
#include <FWApplication.h>
#include <FWPlatform.h>

#include <OpenGLInitEvent.h>
#include <SysEvent.h>
#include <TouchEvent.h>
#include <UpdateEvent.h>
#include <ResizeEvent.h>
#include <DrawEvent.h>

#include <ContextQuartz2D.h>

#define FBO_COLOR       1
#define FBO_DEPTH       2
#define FBO_STENCIL     4

#define USE_CURL

#ifdef USE_CURL
#include <CurlClient.h>
#else
#include <iOSClient.h>
#endif

#import <Foundation/Foundation.h>

#include <iostream>

using namespace std;

namespace canvas {
class BundleFilenameConverter : public FilenameConverter {
public:
  bool convert(const std::string & input, std::string & output) {
    const char * filename = input.c_str();
    int n = (int)input.size();
    for (int i = n - 1; i >= 0; i--)  {
      if (filename[i] == '/')  {
        filename = filename + i + 1;
        break;
      }
    }
    // \Given a fileName, convert into a path that can be used to open from the mainBundle
    NSString* fileNameNS = [NSString stringWithUTF8String:filename];
    NSString* baseName = [fileNameNS stringByDeletingPathExtension];
    NSString* extension = [fileNameNS pathExtension];
    NSString *path = [[NSBundle mainBundle] pathForResource: baseName ofType: extension ];
    const char * filename2 = [path cStringUsingEncoding:1];
    if (filename2) {
      output = filename2;
      return true;
    } else {
      return false;
    }    
  }
};
};

class FWiOSPlatform : public FWPlatform {
public:
    FWiOSPlatform(OpenGLView * _controller, float _display_scale, const std::string & glsl_version, bool has_es3)
  : FWPlatform(_display_scale, glsl_version.c_str(), has_es3),
  controller(_controller) {
      defaults = [NSUserDefaults standardUserDefaults];
    }
    
    void createFBO(int flags) override {
      [controller createFBO: flags];
    }
#if 0
    void postNotification(const std::string & title, const std::string & message) override {
      NSString* message2 = [NSString stringWithUTF8String:message.c_str()];
//      [[NSNotificationCenter defaultCenter] postNotificationName:message2 object:self];
    }
#endif
    std::string showTextEntryDialog(const std::string & message) override {
      // NSString* message2 = [NSString stringWithUTF8String:message];
      InputDialog * dlg = [InputDialog dialog];
      has_active_modal = true;
      [dlg showModal];
      has_active_modal = false;
      NSString * input = [dlg getInput];
      const char * stringAsChar = [input cStringUsingEncoding:NSUTF8StringEncoding];
      return stringAsChar;
    }
    std::string getBundleFilename(const char * filename) override {
      int n = int(strlen(filename));
      for (int i = n - 1; i >= 0; i--)  {
        if (filename[i] == '/')  {
          filename = filename + i + 1;
          break;
        }
      }
      NSString* fileNameNS = [NSString stringWithUTF8String:filename];
      NSString* baseName = [fileNameNS stringByDeletingPathExtension];
      NSString* extension = [fileNameNS pathExtension];
      NSString *path = [[NSBundle mainBundle] pathForResource: baseName ofType: extension ];
      const char * filename2 = [path cStringUsingEncoding:1];
      // release stuff
      if (!filename2) cerr << "could not find file " << filename << " in bundle" << endl;
      return filename2 ? filename2 : "";
    }
   std::string getLocalFilename(const char * filename, FileType type) override {
      NSString* fileNameNS = [NSString stringWithUTF8String:filename];
      NSString *docsDir;
      NSArray *dirPaths;    
      // Get the documents directory
      dirPaths = NSSearchPathForDirectoriesInDomains
	(NSDocumentDirectory, NSUserDomainMask, YES);    
      docsDir = dirPaths[0];
      // Build the path to the database file
      NSString * finalPath = [[NSString alloc] initWithString: 
		      [docsDir stringByAppendingPathComponent: fileNameNS]];
      const char * ptr = [finalPath cStringUsingEncoding:1];
     return ptr;
  }
  void storeValue(const std::string & key, const std::string & value) override {
    NSString *storedVal = [NSString stringWithUTF8String:value.c_str()];
    NSString *storedKey = [NSString stringWithUTF8String:key.c_str()];
    [defaults setObject:storedVal forKey:storedKey];
    [defaults synchronize]; // this method is optional
  }
  std::string loadValue(const std::string & key) override {
    NSString *loadedKey = [NSString stringWithUTF8String:key.c_str()];
    NSString *results = [defaults stringForKey:loadedKey];
    string r = [results cStringUsingEncoding:[NSString defaultCStringEncoding]]; // wrong encoding
    return r;
  }
  std::shared_ptr<canvas::ContextFactory> createContextFactory() const override {
    std::shared_ptr<canvas::FilenameConverter> ptr(new canvas::BundleFilenameConverter);
    return std::make_shared<canvas::Quartz2DContextFactory>(getDisplayScale(), ptr);
  }
  std::shared_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
#ifdef USE_CURL
    return std::make_shared<CurlClientFactory>();
#else
    return std::make_shared<iOSClientFactory>();
#endif
  }
  void sendCommand(const Command & command) override {
    FWPlatform::sendCommand(command);
    switch (command.getType()) {
      case Command::LAUNCH_BROWSER:
      {
        cerr << "trying to open browser" << endl;
        NSString *input_url2 = [NSString stringWithUTF8String:command.getTextValue().c_str()];
        NSURL *url = [NSURL URLWithString:input_url2];
        [[UIApplication sharedApplication] openURL:url];
        // no need to release anything
      }
      case Command::REQUEST_REDRAW:
        [controller requestUpdate];
        break;
      default:
        break;
    }
  }
  double getTime() const override {
    double t = [[NSProcessInfo processInfo] systemUptime];
    return t;
  }
  bool hasActiveModal() const { return has_active_modal; }
#if 0
  int showActionSheet(const FWRect & rect, const FWActionSheet & sheet) override {
    NSString *title = [[NSString alloc] initWithUTF8String:sheet.getTitle().c_str()];
    UIAlertController * view = [UIAlertController
                                 alertControllerWithTitle:title
                                 message:@"Select you Choice"
                                 preferredStyle:UIAlertControllerStyleActionSheet];
    for (const auto & o : sheet.getOptions()) {
      NSString *text = [[NSString alloc] initWithUTF8String:o.getText().c_str()];
      UIAlertAction* action = [UIAlertAction
                        actionWithTitle:text
                        style:UIAlertActionStyleDefault
                        handler:^(UIAlertAction * action)
                        {
                             //Do some thing here
#if 0
                            [view dismissOpenGLViewAnimated:YES completion:nil];
#endif
                        }];
      [view addAction:action];
    }
    [view setModalPresentationStyle:UIModalPresentationPopover];
    
    // UIPopoverPresentationController * popPresenter = [view popoverPresentationController];
    // popPresenter.sourceRect = CGRectMake(10, 10, 10, 10);
    
#if 0
    view.popoverPresentationController.sourceView = controller.view;
    view.popoverPresentationController.sourceRect = CGRectMake(rect.x, rect.y, rect.width, rect.height);
    [controller presentOpenGLView:view animated:YES completion:nil];
#endif
    
    return 0;
  }
#endif
                
private:
  NSUserDefaults *defaults;
  bool has_active_modal = false;
  OpenGLView * controller;
};

extern FWApplication * applicationMain();

@interface OpenGLView ()
{
  FWApplication * _esContext;
  FWiOSPlatform * _platform;
//  float _display_scale;
  int _opengl_version;
  bool _has_es3;
  int drawableWidth, drawableHeight;
  EAGLContext * context;
  bool need_update;
  CADisplayLink* displayLink;
  unsigned int current_framebuffer;
  GLuint framebuffer, color, depth, stencil;
}
// @property (strong, nonatomic) EAGLContext *context;

- (void)tearDownGL;
@end

@implementation OpenGLView

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

#if 0
- (void)application:didFinishLaunchingWithOptions:
{
  NSURL *url = (NSURL *)[launchOptions valueForKey:UIApplicationLaunchOptionsURLKey];
  if ([url isFileURL]) {
    // Handle file being passed in
  } else {
    // Handle custom URL scheme
  }
}
#endif

- (void) createFBO: (int) flags
{
  cerr << "creating renderbuffers and framebuffers (" << drawableWidth << " " << drawableHeight << ")" << endl;

  if (framebuffer) glDeleteFramebuffers(1, &framebuffer);
  if (depth) {
    glDeleteRenderbuffers(1, &depth);
    depth = 0;
  }
  if (stencil) {
    glDeleteRenderbuffers(1, &stencil);
    stencil = 0;
  }
  if (color) glDeleteRenderbuffers(1, &color);

  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  current_framebuffer = framebuffer;
  
  if (flags & FBO_DEPTH) {
    cerr << "creating depth buffer" << endl;
    glGenRenderbuffers(1, &depth);
    glBindRenderbuffer(GL_RENDERBUFFER, depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, drawableWidth, drawableHeight);
  }
  
  if (flags & FBO_STENCIL) {
    cerr << "creating stencil buffer" << endl;
    glGenRenderbuffers(1, &stencil);
    glBindRenderbuffer(GL_RENDERBUFFER, stencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, drawableWidth, drawableHeight);
  }
  
  cerr << "creating color buffer" << endl;
  glGenRenderbuffers(1, &color);
  glBindRenderbuffer(GL_RENDERBUFFER, color);
  CAEAGLLayer* eaglLayer = (CAEAGLLayer*) self.layer;

  [context renderbufferStorage:GL_RENDERBUFFER fromDrawable: eaglLayer];

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color);
  if (flags & FBO_DEPTH) {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
  }
  if (flags & FBO_STENCIL) {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencil);
  }
  
  // Check the Framebuffer status
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  assert(status == GL_FRAMEBUFFER_COMPLETE);

#if 0  
  GLuint backbuffer, backbuffer_color, scenedepth, scenecolor, scene;
  // Create the on-screen FBO:
  glGenFramebuffers(1, &backbuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, backbuffer);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			    GL_RENDERBUFFER, backbuffer_color);
  glBindRenderbuffer(GL_RENDERBUFFER, backbuffer_color);

  // Create the depth buffer for the full-size off-screen FBO:
  glGenRenderbuffers(1, &scenedepth);
  glBindRenderbuffer(GL_RENDERBUFFER, scenedepth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);

  glGenRenderbuffers(1, &scenecolor);
  glBindRenderbuffer(GL_RENDERBUFFER, scenecolor);

  glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES, width, height);
  glGenFramebuffers(1, &scene);
  glBindFramebuffer(GL_FRAMEBUFFER, scene);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, scenecolor);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, scenedepth);
#endif
}

- (id) initWithFrame: (CGRect) frame
{
    if (self = [super initWithFrame:frame])
    {
	[self setMultipleTouchEnabled:YES];
	[UIView setAnimationsEnabled:NO];

        // _display_scale = [[UIScreen mainScreen] scale];
	float scale = [[UIScreen mainScreen] scale];

        CAEAGLLayer* eaglLayer = (CAEAGLLayer*) self.layer;
        eaglLayer.opaque = YES;
        eaglLayer.contentsScale = scale;
#if 0	
  eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking,
                                kEAGLColorFormatRGB565, kEAGLDrawablePropertyColorFormat,
                                nil];
#endif

      
	self->_opengl_version = 300;
	self->_has_es3 = true;
        self->need_update = true;
	
	self->framebuffer = 0;
	self->color = 0;
	self->depth = 0;
	self->stencil = 0;

        EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES3;
        self->context = [[EAGLContext alloc] initWithAPI:api];
	
	if (!self->context) {
	    self->context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	    self->_opengl_version = 200;
	    self->_has_es3 = false;
	}
        
        if (!self->context || ![EAGLContext setCurrentContext:context]) {
            NSLog(@"Sorry, an iPhone 3GS or higher is required for this sample.");
            // [self release];
            return nil;
        }
      
	drawableWidth = drawableHeight = 0; // set to zero so that ResizeEvent is sent

        int actual_width = int(CGRectGetWidth(frame) * scale);
        int actual_height = int(CGRectGetHeight(frame) * scale);
        // m_applicationEngine->Initialize(width, height);

        _platform = new FWiOSPlatform(self, scale, 1 ? "#version 100" : "#version 300 es", _has_es3);
	_platform->setDisplayWidth(actual_width);
	_platform->setDisplayHeight(actual_height);

	_esContext = applicationMain();

	_platform->addChild(std::shared_ptr<Element>(_esContext));
      
	OpenGLInitEvent ev(_platform->getTime(), self->_opengl_version);
	_platform->postEvent(_platform->getActiveViewId(), ev);

        [self drawView: nil];
        // m_timestamp = CACurrentMediaTime();
        
        displayLink = nil;
	[self startRenderLoop];

#if 0
	[[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];

	[[NSNotificationCenter defaultCenter]
		addObserver: self
		selector: @selector(didRotate:)
		name: UIDeviceOrientationDidChangeNotification
		object: nil];
#endif
    }
    return self;
}

- (void)viewDidLoad
{
  cerr << "viewDidLoad" << endl;
  // [super viewDidLoad];
}

- (void)viewDidAppear:(BOOL)animated {
  // [super viewDidAppear:animated];
  [self becomeFirstResponder];
}

#if 0
- (void)viewWillDisappear:(BOOL)animated {
  [self resignFirstResponder];
  [super viewWillDisappear:animated];
}
#endif

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
  [touches enumerateObjectsUsingBlock:^(id obj, BOOL *stop) {
    UITouch *touch = obj;
    long long id = (long long)touch;
    CGPoint touchPoint = [touch locationInView:self];
    TouchEvent ev(event.timestamp, TouchEvent::ACTION_UP, touchPoint.x, touchPoint.y, id);
    _platform->postEvent(_platform->getActiveViewId(), ev);
    need_update |= ev.isRedrawNeeded();
    // need_update |= _esContext->touchesEnded(touchPoint.x, touchPoint.y, event.timestamp, id);
  }];
  TouchEvent ev(event.timestamp, TouchEvent::ACTION_UP, true);
  _platform->postEvent(_platform->getActiveViewId(), ev);
  need_update |= ev.isRedrawNeeded();
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  if (_platform->hasActiveModal()) return;
  [touches enumerateObjectsUsingBlock:^(id obj, BOOL *stop) {
    // Get a single touch and it's location
    UITouch *touch = obj;
    long long id = (long long)touch;
    CGPoint touchPoint = [touch locationInView:self];
    TouchEvent ev(event.timestamp, TouchEvent::ACTION_MOVE, touchPoint.x, touchPoint.y, id);
    _platform->postEvent(_platform->getActiveViewId(), ev);
    need_update |= ev.isRedrawNeeded();
    // need_update |= _esContext->touchesMoved(touchPoint.x, touchPoint.y, event.timestamp, id);
  }];
  TouchEvent ev(event.timestamp, TouchEvent::ACTION_MOVE, true);
  _platform->postEvent(_platform->getActiveViewId(), ev);
  need_update |= ev.isRedrawNeeded();
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
  if (_platform->hasActiveModal()) return;
  // Enumerate over all the touches
  [touches enumerateObjectsUsingBlock:^(id obj, BOOL *stop) {
    // Get a single touch and it's location
    UITouch *touch = obj;
    long long id = (long long)touch;
    CGPoint touchPoint = [touch locationInView:self];
    TouchEvent ev(event.timestamp, TouchEvent::ACTION_DOWN, touchPoint.x, touchPoint.y, id);
    _platform->postEvent(_platform->getActiveViewId(), ev);
    need_update |= ev.isRedrawNeeded();
    // need_update |= _esContext->touchesBegin(touchPoint.x, touchPoint.y, event.timestamp, id);
  }];
  TouchEvent ev(event.timestamp, TouchEvent::ACTION_DOWN, true);
  _platform->postEvent(_platform->getActiveViewId(), ev);
  need_update |= ev.isRedrawNeeded();
}

-(BOOL)canBecomeFirstResponser {
  return YES;
}
- (void)motionBegan:(UIEventSubtype)motion withEvent:(UIEvent *)event
{
}

- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event
{
    if (motion == UIEventSubtypeMotionShake ) {
        // User was shaking the device. Post a notification named "shake".
        // [[NSNotificationCenter defaultCenter] postNotificationName:@"shake" object:self];
	need_update |= _esContext->onShake(event.timestamp);
    }
}

- (void)motionCancelled:(UIEventSubtype)motion withEvent:(UIEvent *)event
{   
}

- (void)shakeSuccess
{
    // do something...
}

- (void)dealloc {
  [self tearDownGL];
  if ([EAGLContext currentContext] == self->context) {
    [EAGLContext setCurrentContext:nil];
  }
  // [super dealloc];
}

- (void)tearDownGL {
  [EAGLContext setCurrentContext:self->context];
  double t = [[NSProcessInfo processInfo] systemUptime];
  SysEvent ev(t, SysEvent::SHUTDOWN);
  _platform->postEvent(0, ev);
  delete _esContext;
}

- (void) drawView: (CADisplayLink*) displayLink
{
  [EAGLContext setCurrentContext:self->context];
  need_update |= _esContext->loadEvents();
  double t = [[NSProcessInfo processInfo] systemUptime];
  assert(_platform->getActiveViewId());
  UpdateEvent ev0(t);
  _platform->postEvent(_platform->getActiveViewId(), ev0);
  need_update |= ev0.isRedrawNeeded();
  // need_update |= _esContext->onUpdate(t);
  if (need_update) {
    // CGRect adjustedFrame = [self convertRect:self.bounds fromView:nil];
    unsigned int logical_width = int(CGRectGetWidth(self.bounds));
    unsigned int logical_height = int(CGRectGetHeight(self.bounds));
  
#if 0
    UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
    cerr << "orientation = " << int(orientation) << endl;
    if (orientation == UIDeviceOrientationPortrait || orientation == UIDeviceOrientationPortraitUpsideDown) {
      cerr << "flipping view" << endl;
      unsigned int tmp = logical_width;
      logical_width = logical_height;
      logical_height = tmp;
    }
#endif
  
    int newWidth = int(logical_width * self.contentScaleFactor);
    int newHeight = int(logical_height * self.contentScaleFactor);
  
    if (newWidth != drawableWidth || newHeight != drawableHeight) {
      drawableWidth = newWidth;
      drawableHeight = newHeight;
      
      cerr << "resize window: " << logical_width << " " << logical_height << endl;
      ResizeEvent ev(t, logical_width, logical_height, drawableWidth, drawableHeight);
      _platform->postEvent(_platform->getActiveViewId(), ev);
      // _esContext->onResize(logical_width, logical_height, drawableWidth, drawableHeight);
    }
    DrawEvent ev1(t);
    _platform->postEvent(_platform->getActiveViewId(), ev1);
    
    [context presentRenderbuffer:GL_RENDERBUFFER];
    need_update = false;
  }
}

- (void) requestUpdate {
  need_update = true;
}

- (void) startRenderLoop {
  if (displayLink == nil) {
    displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawView:)];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    NSLog(@"Started Render Loop");
    need_update = true;
  }
}

- (void)stopRenderLoop
{
    if (displayLink != nil) {
        //if the display link is present, we invalidate it (so the loop stops)
        [displayLink invalidate];
        displayLink = nil;
        NSLog(@"Stopping Render Loop");
	[EAGLContext setCurrentContext:self->context];
    	glFlush(); // Flush so that queued commands are not executed when the app is in the background
    }
}

- (void)sendMemoryWarning
{
  double t = [[NSProcessInfo processInfo] systemUptime];
  SysEvent ev(t, SysEvent::MEMORY_WARNING);
  _platform->postEvent(0, ev);
}

#if 0
- (void)didRotate: (NSNotification*) notification
{
    UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
    int o = (int)orientation;
    cerr << "didRotate: o = " << o << endl;
    // [self drawView2];
}
#endif

@end
