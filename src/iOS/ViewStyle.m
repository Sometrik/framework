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
	self.clipsToBounds = YES;
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
    } else if ([target isKindOfClass:UIScrollView.class]) {
        UIScrollView * view = (UIScrollView*)target;
	view.clipsToBounds = self.clipsToBounds;
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
	    view.layer.shadowOpacity = 0.15;
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
    } else if (currentFont != nil) {
        if (self.fontWeight > 400) {
	    UIFontDescriptor * fontD = [currentFont.fontDescriptor fontDescriptorWithSymbolicTraits:UIFontDescriptorTraitBold];
	    return [UIFont fontWithDescriptor:fontD size:size];
	} else {
	    return [currentFont fontWithSize:size];
	}
    } else if (self.fontWeight != 0 && (self.fontWeight <= 300 || self.fontWeight >= 500)) {
        return [UIFont systemFontOfSize:size weight:[self getUIFontWeight]];
    } else {      
        return [UIFont systemFontOfSize:size];
    }
}

- (UIFontWeight)getUIFontWeight {
    if (self.fontWeight && (self.fontWeight < 400 || self.fontWeight >= 500)) {
        if (self.fontWeight >= 900) {
            return UIFontWeightBlack;
        } else if (self.fontWeight >= 800) {
            return UIFontWeightHeavy;
        } else if (self.fontWeight >= 700) {
            return UIFontWeightBold;
        } else if (self.fontWeight >= 600) {
            return UIFontWeightSemibold;
        } else if (self.fontWeight >= 500) {
            return UIFontWeightMedium;
        } else if (self.fontWeight >= 300) {
            return UIFontWeightLight;
        } else if (self.fontWeight >= 200) {
            return UIFontWeightUltraLight;
        } else {
            return UIFontWeightThin;
        }
    } else {
        return UIFontWeightRegular;
    }
}

- (UIFont *)createBoldFont:(UIFont *)currentFont {
    NSInteger size = self.fontSize > 0 ? self.fontSize : currentFont.pointSize;
    UIFontDescriptor * fontD = [currentFont.fontDescriptor fontDescriptorWithSymbolicTraits:UIFontDescriptorTraitBold];
    return [UIFont fontWithDescriptor:fontD size:size];
}

- (BOOL)isFontDefined {
    return self.fontSize > 0 || self.fontWeight > 0 || self.fontFamily != nil;
}

@end
