#import "ViewManager.h"

#import "FWImageView.h"
#import "PaddedLabel.h"

@implementation ViewManager;

- (id)init
{
    self = [super init];
    self.constraintsSet = NO;
    self.topConstraint = nil;
    self.leftConstraint = nil;
    self.rightConstraint = nil;
    self.bottomConstraint = nil;
    self.gradient = nil;
    self.fontSize = 0;
    self.fontWeight = 0;
    return self;
}

- (void)clear
{
    if ([self.view isKindOfClass:FWImageView.class]) {
        FWImageView * imageView = (FWImageView *)self.view;
        [imageView clear];
    }
}

- (void)setConstraints
{
    self.constraintsSet = YES;

    
    UIView * view = self.view;
    
    self.topConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeTopMargin multiplier:1.0f constant:0];
    self.leftConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeLeftMargin multiplier:1.0f constant:0];
    self.rightConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeRightMargin multiplier:1.0f constant:0];
    self.bottomConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeBottomMargin multiplier:1.0f constant:0];

   [view.superview addConstraints:@[self.topConstraint, self.leftConstraint, self.rightConstraint, self.bottomConstraint]];
}

- (void)addImageUrl:(NSString *)url width:(int)width height:(int)height
{
    if ([self.view isKindOfClass:FWImageView.class]) {
        FWImageView *imageView = (FWImageView *)self.view;
        [imageView addImageUrl:url width:width height:height];
    }
}

- (void)setImage:(UIImage *)data
{
    if ([self.view isKindOfClass:UIImageView.class]) {
        UIImageView * imageView = (UIImageView *)self.view;
        imageView.image = data;
    }
}

- (void)setIntValue:(int)value
{
    if ([self.view isKindOfClass:UISwitch.class]) {
        UISwitch * sw = (UISwitch *)self.view;
        sw.on = value != 0 ? YES : NO;
    } else if ([self.view isKindOfClass:UIPageControl.class]) {
	UIPageControl * pageControl = (UIPageControl *)self.view;
	pageControl.currentPage = value;
    }
}

- (void)setTextValue:(NSString *)value
{
    if ([self.view isKindOfClass:UILabel.class]) {
        UILabel * label = (UILabel*)self.view;
        label.text = value;
        [label sizeToFit];
    } else if ([self.view isKindOfClass:UITextField.class]) {
        UITextField * textField = (UITextField*)self.view;
        textField.text = value;
    }
}

