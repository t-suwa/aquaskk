/* -*- C++ -*-

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

#ifndef SKKInputSession_h
#define SKKInputSession_h

#include "SKKInputModeListener.h"
#include "SKKInputContext.h"
#include "SKKStateMachine.h"
#include <vector>

class SKKInputSessionParameter;
class SKKRecursiveEditor;
class SKKBaseEditor;
class SKKConfig;
class SKKEvent;

class SKKInputSession {
    std::auto_ptr<SKKInputSessionParameter> param_;
    std::vector<SKKRecursiveEditor*> stack_;
    SKKInputModeListenerCollection listeners_;
    SKKInputContext context_;
    bool inEvent_;
    int eventId_;

    void beginEvent();
    void endEvent();
    bool result(const SKKEvent& event);

    SKKRecursiveEditor* top();
    SKKRecursiveEditor* createEditor(SKKBaseEditor* bottom);
    void popEditor();

    SKKInputSession();
    SKKInputSession(const SKKInputSession&);
    SKKInputSession& operator=(const SKKInputSession&);

public:
    SKKInputSession(SKKInputSessionParameter* param);
    ~SKKInputSession();

    void AddInputModeListener(SKKInputModeListener* listener);

    bool HandleEvent(const SKKEvent& event);
    void Commit();
    void Clear();

    void Activate();
    void Deactivate();

    bool IsChildOf(SKKStateMachine::Handler handler);
};

#endif
