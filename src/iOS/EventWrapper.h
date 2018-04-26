#import <Foundation/Foundation.h>

@interface EventWrapper : NSObject
@property (nonatomic, assign) NSInteger targetElementId;
@property (nonatomic, assign) void * eventPtr;
@end
