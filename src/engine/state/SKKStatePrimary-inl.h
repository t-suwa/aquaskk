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

// ======================================================================
// level 1：直接入力
// ======================================================================
State SKKState::Primary(const Event& event) {
    switch(event) {
    case INIT_EVENT:
	return State::Initial(&SKKState::KanaInput);

    case ENTRY_EVENT:
        editor_->SetStatePrimary();
        return 0;

    case SKK_JMODE:
        editor_->Commit();
        return 0;

    case SKK_ENTER:
	editor_->HandleEnter();
	return 0;

    case SKK_CANCEL:
	editor_->HandleCancel();
	return 0;

    case SKK_UNDO:
        // Undo 可能なら見出し語入力に遷移する
        switch(context_->undo.Undo()) {
        case SKKUndoContext::UndoKanaEntry:
            return State::Transition(&SKKState::KanaEntry);

        case SKKUndoContext::UndoAsciiEntry:
            return State::Transition(&SKKState::AsciiEntry);

        default:
            messenger_->SendMessage("Undo できませんでした");
            context_->event_handled = false;
            break;
        }

        return 0;

    case SKK_PASTE:
	editor_->HandlePaste();
	return 0;

    case SKK_PING:
        editor_->HandlePing();
        return 0;

    case SKK_BACKSPACE:
        editor_->HandleBackSpace();
        return 0;

    case SKK_DELETE:
        editor_->HandleDelete();
        return 0;

    case SKK_LEFT:
        editor_->HandleCursorLeft();
        return 0;

    case SKK_RIGHT:
        editor_->HandleCursorRight();
        return 0;

    case SKK_UP:
        editor_->HandleCursorUp();
        return 0;

    case SKK_DOWN:
        editor_->HandleCursorDown();
        return 0;

    case SKK_ASCII_MODE:
        return State::Transition(&SKKState::Ascii);

    case SKK_HIRAKANA_MODE:
        return State::Transition(&SKKState::Hirakana);

    case SKK_KATAKANA_MODE:
        return State::Transition(&SKKState::Katakana);

    case SKK_JISX0201KANA_MODE:
        return State::Transition(&SKKState::Jisx0201Kana);

    case SKK_JISX0208LATIN_MODE:
        return State::Transition(&SKKState::Jisx0208Latin);

    default:
        // editor で処理されなかったイベントは全て「未処理」にする
        // SKK_TAB もここに来るため、SKK_CHAR でテストはできない
        if(event.IsUser()) {
            editor_->Reset();
            return 0;
        }
    }

    return &SKKState::TopState;
}

// ======================================================================
// level 2 (sub of Primary)：かな入力
// ======================================================================
State SKKState::KanaInput(const Event& event) {
    const SKKEvent& param = event.Param();

    switch(event) {
    case INIT_EVENT:
	return State::ShallowHistory(&SKKState::Hirakana);

    case EXIT_EVENT:
	return State::SaveHistory();

    case SKK_CHAR:
	if(!editor_->CanConvert(param.code)) {
	    if(param.IsSwitchToAscii()) {
		return State::Transition(&SKKState::Ascii);
	    }

	    if(param.IsSwitchToJisx0208Latin()) {
		return State::Transition(&SKKState::Jisx0208Latin);
	    }

	    if(param.IsEnterAbbrev()) {
		return State::Transition(&SKKState::AsciiEntry);
	    }

	    if(param.IsEnterJapanese()) {
		return State::Transition(&SKKState::KanaEntry);
	    }
	}

        if(param.IsStickyKey()) {
            return State::Transition(&SKKState::KanaEntry);
        }
	if(param.IsUpperCases()) {
	    return State::Forward(&SKKState::KanaEntry);
	}

        // キー修飾がない場合のみローマ字かな変換を実施する
        if(param.IsInputChars()) {
            editor_->HandleChar(param.code, param.IsDirect());
            return 0;
        }
    }

    return &SKKState::Primary;
}

