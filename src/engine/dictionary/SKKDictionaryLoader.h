/* -*- C++ -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2008-2009 Tomotaka SUWA <tomotaka.suwa@gmail.com>

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

#ifndef SKKDictionaryLoader_h
#define SKKDictionaryLoader_h

#include "SKKDictionaryFile.h"
#include "pthreadutil.h"

class SKKDictionaryLoaderObserver {
public:
    virtual ~SKKDictionaryLoaderObserver() {}

    virtual void SKKDictionaryLoaderUpdate(const SKKDictionaryFile& file) = 0;
};

class SKKDictionaryLoader : public pthread::task {
    bool first_;
    SKKDictionaryLoaderObserver* observer_;
    SKKDictionaryFile file_;

    virtual bool run() {
        if(first_) {
            first_ = false;
            notify();
        }
        
        if(NeedsUpdate()) {
            notify();
        } else {
            // 不要な pthread_cond_wait を回避するため、空のファイルを
            // 通知する
            if(file_.IsEmpty()) {
                observer_->SKKDictionaryLoaderUpdate(file_);
            }
        }

        return true;
    }

    void notify() {
        if(file_.Load(FilePath())) {
            file_.Sort();
            observer_->SKKDictionaryLoaderUpdate(file_);
        }
    }

public:
    SKKDictionaryLoader() : first_(true), observer_(0) {}

    void Connect(SKKDictionaryLoaderObserver* observer) {
	observer_ = observer;
    }

    // 初期化
    virtual void Initialize(const std::string& location) = 0;

    // チェック間隔
    virtual int Interval() const = 0;

    // タイムアウト
    virtual int Timeout() const = 0;

    // 更新が必要かどうか
    virtual bool NeedsUpdate() = 0;

    // ファイルパス
    virtual const std::string& FilePath() const = 0;
};

#endif
