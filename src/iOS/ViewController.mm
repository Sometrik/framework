#import "ViewController.h"

#include <FWApplication.h>
#include "iOSMainThread.h"

#import "ImageWrapper.h"
#import "InAppPurchaseManager.h"
#import "FWImageView.h"

#include <memory>

using namespace std;

// Pointer to main thread
std::shared_ptr<iOSMainThread> mainThread;

// Declare C++ function
extern FWApplication * applicationMain();

@interface ViewController () <UIScrollViewDelegate, UITabBarDelegate, InAppPurchaseManagerDelegate>
@property (nonatomic, strong) NSMutableDictionary *viewsDictionary;
@property (nonatomic, strong) UIView *sideMenuView;
@property (nonatomic, strong) UIView *backgroundOverlayView;
@property (nonatomic, strong) UITabBar *tabBar;
@property (nonatomic, strong) UINavigationBar *navBar;
@property (nonatomic, strong) UIToolbar *statusBarBackgroundView;
@property (nonatomic, strong) UIScrollView *pageView;
@property (nonatomic) int activeDialogId;
@property (nonatomic) int activeViewId;
@property (nonatomic, assign) BOOL sideMenuPanned;
@property (nonatomic, strong) InAppPurchaseManager *inAppPurchaseManager;
//@property (nonatomic, strong) NSSet *tabBarHiddenInThesePages;
@end

static const NSTimeInterval animationDuration = 0.4;
static const CGFloat backgroundOverlayViewAlpha = 0.5;
static const CGFloat sideMenuOpenSpaceWidth = 100.0;

@implementation ViewController

- (void)viewDidLoad {
  [super viewDidLoad];
  // Do any additional setup after loading the view, typically from a nib.
  //self.tabBarHiddenInThesePages = [[NSSet alloc] initWithObjects:[NSNumber numberWithInt:0], nil];
  self.activeViewId = 0;
  self.activeDialogId = 0;
    
  [self createBackgroundOverlay];
  
    self.inAppPurchaseManager = [InAppPurchaseManager sharedInstance];
  //self.view.layoutMargins = UIEdgeInsetsMake(64.0, 0, 50.0, 0);

  // Creating the C++ app
  std::shared_ptr<FWApplication> application(applicationMain());
  
  mainThread = make_shared<iOSMainThread>(application, application);
  mainThread->viewController = self;
//    mainThread->setActualDisplayWidth(screenWidth);
//    mainThread->setActualDisplayHeight(screenHeight);
//    mainThread->setDisplayScale(displayScale);
  
  application->initialize(mainThread.get());
  application->initializeChildren();
  application->load();
}

