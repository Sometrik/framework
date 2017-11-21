#import <UIKit/UIKit.h>

@interface InputDialog : UIView

/** Creates a new dialog. */
+(InputDialog*) dialog;

/** Displays the dialog in a modal fashion. */
-(void) showModal;

/** Returns the input value. */
-(NSString*) getInput;

@end
