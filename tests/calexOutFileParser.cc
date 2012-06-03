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
 * 03/06/2012  V0.1  Daniel Armbruster
 * 
 * ============================================================================
 */
 
#define CALEXOUTFILEPARSER_VERSION "V0.1"
#define CALEXOUTFILEPARSER_LICENSE "GPLv2+"

#include <iostream>
#include <vector>
#include <fstream>
#include <calexxx/resultdata.h>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

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
    "           [-f|--file arg] CALEXOUTFILE [CALEXOUTFILE [...]]" "\n"
    "     or: calexOutFileParser -V|--version" "\n"
    "     or: calexOutFileParser -h|--help" "\n"
  };

  try
  {
    /* ----------------------------------------------------------------------*/
    // Set up commandline option specific stuff
    /* ----------------------------------------------------------------------*/
    // declare only commandline options
    po::options_description generic("Commandline options");
    generic.add_options()
      ("version,V", "Show version of optcalex.")
      ("help,h", "Print this help.")
      ("verbose,v",po::value<int>()->implicit_value(1), "Be verbose.")
      ("overwrite,o", "Overwrite OUTFILE")
      ;

    // Hidden options, will be allowed both on command line and
    // in config file, but will not be shown to the user.
    po::options_description hidden("Hidden options");
    hidden.add_options()
      ("input-file", po::value<std::vector<fs::path>>()->required(),
       "Filepath for calex *.out files.")
      ;

    po::options_description cmdline_options;
    cmdline_options.add(generic).add(hidden);

    po::options_description visible_options("Allowed options", 80);
    visible_options.add(generic);

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
      cout << visible_options;
      exit(0);
    } else
    // version requested? print version string 
    if (vm.count("version"))
    {
      cout << "$Id$" << endl;
      cout << "Version: " << CALEXOUTFILEPARSER_VERSION << endl;
      exit(0);
    }
    po::notify(vm);


    // CONTINUE HERE CONTINUE HERE CONTINUE HERE CONTINUE HERE CONTINUE HERE    
    if (vm.count("verbose"))
    {
      cout << "optcalex: Opening optcalex configuration file." << std::endl;
    }

    // reading configuration file
#if BOOST_FILESYSTEM_VERSION == 2
    std::ifstream ifs(configFilePath.string().c_str());
#else
    std::ifstream ifs(configFilePath.c_str());
