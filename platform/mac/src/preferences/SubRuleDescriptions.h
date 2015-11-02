/* -*- C++ -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2009 Tomotaka SUWA <tomotaka.suwa@gmail.com>

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

#ifndef SubRuleDescriptions_h
#define SubRuleDescriptions_h

#include <string>
#include <map>

class SubRuleDescriptions {
    std::map<std::string, std::string> description_;
    std::map<std::string, std::string> keymap_;

    void add(const std::string& line);

public:
    SubRuleDescriptions(const char* folder);

    const char* Description(const char* rule_path);
    const char* Keymap(const char* rule_path);
};

#endif
