/*! \file systemparameter.h
 * \brief Declarations of classes for calex system parameters.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 16/03/2012
 * 
 * Purpose: Declarations of classes for calex system parameters.
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
#include <optimizexx/parameter.h>
#include <calexxx/error.h>

#ifndef _SYSTEMPARAMETER_H_
#define _SYSTEMPARAMETER_H_

namespace opt = optimize;

namespace calex
{
  /*=========================================================================*/
  /*
   * \brief class for calex system parameters
   * Calex system parameters are specified as follows: a line with
   * - \c nam \c val \c unc (three entries separated by spaces) where \c nam is
   *   a arbitrary name (three characters) that appears in the printout
   * - \c val is the initial value of an active parameter, resp. the fixed value
   *   of a passive parameter
   * - \c unc is the estimated uncertainty of an active parameter, resp. zero
   *   for a passive parameter\n
   *
   * This class implements a \a constant calex system parameter for a class
   * calex::CalexConfig. Constant in a sense that it can't be modified once
   * initialized. The only way to modify such a constant system parameter in
   * class calex::CalexConfig is just to use the class' access member
   * functions. Though the calex program will optimize this system parameter in
   * case it is an active system parameter which means that
   * calex::SystemParameter::Munc is unequal to zero.\n 
   */
  class SystemParameter
  {
    public:
      /*!
       * constructor
       *
       * \param nam identifier of system parameter
       * \param val value of system parameter
       * \param unc uncertainty of system parameter
       */
      SystemParameter(std::string const nam, double const val,
          double const unc);
      //! destructor
      virtual ~SystemParameter() { }
      /*!
       * query function for name
       *
       * \return name with lenght of 3 characters characterizing the
       * calex system parameter
       */
      virtual std::string const& get_nam() const { return Mnam; }
      /*!
       * query function for initial value
       *
       * \return initial value of an active parameter, resp. the fixed value of
       * a passive parameter
       */
      virtual double const& get_val() const { return Mval; }
      /*!
       * query function for estimated uncertainty
       *
       * \return estimated uncertainty of an active parameter, resp. zero for a
       * passive parameter
       */
      virtual double const& get_unc() const { return Munc; }
      /*!
       * Query function if system parameter is an active or passive
       * parameter.
       *
       * \return true if active - false if passive
       */
      virtual bool is_active() const { return 0 != Munc; }
      //! query function if system parameter is a grid system parameter
      virtual bool is_gridSystemParameter() const { return false; }
      /*!
       * member access function for value
       *
       * \param val new value
       */
      virtual void set_val(double const val) { CALEX_illegal; }
      /*!
       * member access function to set coordinate id while synchronizing the
       * parameter with the coordinate order of the builder
       *
       * \param id coordinate id
       */
      virtual void set_coordinateId(int const id) { CALEX_illegal; }
      //! query function for coordinate id
      virtual int const& get_coordinateId() const { CALEX_illegal; }

      /*!
       * overloaded output stream operator for system parameter
       *
       * Since optimize::Parameter has no overloaded ostream operator there are
       * no ambiguity issues
       */
      friend std::ostream& operator<<(std::ostream& os,
          SystemParameter const& param); 

    protected:
      //! write system parameter to output stream
      virtual void write(std::ostream& os) const;

    protected:
      //! arbitrary name of three characters
      std::string Mnam;
      //! initial value of an active parameter
      double Mval;
      //! estimated uncertainty of an active parameter
      double Munc;

  }; // class SystemParameter

  /*=========================================================================*/
  /*!
   * Class calex::GridSystemParameter which provides a calex
   * system parameter that is able to behave both as a
   * optimize::StandardParameter and a calex::SystemParameter. Additionally
   * this class provides the possibility to map calex::SystemParameter and
   * optimize::Standardparameter while it implements both interfaces.\n
   *
   * The advantage of this approach is that functions expecting either a
   * optimize::StandardParameter or a calex::SystemParameter are able to
   * handle as well instances of class calex::GridSystemParameter while
   * using implicit casts. This involves a (simple) compiler technique that
   * ensures that functions expecting a calex::SystemParameter see a
   * different part of calex::GridSystemParameter than do functions
   * expecting a optimize::StandardParameter (TC++PL p.390).
   */
  class GridSystemParameter : public SystemParameter,
                              public opt::StandardParameter<double>
  {
    public:
      //! constructor
      GridSystemParameter(std::string const nam, double const unc,
          std::string const id, double const start, double const end,
          double const delta, std::string const unit="",
          unsigned int coord_id=-1) : 
          Tbase1(nam, start, unc), Tbase2(id, start, end, delta, unit),
          Mcoord_id(coord_id)
      { }
      //! destructor
      virtual ~GridSystemParameter() { }
      //! query function if system parameter is a grid system parameter
      virtual bool is_gridSystemParameter() const { return true; }
      /*!
       * member access function for value
       *
       * \param val new value
       */
      virtual void set_val(double const val) { Tbase1::Mval = val; }
      /*!
       * member access function to set coordinate id while synchronizing the
       * parameter with the coordinate order of the builder
       *
       * \param id coordinate id
       */
      virtual void set_coordinateId(int const id) { Mcoord_id = id; }
      //! query function for coordinate id
      virtual int const& get_coordinateId() const { return Mcoord_id; }

    private:
      //! typedef to first base class
      typedef SystemParameter Tbase1;
      //! typedef to second base class
      typedef opt::StandardParameter<double> Tbase2;
      //! coordinate id for coordinate order of \a liboptimizexx builder
      int Mcoord_id;

  }; // class GridSystemParameter

  /*=========================================================================*/

} // namespace calex

#endif // include guard

/* ----- END OF systemparameter.h  ----- */
