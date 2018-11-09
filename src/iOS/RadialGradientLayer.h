#import <UIKit/UIKit.h>

@interface RadialGradientLayer : CALayer
- (instancetype)init;
- (void)drawInContext:(CGContextRef)ctx;

@property (nonatomic, strong) UIColor * color1;
@property (nonatomic, strong) UIColor * color2;
@end