#endif
    if (!ifs)
    {
      // if default configuration file does not exist -> create default optcalex
      // configuration file
      if (configFilePath == defaultConfigFilePath &&
          !fs::exists(defaultConfigFilePath))
      {
        // create directory
        fs::create_directory(defaultConfigFilePath.parent_path());
        // create default configuration file
#if BOOST_FILESYSTEM_VERSION == 2
        std::ofstream ofs(defaultConfigFilePath.string().c_str());
#else
        std::ofstream ofs(defaultConfigFilePath.c_str());
#endif
        ofs.close();
      } else
      {
        throw std::string(
            "Can not open config file '"+configFilePath.string()+"'");
      }
    }
    else
    {
      po::store(parse_config_file(ifs, config_file_options), vm);
      po:: notify(vm);
    }

    // fetch commandline arguments
    fs::path outpath(vm["output-file"].as<fs::path>());
    if (fs::exists(outpath) && ! vm.count("overwrite"))
    {
      throw std::string("OUTFILE exists. Specify option 'overwrite'.");
    }
    fs::path calibInfile(vm["calib-in"].as<fs::path>());
    fs::path calibOutfile(vm["calib-out"].as<fs::path>());
    // check if path is in working directory
    if (! fs::exists(fs::current_path() /= calibInfile) ||
      ! fs::exists(fs::current_path() /= calibInfile)) 
    {
      throw std::string(
          "Only execute optcalex in directory where datafiles are located.");
    }
    if (vm.count("verbose"))
    {
      cout << "optcalex: Collecting commandline arguments ..." << endl 
        << "optcalex: Calibration input time series:  " 
        << calibInfile.string() << endl 
        << "optcalex: Calibration output time series: " 
        << calibOutfile.string() << endl
        << "optcalex: Result data filepath:           " 
        << outpath.string() << endl;
    }

    /* --------------------------------------------------------------------- */
    // configure calex parameter file
    calex::CalexConfig calex_config(
        calibInfile.string(), calibOutfile.string());
    // fetch system parameter commandline arguments
    if (vm.count("sys-param"))
    {
      std::vector<std::string> sys_params =
        vm["sys-param"].as<std::vector<std::string> >();
      for (auto cit(sys_params.cbegin()); cit != sys_params.cend(); ++cit)
      {
        calex_config.add_systemParameter(
            calex::parser::systemParameterParser(*cit));
      }
    }

    if (vm.count("amp-param"))
    {
      calex_config.set_amp(
          calex::parser::systemParameterParser(
            vm["amp-param"].as<std::string>(),
            "amp"));
    } else
    {
      calex_config.set_amp(
          std::shared_ptr<calex::SystemParameter>(
            new calex::SystemParameter("amp", CALEX_AMP, CALEX_AMPUNC)));
    }
    if (vm.count("del-param"))
    {
      calex_config.set_del(
          calex::parser::systemParameterParser(
            vm["del-param"].as<std::string>(),
            "del"));
    } else
    {
      calex_config.set_del(
          std::shared_ptr<calex::SystemParameter>(
            new calex::SystemParameter("del", CALEX_DEL, CALEX_DELUNC)));
    }
    if (vm.count("sub-param"))
    {
      calex_config.set_sub(
          calex::parser::systemParameterParser(
            vm["sub-param"].as<std::string>(),
            "sub"));
    } else
    {
      calex_config.set_sub(
          std::shared_ptr<calex::SystemParameter>(
            new calex::SystemParameter("sub", CALEX_SUB, CALEX_SUBUNC)));
    }
    if (vm.count("til-param"))
    {
      calex_config.set_til(
          calex::parser::systemParameterParser(
            vm["til-param"].as<std::string>(),
            "til"));
    } else
    {
      calex_config.set_til(
          std::shared_ptr<calex::SystemParameter>(
            new calex::SystemParameter("til", CALEX_TIL, CALEX_TILUNC)));
    }
    // fetch first order subsystem commandline arguments
    if (vm.count("first-order"))
    {
      std::vector<std::string> first_order =
        vm["first-order"].as<std::vector<std::string> >();
      for (auto cit(first_order.cbegin()); cit != first_order.cend(); ++cit)
      {
        calex_config.add_subsystem(calex::parser::firstOrderParser(*cit));
      }
    }
    // fetch second order subsystem commandline arguments
    if (vm.count("second-order"))
    {
      std::vector<std::string> second_order =
        vm["second-order"].as<std::vector<std::string> >();
      for (auto cit(second_order.cbegin()); cit != second_order.cend(); ++cit)
      {
        calex_config.add_subsystem(calex::parser::secondOrderParser(*cit));
      }
    }
    
    if (vm.count("verbose"))
    {
      cout << "optcalex: Set alias to: " << vm["alias"].as<double>() << endl
        << "optcalex: Set m0 to:    " << vm["m0"].as<int>() << endl 
        << "optcalex: Set ns1 to:   " << vm["ns1"].as<int>() << endl
        << "optcalex: Set ns2 to:   " << vm["ns2"].as<int>() << endl 
        << "optcalex: Set qac to:   " << vm["qac"].as<double>() << endl
        << "optcalex: Set finac to: " << vm["finac"].as<double>() << endl 
        << "optcalex: Set maxit to: " << vm["maxit"].as<int>() << endl;
    }
    calex_config.set_alias(vm["alias"].as<double>());
    calex_config.set_m0(vm["m0"].as<int>());
    calex_config.set_ns1(vm["ns1"].as<int>());
    calex_config.set_ns2(vm["ns2"].as<int>());
    calex_config.set_qac(vm["qac"].as<double>());
    calex_config.set_finac(vm["finac"].as<double>());
    calex_config.set_maxit(vm["maxit"].as<int>());

    if (! calex_config.hasGridSystemParameters())
    {
      throw std::string("No grid system parameters specified.");
    }

    /* --------------------------------------------------------------------- */
    // mayor part
    
    if (vm.count("verbose"))
    {
      cout << "optcalex: Setting up parameter space ..." << endl;
    }
    // create parameter space builder
    std::unique_ptr<opt::ParameterSpaceBuilder<TcoordType, TresultType>>
      builder(new opt::StandardParameterSpaceBuilder<TcoordType, TresultType>);

    // gridsearch algorithm
    std::shared_ptr<opt::GlobalAlgorithm<TcoordType, TresultType>> algo( 
      new opt::GridSearch<TcoordType, TresultType>(
          std::move(builder), numThreads));

    calex_config.set_gridSystemParameters<TcoordType>(*algo);

    calex::CalexApplication<TcoordType> app(&calex_config,
       vm.count("verbose"));

    // synchronization grid and calex configuration file
    calex_config.synchronize<TcoordType>(*algo);

    algo->constructParameterSpace();
    if (vm.count("verbose"))
    {
      cout << "optcalex: Sending calex application through parameter space "
        << "grid ..." << endl;
    }
    algo->execute(app);

    // collect results and write to outpath
    if (vm.count("verbose"))
    {
      cout << "optcalex: Collecting results from parameter space grid ..."
        << endl
        << "optcalex: Writing result file ..."
        << endl;
    }

    std::ofstream ofs(outpath.string().c_str());
    // write header information
    // write header information for parameter space parameters
    std::vector<std::string> param_names(
        calex_config.get_gridSystemParameterNames<TcoordType>(*algo));
    for (auto cit(param_names.cbegin()); cit != param_names.cend(); ++cit)
    {
      ofs << std::setw(12) << std::fixed << std::left << *cit << " ";
    }
    ofs << "    ";
    // write header information of result data
    opt::Iterator<TcoordType, TresultType> it(
      algo->getParameterSpace().createIterator(opt::ForwardNodeIter));
    it.first();
    (*it)->getResultData().writeHeaderInfo(ofs);

    // write data
    while (!it.isDone())
    {
      // write search parameter
      std::vector<TcoordType> const& c = (*it)->getCoordinates();
      for (auto cit(c.cbegin()); cit != c.cend(); ++cit)
      {
        ofs << std::setw(12) << std::fixed << std::left << *cit << " ";
      }
      ofs << "    ";
      // write result data
      (*it)->getResultData().writeLine(ofs);

      ++it;
    }

    if (vm.count("verbose"))
    {
      cout << "optcalex: Calculations successfully finished." << endl;
    }
  }
  catch (std::string e) 
  {
    cerr << "ERROR: " << e << "\n";
    cerr << usage_text;
    return 1;
  }
  catch (std::exception& e) 
  {
    cerr << "ERROR: " << e.what() << "\n";
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

  // create instance of calex result data class
  calex::CalexResult result_second;
  // read exemplary calex output file
  std::ifstream ifs_second("calex.out.0");
  // set result data
  ifs_second >> result_second;
  // write result to stdout
  std::cout << result_second;

  result.writeHeaderInfo(std::cout);
  result.writeLine(std::cout);
  return 0;
} // function main

/* ----- END OF calexOutFileParser.cc  ----- */

