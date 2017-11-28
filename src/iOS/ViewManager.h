#import <UIKit/UIKit.h>

@interface ViewManager : NSObject
- (void)setImage:(UIImage *)data;
- (void)addImageUrl:(NSString *)url width:(int)width height:(int)height;
- (void)setStyle:(NSString *)key value:(NSString *)value;
- (void)setIntValue:(int)value;
- (void)setTextValue:(NSString *)value;

@property (nonatomic) NSInteger id;
@property id view;
@end
