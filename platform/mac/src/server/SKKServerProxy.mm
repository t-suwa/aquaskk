/* -*- ObjC -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2008 Tomotaka SUWA <t.suwa@mac.com>

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

#include "SKKServerProxy.h"

@implementation SKKServerProxy

- (id)init {
    self = [super init];

    proxy_ = [[NSConnection
                  rootProxyForConnectionWithRegisteredName:SKKSupervisorConnectionName
                  host:nil] retain];
    [proxy_ setProtocolForProxy:@protocol(SKKSupervisor)];

    return self;
}

- (void)dealloc {
    [proxy_ release];
    [super dealloc];
}

- (void)reloadBlacklistApps {
    [proxy_ reloadBlacklistApps];
}

- (void)reloadUserDefaults {
    [proxy_ reloadUserDefaults];
}

- (void)reloadDictionarySet {
    [proxy_ reloadDictionarySet];
}

- (void)reloadComponents {
    [proxy_ reloadComponents];
}

- (NSArray*)createDictionaryTypes {
    return [proxy_ createDictionaryTypes];
}

@end
