#import "ViewController.h"

#include <FWApplication.h>
#include <FWDefs.h>
#include <SysInfoEvent.h>
#include <VisibilityUpdateEvent.h>
#include <UserInteractionEvent.h>

#include "iOSMainThread.h"

#import "ImageWrapper.h"
#import "InAppPurchaseManager.h"
#import "FWImageView.h"
#import "PaddedLabel.h"
#import "LinearLayoutView.h"
#import "FrameLayoutView.h"
#import "DialogView.h"
#import "FWScrollView.h"
#import "FWPicker.h"
#import "FWButton.h"
#import "NativeCommand.h"
#import "ImageCache.h"

#import <WebKit/WebKit.h>

#include <memory>

using namespace std;

// Pointer to main thread
std::shared_ptr<iOSMainThread> mainThread;

// Declare C++ function
extern FWApplication * applicationMain();

@interface ViewController () <UIScrollViewDelegate, UITabBarDelegate, InAppPurchaseManagerDelegate, FWImageViewDelegate, UITextFieldDelegate, UITextViewDelegate, UIGestureRecognizerDelegate, PaddedLabelDelegate, WKUIDelegate, WKNavigationDelegate, UIBarPositioningDelegate, UINavigationControllerDelegate, UIImagePickerControllerDelegate, UINavigationBarDelegate>
@property (nonatomic, strong) NSMutableDictionary *viewsDictionary;
@property (nonatomic, strong) UIView *sideMenuView;
@property (nonatomic, strong) UIView *backgroundOverlayView;
@property (nonatomic, strong) UITabBar *tabBar;
@property (nonatomic, strong) UINavigationBar *navBar;
@property (nonatomic, strong) UINavigationItem *navItem;
@property (nonatomic, strong) UILabel *navBarTitle;
@property (nonatomic, strong) UILabel *navBarSubtitle;
@property (nonatomic, strong) NSLayoutConstraint *statusBarTopConstraint;
@property (nonatomic, strong) FWScrollView *pageView;
@property (nonatomic, strong) NSMutableArray *dialogIds;
@property (nonatomic, assign) int activeViewId;
@property (nonatomic, assign) BOOL sideMenuPanned;
@property (nonatomic, strong) WKWebView *webView;
//@property (nonatomic, strong) InAppPurchaseManager *inAppPurchaseManager;
@property (nonatomic, strong) NSString * currentTitle;
@property (nonatomic, strong) FWPicker * currentPicker;
@property (nonatomic, assign) int currentPickerSelection;
@property (nonatomic, strong) UIView * currentPickerHolder;
@property (nonatomic, strong) NSLayoutConstraint *currentPickerHolderTopConstraint;
@property (nonatomic, strong) NSLayoutConstraint *currentPickerHolderLeftConstraint;
@property (nonatomic, strong) NSLayoutConstraint *currentPickerHolderRightConstraint;
@property (nonatomic, strong) NSLayoutConstraint *currentPickerHolderBottomConstraint;
@property (nonatomic, strong) NSLayoutConstraint *activeViewRightConstraint;
@property (nonatomic, strong) NSLayoutConstraint *activeViewLeftConstraint;
@property (nonatomic, strong) UIScreenEdgePanGestureRecognizer *panEdgeGestureRecognizer;
@property (nonatomic, assign) BOOL keyboardVisible;
@property (nonatomic, assign) AnimationStyle pickerAnimationStyle;
@property (nonatomic, strong) NSURL *currentURL;
@property (nonatomic, strong) NSMutableDictionary *frameViewConstraints; // constraints saved (for animation purposes), viewId is the key.
@property (nonatomic, strong) NSMutableDictionary *dialogConstraints;
@property (nonatomic, assign) AnimationStyle dialogAnimationStyle;
@property (nonatomic, assign) BOOL sideMenuSwiped;
@property (nonatomic, strong) NSDate *sideMenuPanStartedDate;
@property (nonatomic, assign) CGFloat sideMenuLastTouchLocationX;
@property (nonatomic, assign) CGFloat statusBarHeight;
@property (nonatomic, strong) UIView *titleView;
@property (nonatomic, strong) ImageCache * imageCache;
@end

static const NSTimeInterval animationDuration = 0.4;
static const CGFloat backgroundOverlayViewAlpha = 0.5;
static const CGFloat sideMenuOpenSpaceWidth = 100.0;

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    self.activeViewId = 0;
    self.currentTitle = nil;
    self.dialogIds = [[NSMutableArray alloc] init];
    self.statusBarTopConstraint = nil;

    self.imageCache = [[ImageCache alloc] init];

    // CGFloat statusBarHeight = [UIApplication sharedApplication].statusBarFrame.size.height;
    self.view.layoutMargins = UIEdgeInsetsMake(0, 0, 0, 0);
  
    self.backgroundOverlayView = [self createBackgroundOverlay:self.view];
    self.backgroundOverlayView.hidden = YES;
    self.backgroundOverlayView.tag = 0;
  
    //self.inAppPurchaseManager = [InAppPurchaseManager sharedInstance];

    self.currentPicker = nil;
    self.currentPickerSelection = 0;
    self.currentPickerHolder = nil;
    
    // Creating the C++ app
    std::shared_ptr<FWApplication> application(applicationMain());
    
    NSUserDefaults * defaults = [NSUserDefaults standardUserDefaults];
    FWPreferences preferences;
    NSArray *keys = [[defaults dictionaryRepresentation] allKeys];
    for (NSString * key in keys) {
      NSString * value = [defaults valueForKey:key];
      NSLog(@"loaded data: %@ %@", key, value);  
      if ([value isKindOfClass:[NSString class]]) {
        preferences.setText([key UTF8String], [value UTF8String]);
      }
    }
    application->setPreferences(preferences);
	
    NSString *version = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
    NSString *build = [[[NSBundle mainBundle] infoDictionary] objectForKey:(NSString *)kCFBundleVersionKey];

    application->setVersion([version UTF8String]);
    application->setBuildNumber([build UTF8String]);

    float scale = [[UIScreen mainScreen] scale];
  
    auto runner = std::make_shared<iOSAppRunner>();
    mainThread = make_shared<iOSMainThread>(application, runner);
    mainThread->viewController = self;
//    mainThread->setActualDisplayWidth(screenWidth);
//    mainThread->setActualDisplayHeight(screenHeight);
    mainThread->setDisplayScale(scale);
  
    NSLog(@"starting app, scale = %f, version = %@, build = %@", scale, version, build);

    [self addView:self.view withId:1];

    mainThread->startThread(mainThread);
    
    // Add notification handlers to catch notifications when keyboard opens and closes
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(handleKeyboardWillShowNotification:) name:UIKeyboardWillShowNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(handleKeyboardWillHideNotification:) name:UIKeyboardWillHideNotification object:nil];
    
    // Add tap gesture recognizer to self.view so when tapped, keyboard should close
    UITapGestureRecognizer *tapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(backgroundTapped:)];
    tapGestureRecognizer.delegate = self;
    [self.view addGestureRecognizer:tapGestureRecognizer];
    
    // For testing in-app-purchases
    [self getInAppPurchaseProductsForIdentifiers:@"com.samiramo.sometriktest.unlock_for_week"];
    
}

- (UIBarPosition)positionForBar:(id<UIBarPositioning>)bar {
    return UIBarPositionTopAttached;
}

- (void)handleKeyboardWillShowNotification:(NSNotification *)notification
{
    id frameEnd = notification.userInfo[@"UIKeyboardFrameEndUserInfoKey"];
    if (frameEnd != nil) {
        CGRect rect = [frameEnd CGRectValue];
        [self keyboardTopPositionChanged:(int)rect.origin.y];
    }
    self.keyboardVisible = YES;
}

- (void)handleKeyboardWillHideNotification:(NSNotification *)notification
{
    id frameEnd = notification.userInfo[@"UIKeyboardFrameEndUserInfoKey"];
    if (frameEnd != nil) {
        CGRect rect = [frameEnd CGRectValue];
        [self keyboardTopPositionChanged:(int)rect.origin.y];
    }
    self.keyboardVisible = NO;
}

- (void)keyboardTopPositionChanged:(int)pos
{
    for (NSNumber * dialogId in self.dialogIds) {
        ViewManager * viewManager = [self getViewManager:dialogId.intValue];
        DialogView * dialog = (DialogView *)viewManager.containerView;
        dialog.maxBottomConstraint.constant = -(self.view.frame.size.height - pos + 15);
        [dialog setNeedsLayout];
    }
}

- (void)backgroundTapped:(UITapGestureRecognizer *)gestureRecognizer
{
    [self.view endEditing:YES];
}

- (void)viewWillTransitionToSize: (CGSize)size withTransitionCoordinator:(id)coordinator
{
    if (self.sideMenuView != nil) {
        CGRect frame = CGRectMake(CGRectGetMinX(self.view.bounds), CGRectGetMinY(self.view.bounds), size.width-sideMenuOpenSpaceWidth, size.height);
        self.sideMenuView.frame = frame;
    }
    if (self.webView != nil) {
        for (UIView *view in self.webView.subviews) {
            if ([view isKindOfClass:UINavigationBar.class]) {
                view.frame = CGRectMake(0, 0, size.width, view.frame.size.height);
            }
        }
    }
    if (self.currentPickerHolder != nil) {
        for (UIView *view in self.currentPickerHolder.subviews) {
            if ([view isKindOfClass:UINavigationBar.class]) {
                view.frame = CGRectMake(0, 20, size.width, 44);
                NSLog(@"*** view.frame = %@", NSStringFromCGRect(view.frame));
            }
        }
    }
    CGFloat titleViewWidth = size.width - 140;
    self.statusBarHeight = [UIApplication sharedApplication].statusBarFrame.size.height;
    if (self.statusBarTopConstraint) {
        if (size.width > size.height) {
            self.statusBarTopConstraint.constant = 0.0;
            // self.titleView.frame = CGRectMake(size.width/2 - titleViewWidth/2, 0, titleViewWidth, 44)
        } else {
            self.statusBarTopConstraint.constant = self.statusBarHeight;
            // self.titleView.frame = CGRectMake(size.width/2 - titleViewWidth/2, 0, titleViewWidth, 28);
        }
    }

  [coordinator animateAlongsideTransition:^(id<UIViewControllerTransitionCoordinatorContext> context) {

        // Place code here to perform animations during the rotation.
        // You can pass nil or leave this block empty if not necessary.

    } completion:^(id<UIViewControllerTransitionCoordinatorContext> context) {

        // Code here will execute after the rotation has finished.
        // Equivalent to placing it in the deprecated method -[didRotateFromInterfaceOrientation:]
    }];
}

- (void)didReceiveMemoryWarning {
    SysInfoEvent ev(SysInfoEvent::MEMORY_WARNING);
    mainThread->sendEvent(mainThread->getApplication().getInternalId(), ev);
    [super didReceiveMemoryWarning];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    // make side menu open by swiping from the left edge of the screen
    self.panEdgeGestureRecognizer = [[UIScreenEdgePanGestureRecognizer alloc] initWithTarget:self action:@selector(edgeSwiped:)];
    self.panEdgeGestureRecognizer.edges = UIRectEdgeLeft;
    self.panEdgeGestureRecognizer.delegate = self;
    [self.view addGestureRecognizer:self.panEdgeGestureRecognizer];
    
    self.statusBarHeight = [UIApplication sharedApplication].statusBarFrame.size.height;
    if (self.statusBarTopConstraint != nil) {
        if (self.view.frame.size.width > self.view.frame.size.height) {
            self.statusBarTopConstraint.constant = 0.0;
        } else {
            self.statusBarTopConstraint.constant = self.statusBarHeight;
        }
    }
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    if (self.navBar) {
        [self.navBar.superview bringSubviewToFront:self.navBar];
    }
    if (self.sideMenuView) {
        [self.view bringSubviewToFront:self.backgroundOverlayView];
        [self.view bringSubviewToFront:self.sideMenuView];
    }
}

- (UIView *)createBackgroundOverlay:(UIView *)parentView
{
    // create backgroundoverlay view that's behind sidePanel and dialog and if clicked closes the panel
    UIView * view = [[UIView alloc] init];
    view.backgroundColor = UIColor.blackColor;
    view.translatesAutoresizingMaskIntoConstraints = false;

    [parentView addSubview:view];

    NSLayoutConstraint *topConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
    NSLayoutConstraint *leftConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
    NSLayoutConstraint *rightConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
    NSLayoutConstraint *bottomConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeBottom multiplier:1.0f constant:0];
    [view.superview addConstraints:@[topConstraint, leftConstraint, rightConstraint, bottomConstraint]];

    UITapGestureRecognizer *tapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(backgroundOverlayViewTapped:)];
    [view addGestureRecognizer:tapGestureRecognizer];

    return view;
}

