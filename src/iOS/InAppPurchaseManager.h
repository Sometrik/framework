//
//  InAppPurchaseManager.h
//  Example1
//
//  Created by Sami Rämö on 08/12/2017.
//  Copyright © 2017 sometrik. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <StoreKit/SKProduct.h>
#import "StoreKit/SKPaymentQueue.h"

@class InAppPurchaseManager;

@protocol InAppPurchaseManagerDelegate <NSObject>
- (void)inAppPurchaseManagerDidReceiveProducts:(InAppPurchaseManager *)manager;
@end

@interface InAppPurchaseManager : NSObject

@property (nonatomic, strong) NSArray *products;
@property (nonatomic, assign) id <InAppPurchaseManagerDelegate> delegate;

+ (InAppPurchaseManager *)sharedInstance;
- (void)addProductId:(NSString *)productId;
- (void)fetchProductIdentifiers; // from plist file if it's there
- (void)makeProductsRequest;
- (void)makePaymentRequestForProduct:(SKProduct *)product;

@end