- (void)viewWillTransitionToSize: (CGSize)size withTransitionCoordinator:(id)coordinator
{
  [super viewWillTransitionToSize: size withTransitionCoordinator:coordinator];
  [coordinator animateAlongsideTransition: nil completion: ^(id _Nonnull context) {

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
            [scrollView.panGestureRecognizer requireGestureRecognizerToFail:panEdgeGestureRecognizer];
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

- (void)edgeSwiped:(UIScreenEdgePanGestureRecognizer *)recognizer
{
    if (self.sideMenuView) {
        self.sideMenuView.hidden = NO;
        self.backgroundOverlayView.hidden = NO;
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
    NSLog(@"location = %@", NSStringFromCGPoint(touchLocation));
}

- (void)createBackgroundOverlay
{
    // create backgroundoverlay view that's behind sidePanel and dialog and if clicked closes the panel
    self.backgroundOverlayView = [[UIView alloc] initWithFrame:self.view.bounds];
    self.backgroundOverlayView.hidden = YES;
    self.backgroundOverlayView.backgroundColor = UIColor.blackColor;
    [self.view addSubview:self.backgroundOverlayView];
    UITapGestureRecognizer *tapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(backgroundOverlayViewTapped:)];
    [self.backgroundOverlayView addGestureRecognizer:tapGestureRecognizer];
}

- (void)backgroundOverlayViewTapped:(UITapGestureRecognizer *)gesture
{
    if (!self.sideMenuView.isHidden) {
        [self hideNavigationViewWithAnimation:YES];
    }
    if (self.activeDialogId) {
        [self sendIntValue:self.activeDialogId value:0];
        self.activeDialogId = 0;
    }
    /*
     [UIView animateWithDuration:animationDuration animations:^{
     self.backgroundOverlayView.alpha = 0.0;
     } completion:^(BOOL finished) {
     if (finished) {
     self.backgroundOverlayView.hidden = YES;
     }
     }];
     */
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
        if (view) {
            view.hidden = viewHidden;
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
    CGFloat tabBarHeight = self.tabBar == nil ? 0.0 : 44.0;
    CGFloat topBarsHeight = self.navBar == nil ? 0.0 : 64.0;
    UIView *view = [[UIView alloc] initWithFrame:CGRectMake(0, topBarsHeight, self.view.bounds.size.width, self.view.bounds.size.height-topBarsHeight-tabBarHeight)];
    view.tag = viewId;
    [self.view addSubview:view];
    [self addView:view withId:viewId];
    
    if (self.activeViewId == 0) {
        self.activeViewId = viewId;
    } else {
        view.hidden = YES;
    }
    
    //UIView *parentView = [self.viewsDictionary objectForKey:[NSString stringWithFormat:@"%d", parentId]];
    //[parentView addSubview:view];
}

- (void)createLinearLayoutWithId:(int)viewId parentId:(int)parentId direction:(int)direction
{
    UIStackView *stackView = [[UIStackView alloc] init];
    if (direction == 1) {
        stackView.axis = UILayoutConstraintAxisVertical;
    } else {
        stackView.axis = UILayoutConstraintAxisHorizontal;
    }
    stackView.distribution = UIStackViewDistributionFill;
    // stackView.distribution = UIStackViewDistributionFillProportionally;
    stackView.alignment = UIStackViewAlignmentFill;
    stackView.translatesAutoresizingMaskIntoConstraints = false;
    stackView.frame = self.view.frame;
    
    [self addView:stackView withId:viewId];
    [self addToParent:parentId view:stackView];
}

- (void)createFrameLayoutWithId:(int)viewId parentId:(int)parentId
{
    UIView *view = [[UIView alloc] init];
    view.tag = viewId;
    [self addView:view withId:viewId];
    [self addToParent:parentId view:view];
}

- (void)createTextWithId:(int)viewId parentId:(int)parentId value:(NSString*)value
{
    UILabel *label = [[UILabel alloc] init];
    label.tag = viewId;
    label.text = value;
    label.numberOfLines = 0; // as many lines as needed
    label.lineBreakMode = NSLineBreakByWordWrapping;
    [self addView:label withId:viewId];
    [self addToParent:parentId view:label];
}

- (void)createButtonWithId:(int)viewId parentId:(int)parentId caption:(NSString *)caption
{
    UIButton *button = [[UIButton alloc] init];
    button.tag = viewId;
    [button setTitle:caption forState:UIControlStateNormal];
    [button addTarget:self action:@selector(buttonPushed:) forControlEvents:UIControlEventTouchUpInside];
    [self addView:button withId:viewId];
    [self addToParent:parentId view:button];
}

- (void)buttonPushed:(UIButton *)sender
{
    int viewId = (int)sender.tag;
    NSLog(@"viewId = %d", viewId);
    NSLog(@"buttonTitle = %@", sender.titleLabel.text);
    [self sendIntValue:viewId value:viewId];
}

- (void)createSwitchWithId:(int)viewId parentId:(int)parentId
{
    UISwitch *sw = [[UISwitch alloc] init];
    sw.tag = viewId;
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
    UIImage *image = [UIImage imageNamed:filename];
    FWImageView *imageView = [[FWImageView alloc] initWithImage:image];
    imageView.tag = viewId;
    imageView.contentMode = UIViewContentModeScaleAspectFit;
    [self addView:imageView withId:viewId];
    [self addToParent:parentId view:imageView];
}

- (void)createScrollLayoutWithId:(int)viewId parentId:(int)parentId
{
    UIScrollView * scrollView = [[UIScrollView alloc] init];
    scrollView.tag = viewId;
    scrollView.contentSize = CGSizeMake(self.view.bounds.size.width, 4000);
    scrollView.frame = self.view.frame;
    scrollView.clipsToBounds = YES;
    scrollView.delegate = self;
    scrollView.translatesAutoresizingMaskIntoConstraints = false;
    [self addView:scrollView withId:viewId];
    [self addToParent:parentId view:scrollView];
}

- (void)calculateScrollViewContentSize:(int)scrollViewId
{
    UIView *possibleScrollView = [self viewForId:scrollViewId];
    if (possibleScrollView && [possibleScrollView isKindOfClass:[UIScrollView class]]) {
        UIScrollView *scrollView = (UIScrollView *)possibleScrollView;
        CGRect contentRect = CGRectZero;
        for (UIView *view in scrollView.subviews) {
            contentRect = CGRectUnion(contentRect, view.frame);
        }
        scrollView.contentSize = CGSizeMake(scrollView.contentSize.width, contentRect.size.height);
    }
}

- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    if (!scrollView.pagingEnabled) { // Do nothing if scrollView has paging enabled
        NSLog(@"scrollView scrolled");
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
    CGFloat tabBarHeight = self.tabBar == nil ? 0.0 : 44.0;
    CGFloat topBarsHeight = self.navBar == nil ? 0.0 : 64.0;
    CGFloat frameHeight = self.view.bounds.size.height-topBarsHeight-tabBarHeight;
  
    UIScrollView * scrollView = [[UIScrollView alloc] init];
    scrollView.tag = viewId;
    scrollView.pagingEnabled = YES;
    scrollView.contentSize = CGSizeMake(0, frameHeight);
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
    UIView *view = [[UIView alloc] init];
    view.tag = viewId;
    UITapGestureRecognizer *tapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(viewTapped:)];
    [view addGestureRecognizer:tapGestureRecognizer];
    [self addView:view withId:viewId];
    [self addToParent:parentId view:view];
}

- (void)createNavigationBar:(int)viewId parentId:(int)parentId
{
    // Create navigation bar with a button for opening side menu
    UINavigationBar *navBar = [[UINavigationBar alloc] initWithFrame:CGRectMake(0, 20, self.view.frame.size.width, 44)];
    UINavigationItem *navItem = [[UINavigationItem alloc] initWithTitle:@"title"];
    
    // Add debug event by tapping nav bar 5 times
    UITapGestureRecognizer *debugTapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(navBarTapped5Times:)];
    debugTapGesture.numberOfTapsRequired = 5;
    [navBar addGestureRecognizer:debugTapGesture];
    
    UIBarButtonItem *menuButton = [[UIBarButtonItem alloc] initWithTitle:@"Menu" style:UIBarButtonItemStylePlain target:self action:@selector(menuButtonTapped)];
    navItem.leftBarButtonItem = menuButton;
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
    NSLog(@"tap-tap-tap-tap-tap");
    
}

- (void)menuButtonTapped
{
    [self showNavigationViewWithAnimation:YES];
}

#pragma mark - TabBar
- (void)createTabBar:(int)viewId parentId:(int)parentId
{
    CGFloat tabBarHeight = 44.0;
    UIView *parentView = [self viewForId:parentId];
    UITabBar *tabBar = [[UITabBar alloc] initWithFrame:CGRectMake(0, parentView.frame.size.height-tabBarHeight, parentView.frame.size.width, tabBarHeight)];
    tabBar.tag = viewId;
    // tabBar.contentMode = UIViewContentModeBottom;
    // [tabBar setAutoresizingMask:UIViewAutoresizingFlexibleTopMargin];
    
    //self.tabBar = tabBar;
    tabBar.delegate = self;
    [self addView:tabBar withId:viewId];
    //[tabBar.bottomAnchor constraintEqualToAnchor:parentView.bottomAnchor];
    [self addToParent:parentId view:tabBar];
    //[self.view addSubview:self.tabBar];
    tabBar.items = [[NSArray alloc] init];
    // Put tabbar to the bottom of the view
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
}

- (void)showNavigationViewWithAnimation:(BOOL)animate
{
    if (self.sideMenuView.isHidden || self.sideMenuPanned) {
        [self sendIntValue:self.sideMenuView.tag value:1];
        self.backgroundOverlayView.hidden = NO;
        self.backgroundOverlayView.alpha = backgroundOverlayViewAlpha * CGRectGetMaxX(self.sideMenuView.frame) / (CGRectGetWidth(self.view.frame) - sideMenuOpenSpaceWidth);
        if (self.sideMenuView) {
            if (animate) {
                [UIView animateWithDuration:animationDuration animations:^{
                    self.sideMenuView.transform = CGAffineTransformTranslate(self.sideMenuView.transform, CGRectGetWidth(self.sideMenuView.frame)-CGRectGetMaxX(self.sideMenuView.frame), 0.0);
                    self.backgroundOverlayView.alpha = backgroundOverlayViewAlpha * CGRectGetMaxX(self.sideMenuView.frame) / (CGRectGetWidth(self.view.frame) - sideMenuOpenSpaceWidth);
                }];
            } else {
                self.sideMenuView.transform = CGAffineTransformTranslate(self.sideMenuView.transform, CGRectGetWidth(self.sideMenuView.frame)-CGRectGetMaxX(self.sideMenuView.frame), 0.0);
                self.backgroundOverlayView.alpha = backgroundOverlayViewAlpha * CGRectGetMaxX(self.sideMenuView.frame) / (CGRectGetWidth(self.view.frame) - sideMenuOpenSpaceWidth);
            }
        }
        self.sideMenuView.hidden = NO;
    }
}

- (void)hideNavigationViewWithAnimation:(BOOL)animate
{
    if (self.sideMenuView) {
        [self sendIntValue:self.sideMenuView.tag value:0];
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

#pragma mark -
- (void)createActivityIndicatorWithId:(int)viewId parentId:(int)parentId
{
    UIActivityIndicatorView * view = [[UIActivityIndicatorView alloc] init];
    view.tag = viewId;
    [self addView:view withId:viewId];
    [self addToParent:parentId view:view];
    [view startAnimating];
}

- (void)createDialogWithId:(int)viewId parentId:(int)parentId
{
    self.activeDialogId = viewId;
    
    UIView * dialog = [[UIView alloc] init];
    dialog.tag = viewId;
    dialog.layer.backgroundColor = [UIColor grayColor].CGColor;
    dialog.layer.borderColor = [UIColor blackColor].CGColor;
    dialog.layer.borderWidth = 1;
    dialog.layer.cornerRadius = 10;
    dialog.layer.shadowColor = [UIColor blackColor].CGColor;
    dialog.layer.shadowOpacity = 1.0;
    dialog.layer.shadowRadius = 7.5;
    dialog.layer.shadowOffset = CGSizeMake(1, 4);
    dialog.clipsToBounds = NO;
    
    CGSize s = self.view.bounds.size;
    
    dialog.frame = CGRectMake(50, 50, s.width - 100, s.height - 100);
    // dialog.center = CGPointMake(s.width / 2, (s.height / 2) - 65);
    
    [self addView:dialog withId:viewId];
    [self.view addSubview:dialog];
    
    dialog.alpha = 0.0;
    self.backgroundOverlayView.alpha = 0.0;
    self.backgroundOverlayView.hidden = NO;
    [UIView animateWithDuration:animationDuration/2 animations:^{
        dialog.alpha = 1.0;
        self.backgroundOverlayView.alpha = backgroundOverlayViewAlpha;
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

- (void)sendTimerEvent:(NSTimer *)timer
{
    mainThread->sendTimerEvent(1);
}

- (void)viewTapped:(UITapGestureRecognizer *)gesture
{
    int viewId = (int)gesture.view.tag;
    NSLog(@"viewId = %d", viewId);
    [self sendIntValue:viewId value:viewId];
}

- (void)addToParent:(int)parentId view:(UIView*)view
{
    UIView *parentView = [self viewForId:parentId];
    if (parentView == nil) {
        NSLog(@"Element %d missing in addToParent", parentId);
        return;
    }
    
    BOOL add_basic_constraints = NO;
    
    if ([parentView isKindOfClass:UIStackView.class]) {
        UIStackView *stackView = (UIStackView *)parentView;
        [stackView addArrangedSubview:view];
    } else if ([parentView isKindOfClass:UIScrollView.class]) {
        NSUInteger pos = [[parentView subviews] count];
        [parentView addSubview:view];
        UIScrollView * scrollView = (UIScrollView *)parentView;
        if (scrollView.pagingEnabled) {
            int pageWidth = self.view.bounds.size.width;
            // int pageHeight = self.view.bounds.size.height;
            // view.frame = CGRectMake(pos * pageWidth, 0, pageWidth, pageHeight);
            scrollView.contentSize = CGSizeMake(scrollView.contentSize.width + pageWidth, scrollView.contentSize.height);
            
            NSLayoutConstraint *widthConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeWidth multiplier:1 constant:0];
            //NSLayoutConstraint *heightConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeHeight multiplier:1 constant:0];
            NSLayoutConstraint *topConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
            NSLayoutConstraint *leftConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:pos * pageWidth];
            [view.superview addConstraints:@[widthConstraint, topConstraint, leftConstraint]];
        } else {
            
            view.frame = parentView.frame;
            add_basic_constraints = YES;
        }
    } else {
        //view.frame = parentView.frame;
        
        [parentView addSubview:view];
        if (![view isKindOfClass:UITabBar.class] && ![view isKindOfClass:UINavigationBar.class]) {
            add_basic_constraints = YES;
        }
    }
    
    if (add_basic_constraints) {
        ViewManager * viewManager = [self getViewManager:view.tag];
        viewManager.constraintsSet = YES;
        
        NSLayoutConstraint *widthConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeWidth multiplier:1 constant:0];
        NSLayoutConstraint *heightConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeHeight multiplier:1 constant:0];
        NSLayoutConstraint *topConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
        NSLayoutConstraint *leftConstraint = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:view.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
        [view.superview addConstraints:@[widthConstraint, heightConstraint, topConstraint, leftConstraint]];
    }
}

- (void)removeView:(int)viewId
{
    if (self.activeDialogId == viewId) self.activeDialogId = 0;
    
    NSString * key = [NSString stringWithFormat:@"%d", viewId];
    ViewManager * viewManager = [self.viewsDictionary objectForKey:key];
    if (viewManager != nil) {
        if ([viewManager.view isKindOfClass:UIView.class]) {
            UIView * view = (UIView*)viewManager.view;
            [view removeFromSuperview];
        } else {
            
        }
        [self.viewsDictionary removeObjectForKey:key];
        
        // just to take backgroundOverlayView off the screen as well
        [self hideBackgroundOverlayViewWithAnimation:YES];
    }
}

- (void)reorderChildWithId:(int)viewId parentId:(int)parentId newPosition:(int)position
{
    UIView *parentView = [self viewForId:parentId];
    UIView *childView = [self viewForId:viewId];
    if (parentView && childView) {
        if ([parentView isKindOfClass:[UIStackView class]]) {
            UIStackView *stackView = (UIStackView *)parentView;
            [childView removeFromSuperview];
            [stackView insertArrangedSubview:childView atIndex:position];
            [stackView setNeedsLayout];
            [stackView layoutIfNeeded];
        } else {
            [parentView insertSubview:childView atIndex:position];
        }
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
    mainThread->handleEventFromThread(event.targetElementId, (Event *)event.eventPtr);
}

- (void)setTitle:(NSString*)title
{
    if (self.navBar != nil) {
        self.navBar.items[0].title = title;
    }
}

#pragma mark - In-App Purchase stuff

// delegate method
- (void)inAppPurchaseManagerDidReceiveProducts:(InAppPurchaseManager *)manager {
    // show in-app purchase UI
    // [self showInAppPurchaseView];
}

@end
