#import <UIKit/UIKit.h>
@class FWImageView;

@protocol FWImageViewDelegate <NSObject>
- (void)fwImageView:(FWImageView *)imageView didChangeSize:(CGSize)size ofImageUrl:(NSString *)url;
- (void)didCancelImageRequest:(FWImageView *)imageView;
@end

@interface FWImageView : UIImageView
- (void)layoutSubviews;
- (void)addImageUrl:(NSString *)url width:(int)width height:(int)height;
- (void)clear;
- (void)releaseData;
- (void)updateContentMode;
- (void)updateVisibility:(CGRect)bounds;

@property (nonatomic, assign) NSInteger prevWidth;
@property (nonatomic, assign) NSInteger prevHeight;
@property (nonatomic, assign) BOOL imageRequestPending;
@property (nonatomic, assign) id<FWImageViewDelegate> delegate;
@end
