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
   * \note calex::CalexApplication only will call calex::CalexConfig::update and
   * pass the current coordinates.
   * \code
   * McalexConfig->update<Ctype>(node->getCoordinates());
   * \endcode
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
       */
      CalexApplication(CalexConfig* config) : McalexConfig(config)
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

  }; // class template CalexApplication

  /*=========================================================================*/
  template <typename Ctype>
  void CalexApplication<Ctype>::operator()(opt::Node<Ctype, TresultType>* node)
  {
    McalexConfig->update<Ctype>(node->getCoordinates());

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
    CALEX_assert(fs::remove(param_path) && fs::remove(out_path) &&
        fs::remove("synt") && fs::remove("einf") && fs::remove("ausf") && 
        fs::remove("rest") && fs::remove("winplot.par"),
        "Error while removing current calex files");

  } // function CalexApplication<Ctype>::operator()

  /*-------------------------------------------------------------------------*/

} // namespace calex

#endif // include guard

/* ----- END OF calexvisitor.h  ----- */