- (void)backgroundOverlayViewTapped:(UITapGestureRecognizer *)gesture
{
    int viewId = (int)gesture.view.tag;

    if (gesture.view.superview == self.currentPickerHolder) {
        NSLog(@"Removing picker holder on background click");
        [self.currentPickerHolder removeFromSuperview];
        self.currentPicker = nil;
        self.currentPickerHolder = nil;
    } else if (viewId != 0 && self.dialogIds.count > 0) {
        NSLog(@"background overlay tapped");
        int dialogId = [[self.dialogIds lastObject] intValue];
        if (dialogId == viewId) {
	    NSLog(@"Sending cancel value for dialog %d", viewId);
            [self sendIntValue:dialogId value:0];
	    NSLog(@"Cancel value done");
            [self.dialogIds removeLastObject];
        }
    } else if (!self.sideMenuView.isHidden) {
        [self hideNavigationViewWithAnimation:YES];
    }
}

- (void)hideBackgroundOverlayViewWithAnimation:(BOOL)animate
{
    if (animate) {
        [UIView animateWithDuration:animationDuration animations:^{
            self.backgroundOverlayView.alpha = 0.0;
        } completion:^(BOOL finished) {
            self.backgroundOverlayView.hidden = YES;
        }];
    } else {
        self.backgroundOverlayView.alpha = 0.0;
        self.backgroundOverlayView.hidden = YES;
    }
}

- (UIStatusBarStyle)preferredStatusBarStyle {
    // UIStatusBarStyleDefault:      text is black
    // UIStatusBarStyleLightContent: text is white
    return UIStatusBarStyleDefault;
}

- (void)createTextFieldWithId: (int)viewId parentId:(int)parentId value:(NSString*)value {
    UITextField* text = [[UITextField alloc] init];
    text.tag = viewId;
    text.borderStyle = UITextBorderStyleBezel;
    text.autocorrectionType = UITextAutocorrectionTypeNo;
    text.keyboardType = UIKeyboardTypeDefault;
    text.returnKeyType = UIReturnKeyDone;
    text.clearButtonMode = UITextFieldViewModeWhileEditing;
    text.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
    text.delegate = self;
    text.translatesAutoresizingMaskIntoConstraints = false;
    text.text = value;
    // text.borderStyle = UITextBorderStyleRoundedRect;
    [text addTarget:self action:@selector(textFieldChanged:) forControlEvents:UIControlEventEditingChanged];
    [self addView:text withId:viewId];
    [self addToParent:parentId view:text];
}

- (void)createTextViewWithId: (int)viewId parentId:(int)parentId value:(NSString*)value {
    UITextView *view = [[UITextView alloc] init];
    view.tag = viewId;
    // view.borderStyle = UITextBorderStyleRoundedRect;
    view.keyboardType = UIKeyboardTypeDefault;
    view.returnKeyType = UIReturnKeyDone;
    // view.clearButtonMode = UITextFieldViewModeWhileEditing;
    view.delegate = self;
    view.textColor = [UIColor blackColor];
    view.dataDetectorTypes = UIDataDetectorTypeAll;
    [view setEditable:YES];
    [view setUserInteractionEnabled:YES];
    view.allowsEditingTextAttributes = NO;
    // view.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
    view.translatesAutoresizingMaskIntoConstraints = false;
    view.text = value;
    [self addView:view withId:viewId];
    [self addToParent:parentId view:view];
}

- (void)textFieldChanged:(UITextField *)sender
{
    int viewId = (int)sender.tag;
    [self sendTextValue:viewId value:sender.text];
}

- (void)textViewDidChange:(UITextView *)sender {
    int viewId = (int)sender.tag;
    NSLog(@"viewId = %d", viewId);
    NSLog(@"textView.text = %@", sender.text);
    [self sendTextValue:viewId value:sender.text];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    return NO;
}

- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text {
    NSRange resultRange = [text rangeOfCharacterFromSet:[NSCharacterSet newlineCharacterSet] options:NSBackwardsSearch];
    if ([text length] == 1 && resultRange.location != NSNotFound) {
        [textView resignFirstResponder];
        return NO;
    }

    return YES;
}

- (void)setVisibility:(int)viewId visibility:(int)visibility
{
    UIView *view = [self viewForId:viewId];
    
    if ([view isEqual:self.sideMenuView]) {
        if (visibility == 0) {
            [self hideNavigationViewWithAnimation:YES];
        } else {
            [self showNavigationViewWithAnimation:YES];
        }
    } else {
        BOOL viewHidden = visibility == 0 ? true : false;
        if (view && ((viewHidden && !view.hidden) || (!viewHidden && view.hidden))) {
            view.hidden = viewHidden;
            if ([view.superview isKindOfClass:FWLayoutView.class]) {
                FWLayoutView * layout = (FWLayoutView*)view.superview;
            	[layout relayoutAll];
            } else if ([view.superview isKindOfClass:FWScrollView.class]) {
                FWScrollView * layout = (FWScrollView*)view.superview;
            	[layout setNeedsLayout];
                if (layout.pagingEnabled && [layout reselectCurrentPage]) {
                    [self sendIntValue:(int)layout.tag value:layout.currentPage];
                }
            }
        }
    }
}

- (void)sendVisibilityUpdate:(int)viewId
{
    VisibilityUpdateEvent ev;
    mainThread->sendEvent(viewId, ev);
}

// Lazy initialization
- (NSMutableDictionary *)viewsDictionary
{
    if (!_viewsDictionary) {
        _viewsDictionary = [[NSMutableDictionary alloc] init];
    }
    return _viewsDictionary;
}

- (NSMutableDictionary *)frameViewConstraints
{
    if (!_frameViewConstraints) {
        _frameViewConstraints = [[NSMutableDictionary alloc] init];
    }
    return _frameViewConstraints;
}

- (NSMutableDictionary *)dialogConstraints
{
    if (!_dialogConstraints) {
        _dialogConstraints = [[NSMutableDictionary alloc] init];
    }
    return _dialogConstraints;
}

- (void)createFrameViewWithId:(int)viewId parentId:(int)parentId
{
    ViewManager * parentViewManager = [self getViewManager:parentId];
    if (parentViewManager == nil) {
        NSLog(@"Element %d missing in createFrameViewWithIdt", parentId);
        return;
    }

    FrameLayoutView *view = [[FrameLayoutView alloc] init];
    view.tag = viewId;

    [self addView:view withId:viewId];

    UIView * parentView = (UIView *)parentViewManager.containerView;
    [parentView addSubview:view];
    
    if (self.activeViewId == 0) {
        self.activeViewId = viewId;
    } else {
        view.hidden = YES;
    }
    
    NSLayoutConstraint *topConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
    NSLayoutConstraint *leftConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
    NSLayoutConstraint *rightConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
    NSLayoutConstraint *bottomConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeBottom multiplier:1.0f constant:0];
    NSArray *constraintArray = @[leftConstraint, rightConstraint];
    @try {
        [self.frameViewConstraints setObject:constraintArray forKey:[NSString stringWithFormat:@"%d", viewId]];
    }
    @catch (NSException *e) {
        NSLog(@"exception: %@", e);
        @throw;
    }
    @finally {
        [view.superview addConstraints:@[topConstraint, leftConstraint, rightConstraint, bottomConstraint]];
        
        if (self.navBar) {
            [self.navBar.superview bringSubviewToFront:self.navBar];
        }
    }
}

- (void)createLinearLayoutWithId:(int)viewId parentId:(int)parentId direction:(int)direction
{
    LinearLayoutView *layout = [[LinearLayoutView alloc] init];
    if (direction == 1) {
        layout.orientation = LinearLayoutViewOrientationVertical;
    } else {
        layout.orientation = LinearLayoutViewOrientationHorizontal;
    }
    layout.layoutMargins = UIEdgeInsetsMake(0, 0, 0, 0);
    layout.tag = viewId;
    [self addView:layout withId:viewId];
    [self addToParent:parentId view:layout];
}

- (void)createFrameLayoutWithId:(int)viewId parentId:(int)parentId
{
    FrameLayoutView *view = [[FrameLayoutView alloc] init];
    view.layoutMargins = UIEdgeInsetsMake(0, 0, 0, 0);
    view.tag = viewId;
    [self addView:view withId:viewId];
    [self addToParent:parentId view:view];
}

- (void)createTextWithId:(int)viewId parentId:(int)parentId value:(NSString*)value autolink:(BOOL)autolink markdown:(BOOL)markdown
{
    PaddedLabel *label = [[PaddedLabel alloc] init];
    label.tag = viewId;
    label.delegate = self;
    label.autolink = autolink;
    label.markdown = markdown;
    if (autolink || markdown) {
        if (autolink) label.userInteractionEnabled = YES;
        label.attributedText = [label createAttributedString:value];
	label.origText = value;
    } else {
        label.text = value;
    }
    label.numberOfLines = 0; // as many lines as needed
    label.lineBreakMode = NSLineBreakByWordWrapping;
    label.translatesAutoresizingMaskIntoConstraints = false;
    label.preferredMaxLayoutWidth = self.view.frame.size.width;
    [self addView:label withId:viewId];
    [self addToParent:parentId view:label];
}

- (void)createButtonWithId:(int)viewId parentId:(int)parentId caption:(NSString *)caption
{
    FWButton *button = [[FWButton alloc] init];
    button.tag = viewId;
    [button setTitle:caption forState:UIControlStateNormal];
    [button setTitleColor:UIColor.blackColor forState:UIControlStateNormal];
    [button addTarget:self action:@selector(buttonPushed:) forControlEvents:UIControlEventTouchUpInside];
    [button addTarget:self action:@selector(buttonTouchDown:) forControlEvents:UIControlEventTouchDown];
    [button addTarget:self action:@selector(buttonTouchUp:) forControlEvents:UIControlEventTouchUpInside|UIControlEventTouchUpOutside];
    [self addView:button withId:viewId];
    [self addToParent:parentId view:button];
}

- (void)buttonPushed:(UIButton *)sender
{
    NSLog(@"buttonPushed");
    int viewId = (int)sender.tag;
    [self sendIntValue:viewId value:1];
}

- (void)buttonTouchDown:(UIButton *)sender
{
    NSLog(@"buttonTouchDown");
    ViewManager * viewManager = [self getViewManager:(int)sender.tag];
    [viewManager switchStyle:SelectorActive];
}

- (void)buttonTouchUp:(UIButton *)sender
{
    NSLog(@"buttonTouchUp");
    ViewManager * viewManager = [self getViewManager:(int)sender.tag];
    [viewManager switchStyle:SelectorNormal];
}

- (void)createSwitchWithId:(int)viewId parentId:(int)parentId value:(BOOL)value
{
    UISwitch *sw = [[UISwitch alloc] init];
    sw.tag = viewId;
    sw.translatesAutoresizingMaskIntoConstraints = false;
    [sw setOn:value];
    [sw addTarget:self action:@selector(switchToggled:) forControlEvents:UIControlEventValueChanged];
    [self addView:sw withId:viewId];
    [self addToParent:parentId view:sw];
}

- (void)switchToggled:(UISwitch *)sender
{
    int viewId = (int)sender.tag;
    BOOL switchState = sender.on;
    int value = switchState ? 1 : 0;
    [self sendIntValue:viewId value:value];
}

- (void)createImageWithId:(int)viewId parentId:(int)parentId filename:(NSString *)filename width:(int)width height:(int)height
{
    FWImageView * imageView;
    if (filename != nil && [filename length] > 0 && ![filename hasPrefix:@"http://"] && ![filename hasPrefix:@"https://"]) {
        UIImage *image = [UIImage imageNamed:filename];
        imageView = [[FWImageView alloc] initWithImage:image];
    } else {
        imageView = [[FWImageView alloc] init];
        if (filename != nil && [filename length] > 0) {
            [imageView addImageUrl:filename width:width height:height];
        }
    }
    imageView.tag = viewId;
    imageView.contentMode = UIViewContentModeScaleAspectFit;
    imageView.clipsToBounds = YES;
    imageView.delegate = self;
    [self addView:imageView withId:viewId];
    [self addToParent:parentId view:imageView];
}

- (void)createScrollLayoutWithId:(int)viewId parentId:(int)parentId
{
    FWScrollView * scrollView = [[FWScrollView alloc] init];
    scrollView.tag = viewId;
    scrollView.clipsToBounds = YES;
    scrollView.delegate = self;
    scrollView.contentInsetAdjustmentBehavior = UIScrollViewContentInsetAdjustmentScrollableAxes;
    
    [self addView:scrollView withId:viewId];
    [self addToParent:parentId view:scrollView];
}

- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    if (!scrollView.pagingEnabled) {
        float diff = scrollView.contentSize.height - (scrollView.frame.size.height + scrollView.contentOffset.y);
        mainThread->sendScrollChangedEvent(scrollView.tag, scrollView.contentOffset.y, (int)diff, scrollView.contentSize.height);
    } else if ([scrollView isKindOfClass:FWScrollView.class]) {
        FWScrollView * fwScrollView = (FWScrollView *)scrollView;
        NSInteger page = [fwScrollView indexForVisiblePage];
        
        if (page >= 0 && page != fwScrollView.currentPage) {
            [fwScrollView setPage:page];

            [self sendIntValue:(int)scrollView.tag value:(int)page];
            
	    if (scrollView == self.pageView) {
                // set selected item for all tabbars
                [self updateTabBars:page];
            }
	    [self sendVisibilityUpdate:fwScrollView.tag];
        }
    }
}

