/*! \file calexOutFileParser.cc
 * \brief Testing the calex.out result file parser.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 03/06/2012
 * 
 * Purpose: Testing the calex.out result file parser.
 *          Notice that this program additionally had been written to collect
 *          calex results in an overview.
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
 * 03/06/2012  V0.1   Daniel Armbruster
 * 14/03/2012  V0.1.1 corrected usage text
 * 
 * ============================================================================
 */
 
#define CALEXOUTFILEPARSER_VERSION "V0.1"
#define CALEXOUTFILEPARSER_LICENSE "GPLv2+"

#include <iostream>
#include <vector>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <calexxx/resultdata.h>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using std::cout;
using std::cerr;
using std::endl;

int main(int iargc, char* argv[])
{
  // define usage information
  char usage_text[]=
  {
    "Version: "CALEXOUTFILEPARSER_VERSION"\n"
    "License: "CALEXOUTFILEPARSER_LICENSE"\n" 
    "    SVN: $Id$\n" 
    " Author: Daniel Armbruster" "\n"
    "  Usage: calexOutFileParser [-v|--verbose] [-o|--overwrite]" "\n"
    "           [-H|--header] [-f|--file arg]" "\n"
    "           CALEXOUTFILE [CALEXOUTFILE [...]]" "\n"
    "     or: calexOutFileParser -V|--version" "\n"
    "     or: calexOutFileParser -h|--help" "\n"
  };

  try
  {
    /* ----------------------------------------------------------------------*/
    // Set up commandline option specific stuff
    /* ----------------------------------------------------------------------*/
    // declare only commandline options
    po::options_description generic_options("Commandline options", 80);
    generic_options.add_options()
      ("version,V", "Show version of calexOutFileParser.")
      ("help,h", "Print this help.")
      ("verbose,v", "Be verbose.")
      ("overwrite,o", "Overwrite arg in option '-f|--file'.")
      ("header,H", "Additionally print a header line to each result.")
      ("file,f", po::value<fs::path>(),
      "Write result data to arg instead to stdout.")
      ;

    // Hidden options, will be allowed both on command line and
    // in config file, but will not be shown to the user.
    po::options_description hidden_options("Hidden options");
    hidden_options.add_options()
      ("input-file",
        po::value<std::vector<fs::path>>()->required(),
        "Filepath for calex *.out files.")
      ;

    po::options_description cmdline_options;
    cmdline_options.add(generic_options).add(hidden_options);

    po::positional_options_description p;
    p.add("input-file", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(iargc, argv).
      options(cmdline_options).positional(p).run(), vm);

    /* ----------------------------------------------------------------------*/
    // help requested? print help
    if (vm.count("help"))
    {
      cout << usage_text
        << "------------------------------------------------------------\n";
      cout << generic_options;
      exit(0);
    } else
    // version requested? print version string 
    if (vm.count("version"))
    {
      cout << "$Id$"
        << endl;
      cout << "Version: " << CALEXOUTFILEPARSER_VERSION << endl;
      exit(0);
    }
    po::notify(vm);

    // write result to a file instead to stdout?
    if (vm.count("file"))
    {
      const fs::path& outpath = vm["file"].as<fs::path>();
      if (fs::exists(outpath) && ! vm.count("overwrite"))
      {
        throw std::string("File exists. Specify option 'overwrite'.");
      }
    }
    
    // start loop over all calex result files
    const std::vector<fs::path>& infiles = 
      vm["input-file"].as<std::vector<fs::path>>();
    for (auto cit(infiles.cbegin()); cit != infiles.cend(); ++cit)
    {
      std::string filepath(cit->string());
      // create instance of calex result data class
      calex::CalexResult result;
      if (fs::exists(*cit) && fs::is_regular_file(*cit))
      {
        if (vm.count("verbose"))
        {
          cout << "calexOutFileParser: Opening calex *.out file: '"
            << filepath << "' ..." << endl;
        }
        // read exemplary calex output file
        std::ifstream ifs(filepath.c_str());
        // set result data
        ifs >> result;
        // write result
        if (vm.count("file"))
        {
          const fs::path& outpath = vm["file"].as<fs::path>();
#if BOOST_FILESYSTEM_VERSION == 2
          std::ofstream ofs(outpath.string().c_str());
#else
          std::ofstream ofs(outpath.c_str());
#endif
          ofs << cit->filename() << " ";
          if (vm.count("header")) 
          { 
            cout << endl;
            result.writeHeaderInfo(ofs);
          }
          result.writeLine(ofs);
        }
        else
        {
          cout << cit->filename() << " ";
          if (vm.count("header"))
          { 
            cout << endl;
            result.writeHeaderInfo(cout);
          }
          result.writeLine(cout);
        }
      }
      else
      {
        throw std::string(
          "'"+filepath+"' does not exist or is not a regular file.");
      }
    }
    if (vm.count("verbose"))
    {
      cout << "calexOutFileParser: Data successfully read." << endl;
    }
  }
  catch (const std::string e) 
  {
    cerr << "ERROR: " << e << '\n';
    cerr << usage_text;
    return 1;
  }
  catch (const fs::filesystem_error& ex) 
  {
    cerr << "ERROR: " << ex.what() << '\n';
    cerr << usage_text;
    return 1;
  }
  catch (const std::exception& ex)
  {
    cerr << "ERROR: " << ex.what() << '\n';
    cerr << usage_text;
    return 1;
  }
  catch (...)
  {
    cerr << "ERROR: Exception of unknown type!\n";
    cerr << usage_text;
    return 1;
  }

  return 0;

} // function main

/* ----- END OF calexOutFileParser.cc  ----- */

