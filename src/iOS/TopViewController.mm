#import "TopViewController.h"

#import <WebKit/WebKit.h>

#import "PaddedLabel.h"

@interface TopViewController () <WKUIDelegate, WKNavigationDelegate, UIBarPositioningDelegate, UINavigationBarDelegate>
@property (nonatomic, strong) NSURL *currentURL;
@property (nonatomic, strong) WKWebView *webView;
@property (nonatomic, assign) NSInteger keyboardHeight;
@property (nonatomic, assign) PaddedLabel * currentToast;
@end

@implementation TopViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    self.keyboardHeight = 0;
    self.currentToast = nil;
}

- (void)viewWillTransitionToSize: (CGSize)size withTransitionCoordinator:(id)coordinator
{

}

- (void)createWebBrowserWithUrl:(NSString *)url
{
    NSLog(@"WebView opened");
    self.currentURL = [NSURL URLWithString:url];
    
    if (self.webView == nil) {
        CGFloat statusBarHeight = [UIApplication sharedApplication].statusBarFrame.size.height;
        CGFloat navBarHeight = 44.0;
	
        WKWebViewConfiguration *configuration = [[WKWebViewConfiguration alloc] init];
        CGRect frame = CGRectMake(0, 0, self.view.bounds.size.width, self.view.bounds.size.height);
        self.webView = [[WKWebView alloc] initWithFrame:frame configuration:configuration];
	self.webView.translatesAutoresizingMaskIntoConstraints = false;
        self.webView.UIDelegate = self;
        self.webView.navigationDelegate = self;
        self.webView.scrollView.contentInset = UIEdgeInsetsMake(statusBarHeight + navBarHeight, 0, 0, 0);
        [self.view addSubview:self.webView];

        NSLayoutConstraint *topConstraint = [NSLayoutConstraint constraintWithItem:self.webView attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:self.webView.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:0];
        NSLayoutConstraint *leftConstraint = [NSLayoutConstraint constraintWithItem:self.webView attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:self.webView.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
        NSLayoutConstraint *rightConstraint = [NSLayoutConstraint constraintWithItem:self.webView attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:self.webView.superview attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
        NSLayoutConstraint *bottomConstraint = [NSLayoutConstraint constraintWithItem:self.webView attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:self.webView.superview attribute:NSLayoutAttributeBottom multiplier:1.0f constant:0];
        [self.webView.superview addConstraints:@[topConstraint, leftConstraint, rightConstraint, bottomConstraint]];

        UINavigationBar *navBar = [[UINavigationBar alloc] init];
        navBar.tintColor = [UIColor blackColor];
        navBar.barTintColor = UIColor.whiteColor;
        navBar.translucent = YES;
        navBar.barStyle = UIBarStyleDefault;
        navBar.delegate = self;
        navBar.translatesAutoresizingMaskIntoConstraints = false;        

        CGFloat width = self.view.frame.size.width * 0.6; // just some width related to width of the view
        
        UINavigationItem *navItem = [[UINavigationItem alloc] initWithTitle:@""];
        UILabel *titleLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, width, 22)];
        titleLabel.text = url;
        titleLabel.adjustsFontSizeToFitWidth = YES;
        [titleLabel.widthAnchor constraintEqualToConstant:width].active = YES;
        titleLabel.minimumScaleFactor = 0.6;
        titleLabel.textAlignment = NSTextAlignmentCenter;
        navItem.titleView = titleLabel;

        UIImage *backImage = [self.imageCache loadIcon:@"icons_arrow-left-red.png"];
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

        [self.webView addSubview:navBar];

	NSLayoutConstraint *topConstraint2 = [NSLayoutConstraint constraintWithItem:navBar attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:navBar.superview attribute:NSLayoutAttributeTop multiplier:1.0f constant:statusBarHeight];
	NSLayoutConstraint *leftConstraint2 = [NSLayoutConstraint constraintWithItem:navBar attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:navBar.superview attribute:NSLayoutAttributeLeft multiplier:1.0f constant:0];
	NSLayoutConstraint *rightConstraint2 = [NSLayoutConstraint constraintWithItem:navBar attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:navBar.superview attribute:NSLayoutAttributeRight multiplier:1.0f constant:0];
	[navBar.superview addConstraints:@[topConstraint2, leftConstraint2, rightConstraint2]];
	// self.statusBarTopConstraint = topConstraint2;        
    }
    [self.webView.superview bringSubviewToFront:self.webView];
    
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

- (void)bringWebviewToFront {
    if (self.webView) {
	[self.webView.superview bringSubviewToFront:self.webView];
    }
}

- (void)showTabBar:(UITabBar *)tabBar
{
    if (self.visibleTabBar != nil) {
	self.visibleTabBar.hidden = YES;
    }
    self.visibleTabBar = tabBar;
    self.visibleTabBar.hidden = NO;
}

- (void)showNavBar:(UINavigationBar *)navBar
{
    if (self.visibleNavBar != nil) {
	self.visibleNavBar.hidden = YES;
    }
    self.visibleNavBar = navBar;
    self.visibleNavBar.hidden = NO;
}

- (void)showSearchBar:(UISearchBar *)searchBar
{
    if (self.visibleSearchBar != nil) {
	[self.visibleSearchBar resignFirstResponder];
	self.visibleSearchBar.hidden = YES;
    }
    self.visibleSearchBar = searchBar;
    self.visibleSearchBar.hidden = NO;
}

- (void)showKeyboard:(BOOL)visible height:(NSInteger)height {
    self.additionalSafeAreaInsets = UIEdgeInsetsMake(0, 0, height, 0);
    self.keyboardHeight = height;
}

- (BOOL)isTabBarVisible {
    return self.visibleTabBar != nil;
}

- (BOOL)isNavBarVisible {
    return self.visibleNavBar != nil;
}

- (BOOL)isSearchBarVisible {
    return self.visibleSearchBar != nil;
}

- (BOOL)isKeyboardVisible {
    return self.keyboardHeight > 0;
}

- (NSInteger)getKeyboardHeight {
    return self.keyboardHeight;
}

- (void) showToast:(NSString *)text duration:(NSInteger)duration {
    if (self.currentToast == nil) {
	PaddedLabel* label = [[PaddedLabel alloc] init];
	label.backgroundColor = [UIColor colorWithRed:0.87 green:0.16 blue:0.18 alpha:1.0];
	label.textColor = [UIColor whiteColor];
	label.layer.cornerRadius = 3;
        label.edgeInsets = UIEdgeInsetsMake(5, 5, 5, 5);
	label.numberOfLines = 0; // as many lines as needed
	label.lineBreakMode = NSLineBreakByWordWrapping;
	label.preferredMaxLayoutWidth = self.view.frame.size.width;
        label.translatesAutoresizingMaskIntoConstraints = YES; // temporary
	[self.view addSubview:label];

        self.currentToast = label;
    } else {
        self.currentToast.hidden = NO;
    }

    self.currentToast.alpha = 0.99f;
    self.currentToast.text = text;
    [self.currentToast sizeToFit];
    self.currentToast.center = self.view.center;

    [UIView animateWithDuration:0.3 delay:1 options:0 animations:^{
	self.currentToast.alpha = 0;
    } completion:^(BOOL finished) {
	self.currentToast.hidden = YES;
    }];
}

- (UIBarPosition)positionForBar:(id<UIBarPositioning>)bar {
    return UIBarPositionTopAttached;
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
