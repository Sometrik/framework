#import "ImageCache.h"

#import "CachedImage.h"
#import "FWImageView.h"

@implementation ImageCache;

- (id)init {
    self = [super init];
    if (self) {
        self.cachedIcons = [[NSMutableDictionary alloc] init];
        self.cachedImages = [[NSMutableDictionary alloc] init];
        self.subscriptions = [[NSMutableDictionary alloc] init];
    }
    return self;
}

- (UIImage *)loadIcon:(NSString *)filename
{
    UIImage * image = [self.cachedIcons objectForKey:filename];
    if (image == nil) {
        NSString *maskFilePath = [[NSBundle mainBundle] pathForResource:filename ofType:nil];
        CGDataProviderRef dataProvider = CGDataProviderCreateWithFilename([maskFilePath UTF8String]);
        CGImageRef imageRef = CGImageCreateWithPNGDataProvider(dataProvider, NULL, true, kCGRenderingIntentDefault);

        if (imageRef != 0) {
            image = [UIImage imageWithCGImage:imageRef scale:3.0f orientation:UIImageOrientationUp];
            CGImageRelease(imageRef);
        }
	
        CGDataProviderRelease(dataProvider);

        if (image != nil) {
            [self.cachedIcons setObject:image forKey:filename];
        }
    }
    
    return image;
}

- (void)subscribeImage:(FWImageView *)view url:(NSString *)url width:(NSInteger)width height:(NSInteger)height
{
    NSString * key = [NSString stringWithFormat:@"%@/%d/%d", url, (int)width, (int)height];
    CachedImage * ci = [self.cachedImages objectForKey:key];
    if (ci) {
        ci.refcnt++;
        // [view setCachedImage:ci];
    } else {
        NSMutableArray * views = [self.subscriptions objectForKey:key];
        if (views == nil) {
            views = [[NSMutableArray alloc] init];
            [self.subscriptions setObject:view forKey:key];
        }
        [views addObject:view];
    }
}

- (void)releaseImage:(CachedImage *)ci
{
    ci.refcnt--;
    if (ci.refcnt <= 0) {
        [self.cachedImages removeObjectForKey:ci.key];
    }
}

- (void)dispatchImage:(UIImage *)image url:(NSString *)url width:(NSInteger)width height:(NSInteger)height
{
    NSString * key = [NSString stringWithFormat:@"%@/%d/%d", url, (int)width, (int)height];
    NSArray * views = [self.subscriptions objectForKey:key];
    if (views) {
        if ([views count] > 0) {
            CachedImage * ci = [[CachedImage alloc] initWithImage:image key:key];
            for (FWImageView * view in views) {
                ci.refcnt++;
                // [view setCachedImage:ci];
            }
        }
        [self.subscriptions removeObjectForKey:key];
    }
}

@end
