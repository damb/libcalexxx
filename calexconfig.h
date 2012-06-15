/*! \file calexconfig.h
 * \brief Declaration of a generator class for a calex parameter file.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 14/03/2012
 * 
 * Purpose: Declaration of a generator class for a calex parameter file.  
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
 * 14/03/2012   V0.1  Daniel Armbruster
 * 15/05/2012   V0.2  Query function for grid system parameter names provided
 * 
 * ============================================================================
 */
 
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <ostream>
#include <algorithm>
#include <optimizexx/application.h>
#include <calexxx/systemparameter.h>
#include <calexxx/subsystem.h>
#include <calexxx/resultdata.h>
#include <calexxx/parser.h>
#include <calexxx/defaults.h>
#include <optimizexx/globalalgorithm.h>
#include <optimizexx/parameter.h>

#ifndef _CALEX_CALEXCONFIG_H_
#define _CALEX_CALEXCONFIG_H_

namespace opt = optimize;

namespace calex
{

  /*=========================================================================*/
  /*!
   * Class representing a calex parameter configuration file.\n
   *
   * \note Notice that only either CalexConfig::Mdmp or CalexConfig::Msub can
   * be set.
   *
   * \todo Synchronizing class calex::CalexConfig several times does not work
   * yet. This especially is a problem if using other global algorithms which
   * make use of refining the parameter space and adding subgrids.\n
   *
   * 
   */
  class CalexConfig
  {
    public:
      //! constructor
      CalexConfig(std::string const infile, std::string const outfile);
      //! destructor
      ~CalexConfig() { }
      // member access functions
      void set_comment(std::string const comment) { Mcomment = comment; }
      void set_alias(float const alias) { Malias = alias; }
      void set_m0(int m0) { Mm0 = m0; }
      void set_maxit(unsigned int const maxit) { Mmaxit = maxit; }
      void set_qac(double const qac) { Mqac = qac; }
      void set_finac(double const finac) { Mfinac = finac; }
      void set_ns1(int const ns1) { Mns1 = ns1; }
      void set_ns2(int const ns2) { Mns2 = ns2; }
      //! member access functions
      void set_amp(std::shared_ptr<SystemParameter> amp);
      void set_del(std::shared_ptr<SystemParameter> del);
      void set_sub(std::shared_ptr<SystemParameter> sub);
      void set_til(std::shared_ptr<SystemParameter> til);
      //! add calex system parameters to calex parameter files
      void add_systemParameter(std::shared_ptr<SystemParameter> param);
      //! add a calex subsystem configuration to the calex parameter file
      void add_subsystem(std::shared_ptr<CalexSubsystem> subsys);
      //! remove all subsystems of configuration
      void clear_subsystems();
      //! remove subsystem specified
      void remove_subsystem(std::shared_ptr<CalexSubsystem> subsys);
      /*!
       * query function for GridSystemParameters
       *
       * \param algo global \a liboptimizexx algorithms to store parameter space
       * parameters
       */
      template <typename Ctype>
      void set_gridSystemParameters(typename
          opt::GlobalAlgorithm<Ctype, CalexResult>& algo);
      /*!
       * update of system parameters which are instances of
       * calex::GridSystemParameter
       *
       * \param data new data
       */
      template <typename Ctype>
      void update(typename std::vector<Ctype> const& data);
      /*!
       * Synchronize the calex parameter file with class
       * calex::CalexApplication.\n
       * Call this function if all parameters had been added before the visitor
       * class calex::CalexApplication will be send through the parameter
       * space.
       *
       * \param algo Global algorithm in use.
       */
      template <typename Ctype>
      void synchronize(opt::GlobalAlgorithm<Ctype, CalexResult>& algo);
      /*!
       * query function for grid system parameter names
       * 
       * \return vector containing grid system parameter names in the correct
       * order the parameter space uses the parameters
       */
      template <typename Ctype>
      std::vector<std::string> get_gridSystemParameterNames(
          opt::GlobalAlgorithm<Ctype, CalexResult>& algo);
      /*
       * check if configuration file has grid system parameters
       */
      bool hasGridSystemParameters();
      //! member query functions
      SystemParameter const& get_amp() const { return *Mamp; }
      SystemParameter const& get_del() const { return *Mdel; }
      SystemParameter const& get_sub() const { return *Msub; }
      SystemParameter const& get_til() const { return *Mtil; }
      std::vector<std::shared_ptr<CalexSubsystem>> const& 
        get_subsystems() const;

      //! overloaded ostream operator
      friend std::ostream& operator<<(
          std::ostream& os, CalexConfig const& config);

    protected:
      //! ostream function
      virtual void write(std::ostream&) const;

    private:
      /*! 
       * query function for grid system parameters
       *
       * \param param_vec vector to store grid system parameters
       *
       * \note Within this function using the \c dynamic_cast operator was
       * necessary because downcast from pointers of class
       * calex::SystemParameter to class calex::GridSystemParameter does
       * take place.
       */
      void get_gridSystemParameters(
          std::vector<std::shared_ptr<GridSystemParameter>>& param_vec);

