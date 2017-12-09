#import "FWImageView.h"

@implementation FWImageView

- (void)layoutSubviews {
    [super layoutSubviews];
    NSLog(@"FWImageView, layoutSubview(): %f %f", self.frame.size.width, self.frame.size.height);
}

@end
