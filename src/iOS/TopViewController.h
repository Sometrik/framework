#import <UIKit/UIKit.h>
#import "ImageCache.h"

@interface TopViewController : UIViewController
- (void)createWebBrowserWithUrl:(NSString *)url;
- (void)showTabBar:(UITabBar *)tabBar;
- (void)showNavBar:(UINavigationBar *)navBar;
- (void)showSearchBar:(UISearchBar *)searchBar;
- (void)bringWebviewToFront;

@property (nonatomic, strong) ImageCache * imageCache;
@property (nonatomic, strong) UITabBar * visibleTabBar;
@property (nonatomic, strong) UINavigationBar * visibleNavBar;
@property (nonatomic, strong) UISearchBar * visibleSearchBar;
@end
