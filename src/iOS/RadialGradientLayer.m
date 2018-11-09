#import <RadialGradientLayer.h>

- (instancetype)init
{
    self = [super init];
    if (self) {
        [self setNeedsDisplay];
    }
    return self;
}

- (void)drawInContext:(CGContextRef)ctx {
    CGFloat red1 = 0.0, green1 = 0.0, blue1 = 0.0, alpha1 = 0.0;
    CGFloat red2 = 0.0, green2 = 0.0, blue2 = 0.0, alpha2 = 0.0;
    [self.color1 getRed:&red1 green:&green1 blue:&blue1 alpha:&alpha1];
    [self.color2 getRed:&red2 green:&green2 blue:&blue2 alpha:&alpha2];
     
    size_t gradLocationsNum = 2;
    CGFloat gradLocations[2] = { 0.0f, 1.0f };
    CGFloat gradColors[8] = { red1, green1, blue1, alpha1, red2, green2, blue2, alpha2 };
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGGradientRef gradient = CGGradientCreateWithColorComponents(colorSpace, gradColors, gradLocations, gradLocationsNum);
    CGColorSpaceRelease(colorSpace);

    CGPoint gradCenter = CGPointMake(CGRectGetMidX(self.bounds), CGRectGetMidY(self.bounds));
    CGFloat gradRadius = MIN(self.bounds.size.width, self.bounds.size.height) ;

    CGContextDrawRadialGradient(ctx, gradient, gradCenter, 0, gradCenter, gradRadius, kCGGradientDrawsAfterEndLocation);

    CGGradientRelease(gradient);
}
