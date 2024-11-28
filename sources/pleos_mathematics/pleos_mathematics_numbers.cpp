//******************
//
// pleos_mathematics_numbers.cpp
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
// This file contains the source code of pleos_mathematics_numbers.cpp.
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

// Include pleos_mathematics_numbers.h
#include "../../pleos_mathematics/pleos_mathematics_numbers.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {
    // Returns a division circle
    // This function is inspired by this (french) video from Mickael Launay : https://youtu.be/-X49VQgi86E?si=wvdvNiM0ZBgUUii4.
    std::shared_ptr<scls::Image> division_circle() {
        int width = 2000;
        std::shared_ptr<scls::Image> to_return = std::make_shared<scls::Image>(width, width, scls::Color(255, 255, 255));
        to_return.get()->draw_circle(width / 2, width / 2, 960, 0, 0, 0, 255, 10);

        return to_return;
    }
}
