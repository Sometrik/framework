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
      if ([link hasPrefix:@"http://"]) {
	return [link substringFromIndex:7];
      } else if ([link hasPrefix:@"https://"]) {
	return [link substringFromIndex:8];    
      }
    }
  }
  return link;
}

- (NSAttributedString *)createAttributedString:(NSString *)input
{
    NSError *error = NULL;
    NSRegularExpression *linkRegex = [NSRegularExpression regularExpressionWithPattern:@"(@[A-Za-z0-9_]+|#[\\p{L}0-9_]+|https?://\\S+)" options:0 error:&error];
    
    NSMutableArray *linkRanges = [[NSMutableArray alloc] init];
    NSMutableArray *linkTargets = [[NSMutableArray alloc] init];
       
    __block NSString * s = input;
    __block int offset = 0;
    [linkRegex enumerateMatchesInString:input options:0 range:NSMakeRange(0, input.length) usingBlock:^(NSTextCheckingResult *result, NSMatchingFlags flags, BOOL *stop) {       
        NSRange range = [result rangeAtIndex:0];
	range.location += offset;
	NSString * link = [s substringWithRange:range];
	NSString * displayLink = [self cleanLink:link];
#if 0
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
    
    UIColor * linkColor = [UIColor colorWithRed:0.5 green:0.1 blue:0.1 alpha:1.0];

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
            [attributedString addAttributes:@{
	      NSLinkAttributeName: [NSURL URLWithString:urlString],
		  NSForegroundColorAttributeName: linkColor,
		  NSUnderlineStyleAttributeName: [NSNumber numberWithInt:NSUnderlineStyleNone],
                                              } range:urlRange];
        }
    }
    [attributedString endEditing];
    
    return attributedString;
}

- (void)didTap:(UITapGestureRecognizer *)gestureRecognizer
{
    NSTextStorage *textStorage = [[NSTextStorage alloc] initWithAttributedString:self.attributedText];
    NSLayoutManager *layoutManager = [[NSLayoutManager alloc] init];
    [textStorage addLayoutManager:layoutManager];
    NSTextContainer *textContainer = [[NSTextContainer alloc] initWithSize:self.frame.size];
    
    textContainer.lineFragmentPadding = 0;
    NSRange glyphRange;
    
    CGPoint touchPoint = [gestureRecognizer locationOfTouch:0 inView:self];
    [layoutManager addTextContainer:textContainer];
    NSUInteger index = [layoutManager characterIndexForPoint:touchPoint inTextContainer:textContainer fractionOfDistanceBetweenInsertionPoints:0];
    
    NSRange range = NSMakeRange(index, 1);
    NSDictionary *attributes = [self.attributedText attributesAtIndex:index effectiveRange:&range];
    if ([attributes objectForKey:NSLinkAttributeName] != nil) {
        NSURL *url = [attributes objectForKey:NSLinkAttributeName];
        if ([self.delegate respondsToSelector:@selector(paddedLabel:didOpenLinkURL:)]) {
            [self.delegate paddedLabel:self didOpenLinkURL:url];
        }
    }
}
@end
