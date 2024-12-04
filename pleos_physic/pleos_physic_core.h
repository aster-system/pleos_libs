//******************
//
// pleos_physic_core.h
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
// This file contains the core of PLEOS physic file.
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

#ifndef PLEOS_PHYSIC_CORE
#define PLEOS_PHYSIC_CORE

// Include SCLS Graphic Benoit
#include "../../../scls-graphic-benoit/scls_graphic.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {
    //******************
    //
    // The Physic_Object class
    //
    //*****************

    class __Transform_Container {
        // Class representating an object which contains a Transform_Object_3D
    public:
        // __Transform_Container constructor
        __Transform_Container():a_attached_transform(std::make_shared<scls::Transform_Object_3D>()){a_attached_transform.get()->set_this_object(a_attached_transform);};

        // Returns the distance between a point and this object
        inline double distance(scls::Vector_3D point){return attached_transform()->distance(point);};
        inline double distance(__Transform_Container* container){return attached_transform()->distance(container->attached_transform()->position());};

        // Handle position
        inline void set_x(double new_x){attached_transform()->set_x(new_x);};
        inline void set_y(double new_y){attached_transform()->set_y(new_y);};
        inline void set_z(double new_z){attached_transform()->set_z(new_z);};

        // Returns a vector to an object
        inline scls::Vector_3D vector_to(scls::Vector_3D object) {return attached_transform()->vector_to(object);};
        inline scls::Vector_3D vector_to(__Transform_Container* container) {return attached_transform()->vector_to(container->attached_transform()->position());};

        // Getters and setters
        inline scls::Transform_Object_3D* attached_transform() {return a_attached_transform.get();};
    private:
        // Transformation of the object
        std::shared_ptr<scls::Transform_Object_3D> a_attached_transform;
    };

    class Physic_Object : public __Transform_Container {
        // Class representating a physic object in PLEOS
    public:
        // Physic_Object constructor
        Physic_Object():__Transform_Container(){};

        // Accelerate the object
        inline void accelerate(scls::Vector_3D acceleration){a_velocity+=acceleration;};
        // Simulate the physic of the object
        void simulate_physic(double delta_time);

        // Getters and setters
        inline double mass() const {return a_mass;};
        inline void set_mass(double new_mass) {a_mass = new_mass;};
        inline scls::Vector_3D velocity() {return a_velocity;};

    private:
        // Mass of the object in kg
        double a_mass = 1;
        // Velocity of the object
        scls::Vector_3D a_velocity;
    };
}

#endif // PLEOS_PHYSIC_CORE
