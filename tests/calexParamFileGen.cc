/*! \file calexParamFileGen.cc
 * \brief Testing the calex parameter file generator.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 03/06/2012
 * 
 * Purpose: Testing the calex parameter file generator.
 *          Notice that this program additionally had been written to generate
 *          dynamically a calex parameter file.
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
 * 07/06/2012   V0.1    Daniel Armbruster
 * 
 * ============================================================================
 */
 
#define CALEXPARAMFILEGEN_VERSION "V0.1"
#define CALEXPARAMFILEGEN_LICENSE "GPLv2+"

#include <iostream>
#include <vector>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <calexxx/calexconfig.h>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using std::cout;
using std::cerr;
using std::endl;

/* -------------------------------------------------------------------------- */
namespace
{
  //! helper function which converts a string into an arbitrary type
  template <typename Ttype=double>
  Ttype string2X(std::string const& str)
  {
    std::istringstream iss(str);   
    Ttype val;
    if (!(iss >> val))
    {
      throw po::validation_error(po::validation_error::invalid_option_value);
    }
    return val;
  } // function template string2X

  /* ----------------------------------------------------------------------- */
  // split a string into elements specifying the delimiter
  void split(std::string const &s, char delim, std::vector<std::string> &elems)
  {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) { elems.push_back(item); }
  } // function split

} // anonymous namespace

/* ------------------------------------------------------------------------- */
namespace calex
{
  // custom validator for class calex::SystemParameter
  void validate(boost::any& v, const std::vector<std::string>& values,
      SystemParameter*, int)
  {
    if (values.size() == 0)
    {
      throw po::validation_error(po::validation_error::invalid_option_value);
    }

    const std::string& str = po::validators::get_single_string(values);
    std::vector<std::string> tokens;
    ::split(str, ',', tokens);
    if (tokens.size() != 3 || tokens.at(0).size() != 3)
    {
      throw po::validation_error(po::validation_error::invalid_option_value);
    }

    double val = ::string2X<double>(tokens.at(1));
    double unc = ::string2X<double>(tokens.at(2));
    v = boost::any(SystemParameter(tokens.at(0), val, unc));
  } // function validate for SystemParameter

  /* ----------------------------------------------------------------------- */
  // custom validator for class calex::FirstOrderSubsystem
  void validate(boost::any& v, const std::vector<std::string>& values,
      std::shared_ptr<FirstOrderSubsystem>*, int)
  {
    if (values.size() == 0)
    {
      throw po::validation_error(po::validation_error::invalid_option_value);
    }
    const std::string& str = po::validators::get_single_string(values);
    std::vector<std::string> tokens;
    ::split(str, ',', tokens);
    if (tokens.size() != 3 || tokens.at(0).size() != 2)
    {
      throw po::validation_error(po::validation_error::invalid_option_value);
    }
    double val = ::string2X<double>(tokens.at(1));
    double unc = ::string2X<double>(tokens.at(2));
    std::shared_ptr<SystemParameter> per_ptr(
        new SystemParameter("per", val, unc));

    if ("LP" == tokens.at(0))
    {
      v = boost::any(std::shared_ptr<FirstOrderSubsystem>(
            new FirstOrderSubsystem(LP, per_ptr)));
    } else
    if ("HP" == tokens.at(0))
    {
      v = boost::any(std::shared_ptr<FirstOrderSubsystem>(
            new FirstOrderSubsystem(HP, per_ptr)));
    } else
    {
      throw po::validation_error(po::validation_error::invalid_option_value);
    }
  } // function validate for FirstOrderSubsystem

  /* ----------------------------------------------------------------------- */
  // custom validator for class calex::SecondOrderSubsystem
  void validate(boost::any& v, const std::vector<std::string>& values,
      std::shared_ptr<SecondOrderSubsystem>*, int)
  {
    if (values.size() == 0)
    {
      throw po::validation_error(po::validation_error::invalid_option_value);
    }
    const std::string& str = po::validators::get_single_string(values);
    std::vector<std::string> tokens;
    ::split(str, ',', tokens);
    if (tokens.size() != 5 || tokens.at(0).size() != 2)
    {
      throw po::validation_error(po::validation_error::invalid_option_value);
    }
    double per_val = ::string2X<double>(tokens.at(1));
    double per_unc = ::string2X<double>(tokens.at(2));
    std::shared_ptr<SystemParameter> per_ptr(
        new SystemParameter("per", per_val, per_unc));
    double dmp_val = ::string2X<double>(tokens.at(3));
    double dmp_unc = ::string2X<double>(tokens.at(4));
    std::shared_ptr<SystemParameter> dmp_ptr(
        new SystemParameter("del", dmp_val, dmp_unc));

    if ("LP" == tokens.at(0))
    {
      v = boost::any(std::shared_ptr<SecondOrderSubsystem>(
            new SecondOrderSubsystem(LP, per_ptr, dmp_ptr)));
    } else
    if ("HP" == tokens.at(0))
    {
      v = boost::any(std::shared_ptr<SecondOrderSubsystem>(
            new SecondOrderSubsystem(HP, per_ptr, dmp_ptr)));
    } else
    if ("BP" == tokens.at(0))
    {
      v = boost::any(std::shared_ptr<SecondOrderSubsystem>(
            new SecondOrderSubsystem(BP, per_ptr, dmp_ptr)));
    } else
    {
      throw po::validation_error(po::validation_error::invalid_option_value);
    }

  } // function validate for SecondOrderSubsystem

} // namespace calex

