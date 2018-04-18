#import "DialogView.h"

@implementation DialogView

- (void)layoutSubviews {
    self.heightConstraint.constant = [self calcIntrinsicHeight:self];
    [super layoutSubviews];
}

@end
