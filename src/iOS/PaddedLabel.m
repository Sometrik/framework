#import "PaddedLabel.h"

#import "FWLayoutView.h"

@implementation PaddedLabel

- (id)init {
    self = [super init];
    if (self) {
        self.edgeInsets = UIEdgeInsetsMake(0, 0, 0, 0);
        self.autolink = NO;
        UITapGestureRecognizer *tapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(didTap:)];
        [self addGestureRecognizer:tapGesture];
    }
    return self;
}

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        self.edgeInsets = UIEdgeInsetsMake(0, 0, 0, 0);
	self.autolink = NO;
    }
    return self;
}

- (void)drawTextInRect:(CGRect)rect {
  [super drawTextInRect:UIEdgeInsetsInsetRect(rect, self.edgeInsets)];
}

- (void)setBounds:(CGRect)bounds {
  int adjustedBoundsWidth = bounds.size.width; // - (self.edgeInsets.left + self.edgeInsets.right + 2);
  if (adjustedBoundsWidth < self.preferredMaxLayoutWidth) {
    // NSLog(@"Bounds changed %f => %f", self.bounds.size.width, bounds.size.width);
    [super setBounds:bounds];
#if 0
    [self setNeedsUpdateConstraints];
#else
    self.preferredMaxLayoutWidth = adjustedBoundsWidth;
    [self relayoutAll];
#endif
  } else {
    [super setBounds:bounds];
  }
}

- (void)relayoutAll {
    UIView * view = self.superview;
    while (view != nil && ([view isKindOfClass:FWLayoutView.class] || [view isKindOfClass:UIScrollView.class])) {
        [view setNeedsLayout];
	if ([view isKindOfClass:UIScrollView.class]) {
	  break;
	}
        view = view.superview;
    }
}

#if 0
- (void)updateConstraints {
  if (self.preferredMaxLayoutWidth != self.bounds.size.width) {
    self.preferredMaxLayoutWidth = self.bounds.size.width;
  }
  [super updateConstraints];
}
#endif

#if 0
- (void)layoutSubviews {
    [super layoutSubviews];
    self.preferredMaxLayoutWidth = self.frame.size.width;
    [self.superview layoutIfNeeded];
}
#endif

- (CGSize)intrinsicContentSize
{
  CGSize size = [super intrinsicContentSize];
  size.width  += self.edgeInsets.left + self.edgeInsets.right + 2;
  size.height += self.edgeInsets.top + self.edgeInsets.bottom + 2;
  return size;
}

- (NSString *)cleanLink:(NSString *)link
{
  if (link.length > 0) {
    unichar firstChar = [link characterAtIndex:0];
    if (firstChar != '#' && firstChar != '@') {
#if 0
      if ([link hasPrefix:@"http://"]) {
	link = [link substringFromIndex:7];
      } else if ([link hasPrefix:@"https://"]) {
	link = [link substringFromIndex:8];    
      }
#endif
      NSRange range = [link rangeOfString:@"?"];
      if (range.location != NSNotFound) {
	link = [link substringWithRange:NSMakeRange(0, range.location)];
      }
      range = [link rangeOfString:@"#"];
      if (range.location != NSNotFound) {
	link = [link substringWithRange:NSMakeRange(0, range.location)];
      }
      
    }
  }
  return link;
}

- (NSAttributedString *)createAttributedString:(NSString *)input
{
    NSError *error = NULL;
    NSRegularExpression *linkRegex = [NSRegularExpression regularExpressionWithPattern:@"(@[A-Za-z0-9_]+|#[\\p{L}0-9_]+|https?://[A-Za-z0-9._~:/?#\\[\\]@!$&'()*+,;=%-]+)" options:0 error:&error];
    
    NSMutableArray *linkRanges = [[NSMutableArray alloc] init];
    NSMutableArray *linkTargets = [[NSMutableArray alloc] init];
       
    __block NSString * s = input;
    __block int offset = 0;
    [linkRegex enumerateMatchesInString:input options:0 range:NSMakeRange(0, input.length) usingBlock:^(NSTextCheckingResult *result, NSMatchingFlags flags, BOOL *stop) {       
        NSRange range = [result rangeAtIndex:0];
	range.location += offset;
	NSString * link = [s substringWithRange:range];
	NSString * displayLink = [self cleanLink:link];
#if 1
	int diff = displayLink.length - link.length; 
	if (diff != 0) {
	  offset += diff;
	  s = [s stringByReplacingCharactersInRange:range withString:displayLink];
	  range.length = displayLink.length;
	}
#endif
        [linkRanges addObject:[NSValue valueWithRange:range]];
        [linkTargets addObject:link];
    }];
       
    UIFont * boldFont = self.boldFont;
    if (boldFont == nil) {
        UIFontDescriptor * fontD = [self.font.fontDescriptor fontDescriptorWithSymbolicTraits:UIFontDescriptorTraitBold];
        boldFont = [UIFont fontWithDescriptor:fontD size:self.font.pointSize];
    }

    NSMutableAttributedString *attributedString = [[NSMutableAttributedString alloc] initWithString:s attributes:
  @{
      NSFontAttributeName: self.defaultFont != nil ? self.defaultFont : self.font,
      NSForegroundColorAttributeName: self.defaultColor != nil ? self.defaultColor : UIColor.blackColor,
    }];
    
    [attributedString beginEditing];
    
    UIColor * linkColor = [UIColor colorWithRed:0.765 green:0.145 blue:0.153 alpha:1.0];

    for (NSInteger i = 0; i < linkRanges.count; i++) {
        NSString *urlString = linkTargets[i];
        NSValue *urlRangeValue = linkRanges[i];
        NSRange urlRange;
        [urlRangeValue getValue:&urlRange];
        unichar firstChar = [urlString characterAtIndex:0];
        if (firstChar == '#' || firstChar == '@') {
            [attributedString addAttributes:@{
		// NSFontAttributeName: boldFont,
                                              NSForegroundColorAttributeName: linkColor,
                                              } range:urlRange];
        } else {
            NSURL * url = [NSURL URLWithString:urlString];
            if (url != nil) {
                [attributedString addAttributes:@{
                                                  NSLinkAttributeName: url,
                                                  NSForegroundColorAttributeName: linkColor,
                                                  NSUnderlineStyleAttributeName: [NSNumber numberWithInt:NSUnderlineStyleNone],
                                                  } range:urlRange];
            }
        }
    }
    [attributedString endEditing];
    
    return attributedString;
}

