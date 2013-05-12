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

#ifndef SKKRecursiveEditor_h
#define SKKRecursiveEditor_h

#include "SKKInputEnvironment.h"
#include "SKKInputEngine.h"
#include "SKKStateMachine.h"
#include <memory>

class SKKWidget;
class SKKInputContext;
class SKKConfig;
class SKKAnnotator;
class SKKDynamicCompletor;

class SKKRecursiveEditor {
    std::auto_ptr<SKKInputEnvironment> env_;
    SKKInputContext* context_;
    SKKConfig* config_;
    SKKAnnotator* annotator_;
    SKKDynamicCompletor* completor_;
    SKKInputEngine editor_;
    SKKStateMachine state_;
    std::vector<SKKWidget*> widgets_;

    typedef void (SKKWidget::*WidgetMethod)();
    void forEachWidget(WidgetMethod method);
    void complete();
    void annotate();

    SKKRecursiveEditor();
    SKKRecursiveEditor(const SKKRecursiveEditor&);
    SKKRecursiveEditor& operator=(const SKKRecursiveEditor&);

public:
    SKKRecursiveEditor(SKKInputEnvironment* env);
    ~SKKRecursiveEditor();

    void Input(const SKKEvent& event);
    void Output();

    void Activate();
    void Deactivate();

    bool IsChildOf(SKKStateMachine::Handler handler);
};

#endif
