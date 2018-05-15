#import <UIKit/UIKit.h>

@interface ImageCache : NSObject
@property (nonatomic, strong) NSMutableDictionary * cachedIcons;

- (id)init;
- (UIImage *)loadIcon:(NSString *)filename;

@end
