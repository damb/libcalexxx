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
 * 14/03/2012   V0.1    Daniel Armbruster
 * ============================================================================
 */
 
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <calexxx/calexconfig.h>
#include <calexxx/error.h>

namespace calex
{
  /*=========================================================================*/
  CalexConfig::CalexConfig(std::string const infile, std::string const outfile)
      : Mcomment(""), Minfile(infile), Moutfile(outfile), Malias(CALEX_ALIAS),
      Mm(0), Mm0(CALEX_M0), Mm1(0), Mm2(0), Mmaxit(CALEX_MAXIT),
      Mqac(CALEX_QAC), Mfinac(CALEX_FINAC), Mns1(CALEX_NS1), Mns2(CALEX_NS2),
      Mamp(new SystemParameter("amp",0.,0.)),
      Mdel(new SystemParameter("del",0.,0.)), 
      Msub(new SystemParameter("sub",0.,0.)),
      Mtil(new SystemParameter("til",0.,0.)), 
      MisSynchronized(false)
  { }

  /*-------------------------------------------------------------------------*/
  void CalexConfig::set_amp(std::shared_ptr<SystemParameter> amp)
  {
    if ((! Mamp || !Mamp->is_active()) && amp->is_active()) { ++Mm; }
    Mamp = amp; 
  }

  /*-------------------------------------------------------------------------*/
  void CalexConfig::set_del(std::shared_ptr<SystemParameter> del)
  {
    if (Msub)
    {
      CALEX_assert((0 == Msub->get_val() && 0 == Msub->get_unc()) ||
          (0 == del->get_val() && 0 == del->get_unc()),
          "Assignment not valid.");
    }
    if ((! Mdel || !Mdel->is_active()) && del->is_active()) { ++Mm; }
    Mdel = del; 
  }

  /*-------------------------------------------------------------------------*/
  void CalexConfig::set_sub(std::shared_ptr<SystemParameter> sub)
  {
    if (Mdel)
    {
      CALEX_assert((0 == Mdel->get_val() && 0 == Mdel->get_unc()) ||
          (0 == sub->get_val() && 0 == sub->get_unc()),
          "Assignment not valid.");
    }
    if ((! Msub || !Msub->is_active()) && sub->is_active()) { ++Mm; }
    Msub = sub; 
    std::cout << *Msub << std::endl;
  }

  /*-------------------------------------------------------------------------*/
  void CalexConfig::set_til(std::shared_ptr<SystemParameter> til)
  {
    if ((! Mtil || !Mtil->is_active()) && til->is_active()) { ++Mm; }
    Mtil = til;
  }

  /*-------------------------------------------------------------------------*/
  void CalexConfig::add_systemParameter(std::shared_ptr<SystemParameter> param)
  {
    std::string nam[] = {"amp", "del", "sub", "til"};
    std::vector<std::string> names(nam, nam+sizeof(nam)/sizeof(std::string));
    std::vector<std::string>::iterator it(
        find(names.begin(), names.end(), param->get_nam()));
    CALEX_assert(names.end() != it, "Illegal parameter added.");
    MsystemParameters.push_back(param);
  }

  /*-------------------------------------------------------------------------*/
  void CalexConfig::add_subsystem(std::shared_ptr<CalexSubsystem> subsys)
  { 
    if (0 != subsys->get_per()->get_unc()) { ++Mm; }
    if (subsys->get_per()->is_gridSystemParameter())
    { MisSynchronized = false; }
    if (1 == subsys->get_order()) { ++Mm1; } else
    if (2 == subsys->get_order())
    { 
      ++Mm2;
      if (0 != subsys->get_dmp()->get_unc()) { ++Mm; }
      if (subsys->get_dmp()->is_gridSystemParameter())
      { MisSynchronized = false; }
    }
    else { CALEX_abort("Illegal subsystem type."); }

    MsubSystems.push_back(subsys);
  } // function CalexConfig::add_subsystem

  /*-------------------------------------------------------------------------*/
  void CalexConfig::clear_subsystems()
  {
    for (auto it(MsubSystems.begin()); it != MsubSystems.end(); ++it)
    {
      if (0 != (*it)->get_per()->get_unc()) { --Mm; }
      if ((*it)->get_per()->is_gridSystemParameter()) 
      { MisSynchronized = false; }
      if (2 == (*it)->get_order() && 0 != (*it)->get_dmp()->get_unc())  
      { 
        if ((*it)->get_dmp()->is_gridSystemParameter()) 
        { 
          MisSynchronized = false;
        }
        --Mm;
      } else
      {
        CALEX_abort("Illegal order of subsystem.");
      }
    }
    Mm1 = 0;
    Mm2 = 0;
    MsubSystems.clear();
  } // function CalexConfig::clear_subsystems