- (void)didTap:(UITapGestureRecognizer *)gestureRecognizer
{
    // original layout
    NSLayoutManager *layoutManager = [[NSLayoutManager alloc] init];
    NSTextStorage *origTextStorage = [[NSTextStorage alloc] initWithAttributedString:self.attributedText];
    [origTextStorage addLayoutManager:layoutManager];
    NSTextContainer *textContainer = [[NSTextContainer alloc] initWithSize:self.bounds.size];
    textContainer.lineBreakMode = self.lineBreakMode;
    textContainer.maximumNumberOfLines = self.numberOfLines;
    textContainer.lineFragmentPadding = 0;
    [layoutManager addTextContainer:textContainer];
    
    NSMutableAttributedString *attributedString = [[NSMutableAttributedString alloc] initWithString:self.attributedText.string];
    UIFont *font = (UIFont *)[self.attributedText attribute:NSFontAttributeName atIndex:0 effectiveRange:NULL];
    NSInteger indx = 0;
    NSRange glyphRange = NSMakeRange(0,self.attributedText.length);
    //CGRect rect = [layoutManager lineFragmentRectForGlyphAtIndex:indx effectiveRange:&glyphRange withoutAdditionalLayout:NO];
    CGFloat fontSize = [self getApproximateAdjustedFontSize];
    UIFont *newFont = [font fontWithSize:fontSize];
    
    [origTextStorage removeLayoutManager:layoutManager];
    [attributedString addAttribute:NSFontAttributeName value:newFont range:NSMakeRange(0, attributedString.string.length-1)];
    NSTextStorage *textStorage = [[NSTextStorage alloc] initWithAttributedString:attributedString];
    [textStorage addLayoutManager:layoutManager];
    
    CGPoint touchPoint = [gestureRecognizer locationOfTouch:0 inView:self];
    NSUInteger index = [layoutManager glyphIndexForPoint:touchPoint inTextContainer:textContainer];
    
    NSRange range = NSMakeRange(index, 1);
    NSDictionary *attributes = [self.attributedText attributesAtIndex:index effectiveRange:&range];
    if ([attributes objectForKey:NSLinkAttributeName] != nil) {
        NSURL *url = [attributes objectForKey:NSLinkAttributeName];
        if ([self.delegate respondsToSelector:@selector(paddedLabel:didOpenLinkURL:)]) {
            [self.delegate paddedLabel:self didOpenLinkURL:url];
        }
    }
}

- (CGFloat)getApproximateAdjustedFontSize {
    
    if (self.adjustsFontSizeToFitWidth) {
        
        UIFont *currentFont = self.font;
        CGFloat originalFontSize = currentFont.pointSize;
        CGSize currentSize = [self boundingSizeForString:self.attributedText.string font:currentFont];
        
        while (currentSize.height > self.frame.size.height && currentFont.pointSize > (originalFontSize * self.minimumScaleFactor)) {
            currentFont = [currentFont fontWithSize:currentFont.pointSize - 1];
            currentSize = [self boundingSizeForString:self.attributedText.string font:currentFont];
        }
        return currentFont.pointSize;
    } else {
        return self.font.pointSize;
    }
}

- (CGSize)boundingSizeForString:(NSString *)string font:(UIFont *)font
{
    
    CGRect boundingRect = [string boundingRectWithSize:CGSizeMake(self.frame.size.width, MAXFLOAT)
                                               options:NSStringDrawingUsesLineFragmentOrigin | NSStringDrawingUsesFontLeading
                                            attributes:@{NSFontAttributeName: font}
                                               context:nil];
    return CGSizeMake(ceilf(boundingRect.size.width), ceilf(boundingRect.size.height));
}

@end
