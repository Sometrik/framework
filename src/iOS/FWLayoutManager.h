#import <UIKit/UIKit.h>

@interface FWLayoutManager : NSLayoutManager

- (void)drawUnderlineForGlyphRange:(NSRange)glyphRange 
                     underlineType:(NSUnderlineStyle)underlineVal 
                    baselineOffset:(CGFloat)baselineOffset
                  lineFragmentRect:(CGRect)lineRect 
            lineFragmentGlyphRange:(NSRange)lineGlyphRange
                   containerOrigin:(CGPoint)containerOrigin;

@end
