/*! \file subsystem.h
 * \brief Declaration of a subsystem class.
 * 
 * ----------------------------------------------------------------------------
 * 
 * $Id$
 * \author Daniel Armbruster
 * \date 15/03/2012
 * 
 * Purpose: Declaration of a subsystem class.  
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
 * 15/03/2012   V0.1  Daniel Armbruster
 * 25/04/2012   V0.2  Make use of smart poiters and C++0x.
 * 
 * ============================================================================
 */
 
#include <memory>
#include <calexxx/systemparameter.h>
#include <calexxx/error.h>

#ifndef _CALEX_SUBSYSTEM_H_
#define _CALEX_SUBSYSTEM_H_

namespace calex
{
  //! type of calex subsystems
  enum EsubSystemType
  {
    LP, //!< low-pass subsystem
    HP, //!< high-pass subsystem
    BP  //!< band-pass subsystem
  }; // enum EsubSystemType

  /*=========================================================================*/
  /*!
   * \brief class defining a calex subsystem
   * Abstract base class for any kind of calex subsystem. Subsystems will be
   * added to calex configuration / parameter file.\n
   *
   * System parameter query function return a reference (not constant) to the
   * members the subsystem contains. This is no problem cause only system
   * parameters can be modified if they are instances of
   * calex::GridSystemParameter.
   *
   * Abstract class of template method design pattern (GoF p.325).
   */
  class CalexSubsystem
  {
    public:
      //! destructor
      virtual ~CalexSubsystem() { }
      //! query function for system type
      EsubSystemType get_type() const { return Mtype; }
      //! query function for period uncertainty
      virtual std::shared_ptr<SystemParameter> get_per() { return Mper; }
      //! query function for damping uncertainty
      virtual std::shared_ptr<SystemParameter> get_dmp() 
      { CALEX_abort("Illegal function call."); }
      //! query function of order of subsystem
      virtual unsigned int get_order() const = 0;
      /*! 
       * output stream operator
       *
       * Template method design pattern in use (GoF p.325). Template method of
       * the pattern.
       */
      friend std::ostream& operator<<(
          std::ostream& os, CalexSubsystem const& subsys);

    protected:
      //! constructor
      CalexSubsystem(EsubSystemType type, std::shared_ptr<SystemParameter> per);

      /*!
       * write to output stream
       *
       * Note that template method design pattern is in use (GoF p.325).
       * Primitive operation of the pattern.
       */
      virtual void write(std::ostream& os) const = 0;

      //! template

    protected:
      // subsystem type
      EsubSystemType Mtype;
      //! period system parameter
      std::shared_ptr<SystemParameter> Mper;

  }; // class CalexFilter

  /*=========================================================================*/
  class FirstOrderSubsystem : public CalexSubsystem
  {
    public:
      /*!
       * constructor
       *
       * \param type Subsystem type, which must be of type EsubSystemType::LP or
       * EsubSystemType::HP.
       * \param period system parameter
       */
      FirstOrderSubsystem(EsubSystemType type,
          std::shared_ptr<SystemParameter> per);
      //! destructor
      virtual ~FirstOrderSubsystem() { }
      //! query function of order of subsystem
      virtual unsigned int get_order() const { return 1; }

    protected:
      /*!
       * write to output stream
       *
       * Primitive operation of a concrete class of template method design
       * pattern.
       */
      virtual void write(std::ostream& os) const;

    private:
      typedef CalexSubsystem Tbase;

  }; // class FirstOrderFilter

  /*=========================================================================*/
  class SecondOrderSubsystem : public CalexSubsystem
  {
    public:
      //! constructor
      SecondOrderSubsystem(EsubSystemType type,
          std::shared_ptr<SystemParameter> per,
          std::shared_ptr<SystemParameter> dmp);
      //! destructor
      virtual ~SecondOrderSubsystem() { }
      //! query function of order of subsystem
      virtual unsigned int get_order() const { return 2; }
      //! query function for damping system parameter
      virtual std::shared_ptr<SystemParameter> get_dmp() { return Mdmp; }

    protected:
      /*!
       * write to output stream
       *
       * Primitive operation of a concrete class of template method design
       * pattern.
       */
      virtual void write(std::ostream& os) const;

    private:
      typedef CalexSubsystem Tbase;
      //! system parameter damping
      std::shared_ptr<SystemParameter> Mdmp;

  }; // class SecondOrderFilter

  /*=========================================================================*/

} // namespace calex

#endif // include guard

/* ----- END OF subsystem.h  ----- */
