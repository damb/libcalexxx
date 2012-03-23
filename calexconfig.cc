/*! \file calexconfig.cc
 * \brief Implementation of a generator class for a calex parameter file.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 14/03/2012
 * 
 * Purpose: Implementation of a generator class for a calex parameter file.  
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
 * 14/03/2012  V0.1  Daniel Armbruster
 * 
 * ============================================================================
 */
 
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <calexxx/calexconfig.h>
#include <calexxx/defaults.h>
#include <calexxx/error.h>

namespace calex
{
  /*=========================================================================*/
  CalexConfig::CalexConfig(std::string const infile,
      std::string const outfile) : Mcomment(""), Minfile(infile),
      Moutfile(outfile), Malias(CALEX_ALIAS), Mm(0), Mm0(CALEX_M0), Mm1(0),
      Mm2(0), Mmaxit(CALEX_MAXIT), Mqac(CALEX_QAC), Mfinac(CALEX_FINAC),
      Mns1(CALEX_NS1), Mns2(CALEX_NS2), Mamp("amp", CALEX_AMP, CALEX_AMPUNC),
      Mdel("del", CALEX_DEL, CALEX_DELUNC), 
      Msub("sub", CALEX_SUB, CALEX_SUBUNC),
      Mtil("til", CALEX_TIL, CALEX_TILUNC)
  {
    if (0 != Mamp.get_unc()) { ++Mm; }
    if (0 != Mdel.get_unc()) { ++Mm; }
    CALEX_assert(! (0 != Mdel.get_val() && 0 != Msub.get_val()),
        "Inconsistent system parameter configuration");
    if (0 != Msub.get_unc()) { ++Mm; }
    if (0 != Mtil.get_unc()) { ++Mm; }
  } // constructor CalexConfig::CalexConfig

  /*-------------------------------------------------------------------------*/
  void CalexConfig::set_amp(SystemParameter const amp)
  { 
    CALEX_assert("amp" == amp.get_nam(), "Illegal system parameter name");
    Mamp = amp; 
    if (! amp.is_known())
    {
      MunknownSystemParameters.push_back(const_cast<SystemParameter*>(&amp));
    }
  }

  /*-------------------------------------------------------------------------*/
  void CalexConfig::set_del(SystemParameter const del)
  { 
    CALEX_assert("del" == del.get_nam(), "Illegal system parameter name");
    // reset 'sub' system parameter
    Msub = SystemParameter("sub", 0., 0.);
    Mdel = del; 
    if (! del.is_known())
    {
      MunknownSystemParameters.push_back(const_cast<SystemParameter*>(&del));
    }
  }

  /*-------------------------------------------------------------------------*/
  void CalexConfig::set_sub(SystemParameter const sub)
  { 
    CALEX_assert("sub" == sub.get_nam(), "Illegal system parameter name");
    // reset 'del' system parameter
    Mdel = SystemParameter("del", 0., 0.);
    Msub = sub; 
    if (! sub.is_known())
    {
      MunknownSystemParameters.push_back(const_cast<SystemParameter*>(&sub));
    }
  }

  /*-------------------------------------------------------------------------*/
  void CalexConfig::set_til(SystemParameter const til)
  { 
    CALEX_assert("til" == til.get_nam(), "Illegal system parameter name");
    Mtil = til; 
    if (! til.is_known())
    {
      MunknownSystemParameters.push_back(const_cast<SystemParameter*>(&til));
    }
  }

  /*-------------------------------------------------------------------------*/
  void CalexConfig::add_systemParameters(SystemParameter const& param)
  {
    std::string nam[] = {"amp", "del", "sub", "til"};
    std::vector<std::string> names(nam, nam+sizeof(nam)/sizeof(std::string));
    std::vector<std::string>::iterator it(
        find(names.begin(), names.end(), param.get_nam()));
    CALEX_assert(names.end() != it, "Illegal parameter added.");
    MsystemParameters.push_back(param);
  }

  /*-------------------------------------------------------------------------*/
  void CalexConfig::add_subsystem(CalexSubsystem* subsys)
  { 
    if (1 == subsys->get_order()) 
    { 
      ++Mm1;
      if (0 != subsys->get_per().get_unc())
      {
        ++Mm;
      }
      if (! subsys->get_per().is_known())
      {
        MunknownSystemParameters.push_back(const_cast<SystemParameter*>(
              &subsys->get_per()));
      }
    } else
    if (2 == subsys->get_order())
    { 
      ++Mm2;
      if (0 != subsys->get_per().get_unc())
      {
        ++Mm;
      }
      if (! subsys->get_per().is_known())
      {
        MunknownSystemParameters.push_back(const_cast<SystemParameter*>(
              &subsys->get_per()));
      }
      if (0 != subsys->get_dmp().get_unc())
      {
        ++Mm;
      }
      if (! subsys->get_dmp().is_known())
      {
        MunknownSystemParameters.push_back(const_cast<SystemParameter*>(
              &subsys->get_dmp()));
      }
    }
    else { CALEX_abort("Illegal subsystem type."); }

    MsubSystems.push_back(subsys);
  } // function CalexConfig::add_subsystem

