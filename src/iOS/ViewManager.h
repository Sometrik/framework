#import <UIKit/UIKit.h>

#import "LayoutParams.h"

typedef enum {
  SelectorNormal = 0,	// State during normal operation
  SelectorActive,	// Element is active or pressed
  SelectorHover,	// Mouse cursor is over element
  SelectorSelected,	// Element has been selected
  SelectorDisabled,	// Element is disabled
  SelectorLink		// Element or a part of element is a link
} StyleSelector;

@interface ViewManager : NSObject
- (id)init;
- (void)clear;
- (void)setConstraints;
- (void)setImage:(UIImage *)data;
- (void)addImageUrl:(NSString *)url width:(int)width height:(int)height;
- (void)setStyle:(NSString *)key value:(NSString *)value selector:(StyleSelector)selector;
- (void)setIntValue:(int)value;
- (void)setTextValue:(NSString *)value;

@property (nonatomic) NSInteger viewId; // don't use id as it is reserved word in objective-c
@property id view;
@property BOOL constraintsSet;
@property NSLayoutConstraint *topConstraint;
@property NSLayoutConstraint *leftConstraint;
@property NSLayoutConstraint *rightConstraint;
@property NSLayoutConstraint *bottomConstraint;
@property CAGradientLayer *gradient;
@property NSInteger fontSize;
@property NSInteger fontWeight;
@property LayoutParams * layoutParams;
@property NSInteger level;
@end
