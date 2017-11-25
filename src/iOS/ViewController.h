#import <UIKit/UIKit.h>

@interface ViewController : UIViewController
- (void)viewWillTransitionToSize: (CGSize)size withTransitionCoordinator:(id)coordinator;
- (void)didReceiveMemoryWarning;

- (void)createTextFieldWithId: (int)viewId parentId:(int)parentId;
- (void)setStyle: (int)viewId key:(NSString *)key value:(NSString *)value;
- (void)setVisibility:(int)viewId visibility:(int)visibility;

- (void)createFrameViewWithId:(int)viewId parentId:(int)parentId;
- (void)createLinearLayoutWithId:(int)viewId parentId:(int)parentId direction:(int)direction;
- (void)createFrameLayoutWithId:(int)viewId parentId:(int)parentId;
- (void)createTextWithId:(int)viewId parentId:(int)parentId value:(NSString *)value;
- (void)createButtonWithId:(int)viewId parentId:(int)parentId caption:(NSString *)caption;
- (void)createSwitchWithId:(int)viewId parentId:(int)parentId;
- (void)createImageWithId:(int)viewId parentId:(int)parentId filename:(NSString *)filename;
- (void)createScrollLayoutWithId:(int)viewId parentId:(int)parentId;
- (void)createPageLayoutWithId:(int)viewId parentId:(int)parentId;
- (void)createEventLayoutWithId:(int)viewId parentId:(int)parentId;
- (void)createTabBar:(int)viewId parentId:(int)parentId;
- (void)createTabBarItem:(int)viewId parentId:(int)parentId title:(NSString *)title;
- (void)createActivityIndicatorWithId:(int)viewId parentId:(int)parentId;

- (void)setIntValue:(int)viewId value:(int)value;
- (void)setTextValue:(int)viewId value:(NSString *)value;
- (void)setImage:(int)viewId data:(UIImage *)data;
- (void)addImageUrl:(int)viewId url:(NSString *)url width:(int)width height:(int)height;

- (void)addToParent:(int)parentId view:(UIView*)view;
- (void)removeView:(int)viewId;

- (void)sendIntValue:(int)viewId value:(int)value;
- (void)sendTextValue:(int)viewId value:(NSString *)value;

@end
