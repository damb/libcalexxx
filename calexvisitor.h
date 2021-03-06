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
 * 15/03/2012  V0.1     Daniel Armbruster
 * 09/04/2012  V0.2     If V2 of the Boost filsystem is in use now construct
 *                      calex parameter filepath with thread ID.
 * 15/04/2012  V0.3     Make update process of class calex::CalexConfig thread
 *                      safe to avoid racing conditions.
 * 
 * ============================================================================
 */
 
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <calexxx/calexconfig.h>
#include <calexxx/resultdata.h>
#include <calexxx/error.h>
#include <optimizexx/application.h>

#ifndef _CALEX_CALEXVISITOR_H_
#define _CALEX_CALEXVISITOR_H_

namespace opt = optimize;
namespace fs = boost::filesystem;

namespace calex
{
  //! result data which will be stored at each node.
  typedef CalexResult TresultType;

  /*=========================================================================*/
  /*!
   * Calex liboptimizexx application.
   *
   * The application will generate a calex configuration file for every node.
   * Next it will call the extern calex program written by Erhard Wielandt and
   * store the result data within the liboptimizexx parameter space grids'
   * nodes.
   *
   * From V0.3 updating class calex::CalexConfig had been made thread safe
   * using the <a href="http://www.boost.org/doc/libs/release/libs/thread/">
   * Boost thread library</a>.
   *
   * \note calex::CalexApplication only will call calex::CalexConfig::update and
   * pass the current coordinates.
   * \code
   * McalexConfig->update<Ctype>(node->getCoordinates());
   * \endcode
   *
   * \note If V2 of the Boost filesystem library in use the program using class
   * template calex::CalexApplication must link against \c boost_thread cause
   * calex parameter file names will be build containing the thread ID.
   *
   * \note
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
       * \param verbose Be verbose.
       */
      CalexApplication(CalexConfig* config, bool verbose=false) :
        McalexConfig(config), Mverbose(verbose)
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
      //! be verbose
      bool Mverbose;
      //! mutual exclusion variable to guarantee thread safety
      boost::mutex Mmutex; 

  }; // class template CalexApplication

  /*=========================================================================*/
  template <typename Ctype>
  void CalexApplication<Ctype>::operator()(opt::Node<Ctype, TresultType>* node)
  {
    fs::path param_path;
    // thread safe part
    {
      boost::lock_guard<boost::mutex> lock(Mmutex);
      McalexConfig->update<Ctype>(node->getCoordinates());

      // write calex parameter file to disk
#if BOOST_FILESYSTEM_VERSION == 2
      // If V2 of the Boost filesystem library is in use construct calex
      // parameter file name containing the thread ID.
      std::ostringstream oss;
      oss << "calex-" << boost::this_thread::get_id() << ".par";

      param_path = oss.str();
      std::ofstream ofs(param_path.string().c_str());
#else
      param_path = fs::unique_path("%%%%-%%%%-%%%%-%%%%.par");
      std::ofstream ofs(param_path.c_str());
#endif
      ofs << *McalexConfig;
      ofs.close();
    }

    // execute calex command
    //std::string calex_command("calex "+param_path.string());
    std::string calex_command("calex "+param_path.string()+" >/dev/null 2>&1");
    system(calex_command.c_str());

    // read calex result data of file *.out
#if BOOST_FILESYSTEM_VERSION == 2
    fs::path out_path(std::string(param_path.stem()+".out"));
    std::ifstream ifs(out_path.string().c_str());
#else
    fs::path out_path(std::string(param_path.stem().string()+".out"));
    std::ifstream ifs(out_path.c_str());
#endif
    TresultType calex_result;
    if (ifs)
    {
      ifs >> calex_result;
      
      if (Mverbose) { std::cout << "Result: " << calex_result << std::endl; }
      node->setResultData(calex_result);
      ifs.close();

      node->setComputed();
    }
    // delete *.par and temporary calex files
    CALEX_assert(fs::remove(param_path) && fs::remove(out_path),
        "Error while removing current calex files");

  } // function CalexApplication<Ctype>::operator()

  /*-------------------------------------------------------------------------*/

} // namespace calex

#endif // include guard

/* ----- END OF calexvisitor.h  ----- */
