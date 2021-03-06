/*! \file subsystem.cc
 * \brief Implementation of calex subsystem class.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 15/03/2012
 * 
 * Purpose: Implementation of calex subsystem class.  
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
 * 15/03/2012   V0.1  Daniel Armbruster
 * 25/04/2012   V0.2  Make use of smart poiters and C++0x.
 * 
 * ============================================================================
 */
 
#include <iomanip>
#include <memory>
#include <calexxx/subsystem.h>
#include <calexxx/error.h>

namespace calex
{

  /*=========================================================================*/
  std::ostream& operator<<(std::ostream& os, CalexSubsystem const& subsys)
  {
    if (LP == subsys.get_type()) 
    { 
      os << "lp";
    } else
    if (HP == subsys.get_type())
    { 
      os << "hp"; 
    } else
    if (BP == subsys.get_type())
    { 
      os << "bp"; 
    } 
    else { CALEX_abort("Illegal subsystem type."); }
    os << subsys.get_order() << std::endl;

    subsys.write(os);
    return os;
  }

  /*-------------------------------------------------------------------------*/
  CalexSubsystem::CalexSubsystem(EsubSystemType type,
      std::shared_ptr<SystemParameter> per) : Mtype(type), Mper(per)
  { }

  /*=========================================================================*/
  FirstOrderSubsystem::FirstOrderSubsystem(EsubSystemType type,
      std::shared_ptr<SystemParameter> per) : Tbase(type, per)
  { 
     CALEX_assert(LP == Tbase::Mtype || HP == Tbase::Mtype, 
         "Illegal subsystem type."); 
  }

  /*-------------------------------------------------------------------------*/
  void FirstOrderSubsystem::write(std::ostream& os) const
  {
    os << *Tbase::Mper;
  }

  /*=========================================================================*/
  SecondOrderSubsystem::SecondOrderSubsystem(EsubSystemType type,
      std::shared_ptr<SystemParameter> per,
      std::shared_ptr<SystemParameter> dmp) : Tbase(type, per), Mdmp(dmp)
  { }

  /*-------------------------------------------------------------------------*/
  void SecondOrderSubsystem::write(std::ostream& os) const
  {
    os << *Tbase::Mper;
    os << *Mdmp;
  }

  /*-------------------------------------------------------------------------*/

} // namespace calex

/* ----- END OF subsystem.cc  ----- */
