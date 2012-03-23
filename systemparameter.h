/*! \file systemparameter.h
 * \brief Declaration of class for calex system parameters.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 16/03/2012
 * 
 * Purpose: Declaration of class for calex system parameters.
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
 * 16/03/2012  V0.1  Daniel Armbruster
 * 
 * ============================================================================
 */
 
#include <string>
#include <ostream>

#ifndef _SYSTEMPARAMETER_H_
#define _SYSTEMPARAMETER_H_

namespace calex
{
  /* Implement everything using an abstract base class for both a known and a
   * unknown system parameter. Unknown system parameters will be used in
   * relation to liboptimizexx system parameter reconfiguration.
   */
  /*=========================================================================*/
  /*
   * Class maintaining a calex system parameter.
   * 
   * System parameters are specified as follows: a line with
   * - \c nam \c val \c unc (three entries separated by spaces) where \c nam is
   *   a arbitrary name (three characters) that appears in the printout
   * - \c val is the initial value of an active parameter, resp. the fixed value
   *   of a passive parameter
   * - \c unc is the estimated uncertainty of an active parameter, resp. zero
   *   for a passive parameter
   */
  class SystemParameter
  {
    public:
      /*!
       * constructor
       *
       * \param nam Arbitrary name (three characters).
       * \param val Initial value of an active parameter, resp. the fixed value
       * of a passive parameter.
       * \param unc Estimated uncertainty of an active parameter, resp. zero for
       * a passive parameter.
       */
      SystemParameter(std::string const nam, double const val,
          double const unc);
      /*!
       * constructor
       *
       * \param nam Arbitrary name (three characters).
       * \param unc Estimated uncertainty of an active parameter, resp. zero for
       * a passive parameter.
       * \note {System parameters created with this constructor will be treated
       * as a unknown system parameters which means that such a system
       * parameter might be used as a \a variable system parameter in
       * calex::CalexApplication.}
       */
      SystemParameter(std::string const nam, double const unc);
      /*!
       * query function for name
       *
       * \return name characterizing the system parameter
       */
      std::string const& get_nam() const { return Mnam; }
      /*!
       * query function for initial value
       *
       * \return initial value of an active parameter, resp. the fixed value of
       * a passive parameter
       */
      double const& get_val() const;
      /*!
       * query function for estimated uncertainty
       *
       * \return estimated uncertainty of an active parameter, resp. zero for a
       * passive parameter
       */
      double const& get_unc() const { return Munc; }
      /*!
       * Query function if calex system parameter is an active or passive
       * parameter.
       *
       * \return true if active - false if passive
       */
      bool is_active() const { return 0 != Munc; }
      /*
       * Query function if calex system parameter is known or unknown/variable.
       *
       * \return true if known else false
       */
      bool is_known() const { return Mknown; }

      //! overloaded output stream operator for system parameter
      friend std::ostream& operator<<(std::ostream& os,
          SystemParameter const& param); 
      
    protected:
      void write(std::ostream& os) const;

    private:
      //! arbitrary name of three characters
      std::string Mnam;
      //! initial value of an active parameter
      double Mval;
      //! estimated uncertainty of an active parameter
      double Munc;
      //! flag to store the state if the system parameter is known or unknown
      bool Mknown;

  }; // class SystemParameter

  /*=========================================================================*/

} // namespace calex

#endif // include guard

/* ----- END OF systemparameter.h  ----- */
