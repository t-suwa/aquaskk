/* -*- C++ -*-

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

#ifndef SKKConfig_h
#define SKKConfig_h

class SKKConfig {
public:
    virtual ~SKKConfig() {}

    // 中間的な変換を訂正するか(n → ん)
    virtual bool FixIntermediateConversion() = 0;

    // 自動ダイナミック補完を有効にするか
    virtual bool EnableDynamicCompletion() = 0;

    // 自動ダイナミック補完の幅(表示数)
    virtual int DynamicCompletionRange() = 0;

    // アノテーションを有効にするか
    virtual bool EnableAnnotation() = 0;

    // 最小マッチしたかな変換を表示するか(n → ん)
    virtual bool DisplayShortestMatchOfKanaConversions() = 0;

    // 確定時に改行を抑制(skk-egg-like-newline)
    virtual bool SuppressNewlineOnCommit() = 0;

    // インライン表示する変換候補の最大数
    virtual int MaxCountOfInlineCandidates() = 0;

    // 見出し語再入を送りの開始とみなすか
    virtual bool HandleRecursiveEntryAsOkuri() = 0;

    // 後退を確定とみなすか(skk-delete-implies-kakutei)
    virtual bool InlineBackSpaceImpliesCommit() = 0;

    // 送りキャンセル時に送り仮名を削除(skk-delete-okuri-when-quit)
    virtual bool DeleteOkuriWhenQuit() = 0;
};

#endif