  /*-------------------------------------------------------------------------*/
  void CalexConfig::clear_subsystems()
  {
    for (std::vector<CalexSubsystem*>::const_iterator cit(MsubSystems.begin());
          cit != MsubSystems.end(); ++cit)
    {
      // \todo Do not forget to remove the system parameter pointers of the
      // parameter MunknownSystemParameter vector. 
      if (2 == (*cit)->get_order()) { Mm = Mm-2; } else
      if (1 == (*cit)->get_order()) { --Mm; }
      else
      {
        CALEX_abort("Illegal order of subsystem.");
      }
    }
    Mm1 = 0;
    Mm2 = 0;
    MsubSystems.clear();
  } // function CalexConfig::clear_subsystems

  /*-------------------------------------------------------------------------*/
  void CalexConfig::remove_subsystem(CalexSubsystem* subsys)
  {
    // \todo Do not forget to remove the system parameter pointers of the
    // parameter MunknownSystemParameter vector. 
    std::vector<CalexSubsystem*>::iterator it(
        find(MsubSystems.begin(), MsubSystems.end(), subsys));
    if (it != MsubSystems.end())
    {
      if (2 == (*it)->get_order()) { Mm = Mm-2; --Mm2; }
      if (1 == (*it)->get_order()) { --Mm; --Mm1; }
      MsubSystems.erase(it);
    }
  } // function CalexConfig::remove_subsystem

  /*-------------------------------------------------------------------------*/
  std::vector<CalexSubsystem*> const& CalexConfig::get_subsystems() const
  {
    return MsubSystems;
  }

  /*-------------------------------------------------------------------------*/
  std::ostream& operator<<(std::ostream& os, CalexConfig const& config)
  {
    config.write(os);
    return os;
  }

  /*-------------------------------------------------------------------------*/
  void CalexConfig::write(std::ostream& os) const
  {
    os << Mcomment << "\n" << std::endl;
    os << "'" << Minfile << "'  input to seismo (file name)" << std::endl 
      << "'" << Moutfile << "'  output from seismo (file name)\n" << std::endl;
    std::stringstream ss;
    ss.precision(5);
    ss << std::setw(6) << std::left << std::fixed << Malias << " alias" 
      << std::endl
      << std::setw(6) << std::left << Mm << "m" << std::endl
      << std::setw(6) << std::left << Mm0 << "m0" << std::endl
      << std::setw(6) << std::left << Mm1 << "m1" << std::endl
      << std::setw(6) << std::left << Mm2 << "m2" << std::endl
      << std::setw(6) << std::left << Mmaxit << "maxit" << std::endl
      << std::setw(6) << std::left << std::scientific << Mqac << " qac"
      << std::endl
      << std::setw(6) << std::left << std::scientific << Mfinac << " finac" 
      << std::endl
      << std::setw(6) << std::left << Mns1 << "ns1" << std::endl
      << std::setw(6) << std::left << Mns2 << "ns2" << std::endl;
    os << ss.str() << std::endl;
    os << Mamp << Mdel << Msub << Mtil << std::endl;
    for (std::vector<SystemParameter>::const_iterator cit(
          MsystemParameters.begin()); cit != MsystemParameters.end(); ++cit)
    {
      os << *cit;
    }
    std::cout << std::endl;
    // write first order subsystems
    for (std::vector<CalexSubsystem*>::const_iterator cit(MsubSystems.begin());
        cit != MsubSystems.end(); ++cit)
    {
      if (1 == (*cit)->get_order()) { os << **cit << std::endl; }
    }
    // write second order subsystems
    for (std::vector<CalexSubsystem*>::const_iterator cit(MsubSystems.begin());
        cit != MsubSystems.end(); ++cit)
    {
      if (2 == (*cit)->get_order()) { os << **cit << std::endl; }
    }
    os << std::endl << "end" << std::endl;
  } // function CalexConfig::write

  /*-------------------------------------------------------------------------*/

} // namespace calex


/* ----- END OF calexconfig.cc  ----- */
