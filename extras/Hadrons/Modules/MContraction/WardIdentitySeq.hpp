/*************************************************************************************

Grid physics library, www.github.com/paboyle/Grid 

Source file: extras/Hadrons/Modules/MContraction/WardIdentitySeq.hpp

Copyright (C) 2017

Author: Andrew Lawson    <andrew.lawson1991@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

See the full license in the file "LICENSE" in the top level distribution directory
*************************************************************************************/
/*  END LEGAL */

#ifndef Hadrons_WardIdentitySeq_hpp_
#define Hadrons_WardIdentitySeq_hpp_

#include <Grid/Hadrons/Global.hpp>
#include <Grid/Hadrons/Module.hpp>
#include <Grid/Hadrons/ModuleFactory.hpp>

BEGIN_HADRONS_NAMESPACE

/*
  Ward Identity contractions using sequential propagators.
 -----------------------------
 
 * options:
 - q_x: propagator, mu = x current insertion (string).
 - q_y: propagator, mu = y current insertion (string). 
 - q_z: propagator, mu = z current insertion (string).
 - q_t: propagator, mu = t current insertion (string).
*/

/******************************************************************************
 *                            WardIdentitySeq                                 *
 ******************************************************************************/
BEGIN_MODULE_NAMESPACE(MContraction)

class WardIdentitySeqPar: Serializable
{
public:
    GRID_SERIALIZABLE_CLASS_MEMBERS(WardIdentitySeqPar,
                                    std::string, q_x,
                                    std::string, q_y,
                                    std::string, q_z,
                                    std::string, q_t);
};

template <typename FImpl>
class TWardIdentitySeq: public Module<WardIdentitySeqPar>
{
public:
    TYPE_ALIASES(FImpl,);
public:
    // constructor
    TWardIdentitySeq(const std::string name);
    // destructor
    virtual ~TWardIdentitySeq(void) = default;
    // dependency relation
    virtual std::vector<std::string> getInput(void);
    virtual std::vector<std::string> getOutput(void);
    // setup
    virtual void setup(void);
    // execution
    virtual void execute(void);
};

MODULE_REGISTER_NS(WardIdentitySeq, TWardIdentitySeq<FIMPL>, MContraction);

/******************************************************************************
 *                 TWardIdentitySeq implementation                             *
 ******************************************************************************/
// constructor /////////////////////////////////////////////////////////////////
template <typename FImpl>
TWardIdentitySeq<FImpl>::TWardIdentitySeq(const std::string name)
: Module<WardIdentitySeqPar>(name)
{}

// dependencies/products ///////////////////////////////////////////////////////
template <typename FImpl>
std::vector<std::string> TWardIdentitySeq<FImpl>::getInput(void)
{
    std::vector<std::string> in = {par().q_x, par().q_y, par().q_z, par().q_t};
    
    return in;
}

template <typename FImpl>
std::vector<std::string> TWardIdentitySeq<FImpl>::getOutput(void)
{
    std::vector<std::string> out = {getName()};
    
    return out;
}

// setup ///////////////////////////////////////////////////////////////////////
template <typename FImpl>
void TWardIdentitySeq<FImpl>::setup(void)
{
    
}

// execution ///////////////////////////////////////////////////////////////////
template <typename FImpl>
void TWardIdentitySeq<FImpl>::execute(void)
{
    LatticeComplex  vector_WI(env().getGrid()), c(env().getGrid());
    PropagatorField q_x = *env().template getObject<PropagatorField>(par().q_x);
    PropagatorField q_y = *env().template getObject<PropagatorField>(par().q_y);
    PropagatorField q_z = *env().template getObject<PropagatorField>(par().q_z);
    PropagatorField q_t = *env().template getObject<PropagatorField>(par().q_t);
    PropagatorField *q[Nd] = {&q_x, &q_y, &q_z, &q_t};
    Gamma           g5(Gamma::Algebra::Gamma5);

    // Check D_mu V_mu = 0
    for (unsigned int mu = 0; mu < Nd; ++mu)
    {
        c = trace(g5*(*q[mu]));
        vector_WI += c - Cshift(c, mu, -1);
    }

    LOG(Message) << "Ward Identity checks for sequential vector current "
                 << "insertion = " << norm2(vector_WI) << std::endl;
}

END_MODULE_NAMESPACE

END_HADRONS_NAMESPACE

#endif // Hadrons_WardIdentitySeq_hpp_
