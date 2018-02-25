#import <UIKit/UIKit.h>

#import "LayoutParams.h"
#import "ViewStyle.h"

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
- (void)setImage:(UIImage *)data;
- (void)addImageUrl:(NSString *)url width:(int)width height:(int)height;
- (void)setStyle:(NSString *)key value:(NSString *)value selector:(StyleSelector)selector;
- (void)applyStyles:(BOOL)animate;
- (void)setIntValue:(int)value;
- (void)setTextValue:(NSString *)value;
- (void)switchStyle:(StyleSelector)selector;

@property (nonatomic) NSInteger viewId; // don't use id as it is reserved word in objective-c
@property id view;
@property id containerView;
@property LayoutParams * layoutParams;
@property NSInteger level;
@property ViewStyle * normalStyle;
@property ViewStyle * activeStyle;
@property ViewStyle * currentStyle;
@end
