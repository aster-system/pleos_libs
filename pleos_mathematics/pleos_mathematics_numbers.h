//******************
//
// pleos_mathematics_numbers.h
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
// This file contains some features to understand number theory in the PLEOS mathematics file.
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

#ifndef PLEOS_MATHEMATICS_NUMBERS
#define PLEOS_MATHEMATICS_NUMBERS

// Include SCLS Graphic Benoit
#include "../../../scls-graphic-benoit/scls_graphic.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {
    // Analysed numbers
    struct Number {char a_is_prime = -1;bool is_prime()const{return a_is_prime == 1;}; int number = 0;};;
    extern std::shared_ptr<std::vector<std::shared_ptr<Number>>> numbers;
    extern std::shared_ptr<std::vector<std::shared_ptr<Number>>> prime_numbers;
    // Returns an analysed number
    Number* number(long long position);

    // Arithmetic object
    struct Arithmetic_Object{
        std::shared_ptr<scls::GUI_Text>connected_object;
        std::string choice=std::string();
        inline std::string name()const{return connected_object.get()->name();};

        // Each values of the Euclid algorithm
        std::vector<int> euclid_algorithm_value = std::vector<int>();

        // Needed choice
        bool choice_1 = false;
        // Needed value
        scls::__Formula value_1;
        scls::__Formula value_2;

        // Possible input
        std::shared_ptr<scls::GUI_Text_Input> input_1;
        std::shared_ptr<scls::GUI_Text_Input> input_2;
        // Possible choices
        std::shared_ptr<scls::GUI_Scroller_Choice> choice_input_1;
    };

    // Returns a division circle
    // This function is inspired by this (french) video from Mickael Launay : https://youtu.be/-X49VQgi86E?si=wvdvNiM0ZBgUUii4.
    std::shared_ptr<scls::__Image_Base> division_circle(int image_width, int circle_radius, double modulo, int point_number);

    // Calculate the GCD of two numbers
    long long arithmetic_gcd(Arithmetic_Object* object, std::string* redaction);
    // Calculate the Bezout identity of two numbers
    void arithmetic_bezout_identity(Arithmetic_Object* object, std::string* redaction);
    // Calculate the decomposition of a number
    void arithmetic_decomposition(Arithmetic_Object* object, std::string* redaction);

    // Returns an erathostene sieve
    scls::Image erathostene_sieve(int image_width, int start_number, int end_number);

    // Converts an algebric compelx number to this polar form
    scls::Complex_Polar complex_algebric_to_polar(scls::Complex c, std::string* redaction);

    // Get the root of a complex number
    void complex_root(scls::Complex c, int n, std::string* redaction);
}

#endif // PLEOS_MATHEMATICS_NUMBERS
