/*! \file parser.h
 * \brief Module which provides system parameter commandline parser functions
 * (Implementation).
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 27/03/2012
 * 
 * Purpose: Module which provides system parameter commandline parser functions
 * (Implementation).  
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
 * 27/03/2012  V0.1  Daniel Armbruster
 * 
 * ============================================================================
 */

#include <string>
#include <calexxx/systemparameter.h>
#include <calexxx/subsystem.h>

#ifndef _CALEX_PARSER_H_
#define _CALEX_PARSER_H_

namespace opt = optimize;
 
namespace calex
{
  /*!
   * \brief Namespace containing functions related to commandline system
   * parameter parsing.
   *
   * \defgroup group_parser Parse commandline arguments
   */
  namespace parser
  {
    //! helds util functions
    namespace 
    {
      int findSecondSeparatorPosition(std::string const& str, char sep='|'); 
    } // namespace (unnamed)

    /* --------------------------------------------------------------------- */
    /*!
     * function to convert a \c --sys-param commandline argument into a calex
     * grid standard system parameter or rather into a grid system parameter.\n
     *
     * \param str string to convert
     * \param sysparam Pointer to a system parameter. To avoid a return value
     * the result will be assigned to this pointer.
     * \param nam Name identifier of calex system parameter which will be set if
     * available. Especially convenient for obligatory system parameters in the
     * calex parameter file (represented by class calex::CalexConfig).
     *
     * \ingroup group_parser
     */
    void systemParameterParser(std::string const& str,
        SystemParameter** sysparam, std::string const nam="");

    /* --------------------------------------------------------------------- */
    /*!
     * function to convert a \c --first-order commandline argument into a first
     * order calex subsystem.
     *
     * \param str string to convert
     * \param subsys Pointer to a calex subsystem. To avoid return value the
     * result will be assigned to this pointer.
     *
     * \ingroup group_parser
     */
    void firstOrderParser(std::string const& str, CalexSubsystem** subsys);

    /* --------------------------------------------------------------------- */
    /*!
     * function to convert a \c --second-order commandline argument into a
     * second order calex subsystem.
     *
     * \param str string to convert
     * \param subsys Pointer to a calex subsystem. To avoid return value the
     * result will be assigned to this pointer.
     *
     * \ingroup group_parser
     */
    void secondOrderParser(std::string const& str, CalexSubsystem** subsys);

    /* --------------------------------------------------------------------- */

  } // namespace parser

} // namespace calex

#endif // include guard

/* ----- END OF parser.h  ----- */
