#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

@interface OpenGLView : UIView

- (void) drawView: (CADisplayLink*) displayLink;
- (void) createFBO: (int) flags;

@end
