#import "ViewController.h"

#include <FWApplication.h>
#include "iOSMainThread.h"

#import "ImageWrapper.h"
#import "InAppPurchaseManager.h"
#import "FWImageView.h"
#import "PaddedLabel.h"
#import "LinearLayoutView.h"
#import "FrameLayoutView.h"
#import "FWScrollView.h"
#import "FWPicker.h"

#import <WebKit/WebKit.h>

#include <memory>

using namespace std;

// Pointer to main thread
std::shared_ptr<iOSMainThread> mainThread;

// Declare C++ function
extern FWApplication * applicationMain();

@interface ViewController () <UIScrollViewDelegate, UITabBarDelegate, InAppPurchaseManagerDelegate, FWImageViewDelegate, UITextFieldDelegate, UIPickerViewDelegate, UIPickerViewDataSource, UIGestureRecognizerDelegate>
@property (nonatomic, strong) NSMutableDictionary *viewsDictionary;
@property (nonatomic, strong) UIView *sideMenuView;
@property (nonatomic, strong) UIView *backgroundOverlayView;
@property (nonatomic, strong) UITabBar *tabBar;
@property (nonatomic, strong) UINavigationBar *navBar;
@property (nonatomic, strong) UIToolbar *statusBarBackgroundView;
@property (nonatomic, strong) UIScrollView *pageView;
@property (nonatomic, strong) NSMutableArray *dialogIds;
@property (nonatomic) int activeViewId;
@property (nonatomic, assign) BOOL sideMenuPanned;
@property (nonatomic, strong) WKWebView *webView;
@property (nonatomic, strong) InAppPurchaseManager *inAppPurchaseManager;
@property (nonatomic, assign) NSString * currentTitle;
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

    // Creating the C++ app
    std::shared_ptr<FWApplication> application(applicationMain());
    
    float scale = [[UIScreen mainScreen] scale];
  
    mainThread = make_shared<iOSMainThread>(application, application);
    mainThread->viewController = self;
//    mainThread->setActualDisplayWidth(screenWidth);
//    mainThread->setActualDisplayHeight(screenHeight);
    mainThread->setDisplayScale(scale);
  
    NSLog(@"starting app, scale = %f", scale);

    [self addView:self.view withId:1];

    application->initialize(mainThread.get());
    application->initializeChildren();
    application->load();
}

