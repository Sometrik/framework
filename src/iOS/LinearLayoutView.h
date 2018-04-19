#import "FWLayoutView.h"

typedef enum {
    LinearLayoutViewOrientationVertical,
    LinearLayoutViewOrientationHorizontal
} LinearLayoutViewOrientation;

@interface LinearLayoutView : FWLayoutView

@property (nonatomic, assign) LinearLayoutViewOrientation orientation;

@end