- (void)setStyle:(NSString *)key value:(NSString *)value {
    
    if ([self.view isKindOfClass:UIView.class]) {
        UIView * view = (UIView *)self.view;
        if ([key isEqualToString:@"background-color"]) {
            view.layer.backgroundColor = [self colorFromString:value].CGColor;
        } else if ([key isEqualToString:@"background"]) {
	    NSRange searchedRange = NSMakeRange(0, [value length]);
	    NSString * pattern = @"^linear-gradient\\(\\s*([^, ]+),\\s*([^, ]+)\\s*\\)$";
	    NSError * error = nil;
	    NSRegularExpression * regex = [NSRegularExpression regularExpressionWithPattern:pattern options:0 error:&error];
	    NSTextCheckingResult *match = [regex firstMatchInString:value options:0 range:searchedRange];
	    if (match != nil) {
	        NSString * color1 = [value substringWithRange:[match rangeAtIndex:1]];
	        NSString * color2 = [value substringWithRange:[match rangeAtIndex:2]];
		CAGradientLayer * gradient = self.gradient;
		if (gradient == nil) gradient = [CAGradientLayer layer];
		gradient.colors = @[(id)[self colorFromString:color1].CGColor, (id)[self colorFromString:color2].CGColor];
		if (self.gradient == nil) {
		    gradient.frame = view.bounds;
		    [view.layer insertSublayer:gradient atIndex:0];
		    self.gradient = gradient;
		}
	    } else {
		if (self.gradient != nil) {
		    [self.gradient removeFromSuperlayer];
		    self.gradient = nil;
		}
	        view.layer.backgroundColor = [self colorFromString:value].CGColor;
	    }
        } else if ([key isEqualToString:@"shadow"]) {
            view.layer.shadowOpacity = 0.5;
            // self.view.layer.masksToBounds = NO;
            view.layer.shadowRadius = (float)[value floatValue];
            view.layer.shadowOffset = CGSizeMake(0, 0);
            view.layer.masksToBounds = FALSE;
        } else if ([key isEqualToString:@"width"]) {
            if ([value isEqualToString:@"match-parent"]) {
                
            } else if ([value isEqualToString:@"wrap-content"]) {
                
            } else {
                int width = (int)[value integerValue];
                NSLayoutConstraint * c = [view.widthAnchor constraintEqualToConstant:width];
                c.priority = 999;
                c.active = true;
            }
        } else if ([key isEqualToString:@"height"]) {
            if ([value isEqualToString:@"match-parent"]) {
                
            } else if ([value isEqualToString:@"wrap-content"]) {
                
            } else {
                int height = (int)[value integerValue];
                NSLayoutConstraint * c = [view.heightAnchor constraintEqualToConstant:height];
                c.priority = 999;
                c.active = true;
            }
        } else if ([key isEqualToString:@"border-radius"]) {
            view.layer.cornerRadius = (int)[value integerValue];
        } else if ([key isEqualToString:@"border"]) {
	    if ([value isEqualToString:@"none"] || ![value length]) {
	        view.layer.borderWidth = 0.0f;
	    } else {
		NSArray *array = [value componentsSeparatedByString:@" "];
		if ([array count] <= 1) {
	            view.layer.borderColor = [self colorFromString:value].CGColor;
                    view.layer.borderWidth = 1.0f;
                } else {
		    view.layer.borderWidth = (int)[array[0] integerValue];
		    if ([array count] >= 2) {
		        // parse border style
		    }
		    if ([array count] >= 3) {
		        view.layer.borderColor = [self colorFromString:array[2]].CGColor;
		    } else {
	                view.layer.borderColor = UIColor.blackColor.CGColor;
		    }
		}
	    }
        } else if ([key isEqualToString:@"margin"]) {
	    int v = (int)[value integerValue];
	    self.topConstraint.constant = v;
	    self.leftConstraint.constant = v;
	    self.rightConstraint.constant = -v;
	    self.bottomConstraint.constant = -v;
        } else if ([key isEqualToString:@"margin-top"]) {
	    int v = (int)[value integerValue];
	    self.topConstraint.constant = v;
        } else if ([key isEqualToString:@"margin-right"]) {
	    int v = (int)[value integerValue];
	    self.rightConstraint.constant = -v;
        } else if ([key isEqualToString:@"margin-bottom"]) {
	    int v = (int)[value integerValue];
	    self.bottomConstraint.constant = -v;
        } else if ([key isEqualToString:@"margin-left"]) {
	    int v = (int)[value integerValue];
	    self.leftConstraint.constant = v;
        } else if ([key isEqualToString:@"padding"]) {
            int v = (int)[value integerValue];
            view.layoutMargins = UIEdgeInsetsMake(v, v, v, v);
        } else if ([key isEqualToString:@"padding-top"]) {
            view.layoutMargins = UIEdgeInsetsMake((int)[value integerValue],
                                                  view.layoutMargins.left,
                                                  view.layoutMargins.bottom,
                                                  view.layoutMargins.right);
        } else if ([key isEqualToString:@"padding-right"]) {
            view.layoutMargins = UIEdgeInsetsMake(view.layoutMargins.top,
                                                  view.layoutMargins.left,
                                                  view.layoutMargins.bottom,
                                                  (int)[value integerValue]);
        } else if ([key isEqualToString:@"padding-bottom"]) {
            view.layoutMargins = UIEdgeInsetsMake(view.layoutMargins.top,
                                                  view.layoutMargins.left,
                                                  (int)[value integerValue],
                                                  view.layoutMargins.right);
        } else if ([key isEqualToString:@"padding-left"]) {
            view.layoutMargins = UIEdgeInsetsMake(view.layoutMargins.top,
                                                  (int)[value integerValue],
                                                  view.layoutMargins.bottom,
                                                  view.layoutMargins.right);
        } else if ([key isEqualToString:@"weight"]) {
            
        } else if ([key isEqualToString:@"opacity"]) {
        } else if ([key isEqualToString:@"top"]) {
	    int v = (int)[value integerValue];
	    self.topConstraint.constant = v;            
        } else if ([key isEqualToString:@"right"]) {
       	    int v = (int)[value integerValue];
	    self.rightConstraint.constant = v;
        } else if ([key isEqualToString:@"bottom"]) {
	    int v = (int)[value integerValue];
	    self.bottomConstraint.constant = v;
        } else if ([key isEqualToString:@"left"]) {
	    int v = (int)[value integerValue];
	    self.leftConstraint.constant = v;
        } else if ([key isEqualToString:@"gravity"]) {
        } else if ([key isEqualToString:@"zoom"]) {
        }
    }
    
    if ([self.view isKindOfClass:PaddedLabel.class]) {
        PaddedLabel *label = (UILabel *)self.view;
    
	if ([key isEqualToString:@"font-size"]) {
	    self.fontSize = (int)[value integerValue];
            label.font = [self createFont:label.font];
//	    [label sizeToFit];
	} else if ([key isEqualToString:@"color"]) {
	    label.textColor = [self colorFromString:value];
	} else if ([key isEqualToString:@"text-align"]) {
	    if ([value isEqualToString:@"center"]) {
	        label.textAlignment = NSTextAlignmentCenter;
	    } else if ([value isEqualToString:@"right"]) {
		label.textAlignment = NSTextAlignmentRight;
	    } else {
		label.textAlignment = NSTextAlignmentLeft;
	    }
        } else if ([key isEqualToString:@"white-space"]) {
            if ([value isEqualToString:@"nowrap"]) {
                label.numberOfLines = 1;
                if (label.lineBreakMode != NSLineBreakByTruncatingTail) {
                    label.lineBreakMode = NSLineBreakByClipping;
                }
            } else {
                // label.numberOfLines = 0;
                label.lineBreakMode = NSLineBreakByWordWrapping;
            }
        } else if ([key isEqualToString:@"text-overflow"]) {
            if ([value isEqualToString:@"ellipsis"]) {
                label.lineBreakMode = NSLineBreakByTruncatingTail;
            } else {
                if (label.numberOfLines != 1) {
                    label.lineBreakMode = NSLineBreakByWordWrapping;
                } else {
                    label.lineBreakMode = NSLineBreakByClipping;
                }
            }
	} else if ([key isEqualToString:@"max-lines"]) {
	    label.numberOfLines = (int)[value integerValue];	   
        } else if ([key isEqualToString:@"font-weight"]) {
	    if ([value isEqualToString:@"bold"]) {
	        self.fontWeight = 800;
	    } else {
		self.fontWeight = (int)[value integerValue];
	    }
	    label.font = [self createFont:label.font];
        } else if ([key isEqualToString:@"font-style"]) {
        } else if ([key isEqualToString:@"font-family"]) {
            
        } else if ([key isEqualToString:@"line-spacing"]) {

	} else if ([key isEqualToString:@"padding"]) {
            int v = (int)[value integerValue];
            label.edgeInsets = UIEdgeInsetsMake(v, v, v, v);
        } else if ([key isEqualToString:@"padding-top"]) {
            label.edgeInsets = UIEdgeInsetsMake((int)[value integerValue],
                                                label.edgeInsets.left,
                                                label.edgeInsets.bottom,
                                                label.edgeInsets.right);
        } else if ([key isEqualToString:@"padding-right"]) {
            label.edgeInsets = UIEdgeInsetsMake(label.edgeInsets.top,
                                                label.edgeInsets.left,
                                                label.edgeInsets.bottom,
                                                (int)[value integerValue]);
        } else if ([key isEqualToString:@"padding-bottom"]) {
            label.edgeInsets = UIEdgeInsetsMake(label.edgeInsets.top,
                                                label.edgeInsets.left,
                                                (int)[value integerValue],
                                                label.edgeInsets.right);
        } else if ([key isEqualToString:@"padding-left"]) {
            label.edgeInsets = UIEdgeInsetsMake(label.edgeInsets.top,
                                                (int)[value integerValue],
                                                label.edgeInsets.bottom,
                                                label.edgeInsets.right);
        }
    } else if ([self.view isKindOfClass:UIButton.class]) {
	UIButton *button = (UIButton *)self.view;
        if ([key isEqualToString:@"icon"]) {
            UIImage * icon = [self loadImage:value];
            [button setImage:icon forState:UIControlStateNormal];
        } else if ([key isEqualToString:@"icon-attachment"]) {
            
	} else if ([key isEqualToString:@"color"]) {
	    [button setTitleColor:[self colorFromString:value] forState:UIControlStateNormal];
	} else if ([key isEqualToString:@"font-size"]) {
	    self.fontSize = (int)[value integerValue];
            button.titleLabel.font = [self createFont:button.titleLabel.font];
        } else if ([key isEqualToString:@"padding"]) {
            int v = (int)[value integerValue];
            button.contentEdgeInsets = UIEdgeInsetsMake(v, v, v, v);
        } else if ([key isEqualToString:@"padding-top"]) {
            button.contentEdgeInsets = UIEdgeInsetsMake((int)[value integerValue],
                                                        button.contentEdgeInsets.left,
                                                        button.contentEdgeInsets.bottom,
                                                        button.contentEdgeInsets.right);
        } else if ([key isEqualToString:@"padding-right"]) {
            button.contentEdgeInsets = UIEdgeInsetsMake(button.contentEdgeInsets.top,
                                                        button.contentEdgeInsets.left,
                                                        button.contentEdgeInsets.bottom,
                                                        (int)[value integerValue]);
        } else if ([key isEqualToString:@"padding-bottom"]) {
            button.contentEdgeInsets = UIEdgeInsetsMake(button.contentEdgeInsets.top,
                                                        button.contentEdgeInsets.left,
                                                        (int)[value integerValue],
                                                        button.contentEdgeInsets.right);
        } else if ([key isEqualToString:@"padding-left"]) {
            button.contentEdgeInsets = UIEdgeInsetsMake(button.contentEdgeInsets.top,
                                                        (int)[value integerValue],
                                                        button.contentEdgeInsets.bottom,
                                                        button.contentEdgeInsets.right);
        }
    } else if ([self.view isKindOfClass:UITextField.class]) {
        UITextField *textField = (UITextField *)self.view;
	if ([value isEqualToString:@"hint"]) {
	    textField.placeholder = value;
        }
    } else if ([self.view isKindOfClass:UITabBarItem.class]) {
        UITabBarItem *item = (UITabBarItem *)self.view;
        if ([key isEqualToString:@"icon"]) {
            item.image = [self loadImage:value];
        }
    }
}

