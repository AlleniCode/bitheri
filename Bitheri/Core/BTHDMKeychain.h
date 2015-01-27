//
//  BTHDMKeychain.h
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
#import "BTHDMAddress.h"
#import "BTBIP32Key.h"
#import "BTPasswordSeed.h"

@protocol BTHDMFetchRemotePublicKeys
-(void)completeRemotePublicKeysWithPassword:(NSString*)password andPartialPubs:(NSArray*)partialPubs;
@end

@protocol BTHDMFetchRemoteAddressesDelegate
-(NSArray*) getRemoteExistsPublicKeysWithPassword:(NSString*)password;
@end

@protocol BTHDMAddressChangeDelegate
-(void)hdmAddressAdded:(BTHDMAddress*)address;
@end

@interface BTHDMKeychain : NSObject

@property (nonatomic) int hdSeedId;
@property (nonatomic, copy, readonly) NSString *firstAddressFromDb;
@property (nonatomic, readonly) BOOL isFromXRandom;
@property (nonatomic, readonly) NSArray* addresses;
@property (nonatomic) NSMutableArray *allCompletedAddresses;
@property (nonatomic, readonly) UInt32 uncompletedAddressCount;
@property (nonatomic, readonly) BOOL isInRecovery;
@property (nonatomic, weak) NSObject<BTHDMAddressChangeDelegate> *addressChangeDelegate;

-(instancetype)initWithMnemonicSeed:(NSData*)seed password:(NSString*)password andXRandom:(BOOL)xrandom;

-(instancetype)initWithSeedId:(int)seedId;

-(instancetype)initWithEncrypted:(NSString*)encryptedMnemonicSeed password:(NSString*) password andFetchDelegate:(NSObject<BTHDMFetchRemoteAddressesDelegate>*)fetchDelegate;

-(NSUInteger)prepareAddressesWithCount:(UInt32)count password:(NSString*)password andColdExternalPub:(NSData*)coldExternalPub;
-(NSArray*)completeAddressesWithCount:(UInt32)count password:(NSString*)password andFetchDelegate:(NSObject<BTHDMFetchRemotePublicKeys>*)fetchDelegate;

-(BTBIP32Key*)externalKeyWithIndex:(uint) index andPassword:(NSString*)password;
-(NSData*)externalChainRootPubExtended:(NSString*)password;
-(NSString*)externalChainRootPubExtendedAsHex:(NSString*)password;

-(BOOL)checkWithPassword:(NSString*)password;
-(NSString*)signHDMBIdWithMessageHash:(NSString*)messageHash andPassword:(NSString*)password;
@end