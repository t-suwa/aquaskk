// -*- C++ -*-

/*
 * jconv.h - japanese character code conversion library
 *
 *   Copyright (c) 2006 Tomotaka SUWA, All rights reserved.
 * 
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 * 
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of the authors nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *   Copyright (c) 2000-2003 Shiro Kawai, All rights reserved.
 * 
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 * 
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of the authors nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef jconv_h
#define jconv_h

#include <string>
#include <vector>
#include <stdexcept>

namespace jconv {
    const unsigned char subst_char = '?';

    // ======================================================================
    // substitute functors
    // ======================================================================
    class subst_eucj {
	std::string& output_;

    public:
	subst_eucj(std::string& dest) : output_(dest) {}
	void operator()() {
	    output_ += 0xa2;
	    output_ += 0xae;
	}
    };

    class subst_sjis {
	std::string& output_;

    public:
	subst_sjis(std::string& dest) : output_(dest) {}
	void operator()() {
	    output_ += 0x81;
	    output_ += 0xac;
	}
    };

    class subst_utf8 {
	std::string& output_;

    public:
	subst_utf8(std::string& dest) : output_(dest) {}
	void operator()() {
	    output_ += 0xe3;
	    output_ += 0x80;
	    output_ += 0x93;
	}
    };

    class subst_iso2022jp {
	std::string& output_;

    public:
	subst_iso2022jp(std::string& dest) : output_(dest) {}
	void operator()() {
	    output_ += 0x02;
	    output_ += 0x0e;
	}
    };

#if 0
    // ======================================================================
    // Abstract Converter(NVI pattern)
    // ======================================================================
    class abstract_converter {
	virtual void invoke(unsigned char c) = 0;
	virtual std::string& output() = 0;

    public:
	void operator()(unsigned char c) { invoke(c); }
	std::string& result() { return output(); }
    };
#endif

    // ======================================================================
    // Shift-JIS -> EUC_JP
    // ======================================================================
    class sjis_to_eucj {
	typedef void (sjis_to_eucj::*handler)(unsigned char);

	std::string pool_;
	std::string& output_;
	subst_eucj subst_;
	std::vector<unsigned char> input_;
	handler state_;

	// handler
	void neutral(unsigned char c);
	void jisx0213(unsigned char c);

	void reset() { state_ = &sjis_to_eucj::neutral; }

    public:
	sjis_to_eucj() : output_(pool_), subst_(output_) { reset(); }
	sjis_to_eucj(std::string& dest) : output_(dest), subst_(output_) { reset(); }

	void operator()(unsigned char c) { (this->*state_)(c); }
	std::string& result() { return output_; }
    };

    // ======================================================================
    // EUC_JP -> Shift-JIS
    // ======================================================================
    class eucj_to_sjis {
	typedef void (eucj_to_sjis::*handler)(unsigned char);

	std::string pool_;
	std::string& output_;
	subst_sjis subst_;
	std::vector<unsigned char> input_;
	handler state_;

	// handler
	void neutral(unsigned char c);
	void jisx0201_kana(unsigned char c);
	void jisx0213_plane1(unsigned char c);
	void jisx0213_plane2(unsigned char c);

	void reset() { state_ = &eucj_to_sjis::neutral; }

    public:
	eucj_to_sjis() : output_(pool_), subst_(output_) { reset(); }
	eucj_to_sjis(std::string& dest) : output_(dest), subst_(output_) { reset(); }

	void operator()(unsigned char c) { (this->*state_)(c); }
	std::string& result() { return output_; }
    };

    // ======================================================================
    // UTF8 -> EUC_JP
    // ======================================================================
    class utf8_to_eucj {
	typedef void (utf8_to_eucj::*handler)(unsigned char);

	std::string pool_;
	std::string& output_;
	subst_eucj subst_;
	std::vector<unsigned char> input_;
	bool nothrow_;
	handler state_;

	void emit(unsigned short euc);

	// handler
	void neutral(unsigned char c);
	void sequence_of_2_bytes(unsigned char c);
	void sequence_of_3_bytes(unsigned char c);
	void sequence_of_4_bytes(unsigned char c);
	void sequence_of_5_bytes(unsigned char c);
	void sequence_of_6_bytes(unsigned char c);

	void reset() { state_ = &utf8_to_eucj::neutral; }

	void dead_end(const char* msg) { if(!nothrow_) throw std::runtime_error(msg); }

    public:
	utf8_to_eucj(bool nothrow = true) : output_(pool_), subst_(output_), nothrow_(nothrow) {
	    reset();
	}
	utf8_to_eucj(std::string& dest, bool nothrow = true) : output_(dest), subst_(output_), nothrow_(nothrow) {
	    reset();
	}

	void operator()(unsigned char c) { (this->*state_)(c); }
	std::string& result() { return output_; }
    };

    // ======================================================================
    // EUC_JP -> UTF8
    // ======================================================================
    class eucj_to_utf8 {
	typedef void (eucj_to_utf8::*handler)(unsigned char);

	std::string pool_;
	std::string& output_;
	subst_utf8 subst_;
	std::vector<unsigned char> input_;
	bool nothrow_;
	handler state_;

	void emit(unsigned int ucs);

	// handler
	void neutral(unsigned char c);
	void jisx0201_kana(unsigned char c);
	void jisx0213_plane1(unsigned char c);
	void jisx0213_plane2(unsigned char c);

	void reset() { state_ = &eucj_to_utf8::neutral; }

	void dead_end(const char* msg) { if(!nothrow_) throw std::runtime_error(msg); }

    public:
	eucj_to_utf8(bool nothrow = true) : output_(pool_), subst_(output_), nothrow_(nothrow) {
	    reset();
	}
	eucj_to_utf8(std::string& dest, bool nothrow = true) : output_(dest), subst_(output_), nothrow_(nothrow) {
	    reset();
	}

	void operator()(unsigned char c) { (this->*state_)(c); }
	std::string& result() { return output_; }
    };

    // ======================================================================
    // ISO-2022-JP -> EUC_JP
    // ======================================================================
    class iso2022jp_to_eucj {
	typedef void (iso2022jp_to_eucj::*handler)(unsigned char);

	std::string pool_;
	std::string& output_;
	std::vector<unsigned char> input_;
	bool nothrow_;
	handler state_;
	handler sub_state_;

	// main handler
	void neutral(unsigned char c);
	void escape_sequence(unsigned char c);

	// sub handler(invoked from neutral)
	void us_ascii(unsigned char c);
	void jisx0201_roman(unsigned char c);
	void jisx0201_kana(unsigned char c);
	void jisx0208_1978(unsigned char c);
	void jisx0212(unsigned char c);
	void jisx0213_plane1(unsigned char c);
	void jisx0213_plane2(unsigned char c);
	void unknown(unsigned char c);

	void reset() { state_ = &iso2022jp_to_eucj::neutral; }

	void dead_end(const char* msg) { if(!nothrow_) throw std::runtime_error(msg); }

    public:
	iso2022jp_to_eucj(bool nothrow = true) : output_(pool_), nothrow_(nothrow) {
	    reset();
	    sub_state_ = &iso2022jp_to_eucj::us_ascii;
	}
	iso2022jp_to_eucj(std::string& dest, bool nothrow = true) : output_(dest), nothrow_(nothrow) {
	    reset();
	    sub_state_ = &iso2022jp_to_eucj::us_ascii;
	}

	void operator()(unsigned char c) { (this->*state_)(c); }
	std::string& result() { return output_; }
    };

    // ======================================================================
    // EUC_JP -> ISO-2022-JP
    // ======================================================================
    class eucj_to_iso2022jp {
	typedef void (eucj_to_iso2022jp::*handler)(unsigned char);

	std::string pool_;
	std::string& output_;
	std::vector<unsigned char> input_;
	bool nothrow_;
	handler state_;
	handler sub_state_;

	// main handler
	void neutral(unsigned char c);
	void guess(unsigned char c);

	// sub handler
	void us_ascii(unsigned char c);
	void jisx0201_kana(unsigned char c);
	void jisx0212(unsigned char c);
	void jisx0213_plane1(unsigned char c);
	void jisx0213_plane2(unsigned char c);

	void reset() { state_ = &eucj_to_iso2022jp::neutral; }

	void dead_end(const char* msg) { if(!nothrow_) throw std::runtime_error(msg); }

    public:
	eucj_to_iso2022jp(bool nothrow = true) : output_(pool_), nothrow_(nothrow), sub_state_(0) {
	    reset();
	}
	eucj_to_iso2022jp(std::string& dest, bool nothrow = true) : output_(dest), nothrow_(nothrow), sub_state_(0) {
	    reset();
	}

	void operator()(unsigned char c) { (this->*state_)(c); }
	std::string& result() { return output_; }
    };

    // utility
    void convert_utf8_to_eucj(const std::string& from, std::string& to);
    void convert_eucj_to_utf8(const std::string& from, std::string& to);

    std::string utf8_from_eucj(const std::string& eucj);
    std::string eucj_from_utf8(const std::string& utf8);

    // translator(UTF-8 only)
    void hirakana_to_katakana(const std::string& from, std::string& to);
    void hirakana_to_jisx0201_kana(const std::string& from, std::string& to);
    void hirakana_to_roman(const std::string& from, std::string& to);

    void katakana_to_hirakana(const std::string& from, std::string& to);
    void katakana_to_jisx0201_kana(const std::string& from, std::string& to);

    void jisx0201_kana_to_hirakana(const std::string& from, std::string& to);
    void jisx0201_kana_to_katakana(const std::string& from, std::string& to);

    void ascii_to_jisx0208_latin(const std::string& from, std::string& to);
    void jisx0208_latin_to_ascii(const std::string& from, std::string& to);
} // namespace jconv

#endif // INC__jconv__
