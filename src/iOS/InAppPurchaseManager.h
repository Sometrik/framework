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
- (void)inAppPurchaseManagerDidCompleteTransaction:(InAppPurchaseManager *)manager forProductId:(NSString *)productId;
@end

@interface InAppPurchaseManager : NSObject

@property (nonatomic, strong) NSArray *products;
@property (nonatomic, assign) id<InAppPurchaseManagerDelegate> delegate;
@property (nonatomic, strong) NSMutableSet *productIdentifiers;

+ (InAppPurchaseManager *)sharedInstance;
- (void)addProductIds:(NSArray *)productIds;
- (void)fetchProductIdentifiers; // from plist file if it's there
- (void)makeProductsRequest;
- (void)buyProductWithIdentifier:(NSString *)identifier;

@end
