#import "OpenGLView.h"
#import "InputDialog.h"
#include "FWContextBase.h"
#include "FWPlatformBase.h"

#include <ContextQuartz2D.h>
#include <CurlClient.h>

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

class FWiOSPlatform : public FWPlatformBase {
public:
    FWiOSPlatform(OpenGLView * _controller, float _display_scale, const std::string & glsl_version, bool has_es3)
  : FWPlatformBase(_display_scale, glsl_version.c_str(), has_es3),
  controller(_controller) {
      defaults = [NSUserDefaults standardUserDefaults];
    }
    
    bool createWindow(FWContextBase * context, const char * title) override {
        return true;
    }
    void createFBO(int flags) override {
      [controller createFBO: flags];
    }
    void showMessageBox(const std::string & title, const std::string & message) override {
      has_active_modal = true;
      has_active_modal = false;
    }
    void postNotification(const std::string & title, const std::string & message) override {
      NSString* message2 = [NSString stringWithUTF8String:message.c_str()];
//      [[NSNotificationCenter defaultCenter] postNotificationName:message2 object:self];
    }
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
    NSString *storedVal = [[NSString alloc] initWithUTF8String:value.c_str()];
    NSString *storedKey = [[NSString alloc] initWithUTF8String:key.c_str()];
    [defaults setObject:storedVal forKey:storedKey];
    [defaults synchronize]; // this method is optional
    [storedVal release];
    [storedKey release];
  }
  std::string loadValue(const std::string & key) override {
    NSString *loadedKey = [[NSString alloc] initWithUTF8String:key.c_str()];
    NSString *results = [defaults stringForKey:loadedKey];
    string r = [results cStringUsingEncoding:[NSString defaultCStringEncoding]]; // wrong encoding
    [loadedKey release];
    [results release];
    return r;
  }
  std::shared_ptr<canvas::ContextFactory> createContextFactory() const override {
    std::shared_ptr<canvas::FilenameConverter> ptr(new canvas::BundleFilenameConverter);
    return std::make_shared<canvas::Quartz2DContextFactory>(getDisplayScale(), ptr);
  }
  std::shared_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    return std::make_shared<CurlClientFactory>();
  }
  void launchBrowser(const std::string & input_url) override {
    cerr << "trying to open browser" << endl;
    NSString *input_url2 = [NSString stringWithUTF8String:input_url.c_str()];
    NSURL *url = [NSURL URLWithString:input_url2];
    [[UIApplication sharedApplication] openURL:url];
    // no need to release anything
  }
  double getTime() const override {
    double t = [[NSProcessInfo processInfo] systemUptime];
    return t;
  }
  bool hasActiveModal() const { return has_active_modal; }
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
                            [view dismissOpenGLViewAnimated:YES completion:nil];                          
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
  
private:
  NSUserDefaults *defaults;
  bool has_active_modal = false;
  OpenGLView * controller;
};

extern FWContextBase * esMain(FWPlatformBase * platform);

@interface OpenGLView ()
{
  FWiOSPlatform * _platform;
  FWContextBase * _esContext;
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

//- (void)setupGL;
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
 #ifdef __APPLE__
  cerr << "creating renderbuffers and framebuffers (" << drawableWidth << " " << drawableHeight << ")" << endl;

  if (framebuffer) glDeleteFramebuffers(1, &framebuffer);
  if (depth) glDeleteRenderbuffers(1, &depth);
  if (stencil) glDeleteRenderbuffers(1, &stencil);
  if (color) glDeleteRenderbuffers(1, &color);

  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  current_framebuffer = framebuffer;
  
#if 0
  glGenRenderbuffers(1, &depth);
  glBindRenderbuffer(GL_RENDERBUFFER, depth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, drawableWidth, drawableHeight);
#endif
  
#if 1
  glGenRenderbuffers(1, &stencil);
  glBindRenderbuffer(GL_RENDERBUFFER, stencil);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, drawableWidth, drawableHeight);
#endif
  
  glGenRenderbuffers(1, &color);
  glBindRenderbuffer(GL_RENDERBUFFER, color);
  CAEAGLLayer* eaglLayer = (CAEAGLLayer*) self.layer;
#if 0	
  eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking,
                                kEAGLColorFormatRGB565, kEAGLDrawablePropertyColorFormat,
                                nil];
#endif

  [context renderbufferStorage:GL_RENDERBUFFER fromDrawable: eaglLayer];

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color);
#if 0
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
#endif
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencil);
  
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
#endif

}