int main(int iargc, char* argv[])
{
  // define usage information
  char usage_text[]=
  {
    "Generate a parameter file for Erhard Wielandt's calex program." "\n"
    "Version: "CALEXPARAMFILEGEN_VERSION"\n"
    "License: "CALEXPARAMFILEGEN_LICENSE"\n" 
    "    SVN: $Id$\n" 
    " Author: Daniel Armbruster" "\n"
    "  Usage: calexParamFileGen [-v|--verbose] [-o|--overwrite]" "\n"
    "           [-f|--file arg] [--maxit arg] [--alias arg] [--qac arg]" "\n"
    "           [--finac arg] [--ns1 arg] [ns2 arg] [--m0 arg]" "\n"
    "           [-p|--param arg] [--first-order arg] [--second-order arg]" "\n"
    "           --calib-in arg --calib-out arg" "\n"
    "     or: calexParamFileGen -V|--version" "\n"
    "     or: calexParamFileGen -h|--help" "\n"
    "     or: calexParamFileGen --xhelp" "\n"
  };

  // define notes text to provide additional information on commandline
  // arguments
  char notes_text[]=
  {
    "---------------------------------------------------------\n"
    "Additional notes on commandline syntax:\n"
    "To set a calex system parameter in the calex configuration file the" "\n"
    "following syntax must be used:" "\n"
    "--param nam,val,unc" "\n"
    "where 'nam' is the name (3 letters), 'val' is the value and 'unc' is" "\n"
    "the uncertainty of the system parameter."
    "To set a first order calex subsystem in the calex configuration file" "\n"
    "just use the syntay bellow:" "\n"
    "--first-order LP|HP,val,nam" "\n"
    "where 'LP' or 'HP' is the type, 'val' is the value of the period system\n"
    "parameter and 'unc' is the uncertainty of the period system parameter\n"
    "of the first order calex subsystem." "\n"
    "Second order calex subsystems must be specified as follows on the" "\n"
    "commandline:" "\n"
    "--second-order LP|HP|BP,per_val,per_unc,dmp_val,dmp_unc" "\n"
    "where 'LP' or 'HP' or 'BP' is the type, 'per_val' is the value of the\n"
    "period system parameter, 'per_unc' is the uncertainty of the period" "\n"
    "system parameter, 'dmp_val' is the value of the damping system" "\n"
    "parameter and 'dmp_unc' is the uncertainty of the damping system" "\n"
    "parameter of the second order calex subsystem." "\n"
  };

  try
  {
    std::vector<calex::SystemParameter> sys_params;
    std::vector<std::shared_ptr<calex::FirstOrderSubsystem>> 
      first_order_subsys;
    std::vector<std::shared_ptr<calex::SecondOrderSubsystem>> 
      second_order_subsys;
    /* ----------------------------------------------------------------------*/
    // Set up commandline option specific stuff
    /* ----------------------------------------------------------------------*/
    // declare only commandline options
    po::options_description cmdline_options("Commandline options");
    cmdline_options.add_options()
      ("version,V", "Show version of optcalex.")
      ("help,h", "Print this help.")
      ("xhelp", "Print an extended help text.")
      ("verbose,v",po::value<int>()->implicit_value(1), "Be verbose.")
      ("overwrite,o", "Overwrite OUTFILE")
      ("comment",
       po::value<std::string>()->default_value(
         "Generated with calexParamFileGen "CALEXPARAMFILEGEN_VERSION"."),
       "Calex parameter file comment")
      ("alias", po::value<double>()->default_value(CALEX_ALIAS),
       "Period of anti-alias filter")
      ("qac", po::value<double>()->default_value(CALEX_QAC),
       "Iteration stops when improvement in the rms misfit in one step becomes"
       "less than qac.")
      ("finac", po::value<double>()->default_value(CALEX_FINAC),
       "Iteration stops when normalized parameters change by less than finac.")
      ("ns1", po::value<int>()->default_value(CALEX_NS1),
       "Number of samples skipped at beginning of timeseries.")
      ("ns2", po::value<int>()->default_value(CALEX_NS2),
       "Number of samples skipped at end of timeseries.")
      ("m0", po::value<int>()->default_value(CALEX_M0),
       "Number of additional powers of the Laplace variable s.")
      ("maxit", po::value<int>()->default_value(CALEX_MAXIT),
       "Number of maximum iterations of calex for each parameter "
       "configuration.")
      ("param,p",
       po::value<std::vector<calex::SystemParameter>>(
         &sys_params)->multitoken(),
       "Calex system parameter.")
      ("first-order",
       po::value<std::vector<std::shared_ptr<calex::FirstOrderSubsystem>>>(
         &first_order_subsys)->multitoken(),
       "Add first order subsystem to calex parameter file.")
      ("second-order",
       po::value<std::vector<std::shared_ptr<calex::SecondOrderSubsystem>>>(
         &second_order_subsys)->multitoken(),
       "Add second order subsystem to calex parameter file.")
      ("calib-in", po::value<fs::path>()->required(),
       "Filename of calibration input signal file.")
      ("calib-out", po::value<fs::path>()->required(), 
       "Filename of calibration output signal file.")
      ("file,f", po::value<fs::path>(),
      "Write result data to arg instead to stdout.")
      ;

    po::variables_map vm;
    po::store(po::command_line_parser(iargc, argv).
      options(cmdline_options).run(), vm);

    /* ----------------------------------------------------------------------*/
    // help requested? print help
    if (vm.count("help"))
    {
      cout << usage_text
        << "------------------------------------------------------------\n";
      cout << cmdline_options;
      exit(0);
    } else
    if (vm.count("xhelp"))
    {
      cout << usage_text
        << "------------------------------------------------------------\n";
      cout << cmdline_options;
      cout << notes_text;
      exit(0);
    } else
    // version requested? print version string 
    if (vm.count("version"))
    {
      cout << "$Id$"
        << endl;
      cout << "Version: " << CALEXPARAMFILEGEN_VERSION << endl;
      exit(0);
    }
    po::notify(vm);

    /* --------------------------------------------------------------------- */
    // generate calex configuration file
    /* --------------------------------------------------------------------- */
    if (vm.count("verbose"))
    {
      cout << "calexParamFileGen: Start generating calex parameter file ..."
        << endl;
    }
    // create instance of parameter file
    calex::CalexConfig paramfile(
        vm["calib-in"].as<fs::path>().filename().string(),
        vm["calib-out"].as<fs::path>().filename().string());

    // set parameter file comment
    paramfile.set_comment(vm["comment"].as<std::string>());
    // set parameter file alias
    paramfile.set_alias(vm["alias"].as<double>());
    // set parameter file maxit
    paramfile.set_maxit(vm["maxit"].as<int>());
    // set parameter file qac
    paramfile.set_qac(vm["qac"].as<double>());
    // set parameter file finac
    paramfile.set_finac(vm["finac"].as<double>());
    // set parameter file ns1
    paramfile.set_ns1(vm["ns1"].as<int>());
    // set parameter file ns2
    paramfile.set_ns2(vm["ns2"].as<int>());
    // set parameter file m0
    paramfile.set_m0(vm["m0"].as<int>());

    
    // set system parameters
    for (auto it(sys_params.begin()); it != sys_params.end(); ++it)
    {
      std::shared_ptr<calex::SystemParameter> p(
          new calex::SystemParameter(*it));
      if ("amp" == p->get_nam()) { paramfile.set_amp(p); } else
      if ("del" == p->get_nam()) { paramfile.set_del(p); } else
      if ("sub" == p->get_nam()) { paramfile.set_sub(p); } else
      if ("til" == p->get_nam()) { paramfile.set_sub(p); }
      else { paramfile.add_systemParameter(p); }
    }

    paramfile.clear_subsystems();
    // add subsystems
    std::vector<std::shared_ptr<calex::CalexSubsystem>> subsys;
    subsys.insert(subsys.end(), first_order_subsys.begin(),
        first_order_subsys.end());
    subsys.insert(subsys.end(), second_order_subsys.begin(),
        second_order_subsys.end());
    for (auto it(subsys.begin()); it != subsys.end(); ++it)
    {
      paramfile.add_subsystem(*it);
    }

    // write result to a file instead to stdout?
    if (vm.count("file"))
    {
      const fs::path& outpath = vm["file"].as<fs::path>();
      if (fs::exists(outpath) && ! vm.count("overwrite"))
      {
        throw std::string("File exists. Specify option 'overwrite'.");
      }
#if BOOST_FILESYSTEM_VERSION == 2
      std::ofstream ofs(outpath.string().c_str());
#else
      std::ofstream ofs(outpath.c_str());
#endif
      ofs << paramfile;
      ofs.close();
    } else
    {
      cout << paramfile;
    }
    
    if (vm.count("verbose"))
    {
      cout << "calexParamFileGen: Parameter file successfully generated." 
        << endl;
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

/* ----- END OF calexParamFileGen.cc  ----- */

