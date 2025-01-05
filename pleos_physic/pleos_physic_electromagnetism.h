//******************
//
// pleos_physic_electromagnetism.h
//
//******************
// Presentation :
//
// PLEOS, by Aster System, is a project which aims education.
// By using the power of SCLS and other Aster System's tools, we created  this.
// We want to make education easier for everyone (teachers, as students and pupils).
// The software is made in french, because the main goal is France educational system.
// For more information, see : https://aster-system.github.io/aster-system/projects/pleos.html.
//
// This file contains the handling of electromagnetism in the PLEOS physic file.
//
//******************
//
// License (GPL V3.0) :
//
// Copyright (C) 2024 by Aster System, Inc. <https://aster-system.github.io/aster-system/>
// This file is part of PLEOS.
// PLEOS is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// PLEOS is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with PLEOS. If not, see <https://www.gnu.org/licenses/>.
//

#ifndef PLEOS_PHYSIC_ELECTROMAGNETISM
#define PLEOS_PHYSIC_ELECTROMAGNETISM

// Include SCLS Graphic Benoit
#include "pleos_physic_core.h"

#define PLEOS_PHYSIC_ELECTROSTATIC_CONSTANT (8.988e9)
#define PLEOS_PHYSIC_ELECTRICAL_CHARGE ((1.0)/(6.24150962e18))
// In Teslas meter / amperes
#define PLEOS_PHYSIC_VACUUM_PERMEABILITY ((1.2566)/(1e6))
#define PLEOS_PHYSIC_VACUUM_PERMITTIVITY ((1.0)/(8.85418782e12))

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //******************
    //
    // The Particules class
    //
    //******************

    class Electrical_Charge : public Physic_Object {
        // Class representating a electrical charge
    public:

        // Electrical_Charge constructor
        Electrical_Charge(double charge):Electrical_Charge(){a_charge=charge;};
        Electrical_Charge(scls::Fraction charge):Electrical_Charge(charge.to_double()){};
        Electrical_Charge():Physic_Object(){};

        // Radius of the electrical field in meters
        double electrical_field_force(double distance);
        inline double electrical_field_force(scls::Vector_3D other_position){return electrical_field_force(distance(other_position));};
        inline double electrical_field_force(Electrical_Charge *other_charge){return electrical_field_force(distance(other_charge->attached_transform()->position())) * std::abs(other_charge->a_charge);};
        // Radius of the magnetic field in V/m
        static scls::Vector_3D magnetical_field_force(scls::Vector_3D magnetic_field_effect, Electrical_Charge *other_charge);
        // Orientation of the magnetical field
        double magnetical_orientation(scls::Vector_3D referential);
        inline double magnetical_orientation(){return magnetical_orientation(scls::Vector_3D(0, 0, 0));};
        // Volume of the particule
        inline double volume() const {return ((4.0 * SCLS_PI * a_width * a_width * a_width)/3.0);};

        // Getters and setters
        inline double charge() const {return a_charge;};
        inline void set_charge(double new_charge) {a_charge = new_charge;};

    private:
        // Charge of the particule in Coulomb
        double a_charge = 1;
        // Width of the particule in meters
        double a_width = 0.05;
    };
}

#endif // PLEOS_PHYSIC_ELECTROMAGNETISM
