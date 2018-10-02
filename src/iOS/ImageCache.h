#import <UIKit/UIKit.h>

@interface ImageCache : NSObject
@property (nonatomic, strong) NSMutableDictionary * cachedIcons;
@property (nonatomic, strong) NSMutableDictionary * cachedImages;
@property (nonatomic, strong) NSMutableDictionary * subscriptions;

- (id)init;
- (UIImage *)loadIcon:(NSString *)filename;

@end
