//
//  BTTx.h
//  bitheri
//
//  Copyright 2014 http://Bither.net
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#import <Foundation/Foundation.h>
//#import "BTTxItem.h"

#import "NSString+Base58.h"
#import "NSMutableData+Bitcoin.h"
#import "NSData+Bitcoin.h"
#import "NSData+Hash.h"

@class BTKey;
@class BTAddress;
@class BTOut;

#if TX_FEE_07_RULES
#define TX_FEE_PER_KB        50000llu    // standard tx fee per kb of tx size, rounded up to the nearest kb (0.7 rules)
#else
#define TX_FEE_PER_KB        10000llu    // standard tx fee per kb of tx size, rounded up to the nearest kb
#endif

@interface BTTx : NSObject

@property (nonatomic, assign) uint32_t blockNo;
@property (nonatomic, copy) NSData *txHash;
@property (nonatomic, assign) uint32_t txVer;
@property (nonatomic, assign) uint32_t txLockTime;
@property (nonatomic, assign) uint32_t txTime;
@property (nonatomic, assign) int source;
@property (nonatomic, assign) int sawByPeerCnt;

@property (nonatomic, strong) NSMutableArray *ins;
@property (nonatomic, strong) NSMutableArray *outs;

- (NSArray *)getInAddresses;


@property (nonatomic, readonly) uint64_t standardFee;

- (NSData *)toData;

- (size_t)size;
- (BOOL)isSigned;


@property (nonatomic, readonly) uint confirmationCnt;
@property (nonatomic, readonly) BOOL isCoinBase;


+ (instancetype)transactionWithMessage:(NSData *)message;

- (instancetype)initWithMessage:(NSData *)message;

- (void)addInputHash:(NSData *)hash index:(NSUInteger)index script:(NSData *)script;
- (void)addInputHash:(NSData *)hash index:(NSUInteger)index script:(NSData *)script signature:(NSData *)signature
sequence:(uint32_t)sequence;

- (void)clearIns;

- (void)addOutputAddress:(NSString *)address amount:(uint64_t)amount;
- (void)addOutputScript:(NSData *)script amount:(uint64_t)amount;

- (void)setInputAddress:(NSString *)address atIndex:(NSUInteger)index;

- (BOOL)signWithPrivateKeys:(NSArray *)privateKeys;

// priority = sum(input_amount_in_satoshis*input_age_in_blocks)/tx_size_in_bytes
- (uint64_t)priorityForAmounts:(NSArray *)amounts withAges:(NSArray *)ages;

// the block height after which the transaction can be confirmed without a fee, or TX_UNCONFIRMED for never
- (uint32_t)blockHeightUntilFreeForAmounts:(NSArray *)amounts withBlockHeights:(NSArray *)heights;

- (void)sawByPeer;

- (BTOut *)getOut:(uint)outSn;

// returns the amount received to the wallet by the transaction (total outputs to change and/or recieve addresses)
- (uint64_t)amountReceivedFrom:(BTAddress *)addr;;

// retuns the amount sent from the wallet by the trasaction (total wallet outputs consumed, change and fee included)
- (uint64_t)amountSentFrom:(BTAddress *)addr;

- (uint64_t)amountSentTo:(NSString *)addr;

- (int64_t)deltaAmountFrom:(BTAddress *)addr;

// returns the fee for the given transaction if all its inputs are from wallet transactions, UINT64_MAX otherwise
- (uint64_t)feeForTransaction;

//// returns the first non-change transaction output address, or nil if there aren't any
//- (NSString *)addressForTransaction:(BTTx *)transaction;

// returns the block height after which the transaction is likely to be processed without including a fee
- (uint32_t)blockHeightUntilFree;

- (void)setInScript:(NSData *)script forInHash:(NSData *)inHash andInIndex:(NSUInteger) inIndex;
- (NSArray *)unsignedInHashes;
- (BOOL)signWithSignatures:(NSArray *)signatures;
- (NSData *) hashForSignature:(NSUInteger) inputIndex connectedScript:(NSData *) connectedScript sigHashType:(uint8_t) sigHashType;

- (BOOL)verify;
- (BOOL)verifySignatures;

- (BOOL)hasDustOut;

- (uint)estimateSize;

@end
