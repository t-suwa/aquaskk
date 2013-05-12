/* -*- C++ -*-
 *
 * stringutil.h - string utilities
 *
 *   Copyright (c) 2009 Tomotaka SUWA, All rights reserved.
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

#ifndef	stringutil_h
#define stringutil_h

#include <sstream>
#include <numeric>

namespace string {
    // 置換ユーティリティ
    static void translate(std::string& str, const std::string& from, const std::string& to) {
        std::string::size_type pos = 0;

        if(from == to) return;

        while((pos = str.find(from, pos)) != std::string::npos) {
            str.replace(pos, from.size(), to);
        }
    }

    template <typename T>
    static std::string join(T& sequence, const std::string& delimiter = " ") {
        std::string result;

        for(typename T::iterator iter = sequence.begin(); iter != sequence.end(); ++ iter) {
            result += *iter;
            result += delimiter;
        }

        result.erase(result.size() - 1);

        return result;
    }

    // 分割ユーティリティ
    class splitter {
        std::istringstream buf_;

    public:
        void split(const std::string& target, const std::string& delimiter = ",") {
            std::string str(target);

            translate(str, delimiter, " ");

            buf_.clear();
            buf_.str(str);
        }

        template <typename T>
        splitter& operator>>(T& value) {
            buf_ >> value;
            return *this;
        }

        operator bool() {
            return (bool)buf_;
        }
    };

};

#endif
