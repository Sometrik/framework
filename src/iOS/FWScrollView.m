#import "FWScrollView.h"

#import "FrameLayoutView.h"
#import "LinearLayoutView.h"

@implementation FWScrollView

- (instancetype)init
{
    if (self = [super init]) {
        self.topConstraint = nil;
        self.leftConstraint = nil;
        self.widthConstraint = nil;
        self.heightConstraint = nil;
        self.translatesAutoresizingMaskIntoConstraints = false;
        self.currentPage = 0;
    }
    return self;
}

- (instancetype)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]) {
        self.frame = frame;
        self.topConstraint = nil;
        self.leftConstraint = nil;
        self.widthConstraint = nil;
        self.heightConstraint = nil;
        self.translatesAutoresizingMaskIntoConstraints = false;
        self.currentPage = 0;
    }
    return self;
}

- (void)layoutSubviews {
    if (self.pagingEnabled) {
        int numChildren = 0;
        for (UIView * view in [self subviews]) {
            if ([view isKindOfClass:UIImageView.class]) continue; // ignore scroll indicators
            numChildren++;
        }
        self.contentSize = CGSizeMake(numChildren * self.frame.size.width, self.frame.size.height);
    } else {
        int width = self.frame.size.width;
        int height = 0;

        for (UIView * view in [self subviews]) {
            if ([view isKindOfClass:UIImageView.class]) continue; // ignore scroll indicators
            height = [self calcIntrinsicHeight:view];
            break;
        }

        self.widthConstraint.constant = width;
        self.heightConstraint.constant = height;

        self.contentSize = CGSizeMake(width, height);
    }

    [super layoutSubviews];
}

- (int)calcIntrinsicHeight:(UIView *)view {
    if ([view isKindOfClass:LinearLayoutView.class]) {
        LinearLayoutView * child = (LinearLayoutView*)view;
        int height = 0;
        if (child.orientation == LinearLayoutViewOrientationVertical) {
            for (LayoutParams *item in child.items) {
                if (item.view.hidden) continue;
                
                height += item.margin.top + item.margin.bottom;
                if (item.fixedHeight > 0) {
                    height += item.fixedHeight;
                } else {
                    height += [self calcIntrinsicHeight:item.view] + item.padding.top + item.padding.bottom;
                }
            }
        } else {
            for (LayoutParams *item in child.items) {
                if (item.view.hidden) continue;

                int h = 0;
                if (item.fixedHeight > 0) {
                    h = item.fixedHeight + item.margin.top + item.margin.bottom;
                } else {
                    h = [self calcIntrinsicHeight:item.view] + item.padding.top + item.padding.bottom + item.margin.top + item.margin.bottom;
                }
                if (h > height) height = h;
            }
        }
        return height;
    } else if ([view isKindOfClass:FrameLayoutView.class]) {
        FrameLayoutView * child = (FrameLayoutView*)view;
        int height = 0;
        for (LayoutParams *item in child.items) {
            if (item.view.hidden) continue;

            int h = 0;
            if (item.fixedHeight > 0) {
                h = item.fixedHeight + item.margin.top + item.margin.bottom;
            } else {
                h = [self calcIntrinsicHeight:item.view] + item.padding.top + item.padding.bottom + item.margin.top + item.margin.bottom;
            }
            if (h > height) height = h;
        }
        return height;
    } else if ([view isKindOfClass:FWScrollView.class]) {
        for (UIView * subview in [view subviews]) {
	    if ([subview isKindOfClass:UIImageView.class]) continue; // ignore scroll indicators
	    return [self calcIntrinsicHeight:subview];
	}
	return 0;
    } else {
        return view.intrinsicContentSize.height;
    }
}

- (void)updateVisibility:(CGRect)bounds
{
    for (UIView * subview in [self subviews]) {
        if ([subview isKindOfClass:FWLayoutView.class]) {
            FWLayoutView * layout = (FWLayoutView*)subview;
            [layout updateVisibility:bounds];
        }
    }
}

- (void)addChildConstraints:(UIView *)view position:(int)position pageWidth:(int)pageWidth
{
    NSLayoutConstraint *leftConstraint;
    NSLayoutConstraint *topConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
    NSLayoutConstraint *widthConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeWidth multiplier:1.0f constant:0];
    NSLayoutConstraint *heightConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeHeight multiplier:1.0f constant:0];

    if (position == 0) {
        leftConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
    } else {
#if 0
        leftConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:[[parentView subviews] lastObject] attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
#else
        leftConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:(position * pageWidth)];
#endif
    }

    topConstraint.priority = 999; // - viewManager.level;
    leftConstraint.priority = 999; // - viewManager.level;
    widthConstraint.priority = 999; // - viewManager.level;
    heightConstraint.priority = 999; // - viewManager.level;
    [view.superview addConstraints:@[topConstraint, leftConstraint, widthConstraint, heightConstraint]];
}

- (void)rebuildConstraints:(int)pageWidth
{
    for (NSLayoutConstraint *c in self.constraints) {
        [self removeConstraint:c];
    }
    int pos = 0;
    for (UIView * subview in [self subviews]) {
        if ([subview isKindOfClass:UIImageView.class]) continue; // ignore scroll indicators
        [self addChildConstraints:subview position:pos pageWidth:pageWidth];
        pos++;
    }
}

- (NSInteger)indexForVisiblePage
{
    CGRect frame = self.bounds;
    return (frame.origin.x + frame.size.width / 2) / frame.size.width;
}

@end
