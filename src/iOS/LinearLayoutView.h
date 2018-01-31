#import <UIKit/UIKit.h>

#import "LayoutParams.h"

typedef enum {
    LinearLayoutViewOrientationVertical,
    LinearLayoutViewOrientationHorizontal
} LinearLayoutViewOrientation;

@interface LinearLayoutView : UIView

@property (nonatomic, readonly) NSMutableArray *items;
@property (nonatomic, assign) LinearLayoutViewOrientation orientation;
@property (nonatomic, readonly) CGFloat layoutOffset;
@property (nonatomic, assign) BOOL autoAdjustFrameSize;

- (void)addItem:(LayoutParams *)linearLayoutItem;
- (void)removeItem:(LayoutParams *)linearLayoutItem;
- (void)removeAllItems;

- (void)insertItem:(LayoutParams *)newItem beforeItem:(LayoutParams *)existingItem;
- (void)insertItem:(LayoutParams *)newItem afterItem:(LayoutParams *)existingItem;
- (void)insertItem:(LayoutParams *)newItem atIndex:(NSUInteger)index;

- (void)moveItem:(LayoutParams *)movingItem beforeItem:(LayoutParams *)existingItem;
- (void)moveItem:(LayoutParams *)movingItem afterItem:(LayoutParams *)existingItem;
- (void)moveItem:(LayoutParams *)movingItem toIndex:(NSUInteger)index;

- (void)swapItem:(LayoutParams *)firstItem withItem:(LayoutParams *)secondItem;

@end
