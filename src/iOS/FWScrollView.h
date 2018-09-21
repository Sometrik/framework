#import <UIKit/UIKit.h>

#import "LayoutParams.h"

@interface FWScrollView : UIScrollView

- (void)layoutSubviews;
- (void)updateVisibility:(CGRect)bounds;
- (NSInteger)indexForVisiblePage;
- (void)setPage:(NSInteger)page;
- (BOOL)reselectCurrentPage;
- (void)showPage:(NSInteger)page animated:(BOOL)animated;
- (void)flush;

- (LayoutParams *)findParams:(UIView *)view;
- (void)addItem:(LayoutParams *)linearLayoutItem;
- (void)removeItem:(LayoutParams *)linearLayoutItem;
- (void)removeAllItems;
- (void)insertItem:(LayoutParams *)newItem beforeItem:(LayoutParams *)existingItem;
- (void)insertItem:(LayoutParams *)newItem afterItem:(LayoutParams *)existingItem;
- (void)insertItem:(LayoutParams *)newItem atIndex:(NSUInteger)index;
- (void)moveItem:(LayoutParams *)movingItem toIndex:(NSUInteger)index;
- (void)swapItem:(LayoutParams *)firstItem withItem:(LayoutParams *)secondItem;

@property (nonatomic, strong) NSMutableArray *items;
@property (nonatomic, strong) NSLayoutConstraint *topConstraint;
@property (nonatomic, strong) NSLayoutConstraint *leftConstraint;
@property (nonatomic, strong) NSLayoutConstraint *widthConstraint;
@property (nonatomic, strong) NSLayoutConstraint *heightConstraint;
@property (nonatomic, assign) int currentPage;
@property (nonatomic, assign) int currentPageInternalId;
@end
