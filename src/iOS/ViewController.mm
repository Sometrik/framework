#import "ViewController.h"

#include <FWApplication.h>
#include <FWDefs.h>
#include <SysInfoEvent.h>
#include <VisibilityUpdateEvent.h>

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

#import <WebKit/WebKit.h>

#include <memory>

using namespace std;

// Pointer to main thread
std::shared_ptr<iOSMainThread> mainThread;

// Declare C++ function
extern FWApplication * applicationMain();

@interface ViewController () <UIScrollViewDelegate, UITabBarDelegate, InAppPurchaseManagerDelegate, FWImageViewDelegate, UITextFieldDelegate, UITextViewDelegate, UIGestureRecognizerDelegate, PaddedLabelDelegate, WKUIDelegate, WKNavigationDelegate>
@property (nonatomic, strong) NSMutableDictionary *viewsDictionary;
@property (nonatomic, strong) UIView *sideMenuView;
@property (nonatomic, strong) UIView *backgroundOverlayView;
@property (nonatomic, strong) UITabBar *tabBar;
@property (nonatomic, strong) UINavigationBar *navBar;
@property (nonatomic, strong) UINavigationItem *navItem;
@property (nonatomic, strong) UIToolbar *statusBarBackgroundView;
@property (nonatomic, strong) UIScrollView *pageView;
@property (nonatomic, strong) NSMutableArray *dialogIds;
@property (nonatomic, assign) int activeViewId;
@property (nonatomic, assign) BOOL sideMenuPanned;
@property (nonatomic, strong) WKWebView *webView;
@property (nonatomic, strong) InAppPurchaseManager *inAppPurchaseManager;
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

    self.view.layoutMargins = UIEdgeInsetsMake(0, 0, 0, 0);
    self.automaticallyAdjustsScrollViewInsets = NO;
    //  [self.topLayoutGuide
//  self.bottomLayoutGuide.length = 44;
  
    self.backgroundOverlayView = [self createBackgroundOverlay:self.view];
    self.backgroundOverlayView.hidden = YES;
    self.backgroundOverlayView.tag = 0;
  
    self.inAppPurchaseManager = [InAppPurchaseManager sharedInstance];

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
  
    mainThread = make_shared<iOSMainThread>(application, application);
    mainThread->viewController = self;
//    mainThread->setActualDisplayWidth(screenWidth);
//    mainThread->setActualDisplayHeight(screenHeight);
    mainThread->setDisplayScale(scale);
  
    NSLog(@"starting app, scale = %f, version = %@, build = %@", scale, version, build);

    [self addView:self.view withId:1];

    mainThread->start();
    
    // Add notification handlers to catch notifications when keyboard opens and closes
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(handleKeyboardWillShowNotification:) name:UIKeyboardWillShowNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(handleKeyboardWillHideNotification:) name:UIKeyboardWillHideNotification object:nil];
    
    // Add tap gesture recognizer to self.view so when tapped, keyboard should close
    UITapGestureRecognizer *tapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(backgroundTapped:)];
    tapGestureRecognizer.delegate = self;
    [self.view addGestureRecognizer:tapGestureRecognizer];
    
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

// textField or textView viewId should be used.
- (void)activateKeyboardForViewId:(int)viewId
{
    id view = [self viewForId:viewId];
    if ([view isKindOfClass:UITextField.class] || [view isKindOfClass:UITextView.class]) {
        [view becomeFirstResponder];
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
  // self.view.frame = CGRectMake(0, 0, size.width, size.height);
  // [self.view setNeedsLayout];

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
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    if (self.navBar) {
        [self.statusBarBackgroundView.superview bringSubviewToFront:self.statusBarBackgroundView];
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
            }
        }
    }
}

