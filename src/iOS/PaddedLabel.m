#import "PaddedLabel.h"

#import "FWLayoutView.h"

@implementation PaddedLabel

- (id)init {
    self = [super init];
    if (self) {
        self.edgeInsets = UIEdgeInsetsMake(0, 0, 0, 0);
    }
    return self;
}

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        self.edgeInsets = UIEdgeInsetsMake(0, 0, 0, 0);
    }
    return self;
}

- (void)drawTextInRect:(CGRect)rect {
  [super drawTextInRect:UIEdgeInsetsInsetRect(rect, self.edgeInsets)];
}

- (void)setBounds:(CGRect)bounds {
  int adjustedBoundsWidth = bounds.size.width - (self.edgeInsets.left + self.edgeInsets.right + 2);
  if (adjustedBoundsWidth < self.preferredMaxLayoutWidth) {
      NSLog(@"Bounds changed %f => %f", self.bounds.size.width, bounds.size.width);
    [super setBounds:bounds];
#if 0
    [self setNeedsUpdateConstraints];
#else
    self.preferredMaxLayoutWidth = adjustedBoundsWidth;
    [self relayoutAll];
#endif
  } else {
    [super setBounds:bounds];
  }
}

- (void)relayoutAll {
    UIView * view = self.superview;
    while (view != nil && ([view isKindOfClass:FWLayoutView.class] || [view isKindOfClass:UIScrollView.class])) {
        [view setNeedsLayout];
	if ([view isKindOfClass:UIScrollView.class]) {
	  break;
	}
        view = view.superview;
    }
}

#if 0
- (void)updateConstraints {
  if (self.preferredMaxLayoutWidth != self.bounds.size.width) {
    self.preferredMaxLayoutWidth = self.bounds.size.width;
  }
  [super updateConstraints];
}
#endif

#if 0
- (void)layoutSubviews {
    [super layoutSubviews];
    self.preferredMaxLayoutWidth = self.frame.size.width;
    [self.superview layoutIfNeeded];
}
#endif

- (CGSize)intrinsicContentSize
{
  CGSize size = [super intrinsicContentSize];
  size.width  += self.edgeInsets.left + self.edgeInsets.right + 2;
  size.height += self.edgeInsets.top + self.edgeInsets.bottom + 2;
  return size;
}

@end
