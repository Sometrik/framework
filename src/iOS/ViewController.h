#import <UIKit/UIKit.h>

#import "EventWrapper.h"
#import "ViewManager.h"

typedef enum
{
    AnimationStyleNone = 0,
    AnimationStyleLeftToRight,
    AnimationStyleRightToLeft,
    //AnimationStyleBottomToTop, // for some reason constraints not working with this
    AnimationStyleTopToBottom
} AnimationStyle;

@interface ViewController : UIViewController
- (void)viewWillTransitionToSize: (CGSize)size withTransitionCoordinator:(id)coordinator;
- (void)didReceiveMemoryWarning;

- (void)setVisibility:(int)viewId visibility:(int)visibility;

- (void)createTextFieldWithId: (int)viewId parentId:(int)parentId value:(NSString*)value;
- (void)createTextViewWithId: (int)viewId parentId:(int)parentId value:(NSString*)value;
- (void)createFrameViewWithId:(int)viewId parentId:(int)parentId;
- (void)createLinearLayoutWithId:(int)viewId parentId:(int)parentId direction:(int)direction;
- (void)createFrameLayoutWithId:(int)viewId parentId:(int)parentId;
- (void)createTextWithId:(int)viewId parentId:(int)parentId value:(NSString *)value autolink:(BOOL)autolink;
- (void)createButtonWithId:(int)viewId parentId:(int)parentId caption:(NSString *)caption;
- (void)createSwitchWithId:(int)viewId parentId:(int)parentId;
- (void)createImageWithId:(int)viewId parentId:(int)parentId filename:(NSString *)filename width:(int)width height:(int)height;
- (void)createScrollLayoutWithId:(int)viewId parentId:(int)parentId;
- (void)createPageLayoutWithId:(int)viewId parentId:(int)parentId;
- (void)createEventLayoutWithId:(int)viewId parentId:(int)parentId;
- (void)createNavigationBar:(int)viewId parentId:(int)parentId;
- (void)createTabBar:(int)viewId parentId:(int)parentId;
- (void)createNavigationView:(int)viewId;
- (void)createTabBarItem:(int)viewId parentId:(int)parentId title:(NSString *)title;
- (void)createActivityIndicatorWithId:(int)viewId parentId:(int)parentId;
- (void)createPageControlWithId:(int)viewId parentId:(int)parentId numPages:(int)numPages;
- (void)createPickerWithId:(int)viewId parentId:(int)parentId;
- (void)createActionSheetWithId:(int)viewId parentId:(int)parentId title:(NSString *)title;
- (void)createDialogWithId:(int)viewId parentId:(int)parentId title:(NSString *)title animationStyle:(AnimationStyle)style;
- (void)createTimer:(int)viewId interval:(double)interval;
- (void)createWebBrowserWithUrl:(NSString *)url;

- (void)setImageFromThread:(int)viewId data:(CGImageRef)data;
- (void)sendCommandsFromThread:(NSArray*)data;
- (void)handleCommands:(NSArray*)data;
- (void)showNavigationViewWithAnimation:(BOOL)animate;
- (void)hideNavigationViewWithAnimation:(BOOL)animate;

- (void)addToParent:(int)parentId view:(UIView*)view;
- (void)removeView:(int)viewId;
- (void)reorderChildWithId:(int)viewId parentId:(int)parentId newPosition:(int)position;
- (void)removeChildWithId:(int)viewId parentId:(int)parentId;
- (void)addOption:(int)viewId optionId:(int)optionId title:(NSString *)title;
- (void)sendIntValue:(int)viewId value:(int)value;
- (void)sendTextValue:(int)viewId value:(NSString *)value;

- (void)sendEventToMainThread:(EventWrapper*) event;

- (void)setTitle:(NSString*)title;
- (void)setBackButtonVisibility:(BOOL)v;

- (ViewManager *)getViewManager:(int)viewId;

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer: (UIGestureRecognizer *)otherGestureRecognizer;

@end
