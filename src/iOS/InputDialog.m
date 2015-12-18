#import <QuartzCore/QuartzCore.h>

#import "InputDialog.h"

// Privates
@interface InputDialog ()
@property (nonatomic, assign) IBOutlet UITextField* textField;
@property (nonatomic, assign) CFRunLoopRef runloopRef;
@end

@implementation InputDialog

#pragma mark - public methods

-(void) showModal {
    // add the dialog as the child of the topmost UIView
    UIWindow* keyWindow = [[UIApplication sharedApplication] keyWindow];
    UIView* topmostView = [keyWindow.subviews objectAtIndex:0];
    [topmostView addSubview:self];
    
    // Center the dialog above the keyboard
    CGSize s = topmostView.bounds.size;
    self.center = CGPointMake(s.width / 2, (s.height / 2) - 65);

    // Start a new event loop and block while its running
    CFRunLoopRun();
}

-(NSString*) getInput {
    return self.textField.text;
}

+(InputDialog*) dialog {
    NSArray* nibViews = [[NSBundle mainBundle] loadNibNamed:@"InputDialog"
                                                      owner:nil
                                                    options:nil];
//    InputDialog* dlg = [[[nibViews objectAtIndex:0] retain] autorelease];
    InputDialog* dlg = [nibViews objectAtIndex:0];
  
    // Customize the dialog a bit
    dlg.layer.borderColor = [UIColor blackColor].CGColor;
    dlg.layer.borderWidth = 1;
    dlg.layer.cornerRadius = 10;
    dlg.layer.shadowColor = [UIColor blackColor].CGColor;
    dlg.layer.shadowOpacity = 1.0;
    dlg.layer.shadowRadius = 7.5;
    dlg.layer.shadowOffset = CGSizeMake(1, 4);
    dlg.clipsToBounds = NO;
    
    return dlg;
}

// -(void) dealloc {
//     NSLog(@"InputDialog deallocated!");
    
//    [super dealloc];
// }

-(IBAction) okPressed {
    // Remove the dialog
    [self removeFromSuperview];
    
    // Stop event loop
    CFRunLoopStop(CFRunLoopGetCurrent());
}

@end
