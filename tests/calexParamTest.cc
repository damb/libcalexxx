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
 * 
 * ============================================================================
 */
 
#include <iostream>
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

  calex::SystemParameter per_param("per", 120., 1.);
  calex::SystemParameter dmp_param("dmp", 0.707, 0.01);
  // create a calex subsystem describing a STS-2 seismometer 
  calex::CalexSubsystem* lp2 = new calex::SecondOrderSubsystem(calex::BP,
      per_param, dmp_param);

  // add subsystem to parameter file configuration
  paramfile.add_subsystem(*lp2);

  // write parameter file configuration to 
  std::cout << paramfile << std::endl;

  delete lp2;

  return 0;
} // function main

/* ----- END OF calexParamTest.cc  ----- */
