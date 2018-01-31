#import <UIKit/UIKit.h>

#import "LayoutParams.h"

@interface ViewManager : NSObject
- (id)init;
- (void)clear;
- (void)setConstraints;
- (void)setImage:(UIImage *)data;
- (void)addImageUrl:(NSString *)url width:(int)width height:(int)height;
- (void)setStyle:(NSString *)key value:(NSString *)value;
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
