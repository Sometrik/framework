#import "TopViewController.h"

#import <WebKit/WebKit.h>

@interface TopViewController () <WKUIDelegate, WKNavigationDelegate>
@property (nonatomic, strong) NSURL *currentURL;
@property (nonatomic, strong) WKWebView *webView;
@end

@implementation TopViewController

- (void)viewDidLoad {
    [super viewDidLoad];
}

- (void)viewWillTransitionToSize: (CGSize)size withTransitionCoordinator:(id)coordinator
{
    if (self.webView != nil) {
        for (UIView *view in self.webView.subviews) {
            if ([view isKindOfClass:UINavigationBar.class]) {
                view.frame = CGRectMake(0, 0, size.width, view.frame.size.height);
            }
        }
    }

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
        // navBar.delegate = self;
	navBar.tintColor = [UIColor blackColor];
        navBar.barTintColor = UIColor.whiteColor;

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
        //navBar.translucent = YES;
        navBar.barStyle = UIBarStyleDefault;
        [self.webView addSubview:navBar];
        
    }
    // self.webView.layer.zPosition = 1000000.0f;
    [self.webView.superview bringSubviewToFront:self.webView];
    
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
