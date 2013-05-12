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

#include "SKKRecursiveEditor.h"
#include "SKKInputSessionParameter.h"
#include "SKKInputContext.h"
#include "SKKConfig.h"
#include "SKKAnnotator.h"
#include "SKKCandidateWindow.h"
#include "SKKDynamicCompletor.h"
#include "SKKBackEnd.h"
#include "utf8util.h"

SKKRecursiveEditor::SKKRecursiveEditor(SKKInputEnvironment* env)
    : env_(env)
    , context_(env->InputContext())
    , config_(env->Config())
    , annotator_(env->InputSessionParameter()->Annotator())
    , completor_(env->InputSessionParameter()->DynamicCompletor())
    , editor_(env)
    , state_(SKKState(env, &editor_)) {
    // initialize widgets
    widgets_.push_back(annotator_);
    widgets_.push_back(completor_);
    widgets_.push_back(env->InputSessionParameter()->CandidateWindow());
    widgets_.push_back(env->InputModeSelector());
}

SKKRecursiveEditor::~SKKRecursiveEditor() {
    forEachWidget(&SKKWidget::Hide);
}

void SKKRecursiveEditor::Input(const SKKEvent& event) {
    state_.Dispatch(SKKStateMachine::Event(event.id, event));
}

void SKKRecursiveEditor::Output() {
    editor_.UpdateInputContext();

    context_->output.Output();

    complete();
    annotate();
}

void SKKRecursiveEditor::Activate() {
    forEachWidget(&SKKWidget::Activate);
}

void SKKRecursiveEditor::Deactivate() {
    forEachWidget(&SKKWidget::Deactivate);
}

bool SKKRecursiveEditor::IsChildOf(SKKStateMachine::Handler handler) {
    return state_.IsChildOf(handler);
}

// ----------------------------------------------------------------------

void SKKRecursiveEditor::forEachWidget(WidgetMethod method) {
    std::for_each(widgets_.begin(), widgets_.end(), std::mem_fun(method));
}

void SKKRecursiveEditor::complete() {
    if(context_->dynamic_completion && config_->EnableDynamicCompletion()) {
        SKKEntry entry = context_->entry;
        std::string joined;
        std::string common_prefix;

        if(!entry.IsEmpty() && !entry.IsOkuriAri()) {
            std::vector<std::string> result;
            unsigned range = config_->DynamicCompletionRange();
            std::string key = entry.EntryString();

            if(range && SKKBackEnd::theInstance().Complete(key, result, range)) {
                int limit = std::min((unsigned)result.size(), range);
                common_prefix = result[0];

                for(int i = 0; i < limit; ++ i) {
                    common_prefix = utf8::common_prefix(common_prefix, result[i]);
            
                    joined += result[i];
                    joined += "\n";
                }

                joined.erase(joined.size() - 1);
            }
        }

        completor_->Update(joined, utf8::length(common_prefix), context_->output.GetMark());
        completor_->Show();
    } else {
        completor_->Hide();
    }
}

void SKKRecursiveEditor::annotate() {
    if(context_->annotation && config_->EnableAnnotation()) {
        SKKCandidate candidate = context_->candidate;

        annotator_->Update(candidate, context_->output.GetMark());
        annotator_->Show();
    } else {
        annotator_->Hide();
    }
}
