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
    }
    return self;
}

- (void)layoutSubviews {
    int height = 4000;
    if ([[self subviews] count] >= 1) {
        UIView * view = [[self subviews] lastObject];
        // height = [self calcIntrinsicHeight:view];
    }

    self.heightConstraint.constant = height;

    [super layoutSubviews];

    self.contentSize = CGSizeMake(self.superview.frame.size.width, height);
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
    } else {
        return view.intrinsicContentSize.height;
    }
}

@end
