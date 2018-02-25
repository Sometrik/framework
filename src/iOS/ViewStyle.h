#import <UIKit/UIKit.h>

@interface ViewStyle : NSObject
- (void)apply:(UIView *)view animate:(BOOL)animate;

@property NSInteger fontSize;
@property NSInteger fontWeight;
@property float alpha;
@property float zoom;
@property float shadow;
@property UIColor * color;
@property UIColor * backgroundColor;
@property CAGradientLayer *gradient;

@end