- (UIImage *)loadImage:(NSString *)filename {
    NSString *maskFilePath = [[NSBundle mainBundle] pathForResource:filename ofType:nil];
    CGDataProviderRef dataProvider = CGDataProviderCreateWithFilename([maskFilePath UTF8String]);
    CGImageRef imageRef = CGImageCreateWithPNGDataProvider(dataProvider, NULL, true, kCGRenderingIntentDefault);
    UIImage * image = [UIImage imageWithCGImage:imageRef scale:2.0f orientation:UIImageOrientationUp];
    CGImageRelease(imageRef);
    CGDataProviderRelease(dataProvider);
    return image;
}

- (UIColor *)colorFromString:(NSString *)value {
    if ([value isEqualToString:@"white"]) {
        return UIColor.whiteColor;
    } else if ([value isEqualToString:@"black"]) {
        return UIColor.blackColor;
    } else {
	unsigned int rgbValue = 0;
	NSScanner *scanner = [NSScanner scannerWithString:value];
	[scanner setScanLocation:1]; // bypass '#' character
	[scanner scanHexInt:&rgbValue];
	if (rgbValue <= 0xffffff) {
	    return [UIColor colorWithRed:((rgbValue & 0xFF0000) >> 16)/255.0 green:((rgbValue & 0xFF00) >> 8)/255.0 blue:(rgbValue & 0xFF)/255.0 alpha:1.0];
	} else {
	    return [UIColor colorWithRed:((rgbValue & 0xFF0000) >> 16)/255.0 green:((rgbValue & 0xFF00) >> 8)/255.0 blue:(rgbValue & 0xFF)/255.0 alpha:((rgbValue & 0xFF000000) >> 24)/255.0];
	}
    }
}

- (UIFont *)createFont:(UIFont *)currentFont {
    if (self.fontWeight) {
        UIFontDescriptor * fontD = [currentFont.fontDescriptor fontDescriptorWithSymbolicTraits:UIFontDescriptorTraitBold];
        return [UIFont fontWithDescriptor:fontD size:self.fontSize];
    } else {
        return [currentFont fontWithSize:self.fontSize];
    }
}

@end
