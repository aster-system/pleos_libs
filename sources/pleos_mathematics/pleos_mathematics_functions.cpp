//******************
//
// pleos_mathematics_functions.cpp
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
// This file contains the source code of pleos_mathematics_functions.h.
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

// Include pleos_mathematics_functions.h
#include "../../pleos_mathematics/pleos_mathematics_functions.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {
    //******************
    //
    // Polynomial handling
    //
    //******************

    // Gets the roots of a polynomial
    std::string __redaction_root_0 = std::string("Le polynôme \"<full_formula>\" est constamment égal à 0, il admet tout son ensemble de définition comme racine.");
    std::string __redaction_root_0_not_0 = std::string("Le polynôme \"<full_formula>\" est constant, il n'a pas de racines.");
    std::string __redaction_root_1 = std::string("Le polynôme \"<full_formula>\" est linéaire, il admet comme solution : <math><mi>s</mi><mo>=</mo><mfrac><mi>-b</mi><mi>a</mi></mfrac><mo>=</mo><mfrac><mi>-<polynomial_0></mi><mi><polynomial_1></mi></mfrac><mo>=</mo><mi><solution_0></mi></math>");
    std::string __redaction_root_2_d = std::string("Le polynôme \"<full_formula>\" est de degré 2, utilisons la formule du discriminant : </br><math><mdelta><mo>=</mo><mi>b</mi><msup>2</msup><mi>-4ac</mi><mo>=</mo><polynomial_1><msup>2</msup><mo>-</mo><mi>4</mi><mo>*</mo><polynomial_2><mo>*</mo><polynomial_0><mo>=</mo><mi><solution_d></mi></math></br>");
    std::string __redaction_root_2_2r = std::string("Le discriminant est supérieur à 0, nous pouvons donc dénombrer 2 solutions distinctes : </br><math><mi>x</mi><msub>1</msub><mo>=</mo><mfrac><mrow><mi>-</mi><polynomial_1><mo>+</mo><msqrt><solution_d></msqrt></mrow><mrow><mi>2</mi><mo>*</mo><polynomial_2></mrow></mfrac><mo>=</mo><mi><solution_1></mi></math><math><mi>x</mi><msub>2</msub><mo>=</mo><mfrac><mrow><mi>-</mi><polynomial_1><mo>-</mo><msqrt><solution_d></msqrt></mrow><mrow><mi>2</mi><mo>*</mo><polynomial_2></mrow></mfrac><mo>=</mo><mi><solution_2></mi></math>");
    std::string __redaction_root_2_1r = std::string("Le discriminant est égal à 0, nous pouvons donc dénombrer 1 solution distincte : </br><math><mi>x</mi><mo>=</mo><mfrac><mrow><mi>-</mi><polynomial_1></mrow><mrow><mi>2</mi><mo>*</mo><polynomial_2></mrow></mfrac><mo>=</mo><mi><solution_1></mi></math>");
    std::string __redaction_root_2_0r = std::string("Le discriminant est inférieur à 0, nous n'avons pas de solutions réelles.");
    std::string __redaction_root_3_d = std::string("Le polynôme \"<full_formula>\" est de degré 3, utilisons la méthode de Cardan : </br><math><mdelta><mo>=</mo><mi>q'</mi><msup>2</msup><mo>+</mo><mi>p'</mi><msup>3</msup><mo>=</mo><polynomial_1><msup>2</msup><mo>-</mo><mi>4</mi><mo>*</mo><polynomial_2><mo>*</mo><polynomial_0><mo>=</mo><mi><solution_d></mi></math></br>");
    void polynomial_roots(scls::__Formula formula, std::string* redaction) {
        scls::Polynomial polynomial = *formula.polynomial();
        scls::Textual_Math_Settings settings;settings.set_hide_if_0(false);
        std::string unknown_name = std::string("x");

        // Get the good method
        scls::Complex max_degree = polynomial.degree(unknown_name);
        if(polynomial.degree(unknown_name) == 0) {
            // Set redaction
            if(redaction != 0) {
                if(polynomial.is_null()){(*redaction) += scls::replace(__redaction_root_0, std::string("<full_formula>"), polynomial.to_std_string(&settings));}
                else{(*redaction) += scls::replace(__redaction_root_0_not_0, std::string("<full_formula>"), polynomial.to_std_string(&settings));}
            }
        }
        else if(polynomial.degree(unknown_name) == 1) {
            // Solve it
            scls::Complex a = *polynomial.monomonial(unknown_name, 1)->factor();
            scls::Complex b = polynomial.known_monomonial_factor();
            scls::Complex solution = (b * -1) / a;

            // Set redaction
            if(redaction != 0) {
                (*redaction) += __redaction_root_1;

                // Edit
                (*redaction) = scls::replace(*redaction, std::string("<full_formula>"), polynomial.to_std_string(&settings));
                (*redaction) = scls::replace(*redaction, std::string("<polynomial_0>"), b.to_std_string_simple(&settings));
                (*redaction) = scls::replace(*redaction, std::string("<polynomial_1>"), a.to_std_string_simple(&settings));
                (*redaction) = scls::replace(*redaction, std::string("<solution_0>"), solution.to_std_string_simple(&settings));
            }
        }
        else if(polynomial.degree(unknown_name) == 2) {
            // Solve it
            scls::Complex a = *polynomial.monomonial(unknown_name, 2)->factor();
            scls::Complex b = *polynomial.monomonial(unknown_name, 1)->factor();
            scls::Complex c = polynomial.known_monomonial_factor();
            scls::Complex d = b * b - 4 * a * c;
            scls::__Formula solution_1 = 0;
            scls::__Formula solution_2 = 0;
            if(d.real() > 0){
                scls::__Formula sqrt = scls::__Formula(d);sqrt.set_applied_function<scls::__Sqrt_Function>();
                solution_1 = (scls::__Formula(b * -1) + sqrt) / scls::__Formula(2 * a);
                solution_2 = (scls::__Formula(b * -1) - sqrt) / scls::__Formula(2 * a);
            }
            else if(d.real() == 0){solution_1 = scls::__Formula(b * -1) / scls::__Formula(2 * a);}

            // Set redaction
            if(redaction != 0) {
                (*redaction) += __redaction_root_2_d;
                if(d.real() > 0){(*redaction) += __redaction_root_2_2r;}
                else if(d.real() == 0){(*redaction) += __redaction_root_2_1r;}
                else{(*redaction) += __redaction_root_2_0r;}

                // Edit
                (*redaction) = scls::replace(*redaction, std::string("<full_formula>"), polynomial.to_std_string(&settings));
                (*redaction) = scls::replace(*redaction, std::string("<polynomial_0>"), c.to_std_string_simple(&settings));
                (*redaction) = scls::replace(*redaction, std::string("<polynomial_1>"), b.to_std_string_simple(&settings));
                (*redaction) = scls::replace(*redaction, std::string("<polynomial_2>"), a.to_std_string_simple(&settings));
                (*redaction) = scls::replace(*redaction, std::string("<solution_d>"), d.to_std_string_simple(&settings));
                (*redaction) = scls::replace(*redaction, std::string("<solution_1>"), std::to_string(solution_1.value_to_fraction().to_double()));
                (*redaction) = scls::replace(*redaction, std::string("<solution_2>"), std::to_string(solution_2.value_to_fraction().to_double()));
            }
        }
        else if(max_degree == 3) {
            // Solve it
            scls::Complex a = *polynomial.monomonial(unknown_name, 3)->factor();
            scls::Complex b = *polynomial.monomonial(unknown_name, 2)->factor();
            scls::Complex c = *polynomial.monomonial(unknown_name, 1)->factor();
            scls::Complex d = polynomial.known_monomonial_factor();

            // Solve it
            scls::Complex p = (3 * a * c - b * b) / (3 * a * a);
            scls::Complex q = (2 * b * b * b - 9 * a * b * c + 27 * a * a * d) / (27 * a * a * a);
            scls::Complex p_prime = (p) / (3);
            scls::Complex q_prime = (q) / (2);
            scls::Complex discriminant = 4 * 27 * (q_prime * q_prime + p_prime * p_prime * p_prime) * -1;
            std::cout << "E " << p.to_std_string_simple(0) << " " << p.real().to_double() << " " << q.to_std_string_simple(0) << " " << q.real().to_double() << " " << discriminant.real().to_std_string(0) << " " << discriminant.real().to_double() << std::endl;

            // Get the solution
            scls::__Formula solution_1 = 0;
            scls::__Formula solution_2 = 0;
            if(discriminant.real() > 0){
                scls::__Formula sqrt = scls::__Formula(discriminant) / 27;sqrt.set_applied_function<scls::__Sqrt_Function>();
                sqrt *= scls::Complex(0, 1);
                scls::__Formula u = (sqrt - q) / 2;
                std::cout << "F " << u.to_std_string(0) << std::endl;
            }

            // Set redaction
            if(redaction != 0) {
                (*redaction) += __redaction_root_3_d;

                // Edit
                (*redaction) = scls::replace(*redaction, std::string("<full_formula>"), polynomial.to_std_string(&settings));
                (*redaction) = scls::replace(*redaction, std::string("<polynomial_0>"), c.to_std_string_simple(&settings));
                (*redaction) = scls::replace(*redaction, std::string("<polynomial_1>"), b.to_std_string_simple(&settings));
                (*redaction) = scls::replace(*redaction, std::string("<polynomial_2>"), a.to_std_string_simple(&settings));
                (*redaction) = scls::replace(*redaction, std::string("<solution_d>"), discriminant.to_std_string_simple(&settings));
                (*redaction) = scls::replace(*redaction, std::string("<solution_1>"), std::to_string(solution_1.value_to_fraction().to_double()));
                (*redaction) = scls::replace(*redaction, std::string("<solution_2>"), std::to_string(solution_2.value_to_fraction().to_double()));
                //(*redaction) = scls::replace(*redaction, std::string("<solution_0>"), solution.to_std_string_simple(&settings));
            }
        }
    }

    // Solve an equation
    void solve_equation(scls::__Formula formula, std::string* redaction) {
        // The formula is a polynomial
        if(formula.is_simple_polynomial()){
            polynomial_roots(formula, redaction);
        }
    }
}
