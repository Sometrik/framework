#import <UIKit/UIKit.h>
#import "FWLayoutView.h"
#import "LayoutParams.h"

typedef enum {
    LinearLayoutViewOrientationVertical,
    LinearLayoutViewOrientationHorizontal
} LinearLayoutViewOrientation;

@interface LinearLayoutView : FWLayoutView

@property (nonatomic, assign) LinearLayoutViewOrientation orientation;

- (LayoutParams*)findParams:(UIView *)view;

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
