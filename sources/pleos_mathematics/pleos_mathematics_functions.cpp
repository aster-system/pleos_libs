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

// Include pleos_mathematics_functions.cpp
#include "../../pleos_mathematics/pleos_mathematics_functions.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {
    //******************
    //
    // Analyse handling
    //
    //******************

    // Returns the image of a function
    scls::Formula function_image(Function_Studied current_function, scls::Formula needed_value, std::string& redaction) {
        // Do the ccalculation
        scls::Formula result = current_function.function_formula.replace_unknown(current_function.function_unknown, needed_value);

        // Write the redaction
        redaction += "Nous cherchons la valeur de " + current_function.function_name + "(" + needed_value.to_std_string() + ").";
        redaction += " Pour cela, remplaceons " + current_function.function_unknown + " par " + needed_value.to_std_string() + " dans " + current_function.function_name + ".</br>";
        redaction += current_function.function_name + "(" + needed_value.to_std_string() + ") = " + result.to_std_string() + "</br>";
        redaction += "Donc, " + current_function.function_name + "(" + needed_value.to_std_string() + ") = " + result.to_std_string() + ".";
        return result;
    }

    // Returns the limit of a function / polymonial in + infinity
    scls::Limit polymonial_limit(scls::Polymonial current_monomonial, scls::Limit needed_limit, std::string unknown, std::string& redaction) {
        // Check the limit for infinity
        scls::Limit current_limit = current_monomonial.limit(needed_limit, unknown);
        redaction += "Le monôme " + current_monomonial.to_std_string() + " a pour limite " + current_limit.to_std_string() + ".";
        return current_limit;
    }
    scls::Limit __function_limit_monomonials(Function_Studied current_function, scls::Polymonial polymonial, scls::Limit needed_limit, std::string& redaction) {
        // Cut the formula by monomonial
        redaction += " Comme cette forme est un simple polynôme, étudions les limites de chaque monôme.";
        std::vector<scls::Limit> limits;
        int monomonial_number = static_cast<int>(polymonial.monomonials().size());
        for(int i = 0;i<monomonial_number;i++) {
            scls::Limit result = polymonial_limit(polymonial.monomonials().at(i), needed_limit, current_function.function_unknown, redaction);
            limits.push_back(result); redaction += " ";
        }

        // Get the final limit
        scls::Complex limit = scls::Complex(0);
        char special = 0;
        for(int i = 0;i<monomonial_number;i++) {
            if(limits[i].is_special()) {
                if(limits[i].is_pi()) {
                    if(special == SCLS_MATH_NUMBER_LIMIT_SPECIAL_MI) {special = SCLS_MATH_NUMBER_LIMIT_ERROR_IPI;break;}
                    special = SCLS_MATH_NUMBER_LIMIT_SPECIAL_PI;
                } else if(limits[i].is_mi()) {
                    if(special == SCLS_MATH_NUMBER_LIMIT_SPECIAL_PI) {special = SCLS_MATH_NUMBER_LIMIT_ERROR_IPI;break;}
                    special = SCLS_MATH_NUMBER_LIMIT_SPECIAL_MI;
                }
            } else {
                limit += limits[i].value();
            }
        }
        scls::Limit final_limit;
        if(special == 0){final_limit = limit.real();}
        else{final_limit.set_special_value(special);}
        return final_limit;
    }
    scls::Limit function_limit(Function_Studied current_function, scls::Limit needed_limit, std::string& redaction) {
        // Create the redaction
        scls::Formula& function_studied = current_function.function_formula;
        scls::Limit to_return = scls::Limit();

        // Only one polymonial
        if(function_studied.is_simple_polymonial()) {
            if(function_studied.denominator() != 0) {
                // The limit contains a fraction
                redaction += "Cette forme est une division, nous avons besoin des limites du numérateur et du dénominateur pour avoir sa limite.";
            }

            // Start the search
            redaction += "Nous cherchons la limite de " + current_function.function_name + ", qui peut s'écrire " + function_studied.to_std_string() + ", en " + needed_limit.to_std_string() + ".";
            scls::Polymonial polymonial = function_studied;

            // Handle the polymonial monomonial by monomonial
            scls::Limit limit = __function_limit_monomonials(current_function, polymonial, needed_limit, redaction);

            // Handle possible errors
            if(limit.is_error_ipi() && current_function.function_number <= 1) {
                redaction += " Or, nous avons une forme indéterminée \"infini + ou - infini\".";
                redaction += " Pour lever l'indétermination, factorisons toute la forme par le monôme du plus haut degré, et calculons sa limite. ";
                Function_Studied needed_function; scls::Polymonial needed_monomonial = scls::Polymonial(scls::Complex(1), current_function.function_unknown, polymonial.degree(current_function.function_unknown));
                needed_function.function_formula = function_studied / needed_monomonial;
                needed_function.function_name = current_function.function_name;
                needed_function.function_number = current_function.function_number + 1;
                scls::Limit result = function_limit(needed_function, needed_limit, redaction);
                redaction += " Maintenant, calculons la limite de " + needed_monomonial.to_std_string() + ", qui est de +infini.";
                if(result.value() > 0) {
                    redaction += " Par produit de limites, la limite de f pour " + current_function.function_unknown + " tendant vers " + needed_limit.to_std_string() + " est +infini.";
                } else {
                    redaction += " Par produit de limites, la limite de f pour " + current_function.function_unknown + " tendant vers " + needed_limit.to_std_string() + " est -infini.";
                }
            } else {
                // Finish the redaction
                redaction += " Par somme de limites, la limite de " + current_function.function_name + " pour " + current_function.function_unknown + " tendant vers " + needed_limit.to_std_string() + " est " + limit.to_std_string() + ".";
            }
        }

        return to_return;
    }

    // Returns the set of roots of a function
    scls::Set_Number function_roots(Function_Studied current_function, std::string& redaction) {
        // Create the redaction
        scls::Formula& function_studied = current_function.function_formula;
        redaction += "Nous cherchons les racines de " + current_function.function_name + ", qui peut s'écrire " + function_studied.to_std_string() + ". ";
        scls::Set_Number to_return = scls::Set_Number();

        // Only one polymonial
        if(function_studied.is_simple_polymonial()) {
            scls::Polymonial polymonial = function_studied;
            int degree = polymonial.degree("n").real().to_int();
            if(polymonial.is_known()) {
                // Only one number
                scls::Fraction number = static_cast<scls::Complex>(polymonial.known_monomonial()).real();
                redaction += "Or, " + function_studied.to_std_string() + " n'est pas égal à 0, cette forme n'a donc pas de racines.";
            } else if(degree == 1) {
                // Calculate the known and unknown parts
                scls::Complex known_part = polymonial.known_monomonial().factor();
                scls::Complex unknown_part = polymonial.unknown_monomonials()[0].factor();

                // Create the redaction
                redaction += "Or, cette forme est une forme affine.";
                redaction += " Pour étudier son unique racine, nous devons étudier les deux parties qui la constitue.";
                redaction += " La partie connue du polynôme vaut " + known_part.to_std_string_simple() + ".";

                // Calculate the solution
                known_part *= -1;
                scls::Complex solution = known_part / unknown_part;
                bool inverse_sign = false;
                if((known_part.real() < 0 && unknown_part.real() > 0)) inverse_sign = true;
                redaction += " Donc, la racine de cette forme est atteinte quand l'équation " + unknown_part.to_std_string_simple() + " * n = " + known_part.to_std_string_simple() + " fonctionne.";
                redaction += " Or, cette équation est vérifiée pour n = " + solution.to_std_string_simple() + ".";
                redaction += " Donc, " + function_studied.to_std_string() + " admet pour racine " + solution.to_std_string_simple() + ".";
                scls::Interval interval;
            } else if(degree == 2) {
                // Calculate the known and unknown parts
                scls::Complex a_part = polymonial.monomonial("n", 2).factor();
                scls::Complex b_part = polymonial.monomonial("n").factor();
                scls::Complex c_part = polymonial.known_monomonial().factor();
                scls::Complex discriminant_complex = b_part * b_part - 4 * a_part * c_part;

                // Create the redaction for the discriminant
                redaction += "Or, cette forme est un polynôme de degré 2.";
                redaction += " Pour étudier ses racines, nous devons étudier les trois parties qui la constitue.";
                redaction += " Commençons par calculer le discriminant d de cette forme.";
                redaction += " d = b * b + 4 * a * c = " + discriminant_complex.to_std_string_simple() + ".";

                // Search the needed roots
                scls::Fraction discriminant = discriminant_complex.real();
                if(discriminant < 0) {
                    redaction += " Or, d &lt; 0, donc cette forme n'a pas de solution dans l'ensemble des réels.";
                } else if(discriminant == 0) {
                    scls::Fraction solution = ((b_part * -1) / (2 * a_part)).real();
                    redaction += " Or, d = 0, donc cette forme a une solution dans l'ensemble des réels.";
                    redaction += " Cette solution est n = -b/2a = " + solution.to_std_string() + ".";
                    to_return.add_number(solution);
                } else {
                    scls::Fraction sqrt_d = std::sqrt(discriminant.to_double());
                    scls::Fraction solution_1 = (((b_part * -1) + sqrt_d) / (2 * a_part)).real();
                    scls::Fraction solution_2 = (((b_part * -1) - sqrt_d) / (2 * a_part)).real();
                    redaction += " Or, d > 0, donc cette forme a deux solutions dans l'ensemble des réels.";
                    redaction += " La première solution est x1 = (-b + sqrt(d))/2a, qui est à peu prés égal à " + scls::format_number_to_text(solution_1.to_double()) + ".";
                    redaction += " La deuxième solution est x2 = (-b - sqrt(d))/2a, qui est à peu prés égal à " + scls::format_number_to_text(solution_2.to_double()) + ".";
                    to_return.add_number(solution_1); to_return.add_number(solution_2);
                }
            }
        }

        return to_return;
    }

    // Returns the set of a positive function
    scls::Set_Number function_sign(Function_Studied current_function, std::string& redaction) {
        // Create the redaction
        scls::Formula& function_studied = current_function.function_formula;
        redaction += "Nous cherchons le signe de " + current_function.function_name + ", qui peut s'écrire " + function_studied.to_std_string() + ". ";
        scls::Set_Number to_return = scls::Set_Number();

        // Only one polymonial
        if(function_studied.is_simple_polymonial()) {
            scls::Polymonial polymonial = function_studied;
            int degree = polymonial.degree("n").real().to_int();
            if(polymonial.is_known()) {
                // Only one number
                scls::Fraction number = static_cast<scls::Complex>(polymonial.known_monomonial()).real();
                if(number < 0) {
                    redaction += "Or, " + function_studied.to_std_string() + " &lt; 0, cette forme est donc négative sur N.";
                } else if(number > 0) {
                    redaction += "Or, " + function_studied.to_std_string() + " &gt; 0, cette forme est donc positive sur N.";
                    // Create the needed interval
                    scls::Interval interval; interval.set_start_infinite(true); interval.set_end_infinite(true);
                    to_return = interval;
                } else {
                    redaction += "Or, " + current_function.function_name + " = 0, cette forme est donc nulle sur N.";
                }
                // Add the type of sequence in the redaction
                if(current_function.type == Studied_Type::ST_Sequence && number != 0) {
                    redaction += "</br>De plus, nous pouvons constater que s est une suite arithmétique de raison " + number.to_std_string() + ".";
                }
            } else if(polymonial.degree("n") == 1) {
                // Calculate the known and unknown parts
                scls::Complex known_part = polymonial.known_monomonial().factor();
                scls::Complex unknown_part = polymonial.unknown_monomonials()[0].factor();

                // Create the redaction
                redaction += "Or, cette forme est une forme affine.";
                redaction += " Pour étudier son signe, nous devons étudier les deux parties qui la constitue.";
                redaction += " La partie connue du polynôme vaut " + known_part.to_std_string_simple() + ".";

                // Calculate the solution
                known_part *= -1;
                scls::Complex solution = known_part / unknown_part;
                bool inverse_sign = false;
                if((known_part.real() < 0 && unknown_part.real() > 0)) inverse_sign = true;
                redaction += " Donc, la différence est négative quand l'équation " + unknown_part.to_std_string_simple() + " * n &lt; " + known_part.to_std_string_simple() + " fonctionne.";
                if(inverse_sign) {
                    redaction += " Or, cette équation est vérifiée pour n &lt; " + solution.to_std_string_simple() + ".";
                    redaction += " Donc, " + function_studied.to_std_string() + " &lt; 0 &lt;=&gt; n &lt; " + solution.to_std_string_simple() + ".";
                    scls::Interval interval; interval.set_start(solution.real()); interval.set_end_infinite(true);
                    to_return = interval;
                } else {
                    redaction += " Or, cette équation est vérifiée pour n &gt; " + solution.to_std_string_simple() + ".";
                    redaction += " Donc, " + function_studied.to_std_string() + " &lt; 0 &lt;=&gt; n &gt; " + solution.to_std_string_simple() + ".";
                    scls::Interval interval; interval.set_start_infinite(true); interval.set_end(solution.real());
                    to_return = interval;
                }
            } else if(polymonial.degree("n") == 2) {
                // Create the redaction
                redaction += "Or, cette forme est une forme polymonial de degré 2.";
                redaction += " Commençons par trouver les racines de cette forme.</br>";
                // Create the needed function studied
                scls::Set_Number result = function_roots(current_function, redaction);
                redaction += "</br>";
                // Search the sign
                scls::Fraction a_part = polymonial.monomonial("n", 2).factor().real();
                if(a_part > 0) {
                    if(result.numbers().size() <= 0) {
                        redaction += "De plus, a &gt; 0. Donc la forme est positive.";
                    } else if(result.numbers().size() == 1) {
                        redaction += "De plus, a &gt; 0. Donc la forme est positive, mais s'annule pour n = " + result.numbers().at(0).to_std_string_simple(7) + ".";
                    } else {
                        redaction += "De plus, a &gt; 0. Donc la forme est positive pour n &lt; " + result.numbers().at(0).to_std_string_simple(7) + " et n &gt; " + result.numbers().at(1).to_std_string_simple(7) + ".";
                    }
                } else {
                    if(result.numbers().size() <= 0) {
                        redaction += "De plus, a &lt; 0. Donc la forme est négative.";
                    } else if(result.numbers().size() == 1) {
                        redaction += "De plus, a &lt; 0. Donc la forme est négative, mais s'annule pour n = " + result.numbers().at(0).to_std_string_simple(7) + ".";
                    } else {
                        redaction += "De plus, a &lt; 0. Donc la forme est négative pour n &lt; " + result.numbers().at(0).to_std_string_simple(7) + " et n &gt; " + result.numbers().at(1).to_std_string_simple(7) + ".";
                    }
                }
            }
        }

        return to_return;
    }

    //******************
    //
    // Sequences handling
    //
    //******************

    // Returns the interval of an sequence
    scls::Interval sequence_variation(Function_Studied current_function_studied, std::string& redaction) {
        std::string& needed_unknown = current_function_studied.function_unknown;
        scls::Formula& current_function = current_function_studied.function_formula;
        scls::Formula function_plus = scls::replace_unknown(current_function, needed_unknown, needed_unknown + " + 1");
        scls::Formula function_difference = function_plus - current_function;

        // Create the redaction
        redaction += "La forme " + current_function_studied.function_name + "(" + needed_unknown + "+1) peut s'écrire " + function_plus.to_std_string() + ". ";
        redaction += "La forme " + current_function_studied.function_name + "(" + needed_unknown + "+1) - " + current_function_studied.function_name + "(" + needed_unknown + ") peut s'écrire " + function_difference.to_std_string() + ". ";
        redaction += "Pour étudier les variations de " + current_function_studied.function_name + ", nous devons donc étudier le signe de " + current_function_studied.function_name + "(" + needed_unknown + "+1) - " + current_function_studied.function_name + "(" + needed_unknown + ").</br>";

        // Create the needed function studied
        Function_Studied fs;
        fs.function_formula = function_difference;
        fs.function_name = current_function_studied.function_name;
        fs.function_number = 1;
        scls::Set_Number result = function_sign(fs, redaction);
        redaction += "</br>";

        // Create the variation redaction
        if(result.is_infinite()) {
            redaction += "La suite s est donc strictement croissante sur N.";
        } else if(result.is_empty()) {
            redaction += "La suite s est donc strictement décroissante sur N.";
        } else if(result.intervals().size() == 1) {
            scls::Interval studied_interval = result.intervals()[0];
            if(studied_interval.start_infinite()) {
                redaction += "La suite s est donc strictement croissante sur N pour n &lt; " + studied_interval.end().to_std_string();
                redaction += ", s'annule pour n = " + studied_interval.end().to_std_string();
                redaction += " et est strictement décroissante sur N pour n &gt; " + studied_interval.end().to_std_string() + ".";
            } else if(studied_interval.end_infinite()) {
                redaction += "La suite s est donc strictement croissante sur N pour n &gt; " + studied_interval.start().to_std_string();
                redaction += ", s'annule pour n = " + studied_interval.start().to_std_string();
                redaction += " et est strictement décroissante sur N pour n &lt; " + studied_interval.start().to_std_string() + ".";
            }
        }

        return scls::Interval(0, 0);
    }
}
