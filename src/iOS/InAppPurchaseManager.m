//
//  InAppPurchaseManager.m
//  Example1
//
//  Created by Sami Rämö on 08/12/2017.
//  Copyright © 2017 sometrik. All rights reserved.
//

#import "InAppPurchaseManager.h"
#import "StoreKit/SKProductsRequest.h"
#import "StoreKit/SKPayment.h"
#import "StoreKit/SKPaymentQueue.h"
#import "StoreKit/SKPaymentTransaction.h"

@interface InAppPurchaseManager () <SKProductsRequestDelegate, SKPaymentTransactionObserver>
@property (nonatomic, strong) SKProductsRequest *request;
@property (nonatomic, strong) NSMutableArray *productIdentifiers;
@end

@implementation InAppPurchaseManager

// make singleton instance of InAppPurchaseManager
// Usage example: InAppPurchaseManager *manager = [InAppPurchaseManager sharedInstance];
+ (InAppPurchaseManager *)sharedInstance
{
    static dispatch_once_t onceToken;
    static InAppPurchaseManager *inAppPurchaseManagerSharedInstance;
    dispatch_once(&onceToken, ^{
        inAppPurchaseManagerSharedInstance = [[InAppPurchaseManager alloc] init];
    });
    return inAppPurchaseManagerSharedInstance;
}

- (instancetype)init
{
    self = [super init];
    return self;
}

- (void)addProductId:(NSString *)productId
{
    [self.productIdentifiers addObject:productId];
}

// if id's are in plist file in the bundle, use this
- (void)fetchProductIdentifiers
{
    NSURL *url = [[NSBundle mainBundle] URLForResource:@"product_ids"
                                         withExtension:@"plist"];
    _productIdentifiers = [NSMutableArray arrayWithContentsOfURL:url];
}

// Custom method
- (void)makeProductsRequest
{
    SKProductsRequest *productsRequest = [[SKProductsRequest alloc]
                                          initWithProductIdentifiers:[NSSet setWithArray:self.productIdentifiers]];
    
    // Keep a strong reference to the request.
    self.request = productsRequest;
    productsRequest.delegate = self;
    [productsRequest start];
}

// SKProductsRequestDelegate protocol method
- (void)productsRequest:(SKProductsRequest *)request
     didReceiveResponse:(SKProductsResponse *)response
{
    self.products = response.products;
    
    for (NSString *invalidIdentifier in response.invalidProductIdentifiers) {
        // Handle any invalid product identifiers.
        NSLog(@"invalidIdentifier: %@", invalidIdentifier);
    }
    
    // inform delegate that products are ready
    if ( [self.delegate respondsToSelector:@selector(inAppPurchaseManagerDidReceiveProducts:)] ) {
        [self.delegate inAppPurchaseManagerDidReceiveProducts:self];
    }}

- (void)makePaymentRequestForProduct:(SKProduct *)product
{
    SKMutablePayment *payment = [SKMutablePayment paymentWithProduct:product];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction *transaction in transactions) {
        switch (transaction.transactionState) {
                // Call the appropriate custom method for the transaction state.
            case SKPaymentTransactionStatePurchasing:
                //[self showTransactionAsInProgress:transaction deferred:NO];
                break;
            case SKPaymentTransactionStateDeferred:
                //[self showTransactionAsInProgress:transaction deferred:YES];
                break;
            case SKPaymentTransactionStateFailed:
                //[self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored:
                //[self restoreTransaction:transaction];
                break;
            default:
                // For debugging
                NSLog(@"Unexpected transaction state %@", @(transaction.transactionState));
                break;
        }
    }
}

- (void)completeTransaction:(SKPaymentTransaction *)transaction
{
    // Validate the purchase...

}

@end