- (id) initWithFrame: (CGRect) frame
{
    if (self = [super initWithFrame:frame])
    {
	[self setMultipleTouchEnabled:YES];

        // _display_scale = [[UIScreen mainScreen] scale];
	float scale = [[UIScreen mainScreen] scale];

        CAEAGLLayer* eaglLayer = (CAEAGLLayer*) self.layer;
        eaglLayer.opaque = YES;
        eaglLayer.contentsScale = scale;
      
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
            [self release];
            return nil;
        }
      
        self->drawableWidth = int(CGRectGetWidth(frame) * scale);
        self->drawableHeight = int(CGRectGetHeight(frame) * scale);
        // m_applicationEngine->Initialize(width, height);

        _platform = new FWiOSPlatform(self, scale, 1 ? "#version 100" : "#version 300 es", _has_es3);
        _esContext = esMain(_platform);
      
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

  // self.preferredFramesPerSecond = 60;
    
  // GLKView *view = (GLKView *)self.view;
  // view.drawableDepthFormat = GLKViewDrawableDepthFormat24; // GLKViewDrawableDepthFormatNone;
  // view.drawableMultisample = GLKViewDrawableMultisample4X;
  if (!self->_has_es3) {
    // view.drawableColorFormat = GLKViewDrawableColorFormatRGB565;
  }
  // view.drawableStencilFormat = GLKViewDrawableStencilFormat8;
  // view.enableSetNeedsDisplay = YES;
    
//  [self setupGL];
}

- (void)viewDidAppear:(BOOL)animated {
  // [super viewDidAppear:animated];
  [self becomeFirstResponder];
}

- (void)viewWillDisappear:(BOOL)animated {
  [self resignFirstResponder];
  [super viewWillDisappear:animated];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
  [touches enumerateObjectsUsingBlock:^(id obj, BOOL *stop) {
    UITouch *touch = obj;
    long long id = (long long)touch;
    CGPoint touchPoint = [touch locationInView:self];
    need_update |= _esContext->touchesEnded(touchPoint.x, touchPoint.y, event.timestamp, id);
  }];
  need_update |= _esContext->flushTouches(3, event.timestamp);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  if (_platform->hasActiveModal()) return;
  [touches enumerateObjectsUsingBlock:^(id obj, BOOL *stop) {
    // Get a single touch and it's location
    UITouch *touch = obj;
    long long id = (long long)touch;
    CGPoint touchPoint = [touch locationInView:self];
    need_update |= _esContext->touchesMoved(touchPoint.x, touchPoint.y, event.timestamp, id);
  }];
  need_update |= _esContext->flushTouches(2, event.timestamp);
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
  if (_platform->hasActiveModal()) return;
  // Enumerate over all the touches
  [touches enumerateObjectsUsingBlock:^(id obj, BOOL *stop) {
    // Get a single touch and it's location
    UITouch *touch = obj;
    long long id = (long long)touch;
    CGPoint touchPoint = [touch locationInView:self];
    need_update |= _esContext->touchesBegin(touchPoint.x, touchPoint.y, event.timestamp, id);
  }];
  need_update |= _esContext->flushTouches(1, event.timestamp);
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
	need_update |= _esContext->onShake();
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
  [super dealloc];
}

- (void)didReceiveMemoryWarning {
    cerr << "didReceiveMemoryWarning called" << endl;

#if 0
  if ([self isViewLoaded] && ([[self view] window] == nil)) {
    self.view = nil;
    [self tearDownGL];
        
    if ([EAGLContext currentContext] == self.context) {
      EAGLContext setCurrentContext:nil];
    }
    self.context = nil;
  }
#endif

  _esContext->onMemoryWarning();

  [super didReceiveMemoryWarning];
}

- (void)tearDownGL {
  [EAGLContext setCurrentContext:self->context];
  _esContext->onShutdown();
  delete _esContext;
}

- (void) drawView: (CADisplayLink*) displayLink
{
  [EAGLContext setCurrentContext:self->context];
  need_update |= _esContext->loadEvents();
  double t = [[NSProcessInfo processInfo] systemUptime];
  need_update |= _esContext->onUpdate(t);
  if (need_update) {
    [self drawView2];
  }
}

- (void) drawView2 {
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
  
  drawableWidth = int(logical_width * self.contentScaleFactor);
  drawableHeight = int(logical_height * self.contentScaleFactor);
  
  if (drawableWidth != _esContext->getActualWidth() || drawableHeight != _esContext->getActualHeight()) {
    cerr << "resize window: " << logical_width << " " << logical_height << endl;
    _esContext->onResize(logical_width, logical_height, drawableWidth, drawableHeight);
  }
  _esContext->onDraw();

#if 0
  const GLenum discards[] = { GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT };
  glBindFramebuffer(GL_FRAMEBUFFER, current_framebuffer);
  glDiscardFramebufferEXT(GL_FRAMEBUFFER, 2, discards);
#endif
    
  [context presentRenderbuffer:GL_RENDERBUFFER];
  need_update = false;
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