- (void)viewWillTransitionToSize: (CGSize)size withTransitionCoordinator:(id)coordinator
{
  [super viewWillTransitionToSize: size withTransitionCoordinator:coordinator];

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
    mainThread->sendMemoryWarning();
    [super didReceiveMemoryWarning];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    // make side menu open by swiping from the left edge of the screen
    UIScreenEdgePanGestureRecognizer *panEdgeGestureRecognizer = [[UIScreenEdgePanGestureRecognizer alloc] initWithTarget:self action:@selector(edgeSwiped:)];
    panEdgeGestureRecognizer.edges = UIRectEdgeLeft;
    [self.view addGestureRecognizer:panEdgeGestureRecognizer];
    
    // some additional setups for views
    for (NSString *key in self.viewsDictionary.allKeys) {
        ViewManager *viewManager = [self.viewsDictionary objectForKey:key];
        if ([viewManager.view isKindOfClass:UITabBar.class]) {
            UITabBar *tabBar = (UITabBar *)viewManager.view;
            [tabBar.superview bringSubviewToFront:tabBar];
        } else if ([viewManager.view isKindOfClass:UIScrollView.class]) {
            UIScrollView *scrollView = (UIScrollView *)viewManager.view;
            if (scrollView.pagingEnabled) {
                [scrollView.panGestureRecognizer requireGestureRecognizerToFail:panEdgeGestureRecognizer];
            }
        }
    }
    
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    if (self.navBar) {
        [self.view bringSubviewToFront:self.statusBarBackgroundView];
        [self.view bringSubviewToFront:self.navBar];
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

    if (viewId != 0 && self.dialogIds.count > 0) {
	NSLog(@"background overlay tapped");
        int dialogId = [[self.dialogIds lastObject] intValue];
        if (dialogId == viewId) {
            [self sendIntValue:dialogId value:0];
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

- (void)createTextFieldWithId: (int)viewId parentId:(int)parentId {
    UITextField* text = [[UITextField alloc] init];
    text.tag = viewId;
    text.borderStyle = UITextBorderStyleRoundedRect;
    text.autocorrectionType = UITextAutocorrectionTypeNo;
    text.keyboardType = UIKeyboardTypeDefault;
    text.returnKeyType = UIReturnKeyDone;
    text.clearButtonMode = UITextFieldViewModeWhileEditing;
    text.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
    text.delegate = self;
    text.translatesAutoresizingMaskIntoConstraints = false;
    // text.borderStyle = UITextBorderStyleRoundedRect;
    [text addTarget:self action:@selector(textFieldChanged:) forControlEvents:UIControlEventEditingChanged];
    [self addView:text withId:viewId];
    [self addToParent:parentId view:text];
}

- (void)textFieldChanged:(UITextField *)sender
{
    int viewId = (int)sender.tag;
    NSLog(@"viewId = %d", viewId);
    NSLog(@"textField.text = %@", sender.text);
    [self sendTextValue:viewId value:sender.text];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [textField resignFirstResponder];
    return NO;
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
            [view.superview setNeedsLayout];
        }
    }
}

// Lazy initialization
- (NSMutableDictionary *)viewsDictionary
{
    if (!_viewsDictionary) {
        _viewsDictionary = [[NSMutableDictionary alloc] init];
    }
    return _viewsDictionary;
}

- (void)createFrameViewWithId:(int)viewId parentId:(int)parentId
{
    // CGFloat tabBarHeight = self.tabBar == nil ? 0.0 : 44.0;
    // CGFloat topBarsHeight = self.navBar == nil ? 0.0 : 64.0;
    FrameLayoutView *view = [[FrameLayoutView alloc] init];

    view.tag = viewId;
    // view.layoutMargins = UIEdgeInsetsMake(topBarsHeight, 0, tabBarHeight, 0);
    view.layoutMargins = UIEdgeInsetsMake(0, 0, 0, 0);
    // view.frame = CGRectMake(0, 0, self.view.bounds.size.width, self.view.bounds.size.height);
    view.translatesAutoresizingMaskIntoConstraints = false;
    // [self.view addSubview:view];
    [self addView:view withId:viewId];
    [self addToParent:1 view:view];
    
    if (self.activeViewId == 0) {
        self.activeViewId = viewId;
    } else {
        view.hidden = YES;
    }
    
    NSLayoutConstraint *topConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
    NSLayoutConstraint *leftConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
    NSLayoutConstraint *rightConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
    NSLayoutConstraint *bottomConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeBottom multiplier:1.0f constant:0];
    [view.superview addConstraints:@[topConstraint, leftConstraint, rightConstraint, bottomConstraint]];

    if (self.navBar) {
        [self.view bringSubviewToFront:self.statusBarBackgroundView];
        [self.view bringSubviewToFront:self.navBar];
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
    layout.translatesAutoresizingMaskIntoConstraints = false;
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
    view.translatesAutoresizingMaskIntoConstraints = false;
    [self addView:view withId:viewId];
    [self addToParent:parentId view:view];
}

- (void)createTextWithId:(int)viewId parentId:(int)parentId value:(NSString*)value
{
    PaddedLabel *label = [[PaddedLabel alloc] init];
    label.tag = viewId;
    label.text = value;
    label.numberOfLines = 0; // as many lines as needed
    label.lineBreakMode = NSLineBreakByWordWrapping;
    label.translatesAutoresizingMaskIntoConstraints = false;
    label.preferredMaxLayoutWidth = self.view.frame.size.width;
    [self addView:label withId:viewId];
    [self addToParent:parentId view:label];
}

- (void)createButtonWithId:(int)viewId parentId:(int)parentId caption:(NSString *)caption
{
    UIButton *button = [UIButton buttonWithType:UIButtonTypeCustom];
    button.tag = viewId;
    button.translatesAutoresizingMaskIntoConstraints = false;
    [button setTitle:caption forState:UIControlStateNormal];
    [button addTarget:self action:@selector(buttonPushed:) forControlEvents:UIControlEventTouchUpInside];
    [button addTarget:self action:@selector(buttonTouchDown:) forControlEvents:UIControlEventTouchDown];
    [button addTarget:self action:@selector(buttonTouchUp:) forControlEvents:UIControlEventTouchUpInside|UIControlEventTouchUpOutside];
    [self addView:button withId:viewId];
    [self addToParent:parentId view:button];
}

- (void)buttonPushed:(UIButton *)sender
{
    int viewId = (int)sender.tag;
    [self sendIntValue:viewId value:viewId];
}

- (void)buttonTouchDown:(UIButton *)sender
{
    ViewManager * viewManager = [self getViewManager:(int)sender.tag];
    [viewManager switchStyle:SelectorActive];
}

- (void)buttonTouchUp:(UIButton *)sender
{
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
    NSLog(@"viewId = %d", viewId);
    BOOL switchState = sender.on;
    int value = switchState ? 1 : 0;
    [self sendIntValue:viewId value:value];
    NSLog(@"Switch is %@", switchState ? @"on" : @"off");
}

- (void)createImageWithId:(int)viewId parentId:(int)parentId filename:(NSString *)filename
{
    FWImageView * imageView;
    if (filename != nil && ![filename hasPrefix:@"http://"] && ![filename hasPrefix:@"https://"]) {
        UIImage *image = [UIImage imageNamed:filename];
        imageView = [[FWImageView alloc] initWithImage:image];
    } else {
        imageView = [[FWImageView alloc] init];
        [imageView addImageUrl:filename width:0 height:0];
    }
    imageView.tag = viewId;
    imageView.contentMode = UIViewContentModeScaleAspectFit;
    imageView.delegate = self;
    imageView.translatesAutoresizingMaskIntoConstraints = false;
    [self addView:imageView withId:viewId];
    [self addToParent:parentId view:imageView];
}

- (void)createScrollLayoutWithId:(int)viewId parentId:(int)parentId
{
#if 1
    FWScrollView * scrollView = [[FWScrollView alloc] init];
#else
    UIScrollView * scrollView = [[UIScrollView alloc] init];
    scrollView.contentSize = CGSizeMake(self.view.frame.size.width, 4000);
#endif
    scrollView.tag = viewId;
    scrollView.contentInset = UIEdgeInsetsMake(64, 0, 0, 0);
    // scrollView.contentOffset = CGPointMake(0, 64);
    scrollView.clipsToBounds = YES;
    scrollView.delegate = self;
    scrollView.translatesAutoresizingMaskIntoConstraints = false;
    [self addView:scrollView withId:viewId];
    [self addToParent:parentId view:scrollView];
}

- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    if (!scrollView.pagingEnabled) { // Do nothing if scrollView has paging enabled
	float diff = scrollView.contentSize.height - (scrollView.frame.size.height + scrollView.contentOffset.y);
        NSLog(@"scrollView scrolled, diff = %f", diff);
        mainThread->sendScrollChangedEvent(scrollView.tag, scrollView.contentOffset.y, (int)diff, scrollView.frame.size.height);
    }
}

- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView
{
    NSLog(@"scrollView did end decelerating");
    if (scrollView.isPagingEnabled) {
        NSInteger page = [self indexForScrollViewPage:scrollView];
        
        if (page >= 0) {
            // set selected item for all tabbars
            for (NSString *key in self.viewsDictionary.allKeys) {
                ViewManager *viewManager = [self.viewsDictionary objectForKey:key];
                if ([viewManager.view isKindOfClass:UITabBar.class]) {
                    UITabBar *tabBar = (UITabBar *)viewManager.view;
                    if (page <= tabBar.items.count) {
                        [tabBar setSelectedItem:tabBar.items[page]];
                        [self sendIntValue:(int)scrollView.tag value:(int)page];
                    }
                }
            }
            
            
            //[self updateTabBarVisibility:(int)page];
        }
    }
}

- (void)createPageLayoutWithId:(int)viewId parentId:(int)parentId
{
    CGFloat frameHeight = self.view.frame.size.height - 20;
  
    UIScrollView * scrollView = [[UIScrollView alloc] init];
    scrollView.tag = viewId;
    scrollView.pagingEnabled = YES;
    scrollView.contentInset = UIEdgeInsetsMake(0, 0, 0, 0);
    scrollView.contentSize = CGSizeMake(0, frameHeight);
    scrollView.layoutMargins = UIEdgeInsetsMake(0, 0, 0, 0);
    scrollView.frame = self.view.frame;
    scrollView.clipsToBounds = YES;
    scrollView.delegate = self;
    scrollView.translatesAutoresizingMaskIntoConstraints = false;
    self.pageView = scrollView;
    [self addView:scrollView withId:viewId];
    [self addToParent:parentId view:scrollView];
}

- (void)showPage:(NSInteger)page
{
    if (page != NSNotFound && self.pageView) {
        CGRect frame = self.pageView.frame;
        frame.origin.x = frame.size.width * page;
        frame.origin.y = 0;
        [self.pageView scrollRectToVisible:frame animated:YES];
    }
}

- (NSInteger)indexForScrollViewPage:(UIScrollView *)pageView
{
    if (pageView.pagingEnabled) {
        CGRect frame = pageView.bounds;
        int page = frame.origin.x / frame.size.width;
        return page;
    }
    return NSNotFound;
}

- (void)createEventLayoutWithId:(int)viewId parentId:(int)parentId
{
    FrameLayoutView *view = [[FrameLayoutView alloc] init];
    view.tag = viewId;
    view.translatesAutoresizingMaskIntoConstraints = false;
    view.layoutMargins = UIEdgeInsetsMake(0, 0, 0, 0);
    [self addView:view withId:viewId];
    [self addToParent:parentId view:view];

    UITapGestureRecognizer *tapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(viewTapped:)];
    tapGestureRecognizer.delegate = self;
    [view addGestureRecognizer:tapGestureRecognizer];
    
    UILongPressGestureRecognizer *longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(viewTouched:)];
    longPressGestureRecognizer.minimumPressDuration = 0;
    longPressGestureRecognizer.delegate = self;
    [view addGestureRecognizer:longPressGestureRecognizer];
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer: (UIGestureRecognizer *)otherGestureRecognizer {
    return YES;
}

