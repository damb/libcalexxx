/*! \file defaults.h
 * \brief Defining defaults for calex parameter file.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 15/03/2012
 * 
 * Purpose: Defining defaults for calex parameter file.  
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
 
// default parameters for STS-2 seismometer
#ifdef STS2
  #define CALEX_ALIAS 1.00
  #define CALEX_M0 0
  #define CALEX_MAXIT 1
  #define CALEX_QAC 1e-5 
  #define CALEX_FINAC 1e-3 
  #define CALEX_NS1 0
  #define CALEX_NS2 0
  #define CALEX_AMP -41.5 
  #define CALEX_AMPUNC  5.1
  #define CALEX_DEL 0.01 
  #define CALEX_DELUNC 0.001
  #define CALEX_SUB  0.
  #define CALEX_SUBUNC 0.
  #define CALEX_TIL 0.
  #define CALEX_TILUNC 0.
#elif STS1
  #define CALEX_ALIAS 1.00
  #define CALEX_M0 0
  #define CALEX_MAXIT 1
  #define CALEX_QAC 1e-5 
  #define CALEX_FINAC 1e-2 
  #define CALEX_NS1 0
  #define CALEX_NS2 0
  #define CALEX_AMP 1.3
  #define CALEX_AMPUNC  0.10
  #define CALEX_DEL 0.01
  #define CALEX_DELUNC 0.01
  #define CALEX_SUB  0.
  #define CALEX_SUBUNC 0.
  #define CALEX_TIL 0.
  #define CALEX_TILUNC 0.
#elif GEOPHONE_10HZ
  #define CALEX_ALIAS 0.05
  #define CALEX_M0 0
  #define CALEX_MAXIT 1
  #define CALEX_QAC 1e-5 
  #define CALEX_FINAC 1e-3 
  #define CALEX_NS1 0
  #define CALEX_NS2 0
  #define CALEX_AMP 1.2
  #define CALEX_AMPUNC  0.2
  #define CALEX_DEL 0.
  #define CALEX_DELUNC 0.0
  #define CALEX_SUB  0.5
  #define CALEX_SUBUNC 0.2
  #define CALEX_TIL 0.
  #define CALEX_TILUNC 0.
// if no flag had been specified use STS-2 default parameters as defaults
#else 
  #define CALEX_ALIAS 1.00
  #define CALEX_MAXIT 1
  #define CALEX_QAC 1e-5 
  #define CALEX_FINAC 1e-3 
  #define CALEX_NS1 0
  #define CALEX_NS2 0
  #define CALEX_AMP -41.5 
  #define CALEX_AMPUNC  5.1
  #define CALEX_DEL 0.01 
  #define CALEX_DELUNC 0.001
  #define CALEX_SUB  0.
  #define CALEX_SUBUNC 0.
  #define CALEX_TIL 0.
  #define CALEX_TILUNC 0.
#endif // include guard

/* ----- END OF defaults.h  ----- */
