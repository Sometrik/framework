//
//  FWImage.h
//  Example1
//
//  Created by Sami Rämö on 12/12/2017.
//  Copyright © 2017 sometrik. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface FWImage : NSObject

@property (nonatomic, strong) NSString* url;
@property (nonatomic, assign) int width;
@property (nonatomic, assign) int height;

- (instancetype)initWithUrl:(NSString *)url width:(int)width height:(int)height;

@end