- (void)createNavigationBar:(int)viewId parentId:(int)parentId
{
    // Create navigation bar with a button for opening side menu
    UINavigationBar *navBar = [[UINavigationBar alloc] initWithFrame:CGRectMake(0, 20, self.view.frame.size.width, 44)];
    UINavigationItem *navItem;
    if (self.currentTitle != nil) {
      navItem = [[UINavigationItem alloc] initWithTitle:self.currentTitle];
    } else {
      navItem = [[UINavigationItem alloc] init];
    }

    // Add debug event by tapping nav bar 5 times
    UITapGestureRecognizer *debugTapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(navBarTapped5Times:)];
    debugTapGesture.numberOfTapsRequired = 5;
    [navBar addGestureRecognizer:debugTapGesture];
    
#if 0
    UIBarButtonItem *backButton = [[UIBarButtonItem alloc] initWithTitle:@"Back" style:UIBarButtonItemStylePlain target:self action:@selector(backButtonTapped)];
#else
    UIImage *image = [self loadImage:@"icons_hamburger-menu.png"];
    UIBarButtonItem *backButton = [[UIBarButtonItem alloc] initWithImage:image style:UIBarButtonItemStylePlain target:self action:@selector(backButtonTapped)];
#endif
    navItem.leftBarButtonItem = backButton;
    [navBar setItems:@[navItem]];
    navBar.translucent = YES;
    self.navBar = navBar;
    [self.view addSubview:navBar];
    
    UIToolbar *statusBarBackgroundView = [[UIToolbar alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, 20)];
    statusBarBackgroundView.barStyle = self.navBar.barStyle;
    statusBarBackgroundView.translucent = self.navBar.translucent;
    statusBarBackgroundView.autoresizingMask = UIViewAutoresizingFlexibleBottomMargin | UIViewAutoresizingFlexibleWidth;
    
    self.statusBarBackgroundView = statusBarBackgroundView;
    [self.view addSubview:statusBarBackgroundView];
}

