#import "FrameLayoutView.h"

@implementation FrameLayoutView

- (void)layoutSubviews {
    LayoutParams * myParams = nil;
    if ([self.superview isKindOfClass:FWLayoutView.class]) {
        FWLayoutView * layout = (FWLayoutView*)self.superview;
        myParams = [layout findParams:self];
    }

    CGFloat paddingLeft = 0, paddingRight = 0, paddingTop = 0, paddingBottom = 0;
    if (myParams != nil) {
      paddingLeft = myParams.padding.left;
      paddingRight = myParams.padding.right;
      paddingTop = myParams.padding.top;
      paddingBottom = myParams.padding.bottom;
    }

    for (LayoutParams *item in self.items) {
        if (item.view.hidden) continue;

        if (item.fixedWidth == -1) { // match-parent
            item.leftConstraint.constant = paddingLeft + item.margin.left;
            item.rightConstraint.constant = -(paddingRight + item.margin.right);

            item.leftConstraint.active = YES;
            item.rightConstraint.active = YES;
            item.centerXConstraint.active = NO;
            item.widthConstraint.active = NO;
            item.maxWidthConstraint.active = NO;
        } else {
            item.maxWidthConstraint.constant = -(paddingLeft + item.margin.left + paddingRight + item.margin.right);
            item.maxWidthConstraint.active = YES;

            CGFloat width;
            if (item.fixedWidth > 0) {
                width = item.fixedWidth;
            } else {
                width = [self calcIntrinsicWidth:item.view] + item.padding.left + item.padding.right;
            }

            item.widthConstraint.constant = width;
            item.widthConstraint.active = YES;

            if (item.horizontalAlignment == LinearLayoutItemHorizontalAlignmentRight) {
                item.rightConstraint.constant = -(paddingRight + item.margin.left);

                item.leftConstraint.active = NO;
                item.rightConstraint.active = YES;
                item.centerXConstraint.active = NO;
            } else if (item.horizontalAlignment == LinearLayoutItemHorizontalAlignmentLeft) {
                item.leftConstraint.constant = paddingLeft + item.margin.left;

                item.leftConstraint.active = YES;
                item.rightConstraint.active = NO;
                item.centerXConstraint.active = NO;
            } else {
                item.leftConstraint.active = NO;
                item.rightConstraint.active = NO;
                item.centerXConstraint.active = YES;
            }
        }

        if (item.fixedHeight == -1) { // match-parent
	    item.topConstraint.constant = paddingTop + item.margin.top;
	    item.bottomConstraint.constant = -(paddingBottom + item.margin.bottom);

	    item.topConstraint.active = YES;
	    item.bottomConstraint.active = YES;
	    item.centerYConstraint.active = NO;
	    item.heightConstraint.active = NO;
	    item.maxHeightConstraint.active = NO;
        } else {
#if 0
	    item.maxHeightConstraint.constant = -(paddingTop + item.margin.top + paddingBottom + item.margin.bottom);
	    item.maxHeightConstraint.active = YES;
#endif

	  CGFloat height;
	  if (item.fixedHeight > 0) {
            height = item.fixedHeight;
	  } else {
	    height = [self calcIntrinsicHeight:item.view] + item.padding.left + item.padding.right;
	  }

	  item.heightConstraint.constant = height;
	  item.heightConstraint.active = YES;

	  if (item.verticalAlignment == LinearLayoutItemVerticalAlignmentBottom) {
	    item.bottomConstraint.constant = -(paddingBottom + item.margin.bottom);
	    item.topConstraint.active = NO;
	    item.bottomConstraint.active = YES;
	    item.centerYConstraint.active = NO;
	  } else if (item.verticalAlignment == LinearLayoutItemVerticalAlignmentTop) {
	    item.topConstraint.constant = paddingTop + item.margin.top;
	    item.topConstraint.active = YES;
	    item.bottomConstraint.active = NO;
	    item.centerYConstraint.active = NO;
	  } else {
	    item.topConstraint.active = NO;
	    item.bottomConstraint.active = NO;
	    item.centerYConstraint.active = YES;
	  }
	}
    }

    for (CALayer * layer in self.layer.sublayers) {
      layer.frame = self.bounds;
    }
}

@end