- (void)sendVisibilityUpdate
{
    VisibilityUpdateEvent ev;
    mainThread->sendEvent(mainThread->getApplication().getInternalId(), ev);
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

    // CGFloat tabBarHeight = self.tabBar == nil ? 0.0 : 44.0;
    // CGFloat topBarsHeight = self.navBar == nil ? 0.0 : 64.0;
    FrameLayoutView *view = [[FrameLayoutView alloc] init];

    view.tag = viewId;
    view.layoutMargins = UIEdgeInsetsMake(0, 0, 0, 0);
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
            [self.statusBarBackgroundView.superview bringSubviewToFront:self.statusBarBackgroundView];
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

- (void)createTextWithId:(int)viewId parentId:(int)parentId value:(NSString*)value autolink:(BOOL)autolink
{
    PaddedLabel *label = [[PaddedLabel alloc] init];
    label.tag = viewId;
    label.delegate = self;
    label.autolink = autolink;
    if (autolink) {
        label.userInteractionEnabled = YES;
        label.attributedText = [label createAttributedString:value];
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

- (void)createSwitchWithId:(int)viewId parentId:(int)parentId
{
    UISwitch *sw = [[UISwitch alloc] init];
    sw.tag = viewId;
    sw.translatesAutoresizingMaskIntoConstraints = false;
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
    if (filename != nil && ![filename hasPrefix:@"http://"] && ![filename hasPrefix:@"https://"]) {
        UIImage *image = [UIImage imageNamed:filename];
        imageView = [[FWImageView alloc] initWithImage:image];
    } else {
        imageView = [[FWImageView alloc] init];
        [imageView addImageUrl:filename width:width height:height];
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
    scrollView.contentInset = UIEdgeInsetsMake(0, 0, 0, 0);
    scrollView.clipsToBounds = YES;
    scrollView.delegate = self;
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
            fwScrollView.currentPage = page;

            [self sendIntValue:(int)scrollView.tag value:(int)page];
            
	    if (scrollView == self.pageView) {
                // set selected item for all tabbars
                [self updateTabBars:page];
            }
	    [self sendVisibilityUpdate];
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
}

- (void)createPageLayoutWithId:(int)viewId parentId:(int)parentId
{
    FWScrollView * scrollView = [[FWScrollView alloc] init];
    scrollView.tag = viewId;
    scrollView.pagingEnabled = YES;
    scrollView.contentInset = UIEdgeInsetsMake(0, 0, 0, 0);
    scrollView.layoutMargins = UIEdgeInsetsMake(0, 0, 0, 0);
    scrollView.clipsToBounds = YES;
    scrollView.delegate = self;
    if (self.pageView == nil) self.pageView = scrollView;
    [self addView:scrollView withId:viewId];
    [self addToParent:parentId view:scrollView];

    [scrollView.panGestureRecognizer requireGestureRecognizerToFail:self.panEdgeGestureRecognizer];
}

- (void)showPage:(UIScrollView *)scrollView page:(NSInteger)page animated:(BOOL)animated
{
    CGRect frame = scrollView.frame;
    frame.origin.x = frame.size.width * page;
    frame.origin.y = 0;
    [scrollView scrollRectToVisible:frame animated:animated];
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
    if ([otherGestureRecognizer isEqual:self.pageView.panGestureRecognizer] && [gestureRecognizer isEqual:self.panEdgeGestureRecognizer]) {
        return NO;
    }
    return YES;
}
*/

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldBeRequiredToFailByGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
{
    if ([otherGestureRecognizer isKindOfClass:UIPanGestureRecognizer.class] && [gestureRecognizer isEqual:self.panEdgeGestureRecognizer]) {
        return YES;
        
    }
    return NO;
}

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
    //navBar.layer.zPosition = 1000;

    if (self.currentTitle != nil) {
      self.navItem = [[UINavigationItem alloc] initWithTitle:self.currentTitle];
    } else {
      self.navItem = [[UINavigationItem alloc] init];
    }

    // Add debug event by tapping nav bar 5 times
    UITapGestureRecognizer *debugTapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(navBarTapped5Times:)];
    debugTapGesture.numberOfTapsRequired = 5;
    [navBar addGestureRecognizer:debugTapGesture];
     
    UIBarButtonItem *menuButton;
    UIImage *image = [self loadImage:@"icons_hamburger-menu.png"];
    if (image == nil) {
        menuButton = [[UIBarButtonItem alloc] initWithTitle:@"Menu" style:UIBarButtonItemStylePlain target:self action:@selector(menuButtonTapped)];
    } else {
        menuButton = [[UIBarButtonItem alloc] initWithImage:image style:UIBarButtonItemStylePlain target:self action:@selector(menuButtonTapped)];
    }
    self.navItem.leftBarButtonItem = menuButton;

    UIImage *image2 = [self loadImage:@"write_icon_small.png"];
    if (image2 != nil) {
        UIBarButtonItem *composeButton = [[UIBarButtonItem alloc] initWithImage:image2 style:UIBarButtonItemStylePlain target:self action:@selector(composeButtonTapped)];
        self.navItem.rightBarButtonItem = composeButton;
    }

    [navBar setItems:@[self.navItem]];

    UIToolbar *statusBarBackgroundView = [[UIToolbar alloc] init];
    // statusBarBackgroundView.barStyle = UIStatusBarStyleDefault;
    statusBarBackgroundView.translucent = YES;
    statusBarBackgroundView.translatesAutoresizingMaskIntoConstraints = false;
    // statusBarBackgroundView.layer.zPosition = 1000;
    
    UIView * parentView = (UIView *)[self viewForId:parentId];
    [parentView addSubview:statusBarBackgroundView];

    [self addView:navBar withId:viewId];
    // [self addToParent:parentId view:navBar];
    [parentView addSubview:navBar];

    NSLayoutConstraint *topConstraint = [NSLayoutConstraint constraintWithItem:statusBarBackgroundView attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:statusBarBackgroundView.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
    NSLayoutConstraint *leftConstraint = [NSLayoutConstraint constraintWithItem:statusBarBackgroundView attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:statusBarBackgroundView.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
    NSLayoutConstraint *rightConstraint = [NSLayoutConstraint constraintWithItem:statusBarBackgroundView attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:statusBarBackgroundView.superview attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
    NSLayoutConstraint *bottomConstraint = [NSLayoutConstraint constraintWithItem:statusBarBackgroundView attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:statusBarBackgroundView.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:statusBarHeight];
    [statusBarBackgroundView.superview addConstraints:@[topConstraint, leftConstraint, rightConstraint, bottomConstraint]];
    
    NSLayoutConstraint *topConstraint2 = [NSLayoutConstraint constraintWithItem:navBar attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:navBar.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:statusBarHeight];
    NSLayoutConstraint *leftConstraint2 = [NSLayoutConstraint constraintWithItem:navBar attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:navBar.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
    NSLayoutConstraint *rightConstraint2 = [NSLayoutConstraint constraintWithItem:navBar attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:navBar.superview attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
    NSLayoutConstraint *bottomConstraint2 = [NSLayoutConstraint constraintWithItem:navBar attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:navBar.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:statusBarHeight + 44];
    [navBar.superview addConstraints:@[topConstraint2, leftConstraint2, rightConstraint2, bottomConstraint2]];
    
    self.statusBarBackgroundView = statusBarBackgroundView;
    self.navBar = navBar;
}

- (void)navBarTapped5Times:(UITapGestureRecognizer *)recognizer
{
    mainThread->startDebugMode();
}

- (void)menuButtonTapped
{
    if (self.sideMenuView.isHidden) {
        [self showNavigationViewWithAnimation:NO];
    } else {
	[self hideNavigationViewWithAnimation:NO];
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
        [self showPage:self.pageView page:itemIndex animated:NO];
	[self sendVisibilityUpdate];
    }
    [self sendIntValue:(int)item.tag value:1];
}

/*
 - (void)updateTabBarVisibility:(int)pageIndex
 {
 BOOL hidden = NO;
 
 for (NSNumber *number in self.tabBarHiddenInThesePages) {
 if (pageIndex == number.intValue) {
 hidden = YES;
 break;
 }
 }
 
 if (hidden) {
 if (self.tabBar) {
 // Hide from first page
 [self.tabBar setHidden:YES];
 }
 } else {
 if (self.tabBar) {
 // Show elsewhere
 [self.tabBar setHidden:NO];
 }
 }
 }
 */

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
    
    UIPanGestureRecognizer *panGesture = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(sideMenuPanned:)];
    [self.sideMenuView addGestureRecognizer:panGesture];

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
        [self sendIntValue:self.sideMenuView.tag value:1];
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
    NSLog(@"panned location = %@", NSStringFromCGPoint(touchLocation));
    BOOL gestureIsDraggingFromLeftToRight = [recognizer velocityInView:self.view].x > 0;
    
    if (recognizer.state == UIGestureRecognizerStateBegan) {
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
        if (CGRectGetMaxX(self.sideMenuView.frame) > (CGRectGetWidth(self.view.frame)-sideMenuOpenSpaceWidth) / 2) {
            [self showNavigationViewWithAnimation:YES];
            self.sideMenuPanned = NO;
        } else {
            [self hideNavigationViewWithAnimation:YES];
            self.sideMenuPanned = NO;
        }
    }
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

- (void)createPickerWithId:(int)viewId parentId:(int)parentId
{
    FWPicker * view = [[FWPicker alloc] init];
    view.tag = viewId;

    UIImage *image = [self loadImage:@"icons_arrow-down.png"];
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
        FWScrollView * scrollView = (FWScrollView *)parentView;
        if (scrollView.pagingEnabled) {
            LayoutParams * item = [LayoutParams layoutItemForView:view];
            item.level = parentViewManager.level + 1;
            [scrollView addItem:item];
            viewManager.layoutParams = item;
        } else {
            [parentView addSubview:view];
            scrollView.topConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
            scrollView.leftConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
            scrollView.widthConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:0.0f constant:0];
            scrollView.heightConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:0.0f constant:0];

            scrollView.topConstraint.priority = 999 - viewManager.level;
            scrollView.leftConstraint.priority = 999 - viewManager.level;
            scrollView.widthConstraint.priority = 999 - viewManager.level;
            scrollView.heightConstraint.priority = 999 - viewManager.level;

            [view.superview addConstraints:@[scrollView.topConstraint, scrollView.leftConstraint, scrollView.widthConstraint, scrollView.heightConstraint]];
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
                UIView * view = (UIView*)viewManager.view;
            } else if ([parentView isKindOfClass:FWScrollView.class]) {
                FWScrollView * layout = (FWScrollView*)parentView;
                [layout removeItem:viewManager.layoutParams];
                UIView * view = (UIView*)viewManager.view;
            } else {
                UIView * view = (UIView*)viewManager.view;
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

- (void)addView:(id)view withId:(int)viewId
{
    ViewManager * viewManager = [[ViewManager alloc] init];
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
        switch (command.type) {
            case CREATE_APPLICATION:
            // set app name from textValue
            break;
            
        case CREATE_FRAMEVIEW: {
            @try {
                [self createFrameViewWithId:command.childInternalId parentId:command.internalId];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;
        
        case CREATE_LINEAR_LAYOUT: {
            @try {
                [self createLinearLayoutWithId:command.childInternalId parentId:command.internalId direction:command.value];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;
        
        case CREATE_EVENT_LAYOUT: {
            @try {
                [self createEventLayoutWithId:command.childInternalId parentId:command.internalId];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;
        
        case CREATE_FRAME_LAYOUT: {
            @try {
                [self createFrameLayoutWithId:command.childInternalId parentId:command.internalId];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;
        
        case CREATE_PAGER:
        case CREATE_FLIPPER_LAYOUT: {
            @try {
                [self createPageLayoutWithId:command.childInternalId parentId:command.internalId];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;
        
        case CREATE_TEXT: {
            @try {
                [self createTextWithId:command.childInternalId parentId:command.internalId value:command.textValue autolink:command.value != 0];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;

        case CREATE_BUTTON: {
            [self createButtonWithId:command.childInternalId parentId:command.internalId caption:command.textValue];
        }
            break;
        
        case CREATE_TEXTFIELD: {
            @try {
                [self createTextFieldWithId:command.childInternalId parentId:command.internalId value:command.textValue];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;

        case CREATE_TEXTVIEW: {
            @try {
                [self createTextViewWithId:command.childInternalId parentId:command.internalId value:command.textValue];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;
        
        case CREATE_IMAGEVIEW: {
            @try {
                [self createImageWithId:command.childInternalId parentId:command.internalId filename:command.textValue width:command.width height:command.height];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;
        
        case CREATE_SCROLL_LAYOUT: {
            @try {
                [self createScrollLayoutWithId:command.childInternalId parentId:command.internalId];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;
        
        case CREATE_SWITCH: {
            @try {
                [self createSwitchWithId:command.childInternalId parentId:command.internalId];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;
        
        case CREATE_ACTIONBAR: {
            @try {
                [self createNavigationBar:command.childInternalId parentId:command.internalId];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;
        
        case CREATE_NAVIGATIONBAR: {
            @try {
                [self createTabBar:command.childInternalId parentId:command.internalId];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;
        
        case CREATE_NAVIGATIONVIEW: {
            @try {
                [self createNavigationView:command.childInternalId];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;
            
        case CREATE_NAVIGATIONBAR_ITEM: {
            @try {
                [self createTabBarItem:command.childInternalId parentId:command.internalId title:command.textValue];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;
        
        case CREATE_PROGRESS_SPINNER: {
            @try {
                [self createActivityIndicatorWithId:command.childInternalId parentId:command.internalId];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;

        case CREATE_PAGE_CONTROL: {
            @try {
                [self createPageControlWithId:command.childInternalId parentId:command.internalId numPages:command.value];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;
	
        case CREATE_PICKER: {
            @try {
                [self createPickerWithId:command.childInternalId parentId:command.internalId];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;

        case CREATE_DIALOG: {
            @try {
                [self createDialogWithId:command.childInternalId parentId:command.internalId title:command.textValue animationStyle:AnimationStyleTopToBottom];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
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
            @try {
                [self setVisibility:command.internalId visibility:command.value];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;
        
        case CREATE_ALERT_DIALOG: {
      	
        }
            break;
        
        case CREATE_ACTION_SHEET: {
            @try {
                [self createActionSheetWithId:command.childInternalId parentId:command.internalId title:command.textValue];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;
            
        case CLEAR: {
            ViewManager * viewManager = [self getViewManager:command.internalId];
            if (viewManager) {
                [viewManager clear];
            }
        }
            break;
            
        case REORDER_CHILD: {
            @try {
                [self reorderChildWithId:command.childInternalId parentId:command.internalId newPosition:command.value];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;

        case REMOVE_CHILD: {
            @try {
                [self removeChildWithId:command.childInternalId parentId:command.internalId];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;
        
        case DELETE_ELEMENT: {
            @try {
                [self removeView:command.internalId];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;
        
        case ADD_IMAGE_URL: {
            ViewManager * viewManager = [self getViewManager:command.internalId];
            if (viewManager != nil) {
                [viewManager addImageUrl:command.textValue width:command.width height:command.height];
            }
        }
            break;
        
        case SET_INT_VALUE: {
            if (command.internalId == 1) {
                if (command.childInternalId != self.activeViewId) {
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
		    [self sendVisibilityUpdate];
#if 0
                    NSString * title = [NSString stringWithUTF8String:command.getTextValue().c_str()];
                    [self setTitle:title];
#endif
                }
            } else {
                ViewManager * viewManager = [self getViewManager:command.internalId];
                if (viewManager != nil) {
                    if ([viewManager.view isKindOfClass:UIScrollView.class]) {
                        UIScrollView * scrollView = (UIScrollView *)viewManager.view;
                        if (scrollView.isPagingEnabled) {
			    [self showPage:scrollView page:command.value animated:NO];
			    if (scrollView == self.pageView) {
                                [self updateTabBars:command.value];
				[self sendVisibilityUpdate];
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
                [viewManager setTextValue:command.textValue];
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
            @try {
                [self addOption:command.internalId optionId:command.value title:command.textValue];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;
        
        case LAUNCH_BROWSER: {
            @try {
                [self createWebBrowserWithUrl:(NSString *)command.textValue];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
        }
            break;
        
        case CREATE_TIMER: {
            @try {
                [self createTimer:command.internalId interval:command.value / 1000.0];
            }
            @catch (NSException *e) {
                [self exceptionThrown:e];
            }
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

- (void)setTitle:(NSString*)title
{
    self.currentTitle = title;

    if (self.navBar != nil) {
        self.navBar.items[0].title = title;
    }
}

- (void)setBackButtonVisibility:(BOOL)v {
    if (v) {
        UIImage *image = [self loadImage:@"icons_arrow-left-red.png"];
        UIBarButtonItem *backButton = [[UIBarButtonItem alloc] initWithImage:image style:UIBarButtonItemStylePlain target:self action:@selector(backButtonTapped)];
        self.navItem.leftBarButtonItem = backButton;
    } else {
        UIImage *image = [self loadImage:@"icons_hamburger-menu.png"];
        UIBarButtonItem *menuButton;
	if (image == nil) {
            menuButton = [[UIBarButtonItem alloc] initWithTitle:@"Menu" style:UIBarButtonItemStylePlain target:self action:@selector(menuButtonTapped)];
	} else {
	    menuButton = [[UIBarButtonItem alloc] initWithImage:image style:UIBarButtonItemStylePlain target:self action:@selector(menuButtonTapped)];
	}
        self.navItem.leftBarButtonItem = menuButton;
    }
}

- (UIImage *)loadImage:(NSString *)filename {
    NSString *maskFilePath = [[NSBundle mainBundle] pathForResource:filename ofType:nil];
    CGDataProviderRef dataProvider = CGDataProviderCreateWithFilename([maskFilePath UTF8String]);
    CGImageRef imageRef = CGImageCreateWithPNGDataProvider(dataProvider, NULL, true, kCGRenderingIntentDefault);
    UIImage * image = [UIImage imageWithCGImage:imageRef scale:3.0f orientation:UIImageOrientationUp];
    CGImageRelease(imageRef);
    CGDataProviderRelease(dataProvider);
    return image;
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

// delegate method
- (void)inAppPurchaseManagerDidReceiveProducts:(InAppPurchaseManager *)manager {
    // show in-app purchase UI
    // [self showInAppPurchaseView];
}

#pragma mark - FWImageViewDelegate

- (void)fwImageView:(FWImageView *)imageView didChangeSize:(CGSize)size ofImageUrl:(NSString *)url
{
    string s;
    if (url != nil) {
      s = [url cStringUsingEncoding:NSUTF8StringEncoding];
    }
    mainThread->sendImageRequest((int)imageView.tag, size.width, 0, s, 5); // RGBA8
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