- (void)updateTabBars:(NSInteger)page
{
    for (NSString *key in self.viewsDictionary.allKeys) {
        ViewManager *viewManager = [self.viewsDictionary objectForKey:key];
        if ([viewManager.view isKindOfClass:UITabBar.class]) {
            UITabBar *tabBar = (UITabBar *)viewManager.view;
            if (page <= tabBar.items.count) {
                [tabBar setSelectedItem:tabBar.items[page]];
            }
        }
    }
}

- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView
{
    if ([scrollView isKindOfClass:FWScrollView.class]) {
        FWScrollView * fwScrollView = (FWScrollView *)scrollView;
	[fwScrollView updateVisibility:fwScrollView.bounds];
    }

    UserInteractionEvent ev(UserInteractionEvent::ENDED);
    mainThread->sendEvent(mainThread->getApplication().getInternalId(), ev);
}

- (void)scrollViewWillBeginDragging:(UIScrollView *)scrollView
{
    UserInteractionEvent ev(UserInteractionEvent::STARTED);
    mainThread->sendEvent(mainThread->getApplication().getInternalId(), ev);
}

- (void)createPageLayoutWithId:(int)viewId parentId:(int)parentId
{
    FWScrollView * scrollView = [[FWScrollView alloc] init];
    scrollView.tag = viewId;
    scrollView.pagingEnabled = YES;
    scrollView.layoutMargins = UIEdgeInsetsMake(0, 0, 0, 0);
    scrollView.clipsToBounds = YES;
    scrollView.delegate = self;
    scrollView.contentInsetAdjustmentBehavior = UIScrollViewContentInsetAdjustmentScrollableAxes;
    
    if (self.pageView == nil) self.pageView = scrollView;
    [self addView:scrollView withId:viewId];
    [self addToParent:parentId view:scrollView];

    [scrollView.panGestureRecognizer requireGestureRecognizerToFail:self.panEdgeGestureRecognizer];
}

- (void)createEventLayoutWithId:(int)viewId parentId:(int)parentId
{
    FrameLayoutView *view = [[FrameLayoutView alloc] init];
    view.tag = viewId;
    view.layoutMargins = UIEdgeInsetsMake(0, 0, 0, 0);
    [self addView:view withId:viewId];
    [self addToParent:parentId view:view];

    UITapGestureRecognizer *tapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(viewTapped:)];
    tapGestureRecognizer.delegate = self;
    [view addGestureRecognizer:tapGestureRecognizer];
    
    UILongPressGestureRecognizer *longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(viewTouched:)];
    longPressGestureRecognizer.minimumPressDuration = 0.25f;
    longPressGestureRecognizer.delegate = self;
    [view addGestureRecognizer:longPressGestureRecognizer];
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer: (UIGestureRecognizer *)otherGestureRecognizer {
    if ([otherGestureRecognizer isEqual:self.pageView.panGestureRecognizer] && [gestureRecognizer isEqual:self.panEdgeGestureRecognizer]) {
        return YES;
    }
    return gestureRecognizer.view == otherGestureRecognizer.view;
}

/*- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRequireFailureOfGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
{
    if ([gestureRecognizer isEqual:self.sideMenuSwipeGestureRecognizer] && [otherGestureRecognizer isEqual:self.sideMenuPanGestureRecognizer]) {
        return YES;
    }
    return NO;
}


- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldBeRequiredToFailByGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
{
    if ([otherGestureRecognizer isKindOfClass:UIPanGestureRecognizer.class] && [gestureRecognizer isEqual:self.panEdgeGestureRecognizer]) {
        return YES;
        
    }
    return NO;
}
*/
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch
{
    UITapGestureRecognizer *tapGestureRecognizer = self.view.gestureRecognizers.firstObject;
    if ([gestureRecognizer isEqual:tapGestureRecognizer]) {
        if (self.keyboardVisible) {
            return YES;
        } else {
            return NO;
        }
    }
    return YES;
}

