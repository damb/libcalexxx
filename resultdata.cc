/*! \file resultdata.cc
 * \brief Implementation of class to store calex results for a liboptimizexx
 * parameter space.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 15/03/2012
 * 
 * Purpose: Implementation of class to store calex results for a liboptimizexx
 * parameter space.
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
 * 15/03/2012  V0.1  Daniel Armbruster
 * 
 * ============================================================================
 */
 
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <calexxx/resultdata.h>
#include <calexxx/error.h>

namespace calex
{
  /*-------------------------------------------------------------------------*/
  CalexResult::CalexResult(unsigned int const iter, double const rms,
      double const amp, double const del) : Mcomputed(true), Miter(iter),
      Mrms(rms), Mamp(amp), Mdel(del)
  { }

  /*-------------------------------------------------------------------------*/
  std::map<std::string, double> const&
    CalexResult::get_systemParameters() const
  {
    return MsystemParameters;
  }

  /*-------------------------------------------------------------------------*/
  void CalexResult::writeLine(std::ostream& os) const
  {
    std::ostringstream oss;
    oss << std::right << std::setw(5) << Miter
      << std::setw(12) << std::right << std::fixed << Mrms
      << std::setw(12) << std::right << std::fixed << Mamp
      << std::setw(12) << std::right << std::fixed << Mdel;
    for (TsystemParameters::const_iterator cit(MsystemParameters.begin());
        cit != MsystemParameters.end(); ++cit)
    {
      oss << std::setw(12) << std::right << std::fixed << cit->second;
    }
    os << oss.str() << std::endl;
  } // function CalexResult::writeLine

  /*-------------------------------------------------------------------------*/
  void CalexResult::read(std::istream& is)
  {
    std::string line;
    while (getline(is >> std::ws, line))
    {
      if (line.substr(0,line.find(":")) == "final system parameters")
      {
        getline(is >> std::ws, line);
        // collect names of result data
        std::vector<std::string> names;
        {
          std::istringstream iss(line); 
          copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(),
            std::back_inserter<std::vector<std::string> >(names));
        }

        getline(is >> std::ws, line);
        // collect result data
        std::vector<double> vals;
        {
          std::istringstream iss(line); 
          copy(std::istream_iterator<double>(iss),
            std::istream_iterator<double>(),
            std::back_inserter<std::vector<double> >(vals));
        }

        CALEX_assert(vals.size() == names.size(),
            "Error while reading result data.");
        Miter = vals[0];
        Mrms = vals[1];
        Mamp = vals[2];
        Mdel = vals[3];

        for (size_t i=4; i < vals.size(); ++i)
        {
          MsystemParameters[names[i]] = vals[i];
        }
        break;
      }
    }
    Mcomputed = true;
  } // function CalexResult::read

  /*-------------------------------------------------------------------------*/
  void CalexResult::write(std::ostream& os) const
  {
    os << " final system parameters:\n" << std::endl
      << std::setw(5) << std::right << "iter"
      << std::setw(12) << std::right << "RMS"
      << std::setw(12) << std::right << "amp"
      << std::setw(12) << std::right << "del";
    for (TsystemParameters::const_iterator cit(MsystemParameters.begin());
        cit != MsystemParameters.end(); ++cit)
    {
      os << std::setw(12) << std::right << cit->first;
    }
    os << std::endl;

    std::stringstream ss;
    ss << std::right << std::setw(5) << Miter
      << std::setw(12) << std::right << std::fixed << Mrms
      << std::setw(12) << std::right << std::fixed << Mamp
      << std::setw(12) << std::right << std::fixed << Mdel;
    for (TsystemParameters::const_iterator cit(MsystemParameters.begin());
        cit != MsystemParameters.end(); ++cit)
    {
      ss << std::setw(12) << std::right << std::fixed << cit->second;
    }
    os << ss.str() << std::endl;
  } // function CalexResult::write

  /*-------------------------------------------------------------------------*/
  std::istream& operator>>(std::istream& is, CalexResult& result)
  {
    result.read(is);
    return is;
  }

  /*-------------------------------------------------------------------------*/
  std::ostream& operator<<(std::ostream& os, CalexResult const& result)
  {
    result.write(os);
    return os;
  }

  /*-------------------------------------------------------------------------*/

} // namespace calex

/* ----- END OF resultdata.cc  ----- */
