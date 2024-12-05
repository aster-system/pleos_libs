//******************
//
// pleos_physic_core.cpp
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
// This file contains the source code of pleos_physic_core.h.
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

// Include pleos_physic_core.h
#include "../../pleos_physic/pleos_physic_core.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {
    // Simulate the physic of the object
    void Physic_Object::simulate_physic(double delta_time) {
        // Handle the velocity
        if(!x_fixed()) {attached_transform()->move_x(velocity().x() * delta_time);}
        if(!y_fixed()) {attached_transform()->move_y(velocity().y() * delta_time);}
        if(!z_fixed()) {attached_transform()->move_z(velocity().z() * delta_time);}
    }
}