- (void)navBarTapped5Times:(UITapGestureRecognizer *)recognizer
{
    mainThread->startDebugMode();
}

- (void)backButtonTapped
{
    if (!mainThread->back()) {
        [self showNavigationViewWithAnimation:YES];
    }
}

#pragma mark - TabBar
- (void)createTabBar:(int)viewId parentId:(int)parentId
{
    UITabBar *tabBar = [[UITabBar alloc] init];
    tabBar.tag = viewId;
    //self.tabBar = tabBar;
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
        }
        [self addView:tabBarItem withId:viewId];
    }
}

- (void)tabBar:(UITabBar *)tabBar didSelectItem:(UITabBarItem *)item
{
    //if ([tabBar isEqual:self.tabBar]) {
    int itemIndex = (int)[self indexForTabBar:tabBar item:item];
    [self showPage:itemIndex];
    [self sendIntValue:(int)tabBar.tag value:(int)item.tag];
    //[self updateTabBarVisibility:itemIndex];
    //}
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
    //if (self.tabBar) {
    for (int i = 0; i < tabBar.items.count; i++) {
        if ([tabBar.items[i] isEqual:item]) {
            return i;
        }
    }
    //}
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
    [self addView:view withId:viewId];
    [self addToParent:parentId view:view];    
}

