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

#ifndef calculator_h
#define calculator_h

#include <sstream>
#include <stdexcept>
#include <cmath>

/*

expression	= term { ('+' | '-') term }
    		;

term		= primary { ('*' | '/' | '%' ) primary }
    		;

primary		= [ '+' | '-' ] number | '(' expression ')'
		;

number		=  floating-point-literal
		;

*/

namespace calculator {
    struct token {
        token(char k = 0, double v = 0) : kind(k), value(v) {}

        char kind;
        double value;
    };

    class engine {
        std::istringstream input_;
        token buf_;

        token get_token() {
            token result;

            if(buf_.kind != 0) {
                result = buf_;
                buf_.kind = 0;
                return result;
            }

            if(input_ >> result.kind) {
                switch(result.kind) {
                case '(': case ')': case '+': case '-': case '*': case '/': case '%':
                    return result;

                case '.':
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                    input_.putback(result.kind);
                    input_ >> result.value;
                    result.kind = '#';

                    return result;

                default:
                    throw std::runtime_error("計算エラー:不正な文字です"); 
                }
            }

            return result;      // EOF
        }

        void save_token(const token& token) {
            buf_ = token;
        }

        double expression() {
            double left = term();
            token token = get_token();

            while(true) {
                switch(token.kind) {
                case '+':
                    left += term();
                    break;

                case '-':
                    left -= term();
                    break;

                default:
                    save_token(token);
                    return left;
                }

                token = get_token();
            }
        }

        double term() {
            double left = primary();
            token token = get_token();

            while(true) {
                switch(token.kind) {
                case '*':
                    left *= primary();
                    break;

                case '/':
                    if(double divisor = primary()) {
                        left /= divisor;
                    } else {
                        throw std::logic_error("計算エラー:ゼロ除算です");
                    }
                    break;

                case '%':
                    left = fmod(left, primary());
                    break;

                default:
                    save_token(token);
                    return left;
                }

                token = get_token();
            }
        }

        double primary() {
            token token = get_token();

            // group
            if(token.kind == '(') {
                double val = expression();

                token = get_token();
                if(token.kind != ')') {
                    throw std::logic_error("計算エラー:')'が見つかりませんでした");
                }

                return val;
            }

            if(token.kind == '#') {
                return token.value;
            }

            if(token.kind == '-') {
                return - primary();
            }

            if(token.kind == '+') {
                return primary();
            }

            throw std::runtime_error("計算エラー:数値が見つかりませんでした");
        }

    public:
        double run(const std::string& input) {
            input_.clear();
            input_.str(input);
            buf_.kind = 0;

            return expression();
        }
    };
}

#endif
