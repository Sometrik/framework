#import "PaddedLabel.h"

#import "FWLayoutView.h"

@implementation PaddedLabel

- (id)init {
    self = [super init];
    if (self) {
        self.edgeInsets = UIEdgeInsetsMake(0, 0, 0, 0);
        self.autolink = NO;
        self.markdown = NO;
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
        self.markdown = NO;
        UITapGestureRecognizer *tapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(didTap:)];
        [self addGestureRecognizer:tapGesture];
    }
    return self;
}

- (void)drawTextInRect:(CGRect)rect {
  [super drawTextInRect:UIEdgeInsetsInsetRect(rect, self.edgeInsets)];
}

- (void)setBounds:(CGRect)bounds {
  int adjustedBoundsWidth = bounds.size.width; // - (self.edgeInsets.left + self.edgeInsets.right + 2);
  if (adjustedBoundsWidth < self.preferredMaxLayoutWidth) {
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
    if ([link hasPrefix:@"# "]) {
      return [link substringFromIndex:2];
    } else {
      unichar firstChar = [link characterAtIndex:0];
      if (firstChar != '#' && firstChar != '@') {
        if ([link hasPrefix:@"http://"]) {
	  link = [link substringFromIndex:7];
        } else if ([link hasPrefix:@"https://"]) {
	  link = [link substringFromIndex:8];    
        }

        NSRange range = [link rangeOfString:@"?"];
        if (range.location != NSNotFound) {
	  link = [link substringWithRange:NSMakeRange(0, range.location)];
        }
        range = [link rangeOfString:@"#"];
        if (range.location != NSNotFound) {
	  link = [link substringWithRange:NSMakeRange(0, range.location)];
        }
	if (link.length > 0 && [link characterAtIndex:(link.length - 1)] == '/') {
	  link = [link substringWithRange:NSMakeRange(0, link.length - 1)];
	}
      }
    }
  }
  return link;
}

