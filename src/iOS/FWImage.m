//
//  FWImage.m
//  Example1
//
//  Created by Sami Rämö on 12/12/2017.
//  Copyright © 2017 sometrik. All rights reserved.
//

#import "FWImage.h"

@implementation FWImage

- (instancetype)initWithUrl:(NSString *)url width:(int)width height:(int)height
{
    if (self = [super init]) {
        self.url = url;
        self.width = width;
        self.height = height;
    }
    return self;
}

@end
