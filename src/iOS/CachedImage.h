#import <UIKit/UIKit.h>

@interface CachedImage : NSObject

@property (nonatomic, strong) UIImage * image;
@property (nonatomic, strong) NSString * key;
@property (nonatomic, assign) NSInteger refcnt;

- (id)initWithImage:(UIImage *)image key:(NSString *)key;

@end
