#import <UIKit/UIKit.h>

@interface FWImageView : UIImageView
- (void)layoutSubviews;
- (void)addImageUrl: (int)viewId width:(int)width height:(int)height url:(NSString *)url;

@property NSString * url;
@end
