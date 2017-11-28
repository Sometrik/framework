#import <Foundation/Foundation.h>

@interface EventWrapper : NSObject
@property (nonatomic) NSInteger targetElementId;
@property (nonatomic) void * eventPtr;
@end