- (void)createPickerWithId:(int)viewId parentId:(int)parentId
{
    FWPicker * view = [[FWPicker alloc] init];
    view.tag = viewId;
    view.translatesAutoresizingMaskIntoConstraints = false;

    UIImage *image = [self loadImage:@"icons_arrow-down.png"];
    [view setImage:image forState:UIControlStateNormal];

    [self addView:view withId:viewId];
    [self addToParent:parentId view:view];    

    [view addTarget:self action:@selector(showPicker:) forControlEvents:UIControlEventTouchUpInside];
}

- (void)showPicker:(UIButton *)sender {   
    UIView * pickerHolder = [[UIView alloc] init];
    pickerHolder.translatesAutoresizingMaskIntoConstraints = false;
    [self.view addSubview:pickerHolder];

    NSLayoutConstraint *topConstraint0 = [NSLayoutConstraint constraintWithItem:pickerHolder attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:pickerHolder.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
    NSLayoutConstraint *leftConstraint0 = [NSLayoutConstraint constraintWithItem:pickerHolder attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:pickerHolder.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
    NSLayoutConstraint *rightConstraint0 = [NSLayoutConstraint constraintWithItem:pickerHolder attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:pickerHolder.superview attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
    NSLayoutConstraint *bottomConstraint0 = [NSLayoutConstraint constraintWithItem:pickerHolder attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:pickerHolder.superview attribute:NSLayoutAttributeBottom multiplier:1.0f constant:0];
    [pickerHolder.superview addConstraints:@[topConstraint0, leftConstraint0, rightConstraint0, bottomConstraint0]];

    UIView * pickerBackground = [self createBackgroundOverlay:pickerHolder];
    // dialogBackground.tag = viewId;
    pickerBackground.alpha = backgroundOverlayViewAlpha;

    UIPickerView * view = [UIPickerView new];
    view.delegate = self;
    view.dataSource = self;
    view.showsSelectionIndicator = YES;
    view.translatesAutoresizingMaskIntoConstraints = false;
    view.tag = sender.tag;
    view.layer.backgroundColor = UIColor.whiteColor.CGColor;
    [pickerHolder addSubview:view];

    NSLayoutConstraint *topConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeBottom multiplier:0.6f constant:0];
    NSLayoutConstraint *leftConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
    NSLayoutConstraint *rightConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
    NSLayoutConstraint *bottomConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeBottom multiplier:1.0f constant:0];
    [view.superview addConstraints:@[topConstraint, leftConstraint, rightConstraint, bottomConstraint]];

    mainThread->startModal();
} 

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView {
    return 1;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component {
    FWPicker * picker = (FWPicker *)[self viewForId:pickerView.tag];
    return [picker.options count];
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component {
    FWPicker * picker = (FWPicker *)[self viewForId:pickerView.tag];
    return picker.options[row];
}

-(void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
    FWPicker * picker = (FWPicker *)[self viewForId:pickerView.tag];
    [picker setSelection:row];

    [pickerView.superview removeFromSuperview];

    NSLog(@"picker selected %d", row);
    mainThread->endModal(row);
}

- (void)createActionSheetWithId:(int)viewId parentId:(int)parentId title:(NSString *)title
{
    UIAlertController *actionSheet = [UIAlertController alertControllerWithTitle:title message:nil preferredStyle:UIAlertControllerStyleActionSheet];
  
    [self addView:actionSheet withId:viewId];
  
    [actionSheet addAction:[UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:^(UIAlertAction *action) {
        NSLog(@"User clicked button called %@ or tapped elsewhere",action.title);
        mainThread->endModal(0);
    }]];

    [self presentViewController:actionSheet animated:YES completion:nil];
}

- (void)createDialogWithId:(int)viewId parentId:(int)parentId
{
    [self.dialogIds addObject:[NSNumber numberWithInt:viewId]];

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
    
    UIView * dialog = [[UIView alloc] init];
    dialog.layer.backgroundColor = [UIColor whiteColor].CGColor;
    dialog.layer.cornerRadius = 5;
    dialog.layer.shadowColor = [UIColor blackColor].CGColor;
    dialog.layer.shadowOpacity = 1.0;
    dialog.layer.shadowRadius = 7.5;
    dialog.layer.shadowOffset = CGSizeMake(1, 4);
    dialog.clipsToBounds = YES;
    dialog.translatesAutoresizingMaskIntoConstraints = false;
    dialog.tag = viewId;
    [dialogHolder addSubview:dialog];

    NSLayoutConstraint *topConstraint = [NSLayoutConstraint constraintWithItem:dialog attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:dialog.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:100];
    NSLayoutConstraint *leftConstraint = [NSLayoutConstraint constraintWithItem:dialog attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:dialog.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:50];
    NSLayoutConstraint *rightConstraint = [NSLayoutConstraint constraintWithItem:dialog attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:dialog.superview attribute:NSLayoutAttributeRight multiplier:1.0f constant:-50];
    NSLayoutConstraint *bottomConstraint = [NSLayoutConstraint constraintWithItem:dialog attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:dialog.superview attribute:NSLayoutAttributeBottom multiplier:1.0f constant:-50];
    [dialog.superview addConstraints:@[topConstraint, leftConstraint, rightConstraint, bottomConstraint]];

    ViewManager * viewManager = [self getViewManager:viewId];
    viewManager.containerView = dialog;

    [UIView animateWithDuration:animationDuration/2 animations:^{
        dialogHolder.alpha = 1.0;
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
    if (self.webView == nil) {
        WKWebViewConfiguration *configuration = [[WKWebViewConfiguration alloc] init];
        CGRect frame = CGRectMake(0, 20, self.view.bounds.size.width, self.view.bounds.size.height);
        self.webView = [[WKWebView alloc] initWithFrame:frame configuration:configuration];
	    self.webView.translatesAutoresizingMaskIntoConstraints = false;
        [self.view addSubview:self.webView];
        
        NSLayoutConstraint *topConstraint = [NSLayoutConstraint constraintWithItem:self.webView attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:self.webView.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:20];
        NSLayoutConstraint *leftConstraint = [NSLayoutConstraint constraintWithItem:self.webView attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:self.webView.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
        NSLayoutConstraint *rightConstraint = [NSLayoutConstraint constraintWithItem:self.webView attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:self.webView.superview attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
        NSLayoutConstraint *bottomConstraint = [NSLayoutConstraint constraintWithItem:self.webView attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:self.webView.superview attribute:NSLayoutAttributeBottom multiplier:1.0f constant:0];
        [self.webView.superview addConstraints:@[topConstraint, leftConstraint, rightConstraint, bottomConstraint]];

        // add close button (x) to the top left corner of the view
        UIButton *closeButton = [[UIButton alloc] initWithFrame:CGRectMake(10.0, 20.0, 40.0, 40.0)];
        [closeButton setTitle:@"X" forState:UIControlStateNormal];
        [closeButton addTarget:self action:@selector(webViewCloseButtonPushed:) forControlEvents:UIControlEventTouchUpInside];
        [closeButton setTitleColor:UIColor.whiteColor forState:UIControlStateNormal];
        [closeButton setBackgroundColor:UIColor.blackColor];
        closeButton.layer.cornerRadius = closeButton.frame.size.width / 2;
        closeButton.layer.borderWidth = 2.0;
        closeButton.layer.borderColor = UIColor.whiteColor.CGColor;
        [self.webView addSubview:closeButton];
    }
    // self.webView.layer.zPosition = 1000000.0f;
    [self.view bringSubviewToFront:self.webView];
    
    NSURL *webURL = [NSURL URLWithString:url];
    NSURLRequest *request = [NSURLRequest requestWithURL:webURL];
    [self.webView loadRequest:request];
    mainThread->startModal();
}

- (void)webViewCloseButtonPushed:(UIButton *)button
{
    NSLog(@"closeButton pushed");
    [self.webView removeFromSuperview];
    self.webView = nil;
    mainThread->endModal(0);
}

- (void)sendTimerEvent:(NSTimer *)timer
{
    mainThread->sendTimerEvent(1);
}

- (void)viewTapped:(UITapGestureRecognizer *)gesture
{
    int viewId = (int)gesture.view.tag;
    NSLog(@"VIEW TAPPED: viewId = %d", viewId);
    [self sendIntValue:viewId value:1];
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
  
    if ([parentView isKindOfClass:LinearLayoutView.class]) {
        // view.autoresizingMask = 0;
        LinearLayoutView * layout = (LinearLayoutView *)parentView;
        LayoutParams * item = [LayoutParams layoutItemForView:view];
        // item.padding = LinearLayoutMakePadding(5.0, 10.0, 5.0, 10.0);
        // item.horizontalAlignment = LinearLayoutItemHorizontalAlignmentCenter;	
        item.level = parentViewManager.level + 1;
        [layout addItem:item];
        viewManager.layoutParams = item;
    } else if ([parentView isKindOfClass:FrameLayoutView.class]) {
        // view.autoresizingMask = 0;
        FrameLayoutView * layout = (FrameLayoutView *)parentView;
        LayoutParams * item = [LayoutParams layoutItemForView:view];
        item.level = parentViewManager.level + 1;
        [layout addItem:item];
        viewManager.layoutParams = item;
    } else if ([parentView isKindOfClass:UIScrollView.class]) {
        NSUInteger pos = [[parentView subviews] count];
        [parentView addSubview:view];
        UIScrollView * scrollView = (UIScrollView *)parentView;
        int pageWidth = self.view.frame.size.width;
        if (scrollView.pagingEnabled) {
            scrollView.contentSize = CGSizeMake(scrollView.contentSize.width + pageWidth, scrollView.contentSize.height);
            
            NSLayoutConstraint *leftConstraint;
            NSLayoutConstraint *topConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
            NSLayoutConstraint *widthConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeWidth multiplier:1.0f constant:0];
            NSLayoutConstraint *heightConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeHeight multiplier:1.0f constant:0];

            if (pos == 0) {
                leftConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
            } else {
#if 0
                leftConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:[[parentView subviews] lastObject] attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
#else
                leftConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:(pos * pageWidth)];
#endif
            }

            topConstraint.priority = 999 - viewManager.level;
            leftConstraint.priority = 999 - viewManager.level;
            widthConstraint.priority = 999 - viewManager.level;
            heightConstraint.priority = 999 - viewManager.level;
            [view.superview addConstraints:@[topConstraint, leftConstraint, widthConstraint, heightConstraint]];
        } else {
	    FWScrollView * fwScrollView = (FWScrollView*)scrollView;
            fwScrollView.topConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
            fwScrollView.leftConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
            fwScrollView.widthConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeWidth multiplier:1.0f constant:0];
            fwScrollView.heightConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:0.0f constant:4000];

            fwScrollView.topConstraint.priority = 999 - viewManager.level;
            fwScrollView.leftConstraint.priority = 999 - viewManager.level;
            fwScrollView.widthConstraint.priority = 999 - viewManager.level;
            fwScrollView.heightConstraint.priority = 999 - viewManager.level;

            [view.superview addConstraints:@[fwScrollView.topConstraint, fwScrollView.leftConstraint, fwScrollView.widthConstraint, fwScrollView.heightConstraint]];
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
            NSLog(@"removing dialog");
            [self sendIntValue:dialogId value:0];
            [self.dialogIds removeObjectAtIndex:i];
            break;
        }
    }

    NSString * key = [NSString stringWithFormat:@"%d", viewId];
    ViewManager * viewManager = [self.viewsDictionary objectForKey:key];
    if (viewManager != nil) {
        if ([viewManager.view isKindOfClass:UIView.class]) {
            UIView * view = viewManager.view;
            UIView * parentView = view.superview;

            if ([parentView isKindOfClass:LinearLayoutView.class]) {
                LinearLayoutView * layout = (LinearLayoutView*)parentView;
                [layout removeItem:viewManager.layoutParams];
            } else if ([parentView isKindOfClass:LinearLayoutView.class]) {
                FrameLayoutView * layout = (FrameLayoutView*)parentView;
                [layout removeItem:viewManager.layoutParams];
            } else {
                UIView * view = (UIView*)viewManager.view;
                [view removeFromSuperview];
            }
  
            if (view == self.sideMenuView) {
                [self hideBackgroundOverlayViewWithAnimation:YES];
            }
        }
        [self.viewsDictionary removeObjectForKey:key];
    }
}

- (void)reorderChildWithId:(int)viewId parentId:(int)parentId newPosition:(int)position
{
    UIView *parentView = [self viewForId:parentId];
    UIView *childView = [self viewForId:viewId];
    if (parentView && childView) {
	if ([parentView isKindOfClass:[LinearLayoutView class]]) {
	    LinearLayoutView * layout = (LinearLayoutView *)parentView;

	    ViewManager * viewManager = [self getViewManager:viewId];

	    [layout moveItem:viewManager.layoutParams toIndex:position];
	} else if ([parentView isKindOfClass:[FrameLayoutView class]]) {

        } else {
            [parentView insertSubview:childView atIndex:position];
        }
    }
}

- (void)addOption:(int)viewId optionId:(int)optionId title:(NSString *)title
{
    id view = [self viewForId:viewId];
    if ([view isKindOfClass:UIAlertController.class]) {
        UIAlertController * alertController = (UIAlertController*)view;
        [alertController addAction:[UIAlertAction actionWithTitle:title style:(optionId == 0 ? UIAlertActionStyleCancel : UIAlertActionStyleDefault) handler:^(UIAlertAction *action) {
            mainThread->endModal(optionId);
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

- (void)setImageFromThread:(int)viewId data:(UIImage *)data
{
    ImageWrapper * iw = [[ImageWrapper alloc] init];
    [self performSelectorOnMainThread:@selector(handleImage:) withObject:iw waitUntilDone:NO];
}

- (void)handleImage:(ImageWrapper*)iw
{
    ViewManager * viewManager = [self getViewManager:iw.targetElementId];
    if (viewManager) {
        [viewManager setImage:iw.image];
    }
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

- (void)sendEventToMainThread:(EventWrapper*) event
{
    [self performSelectorOnMainThread:@selector(sendEvent:) withObject:event waitUntilDone:NO];
}

- (void)sendEvent:(EventWrapper*)event
{
    mainThread->handleEventFromThread((int)event.targetElementId, (Event *)event.eventPtr);
}

- (void)setTitle:(NSString*)title
{
    self.currentTitle = title;

    if (self.navBar != nil) {
        self.navBar.items[0].title = title;
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
    mainThread->sendImageRequest((int)imageView.tag, size.width, size.height, s);
}

@end
