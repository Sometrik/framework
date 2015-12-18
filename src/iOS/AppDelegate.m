#import "AppDelegate.h"

@implementation AppDelegate

#if 0
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Override point for customization after application launch.
    return YES;
}

// - (void) applicationDidFinishLaunching: (UIApplication*) application
// {
//   CGRect screenBounds = [[UIScreen mainScreen] bounds];

//   window = [[UIWindow alloc] initWithFrame: screenBounds];
//   m_view = [[GLView alloc] initWithFrame: screenBounds];

//   [m_window addSubview: m_view];
//   [m_window makeKeyAndVisible];
// }
							
- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}
#else

- (void) applicationDidFinishLaunching: (UIApplication*) application
{
  CGRect screenBounds = [[UIScreen mainScreen] bounds];

  window = [[UIWindow alloc] initWithFrame: screenBounds];
  view = [[OpenGLView alloc] initWithFrame: screenBounds];
  controller = [[UIViewController alloc] init];
  controller.view = view;
  //  [window addSubview: view];
  [window setRootViewController:controller];
  [window makeKeyAndVisible];
}

- (void) dealloc
{
  [view release];
  [window release];
  [super dealloc];
}

#endif

@end
