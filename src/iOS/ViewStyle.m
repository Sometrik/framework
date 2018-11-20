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
	self.fontFamily = nil;
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
	self.borderColor = nil;
    }
    return self;
}

- (void)apply:(id)target animate:(BOOL)animate {
    if ([target isKindOfClass:PaddedLabel.class]) {
        PaddedLabel * label = (PaddedLabel*)target;
	label.defaultSize = self.fontSize;
	if (label.autolink || label.markdown) {
	  label.defaultColor = self.color;
	  label.attributedText = [label createAttributedString:label.origText];
	} else {
	  if ([self isFontDefined]) {
            label.font = [self createFont:label.font];
	  }
	  if (self.color != nil) label.textColor = self.color;
	}
        label.edgeInsets = UIEdgeInsetsMake(self.paddingTop, self.paddingLeft, self.paddingBottom, self.paddingRight);
    } else if ([target isKindOfClass:UIButton.class]) {
        UIButton * button = (UIButton*)target;
        if ([self isFontDefined]) {
            button.titleLabel.font = [self createFont:button.titleLabel.font];
        }
        if (self.color != nil) [button setTitleColor:self.color forState:UIControlStateNormal];
        button.contentEdgeInsets = UIEdgeInsetsMake(self.paddingTop, self.paddingLeft, self.paddingBottom, self.paddingRight);
    }

    if ([target isKindOfClass:UIView.class]) {
        UIView * view = (UIView *)target;
	
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
	    view.layer.shadowOpacity = 0.25;
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
		[CATransaction setAnimationDuration:0.2];
		[CATransaction setAnimationTimingFunction:[CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut] ];
		view.layer.shadowRadius = self.shadow;
		[CATransaction commit];
#endif
	    }
	    float prevZoom = view.transform.a;
	    if (fabs(view.alpha - self.alpha) > EPSILON || fabs(prevZoom - self.zoom) > EPSILON) {
	        [UIView animateWithDuration:0.2 animations:^{
		    view.alpha = self.alpha;
		    view.transform = CGAffineTransformMakeScale(self.zoom, self.zoom);
		}];
	    }
	} else {
	    view.alpha = self.alpha;
	    view.transform = CGAffineTransformMakeScale(self.zoom, self.zoom);
	    view.layer.shadowRadius = self.shadow;
	}
    } else if ([target isKindOfClass:UIBarItem.class]) {
        UIBarItem * item = (UIBarItem *)target;
	if ([self isFontDefined]) {
	    UIFont * font = [self createFont:nil];
	    [item setTitleTextAttributes:@{NSFontAttributeName: font} forState:UIControlStateNormal];
	}
    }
}

- (UIFont *)createFont:(UIFont *)currentFont {
   NSInteger size = self.fontSize > 0 ? self.fontSize : (currentFont != nil ? currentFont.pointSize : 10);
    if (self.fontFamily != nil) {
        return [UIFont fontWithName:self.fontFamily size:size];
    } else if (self.fontWeight && currentFont != nil) {
        UIFontDescriptor * fontD = [currentFont.fontDescriptor fontDescriptorWithSymbolicTraits:UIFontDescriptorTraitBold];
        return [UIFont fontWithDescriptor:fontD size:size];
    } else if (currentFont != nil) {
        return [currentFont fontWithSize:size];
    } else {
        return [UIFont systemFontOfSize:size];
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
