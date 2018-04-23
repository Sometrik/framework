#import "LinearLayoutView.h"

@implementation LinearLayoutView

- (id)init {
    self = [super init];
    if (self) {
        self.orientation = LinearLayoutViewOrientationVertical;
    }
    return self;
}

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        self.orientation = LinearLayoutViewOrientationVertical;
    }
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithCoder:aDecoder];
    if (self) {
        self.orientation = LinearLayoutViewOrientationVertical;
    }
    return self;
}

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

    CGFloat relativePosition = 0.0;
    CGFloat extraSpace = 0.0;
    CGFloat totalWeight = 0.0;

    if (self.orientation == LinearLayoutViewOrientationVertical) {
        extraSpace = self.frame.size.height - paddingTop - paddingBottom;
        relativePosition = paddingTop;
    } else {
        extraSpace = self.frame.size.width - paddingLeft - paddingRight;
        relativePosition = paddingLeft;
    }
    
    for (LayoutParams *item in self.items) {
        if (item.view.hidden) {
            continue;
        }
        int size;
        if (self.orientation == LinearLayoutViewOrientationVertical) {
            if (item.fixedHeight > 0) {
                size = item.fixedHeight + item.margin.top + item.margin.bottom;
            } else {
                size = [self calcIntrinsicHeight:item.view] + item.margin.top + item.margin.bottom + item.padding.top + item.padding.bottom;
            }
        } else {
            if (item.fixedWidth > 0) {
                size = item.fixedWidth + item.margin.left + item.margin.right;
            } else {
                size = [self calcIntrinsicWidth:item.view] + item.margin.left + item.margin.right + item.padding.left + item.padding.right;
            }
        }
        extraSpace -= size;
	totalWeight += item.weight;
    }
    
    for (LayoutParams *item in self.items) {
        if (item.view.hidden) {
            continue;
        }

        CGFloat startMargin = 0.0;
        CGFloat endMargin = 0.0;
        CGFloat currentDimension = 0.0;

        CGFloat minWidth, minHeight;

        if (item.fixedWidth > 0) {
            minWidth = item.fixedWidth;
        } else {
            minWidth = [self calcIntrinsicWidth:item.view] + item.padding.left + item.padding.right;
        }
        if (item.fixedHeight > 0) {
            minHeight = item.fixedHeight;
        } else {
            minHeight = [self calcIntrinsicHeight:item.view] + item.padding.top + item.padding.bottom;
        }

	CGFloat itemExtraSpace = 0;
	if (totalWeight > 0) {
	    itemExtraSpace = item.weight * extraSpace / totalWeight;
	}

        if (self.orientation == LinearLayoutViewOrientationHorizontal) {
            minWidth += itemExtraSpace;

            startMargin = item.margin.left;
            endMargin = item.margin.right;
                    
            relativePosition += startMargin;

            item.leftConstraint.constant = relativePosition;
            item.widthConstraint.constant = minWidth;
            item.leftConstraint.active = YES;
            item.widthConstraint.active = YES;
            item.rightConstraint.active = NO;
            item.centerXConstraint.active = NO;
	    
            if (item.fixedHeight == -1) {
                item.topConstraint.constant = paddingTop + item.margin.top;
                item.bottomConstraint.constant = -(paddingBottom + item.margin.bottom);
                item.topConstraint.active = YES;
                item.bottomConstraint.active = YES;
                item.centerYConstraint.active = NO;
                item.heightConstraint.active = NO;
                item.maxHeightConstraint.active = NO;
            } else {
                item.maxHeightConstraint.constant = -(paddingTop + item.margin.top + paddingBottom + item.margin.bottom);
                item.maxHeightConstraint.active = YES;

                item.heightConstraint.constant = minHeight;
                item.heightConstraint.active = YES;

                if (item.verticalAlignment == LinearLayoutItemVerticalAlignmentTop) {
                    item.topConstraint.constant = paddingTop + item.margin.top;
                    item.topConstraint.active = YES;
                    item.bottomConstraint.active = NO;
                    item.centerYConstraint.active = NO;
                } else if (item.verticalAlignment == LinearLayoutItemVerticalAlignmentBottom) {
                    item.bottomConstraint.constant = -(paddingBottom + item.margin.bottom);
                    item.topConstraint.active = NO;
                    item.bottomConstraint.active = YES;
                    item.centerYConstraint.active = NO;
                } else { // LinearLayoutItemVerticalCenter
                    // absolutePosition = (self.frame.size.height / 2) - ((height + (item.margin.bottom - item.margin.top)) / 2);
                    item.topConstraint.active = NO;
                    item.bottomConstraint.active = NO;
                    item.centerYConstraint.active = YES;
                }
	        }

            currentDimension = minWidth;
        } else {           
            minHeight += itemExtraSpace;

            startMargin = item.margin.top;
            endMargin = item.margin.bottom;
            
            relativePosition += startMargin;

            item.topConstraint.constant = relativePosition;
            item.heightConstraint.constant = minHeight;
            item.topConstraint.active = YES;
            item.bottomConstraint.active = NO;
            item.centerYConstraint.active = NO;
            item.heightConstraint.active = YES;
            
            if (item.fixedWidth == -1) {
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

                item.widthConstraint.constant = minWidth;
                item.widthConstraint.active = YES;

                if (item.horizontalAlignment == LinearLayoutItemHorizontalAlignmentLeft) {
                    item.leftConstraint.constant = paddingLeft + item.margin.left;
                    item.leftConstraint.active = YES;
                    item.rightConstraint.active = NO;
                    item.centerXConstraint.active = NO;
                } else if (item.horizontalAlignment == LinearLayoutItemHorizontalAlignmentRight) {
                    item.rightConstraint.constant = -(paddingRight + item.margin.right);
                    item.leftConstraint.active = NO;
                    item.rightConstraint.active = YES;
                    item.centerXConstraint.active = NO;
                } else { // LinearLayoutItemHorizontalCenter
                    // absolutePosition = (self.frame.size.width / 2) - ((width + (item.margin.right - item.margin.left)) / 2);
                    item.leftConstraint.active = NO;
                    item.rightConstraint.active = NO;
                    item.centerXConstraint.active = YES;
                }
	        }
            currentDimension = minHeight;
        }

        relativePosition += currentDimension + endMargin;
    }

    for (CALayer * layer in self.layer.sublayers) {
      layer.frame = self.bounds;
    }
}

@end