    private:
      //! header line of calex parameter file
      std::string Mcomment;
      //! filename of file containing the calibration signal data e.g. sweep
      std::string Minfile;
      //! filename of file containing the output of the seismometer
      std::string Moutfile;
      //! corner period of the numerical anti-alias filter
      float Malias;
      //! number of active (unknown) parameters in the inversion
      unsigned int Mm;
      //! number of additional powers of the Laplace variable \a s
      unsigned int Mm0;
      /*!
       * number of first-order high-pass or low-pass subsystems in the transfer
       * function
       */
      unsigned int Mm1;
      /*!
       * number of second-order high-pass, low-pass or band-pass subsystems in
       * the transfer function
       */
      unsigned int Mm2;
      /*!
       * the mayimum number of iterations in the conjugate-gradient
       * optimization procedure
       */
      unsigned int Mmaxit;
      /*!
       * iteration stops when improvement in the rms misfit in one step becomes
       * less than \a qac
       */
      double Mqac;
      /*!
       * iteration stops when normalized parameters change by less than \a finac
       */
      double Mfinac;
      //! start analyzing the data after \a ns1 samples
      int Mns1;
      //! finish analyzing the data at \a ns2 samples from end
      int Mns2;
      //! gain factor (required system parameter)
      std::shared_ptr<SystemParameter> Mamp;
      //! time delay (required system parameter)
      std::shared_ptr<SystemParameter> Mdel;
      /*!
       * fraction sub - will be subtracted from the output to numerically
       * simulate a full bridge if a half-bridge circuit was used. Set start
       * value and uncertainty to zero if no bridge circuit was used.
       * (required system parameter)
       */
      std::shared_ptr<SystemParameter> Msub;
      /*!
       * This parameter permits to subtract \a til times the twice integrated
       * synthetic output from the synthetic output signal, to compensate for
       * the tilt of a shake table. Set start value and uncertainty to zero if
       * you did not use a shake table.\n
       * \a til is in units of microradians per millimeter of the table motion
       * provided that the motion of the table, the gravitational force coupled
       * in by tilt and the sensitive axis of the seismometer have the same
       * direction. More generally, \a til (tilt along the sensitice axis) per
       * (table displacement along the sensitive axis), in the above units
       * (required system parameter)
       */
      std::shared_ptr<SystemParameter> Mtil;
      /*!
       * Further system parameters.
       *
       * CalexConfig::Mdel can be replaced by CalexConfig::Msub system
       * parameter.
       * Specifies a fraction of the input signal that is subtracted from the
       * output signal. Necessary if e.g. a geophone has a separate calibration
       * coil.
       */
      std::vector<std::shared_ptr<SystemParameter>> MsystemParameters;
      //! vector of calex subsystems of the calex configuration
      std::vector<std::shared_ptr<CalexSubsystem>> MsubSystems;
      //! flag to save the state of synchronization
      bool MisSynchronized;

  }; // class CalexConfig

  /*=========================================================================*/
  template <typename Ctype>
  void CalexConfig::update(typename std::vector<Ctype> const& data)
  {
    CALEX_assert(MisSynchronized,
        "Parameters not synchronized.");
    std::vector<std::shared_ptr<GridSystemParameter>> grid_params;
    get_gridSystemParameters(grid_params);
    CALEX_assert(data.size() == grid_params.size(),
        "Invalid parameter configuration.");
    for (size_t idx=0; idx < data.size(); ++idx)
    {
      for (auto it(grid_params.begin()); it != grid_params.end(); ++it)
      {
        if(idx == static_cast<size_t>((*it)->get_coordinateId()))
        {
          (*it)->set_val(data[idx]);
          break;
        }
      }
    }
  } // function template CalexConfig::update

  /*-------------------------------------------------------------------------*/
  template <typename Ctype>
  void CalexConfig::synchronize(opt::GlobalAlgorithm<Ctype, CalexResult>& algo)
  {
    CALEX_assert(algo.getParameterSpaceDimensions() != 0,
        "Parameters not yet assigned to liboptimizexx global algorithm.")
    // fetch parameter order vector of parameters of builder
    std::vector<int> order(algo.getParameterSpaceBuilder().getParameterOrder(
        algo.getParameterSpaceDimensions()));
    // synchronize grid system parameters
    std::vector<std::shared_ptr<GridSystemParameter>> grid_params;
    get_gridSystemParameters(grid_params);
    CALEX_assert(order.size() == grid_params.size(),
        "Invalid liboptimizexx parameter configuration.");
    for (size_t j = 0; j < order.size(); ++j)
    {
      grid_params.at(j)->set_coordinateId(order.at(j));
    }
    MisSynchronized = true;
  } // function template CalexConfig::synchronize

  /*-------------------------------------------------------------------------*/
  template <typename Ctype>
  void CalexConfig::set_gridSystemParameters(typename
      opt::GlobalAlgorithm<Ctype, CalexResult>& algo)
  {
    std::vector<std::shared_ptr<GridSystemParameter>> grid_params;
    get_gridSystemParameters(grid_params);
    CALEX_assert(grid_params.size() != 0,
       "No grid system parameters assigned.");
    for (auto it(grid_params.begin()); it != grid_params.end(); ++it)
    {
      algo.addParameter(*it);
    }
  } // function template CalexConfig::set_gridSystemParameters

  /*-------------------------------------------------------------------------*/
  template <typename Ctype>
  std::vector<std::string> CalexConfig::get_gridSystemParameterNames(
      typename opt::GlobalAlgorithm<Ctype, CalexResult>& algo)
  {
    std::vector<std::shared_ptr<GridSystemParameter>> grid_params;
    get_gridSystemParameters(grid_params);
    // convert pointers implicitly to usual calex system parameter pointers
    // to make use of the get_nam() query function
    std::vector<std::shared_ptr<SystemParameter>> params(grid_params.size());
    copy(grid_params.begin(), grid_params.end(), params.begin());
    // fetch parameter order vector of parameters of builder
    std::vector<int> order(algo.getParameterSpaceBuilder().getParameterOrder(
        algo.getParameterSpaceDimensions()));
    std::vector<std::string> grid_param_names(order.size());
    for (size_t j = 0; j < order.size(); ++j)
    {
      grid_param_names[j] = params.at(order.at(j))->get_nam();
    }
    return grid_param_names;
  }

  /*-------------------------------------------------------------------------*/

} // namespace calex

#endif

/* ----- END OF calexconfig.h  ----- */
