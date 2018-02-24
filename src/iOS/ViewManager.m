#import "ViewManager.h"

#import "FWImageView.h"
#import "PaddedLabel.h"

LinearLayoutItemMargin LLMakeMargin(CGFloat top, CGFloat left, CGFloat bottom, CGFloat right) {
    LinearLayoutItemMargin margin;
    margin.top = top;
    margin.left = left;
    margin.bottom = bottom;
    margin.right = right;
    
    return margin;
}

@implementation ViewManager;

- (id)init
{
    self = [super init];
    self.gradient = nil;
    self.layoutParams = nil;
    self.level = 0;
    self.normalStyle = [[ViewStyle alloc] init];
    self.activeStyle = [[ViewStyle alloc] init];
    self.currentStyle = self.normalStyle;
    return self;
}

- (void)clear
{
    if ([self.view isKindOfClass:FWImageView.class]) {
        FWImageView * imageView = (FWImageView *)self.view;
        [imageView clear];
    }
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
    } else if ([self.view isKindOfClass:UIPickerView.class]) {
    	UIPickerView * picker = (UIPickerView *)self.view;
        [picker reloadAllComponents];
    	[picker selectRow:value inComponent:0 animated:NO];
    }
}

- (void)setTextValue:(NSString *)value
{
    if ([self.view isKindOfClass:UILabel.class]) {
        UILabel * label = (UILabel*)self.view;
        label.text = value;
	[label.superview setNeedsLayout];
    } else if ([self.view isKindOfClass:UITextField.class]) {
        UITextField * textField = (UITextField*)self.view;
        textField.text = value;
    } else if ([self.view isKindOfClass:UIButton.class]) {
        UIButton * button = (UIButton*)self.view;
        [button setTitle:value forState:UIControlStateNormal];
	[button.superview setNeedsLayout];
    }
}

