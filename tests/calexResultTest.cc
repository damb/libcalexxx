/*! \file calexResultTest.cc
 * \brief Testing the calex.out result file parser.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 20/03/2012
 * 
 * Purpose: Testing the calex.out result file parser.
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
 * 15/05/2012  V0.2  Test header information.
 * 
 * ============================================================================
 */
 
#include <iostream>
#include <fstream>
#include <calexxx/resultdata.h>


int main(int iargc, char* argv[])
{
  // create instance of calex result data class
  calex::CalexResult result;

  // read exemplary calex output file
  std::ifstream ifs("calex.out");

  // set result data
  ifs >> result;

  // write result to stdout
  std::cout << result;

  result.writeHeaderInfo(std::cout);
  result.writeLine(std::cout);
  std::cout << std::endl;

  // create instance of calex result data class
  calex::CalexResult result_second;
  // read exemplary calex output file
  std::ifstream ifs_second("calex.out.0");
  // set result data
  ifs_second >> result_second;
  // write result to stdout
  std::cout << result_second;

  result_second.writeHeaderInfo(std::cout);
  result_second.writeLine(std::cout);
  return 0;
} // function main

/* ----- END OF calexResultTest.cc  ----- */
