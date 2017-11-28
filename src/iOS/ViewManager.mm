#import "ViewManager.h"

@implementation ViewManager;

- (void)addImageUrl:(NSString *)url width:(int)width height:(int)height
{
  
}

- (void)setImage:(UIImage *)data
{
	// if view is image, set the content
}

- (void)setIntValue:(int)value
{
	// if view is switch, set state to value
}

- (void)setTextValue:(NSString *)value;
{
	// if view is text field or label, set the text
}

- (void)setStyle:(NSString *)key value:(NSString *)value {
    
    if ([self.view isKindOfClass:UIView.class]) {
        UIView * view = (UIView *)self.view;
        if ([key isEqualToString:@"background-color"]) {
            view.backgroundColor = [self colorFromString:value];
        } else if ([key isEqualToString:@"background"]) {
            view.backgroundColor = [self colorFromString:value];
        } else if ([key isEqualToString:@"shadow"]) {
            view.layer.shadowOpacity = 0.25;
            // self.view.layer.masksToBounds = NO;
            view.layer.shadowRadius = (float)[value floatValue];
            view.layer.shadowOffset = CGSizeMake(0, 0);
        } else if ([key isEqualToString:@"width"]) {
            if ([value isEqualToString:@"match-parent"]) {
                
            } else if ([value isEqualToString:@"wrap-content"]) {
                
            } else {
                int width = (int)[value integerValue];
                // [view.widthAnchor constraintEqualToConstant:width].active = true;
            }
        } else if ([key isEqualToString:@"height"]) {
            if ([value isEqualToString:@"match-parent"]) {
                
            } else if ([value isEqualToString:@"wrap-content"]) {
                
            } else {
                int height = (int)[value integerValue];
                // [self.view.heightAnchor constraintEqualToConstant:height].active = true;
            }
        } else if ([key isEqualToString:@"border-radius"]) {
            view.layer.cornerRadius = (int)[value integerValue];
        } else if ([key isEqualToString:@"border"]) {
	        view.layer.borderColor = [self colorFromString:value].CGColor;
            view.layer.borderWidth = 1.0f;
        } else if ([key isEqualToString:@"margin"]) {
            int v = (int)[value integerValue];
            view.layoutMargins = UIEdgeInsetsMake(v, v, v, v);
        }
    }
  
    if ([self.view isKindOfClass:UILabel.class]) {
        UILabel *label = (UILabel *)self.view;
    
	if ([key isEqualToString:@"font-size"]) {
	    int b = (int)[value integerValue];
            label.font = [label.font fontWithSize:b];
	} else if ([key isEqualToString:@"color"]) {
	    label.textColor = [self colorFromString:value];
	} else if ([key isEqualToString:@"text-alignment"]) {
	    if ([value isEqualToString:@"center"]) {
	        label.textAlignment = NSTextAlignmentCenter;
	    } else if ([value isEqualToString:@"right"]) {
		label.textAlignment = NSTextAlignmentRight;
	    } else {
		label.textAlignment = NSTextAlignmentLeft;
	    }
        }
    } else if ([self.view isKindOfClass:UIButton.class]) {
	UIButton *button = (UIButton *)self.view;
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
