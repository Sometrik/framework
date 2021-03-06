#import <UIKit/UIKit.h>  

#import "TopViewController.h"
#import "ViewController.h"

#if 0
@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property ( strong, nonatomic ) UIWindow *window;
@property ( strong, nonatomic ) ViewController *view;

#else
@interface AppDelegate : NSObject <UIApplicationDelegate> {
@private
  UIWindow* window;
  ViewController* viewController;
  TopViewController* topViewController;
}

#endif

@end
