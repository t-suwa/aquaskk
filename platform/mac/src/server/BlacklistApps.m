/* -*- ObjC -*-

 MacOS X implementation of the SKK input method.

 Copyright (C) 2009 Tomotaka SUWA <t.suwa@mac.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 */

#import "BlacklistApps.h"

@interface BlacklistApps(Local)
- (BOOL)isJavaApp:(NSBundle*)bunlde;
- (BOOL)isBlacklistApp:(NSString*)bunldeIdentifier withKey:(NSString*)key;
@end

@implementation BlacklistApps
static BlacklistApps* sharedData_ = nil;

- (void)load:(NSArray*)xs {
    id old = blacklistApps_;
    blacklistApps_ = [xs retain];
    [old release];
}

- (BOOL)isInsertEmptyString:(NSBundle *)bundle {
    NSMutableDictionary* entry = [self getEntry:[bundle bundleIdentifier]];
    if(entry) {
        return [entry[@"insertEmptyString"] boolValue];
    }

    if([self isJavaApp:bundle]) {
        return YES;
    }

    // very special apps
    if([[bundle bundleIdentifier] hasPrefix:@"com.microsoft.Excel"] &&
       [[bundle objectForInfoDictionaryKey:@"CFBundleVersion"] hasPrefix:@"15."]) {
        return YES;
    }

    return NO;
}

- (BOOL)isInsertMarkedText:(NSString *)bundleIdentifier {
    return [self isBlacklistApp:bundleIdentifier withKey:@"insertMarkedText"];
}

- (BOOL)isSyncInputSource:(NSBundle *)bundle {
    return [self isBlacklistApp:[bundle bundleIdentifier] withKey:@"syncInputSource"];
}

- (NSMutableDictionary*)getEntry:(NSString*)bundleIdentifier{
    for (NSMutableDictionary* entry in blacklistApps_) {
        if([bundleIdentifier hasPrefix: entry[@"bundleIdentifier"]]) {
            return entry;
        }
    }
    return nil;
}

- (BOOL)isBlacklistApp:(NSString*)bundleIdentifier withKey:(NSString*)key {
    NSMutableDictionary* entry = [self getEntry: bundleIdentifier];

    return [entry[key] boolValue];
}

- (BOOL)isJavaApp:(NSBundle*)bundle {
    if([[bundle bundleIdentifier] hasPrefix:@"jp.naver.line.mac"]) {
        return YES;
    }
    if([bundle objectForInfoDictionaryKey:@"Java"]) {
        return YES;
    }
    if([bundle objectForInfoDictionaryKey:@"Eclipse"]) {
        return YES;
    }
    if([bundle objectForInfoDictionaryKey:@"JVMOptions"]) {
        return YES;
    }
    return NO;
}

+ (BlacklistApps*)sharedManager {
    if (!sharedData_) {
        sharedData_ = [[BlacklistApps alloc] init];
    }
    return sharedData_;
}
@end
