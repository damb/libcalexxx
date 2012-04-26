/*! \file parser.cc
 * \brief Module which provides system parameter commandline parser functions
 * (Implementation).
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 27/03/2012
 * 
 * Purpose: Module which provides system parameter commandline parser functions 
 * (Implementation).  
 *
 * ----
 * This file is part of libcalexxx.
 *
 * libcalexxx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libcalexxx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libcalexxx.  If not, see <http://www.gnu.org/licenses/>.
 * ----
 * 
 * Copyright (c) 2012 by Daniel Armbruster
 * 
 * REVISIONS and CHANGES 
 * 27/03/2012   V0.1  Daniel Armbruster
 * 25/04/2012   V0.2  Make use of smart poiters and C++0x.
 * 
 * ============================================================================
 */

#include <iostream>
#include <sstream>
#include <algorithm>
#include <memory>
#include <calexxx/parser.h>
#include <calexxx/error.h>

namespace opt = optimize;

namespace calex
{
  namespace parser
  {
    /* --------------------------------------------------------------------- */
    namespace 
    {

      int findSecondSeparatorPosition(std::string const& str, char sep)
      {
        if (std::string::npos != str.find(sep))
        {
          std::string tmp(str.substr(str.find(sep)+1));
          if (std::string::npos != tmp.find(sep))
            return tmp.find(sep)+str.size()-tmp.size();
        }
        return -1;
      } // function getSecondSeparator

    } // namespace (unnamed)

    /* --------------------------------------------------------------------- */
    std::shared_ptr<SystemParameter> systemParameterParser(
        std::string const& str, std::string const nam)
    {
      if (nam.size() != 0)
      {
        CALEX_assert(nam.size() == 3, "Invalid length of 'nam' field.");
      }
      std::string id(nam);
      double val, unc, start, end, delta;
      char c;
      std::istringstream iss;
      // detect if system parameter will be treated additionally as a grid
      // system parameter
      std::shared_ptr<SystemParameter> ret_ptr;
      if (std::string::npos != str.find(";"))
      {
        // check if obligatory system parameter
        if (0 == id.size())
        {
          id = str.substr(0,str.find("|"));
          iss.str(str.substr(str.find("|")+1));
        } else
        {
          iss.str(str);
        }
        iss >> std::fixed >> start >> c >> end >> c >> delta >> c >> unc;
        ret_ptr.reset(new GridSystemParameter(id, unc, id, start, end, delta));
      } else
      {
        // check if obligatory system parameter
        if (0 == id.size())
        {
          id = str.substr(0,str.find("|"));
          iss.str(str.substr(str.find("|")+1));
        } else
        {
          iss.str(str);
        }
        iss >> val >> c >> unc;
        ret_ptr.reset(new SystemParameter(id, val, unc));
      }
      return ret_ptr;
    } // function systemParameterParser

    /* --------------------------------------------------------------------- */
    std::shared_ptr<CalexSubsystem> firstOrderParser(std::string const& str)
    {
      std::string type(str.substr(0,2));
      std::shared_ptr<SystemParameter> per_ptr(
          systemParameterParser(str.substr(3), "per"));

      std::shared_ptr<CalexSubsystem> ret_ptr;
      if ("LP" == type)
      {
         ret_ptr.reset(new FirstOrderSubsystem(LP, per_ptr));
      } else
      if ("HP" == type)
      {
         ret_ptr.reset(new FirstOrderSubsystem(HP, per_ptr));
      } else { CALEX_abort("Illegal subsystem type."); }

      return ret_ptr;
    } // function firstOrderParser

    /* --------------------------------------------------------------------- */
    std::shared_ptr<CalexSubsystem> secondOrderParser(std::string const& str)
    {
      std::string type(str.substr(0,2));
      std::string param_str(str.substr(str.find("|")+1,
            findSecondSeparatorPosition(str)-1));
      std::shared_ptr<SystemParameter> per_ptr(
          systemParameterParser(str.substr(3), "per"));
      param_str = str.substr(type.size()+1+param_str.size()+1);
      std::shared_ptr<SystemParameter> dmp_ptr(
          systemParameterParser(str.substr(3), "dmp"));

      std::shared_ptr<CalexSubsystem> ret_ptr;
      if ("LP" == type)
      {
        ret_ptr.reset(new SecondOrderSubsystem(LP, per_ptr, dmp_ptr));
      } else
      if ("HP" == type)
      {
        ret_ptr.reset(new SecondOrderSubsystem(HP, per_ptr, dmp_ptr));
      } else
      if ("BP" == type)
      {
        ret_ptr.reset(new SecondOrderSubsystem(BP, per_ptr, dmp_ptr));
      } else { CALEX_abort("Illegal subsystem type."); }

      return ret_ptr;
    } // function secondOrderParser

    /* --------------------------------------------------------------------- */

  } // namespace parser

} // namespace calex
 
/* ----- END OF parser.cc  ----- */
