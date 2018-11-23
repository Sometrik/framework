#import <UIKit/UIKit.h>

#import "LayoutParams.h"
#import "ViewStyle.h"
#import "ImageCache.h"

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
- (void)releaseData;
- (void)setImage:(CGImageRef)data;
- (void)addImageUrl:(NSString *)url width:(int)width height:(int)height;
- (void)flush;
- (void)setStyle:(NSString *)key value:(NSString *)value selector:(StyleSelector)selector;
- (void)applyStyles:(BOOL)animate;
- (void)setIntValue:(int)value;
- (void)setTextValue:(NSString *)value;
- (void)reshapeTable:(int)value;
- (void)stop;
- (void)switchStyle:(StyleSelector)selector;

@property (nonatomic, assign) NSInteger viewId; // don't use id as it is reserved word in objective-c
@property (nonatomic, strong) id view;
@property (nonatomic, strong) id containerView;
@property (nonatomic, strong) LayoutParams * layoutParams;
@property (nonatomic, assign) NSInteger level;
@property (nonatomic, strong) ViewStyle * normalStyle;
@property (nonatomic, strong) ViewStyle * activeStyle;
@property (nonatomic, strong) ViewStyle * currentStyle;
@property (nonatomic, strong) ImageCache * imageCache;
@property (nonatomic, strong) UITabBar * tabBar;
@end
