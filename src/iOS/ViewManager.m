#import "ViewManager.h"

#import "FWImageView.h"
#import "PaddedLabel.h"
#import "FWPicker.h"

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
    } else if ([self.view isKindOfClass:FWPicker.class]) {
    	FWPicker * picker = (FWPicker *)self.view;
    	[picker setSelection:value];
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
    } else if ([self.view isKindOfClass:UINavigationBar.class]) {
        UINavigationBar * navBar = (UINavigationBar*)self.view;
        navBar.items[0].title = value;
    }
}

- (void)setStyle:(NSString *)key value:(NSString *)value selector:(StyleSelector)selector {
    ViewStyle * targetStyle = [self getStyleForSelector:selector];

    if (targetStyle == nil) {
        return;
    }

    BOOL hasNativePadding = [self.view isKindOfClass:UIButton.class] || [self.view isKindOfClass:PaddedLabel.class];
    
    if ([key isEqualToString:@"color"]) {
        targetStyle.color = [self colorFromString:value];
    } else if ([key isEqualToString:@"opacity"]) {
        targetStyle.alpha = (float)[value floatValue];
    } else if ([key isEqualToString:@"zoom"]) {
        targetStyle.zoom = (float)[value floatValue];
    } else if ([key isEqualToString:@"font-size"]) {
        targetStyle.fontSize = (int)[value integerValue];
        if ([self.view isKindOfClass:UIView.class]) {
            UIView * view = (UIView*)self.view;
            [view.superview setNeedsLayout];
        }
    } else if ([key isEqualToString:@"font-weight"]) {
        if ([value isEqualToString:@"bold"]) {
            targetStyle.fontWeight = 800;
        } else {
            targetStyle.fontWeight = (int)[value integerValue];
        }
    } else if ([key isEqualToString:@"font-style"]) {
    } else if ([key isEqualToString:@"font-family"]) {
    } else if ([key isEqualToString:@"background-color"]) {
        targetStyle.backgroundColor = [self colorFromString:value];
    } else if ([key isEqualToString:@"padding"]) {
        int v = (int)[value integerValue];
        targetStyle.paddingTop = v;
        targetStyle.paddingRight = v;
        targetStyle.paddingBottom = v;
        targetStyle.paddingLeft = v;
    } else if ([key isEqualToString:@"padding-top"]) {
        targetStyle.paddingTop = (int)[value integerValue];
    } else if ([key isEqualToString:@"padding-right"]) {
        targetStyle.paddingRight = (int)[value integerValue];
    } else if ([key isEqualToString:@"padding-bottom"]) {
        targetStyle.paddingBottom = (int)[value integerValue];
    } else if ([key isEqualToString:@"padding-left"]) {
        targetStyle.paddingLeft = (int)[value integerValue];
    } else if ([key isEqualToString:@"shadow"]) {
        targetStyle.shadow = (float)[value floatValue];
    } else if ([key isEqualToString:@"border-radius"]) {
        NSArray *array = [value componentsSeparatedByString:@" "];
        if ([array count] >= 1) {
            targetStyle.borderRadius = (int)[array[0] integerValue];
        }
    } else if ([key isEqualToString:@"border"]) {
        if ([value isEqualToString:@"none"] || ![value length]) {
            targetStyle.borderWidth = 0;
        } else {
            NSArray *array = [value componentsSeparatedByString:@" "];
            if ([array count] <= 1) {
                targetStyle.borderColor = [self colorFromString:value];
                targetStyle.borderWidth = 1.0f;
            } else {
                targetStyle.borderWidth = (int)[array[0] integerValue];
                if ([array count] >= 2) {
                    // parse border style
                }
                if ([array count] >= 3) {
                    targetStyle.borderColor = [self colorFromString:array[2]];
                } else {
                    targetStyle.borderColor = UIColor.blackColor;
                }
            }
        }
    } else if ([key isEqualToString:@"content-inset-top"]) {
        targetStyle.contentInsetTop = (int)[value integerValue];
    } else if ([key isEqualToString:@"content-inset-bottom"]) {
        targetStyle.contentInsetBottom = (int)[value integerValue];
    }

    if (selector != SelectorNormal) {
        return;
    }

    if ([self.view isKindOfClass:PaddedLabel.class]) {
        PaddedLabel *label = (PaddedLabel*)self.view;
    
        if ([key isEqualToString:@"text-align"]) {
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
        } else if ([key isEqualToString:@"line-spacing"]) {
        }
    } else if ([self.view isKindOfClass:UIButton.class]) {
        UIButton *button = (UIButton *)self.view;
        UIControlState state = UIControlStateNormal;
	
        if ([key isEqualToString:@"icon"]) {
            UIImage * icon = [self loadImage:value];
            [button setImage:icon forState:state];
        } else if ([key isEqualToString:@"icon-attachment"]) {
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
        if ([key isEqualToString:@"background"]) {
            NSRange searchedRange = NSMakeRange(0, [value length]);
            NSString * pattern = @"^linear-gradient\\(\\s*([^, ]+),\\s*([^, ]+)\\s*\\)$";
            NSError * error = nil;
            NSRegularExpression * regex = [NSRegularExpression regularExpressionWithPattern:pattern options:0 error:&error];
            NSTextCheckingResult *match = [regex firstMatchInString:value options:0 range:searchedRange];
            if (match != nil) {
                NSString * color1 = [value substringWithRange:[match rangeAtIndex:1]];
                NSString * color2 = [value substringWithRange:[match rangeAtIndex:2]];
                CAGradientLayer * gradient = targetStyle.gradient;
                if (gradient == nil) gradient = [CAGradientLayer layer];
                gradient.colors = @[(id)[self colorFromString:color1].CGColor, (id)[self colorFromString:color2].CGColor];
                if (targetStyle.gradient == nil) {
                    gradient.frame = view.bounds;
                    [view.layer insertSublayer:gradient atIndex:0];
                    targetStyle.gradient = gradient;
                }
            } else {
                if (targetStyle.gradient != nil) {
                    [targetStyle.gradient removeFromSuperlayer];
                    targetStyle.gradient = nil;
                }
                targetStyle.backgroundColor = [self colorFromString:value];
            }
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
        } else if (!hasNativePadding && self.layoutParams != nil) {
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
    if ([self.view isKindOfClass:UIView.class]) {
        [self.currentStyle apply:self.view animate:animate];
    }
}

- (UIImage *)loadImage:(NSString *)filename {
    NSString *maskFilePath = [[NSBundle mainBundle] pathForResource:filename ofType:nil];
    CGDataProviderRef dataProvider = CGDataProviderCreateWithFilename([maskFilePath UTF8String]);
    CGImageRef imageRef = CGImageCreateWithPNGDataProvider(dataProvider, NULL, true, kCGRenderingIntentDefault);
    UIImage * image = [UIImage imageWithCGImage:imageRef scale:3.0f orientation:UIImageOrientationUp];
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

- (ViewStyle*)getStyleForSelector:(StyleSelector)selector {
    switch (selector) {
    case SelectorNormal: return self.normalStyle;
    case SelectorActive: return self.activeStyle;
    }
    return nil;
}

- (void)switchStyle:(StyleSelector)selector {
    ViewStyle * newStyle = [self getStyleForSelector:selector];
    if (newStyle != nil && newStyle != self.currentStyle) {
        self.currentStyle = newStyle;
        [self applyStyles:YES];
    }
}

@end