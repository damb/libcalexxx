/*! \file commandlineParserTest.cc
 * \brief Check the functionalism of the system parameter commandline parser.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 31/03/2012
 * 
 * Purpose: Check the functionalism of the system parameter commandline parser.
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
 * 31/03/2012  V0.1  Daniel Armbruster
 * 
 * ============================================================================
 */
 
#include <iostream>
#include <string>
#include <calexxx/parser.h>
#include <calexxx/systemparameter.h>
#include <calexxx/subsystem.h>

int main(int iargc, char* argv[])
{
  std::cout << "----------------------------------------\n"
    << "Commandline system parameter parser test\n"
    << "----------------------------------------" << std::endl;

  // system parameter parser tests
  std::string amp_str("-41.5|5.1");
  std::string amp_str_dyn("-41.5;10;0.5|5.1");
  std::string sys_str("sys|30.14|5.1");
  std::string sys_str_dyn("sys|30.14;15.3;0.2|5.1");
  calex::SystemParameter* sys_param=0;

  calex::parser::systemParameterParser(amp_str, &sys_param, "amp");
  std::cout << *sys_param;
  delete sys_param;
  calex::parser::systemParameterParser(amp_str_dyn, &sys_param, "amp");
  std::cout << *sys_param;
  delete sys_param;
  calex::parser::systemParameterParser(sys_str, &sys_param);
  std::cout << *sys_param;
  delete sys_param;
  calex::parser::systemParameterParser(sys_str_dyn, &sys_param);
  std::cout << *sys_param;
  delete sys_param;

  // first order subsystem parser tests
  calex::CalexSubsystem* subsys=0;
  std::string first_order("LP|50|2");
  std::string first_order_dyn("LP|30;60;2|2");

  calex::parser::firstOrderParser(first_order, &subsys);
  std::cout << *subsys;
  delete subsys;
  calex::parser::firstOrderParser(first_order_dyn, &subsys);
  std::cout << *subsys;
  delete subsys;

  // second order subsystem parser test
  std::string second_order("BP|50|2|30|1");
  std::string second_order_dyn1("BP|30;60;2|2|30|1");
  std::string second_order_dyn2("BP|30|1|30;60;2|2");
  std::string second_order_dyndyn("BP|115;125;0.1|1|30;60;2|2");
  calex::parser::secondOrderParser(second_order, &subsys);
  std::cout << *subsys;
  delete subsys;
  calex::parser::secondOrderParser(second_order_dyn1, &subsys);
  std::cout << *subsys;
  delete subsys;
  calex::parser::secondOrderParser(second_order_dyn2, &subsys);
  std::cout << *subsys;
  delete subsys;
  calex::parser::secondOrderParser(second_order_dyndyn, &subsys);
  std::cout << *subsys;
  delete subsys;

  return 0;
} // function main

/* ----- END OF commandlineParserTest.cc  ----- */
