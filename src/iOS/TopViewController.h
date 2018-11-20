#import <UIKit/UIKit.h>
#import "ImageCache.h"

@interface TopViewController : UIViewController
- (void)createWebBrowserWithUrl:(NSString *)url;
- (void)bringWebviewToFront;

@property (nonatomic, strong) ImageCache * imageCache;
@end
