/*! \file resultdata.h
 * \brief Datatype declaration to store the result data for the liboptimizexx
 * parameter space.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 15/03/2012
 * 
 * Purpose: Datatype declaration to store the result data for the liboptimizexx
 * parameter space.
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
 * 15/03/2012   V0.1    Daniel Armbruster
 * 15/05/2012   V0.2    provide member function to write header information to
 *                      an outputstream
 * 14/06/2012   V0.3    Bug fix parsing a calex *.out file - amp and del system
 *                      parameters from now on are deprecated
 * 
 * ============================================================================
 */
 
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <calexxx/error.h>

#ifndef _CALEX_RESULTDATA_H_
#define _CALEX_RESULTDATA_H_

namespace calex
{
  /*!
   * Datatype to store the result data after calculating the residuals with
   * Erhard Wielandt's calex program.
   *
   * \todo \c sub and \c til parameters were not printed yet.
   *
   */
  class CalexResult
  {
    public:
      typedef std::vector<std::pair<std::string, double>> TsystemParameters;
    public:
      //! constructor
      CalexResult() : Mcomputed(false), Miter(0), Mrms(0)
      { }
      //! constructor
      CalexResult(unsigned int const iter, double const rms, 
        TsystemParameters const params) : Mcomputed(true), Miter(iter),
        Mrms(rms), MsystemParameters(params)
      { }

      //! query function if entire data had been set
      bool isComputed() const { return Mcomputed; }
      //! query function for number of iterations
      unsigned int const& get_iter() const { return Miter; }
      //! query function for root mean square
      double const& get_rms() const { return Mrms; }
      //! query function for additional system parameters
      std::vector<std::pair<std::string, double>> const& get_systemParameters()
        const;
      //! write the calex result data to an outputstream
      void writeLine(std::ostream& os) const;
      //! write header information to an outputstream
      void writeHeaderInfo(std::ostream& os) const;

      //! input stream operator
      friend std::istream& operator>>(std::istream& is, CalexResult& result);
      //! output stream operator
      friend std::ostream& operator<<(
          std::ostream& os, CalexResult const& result);

    protected:
      //! read the calex result data from an inputstream
      void read(std::istream& is);
      //! write the calex result data to an outputstream
      void write(std::ostream& os) const;

    private:
      //! flag if member data had been set
      bool Mcomputed;
      //! number of iterations
      unsigned int Miter;
      //! root mean square
      double Mrms;
      //! additional result system parameters
      TsystemParameters MsystemParameters;

  }; // class CalexResult

} // namespace calex

#endif // include guard

/* ----- END OF resultdata.h  ----- */
