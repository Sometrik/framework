#import <UIKit/UIKit.h>

@interface ViewManager : NSObject
- (id)init;
- (void)clear;
- (void)setImage:(UIImage *)data;
- (void)addImageUrl:(NSString *)url width:(int)width height:(int)height;
- (void)setStyle:(NSString *)key value:(NSString *)value;
- (void)setIntValue:(int)value;
- (void)setTextValue:(NSString *)value;

@property (nonatomic) NSInteger viewId; // don't use id as it is reserved word in objective-c
@property id view;
@property BOOL constraintsSet;
@end
