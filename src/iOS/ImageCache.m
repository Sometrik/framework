#import "ImageCache.h"

@implementation ImageCache;

- (id)init {
    self = [super init];
    if (self) {
        self.cachedIcons = [[NSMutableDictionary alloc] init];
    }
    return self;
}

- (UIImage *)loadIcon:(NSString *)filename
{
    UIImage * image = [self.cachedICons objectForKey:filename];
    if (image == nil) {
        NSString *maskFilePath = [[NSBundle mainBundle] pathForResource:filename ofType:nil];
        CGDataProviderRef dataProvider = CGDataProviderCreateWithFilename([maskFilePath UTF8String]);
        CGImageRef imageRef = CGImageCreateWithPNGDataProvider(dataProvider, NULL, true, kCGRenderingIntentDefault);

	UIImage * image;
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

@end
