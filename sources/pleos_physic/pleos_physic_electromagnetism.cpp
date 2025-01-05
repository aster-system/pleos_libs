//******************
//
// pleos_physic_electromagnetism.cpp
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
// This file contains the source code of pleos_physic_electromagnetism.h.
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

// Include pleos_physic_electromagnetism.h
#include "../../pleos_physic/pleos_physic_electromagnetism.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {
    // Radius of the produced field in meters
    double Electrical_Charge::electrical_field_force(double distance) {if(distance==0){return 0;}return std::abs((PLEOS_PHYSIC_ELECTROSTATIC_CONSTANT * a_charge) / (distance * distance));}

    // Radius of the magnetic field in V/m
    scls::Vector_3D Electrical_Charge::magnetical_field_force(scls::Vector_3D magnetic_field_effect, Electrical_Charge *other_charge) {
        double force = magnetic_field_effect.norm();
        if(force == 0){return scls::Vector_3D(0,0,0);}

        // Get the other angle
        magnetic_field_effect *= -1;
        double needed_angle_magnetic = scls::vector_2d_angle(magnetic_field_effect.x(), magnetic_field_effect.y());
        while(needed_angle_magnetic > SCLS_PI) {needed_angle_magnetic -= SCLS_PI * 2;}
        while(needed_angle_magnetic < -SCLS_PI) {needed_angle_magnetic += SCLS_PI * 2;}
        // Get the velocity angle
        double needed_angle_velocity = other_charge->magnetical_orientation();
        // Apply the needed modification
        double needed_angle = needed_angle_velocity - needed_angle_magnetic;
        while(needed_angle > SCLS_PI * 2.0) {needed_angle -= SCLS_PI * 2.0;}
        while(needed_angle <= 0) {needed_angle += SCLS_PI * 2.0;}
        // Get the good normal
        if(needed_angle > SCLS_PI){needed_angle -= SCLS_PI * 2.0;}

        // Get the final vector
        scls::Vector_3D to_return = other_charge->velocity(); to_return.set_z(to_return.y());
        to_return.rotate(scls::Vector_3D(0, needed_angle/force, 0)); to_return.set_y(to_return.z()); to_return.set_z(0);

        return to_return;
    }

    // Orientation of the magnetical field
    double Electrical_Charge::magnetical_orientation(scls::Vector_3D referential) {
        // Get the angle
        double needed_angle_other = scls::vector_2d_angle(velocity().x() - referential.x(), velocity().y() - referential.y());
        while(needed_angle_other > SCLS_PI) {needed_angle_other -= SCLS_PI * 2;}
        while(needed_angle_other < -SCLS_PI) {needed_angle_other += SCLS_PI * 2;}
        return needed_angle_other;
    }
}
