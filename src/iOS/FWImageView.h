#import <UIKit/UIKit.h>

@interface FWImageView : UIImageView
- (void)layoutSubviews;
- (void)addImageUrl:(NSString *)url width:(int)width height:(int)height;

@property NSString * url;
@end
