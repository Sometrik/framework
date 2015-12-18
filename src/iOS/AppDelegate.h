#include "OpenGLView.h"
// #include "ViewController.h"

#if 0
@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property ( strong, nonatomic ) UIWindow *window;
@property ( strong, nonatomic ) ViewController *view;

#else
@interface AppDelegate : NSObject <UIApplicationDelegate> {
@private
  UIWindow* window;
  OpenGLView* view;
  UIViewController* controller;
}

#endif

@end