- (void)setStyle:(NSString *)key value:(NSString *)value selector:(StyleSelector)selector {
    if (selector != SelectorNormal && ![self.view isKindOfClass:UIButton.class]) {
	return;
    }

    ViewStyle * targetStyle = self.normalStyle;

    BOOL padding_applied = NO;

    if ([self.view isKindOfClass:PaddedLabel.class]) {
        PaddedLabel *label = (PaddedLabel*)self.view;
    
	if ([key isEqualToString:@"font-size"]) {
	    targetStyle.fontSize = (int)[value integerValue];
            label.font = [self createFont:label.font];
	    [label.superview setNeedsLayout];
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
	        targetStyle.fontWeight = 800;
	    } else {
		targetStyle.fontWeight = (int)[value integerValue];
	    }
	    label.font = [self createFont:label.font];
        } else if ([key isEqualToString:@"font-style"]) {
        } else if ([key isEqualToString:@"font-family"]) {
            
        } else if ([key isEqualToString:@"line-spacing"]) {

	} else if ([key isEqualToString:@"padding"]) {
            int v = (int)[value integerValue];
            label.edgeInsets = UIEdgeInsetsMake(v, v, v, v);
	    padding_applied = YES;
        } else if ([key isEqualToString:@"padding-top"]) {
            label.edgeInsets = UIEdgeInsetsMake((int)[value integerValue],
                                                label.edgeInsets.left,
                                                label.edgeInsets.bottom,
                                                label.edgeInsets.right);
	    padding_applied = YES;
        } else if ([key isEqualToString:@"padding-right"]) {
            label.edgeInsets = UIEdgeInsetsMake(label.edgeInsets.top,
                                                label.edgeInsets.left,
                                                label.edgeInsets.bottom,
                                                (int)[value integerValue]);
	    padding_applied = YES;
        } else if ([key isEqualToString:@"padding-bottom"]) {
            label.edgeInsets = UIEdgeInsetsMake(label.edgeInsets.top,
                                                label.edgeInsets.left,
                                                (int)[value integerValue],
                                                label.edgeInsets.right);
	    padding_applied = YES;
        } else if ([key isEqualToString:@"padding-left"]) {
            label.edgeInsets = UIEdgeInsetsMake(label.edgeInsets.top,
                                                (int)[value integerValue],
                                                label.edgeInsets.bottom,
                                                label.edgeInsets.right);
	    padding_applied = YES;
        }
    } else if ([self.view isKindOfClass:UIButton.class]) {
	UIButton *button = (UIButton *)self.view;
	UIControlState state;
	if (selector == SelectorNormal) {
	  state = UIControlStateNormal;
	} else if (selector == SelectorActive) {
	  state = UIControlStateHighlighted;
	} else if (selector == SelectorDisabled) {
	  state = UIControlStateDisabled;
	} else if (selector == SelectorSelected) {
	  state = UIControlStateSelected;
	} else {
	  return;
	}	  
	
        if ([key isEqualToString:@"icon"]) {
            UIImage * icon = [self loadImage:value];
            [button setImage:icon forState:state];
        } else if ([key isEqualToString:@"icon-attachment"]) {
            
	} else if ([key isEqualToString:@"color"]) {
	    [button setTitleColor:[self colorFromString:value] forState:state];
	} else if ([key isEqualToString:@"font-size"]) {
	    targetStyle.fontSize = (int)[value integerValue];
            button.titleLabel.font = [self createFont:button.titleLabel.font];
        } else if ([key isEqualToString:@"padding"]) {
            int v = (int)[value integerValue];
            button.contentEdgeInsets = UIEdgeInsetsMake(v, v, v, v);
	    padding_applied = YES;
        } else if ([key isEqualToString:@"padding-top"]) {
            button.contentEdgeInsets = UIEdgeInsetsMake((int)[value integerValue],
                                                        button.contentEdgeInsets.left,
                                                        button.contentEdgeInsets.bottom,
                                                        button.contentEdgeInsets.right);
	    padding_applied = YES;
        } else if ([key isEqualToString:@"padding-right"]) {
            button.contentEdgeInsets = UIEdgeInsetsMake(button.contentEdgeInsets.top,
                                                        button.contentEdgeInsets.left,
                                                        button.contentEdgeInsets.bottom,
                                                        (int)[value integerValue]);
	    padding_applied = YES;
        } else if ([key isEqualToString:@"padding-bottom"]) {
            button.contentEdgeInsets = UIEdgeInsetsMake(button.contentEdgeInsets.top,
                                                        button.contentEdgeInsets.left,
                                                        (int)[value integerValue],
                                                        button.contentEdgeInsets.right);
	    padding_applied = YES;
        } else if ([key isEqualToString:@"padding-left"]) {
            button.contentEdgeInsets = UIEdgeInsetsMake(button.contentEdgeInsets.top,
                                                        (int)[value integerValue],
                                                        button.contentEdgeInsets.bottom,
                                                        button.contentEdgeInsets.right);
	    padding_applied = YES;
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
	    int w;
            if ([value isEqualToString:@"match-parent"]) {
	        w = -1;
            } else if ([value isEqualToString:@"wrap-content"]) {
	        w = 0;
            } else {
                w = (int)[value integerValue];
	    }
	    if (self.layoutParams != nil) {
		self.layoutParams.fixedWidth = w;
		[view.superview setNeedsLayout];
	    }
        } else if ([key isEqualToString:@"height"]) {
	    int h;
            if ([value isEqualToString:@"match-parent"]) {
	       h = -1;
            } else if ([value isEqualToString:@"wrap-content"]) {
	       h = 0;                
            } else {
               h = (int)[value integerValue];
	    }
	    if (self.layoutParams != nil) {
		self.layoutParams.fixedHeight = h;
		[view.superview setNeedsLayout];
	    }
        } else if ([key isEqualToString:@"border-radius"]) {
	    NSArray *array = [value componentsSeparatedByString:@" "];
	    if ([array count] >= 1) {
	        int r = (int)[array[0] integerValue];
		view.layer.cornerRadius = r;
		if ([view isKindOfClass:UIImageView.class]) {
		    view.layer.masksToBounds = r > 0;
		}
	    }
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
	    if (self.layoutParams != nil) {
		self.layoutParams.margin = LLMakeMargin(v, v, v, v);
	    }
        } else if ([key isEqualToString:@"margin-top"]) {
	    int v = (int)[value integerValue];
	    if (self.layoutParams != nil) {
		self.layoutParams.margin = LLMakeMargin(v, 
							self.layoutParams.margin.left,
							self.layoutParams.margin.bottom,
							self.layoutParams.margin.right);
	    }
        } else if ([key isEqualToString:@"margin-right"]) {
	    int v = (int)[value integerValue];
	    if (self.layoutParams != nil) {
		self.layoutParams.margin = LLMakeMargin(self.layoutParams.margin.top, 
							self.layoutParams.margin.left,
							self.layoutParams.margin.bottom,
							v);
	    }
        } else if ([key isEqualToString:@"margin-bottom"]) {
	    int v = (int)[value integerValue];
	    if (self.layoutParams != nil) {
		self.layoutParams.margin = LLMakeMargin(self.layoutParams.margin.top, 
							self.layoutParams.margin.left,
							v,
							self.layoutParams.margin.right);
	    }
        } else if ([key isEqualToString:@"margin-left"]) {
	    int v = (int)[value integerValue];
	    if (self.layoutParams != nil) {
		self.layoutParams.margin = LLMakeMargin(self.layoutParams.margin.top, 
                                                        v,
							self.layoutParams.margin.bottom,
							self.layoutParams.margin.right);
	    }
        } else if ([key isEqualToString:@"weight"]) {
	    if (self.layoutParams != nil) {
		self.layoutParams.weight = (int)[value integerValue];
	    }
        } else if ([key isEqualToString:@"opacity"]) {
	    view.alpha = (float)[value floatValue];
        } else if ([key isEqualToString:@"gravity"]) {
	    if (self.layoutParams != nil) {
                if ([value isEqualToString:@"bottom"]) {
		  self.layoutParams.verticalAlignment = LinearLayoutItemVerticalAlignmentBottom;
		} else if ([value isEqualToString:@"top"]) {
		  self.layoutParams.verticalAlignment = LinearLayoutItemVerticalAlignmentTop;
		} else if ([value isEqualToString:@"left"]) {
		  self.layoutParams.horizontalAlignment = LinearLayoutItemHorizontalAlignmentLeft;
		} else if ([value isEqualToString:@"right"]) {
		  self.layoutParams.horizontalAlignment = LinearLayoutItemHorizontalAlignmentRight;
		} else if ([value isEqualToString:@"center"]) {
		  self.layoutParams.horizontalAlignment = LinearLayoutItemHorizontalAlignmentCenter;
		  self.layoutParams.verticalAlignment = LinearLayoutItemVerticalAlignmentCenter;
		} else if ([value isEqualToString:@"center-vertical"]) {
		  self.layoutParams.verticalAlignment = LinearLayoutItemVerticalAlignmentCenter;
		} else if ([value isEqualToString:@"center-horizontal"]) {
		  self.layoutParams.horizontalAlignment = LinearLayoutItemHorizontalAlignmentCenter;
		} else if ([value isEqualToString:@"start"]) {
		  self.layoutParams.horizontalAlignment = LinearLayoutItemHorizontalAlignmentLeft;
		}
	    }
        } else if ([key isEqualToString:@"zoom"]) {
        } else if (!padding_applied && self.layoutParams != nil) {
	    if ([key isEqualToString:@"padding"]) {
		int v = (int)[value integerValue];
                self.layoutParams.padding = LLMakeMargin(v, v, v, v);
            } else if ([key isEqualToString:@"padding-top"]) {
		int v = (int)[value integerValue];
                self.layoutParams.padding = LLMakeMargin(v,
                                                         self.layoutParams.padding.left,
                                                         self.layoutParams.padding.bottom,
                                                         self.layoutParams.padding.right);
	    } else if ([key isEqualToString:@"padding-right"]) {
		int v = (int)[value integerValue];
                self.layoutParams.padding = LLMakeMargin(self.layoutParams.padding.top,
                                                         self.layoutParams.padding.left,
                                                         self.layoutParams.padding.bottom,
                                                         v);
	    } else if ([key isEqualToString:@"padding-bottom"]) {
	        int v = (int)[value integerValue];
                self.layoutParams.padding = LLMakeMargin(self.layoutParams.padding.top,
                                                         self.layoutParams.padding.left,
                                                         v,
                                                         self.layoutParams.padding.right);
            } else if ([key isEqualToString:@"padding-left"]) {
		int v = (int)[value integerValue];
                self.layoutParams.padding = LLMakeMargin(self.layoutParams.padding.top,
                                                         v,
                                                         self.layoutParams.padding.bottom,
                                                         self.layoutParams.padding.right);
	    }
        }
    }    
}

- (void)applyStyles:(BOOL)animate {
    
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
    if (self.currentStyle.fontWeight) {
        UIFontDescriptor * fontD = [currentFont.fontDescriptor fontDescriptorWithSymbolicTraits:UIFontDescriptorTraitBold];
        return [UIFont fontWithDescriptor:fontD size:self.currentStyle.fontSize];
    } else {
        return [currentFont fontWithSize:self.currentStyle.fontSize];
    }
}

@end
