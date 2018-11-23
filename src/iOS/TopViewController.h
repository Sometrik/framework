#import <UIKit/UIKit.h>
#import "ImageCache.h"

@interface TopViewController : UIViewController
- (void)createWebBrowserWithUrl:(NSString *)url;
- (void)showTabBar:(UITabBar *)tabBar;
- (void)bringWebviewToFront;

@property (nonatomic, strong) ImageCache * imageCache;
@property (nonatomic, strong) UITabBar * visibleTabBar;
@end
