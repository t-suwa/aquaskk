/* -*- C++ -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2002 phonohawk
  Copyright (C) 2005-2010 Tomotaka SUWA <tomotaka.suwa@gmail.com>

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
/*
    Directory Maneger対応　2002.09.25 Shin_ichi Abe.
*/

#include <iostream>
#include <vector>
#include <Carbon/Carbon.h>
#include "SKKCandidate.h"
#include "MacKotoeriDictionary.h"

#if MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_6
// ======================================================================
// ことえり辞書実装クラス
// ======================================================================
class KotoeriImpl {
    CFStringRef path_;
    bool isRegistered_;
    DCMDictionaryID id_;

    CFStringRef CFStringWithUTF8(const char* utf8) {
	CFMutableStringRef cfstr = CFStringCreateMutable(0, 0);
	CFStringAppendCString(cfstr, utf8, kCFStringEncodingUTF8);

	CFStringRef result;
	CFIndex len = CFStringGetLength(cfstr);
	CFIndex bufsize = len * sizeof(UInt16);
	UInt16* buf = new UInt16[len];

	CFStringGetBytes(cfstr, CFRangeMake(0, len), kCFStringEncodingUnicode, 0, false,
			 reinterpret_cast<UInt8*>(buf), bufsize, NULL);

	CFRelease(cfstr);

	for(int i = 0; i < len; ++ i) {
	    buf[i] = CFSwapInt16HostToBig(buf[i]);
	}

	result = CFStringCreateWithBytes(0, reinterpret_cast<UInt8*>(buf), bufsize, kCFStringEncodingUnicode, false);

	delete[] buf;
	
	return result;
    }

    std::string UTF8WithCFString(CFStringRef cfstr) {
	int length = CFStringGetLength(cfstr);

	if(length == 0) return "";

	int bufsize = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
	char* buf = new char[bufsize];
	memset(buf, 0, bufsize);
	CFStringGetCString(cfstr, buf, bufsize, kCFStringEncodingUTF8);

	std::string result(buf, strlen(buf));

	delete[] buf;

	return result;
    }

    bool find(const std::string& key, DCMFoundRecordIterator* iterator) {
	DCMDictionaryRef ref;
	if(DCMOpenDictionary(id_, 0, NULL, &ref) != noErr) {
	    std::cout << "DCMOpenDictionary() failed" << std::endl;
	    return false;
	}

	// レコードを検索
	OSStatus status;
	DCMFieldTag	dataFieldTagList[] = { kDCMJapaneseHyokiTag };
	CFStringRef keydata = CFStringWithUTF8(key.c_str());

	status = DCMFindRecords(ref, // Dictionary reference
				kDCMJapaneseYomiTag,      // key field tag
				CFStringGetLength(keydata) * sizeof(UniChar), // key data length
				CFStringGetCharactersPtr(keydata), // key data
				kDCMFindMethodExactMatch, // find method
				1,		      // number of data field
				dataFieldTagList, // data field tag list
				0, 0,	      // search all records
				iterator);	      // found result

	DCMCloseDictionary(ref);
	CFRelease(keydata);

	return (status == noErr);
    }

public:
    ~KotoeriImpl() {
	if(isRegistered_) {
	    DCMUnregisterDictionary(id_);
	}

	if(path_) {
	    CFRelease(path_);
	}
    }

    void Initialize(const std::string& location) {
	path_ = CFStringCreateWithCString(kCFAllocatorDefault, location.c_str(), kCFStringEncodingUTF8);
	if(!path_) {
	    std::cout << "KotoeriImpl: CFStringCreateCopy() failed" << std::endl;
	    return;
	}

	FSSpec spec;
	FSRef fileRef;
	CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, path_, kCFURLPOSIXPathStyle, false);
	OSErr err = !noErr;
	if(CFURLGetFSRef(url, &fileRef)) {
	    err = FSGetCatalogInfo(&fileRef, kFSCatInfoNone, NULL, NULL, &spec, NULL);
	}
	if(url) CFRelease(url);
	if(err != noErr) {
	    return;
	}

	if(DCMGetDictionaryIDFromFile(&spec, &id_) != noErr) {
	    std::cout << "DCMGetDictionaryIDFromFile() failed" << std::endl;
	    if(DCMRegisterDictionaryFile(&spec, &id_) != noErr) {
		std::cout << "DCMRegisterDictionaryFile() failed" << std::endl;
		return;
	    }
	    isRegistered_ = true;
	}
    }

    void Find(const std::string& str, SKKCandidateSuite& result) {
	OSStatus status;

	DCMFoundRecordIterator iterator;
	if(!find(str, &iterator)) {
	    return;
	}

	while(true) {
	    ByteCount keySize;
	    char foundKeyStr[kMaxKanjiLengthInAppleJapaneseDictionary];
	    DCMUniqueID uniqueID;
	    AERecord dataList;

	    // Get one record from result list
	    status = DCMIterateFoundRecord(iterator, // found result
					   kMaxKanjiLengthInAppleJapaneseDictionary, // key buffer size
					   &keySize, // actual found key size
					   foundKeyStr, // found key data
					   &uniqueID,   // ユニークID
					   &dataList);  // AERecordデータ

	    if(status != noErr) break;

	    DescType actualType;
	    UInt8 dataBuffer[kMaxKanjiLengthInAppleJapaneseDictionary];
	    Size actualSize;

	    // Get one data from AERecord
	    status = AEGetParamPtr(&dataList, 
				   kDCMJapaneseHyokiTag,
				   typeUnicodeText,
				   &actualType,
				   dataBuffer,
				   kMaxKanjiLengthInAppleJapaneseDictionary,
				   &actualSize);

	    // Dispose data AERecord
	    AEDisposeDesc(&dataList);

	    if(status != noErr) break;

	    CFStringRef entry = CFStringCreateWithBytes(0, dataBuffer, actualSize, kCFStringEncodingUnicode, 0);
	    std::string tmp = UTF8WithCFString(entry);
	    CFRelease(entry);

            result.Add(SKKCandidate(tmp, false));
	}
	DCMDisposeRecordIterator(iterator);
    }
};
#else
// Snow Leopard 以降では Dictionary Manager は非サポート
class KotoeriImpl {
public:
    void Initialize(const std::string&) {}
    void Find(const std::string&, SKKCandidateSuite&) {}
};
#endif

// ======================================================================
// ことえり辞書インタフェース
// ======================================================================
MacKotoeriDictionary::MacKotoeriDictionary() : impl_(new KotoeriImpl()) {}

MacKotoeriDictionary::~MacKotoeriDictionary() {
    // auto_ptr<KotoeriImpl> のデストラクタが実体化されるタイミングを遅
    // 延させるために、空のデストラクタ実装が必要
}

void MacKotoeriDictionary::Initialize(const std::string& location) {
    impl_->Initialize(location);
}

void MacKotoeriDictionary::Find(const SKKEntry& entry, SKKCandidateSuite& result) {
    if(!entry.IsOkuriAri()) {
        impl_->Find(entry.EntryString(), result);
    }
}
