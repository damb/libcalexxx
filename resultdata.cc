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
 * 15/03/2012   V0.1    Daniel Armbruster
 * 15/05/2012   V0.2    provide member function to write header information to
 *                      an outputstream
 * 14/06/2012   V0.3    Bug fix parsing a calex *.out file - amp and del system
 *                      parameters from now on are deprecated
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
  std::vector<std::pair<std::string, double>> const&
    CalexResult::get_systemParameters() const
  {
    return MsystemParameters;
  }

  /*-------------------------------------------------------------------------*/
  void CalexResult::writeLine(std::ostream& os) const
  {
    std::ostringstream oss;
    oss << std::right << std::setw(5) << Miter
      << std::setw(12) << std::right << std::fixed << Mrms;
#if ((__GNUC__ <= 4) && (__GNUC_MINOR__ < 6))
    for (auto cit(MsystemParameters.cbegin()); cit != MsystemParameters.cend();
      ++cit)
    {
      oss << std::setw(12) << std::right << std::fixed << cit->second;
    }
#else
    for (auto x : MsystemParameters)
    {
      oss << std::setw(12) << std::right << std::fixed << x.second;
    }
#endif
    os << oss.str() << std::endl;
  } // function CalexResult::writeLine

  /*-------------------------------------------------------------------------*/
  void CalexResult::writeHeaderInfo(std::ostream& os) const
  {
    os << std::setw(5) << std::right << "iter"
      << std::setw(12) << std::right << "RMS";
#if ((__GNUC__ <= 4) && (__GNUC_MINOR__ < 6))
    for (auto cit(MsystemParameters.cbegin()); cit != MsystemParameters.cend();
      ++cit)
    {
      os << std::setw(12) << std::right << std::fixed << cit->first;
    }
#else
    for (auto x : MsystemParameters)
    {
      os << std::setw(12) << std::right << std::fixed << x.first;
    }
#endif
    os << std::endl;
  } // function CalexResult::writeHeaderInfo

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
            std::back_inserter<std::vector<std::string>>(names));
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

        CALEX_assert(vals.size() == names.size() && vals.size() >= 2,
            "Error while reading result data.");
        Miter = vals[0];
        Mrms = vals[1];

        // add remaining result parameters
        for (size_t i=2; i < vals.size(); ++i)
        {
          MsystemParameters.push_back(std::make_pair(names[i], vals[i]));
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
      << std::setw(12) << std::right << "RMS";
#if ((__GNUC__ <= 4) && (__GNUC_MINOR__ < 6))
    for (auto cit(MsystemParameters.cbegin()); cit != MsystemParameters.cend();
      ++cit)
    {
      os << std::setw(12) << std::right << cit->first;
    }
#else
    for (auto x : MsystemParameters)
    {
      os << std::setw(12) << std::right << x.first;
    }
#endif
    os << std::endl;

    std::stringstream ss;
    ss << std::right << std::setw(5) << Miter
      << std::setw(12) << std::right << std::fixed << Mrms;
#if ((__GNUC__ <= 4) && (__GNUC_MINOR__ < 6))
    for (auto cit(MsystemParameters.cbegin()); cit != MsystemParameters.cend();
      ++cit)
    {
      ss << std::setw(12) << std::right << std::fixed << cit->second;
    }
#else
    for (auto x : MsystemParameters)
    {
      ss << std::setw(12) << std::right << std::fixed << x.second;
    }
#endif
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
