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

#include "SubRuleDescriptions.h"
#include <fstream>
#include <sstream>
#include <iostream>

SubRuleDescriptions::SubRuleDescriptions(const char* folder) {
    std::string path(std::string(folder) + "/sub-rule.desc");
    std::ifstream ifs(path.c_str());
    std::string line;

    while(std::getline(ifs, line)) {
        add(line);
    }
}

const char* SubRuleDescriptions::Description(const char* rule_path) {
    if(table_.find(rule_path) != table_.end()) {
        return table_[rule_path].c_str();
    }

    return rule_path;
}

// ----------------------------------------------------------------------

void SubRuleDescriptions::add(const std::string& line) {
    if(line.empty() || line[0] == '#') return;

    std::istringstream buf(line);
    std::string path;
    std::string description;

    if(buf >> path >> description) {
        table_[path] = description;
    }
}
