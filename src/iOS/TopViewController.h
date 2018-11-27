#import <UIKit/UIKit.h>
#import "ImageCache.h"

@interface TopViewController : UIViewController
- (void)createWebBrowserWithUrl:(NSString *)url;
- (void)showTabBar:(UITabBar *)tabBar;
- (void)showNavBar:(UINavigationBar *)navBar;
- (void)showSearchBar:(UISearchBar *)searchBar;
- (void)showKeyboard:(BOOL)visible height:(NSInteger)height;
- (void)bringWebviewToFront;
- (BOOL)isTabBarVisible;
- (BOOL)isNavBarVisible;
- (BOOL)isSearchBarVisible;
- (BOOL)isKeyboardVisible;
- (NSInteger)getKeyboardHeight;

@property (nonatomic, strong) ImageCache * imageCache;
@property (nonatomic, strong) UITabBar * visibleTabBar;
@property (nonatomic, strong) UINavigationBar * visibleNavBar;
@property (nonatomic, strong) UISearchBar * visibleSearchBar;
@end