// ======================================================================
// level 3 (sub of KanaInput)：ひらかな
// ======================================================================
State SKKState::Hirakana(const Event& event) {
    const SKKEvent& param = event.Param();

    switch(event) {
    case ENTRY_EVENT:
        editor_->SelectInputMode(HirakanaInputMode);
	return 0;

    case SKK_HIRAKANA_MODE:
        return 0;

    case SKK_CHAR:
        if(!(param.IsInputChars() && editor_->CanConvert(param.code))) {
            // 変換する文字がない場合のみ、ToggleKana等の処理する
            //
            // 例: AZIKの場合
            //
            //   - [: ToggeKana
            //   - x[: 鍵括弧
            //
            // が割り当てられている
            if(param.IsToggleKana()) {
                return State::Transition(&SKKState::Katakana);
            }

            if(param.IsToggleJisx0201Kana()) {
              return State::Transition(&SKKState::Jisx0201Kana);
            }
        }
    }

    return &SKKState::KanaInput;
}

// ======================================================================
// level 3 (sub of KanaInput)：カタカナ
// ======================================================================
State SKKState::Katakana(const Event& event) {
    const SKKEvent& param = event.Param();

    switch(event) {
    case ENTRY_EVENT:
        editor_->SelectInputMode(KatakanaInputMode);
	return 0;

    case SKK_KATAKANA_MODE:
        return 0;

    default:
        if(!(event == SKK_CHAR && param.IsInputChars() && editor_->CanConvert(param.code))) {
            // 変換する文字がない場合のみ、ToggleKana等の処理する
            if(event == SKK_JMODE || param.IsToggleKana()) {
                return State::Transition(&SKKState::Hirakana);
            }

            if(param.IsToggleJisx0201Kana()) {
                return State::Transition(&SKKState::Jisx0201Kana);
            }
        }
    }

    return &SKKState::KanaInput;
}

// ======================================================================
// level 3 (sub of KanaInput)：半角カタカナ
// ======================================================================
State SKKState::Jisx0201Kana(const Event& event) {
    const SKKEvent& param = event.Param();

    switch(event) {
    case ENTRY_EVENT:
        editor_->SelectInputMode(Jisx0201KanaInputMode);
	return 0;

    case SKK_JISX0201KANA_MODE:
        return 0;

    default:
        if(!(event == SKK_CHAR && param.IsInputChars() && editor_->CanConvert(param.code))) {
            // 変換する文字がない場合のみ、ToggleKana等の処理する
            if(event == SKK_JMODE ||
                param.IsToggleKana() || param.IsToggleJisx0201Kana()) {
              return State::Transition(&SKKState::Hirakana);
            }
        }
    }

    return &SKKState::KanaInput;
}

// ======================================================================
// level 2 (sub of Primary)：Latin 入力
// ======================================================================
State SKKState::LatinInput(const Event& event) {
    SKKEvent param(event.Param());

    switch(event) {
    case SKK_JMODE:
	return State::Transition(&SKKState::Hirakana);

    case SKK_CHAR:
        if(param.IsInputChars()) {
            if(param.option & CapsLock) {
                param.code = std::toupper(param.code);
            }
            editor_->HandleChar(param.code, param.IsDirect());
            return 0;
        }
    }

    return &SKKState::Primary;
}

// ======================================================================
// level 2 (sub of LatinInput)：ASCII
// ======================================================================
State SKKState::Ascii(const Event& event) {
    switch(event) {
    case ENTRY_EVENT:
        editor_->SelectInputMode(AsciiInputMode);
	return 0;

    case SKK_ASCII_MODE:
        return 0;
    }

    return &SKKState::LatinInput;
}

// ======================================================================
// level 2 (sub of LatinInput)：全角英数
// ======================================================================
State SKKState::Jisx0208Latin(const Event& event) {
    const SKKEvent& param = event.Param();

    switch(event) {
    case ENTRY_EVENT:
        editor_->SelectInputMode(Jisx0208LatinInputMode);
	return 0;

    case SKK_JISX0208LATIN_MODE:
        return 0;

    default:
        if(event == SKK_ASCII_MODE ||
           (!param.IsInputChars() && param.IsSwitchToAscii())) {
           return State::Transition(&SKKState::Ascii);
        }
    }

    return &SKKState::LatinInput;
}
