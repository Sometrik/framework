#import <UIKit/UIKit.h>

@interface ViewStyle : NSObject
- (void)apply:(UIView *)view animate:(BOOL)animate;

@property (nonatomic, assign) NSInteger fontSize;
@property (nonatomic, assign) NSInteger fontWeight;
@property (nonatomic, assign) float alpha;
@property (nonatomic, assign) float zoom;
@property (nonatomic, assign) float shadow;
@property (nonatomic, strong) UIColor * color;
@property (nonatomic, strong) UIColor * backgroundColor;
@property (nonatomic, strong) UIColor * tintColor;
@property (nonatomic, strong) CAGradientLayer * gradient;
@property (nonatomic, assign) int paddingTop;
@property (nonatomic, assign) int paddingRight;
@property (nonatomic, assign) int paddingBottom;
@property (nonatomic, assign) int paddingLeft;
@property (nonatomic, assign) int borderRadius;
@property (nonatomic, assign) int borderWidth;
@property (nonatomic, strong) UIColor * borderColor;
@end
