#import "ViewManager.h"

#import "FWImageView.h"

@implementation ViewManager;

- (id)init
{
    self = [super init];
    self.constraintsSet = NO;
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
    }
}

- (void)setTextValue:(NSString *)value
{
    if ([self.view isKindOfClass:UILabel.class]) {
        UILabel * label = (UILabel*)self.view;
        label.text = value;
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
            view.layer.backgroundColor = [self colorFromString:value].CGColor;
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
            view.layer.borderColor = [self colorFromString:value].CGColor;
            view.layer.borderWidth = 1.0f;
        } else if ([key isEqualToString:@"margin"]) {
        } else if ([key isEqualToString:@"margin-top"]) {
        } else if ([key isEqualToString:@"margin-right"]) {
        } else if ([key isEqualToString:@"margin-borrom"]) {
        } else if ([key isEqualToString:@"margin-left"]) {
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
            
        } else if ([key isEqualToString:@"right"]) {
            
        } else if ([key isEqualToString:@"bottom"]) {
        } else if ([key isEqualToString:@"left"]) {
        } else if ([key isEqualToString:@"gravity"]) {
        } else if ([key isEqualToString:@"zoom"]) {
        }
    }
    
    if ([self.view isKindOfClass:UILabel.class]) {
        UILabel *label = (UILabel *)self.view;
    
	if ([key isEqualToString:@"font-size"]) {
	    int b = (int)[value integerValue];
            label.font = [label.font fontWithSize:b];
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
                label.numberOfLines = 0;
                label.lineBreakMode = NSLineBreakByWordWrapping;
            }
        } else if ([key isEqualToString:@"text-overflow"]) {
            if ([value isEqualToString:@"ellipsis"]) {
                label.lineBreakMode = NSLineBreakByTruncatingTail;
            } else {
                if (label.numberOfLines == 0) {
                    label.lineBreakMode = NSLineBreakByWordWrapping;
                } else {
                    label.lineBreakMode = NSLineBreakByClipping;
                }
            }
        } else if ([key isEqualToString:@"font-weight"]) {
        } else if ([key isEqualToString:@"font-style"]) {
        } else if ([key isEqualToString:@"font-family"]) {
            
        }
    } else if ([self.view isKindOfClass:UIButton.class]) {
	UIButton *button = (UIButton *)self.view;
        if ([key isEqualToString:@"icon"]) {
            UIImage * icon = [UIImage imageNamed:value];
            [button setImage:icon forState:UIControlStateNormal];
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
            NSString *imageString = value;
            UIImage *icon = [UIImage imageNamed:imageString];
            item.image = icon;
        }
    }
}

- (UIColor *)colorFromString:(NSString *)hexString {
  unsigned rgbValue = 0;
  NSScanner *scanner = [NSScanner scannerWithString:hexString];
  [scanner setScanLocation:1]; // bypass '#' character
  [scanner scanHexInt:&rgbValue];
  return [UIColor colorWithRed:((rgbValue & 0xFF0000) >> 16)/255.0 green:((rgbValue & 0xFF00) >> 8)/255.0 blue:(rgbValue & 0xFF)/255.0 alpha:1.0];
}

@end
