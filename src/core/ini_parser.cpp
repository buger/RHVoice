/* Copyright (C) 2012  Olga Yakovleva <yakovleva.o.v@gmail.com> */

/* This program is free software: you can redistribute it and/or modify */
/* it under the terms of the GNU Lesser General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or */
/* (at your option) any later version. */

/* This program is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/* GNU Lesser General Public License for more details. */

/* You should have received a copy of the GNU Lesser General Public License */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include "core/io.hpp"
#include "core/str.hpp"
#include "core/ini_parser.hpp"

namespace RHVoice
{
  ini_parser::ini_parser(const std::string& file_path):
    instream(new std::ifstream)
  {
    io::open_ifstream(*instream,file_path);
    next();
  }

  void ini_parser::next()
  {
    if(instream.get()==0)
      return;
    std::string line;
    while(std::getline(*instream,line))
      {
        if(line.empty())
          continue;
        if(!utf8::is_valid(line.begin(),line.end()))
          continue;
        line=str::trim(line);
        if (line.empty())
          continue;
        if(str::startswith(line,";"))
          continue;
        if(str::startswith(line,"[")&&str::endswith(line,"]"))
          {
            section=str::trim(line.substr(1,line.length()-2));
            if(section.empty())
              break;
          }
        else
          {
            std::size_t pos=line.find('=');
            if((pos==std::string::npos)||(pos==0)||(pos==(line.length()-1)))
              continue;
            key=str::trim(line.substr(0,pos));
            if(key.empty())
              continue;
            value=str::trim(line.substr(pos+1,line.length()-pos-1));
            if(value.empty())
              continue;
            return;
          }
      }
    instream.reset();
  }
}