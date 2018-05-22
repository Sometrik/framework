#import "ViewStyle.h"

#include <math.h>
#import "PaddedLabel.h"

#define EPSILON		0.001

@implementation ViewStyle

- (id)init {
    self = [super init];
    if (self) {
        self.alpha = 1.0f;
        self.zoom = 1.0f;
        self.fontSize = 0;
        self.fontWeight = 0;
        self.shadow = 0;
	self.color = nil;
        self.backgroundColor = nil;
	self.tintColor = nil;
        self.gradient = nil;
        self.paddingTop = 0;
        self.paddingRight = 0;
        self.paddingBottom = 0;
        self.paddingLeft = 0;
        self.borderRadius = -1;
        self.borderWidth = -1;
        self.contentInsetTop = 0;
        self.contentInsetBottom = 0;
    }
    return self;
}

- (void)apply:(UIView *)view animate:(BOOL)animate {
    if ([view isKindOfClass:PaddedLabel.class]) {
        PaddedLabel * label = (PaddedLabel*)view;
	if (label.autolink || label.markdown) {
	  label.defaultFont = [self createFont:label.font];
	  label.boldFont = [self createBoldFont:label.font];
	  label.defaultColor = self.color;
	  label.attributedText = [label createAttributedString:label.origText];
	} else {
	  if ([self isFontDefined]) {
            label.font = [self createFont:label.font];
	  }
	  if (self.color != nil) label.textColor = self.color;
	}
        label.edgeInsets = UIEdgeInsetsMake(self.paddingTop, self.paddingLeft, self.paddingBottom, self.paddingRight);
    } else if ([view isKindOfClass:UIButton.class]) {
        UIButton * button = (UIButton*)view;
        if ([self isFontDefined]) {
            button.titleLabel.font = [self createFont:button.titleLabel.font];
        }
        if (self.color != nil) [button setTitleColor:self.color forState:UIControlStateNormal];
        button.contentEdgeInsets = UIEdgeInsetsMake(self.paddingTop, self.paddingLeft, self.paddingBottom, self.paddingRight);
    } else if ([view isKindOfClass:UIScrollView.class]) {
        UIScrollView * scrollView = (UIScrollView*)view;
        if (self.contentInsetTop != scrollView.contentInset.top ||
            self.contentInsetBottom != scrollView.contentInset.bottom) {
            scrollView.contentInset = UIEdgeInsetsMake(self.contentInsetTop, 0, self.contentInsetBottom, 0);
            scrollView.contentOffset = CGPointMake(0, -self.contentInsetTop);
        }
    }
    
    if (self.backgroundColor != nil) {
        view.layer.backgroundColor = self.backgroundColor.CGColor;
    }

    if (self.tintColor != nil) {
        view.tintColor = self.tintColor;
    }

    if (self.borderRadius != -1) {
        view.layer.cornerRadius = self.borderRadius;
        if ([view isKindOfClass:UIImageView.class]) {
            view.layer.masksToBounds = self.borderRadius > 0;
        }
    }

    if (self.borderWidth != -1) {
        view.layer.borderWidth = self.borderWidth;
    }

    if (self.borderColor != nil) {
        view.layer.borderColor = self.borderColor.CGColor;
    }

    if (self.shadow > 0) {
        view.layer.shadowOpacity = 0.5;
        // self.view.layer.masksToBounds = NO;
        view.layer.shadowOffset = CGSizeMake(0, 0);
        view.layer.masksToBounds = FALSE;
    }

    if (animate) {
      if (fabs(view.layer.shadowRadius - self.shadow) > EPSILON) {
#if 1
            CABasicAnimation* animation = [CABasicAnimation animationWithKeyPath:@"shadowRadius"];
            animation.fromValue = [NSNumber numberWithFloat:view.layer.shadowRadius];
            animation.toValue = [NSNumber numberWithFloat:self.shadow];
            animation.duration = 0.3;
            [view.layer addAnimation:animation forKey:@"shadowRadius"];
            view.layer.shadowRadius = self.shadow;
#else
            [CATransaction begin];
            [CATransaction setAnimationDuration:0.3];
            [CATransaction setAnimationTimingFunction:[CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut] ];
            view.layer.shadowRadius = self.shadow;
            [CATransaction commit];
#endif
        }
	if (fabs(view.alpha - self.alpha) > EPSILON) {
	  [UIView animateWithDuration:0.3 animations:^{
	      view.alpha = self.alpha;
	      view.transform = CGAffineTransformMakeScale(self.zoom, self.zoom);
	    }];
	}
    } else {
        view.alpha = self.alpha;
        view.transform = CGAffineTransformMakeScale(self.zoom, self.zoom);
        view.layer.shadowRadius = self.shadow;
    }
}

- (UIFont *)createFont:(UIFont *)currentFont {
    NSInteger size = self.fontSize > 0 ? self.fontSize : currentFont.pointSize;
    if (self.fontWeight) {
        UIFontDescriptor * fontD = [currentFont.fontDescriptor fontDescriptorWithSymbolicTraits:UIFontDescriptorTraitBold];
        return [UIFont fontWithDescriptor:fontD size:size];
    } else {
        return [currentFont fontWithSize:size];
    }
}

- (UIFont *)createBoldFont:(UIFont *)currentFont {
    NSInteger size = self.fontSize > 0 ? self.fontSize : currentFont.pointSize;
    UIFontDescriptor * fontD = [currentFont.fontDescriptor fontDescriptorWithSymbolicTraits:UIFontDescriptorTraitBold];
    return [UIFont fontWithDescriptor:fontD size:size];
}

- (BOOL)isFontDefined {
    return self.fontSize > 0 || self.fontWeight > 0;
}

@end