- (NSAttributedString *)createAttributedString:(NSString *)input
{
    NSError *error = NULL;
    NSRegularExpression * linkRegex;
    if (self.autolink && self.markdown) {
        linkRegex = [NSRegularExpression regularExpressionWithPattern:@"(^#\\s+.*$|@[A-Za-z0-9_]+|#[\\p{L}0-9_]*[\\p{L}][\\p{L}0-9_]*|https?://[A-Za-z0-9._~:/?#\\[\\]@!$&'()*+,;=%-]*[A-Za-z0-9._~:/?#@!$&'*+;=%-]+)" options:NSRegularExpressionAnchorsMatchLines error:&error];
    } else if (self.autolink && !self.markdown) {
        linkRegex = [NSRegularExpression regularExpressionWithPattern:@"(@[A-Za-z0-9_]+|#[\\p{L}0-9_]*[\\p{L}][\\p{L}0-9_]*|https?://[A-Za-z0-9._~:/?#\\[\\]@!$&'()*+,;=%-]+)" options:NSRegularExpressionAnchorsMatchLines error:&error];
    } else if (!self.autolink && self.markdown) {
        linkRegex = [NSRegularExpression regularExpressionWithPattern:@"(^#\\s+.*$)" options:NSRegularExpressionAnchorsMatchLines error:&error];
    } else {
        return nil;
    }
    
    NSMutableArray *linkRanges = [[NSMutableArray alloc] init];
    NSMutableArray *linkTargets = [[NSMutableArray alloc] init];
       
    __block NSString * s = input;
    __block int offset = 0;
    [linkRegex enumerateMatchesInString:input options:0 range:NSMakeRange(0, input.length) usingBlock:^(NSTextCheckingResult *result, NSMatchingFlags flags, BOOL *stop) {       
        NSRange range = [result rangeAtIndex:0];
        range.location += offset;
        NSString * link = [s substringWithRange:range];
        NSString * displayLink = [self cleanLink:link];

        int diff = displayLink.length - link.length;
        if (diff != 0) {
            offset += diff;
            s = [s stringByReplacingCharactersInRange:range withString:displayLink];
            range.length = displayLink.length;
        }

        [linkRanges addObject:[NSValue valueWithRange:range]];
        [linkTargets addObject:link];
    }];
    
    UIFontDescriptor * fontD = [self.font.fontDescriptor fontDescriptorWithSymbolicTraits:UIFontDescriptorTraitBold];
    UIFont * boldFont = [UIFont fontWithDescriptor:fontD size:self.defaultSize + 4];
    
    UIFontDescriptor * fontD2 = [self.font.fontDescriptor fontDescriptorWithSymbolicTraits:UIFontDescriptorTraitUIOptimized];
    UIFont * defaultFont = [UIFont fontWithDescriptor:fontD2 size:self.defaultSize];
    
    UIColor * defaultColor = self.defaultColor != nil ? self.defaultColor : UIColor.blackColor;
    
    NSMutableAttributedString *attributedString = [[NSMutableAttributedString alloc] initWithString:s attributes:
  @{
          NSFontAttributeName: defaultFont,
    	  NSForegroundColorAttributeName: defaultColor
    }];
    
    [attributedString beginEditing];
    
    UIColor * linkColor = [UIColor colorWithRed:0.765 green:0.145 blue:0.153 alpha:1.0];

    NSInteger prevLocation = 0;
    for (NSInteger i = 0; i < linkRanges.count; i++) {
        NSString *urlString = linkTargets[i];
        NSValue *urlRangeValue = linkRanges[i];
        NSRange urlRange;
        [urlRangeValue getValue:&urlRange];

	if (urlRange.location > prevLocation) {	  
            [attributedString addAttributes:@{
                                              NSFontAttributeName: defaultFont,
                                      NSForegroundColorAttributeName: defaultColor
		  } range:NSMakeRange(prevLocation, urlRange.location - prevLocation)];
	}
	prevLocation = urlRange.location + urlRange.length;

        if ([urlString hasPrefix:@"# "]) {
            [attributedString removeAttribute:NSFontAttributeName range:urlRange];
            [attributedString addAttributes:@{
		                              NSFontAttributeName: boldFont,
                                      NSForegroundColorAttributeName: defaultColor
                                              } range:urlRange];
        } else if ([urlString hasPrefix:@"#"] || [urlString hasPrefix:@"@"]) {
            [attributedString removeAttribute:NSFontAttributeName range:urlRange];
            [attributedString addAttributes:@{
                                              NSFontAttributeName: defaultFont,
                                              NSForegroundColorAttributeName: linkColor,
                                              } range:urlRange];
        } else {
            NSURL * url = [NSURL URLWithString:urlString];
            if (url != nil) {
                [attributedString removeAttribute:NSFontAttributeName range:urlRange];
                [attributedString addAttributes:@{
                                              NSFontAttributeName: defaultFont,
                                                  NSLinkAttributeName: url,
                                                  NSForegroundColorAttributeName: linkColor,
                                                  NSUnderlineStyleAttributeName: [NSNumber numberWithInt:NSUnderlineStyleSingle],
                                                  } range:urlRange];
            }
        }
    }

    if (s.length > prevLocation) {	  
        [attributedString addAttributes:@{
                                             NSFontAttributeName: defaultFont,
                                      NSForegroundColorAttributeName: defaultColor
	} range:NSMakeRange(prevLocation, s.length - prevLocation)];
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
    NSLog(@"fontSize = %f", fontSize);
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
            
            UIFont *font = [currentFont fontWithSize:currentFont.pointSize - 1];
            CGSize size = [self boundingSizeForString:self.attributedText.string font:font];
            if (currentSize.height < self.frame.size.height || font.pointSize < (originalFontSize * self.minimumScaleFactor)) {
                break;
            }
            currentFont = font;
            currentSize = size;
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
