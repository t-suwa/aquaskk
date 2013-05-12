/* -*- C++ -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2007 Tomotaka SUWA <t.suwa@mac.com>

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

#ifndef SKKDictionaryFactory_h
#define SKKDictionaryFactory_h

#include <string>
#include <map>
#include "SKKDictionaryKey.h"

class SKKBaseDictionary;

class SKKDictionaryFactory {
    typedef SKKBaseDictionary* (*SKKDictionaryCreator)(const std::string& location);

    std::map<int, SKKDictionaryCreator> creators_;

    SKKDictionaryFactory();
    SKKDictionaryFactory(const SKKDictionaryFactory&);
    SKKDictionaryFactory& operator=(const SKKDictionaryFactory&);

public:
    static SKKDictionaryFactory& theInstance();

    void Register(int type, SKKDictionaryCreator creator);
    SKKBaseDictionary* Create(int type, const std::string& location);
};

// ======================================================================
// ファクトリメソッド登録用ユーティリティ
// ======================================================================
// SKKBaseDictionary を継承した具象辞書クラスの実装ファイルに bool 静的
// 変数を宣言し、SKKRegisterFactoryMethod で初期化することでファクトリ
// メソッドが登録される。
//
// 例)
// static bool initialized = SKKRegisterFactoryMethod<ClassName>(ClassTypeID);
//
// 具象辞書クラスをライブラリ化する場合には、明示的なインスタンス生成等
// でオブジェクトファイルを強制的にロードする必要がある。
//
template <typename ConcreteDictionary>
bool SKKRegisterFactoryMethod(int type) {
    struct Factory {
	static SKKBaseDictionary* Method(const std::string& location) {
	    ConcreteDictionary* obj = new ConcreteDictionary();
	    obj->Initialize(location);
	    return obj;
	}
    };

    SKKDictionaryFactory::theInstance().Register(type, Factory::Method);

    return true;
}

#endif
