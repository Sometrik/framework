#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

@interface OpenGLView : UIView

- (void) drawView: (CADisplayLink*) displayLink;
- (void) drawView2;
- (void) createFBO: (int) flags;
- (void) startRenderLoop;
- (void) stopRenderLoop;
- (void) sendMemoryWarning;

@end
