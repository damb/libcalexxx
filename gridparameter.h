/*! \file gridparameter.h
 * \brief Declares a specific parameter space parameter for a calex grid by
 * inheritenance of a liboptimizexx parameter.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 22/03/2012
 * 
 * Purpose: Declares a specific parameter space parameter for a calex grid by
 * inheritenance of a liboptimizexx parameter.  
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
 * 22/03/2012  V0.1  Daniel Armbruster
 * 
 * ============================================================================
 */
 
#include <string>
#include <liboptimizexx/parameter.h>

namespace calex
{
  /* ======================================================================= */
  /*
   * Instead of a usual liboptimizexx Parameter a calex::CalexParameter provides
   * an additional member to store the calexId of the parameter
   *
   * \todo {How to avoid ambiguities with various \a per system parameters?}
   */
  template <typename Ctype>
  class CalexParameter : public optimize::Parameter<Ctype>
  {
    public:
      //! constructor
      CalexParameter(Ctype start, Ctype end, Ctype delta,
          std::string const id="", std::string const unit="",
          std::string const calexid="") : Tbase(start, end, delta, id, unit),
          McalexId(calexId.substr(0,3))
      { }
      //! query function for calexId
      std::string const& get_calexId() const { return McalexId; }

    private:
      //! typedef to base class
      typedef optimize::Parameter<Ctype> Tbase;
      //! calex system parameter id
      std::string McalexId;

  }; // class template CalexParameter

} // namespace calex

/* ----- END OF gridparameter.h  ----- */
