#import "FWScrollView.h"

@implementation FWScrollView

- (instancetype)init
{
    if (self = [super init]) {
    }
    return self;
}

- (instancetype)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]) {
        self.frame = frame;
    }
    return self;
}

- (void)layoutSubviews {
    [super layoutSubviews];

    CGRect contentRect = CGRectZero;
    for (UIView *view in self.subviews) {
	contentRect = CGRectUnion(contentRect, view.frame);
    }
    self.contentSize = CGSizeMake(self.frame.width, contentRect.size.height);
}

@end
