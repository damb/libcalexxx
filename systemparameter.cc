/*! \file systemparameter.cc
 * \brief Implementation of a calex system parameter.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 16/03/2012
 * 
 * Purpose: Implementation of a calex system parameter
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
 * 16/03/2012  V0.1  Daniel Armbruster
 * 
 * ============================================================================
 */
 
#include <sstream>
#include <iomanip>
#include <calexxx/systemparameter.h>
#include <calexxx/error.h>

namespace calex
{
  
  /*-------------------------------------------------------------------------*/
  SystemParameter::SystemParameter(std::string const nam, double const val,
      double const unc) : Mnam(nam.substr(0,3)), Mval(val), Munc(unc),
      Mknown(true)
  { 
    CALEX_assert(Mnam.size() == 3, "Illegal length of name.");
  }

  /*-------------------------------------------------------------------------*/
  SystemParameter::SystemParameter(std::string const nam, double const unc)
      : Mnam(nam.substr(0,3)), Munc(unc), Mknown(false)
  { 
    CALEX_assert(Mnam.size() == 3, "Illegal length of name.");
  }

  /*-------------------------------------------------------------------------*/
  double const& SystemParameter::get_val() const 
  { 
    CALEX_assert(Mknown, "Illegal to call for unknown system parameters");
    return Mval; 
  }

  /*-------------------------------------------------------------------------*/
  std::ostream& operator<<(std::ostream& os, SystemParameter const& param)
  {
    param.write(os);
    return os;
  }

  /*-------------------------------------------------------------------------*/
  void SystemParameter::write(std::ostream& os) const
  {
    std::stringstream ss;
    ss << Mnam << "  " << std::setw(12) << std::right << std::fixed << Mval
      << "  " << std::setw(12) << std::right << std::fixed << Munc;
    os << ss.str() << std::endl;
  }
  /*-------------------------------------------------------------------------*/

} // namespace calex

/* ----- END OF systemparameter.cc  ----- */
