#import "CachedImage.h"

@implementation CachedImage

- (id)init {
    self = [super init];
    if (self) {
        self.image = nil;
	self.key = nil;
	self.refcnt = 0;	
    }
    return self;
}

- (id)initWithImage:(UIImage*)image key:(NSString *)key {
    self = [super init];
    if (self) {
        self.image = image;
	self.key = key;
	self.refcnt = 0;
    }
    return self;
}

@end



