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

#include "MacInputSessionParameter.h"
#include "MacConfig.h"
#include "MacFrontEnd.h"
#include "MacMessenger.h"
#include "MacClipboard.h"
#include "MacCandidateWindow.h"
#include "MacAnnotator.h"
#include "MacDynamicCompletor.h"

MacInputSessionParameter::MacInputSessionParameter(id client, SKKLayoutManager* layout)
    : config_(new MacConfig())
    , frontend_(new MacFrontEnd(client))
    , messenger_(new MacMessenger(layout))
    , clipboard_(new MacClipboard())
    , candidateWindow_(new MacCandidateWindow(layout))
    , annotator_(new MacAnnotator(layout))
    , completor_(new MacDynamicCompletor(layout))
{}

SKKConfig* MacInputSessionParameter::Config() {
    return config_.get();
}

SKKFrontEnd* MacInputSessionParameter::FrontEnd() {
    return frontend_.get();
}

SKKMessenger* MacInputSessionParameter::Messenger() {
    return messenger_.get();
}

SKKClipboard* MacInputSessionParameter::Clipboard() {
    return clipboard_.get();
}

SKKCandidateWindow* MacInputSessionParameter::CandidateWindow() {
    return candidateWindow_.get();
}

SKKAnnotator* MacInputSessionParameter::Annotator() {
    return annotator_.get();
}

SKKDynamicCompletor* MacInputSessionParameter::DynamicCompletor() {
    return completor_.get();
}
