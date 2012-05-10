/*! \file calexParamTest.cc
 * \brief Generate an exemplary calex STS-2 parameter file.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 20/03/2012
 * 
 * Purpose: Generate an exemplary calex STS-2 parameter file.  
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
 * 20/03/2012  V0.1  Daniel Armbruster
 * 26/04/2012  V0.2  Make use of C++0x smart pointers.
 * 
 * ============================================================================
 */
 
#include <iostream>
#include <memory>
#include <calexxx/calexconfig.h>
#include <calexxx/subsystem.h>
#include <calexxx/systemparameter.h>

int main(int iargc, char* argv[])
{
  // create instance of parameter file
  calex::CalexConfig paramfile("input.sfe", "output.sfe");
  paramfile.clear_subsystems();

  // add comment to parameter file configuration
  paramfile.set_comment(
      "exemplary calex parameter file for a STS-2 seismometer");
  // add obligatory system parameters
  std::shared_ptr<calex::SystemParameter> amp_param(
      new calex::SystemParameter("amp",-41.5,5.1));
  std::shared_ptr<calex::SystemParameter> del_param(
      new calex::SystemParameter("del",0.01,0.001));
  std::shared_ptr<calex::SystemParameter> sub_param(
      new calex::SystemParameter("sub",0.,0.));
  std::shared_ptr<calex::SystemParameter> til_param(
      new calex::SystemParameter("til",0.,0.));
  paramfile.set_amp(amp_param);
  paramfile.set_del(del_param);
  paramfile.set_sub(sub_param);
  paramfile.set_til(til_param);

  std::shared_ptr<calex::SystemParameter> per_param(
      new calex::SystemParameter("per", 120., 1.));
  std::shared_ptr<calex::SystemParameter> dmp_param(
      new calex::SystemParameter ("dmp", 0.707, 0.01));
  // create a calex subsystem describing a STS-2 seismometer 
  std::shared_ptr<calex::CalexSubsystem> lp2(
      new calex::SecondOrderSubsystem(calex::BP, per_param, dmp_param));

  // add subsystem to parameter file configuration
  paramfile.add_subsystem(lp2);
  // write parameter file configuration to 
  std::cout << paramfile << std::endl;

  return 0;
} // function main

/* ----- END OF calexParamTest.cc  ----- */
