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
@property UIColor * tintColor;
@property CAGradientLayer *gradient;
@property int paddingTop;
@property int paddingRight;
@property int paddingBottom;
@property int paddingLeft;
@property int borderRadius;
@property int borderWidth;
@property UIColor * borderColor;
@property int contentInsetTop;
@property int contentInsetBottom;
@end