  /*-------------------------------------------------------------------------*/
  void CalexConfig::remove_subsystem(std::shared_ptr<CalexSubsystem> subsys)
  {
    auto it(find(MsubSystems.begin(), MsubSystems.end(), subsys));
    if (it != MsubSystems.end())
    {
      if (0 != (*it)->get_per()->get_unc()) { --Mm; }
      if (subsys->get_per()->is_gridSystemParameter())
      { MisSynchronized = false; }
      if (2 == (*it)->get_order())
      {
        if (0 != (*it)->get_dmp()->get_unc()) { --Mm; }
        if (subsys->get_per()->is_gridSystemParameter())
        { 
          MisSynchronized = false;
        }
        --Mm2;
      }
      if (1 == (*it)->get_order()) { --Mm1; }
      MsubSystems.erase(it);
    }
  } // function CalexConfig::remove_subsystem

  /*-------------------------------------------------------------------------*/
  std::vector<std::shared_ptr<CalexSubsystem>> const&
    CalexConfig::get_subsystems() const
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
    os << *Mamp << *Mdel << *Msub << *Mtil << std::endl;
    for (auto cit(MsystemParameters.cbegin()); cit != MsystemParameters.cend();
        ++cit)
    {
      os << **cit;
    }
    std::cout << std::endl;
    // write first order subsystems
    for (auto cit(MsubSystems.begin()); cit != MsubSystems.end(); ++cit)
    {
      if (1 == (*cit)->get_order()) { os << **cit << std::endl; }
    }
    // write second order subsystems
    for (auto cit(MsubSystems.begin()); cit != MsubSystems.end(); ++cit)
    {
      if (2 == (*cit)->get_order()) { os << **cit << std::endl; }
    }
    os << "end" << std::endl;
  } // function CalexConfig::write

  /*-------------------------------------------------------------------------*/
  void CalexConfig::get_gridSystemParameters(
      std::vector<std::shared_ptr<GridSystemParameter>>& param_vec)
  {
    CALEX_assert(Mamp && Mdel && Msub && Mtil,
        "System parameters not assigned.");
    if (Mamp->is_gridSystemParameter())
    {
      param_vec.push_back(std::dynamic_pointer_cast<GridSystemParameter>(Mamp));
    }
    if (Mdel->is_gridSystemParameter())
    {
      param_vec.push_back(std::dynamic_pointer_cast<GridSystemParameter>(Mdel));
    }
    if (Msub->is_gridSystemParameter())
    {
      param_vec.push_back(std::dynamic_pointer_cast<GridSystemParameter>(Msub));
    }
    if (Mtil->is_gridSystemParameter())
    {
      param_vec.push_back(std::dynamic_pointer_cast<GridSystemParameter>(Mtil));
    }
    for (auto it(MsystemParameters.begin()); it != MsystemParameters.end();
        ++it)
    {
      if ((*it)->is_gridSystemParameter())
      {
        param_vec.push_back(
            std::dynamic_pointer_cast<GridSystemParameter>(*it));
      }
    }
    for (auto it(MsubSystems.begin()); it != MsubSystems.end(); ++it)
    {
      if ((*it)->get_per()->is_gridSystemParameter())
      {
        // ATTENTION -> get_per should return a shared pointer
        param_vec.push_back(
            std::dynamic_pointer_cast<GridSystemParameter>((*it)->get_per()));
      }
      if (2 == (*it)->get_order())
      {
        if ((*it)->get_dmp()->is_gridSystemParameter())
        {
        // ATTENTION -> get_dmp should return a shared pointer
        param_vec.push_back(
            std::dynamic_pointer_cast<GridSystemParameter>((*it)->get_dmp()));
        }
      }
    }
  } // function CalexConfig::get_gridSystemParameters

  /*-------------------------------------------------------------------------*/
  bool CalexConfig::hasGridSystemParameters()
  {
    std::vector<std::shared_ptr<GridSystemParameter>> grid_params; 
    get_gridSystemParameters(grid_params);
    return 0 != grid_params.size();
  } // function CalexConfig::hasGridSystemParameters

  /*-------------------------------------------------------------------------*/

} // namespace calex


/* ----- END OF calexconfig.cc  ----- */
