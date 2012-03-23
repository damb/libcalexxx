/*! \file calexvisitor.h
 * \brief Declaration and implementation of a the calex liboptimizexx
 * application.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 15/03/2012
 * 
 * Purpose: Declaration and implementation of a the calex liboptimizexx
 * application.  
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
 * 15/03/2012  V0.1  Daniel Armbruster
 * 
 * ============================================================================
 */
 
#include <fstream>
#include <string>
#include <cstdlib>
#include <boost/filesystem.hpp>
#include <calexxx/calexconfig.h>
#include <calexxx/resultdata.h>
#include <calexxx/error.h>
#include <optimizexx/application.h>

#ifndef _CALEXVISITOR_H_
#define _CALEXVISITOR_H_

namespace opt = optimize;
namespace fs = boost::filesystem;

namespace calex
{
  //! result data which will be stored at each node.
  typedef CalexResult TresultType;

  /*=========================================================================*/
  /*
   * Calex liboptimizexx application.
   *
   * The application will generate a calex configuration file for every node.
   * Next it will call the extern calex program written by Erhard Wielandt and
   * store the result data within the liboptimizexx parameter space grids'
   * nodes.
   *
   * \todo The class template CalexApplication yet isn't really modular in a
   * sense that it is able to handle the manipulation of an aribrary number
   * calex system of calex system parameters.
   * So I should aim on writing a fully configurable calex application class
   * template. Suggestions?
   */
  template <typename Ctype>
  class CalexApplication : 
      public opt::ParameterSpaceVisitor<Ctype, TresultType>
  {
    public:
      /*!
       * constructor
       *
       * \param config Calex parameter file configuration.
       * \param per_unc uncertainty for seismometer period
       * \param dmp_unc uncertainty for seismometer damping
       */
      CalexApplication(CalexConfig* config, double const per_unc,
          double const dmp_unc) : McalexConfig(config), Mper_unc(per_unc),
          Mdmp_unc(dmp_unc)
      { }
      //! Visit function for a liboptimizexx grid.
      /*!
       * Does nothing by default.
       * Since a grid has no coordinates the body of this function is empty.
       *
       * \param grid Grid to be visited.
       */
      virtual void operator()(opt::Grid<Ctype, TresultType>* grid) { }
      //! Visit function / application for a liboptimizexx node.
      /*!
       * Calculates the sum of the coordinate data or rather the parameters
       *
       * \param node Node to be visited.
       */
      virtual void operator()(opt::Node<Ctype, TresultType>* node);
      
    private:
      //! calex parameter file configuration
      CalexConfig* McalexConfig;
      //! uncertainty for period
      double Mper_unc;
      //! uncertainty for damping
      double Mdmp_unc;

  }; // class template CalexApplication

  /*=========================================================================*/
  template <typename Ctype>
  void CalexApplication<Ctype>::operator()(opt::Node<Ctype, TresultType>* node)
  {
    // configure calex parameter file
    /*! 
     * \note{Delegate the updating functionalism of the parameter file (class
     * calex::CalexConfig) to calex::CalexConfig and only call its
     * calex::Calexconfig::update function passing the corresponding parameters
     * (node coordinates). A positive side effect of this approach is that
     * additional system parameters and subsystems needn't to be deleted
     * anymore. Additionally a calex system parameter must provide a variable to
     * store the corresponding gridId if unknown.}
     */
    std::vector<Ctype> const& params = node->getCoordinates();
    CalexSubsystem* lp2 = new SecondOrderSubsystem(BP, 
        SystemParameter("per", params[0], Mper_unc),
        SystemParameter("dmp", params[1], Mdmp_unc));
    McalexConfig->set_comment("calex liboptimizexx application");
    McalexConfig->add_subsystem(lp2);

    // write calex parameter file to disk
#if BOOST_FILESYSTEM_VERSION == 2
    fs::path param_path("calex.par");
    std::ofstream ofs(param_path.string().c_str());
#else
    fs::path param_path(fs::unique_path("%%%%-%%%%-%%%%-%%%%.par"));
    std::ofstream ofs(param_path.c_str());
#endif
    ofs << *McalexConfig;
    ofs.close();

    // execute calex command
    std::string calex_command("calex "+param_path.string());
    CALEX_assert(!system(calex_command.c_str()), 
        "Error while executing extern calex program.");


    // read calex result data of file *.out
#if BOOST_FILESYSTEM_VERSION == 2
    fs::path out_path(std::string(param_path.stem()+".out"));
    std::ifstream ifs(out_path.string().c_str());
#else
    fs::path out_path(std::string(param_path.stem().string()+".out"));
    std::ifstream ifs(out_path.c_str());
#endif
    TresultType calex_result;
    ifs >> calex_result;
    node->setResultData(calex_result);
    ifs.close();
    
    // delete *.par and temporary calex files
    CALEX_assert(!(fs::remove(param_path) && fs::remove(out_path) &&
        fs::remove("synt") && fs::remove("einf") && fs::remove("ausf") && 
        fs::remove("rest") && fs::remove("winplot.par")),
        "Error while removing current calex files");
    McalexConfig->clear_subsystems();
    delete lp2;

  } // function CalexApplication<Ctype>::operator()

  /*-------------------------------------------------------------------------*/

} // namespace calex

#endif // include guard

/* ----- END OF calexvisitor.h  ----- */