- (void)createNavigationBar:(int)viewId parentId:(int)parentId
{
    CGFloat statusBarHeight = [UIApplication sharedApplication].statusBarFrame.size.height;
    // Create navigation bar with a button for opening side menu
    UINavigationBar *navBar = [[UINavigationBar alloc] init];

    navBar.translatesAutoresizingMaskIntoConstraints = false;
    navBar.tag = viewId;
    navBar.translucent = YES;
    navBar.delegate = self;
    //navBar.layer.zPosition = 1000;

    // create titleView that has title and subtitle
    CGFloat titleViewWidth = self.view.frame.size.width - 140;
    //CGFloat titleViewLeading = 8;
    UIView *titleView = [[UIView alloc] initWithFrame:CGRectMake(self.view.frame.size.width/2 - titleViewWidth/2, 0, titleViewWidth, 44)];
    UILabel *titleViewTitle = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, titleViewWidth, 22)];
    titleViewTitle.textAlignment = NSTextAlignmentCenter;
    titleViewTitle.font = [UIFont systemFontOfSize:18 weight:UIFontWeightBold];
    titleViewTitle.adjustsFontSizeToFitWidth = YES;
    titleViewTitle.minimumScaleFactor = 0.7;
    UILabel *titleViewSubtitle = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, titleViewWidth, 12)];
    titleViewSubtitle.textAlignment = NSTextAlignmentCenter;
    titleViewSubtitle.font = [UIFont systemFontOfSize:12 weight:UIFontWeightRegular];
    titleViewSubtitle.textColor = UIColor.darkGrayColor;
    //[stackView addArrangedSubview:titleViewTitle];
    //[stackView addArrangedSubview:titleViewSubtitle];
    
    // Add debug event by tapping nav bar 5 times
    UITapGestureRecognizer *debugTapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(navBarTapped5Times:)];
    debugTapGesture.numberOfTapsRequired = 5;
    [titleView addGestureRecognizer:debugTapGesture];
    
    UIStackView *stackView = [[UIStackView alloc] initWithArrangedSubviews:@[titleViewTitle, titleViewSubtitle]];
    stackView.spacing = 0.0;
    stackView.axis = UILayoutConstraintAxisVertical;
    stackView.alignment = UIStackViewAlignmentCenter;
    stackView.distribution = UIStackViewDistributionFillProportionally;
    stackView.translatesAutoresizingMaskIntoConstraints = NO;
    [titleView addSubview:stackView];
    NSLayoutConstraint *stackViewTopConstraint = [NSLayoutConstraint constraintWithItem:stackView attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:titleView attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
    NSLayoutConstraint *stackViewLeftConstraint = [NSLayoutConstraint constraintWithItem:stackView attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:titleView attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
    NSLayoutConstraint *stackViewRightConstraint = [NSLayoutConstraint constraintWithItem:stackView attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:titleView attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
    NSLayoutConstraint *stackViewBottomConstraint = [NSLayoutConstraint constraintWithItem:stackView attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:titleView attribute:NSLayoutAttributeBottom multiplier:1.0f constant:0];
    [titleView addConstraints:@[stackViewTopConstraint, stackViewLeftConstraint, stackViewRightConstraint, stackViewBottomConstraint]];
    self.titleView = titleView;

    //[stackView.leadingAnchor constraintEqualToAnchor:titleView.leadingAnchor].active = YES;
    //[stackView.trailingAnchor constraintEqualToAnchor:titleView.trailingAnchor].active = YES;
    //[stackView.topAnchor constraintEqualToAnchor:titleView.topAnchor].active = YES;
    //[stackView.bottomAnchor constraintEqualToAnchor:titleView.bottomAnchor].active = YES;
    
    self.navBarTitle = titleViewTitle;
    self.navBarSubtitle = titleViewSubtitle;
    self.navItem = [[UINavigationItem alloc] init];
    self.navItem.titleView = titleView;
    /*
     if (self.currentTitle != nil) {
      self.navItem = [[UINavigationItem alloc] initWithTitle:self.currentTitle];
    } else {
      self.navItem = [[UINavigationItem alloc] init];
    }
     */

    UIBarButtonItem *menuButton;
    UIImage *image = [self.imageCache loadIcon:@"icons_hamburger-menu.png"];
    if (image == nil) {
        menuButton = [[UIBarButtonItem alloc] initWithTitle:@"Menu" style:UIBarButtonItemStylePlain target:self action:@selector(menuButtonTapped)];
    } else {
        menuButton = [[UIBarButtonItem alloc] initWithImage:image style:UIBarButtonItemStylePlain target:self action:@selector(menuButtonTapped)];
    }
    self.navItem.leftBarButtonItem = menuButton;

    UIImage *image2 = [self.imageCache loadIcon:@"write_icon_small.png"];
    if (image2 != nil) {
        UIBarButtonItem *composeButton = [[UIBarButtonItem alloc] initWithImage:image2 style:UIBarButtonItemStylePlain target:self action:@selector(composeButtonTapped)];
        self.navItem.rightBarButtonItem = composeButton;
    }

    [navBar setItems:@[self.navItem]];
    
    UIView * parentView = (UIView *)[self viewForId:parentId];

    [self addView:navBar withId:viewId];
    [parentView addSubview:navBar];
    
    NSLayoutConstraint *topConstraint2 = [NSLayoutConstraint constraintWithItem:navBar attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:navBar.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:statusBarHeight];
    NSLayoutConstraint *leftConstraint2 = [NSLayoutConstraint constraintWithItem:navBar attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:navBar.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
    NSLayoutConstraint *rightConstraint2 = [NSLayoutConstraint constraintWithItem:navBar attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:navBar.superview attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
    // NSLayoutConstraint *heightConstraint = [NSLayoutConstraint constraintWithItem:navBar attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:1.0f constant:44];
    [navBar.superview addConstraints:@[topConstraint2, leftConstraint2, rightConstraint2]];
    self.statusBarTopConstraint = topConstraint2;
    
    self.navBar = navBar;

    self.additionalSafeAreaInsets = UIEdgeInsetsMake(44, 0, self.additionalSafeAreaInsets.bottom, 0);
}

- (void)navBarTapped5Times:(UITapGestureRecognizer *)recognizer
{
    mainThread->startDebugMode();
}

- (void)menuButtonTapped
{
    if (self.sideMenuView.isHidden) {
        [self showNavigationViewWithAnimation:YES];
    } else {
        [self hideNavigationViewWithAnimation:YES];
    }
}

- (void)composeButtonTapped
{
    mainThread->sendCommandEvent(self.navBar.tag, FW_ID_COMPOSE);
}

- (void)backButtonTapped
{
    mainThread->sendCommandEvent(self.navBar.tag, FW_ID_BACK);
}


#pragma mark - TabBar
- (void)createTabBar:(int)viewId parentId:(int)parentId
{
    UITabBar *tabBar = [[UITabBar alloc] init];
    tabBar.tag = viewId;
    self.tabBar = tabBar;
    tabBar.delegate = self;
    tabBar.translucent = YES;
    tabBar.translatesAutoresizingMaskIntoConstraints = false;
    [self addView:tabBar withId:viewId];
    [self addToParent:parentId view:tabBar];
    tabBar.items = [[NSArray alloc] init];

    self.additionalSafeAreaInsets = UIEdgeInsetsMake(self.additionalSafeAreaInsets.top, 0, 49, 0);
}

- (void)createTabBarItem:(int)viewId parentId:(int)parentId title:(NSString *)title
{
    UIView *parentView = [self viewForId:parentId];
    
    if ([parentView isKindOfClass:UITabBar.class]) {
        UITabBar *tabBar = (UITabBar *)parentView;
        
        UITabBarItem * tabBarItem = [[UITabBarItem alloc] initWithTitle:title image:nil tag:viewId];
        
        NSMutableArray *items = tabBar.items.mutableCopy;
        if (items != nil) {
            [items addObject:tabBarItem];
            [tabBar setItems:(NSArray *)items animated:false];
            if ([items count] == 1) {
                [tabBar setSelectedItem:tabBarItem];
            }
        }
        [self addView:tabBarItem withId:viewId];
    }
}

- (void)tabBar:(UITabBar *)tabBar didSelectItem:(UITabBarItem *)item
{
    NSInteger itemIndex = (int)[self indexForTabBar:tabBar item:item];
    if (itemIndex != NSNotFound) {
        if (self.pageView) {
            [self.pageView showPage:itemIndex animated:NO];
            [self sendIntValue:(int)self.pageView.tag value:itemIndex];
	    [self sendVisibilityUpdate:self.pageView.tag];
	}
    }
    [self sendIntValue:(int)item.tag value:1];
}

// returns index for item in tabBar.items array.
- (NSInteger)indexForTabBar:(UITabBar *)tabBar item:(UITabBarItem *)item
{
    for (int i = 0; i < tabBar.items.count; i++) {
        if ([tabBar.items[i] isEqual:item]) {
            return i;
        }
    }
    return NSNotFound;
}

#pragma mark - Side menu
- (void)createNavigationView:(int)viewId
{
    CGRect viewFrame = self.view.bounds;
    CGRect frame = CGRectMake(CGRectGetMinX(viewFrame), CGRectGetMinY(viewFrame), CGRectGetWidth(viewFrame)-sideMenuOpenSpaceWidth, CGRectGetHeight(viewFrame));
    self.sideMenuView = [[UIView alloc] initWithFrame:frame];
    self.sideMenuView.tag = viewId;
    self.sideMenuView.hidden = YES;
    self.sideMenuView.layer.shadowColor = [UIColor blackColor].CGColor;
    self.sideMenuView.layer.shadowOpacity = 1.0;
    self.sideMenuView.layer.shadowRadius = 7.5;
    self.sideMenuView.layer.shadowOffset = CGSizeMake(1, 4);    
    
    [self.view addSubview:self.sideMenuView];
    self.sideMenuView.transform = CGAffineTransformTranslate(self.sideMenuView.transform, -CGRectGetWidth(self.sideMenuView.frame), 0.0);
    [self addView:self.sideMenuView withId:viewId];
    
    UIPanGestureRecognizer *sideMenuPanGestureRecognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(sideMenuPanned:)];
    //self.sideMenuPanGestureRecognizer.delegate = self;
    [self.sideMenuView addGestureRecognizer:sideMenuPanGestureRecognizer];

    UISwipeGestureRecognizer *sideMenuSwipeGestureRecognizer = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(sideMenuSwiped:)];
    sideMenuSwipeGestureRecognizer.direction = UISwipeGestureRecognizerDirectionLeft;
    sideMenuSwipeGestureRecognizer.delegate = self;
    [self.sideMenuView addGestureRecognizer:sideMenuSwipeGestureRecognizer];
    
    UIView * sideMenuContainer = [[UIView alloc] init];
    sideMenuContainer.translatesAutoresizingMaskIntoConstraints = false;
    [self.sideMenuView addSubview:sideMenuContainer];

    CGFloat statusBarHeight = [UIApplication sharedApplication].statusBarFrame.size.height;

    NSLayoutConstraint *topConstraint = [NSLayoutConstraint constraintWithItem:sideMenuContainer attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:sideMenuContainer.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:statusBarHeight];
    NSLayoutConstraint *leftConstraint = [NSLayoutConstraint constraintWithItem:sideMenuContainer attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:sideMenuContainer.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
    NSLayoutConstraint *rightConstraint = [NSLayoutConstraint constraintWithItem:sideMenuContainer attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:sideMenuContainer.superview attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
    NSLayoutConstraint *bottomConstraint = [NSLayoutConstraint constraintWithItem:sideMenuContainer attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:sideMenuContainer.superview attribute:NSLayoutAttributeBottom multiplier:1.0f constant:0];
    [sideMenuContainer.superview addConstraints:@[topConstraint, leftConstraint, rightConstraint, bottomConstraint]];
    
    ViewManager * viewManager = [self getViewManager:viewId];
    viewManager.containerView = sideMenuContainer;
}

- (void)showNavigationViewWithAnimation:(BOOL)animate
{
    if (self.sideMenuView != nil && (self.sideMenuView.isHidden || self.sideMenuPanned)) {
#if 0
        [self sendIntValue:self.sideMenuView.tag value:1];
#endif
        mainThread->sendVisibilityEvent(self.sideMenuView.tag, 1);

	[self.view bringSubviewToFront:self.backgroundOverlayView];
        self.backgroundOverlayView.hidden = NO;
        self.backgroundOverlayView.alpha = backgroundOverlayViewAlpha * CGRectGetMaxX(self.sideMenuView.frame) / (CGRectGetWidth(self.view.frame) - sideMenuOpenSpaceWidth);

	[self.view bringSubviewToFront:self.sideMenuView];

        if (animate) {
            [UIView animateWithDuration:animationDuration animations:^{
                self.sideMenuView.transform = CGAffineTransformTranslate(self.sideMenuView.transform, CGRectGetWidth(self.sideMenuView.frame)-CGRectGetMaxX(self.sideMenuView.frame), 0.0);
                self.backgroundOverlayView.alpha = backgroundOverlayViewAlpha * CGRectGetMaxX(self.sideMenuView.frame) / (CGRectGetWidth(self.view.frame) - sideMenuOpenSpaceWidth);
            }];
        } else {
            self.sideMenuView.transform = CGAffineTransformTranslate(self.sideMenuView.transform, CGRectGetWidth(self.sideMenuView.frame)-CGRectGetMaxX(self.sideMenuView.frame), 0.0);
            self.backgroundOverlayView.alpha = backgroundOverlayViewAlpha * CGRectGetMaxX(self.sideMenuView.frame) / (CGRectGetWidth(self.view.frame) - sideMenuOpenSpaceWidth);
        }
        self.sideMenuView.hidden = NO;
    }
}

- (void)hideNavigationViewWithAnimation:(BOOL)animate
{
    if (self.sideMenuView) {
        [self sendIntValue:self.sideMenuView.tag value:0];
        mainThread->sendVisibilityEvent(self.sideMenuView.tag, 0);

        if (animate) {
            [UIView animateWithDuration:animationDuration animations:^{
                self.sideMenuView.transform = CGAffineTransformTranslate(self.sideMenuView.transform, -((CGRectGetWidth(self.sideMenuView.frame)+CGRectGetMaxX(self.sideMenuView.frame))), 0.0);
                self.backgroundOverlayView.alpha = backgroundOverlayViewAlpha * CGRectGetMaxX(self.sideMenuView.frame) / (CGRectGetWidth(self.view.frame) - sideMenuOpenSpaceWidth);
            } completion:^(BOOL finished) {
                if (finished) {
                    self.sideMenuView.hidden = YES;
                    self.backgroundOverlayView.hidden = YES;
                }
            }];
        } else {
            self.sideMenuView.transform = CGAffineTransformTranslate(self.sideMenuView.transform, -((CGRectGetWidth(self.sideMenuView.frame)+CGRectGetMaxX(self.sideMenuView.frame))), 0.0);
            self.backgroundOverlayView.alpha = 0.0;
            self.backgroundOverlayView.hidden = YES;
            self.sideMenuView.hidden = YES;
        }
    }
}

- (void)sideMenuSwiped:(UISwipeGestureRecognizer *)recognizer
{
    self.sideMenuSwiped = YES;
}
    
- (void)edgeSwiped:(UIScreenEdgePanGestureRecognizer *)recognizer
{
    if (self.sideMenuView) {
        if (self.sideMenuView.hidden || self.backgroundOverlayView.hidden) {
            self.sideMenuView.hidden = NO;
            self.backgroundOverlayView.hidden = NO;
            [self.view bringSubviewToFront:self.backgroundOverlayView];
            [self.view bringSubviewToFront:self.sideMenuView];
        }

        CGPoint touchLocation = [recognizer translationInView:self.view];
        NSLog(@"location = %@", NSStringFromCGPoint(touchLocation));
        if (recognizer.state == UIGestureRecognizerStateBegan || recognizer.state == UIGestureRecognizerStateChanged) {
            if (touchLocation.x < CGRectGetWidth(self.view.frame)-sideMenuOpenSpaceWidth) {
                self.sideMenuView.frame = CGRectMake(touchLocation.x-CGRectGetWidth(self.sideMenuView.frame), 0, CGRectGetWidth(self.sideMenuView.frame), CGRectGetHeight(self.sideMenuView.frame));
                self.backgroundOverlayView.alpha = backgroundOverlayViewAlpha * CGRectGetMaxX(self.sideMenuView.frame) / (CGRectGetWidth(self.view.frame) - sideMenuOpenSpaceWidth);
                self.sideMenuPanned = YES;
            }
        } else if (recognizer.state == UIGestureRecognizerStateEnded) {
            if (CGRectGetMaxX(self.sideMenuView.frame) > (CGRectGetWidth(self.view.frame)-sideMenuOpenSpaceWidth) / 2) {
                [self showNavigationViewWithAnimation:YES];
                self.sideMenuPanned = NO;
            } else {
                [self hideNavigationViewWithAnimation:YES];
                self.sideMenuPanned = NO;
            }
        }
    }
}

- (void)sideMenuPanned:(UIPanGestureRecognizer *)recognizer
{
    CGPoint touchLocation = [recognizer translationInView:recognizer.view];
    //NSLog(@"panned location = %@", NSStringFromCGPoint(touchLocation));
    //NSLog(@"touchLocation = %@", NSStringFromCGPoint(touchLocation));
    BOOL gestureIsDraggingFromLeftToRight = [recognizer velocityInView:self.view].x > 0;
    if (recognizer.state == UIGestureRecognizerStateBegan) {
        self.sideMenuPanStartedDate = [NSDate date];
        [recognizer setTranslation:touchLocation inView:self.view];
    } else if (recognizer.state == UIGestureRecognizerStateChanged) {
        recognizer.view.center = CGPointMake(recognizer.view.center.x + [recognizer translationInView:self.view].x, recognizer.view.center.y);
        if (CGRectGetMaxX(recognizer.view.frame) > CGRectGetWidth(self.view.frame)-sideMenuOpenSpaceWidth) {
            recognizer.view.frame = CGRectMake(0, 0, recognizer.view.frame.size.width, recognizer.view.frame.size.height);
        }
        
        [recognizer setTranslation:CGPointZero inView:self.view];
        self.backgroundOverlayView.alpha = backgroundOverlayViewAlpha * CGRectGetMaxX(self.sideMenuView.frame) / (CGRectGetWidth(self.view.frame) - sideMenuOpenSpaceWidth);
        self.sideMenuPanned = YES;
    } else if (recognizer.state == UIGestureRecognizerStateEnded) {
        NSTimeInterval timeFromBegin =  -[self.sideMenuPanStartedDate timeIntervalSinceNow];
        //NSLog(@"timeFromBegin = %f", timeFromBegin);
        
        // If swipe gesture recognized and "panning" is under one second and direction left, then close side panel
        if (self.sideMenuSwiped && timeFromBegin < 1 && self.sideMenuLastTouchLocationX < 0) {
            [self hideNavigationViewWithAnimation:YES];
        } else {
            if (CGRectGetMaxX(self.sideMenuView.frame) > (CGRectGetWidth(self.view.frame)-sideMenuOpenSpaceWidth) / 2) {
                [self showNavigationViewWithAnimation:YES];
                self.sideMenuPanned = NO;
            } else {
                [self hideNavigationViewWithAnimation:YES];
                self.sideMenuPanned = NO;
            }
        }
        self.sideMenuSwiped = NO;
    }
    self.sideMenuLastTouchLocationX = touchLocation.x;
}

#pragma mark -
- (void)createActivityIndicatorWithId:(int)viewId parentId:(int)parentId
{
    UIActivityIndicatorView * view = [[UIActivityIndicatorView alloc] init];
    view.tag = viewId;
    view.translatesAutoresizingMaskIntoConstraints = false;
    view.activityIndicatorViewStyle = UIActivityIndicatorViewStyleGray;
    view.hidesWhenStopped = YES;
    [self addView:view withId:viewId];
    [self addToParent:parentId view:view];
    [view startAnimating];
}

- (void)createPageControlWithId:(int)viewId parentId:(int)parentId numPages:(int)numPages
{
    UIPageControl * view = [[UIPageControl alloc] init];
    view.tag = viewId;
    view.translatesAutoresizingMaskIntoConstraints = false;
    view.numberOfPages = numPages;
    view.pageIndicatorTintColor = [UIColor colorWithRed:0.7 green:0.7 blue:0.7 alpha:1.0];
    view.currentPageIndicatorTintColor = [UIColor colorWithRed:0.1 green:0.1 blue:0.1 alpha:1.0];
    [self addView:view withId:viewId];
    [self addToParent:parentId view:view];
}

- (void)createSegmentedControl:(int)viewId parentId:(int)parentId
{
    UISegmentedControl * view = [[UISegmentedControl alloc] init];
    view.tag = viewId;
    view.translatesAutoresizingMaskIntoConstraints = false;
    [view addTarget:self action:@selector(segmentedControlToggled:) forControlEvents:UIControlEventValueChanged];
    [self addView:view withId:viewId];
    [self addToParent:parentId view:view];
}

- (void)segmentedControlToggled:(UISegmentedControl *)sender
{
    int viewId = (int)sender.tag;
    int value = (int)sender.selectedSegmentIndex;
    [self sendIntValue:viewId value:value];
}

- (void)createPickerWithId:(int)viewId parentId:(int)parentId
{
    FWPicker * view = [[FWPicker alloc] init];
    view.tag = viewId;

    UIImage *image = [self.imageCache loadIcon:@"icons_arrow-down.png"];
    [view setImage:image forState:UIControlStateNormal];

    [self addView:view withId:viewId];
    [self addToParent:parentId view:view];
        
    [view addTarget:self action:@selector(showPicker:) forControlEvents:UIControlEventTouchUpInside];
}

- (void)showPicker:(UIButton *)sender{
    if (self.currentPickerHolder != nil) {
        [self.currentPickerHolder removeFromSuperview];
    }

    self.currentPicker = (FWPicker *)sender;

    UIView * pickerHolder = [[UIView alloc] init];
    pickerHolder.translatesAutoresizingMaskIntoConstraints = false;
    [self.view addSubview:pickerHolder];

    self.currentPickerHolderTopConstraint = [NSLayoutConstraint constraintWithItem:pickerHolder attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:pickerHolder.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
    self.currentPickerHolderLeftConstraint = [NSLayoutConstraint constraintWithItem:pickerHolder attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:pickerHolder.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
    self.currentPickerHolderRightConstraint = [NSLayoutConstraint constraintWithItem:pickerHolder attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:pickerHolder.superview attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
    self.currentPickerHolderBottomConstraint = [NSLayoutConstraint constraintWithItem:pickerHolder attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:pickerHolder.superview attribute:NSLayoutAttributeBottom multiplier:1.0f constant:0];
    [pickerHolder.superview addConstraints:@[self.currentPickerHolderTopConstraint, self.currentPickerHolderLeftConstraint, self.currentPickerHolderRightConstraint, self.currentPickerHolderBottomConstraint]];

    self.currentPickerHolder = pickerHolder;

    CGFloat statusBarHeight = [UIApplication sharedApplication].statusBarFrame.size.height;

    FrameLayoutView *layout = [[FrameLayoutView alloc] init];
    layout.layoutMargins = UIEdgeInsetsMake(0, 0, 0, 0);
    layout.layer.backgroundColor = UIColor.whiteColor.CGColor;
    [pickerHolder addSubview:layout];

    UIToolbar *statusBarBackgroundView = [[UIToolbar alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, statusBarHeight)];
    statusBarBackgroundView.translucent = NO;
    statusBarBackgroundView.autoresizingMask = UIViewAutoresizingFlexibleBottomMargin | UIViewAutoresizingFlexibleWidth;
    [pickerHolder addSubview:statusBarBackgroundView];

    UINavigationBar *navBar = [[UINavigationBar alloc] initWithFrame:CGRectMake(0, statusBarHeight, self.view.frame.size.width, 44)];
    
    UINavigationItem *navItem = [[UINavigationItem alloc] init];    
    UIBarButtonItem *backButton = [[UIBarButtonItem alloc] initWithTitle:@"Back" style:UIBarButtonItemStylePlain target:self action:@selector(cancelPicker)];
    navItem.leftBarButtonItem = backButton;
    [navBar setItems:@[navItem]];
    navBar.translatesAutoresizingMaskIntoConstraints = YES;
    // navBar.translucent = YES;
    [pickerHolder addSubview:navBar];

    NSLayoutConstraint *topConstraint = [NSLayoutConstraint constraintWithItem:layout attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:layout.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:64];
    NSLayoutConstraint *leftConstraint = [NSLayoutConstraint constraintWithItem:layout attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:layout.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
    NSLayoutConstraint *rightConstraint = [NSLayoutConstraint constraintWithItem:layout attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:layout.superview attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
    NSLayoutConstraint *bottomConstraint = [NSLayoutConstraint constraintWithItem:layout attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:layout.superview attribute:NSLayoutAttributeBottom multiplier:1.0f constant:0];
    [layout.superview addConstraints:@[topConstraint, leftConstraint, rightConstraint, bottomConstraint]];
    
    LinearLayoutView *layout2 = [[LinearLayoutView alloc] init];
    layout2.orientation = LinearLayoutViewOrientationVertical;
    layout2.layoutMargins = UIEdgeInsetsMake(0, 0, 0, 0);

    LayoutParams * item = [LayoutParams layoutItemForView:layout2];
    item.fixedWidth = -1;
    [layout addItem:item];

    for (int i = 0; i < [self.currentPicker.options count]; i++) {
        NSString * label = self.currentPicker.options[i];
        
        UIButton *button = [UIButton buttonWithType:UIButtonTypeCustom];
        [button setTitleColor:UIColor.blackColor forState:UIControlStateNormal];
        button.translatesAutoresizingMaskIntoConstraints = false;
        [button setTitle:label forState:UIControlStateNormal];
        button.tag = i;
        [button addTarget:self action:@selector(pickerButtonPushed:) forControlEvents:UIControlEventTouchUpInside];
        
        LayoutParams * item = [LayoutParams layoutItemForView:button];
        item.fixedWidth = -1;
        [layout2 addItem:item];
    }
    
    // Animations
    CGFloat topConstraintConstantFinal = self.currentPickerHolderTopConstraint.constant;
    CGFloat leftConstraintConstantFinal = self.currentPickerHolderLeftConstraint.constant;
    CGFloat rightConstraintConstantFinal = self.currentPickerHolderRightConstraint.constant;
    CGFloat bottomConstraintConstantFinal = self.currentPickerHolderBottomConstraint.constant;
    
    [pickerHolder layoutIfNeeded];
    
    self.currentPickerHolderLeftConstraint.constant = self.view.frame.size.width;
    self.currentPickerHolderRightConstraint.constant = self.view.frame.size.width + self.currentPickerHolderRightConstraint.constant;
    
    [pickerHolder.superview addConstraints:@[self.currentPickerHolderTopConstraint, self.currentPickerHolderLeftConstraint, self.currentPickerHolderRightConstraint, self.currentPickerHolderBottomConstraint]];
    [pickerHolder.superview layoutIfNeeded];
    [UIView animateWithDuration:animationDuration/1.5 delay:0 usingSpringWithDamping:1 initialSpringVelocity:0 options:UIViewAnimationOptionCurveEaseInOut animations:^{
        self.currentPickerHolderLeftConstraint.constant = leftConstraintConstantFinal;
        self.currentPickerHolderRightConstraint.constant = rightConstraintConstantFinal;
        [pickerHolder.superview layoutIfNeeded];
    } completion:^(BOOL finished) {
        [pickerHolder layoutIfNeeded];
    }];
} 

- (void)pickerButtonPushed:(UIButton *)sender
{
    int row = (int)sender.tag;

    [self sendIntValue:self.currentPicker.tag value:row];
    [self.currentPicker setSelection:row];
    [self.currentPickerHolder layoutIfNeeded];
    self.currentPickerHolderTopConstraint.constant = 0.0;
    self.currentPickerHolderLeftConstraint.constant = self.view.frame.size.width;
    self.currentPickerHolderRightConstraint.constant = self.view.frame.size.width;
    self.currentPickerHolderBottomConstraint.constant = 0.0;
    [UIView animateWithDuration:animationDuration/1.5 delay:0 usingSpringWithDamping:1 initialSpringVelocity:0 options:UIViewAnimationOptionCurveEaseInOut animations:^{
        [self.currentPickerHolder.superview layoutIfNeeded];
    } completion:^(BOOL finished) {
        if (finished) {
            [self.currentPickerHolder removeFromSuperview];
            self.currentPickerHolder = nil;
            self.currentPicker = nil;
        }
    }];
}

- (void)cancelPicker
{
    [self.currentPickerHolder layoutIfNeeded];
    self.currentPickerHolderTopConstraint.constant = 0.0;
    self.currentPickerHolderLeftConstraint.constant = self.view.frame.size.width;
    self.currentPickerHolderRightConstraint.constant = self.view.frame.size.width;
    self.currentPickerHolderBottomConstraint.constant = 0.0;
    [UIView animateWithDuration:animationDuration/1.5 delay:0 usingSpringWithDamping:1 initialSpringVelocity:0 options:UIViewAnimationOptionCurveEaseInOut animations:^{
        [self.currentPickerHolder.superview layoutIfNeeded];
    } completion:^(BOOL finished) {
        if (finished) {
            [self.currentPickerHolder removeFromSuperview];
            self.currentPickerHolder = nil;
            self.currentPicker = nil;
        }
    }];
}

- (void)createActionSheetWithId:(int)viewId parentId:(int)parentId title:(NSString *)title
{  
    UIAlertController *actionSheet = [UIAlertController alertControllerWithTitle:title message:nil preferredStyle:UIAlertControllerStyleActionSheet];
  
    [self addView:actionSheet withId:viewId];
  
    [actionSheet addAction:[UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:^(UIAlertAction *action) {
        NSLog(@"User clicked button called %@ or tapped elsewhere",action.title);
        [self sendIntValue:viewId value:0];
    }]];

    [self presentViewController:actionSheet animated:YES completion:nil];
}

- (void)createDialogWithId:(int)viewId parentId:(int)parentId title:(NSString*)title animationStyle:(AnimationStyle)style
{
    [self.dialogIds addObject:[NSNumber numberWithInt:viewId]];
    self.dialogAnimationStyle = style;
    UIView * dialogHolder = [[UIView alloc] init];
    dialogHolder.tag = viewId;
    dialogHolder.translatesAutoresizingMaskIntoConstraints = false;
    dialogHolder.alpha = 0.0;
    [self.view addSubview:dialogHolder];
    [self addView:dialogHolder withId:viewId];

    NSLayoutConstraint *topConstraint0 = [NSLayoutConstraint constraintWithItem:dialogHolder attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:dialogHolder.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
    NSLayoutConstraint *leftConstraint0 = [NSLayoutConstraint constraintWithItem:dialogHolder attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:dialogHolder.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
    NSLayoutConstraint *rightConstraint0 = [NSLayoutConstraint constraintWithItem:dialogHolder attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:dialogHolder.superview attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
    NSLayoutConstraint *bottomConstraint0 = [NSLayoutConstraint constraintWithItem:dialogHolder attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:dialogHolder.superview attribute:NSLayoutAttributeBottom multiplier:1.0f constant:0];
    [dialogHolder.superview addConstraints:@[topConstraint0, leftConstraint0, rightConstraint0, bottomConstraint0]];
    
    UIView * dialogBackground = [self createBackgroundOverlay:dialogHolder];
    dialogBackground.tag = viewId;
    dialogBackground.alpha = backgroundOverlayViewAlpha;

    DialogView * dialog = [[DialogView alloc] init];
    dialog.layer.backgroundColor = [UIColor whiteColor].CGColor;
    dialog.layer.cornerRadius = 5;
    dialog.layer.shadowColor = [UIColor blackColor].CGColor;
    dialog.layer.shadowOpacity = 1.0;
    dialog.layer.shadowRadius = 7.5;
    dialog.layer.shadowOffset = CGSizeMake(1, 4);
    dialog.clipsToBounds = YES;
    dialog.tag = viewId;
    [dialogHolder addSubview:dialog];

    NSLayoutConstraint *topConstraint = [NSLayoutConstraint constraintWithItem:dialog attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:dialog.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:65];
    NSLayoutConstraint *leftConstraint = [NSLayoutConstraint constraintWithItem:dialog attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:dialog.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
    NSLayoutConstraint *rightConstraint = [NSLayoutConstraint constraintWithItem:dialog attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:dialog.superview attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
    dialog.heightConstraint = [NSLayoutConstraint constraintWithItem:dialog attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:0.0f constant:0];
    dialog.maxBottomConstraint = [NSLayoutConstraint constraintWithItem:dialog attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationLessThanOrEqual toItem:dialog.superview attribute:NSLayoutAttributeBottom multiplier:1.0f constant:-15];
    
    NSArray *constraintArray = @[topConstraint, dialog.maxBottomConstraint, leftConstraint, rightConstraint];
    
    dialog.heightConstraint.priority = 998;
    dialog.maxBottomConstraint.priority = 999;
    
    [dialog.superview addConstraints:@[topConstraint, leftConstraint, rightConstraint, dialog.heightConstraint, dialog.maxBottomConstraint]];

    ViewManager * viewManager = [self getViewManager:viewId];
    viewManager.containerView = dialog;

    if (self.webView) {
	[self.view bringSubviewToFront:self.webView];
    }

    CGFloat topConstraintConstantFinal = topConstraint.constant;
    CGFloat leftConstraintConstantFinal = leftConstraint.constant;
    CGFloat rightConstraintConstantFinal = rightConstraint.constant;
    CGFloat bottomConstraintConstantFinal = dialog.maxBottomConstraint.constant;
    
    [dialogHolder layoutIfNeeded];
    
    switch (style) {
        case AnimationStyleNone:
            break;
        case AnimationStyleTopToBottom:
            topConstraint.constant = -(topConstraint.constant + dialog.maxBottomConstraint.constant);
            dialog.maxBottomConstraint.constant = -(self.view.frame.size.height);
            break;
        /*case AnimationStyleBottomToTop:
            topConstraint.constant = self.view.frame.size.height;
            dialog.maxBottomConstraint.constant = 0;//self.view.frame.size.height - dialog.maxBottomConstraint.constant + topConstraint.constant;
            break;*/
        case AnimationStyleLeftToRight:
            leftConstraint.constant = -(leftConstraint.constant + self.view.frame.size.width);
            rightConstraint.constant = -(self.view.frame.size.width - rightConstraint.constant);
            break;
        case AnimationStyleRightToLeft:
            leftConstraint.constant = self.view.frame.size.width;
            rightConstraint.constant = self.view.frame.size.width + rightConstraint.constant;
            break;
        default:
            break;
    }
    
    //[dialog.superview addConstraints:@[topConstraint, leftConstraint, rightConstraint, dialog.heightConstraint]];//, dialog.maxBottomConstraint]];
    //dialogHolder.alpha = 1.0;
    [UIView animateWithDuration:animationDuration/2 animations:^{
        dialogHolder.alpha = 1.0;
    } completion:^(BOOL finished) {
        [UIView animateWithDuration:animationDuration/1.5 delay:0 options:UIViewAnimationOptionCurveEaseInOut animations:^{
            //dialogHolder.alpha = 1.0;
            switch (style) {
                case AnimationStyleNone:
                    break;
                case AnimationStyleTopToBottom:
                    topConstraint.constant = topConstraintConstantFinal;
                    dialog.maxBottomConstraint.constant = bottomConstraintConstantFinal;
                    break;
                /*case AnimationStyleBottomToTop:
                    topConstraint.constant = topConstraintConstantFinal;
                    dialog.maxBottomConstraint.constant = bottomConstraintConstantFinal;
                    break;*/
                case AnimationStyleLeftToRight:
                    leftConstraint.constant = leftConstraintConstantFinal;
                    rightConstraint.constant = rightConstraintConstantFinal;
                    break;
                case AnimationStyleRightToLeft:
                    leftConstraint.constant = leftConstraintConstantFinal;
                    rightConstraint.constant = rightConstraintConstantFinal;
                    break;
                default:
                    break;
            }
            [dialogHolder layoutIfNeeded];
            
        } completion:^(BOOL finished) {
            [dialog layoutIfNeeded];
        }];
    }];
    //[UIView animateWithDuration:animationDuration/2 animations:^{
    //    dialogHolder.alpha = 1.0;
    //}];
    @try {
       [self.dialogConstraints setObject:constraintArray forKey:[NSString stringWithFormat:@"%d", viewId]];
    }
    @catch (NSException *e) {
        // Some code here if it is needed at this level
        
        @throw;
    }
}

// switch view from old to new (push view over old one). If direction is NO, direction is left, otherwise right.
- (void)switchView:(int)oldViewId newView:(int)newViewId direction:(BOOL)direction
{
    NSLog(@"switchView: %d => %d", oldViewId, newViewId);
    NSLayoutConstraint *newViewLeftConstraint, *newViewRightConstraint, *oldViewLeftConstraint, *oldViewRightConstraint;
    UIView *oldView = [self viewForId:oldViewId];
    UIView *newView = [self viewForId:newViewId];    
    
    NSArray *newViewConstraints = [self.frameViewConstraints objectForKey:[NSString stringWithFormat:@"%d", newViewId]];
    NSArray *oldViewConstraints = [self.frameViewConstraints objectForKey:[NSString stringWithFormat:@"%d", oldViewId]];
    newViewLeftConstraint = (NSLayoutConstraint *)newViewConstraints[0];
    newViewRightConstraint = (NSLayoutConstraint *)newViewConstraints[1];
    oldViewLeftConstraint = (NSLayoutConstraint *)oldViewConstraints[0];
    oldViewRightConstraint = (NSLayoutConstraint *)oldViewConstraints[1];
    CGFloat newViewLeftConstraintConstantFinal = newViewLeftConstraint.constant;
    CGFloat newViewRightConstraintConstantFinal = newViewRightConstraint.constant;
    CGFloat oldViewLeftConstraintConstantFinal = oldViewLeftConstraint.constant;
    CGFloat oldViewRightConstraintConstantFinal = oldViewRightConstraint.constant;

    if (direction) { // right
        newViewLeftConstraint.constant = -self.view.frame.size.width;
        newViewRightConstraint.constant = -self.view.frame.size.width;
    } else { // left
        newViewLeftConstraint.constant = self.view.frame.size.width;
        newViewRightConstraint.constant = self.view.frame.size.width;
    }
    
    [newView.superview layoutIfNeeded];
    
    [UIView animateWithDuration:animationDuration delay:0 usingSpringWithDamping:1 initialSpringVelocity:0 options:UIViewAnimationOptionCurveEaseInOut animations:^{
        newViewLeftConstraint.constant = newViewLeftConstraintConstantFinal;
        newViewRightConstraint.constant = newViewRightConstraintConstantFinal;
        if (direction) {
            oldViewLeftConstraint.constant = self.view.frame.size.width/2;
            oldViewRightConstraint.constant = self.view.frame.size.width/2;
        } else {
            oldViewLeftConstraint.constant = -self.view.frame.size.width/2;
            oldViewRightConstraint.constant = -self.view.frame.size.width/2;
        }
        [newView.superview layoutIfNeeded];
    } completion:^(BOOL finished) {
        oldViewLeftConstraint.constant = oldViewLeftConstraintConstantFinal;
        oldViewRightConstraint.constant = oldViewRightConstraintConstantFinal;
        [oldView.superview layoutIfNeeded];
        [oldView layoutIfNeeded];
        [newView layoutIfNeeded];
    }];
}

- (void)createTimer:(int)viewId interval:(double)interval
{
    NSTimer *timer = [NSTimer scheduledTimerWithTimeInterval:interval
                                                      target:self
                                                    selector:@selector(sendTimerEvent:)
                                                    userInfo:nil
                                                     repeats:YES];
    [[NSRunLoop mainRunLoop] addTimer:timer forMode:NSRunLoopCommonModes];
}

- (void)createWebBrowserWithUrl:(NSString *)url
{
    NSLog(@"WebView opened");
    self.currentURL = [NSURL URLWithString:url];
    
    if (self.webView == nil) {
        CGFloat statusBarHeight = [UIApplication sharedApplication].statusBarFrame.size.height;
        CGFloat navBarHeight = 44.0;
        WKWebViewConfiguration *configuration = [[WKWebViewConfiguration alloc] init];
        CGRect frame = CGRectMake(0, statusBarHeight, self.view.bounds.size.width, self.view.bounds.size.height);
        self.webView = [[WKWebView alloc] initWithFrame:frame configuration:configuration];
	    self.webView.translatesAutoresizingMaskIntoConstraints = false;
        self.webView.UIDelegate = self;
        self.webView.navigationDelegate = self;
        self.webView.scrollView.contentInset = UIEdgeInsetsMake(navBarHeight, 0, 0, 0);
        [self.view addSubview:self.webView];
        
        NSLayoutConstraint *topConstraint = [NSLayoutConstraint constraintWithItem:self.webView attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:self.webView.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:statusBarHeight];
        NSLayoutConstraint *leftConstraint = [NSLayoutConstraint constraintWithItem:self.webView attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:self.webView.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
        NSLayoutConstraint *rightConstraint = [NSLayoutConstraint constraintWithItem:self.webView attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:self.webView.superview attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
        NSLayoutConstraint *bottomConstraint = [NSLayoutConstraint constraintWithItem:self.webView attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:self.webView.superview attribute:NSLayoutAttributeBottom multiplier:1.0f constant:0];
        [self.webView.superview addConstraints:@[topConstraint, leftConstraint, rightConstraint, bottomConstraint]];

        UINavigationBar *navBar = [[UINavigationBar alloc] initWithFrame:CGRectMake(0, 0, CGRectGetWidth(self.view.frame), navBarHeight)];
        navBar.delegate = self;
        
        CGFloat width = self.view.frame.size.width * 0.6; // just some width related to width of the view
        
        UINavigationItem *navItem = [[UINavigationItem alloc] initWithTitle:@""];
        UILabel *titleLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, width, 22)];
        titleLabel.text = url;
        titleLabel.adjustsFontSizeToFitWidth = YES;
        [titleLabel.widthAnchor constraintEqualToConstant:width].active = YES;
        titleLabel.minimumScaleFactor = 0.6;
        titleLabel.textAlignment = NSTextAlignmentCenter;
        navItem.titleView = titleLabel;
        
        UIImage *backImage = [UIImage imageNamed:@"backButton"];
        UIBarButtonItem *closeButton;
        if (backImage != nil) {
            closeButton = [[UIBarButtonItem alloc] initWithImage:backImage style:UIBarButtonItemStylePlain target:self action:@selector(webViewCloseButtonPushed:)];
        } else {
            closeButton = [[UIBarButtonItem alloc] initWithTitle:@"<" style:UIBarButtonItemStyleDone target:self action:@selector(webViewCloseButtonPushed:)];
        }
        navItem.leftBarButtonItem = closeButton;
        
        UIImage *moreImage = [UIImage imageNamed:@"moreButton"];
        UIBarButtonItem *actionButton;
        if (moreImage != nil) {
            actionButton = [[UIBarButtonItem alloc] initWithImage:moreImage style:UIBarButtonItemStylePlain target:self action:@selector(webViewActionButtonPushed:)];
        } else {
            actionButton = [[UIBarButtonItem alloc] initWithTitle:@"⋮" style:UIBarButtonItemStyleDone target:self action:@selector(webViewActionButtonPushed:)];
        }
        navItem.rightBarButtonItem = actionButton;
        
        [navBar setItems:@[navItem]];
        //navBar.translucent = YES;
        navBar.barStyle = UIBarStyleDefault;
        [self.webView addSubview:navBar];
        
    }
    // self.webView.layer.zPosition = 1000000.0f;
    [self.view bringSubviewToFront:self.webView];
    
    //NSURL *webURL = [NSURL URLWithString:url];
    NSURLRequest *request = [NSURLRequest requestWithURL:self.currentURL];
    [self.webView loadRequest:request];
}

- (void)webViewCloseButtonPushed:(UIButton *)button
{
    NSLog(@"closeButton pushed");
    [self.webView removeFromSuperview];
    self.webView = nil;
}

- (void)webViewActionButtonPushed:(UIBarButtonItem *)barButton
{
    UIAlertController *actionSheet = [UIAlertController alertControllerWithTitle:nil message:nil preferredStyle:UIAlertControllerStyleActionSheet];
    
    [actionSheet addAction:[UIAlertAction actionWithTitle:@"Open with Safari" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [[UIApplication sharedApplication] openURL:self.currentURL options:@{} completionHandler:nil];
    }]];
    [actionSheet addAction:[UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:^(UIAlertAction *action) {
        NSLog(@"User clicked button called %@ or tapped elsewhere",action.title);
    }]];
    
    [self presentViewController:actionSheet animated:YES completion:nil];
    
}

- (void)createImagePicker
{
    UIImagePickerController * picker = [[UIImagePickerController alloc] init];
    picker.delegate = self;
    picker.allowsEditing = YES;
    picker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    [self presentViewController:picker animated:YES completion:nil];
}

- (void)shareImage:(UIImage *)image caption:(NSString *)caption {
    NSArray *activityItems = @[caption, image];  
    UIActivityViewController *activityViewControntroller = [[UIActivityViewController alloc] initWithActivityItems:activityItems applicationActivities:nil];  
    activityViewControntroller.excludedActivityTypes = @[];  
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {  
        activityViewControntroller.popoverPresentationController.sourceView = self.view;  
        activityViewControntroller.popoverPresentationController.sourceRect = CGRectMake(self.view.bounds.size.width/2, self.view.bounds.size.height/4, 0, 0);  
    }  
    [self presentViewController:activityViewControntroller animated:true completion:nil];
}

- (void)shareLink:(NSString *)link {
    NSURL *url = [NSURL URLWithString:link];
    NSArray *activityItems = @[url];  
    UIActivityViewController *activityViewControntroller = [[UIActivityViewController alloc] initWithActivityItems:activityItems applicationActivities:nil];  
    activityViewControntroller.excludedActivityTypes = @[];  
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {  
        activityViewControntroller.popoverPresentationController.sourceView = self.view;  
        activityViewControntroller.popoverPresentationController.sourceRect = CGRectMake(self.view.bounds.size.width/2, self.view.bounds.size.height/4, 0, 0);  
    }  
    [self presentViewController:activityViewControntroller animated:true completion:nil];
}

- (void)sendTimerEvent:(NSTimer *)timer
{
    mainThread->sendTimerEvent(1);
}

- (void)viewTapped:(UITapGestureRecognizer *)gesture
{
    if (gesture.state == UIGestureRecognizerStateEnded) {
        int viewId = (int)gesture.view.tag;
        [self sendIntValue:viewId value:1];
    }
}

- (void)viewTouched:(UILongPressGestureRecognizer *)gesture
{
    ViewManager * viewManager = [self getViewManager:(int)gesture.view.tag];
    if (gesture.state == UIGestureRecognizerStateBegan) {
        [viewManager switchStyle:SelectorActive];
    } else if (gesture.state == UIGestureRecognizerStateEnded) {
        [viewManager switchStyle:SelectorNormal];
    }
}

- (void)addToParent:(int)parentId view:(UIView*)view
{
    ViewManager * parentViewManager = [self getViewManager:parentId];
    if (parentViewManager == nil) {
        NSLog(@"Element %d missing in addToParent", parentId);
        return;
    }
    UIView * parentView = (UIView *)parentViewManager.containerView;

    ViewManager * viewManager = [self getViewManager:view.tag];
    viewManager.level = parentViewManager.level + 1;    

    BOOL add_constraints = NO;
  
    if ([parentView isKindOfClass:FWLayoutView.class]) {
        FWLayoutView * layout = (FWLayoutView *)parentView;
        LayoutParams * item = [LayoutParams layoutItemForView:view];
        item.level = parentViewManager.level + 1;
        [layout addItem:item];
        viewManager.layoutParams = item;
    } else if ([parentView isKindOfClass:FWScrollView.class]) {
        LayoutParams * item = [LayoutParams layoutItemForView:view];
	item.level = parentViewManager.level + 1;
        viewManager.layoutParams = item;
    
        FWScrollView * scrollView = (FWScrollView *)parentView;
	[scrollView addItem:item];
            
        if (scrollView.pagingEnabled && !scrollView.currentPageInternalId) {
	    [scrollView setPage:scrollView.currentPage];		
	    [self sendIntValue:(int)scrollView.tag value:scrollView.currentPage];
        }
    } else {
        [parentView addSubview:view];

        if (![view isKindOfClass:UINavigationBar.class]) {
            add_constraints = YES;
        }
    }
  
    if (add_constraints) {
        NSLayoutConstraint * topConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
        NSLayoutConstraint * leftConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
        NSLayoutConstraint * rightConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
        NSLayoutConstraint * bottomConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeBottom multiplier:1.0f constant:0];

        topConstraint.priority = 999 - viewManager.level;
        leftConstraint.priority = 999 - viewManager.level;
        rightConstraint.priority = 999 - viewManager.level;
        bottomConstraint.priority = 999 - viewManager.level;

        [view.superview addConstraints:@[topConstraint, leftConstraint, rightConstraint, bottomConstraint]];
    }
}

- (void)removeView:(int)viewId
{
    for (int i = 0; i < self.dialogIds.count; i++) {
        int dialogId = [[self.dialogIds objectAtIndex:i] intValue];
        if (dialogId == viewId) {
            // animating dialog removal
            UIView *dialogView = [self viewForId:viewId];

            NSArray *dialogConstraints = [self.dialogConstraints objectForKey:[NSString stringWithFormat:@"%d", viewId]];
            NSLayoutConstraint *topConstraint = dialogConstraints[0];
            NSLayoutConstraint *bottomConstraint = dialogConstraints[1];
            NSLayoutConstraint *leftConstraint = dialogConstraints[2];
            NSLayoutConstraint *rightConstraint = dialogConstraints[3];
            [UIView animateWithDuration:animationDuration animations:^{
                switch (self.dialogAnimationStyle) {
                    case AnimationStyleLeftToRight:
                        leftConstraint.constant = -self.view.frame.size.width;
                        rightConstraint.constant = -self.view.frame.size.width;
                        break;
                    case AnimationStyleTopToBottom:
                        topConstraint.constant = -self.view.frame.size.height;
                        bottomConstraint.constant = -self.view.frame.size.height;
                        break;
                    case AnimationStyleRightToLeft:
                        leftConstraint.constant = self.view.frame.size.width;
                        rightConstraint.constant = self.view.frame.size.width;
                        break;
                    default:
                        break;
                }
                [dialogView.superview layoutIfNeeded];
            } completion:^(BOOL finished) {
                NSLog(@"removing dialog %d", viewId);
                [self sendIntValue:dialogId value:0];
                // Must reiterate since the indices may have changed
	        for (int j = 0; j < self.dialogIds.count; j++) {
                    int dialogId = [[self.dialogIds objectAtIndex:j] intValue];
                    if (dialogId == viewId) {
                        [self.dialogIds removeObjectAtIndex:j];
                    }
                }
                [self.dialogConstraints removeObjectForKey:[NSString stringWithFormat:@"%d", viewId]];
            }];
            break;
        }
    }

    NSString * key = [NSString stringWithFormat:@"%d", viewId];
    ViewManager * viewManager = [self.viewsDictionary objectForKey:key];
    if (viewManager != nil) {
        if ([viewManager.view isKindOfClass:UIView.class]) {
            UIView * view = viewManager.view;
            UIView * parentView = view.superview;

            if ([parentView isKindOfClass:FWLayoutView.class]) {
                FWLayoutView * layout = (FWLayoutView*)parentView;
                [layout removeItem:viewManager.layoutParams];
            } else if ([parentView isKindOfClass:FWScrollView.class]) {
                FWScrollView * layout = (FWScrollView*)parentView;
                [layout removeItem:viewManager.layoutParams];
	        if (layout.pagingEnabled && [layout reselectCurrentPage]) {
                    [self sendIntValue:(int)layout.tag value:layout.currentPage];
                }
            }
            [view removeFromSuperview]; // some views might be added directly
  
            if (view == self.sideMenuView) {
                [self hideBackgroundOverlayViewWithAnimation:YES];
            }
        }
        [self.viewsDictionary removeObjectForKey:key];
    }
    [self.frameViewConstraints removeObjectForKey:[NSString stringWithFormat:@"%d", viewId]];
}

- (void)reorderChildWithId:(int)viewId parentId:(int)parentId newPosition:(int)position
{
    UIView *parentView = [self viewForId:parentId];
    UIView *childView = [self viewForId:viewId];
    if (parentView && childView) {
        if ([parentView isKindOfClass:[FWLayoutView class]]) {
            FWLayoutView * layout = (FWLayoutView *)parentView;
            ViewManager * viewManager = [self getViewManager:viewId];
            [layout moveItem:viewManager.layoutParams toIndex:position];
        } else if ([parentView isKindOfClass:[FWScrollView class]]) {
            FWScrollView * scrollView = (FWScrollView *)parentView;
            ViewManager * viewManager = [self getViewManager:viewId];
            [scrollView moveItem:viewManager.layoutParams toIndex:position];
            if (scrollView.pagingEnabled && [scrollView reselectCurrentPage]) {
                [self sendIntValue:(int)scrollView.tag value:scrollView.currentPage];
            }
        } else {
            [childView removeFromSuperview];
            [parentView insertSubview:childView atIndex:position];
        }
    }
}

- (void)removeChildWithId:(int)viewId parentId:(int)parentId
{
    UIView *parentView = [self viewForId:parentId];
    UIView *childView = [self viewForId:viewId];
    if (parentView && childView) {
        if ([parentView isKindOfClass:[FWLayoutView class]]) {
            FWLayoutView * layout = (FWLayoutView *)parentView;
            ViewManager * viewManager = [self getViewManager:viewId];
            [layout removeItem:viewManager.layoutParams];
        } else if ([parentView isKindOfClass:[FWScrollView class]]) {
            FWScrollView * scrollView = (FWScrollView *)parentView;
            ViewManager * viewManager = [self getViewManager:viewId];
            [scrollView removeItem:viewManager.layoutParams];
            if (scrollView.pagingEnabled && [scrollView reselectCurrentPage]) {
                [self sendIntValue:(int)scrollView.tag value:scrollView.currentPage];
            }
        }
        [childView removeFromSuperview];  // some views might be added directly
    }
}

- (void)addOption:(int)viewId optionId:(int)optionId title:(NSString *)title
{
    id view = [self viewForId:viewId];
    if ([view isKindOfClass:UIAlertController.class]) {
        UIAlertController * alertController = (UIAlertController*)view;
        [alertController addAction:[UIAlertAction actionWithTitle:title style:(optionId == 0 ? UIAlertActionStyleCancel : UIAlertActionStyleDefault) handler:^(UIAlertAction *action) {
	    NSLog(@"end modal from option");
	    [self sendIntValue:viewId value:optionId];
        }]];
    } else if ([view isKindOfClass:FWPicker.class]) {
        FWPicker * picker = (FWPicker *)view;
        [picker addOption:title];
    } else if ([view isKindOfClass:UISegmentedControl.class]) {
	UISegmentedControl * sc = (UISegmentedControl*)view;
	[sc insertSegmentWithTitle:title atIndex:sc.numberOfSegments animated:NO];
    }
}

- (id)viewForId:(int)viewId
{
    ViewManager * viewManager = [self.viewsDictionary objectForKey:[NSString stringWithFormat:@"%d", viewId]];
    return viewManager.view;
}

- (ViewManager*)getViewManager:(int)viewId
{
    return [self.viewsDictionary objectForKey:[NSString stringWithFormat:@"%d", viewId]];
}
 
- (void)sendPauseEvent {
    SysInfoEvent ev(SysInfoEvent::PAUSE);
    mainThread->sendEvent(mainThread->getApplication().getInternalId(), ev);
}

- (void)sendResumeEvent {
    SysInfoEvent ev(SysInfoEvent::RESUME);
    mainThread->sendEvent(mainThread->getApplication().getInternalId(), ev);
}

- (void)sendDestroyEvent {
    SysInfoEvent ev(SysInfoEvent::DESTROY);
    mainThread->sendEvent(mainThread->getApplication().getInternalId(), ev);
}

- (void)addView:(id)view withId:(int)viewId
{
    ViewManager * viewManager = [[ViewManager alloc] init];
    viewManager.imageCache = self.imageCache;
    viewManager.viewId = viewId;
    viewManager.view = view;
    viewManager.containerView = view;
    [self.viewsDictionary setObject:viewManager forKey:[NSString stringWithFormat:@"%d", viewId]];
}

- (void)setImageFromThread:(int)viewId data:(CGImageRef)data
{
    ImageWrapper * iw = [[ImageWrapper alloc] init];
    iw.targetElementId = viewId;
    iw.image = data;
    [self performSelectorOnMainThread:@selector(handleImage:) withObject:iw waitUntilDone:NO];
}

- (void)handleImage:(ImageWrapper*)iw
{
    ViewManager * viewManager = [self getViewManager:iw.targetElementId];
    if (viewManager) {
        [viewManager setImage:iw.image];
    }
    CGImageRelease(iw.image);
}

- (void)sendCommandsFromThread:(NSArray*)data
{
    [self performSelectorOnMainThread:@selector(handleCommands:) withObject:data waitUntilDone:NO];
}

- (void)handleCommands:(NSArray*)data
{
    unordered_set<int> changedViews;
    for (NativeCommand *command in data) {       
        @try {
        switch (command.type) {
            case CREATE_APPLICATION:
            // set app name from textValue
            break;
            
        case CREATE_FRAMEVIEW: {
            [self createFrameViewWithId:command.childInternalId parentId:command.internalId];
        }
            break;
        
        case CREATE_LINEAR_LAYOUT: {
            [self createLinearLayoutWithId:command.childInternalId parentId:command.internalId direction:command.value];         
        }
            break;
        
        case CREATE_EVENT_LAYOUT: {
            [self createEventLayoutWithId:command.childInternalId parentId:command.internalId];
        }
            break;
        
        case CREATE_FRAME_LAYOUT: {
            [self createFrameLayoutWithId:command.childInternalId parentId:command.internalId];
        }
            break;
        
        case CREATE_PAGER:
        case CREATE_FLIPPER_LAYOUT: {
            [self createPageLayoutWithId:command.childInternalId parentId:command.internalId];
        }
            break;
        
        case CREATE_TEXT: {
            [self createTextWithId:command.childInternalId parentId:command.internalId value:command.textValue autolink:(command.value & 0x01) != 0 markdown:(command.value & 0x02) != 0];
        }
            break;

        case CREATE_BUTTON: {
            [self createButtonWithId:command.childInternalId parentId:command.internalId caption:command.textValue];
        }
            break;
        
        case CREATE_TEXTFIELD: {
            [self createTextFieldWithId:command.childInternalId parentId:command.internalId value:command.textValue];
        }
            break;

        case CREATE_TEXTVIEW: {
            [self createTextViewWithId:command.childInternalId parentId:command.internalId value:command.textValue];
        }
            break;
        
        case CREATE_IMAGEVIEW: {
	    float scale = [[UIScreen mainScreen] scale];
	    int width = int(command.width / scale);
	    int height = int(command.height / scale);
            [self createImageWithId:command.childInternalId parentId:command.internalId filename:command.textValue width:width height:height];
        }
            break;
        
        case CREATE_SCROLL_LAYOUT: {
            [self createScrollLayoutWithId:command.childInternalId parentId:command.internalId];
        }
            break;
        
        case CREATE_SWITCH: {
            [self createSwitchWithId:command.childInternalId parentId:command.internalId value:command.value != 0];
        }
            break;
        
        case CREATE_ACTIONBAR: {
            [self createNavigationBar:command.childInternalId parentId:command.internalId];
        }
            break;
        
        case CREATE_NAVIGATIONBAR: {
            [self createTabBar:command.childInternalId parentId:command.internalId];
        }
            break;
        
        case CREATE_NAVIGATIONVIEW: {
            [self createNavigationView:command.childInternalId];
        }
            break;
            
        case CREATE_NAVIGATIONBAR_ITEM: {
            [self createTabBarItem:command.childInternalId parentId:command.internalId title:command.textValue];
        }
            break;
        
        case CREATE_PROGRESS_SPINNER: {
            [self createActivityIndicatorWithId:command.childInternalId parentId:command.internalId];
        }
            break;

        case CREATE_PAGE_CONTROL: {
            [self createPageControlWithId:command.childInternalId parentId:command.internalId numPages:command.value];
        }
            break;
	
        case CREATE_PICKER: {
            [self createPickerWithId:command.childInternalId parentId:command.internalId];
        }
            break;

        case CREATE_DIALOG: {
            [self createDialogWithId:command.childInternalId parentId:command.internalId title:command.textValue animationStyle:AnimationStyleTopToBottom];
        }
            break;

        case SET_STYLE: {
            Selector selector = (Selector)command.value;
            ViewManager * viewManager = [self getViewManager:command.internalId];
            if (viewManager != nil) {
                [viewManager setStyle:command.textValue value:command.textValue2 selector:(StyleSelector)selector];
                changedViews.insert(command.internalId);
            }
        }
            break;
            
        case SET_VISIBILITY: {
            [self setVisibility:command.internalId visibility:command.value];
        }
            break;
        
        case CREATE_ALERT_DIALOG: {
      	
        }
            break;
        
        case CREATE_ACTION_SHEET: {
            [self createActionSheetWithId:command.childInternalId parentId:command.internalId title:command.textValue];
        }
            break;

        case CREATE_SEGMENTED_CONTROL: {
            [self createSegmentedControl:command.childInternalId parentId:command.internalId];
	}
	    break;
	    
        case CLEAR: {
            ViewManager * viewManager = [self getViewManager:command.internalId];
            if (viewManager) {
                [viewManager clear];
            }
        }
            break;

        case RELEASE: {
            ViewManager * viewManager = [self getViewManager:command.internalId];
            if (viewManager) {
                [viewManager releaseData];
            }
        }
            break;

        case REORDER_CHILD: {
            [self reorderChildWithId:command.childInternalId parentId:command.internalId newPosition:command.value];
        }
            break;

        case REMOVE_CHILD: {
            [self removeChildWithId:command.childInternalId parentId:command.internalId];
        }
            break;
        
        case DELETE_ELEMENT: {
            [self removeView:command.internalId];
        }
            break;
        
        case ADD_IMAGE_URL: {
            ViewManager * viewManager = [self getViewManager:command.internalId];
            if (viewManager != nil) {
	        float scale = [[UIScreen mainScreen] scale];
		int width = int(command.width / scale);
		int height = int(command.height / scale);
                [viewManager addImageUrl:command.textValue width:width height:height];
            }
        }
            break;

	case FLUSH_VIEW: {
	    ViewManager * viewManager = [self getViewManager:command.internalId];
            if (viewManager != nil) {
                [viewManager flush];
            }        
	}
	    break;
        
        case SET_INT_VALUE: {
            if (command.internalId == 1) {
                if (command.childInternalId != self.activeViewId) {
                    int oldActiveViewId = self.activeViewId;
#if 1
                    if (self.activeViewId && ![self isParentView:self.activeViewId childId:command.childInternalId]) {
                        [self switchView:self.activeViewId newView:command.childInternalId direction:NO];
                    } else {
                        [self setVisibility:command.childInternalId visibility:1];
                    }
                    self.activeViewId = command.childInternalId;
#else
                    if (self.activeViewId && ![self isParentView:self.activeViewId childId:command.childInternalId]) {
                        [self setVisibility:self.activeViewId visibility:0];
                    }
                    self.activeViewId = command.childInternalId;
                    [self setVisibility:self.activeViewId visibility:1];
#endif
		    [self sendVisibilityUpdate:oldActiveViewId];
		    [self sendVisibilityUpdate:self.activeViewId];
#if 0
                    NSString * title = [NSString stringWithUTF8String:command.getTextValue().c_str()];
                    [self setTitle:title];
#endif
                }
            } else {
                ViewManager * viewManager = [self getViewManager:command.internalId];
                if (viewManager != nil) {
                    if ([viewManager.view isKindOfClass:FWScrollView.class]) {
                        FWScrollView * scrollView = (FWScrollView *)viewManager.view;
                        if (scrollView.isPagingEnabled) {
			    [scrollView showPage:command.value animated:NO];
			    if (scrollView == self.pageView) {
                                [self updateTabBars:command.value];
				[self sendVisibilityUpdate:scrollView.tag];
			    }
                        }
                    } else {
                        [viewManager setIntValue:command.value];
                    }
                }
            }
        }
            break;

        case SET_TEXT_VALUE: {
            ViewManager * viewManager = [self getViewManager:command.internalId];
            if (viewManager != nil) {
                if ([viewManager.view isKindOfClass:UINavigationBar.class]) {
                    [self setTitles:command.textValue subtitle:(command.textValue2 != nil ? command.textValue2 : @"")];
                } else {
                    [viewManager setTextValue:command.textValue];
                }
            }
        }
            break;

	case RESHAPE_TABLE: {
	    ViewManager * viewManager = [self getViewManager:command.internalId];
	    if (viewManager != nil) {
	        [viewManager reshapeTable:command.value];
	    }
	}
	    break;
	
        case ADD_OPTION: {
            [self addOption:command.internalId optionId:command.value title:command.textValue];
        }
            break;
        
        case LAUNCH_BROWSER: {
            [self createWebBrowserWithUrl:(NSString *)command.textValue];
        }
            break;
        
        case CREATE_TIMER: {
            [self createTimer:command.internalId interval:command.value / 1000.0];
        }
            break;
        
        case UPDATE_PREFERENCE: {
            [[NSUserDefaults standardUserDefaults] setValue:command.textValue2 forKey:command.textValue];
        }
            break;
        
        case DELETE_PREFERENCE: {
            [[NSUserDefaults standardUserDefaults] setValue:nil forKey:command.textValue];
        }
            break;
            
        case COMMIT_PREFERENCES: {
            [[NSUserDefaults standardUserDefaults] synchronize];
        }
            break;
            
        case STOP: {
	    ViewManager * viewManager = [self getViewManager:command.internalId];
	    if (viewManager != nil) {
	        [viewManager stop];
	    }            
        }
            break;
        case SET_BACK_BUTTON_VISIBILITY: {
            [self setBackButtonVisibility:command.value ? true : false];
        }
            break;
        case TOGGLE_MENU: {
            [self menuButtonTapped];
        }
            break;
        case LIST_PRODUCTS: {
            [self getInAppPurchaseProductsForIdentifiers:command.textValue];
        }
            break;
        case BUY_PRODUCT: {
            [self buyProductWithIdentifier:command.textValue];
        }
            break;
        case LIST_PURCHASES:
            break;
        case CONSUME_PURCHASE:
            break;
        case SHARE_LINK:
            [self shareLink:command.textValue];
            break;
        case SELECT_FROM_GALLERY:
            [self createImagePicker];
            break;
        }
	}
	@catch (NSException *e) {
	    [self exceptionThrown:e];
    }
    }

    for (auto id : changedViews) {
        ViewManager * viewManager = [self getViewManager:id];
        if (viewManager != nil) {
            [viewManager applyStyles:NO];
        }
    }
}

- (void)exceptionThrown:(NSException *)exception
{
    NSLog(@"exception.name: %@", exception.name);
    NSLog(@"exception.reason: %@", exception.reason);
    [self sendExceptionReason:exception.reason];
}

- (void)sendExceptionReason:(NSString *)reason
{
    // Code here that sends exception text to application
}

// This method send changed integer or boolean values back to application.
// For example, a button click sends value 1
- (void)sendIntValue:(int)viewId value:(int)value {
    mainThread->sendIntValue(viewId, value);
}

// This method send changed text values back to application.
- (void)sendTextValue:(int)viewId value:(NSString *)value {
    string s = [value cStringUsingEncoding:NSUTF8StringEncoding];
    mainThread->sendTextValue(viewId, s);
}

- (void)setTitles:(NSString*)title subtitle:(NSString*)subtitle
{
    NSLog(@"setting titles: %@ %@", title, subtitle);
    self.currentTitle = title;
    self.navBarTitle.text = title;
    self.navBarSubtitle.text = subtitle;
}

- (void)setBackButtonVisibility:(BOOL)v {
    if (v) {
        UIImage *image = [self.imageCache loadIcon:@"icons_arrow-left-red.png"];
        UIBarButtonItem *backButton = [[UIBarButtonItem alloc] initWithImage:image style:UIBarButtonItemStylePlain target:self action:@selector(backButtonTapped)];
        self.navItem.leftBarButtonItem = backButton;
    } else {
        UIImage *image = [self.imageCache loadIcon:@"icons_hamburger-menu.png"];
        UIBarButtonItem *menuButton;
	if (image == nil) {
            menuButton = [[UIBarButtonItem alloc] initWithTitle:@"Menu" style:UIBarButtonItemStylePlain target:self action:@selector(menuButtonTapped)];
	} else {
	    menuButton = [[UIBarButtonItem alloc] initWithImage:image style:UIBarButtonItemStylePlain target:self action:@selector(menuButtonTapped)];
	}
        self.navItem.leftBarButtonItem = menuButton;
    }
}

- (BOOL)isParentView:(int)parentId childId:(int)childId
{
    id id1 = [self viewForId:parentId];
    id id2 = [self viewForId:childId];
    if ([id1 isKindOfClass:UIView.class] && [id2 isKindOfClass:UIView.class]) {
        UIView * parentView = (UIView *)id1;
        UIView * childView = (UIView *)id2;
        return [childView isDescendantOfView:parentView];
    } else {
        return NO;
    }
}

#pragma mark - In-App Purchase stuff

// delegate methods
- (void)inAppPurchaseManagerDidReceiveProducts:(InAppPurchaseManager *)manager {
    // show in-app purchase UI
    // [self showInAppPurchaseView];
    NSLog(@"products: %@", manager.products);
}

- (void)inAppPurchaseManagerDidCompleteTransaction:(InAppPurchaseManager *)manager forProductId:(NSString *)productId {
    NSLog(@"purchased: %@", productId);
    // maybe send some event to application that purchase was completed (so it can unlock features etc.)
}
    
- (void)addProductIds:(NSString *)productIds { // comma separated
    NSArray *stringArray = [productIds componentsSeparatedByString:@","];
    [InAppPurchaseManager.sharedInstance addProductIds:stringArray];
}

// make products request if product id's have been added
- (void)getInAppPurchaseProducts {
    [InAppPurchaseManager.sharedInstance makeProductsRequest];
}
    
// Get product identifiers from application
- (void)getInAppPurchaseProductsForIdentifiers:(NSString *)identifiers { // comma separated
    NSArray *stringArray = [identifiers componentsSeparatedByString:@","];
    NSSet *productIds = [NSSet setWithArray:stringArray];
    InAppPurchaseManager.sharedInstance.productIdentifiers = (NSMutableSet *)productIds;
    [InAppPurchaseManager.sharedInstance makeProductsRequest]; // When this completes it calls delegate method inAppPurchaseManagerDidReceiveProducts (above)
}

- (void)buyProductWithIdentifier:(NSString *)identifier {
    [InAppPurchaseManager.sharedInstance buyProductWithIdentifier:identifier];
}

#pragma mark - UIImagePickerControllerDelegate

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info {
    UIImage *chosenImage = info[UIImagePickerControllerEditedImage];
    // self.imageView.image = chosenImage;
    
    [picker dismissViewControllerAnimated:YES completion:NULL];
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker {
    [picker dismissViewControllerAnimated:YES completion:NULL];
}

#pragma mark - FWImageViewDelegate

- (void)fwImageView:(FWImageView *)imageView didChangeSize:(CGSize)size ofImageUrl:(NSString *)url
{
    string s;
    if (url != nil) {
      s = [url cStringUsingEncoding:NSUTF8StringEncoding];
    }
    mainThread->sendImageRequest((int)imageView.tag, size.width, size.height, s, 15); // RGB555
}

- (void)didCancelImageRequest:(FWImageView *)imageView
{
    mainThread->cancelImageRequest((int)imageView.tag);
}

#pragma mark - PaddedLabelDelegate

- (void)paddedLabel:(PaddedLabel *)label didOpenLinkURL:(NSURL *)url
{
    NSString *urlString = url.absoluteString;
    [self createWebBrowserWithUrl:urlString];
}

#pragma mark - WKNavigationDelegate

- (void)webView:(WKWebView *)webView didCommitNavigation:(WKNavigation *)navigation
{
    for (UIView *view in self.webView.subviews) {
        if ([view isKindOfClass:UINavigationBar.class]) {
            UINavigationBar *navBar = (UINavigationBar *)view;
            for (UINavigationItem *item in navBar.items) {
                UILabel *titleLabel = (UILabel *)item.titleView;
                if (titleLabel != nil) {
                    titleLabel.text = webView.URL.absoluteString;
                }
            }
        }
    }
}

@end
