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
    // Analyse handling
    //
    //******************

    // Returns the definition set of a function
    scls::Set_Number function_definition_set(Function_Studied* current_function, std::string* redaction) {
        // Create the redaction
        scls::Formula& function_studied = current_function->function_formula;
        scls::Set_Number to_return = scls::Set_Number::set_real();

        // Do the redaction
        if(redaction != 0) {
            (*redaction) += "Nous cherchons l'ensemble de définition maximale de la forme " + function_studied.to_std_string() + ". ";
        }

        // Fraction part
        std::shared_ptr<scls::Set_Number> denominator_null;
        if(function_studied.denominator() != 0) {
            Function_Studied fs; fs.function_formula = *function_studied.denominator();
            fs.function_number = current_function->function_number + 1; fs.function_unknown = current_function->function_unknown;
            if(redaction == 0) {std::string s;denominator_null = std::make_shared<scls::Set_Number>(function_roots(fs, s));}
            else {denominator_null = std::make_shared<scls::Set_Number>(function_roots(fs, *redaction));}
            scls::Fraction needed_value = denominator_null.get()->numbers().at(0).real();

            // TEMPORARY SET THE INTERVAL
            to_return = scls::Set_Number();
            scls::Interval interval = scls::Interval(needed_value - 1, needed_value);interval.set_start_infinite(true);to_return.add_interval(interval);
            interval = scls::Interval(needed_value, needed_value + 1);interval.set_end_infinite(true);to_return.add_interval(interval);

            // Do the redaction
            if(redaction != 0) {
                (*redaction) += "Cette forme contient un dénominateur global " + function_studied.denominator()->to_std_string() + ". ";
                (*redaction) += "Donc, elle n'est pas définie pour " + function_studied.denominator()->to_std_string() + " = 0. ";
                (*redaction) += "Donc, la forme " + function_studied.to_std_string() + " n'est pas définie pour x appartenant à " + denominator_null.get()->to_std_string() + ". ";
            }
        }

        // Finish the redaction
        if(redaction != 0 && to_return == scls::Set_Number::set_real()) {
            (*redaction) += "Or, cette fonction ne contient aucune forme pouvant amener à une valeur interdite. Donc, elle est définie sur l'ensemble des réels. ";
        }

        // Return the result
        current_function->definition_set = std::make_shared<scls::Set_Number>(to_return);
        return to_return;
    }

    // Returns the derivation of a function
    scls::Formula function_derivation_monomonial(scls::__Monomonial current_monomonial, std::string& redaction) {
        // Do the calculation
        scls::_Base_Unknown* needed_unknown = current_monomonial.contains_unknown("x");
        scls::Formula result;
        if(needed_unknown != 0) {
            // Basic exponential form
            scls::Fraction new_exponent = needed_unknown->exponent().real() - 1;
            if(new_exponent != 0) {
                result = scls::__Monomonial(needed_unknown->exponent() * current_monomonial.factor(), "x", new_exponent);
            } else {
                result = current_monomonial.factor().real();
            }
        }

        // Write the redaction
        redaction += "La forme dérivée de la forme " + current_monomonial.to_std_string() + " est " + result.to_std_string() + ". ";

        return result;
    }
    scls::Formula function_derivation_polymonial(Function_Studied current_function, std::string& redaction) {
        // Do the calculation
        scls::Formula result;

        // Write the redaction
        scls::Polymonial needed_polymonial = current_function.function_formula.to_polymonial();
        redaction += "Or, cette fonction n'est qu'un simple polymône. ";
        redaction += "Pour étudier sa dérivée, découpons cette fonction en plusieurs monômes, que nous étudierons l'un après l'autre. ";
        // Study each monomonials
        for(int i = 0;i<static_cast<int>(needed_polymonial.monomonials().size());i++) {
            result += function_derivation_monomonial(needed_polymonial.monomonials()[i], redaction);
        }
        redaction += "Donc, la forme dérivée de " + needed_polymonial.to_std_string() + " est " + result.to_std_string() + ". ";

        return result;
    }
    scls::Formula function_derivation(Function_Studied current_function, std::string& redaction) {
        // Do the calculation
        scls::Formula result;

        // Write the redaction
        redaction += "Nous cherchons la dérivée de la fonction " + current_function.function_name + "(" + current_function.function_unknown + "). ";
        if(current_function.function_formula.is_simple_polymonial()) {
            // The function is a simple polymonial
            result = function_derivation_polymonial(current_function, redaction);
        } else if(current_function.function_formula.applied_function() == 0) {
            // The function is more complicated
            std::string function_name = current_function.function_name + "_";
            scls::Formula needed_added_element = current_function.function_formula.added_element();
            redaction += "Premièrement, étudions " + function_name + " tel que " + function_name + "(" + current_function.function_unknown + ") = " + needed_added_element.to_std_string() + ". ";
            Function_Studied fs; fs.function_formula = needed_added_element;
            fs.function_number = current_function.function_number + 1; fs.function_unknown = current_function.function_unknown;
            scls::Formula added_element_derivate = function_derivation(fs, redaction);

            // Calculate the denominator
            scls::Formula* needed_denominator = current_function.function_formula.denominator();
            redaction += "En suite, étudions " + function_name + " tel que " + function_name + "(" + current_function.function_unknown + ") = " + needed_denominator->to_std_string() + ". ";
            fs.function_formula = *needed_denominator;
            fs.function_number = current_function.function_number + 1; fs.function_unknown = current_function.function_unknown;
            scls::Formula denominator_derivate = function_derivation(fs, redaction);

            // Do the division
            result = ((added_element_derivate * (*needed_denominator)) - (needed_added_element * denominator_derivate)) / ((*needed_denominator) * (*needed_denominator));
            redaction += "Finalement, appliquons la formule de division de formes dérivées. ";
            redaction += "Au final, la dérivé de " + current_function.function_name + " est " + current_function.function_name + "' tel que " + current_function.function_name + "'(" + current_function.function_unknown + ") = " + result.to_std_string() + ". ";
        }

        return result;
    }

    // Returns the image of a function
    scls::__Formula_Base::Formula function_image(Function_Studied current_function, scls::Formula needed_value, std::string& redaction) {
        // Do the ccalculation
        scls::__Formula_Base::Formula result = current_function.function_formula.replace_unknown(current_function.function_unknown, needed_value);

        // Write the redaction
        redaction += "Nous cherchons la valeur de " + current_function.function_name + "(" + needed_value.to_std_string() + "). ";
        redaction += "Pour cela, remplaceons " + current_function.function_unknown + " par " + needed_value.to_std_string() + " dans " + current_function.function_name + ".</br>";
        redaction += current_function.function_name + "(" + needed_value.to_std_string() + ") = " + result.to_std_string() + "</br>";
        redaction += "Donc, " + current_function.function_name + "(" + needed_value.to_std_string() + ") = " + result.to_std_string() + ". ";
        return result;
    }

    // Returns the limit of a function / polymonial in + infinity
    scls::Limit polymonial_limit(scls::Polymonial current_monomonial, scls::Limit needed_limit, std::string unknown, std::string& redaction) {
        // Check the limit for infinity
        scls::Limit current_limit = current_monomonial.limit(needed_limit, unknown);
        redaction += "Le monôme " + current_monomonial.to_std_string() + " a pour limite " + current_limit.to_std_string() + ". ";
        return current_limit;
    }
    scls::Limit __function_limit_monomonials(Function_Studied current_function, scls::Polymonial polymonial, scls::Limit needed_limit, std::string& redaction) {
        // Cut the formula by monomonial
        redaction += "Comme cette forme est un simple polynôme, étudions les limites de chaque monôme. ";
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
                redaction += "Cette forme est une division, nous avons besoin des limites du numérateur et du dénominateur pour avoir sa limite. ";
            }

            // Start the search
            redaction += "Nous cherchons la limite de " + current_function.function_name + ", qui peut s'écrire " + function_studied.to_std_string() + ", en " + needed_limit.to_std_string() + ". ";
            scls::Polymonial polymonial = function_studied;

            // Handle the polymonial monomonial by monomonial
            to_return = __function_limit_monomonials(current_function, polymonial, needed_limit, redaction);

            // Handle possible errors
            if(to_return.is_error_ipi() && current_function.function_number <= 1) {
                redaction += "Or, nous avons une forme indéterminée \"infini + ou - infini\". ";
                redaction += "Pour lever l'indétermination, factorisons toute la forme par le monôme du plus haut degré, et calculons sa limite. ";
                Function_Studied needed_function; scls::Polymonial needed_monomonial = scls::Polymonial(scls::Complex(1), current_function.function_unknown, polymonial.degree(current_function.function_unknown));
                needed_function.function_formula = function_studied / needed_monomonial;
                needed_function.function_name = current_function.function_name;
                needed_function.function_number = current_function.function_number + 1;
                scls::Limit result = function_limit(needed_function, needed_limit, redaction);
                redaction += "Maintenant, calculons la limite de " + needed_monomonial.to_std_string() + ", qui est de +infini. ";
                if(result.value() > 0) {
                    redaction += "Par produit de limites, la limite de f pour " + current_function.function_unknown + " tendant vers " + needed_limit.to_std_string() + " est +infini. ";
                } else {
                    redaction += "Par produit de limites, la limite de f pour " + current_function.function_unknown + " tendant vers " + needed_limit.to_std_string() + " est -infini. ";
                }
            } else {
                // Finish the redaction
                redaction += "Par somme de limites, la limite de " + current_function.function_name + " pour " + current_function.function_unknown + " tendant vers " + needed_limit.to_std_string() + " est " + to_return.to_std_string() + ". ";
            }
        }

        return to_return;
    }

    // Returns the set of roots of a function
    scls::Set_Number function_roots(Function_Studied current_function, std::string& redaction) {
        // Create the redaction
        scls::Formula& function_studied = current_function.function_formula;
        redaction += "Nous cherchons les racines de " + function_studied.to_std_string() + ". ";
        scls::Set_Number to_return = scls::Set_Number();

        // Only one polymonial
        if(function_studied.is_simple_polymonial()) {
            scls::Polymonial polymonial = function_studied;
            int degree = polymonial.degree(current_function.function_unknown).real().to_int();
            if(polymonial.is_known()) {
                // Only one number
                scls::Fraction number = static_cast<scls::Complex>(polymonial.known_monomonial()).real();
                redaction += "Or, " + function_studied.to_std_string() + " n'est pas égal à 0, cette forme n'a donc pas de racines. ";
            } else if(degree == 1) {
                // Calculate the known and unknown parts
                scls::Complex known_part = polymonial.known_monomonial().factor();
                scls::Complex unknown_part = polymonial.unknown_monomonials()[0].factor();

                // Create the redaction
                redaction += "Or, cette forme est une forme affine. ";
                redaction += "Pour étudier son unique racine, nous devons étudier les deux parties qui la constitue. ";
                redaction += "La partie connue du polynôme vaut " + known_part.to_std_string_simple() + ". ";

                // Calculate the solution
                known_part *= -1;
                scls::Complex solution = known_part / unknown_part;
                bool inverse_sign = false;
                if((known_part.real() < 0 && unknown_part.real() > 0)) inverse_sign = true;
                redaction += "Donc, la racine de cette forme est atteinte quand l'équation " + unknown_part.to_std_string_simple() + " * n = " + known_part.to_std_string_simple() + " fonctionne. ";
                redaction += "Or, cette équation est vérifiée pour n = " + solution.to_std_string_simple() + ". ";
                redaction += "Donc, " + function_studied.to_std_string() + " admet pour racine " + solution.to_std_string_simple() + ". ";
                to_return.add_number(solution);
                scls::Interval interval;
            } else if(degree == 2) {
                // Calculate the known and unknown parts
                scls::Complex a_part = polymonial.monomonial("n", 2).factor();
                scls::Complex b_part = polymonial.monomonial("n").factor();
                scls::Complex c_part = polymonial.known_monomonial().factor();
                scls::Complex discriminant_complex = b_part * b_part - 4 * a_part * c_part;

                // Create the redaction for the discriminant
                redaction += "Or, cette forme est un polynôme de degré 2. ";
                redaction += "Pour étudier ses racines, nous devons étudier les trois parties qui la constitue. ";
                redaction += "Commençons par calculer le discriminant d de cette forme. ";
                redaction += " d = b * b + 4 * a * c = " + discriminant_complex.to_std_string_simple() + ". ";

                // Search the needed roots
                scls::Fraction discriminant = discriminant_complex.real();
                if(discriminant < 0) {
                    redaction += "Or, d &lt; 0, donc cette forme n'a pas de solution dans l'ensemble des réels. ";
                } else if(discriminant == 0) {
                    scls::Fraction solution = ((b_part * -1) / (2 * a_part)).real();
                    redaction += "Or, d = 0, donc cette forme a une solution dans l'ensemble des réels. ";
                    redaction += "Cette solution est n = -b/2a = " + solution.to_std_string() + ". ";
                    to_return.add_number(solution);
                } else {
                    scls::Fraction sqrt_d = std::sqrt(discriminant.to_double());
                    scls::Fraction solution_1 = (((b_part * -1) + sqrt_d) / (2 * a_part)).real();
                    scls::Fraction solution_2 = (((b_part * -1) - sqrt_d) / (2 * a_part)).real();
                    redaction += "Or, d > 0, donc cette forme a deux solutions dans l'ensemble des réels. ";
                    redaction += "La première solution est x1 = (-b + sqrt(d))/2a, qui est à peu prés égal à " + scls::format_number_to_text(solution_1.to_double()) + ". ";
                    redaction += "La deuxième solution est x2 = (-b - sqrt(d))/2a, qui est à peu prés égal à " + scls::format_number_to_text(solution_2.to_double()) + ". ";
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
        scls::__Formula_Base::Formula function_plus = scls::replace_unknown(current_function, needed_unknown, needed_unknown + " + 1");
        scls::__Formula_Base::Formula function_difference = function_plus - current_function;

        // Create the redaction
        redaction += "La forme " + current_function_studied.function_name + "(" + needed_unknown + "+1) peut s'écrire " + function_plus.to_std_string() + ". ";
        redaction += "La forme " + current_function_studied.function_name + "(" + needed_unknown + "+1) - " + current_function_studied.function_name + "(" + needed_unknown + ") peut s'écrire " + function_difference.to_std_string() + ". ";
        redaction += "Pour étudier les variations de " + current_function_studied.function_name + ", nous devons donc étudier le signe de " + current_function_studied.function_name + "(" + needed_unknown + "+1) - " + current_function_studied.function_name + "(" + needed_unknown + ").</br>";

        // Create the needed function studied
        Function_Studied fs;
        fs.function_formula = *function_difference.formula_base();
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

    //******************
    //
    // Graphic handling
    //
    //******************

    // Graphic constructor
    Graphic::Graphic_Function::Graphic_Function(std::shared_ptr<Function_Studied> function_studied):a_function_studied(function_studied){}

    // Graphic constructor
    Graphic::Graphic(scls::_Window_Advanced_Struct& window, std::string name, std::weak_ptr<scls::GUI_Object> parent):scls::GUI_Object(window, name, parent){update_texture();}

    // Adds a function to the graphic
    void Graphic::add_function(std::shared_ptr<Function_Studied> function_studied) {
        // Create the function
        std::shared_ptr<Graphic_Function> new_function = std::make_shared<Graphic_Function>(function_studied);
        a_functions.push_back(new_function);

        a_graphic_base.get()->a_function_number++;
    }

    // Function called after creation
    void Graphic::after_creation(){}

    // Needed fragment shader for the function
    std::string Graphic::graphic_function_fragment_shader(scls::Formula needed_formula) {
        std::string to_return = "#version 330 core\n";
        to_return += "in vec2 tex_pos;"; // Uniform / in/out variables
        to_return += "out vec4 FragColor;";
        to_return += "uniform vec4 background_color;";
        to_return += "uniform vec4 border_color;";
        to_return += "uniform vec4 border_width;";
        to_return += "uniform vec2 middle_position;";
        to_return += "uniform vec4 object_extremum;";
        to_return += "uniform vec4 object_rect;";
        to_return += "uniform vec2 pixel_by_base;";
        to_return += "uniform sampler2D texture_0;";
        to_return += "uniform bool texture_binded;";
        to_return += "uniform vec4 texture_rect;\n";

        // Function
        std::string needed_glsl = needed_formula.to_polymonial().to_glsl();
        to_return += needed_glsl;
        to_return += "float foo(float x){return poly(x);}\n";

        to_return += "void main(){";
        to_return += scls::Shader_Program::default_gui_extremum_handling();
        to_return += "vec2 current_pos = tex_pos;vec4 final_color = vec4(1, 1, 1, 0);";
        to_return += "float height_multiplier = (object_rect.w / pixel_by_base.y);float width_multiplier = (object_rect.z / pixel_by_base.x);";
        to_return += "float x_1 = ((current_pos.x - 0.5) + middle_position.x) * width_multiplier;";
        to_return += "float x_2 = x_1 + (width_multiplier/object_rect.z);";

        // Function
        to_return += "float y_1 = foo(x_1);float y_2 = foo(x_2);if(y_1 > y_2){y_1+=height_multiplier/object_rect.w;}else{y_2+=height_multiplier/object_rect.w;}";
        to_return += "float p = (((current_pos.y - 0.5) + middle_position.y)) * height_multiplier;";
        to_return += "if((y_1 >= p && y_2 <= p) || (y_1 <= p && y_2 >= p)){final_color = vec4(1, 0, 0, 1);}";
        to_return += "FragColor = final_color;}";
        return to_return;
    }

    // Render the object
    void Graphic::render(glm::vec3 scale_multiplier) {scls::GUI_Object::render(scale_multiplier);}

    // Returns the image of the graphic
    int Graphic::graphic_x_to_pixel_x(double x, std::shared_ptr<scls::Image>& needed_image){return (x - middle_x().to_double()) * floor(pixel_by_case_x()) + (needed_image.get()->width() / 2.0);};
    int Graphic::graphic_y_to_pixel_y(double y, std::shared_ptr<scls::Image>& needed_image){return (y - middle_y().to_double()) * floor(pixel_by_case_y()) + (needed_image.get()->height() / 2.0);};
    int Graphic::graphic_y_to_pixel_y_inversed(double y, std::shared_ptr<scls::Image>& needed_image){return needed_image.get()->height() - graphic_y_to_pixel_y(y, needed_image);};
    scls::Fraction Graphic::pixel_x_to_graphic_x(int x, std::shared_ptr<scls::Image>& needed_image){return middle_x() + ((scls::Fraction(x) - scls::Fraction(needed_image.get()->width(), 2)) / scls::Fraction(floor(pixel_by_case_x())));}
    scls::Fraction Graphic::pixel_y_to_graphic_y(int y, std::shared_ptr<scls::Image>& needed_image){return middle_y() + ((scls::Fraction(needed_image.get()->height(), 2) - scls::Fraction(y)) / scls::Fraction(floor(pixel_by_case_y())));}
    std::shared_ptr<scls::Image> Graphic::to_image() {
        // Create the image
        std::shared_ptr<scls::Image> to_return = std::make_shared<scls::Image>(width_in_pixel(), height_in_pixel(), scls::Color(255, 255, 255));

        // Draw the basic lines
        if(draw_base() || draw_sub_bases()) {
            // Horizontal lines
            double current_y = pixel_y_to_graphic_y(to_return.get()->height(), to_return).to_double();
            current_y = floor(current_y);
            double needed_y = graphic_y_to_pixel_y(current_y, to_return);
            while(needed_y < to_return.get()->width()) {
                scls::Color needed_color = scls::Color(0, 0, 0);
                int needed_height = 1;
                if(current_y == 0){needed_height = 2;}else{needed_color=scls::Color(125, 125, 125);}
                to_return.get()->fill_rect(0, to_return.get()->height() - needed_y, to_return.get()->width(), needed_height, needed_color);
                current_y++;
                needed_y = graphic_y_to_pixel_y(current_y, to_return);
            }
            // Vertical lines
            double current_x = pixel_x_to_graphic_x(0, to_return).to_double();
            current_x = ceil(current_x);
            int needed_x = graphic_x_to_pixel_x(current_x, to_return);
            while(needed_x < to_return.get()->width()) {
                scls::Color needed_color = scls::Color(0, 0, 0);
                int needed_width = 1;
                if(current_x == 0){needed_width = 2;}else{needed_color=scls::Color(125, 125, 125);}
                to_return.get()->fill_rect(needed_x, 0, needed_width, to_return.get()->height(), needed_color);
                current_x++;
                needed_x = graphic_x_to_pixel_x(current_x, to_return);
            }
        }

        // Get the datas for the drawing
        scls::Fraction image = pixel_x_to_graphic_x(0, to_return);
        scls::Fraction multiplier = scls::Fraction(1, pixel_by_case_x());
        struct Needed_Pos {scls::__Formula_Base::Formula pos;scls::__Formula_Base::Formula previous_pos;bool previous_pos_used = false;};
        std::vector<scls::Fraction> screen_pos = std::vector<scls::Fraction>(to_return.get()->width() + 1);
        for(int i = 0;i<static_cast<int>(to_return.get()->width()) + 1;i++){screen_pos[i] = image; image += multiplier;}
        // Draw the functions
        for(int i = 0;i<static_cast<int>(a_functions.size());i++) {
            // Get the values
            scls::Formula needed_formula = a_functions[i].get()->formula();
            std::vector<Needed_Pos> needed_pos = std::vector<Needed_Pos>(to_return.get()->width() + 1);
            // Get each values of the function
            scls::__Formula_Base::Formula last_pos;
            for(int j = 0;j<static_cast<int>(to_return.get()->width()) + 1;j++){
                // Get the needed pos
                Needed_Pos to_add;
                if(a_functions[i].get()->definition_set()->is_in(screen_pos[j])) {
                    to_add.pos = needed_formula.replace_unknown("x", scls::Formula(screen_pos[j]));
                }
                else {
                    to_add.pos = needed_formula.replace_unknown("x", scls::Formula(screen_pos[j] - scls::Fraction(1, 1000)));
                }
                needed_pos[j] = to_add;

                // Check according to the last value
                if(j > 0 && !a_functions[i].get()->definition_set()->is_in(scls::Interval(screen_pos[j - 1], screen_pos[j]))) {
                    needed_pos[j - 1].pos = needed_formula.replace_unknown("x", scls::Formula(screen_pos[j - 1] + scls::Fraction(1, 1000)));
                    needed_pos[j].previous_pos = needed_formula.replace_unknown("x", scls::Formula(screen_pos[j] - scls::Fraction(1, 1000)));
                    needed_pos[j].previous_pos_used = true;
                }

                // Finalise the creation
                last_pos = to_add.pos;
            }

            // Adapt each values to the screen
            struct Needed_Pos_Screen {int pos;int previous_pos;bool previous_pos_used = false;};
            std::vector<Needed_Pos_Screen> needed_y = std::vector<Needed_Pos_Screen>(to_return.get()->width() + 1);
            for(int i = 0;i<static_cast<int>(to_return.get()->width()) + 1;i++){
                scls::Fraction value = needed_pos[i].pos.to_polymonial().known_monomonial().factor().real();
                needed_y[i].pos = graphic_y_to_pixel_y(value.to_double(), to_return);
                // Check the previous pos
                if(needed_pos[i].previous_pos_used) {
                    value = needed_pos[i].previous_pos.to_polymonial().known_monomonial().factor().real();
                    needed_y[i].previous_pos = graphic_y_to_pixel_y(value.to_double(), to_return);
                    needed_y[i].previous_pos_used = true;
                }
            }

            // Draw each pixel
            int width = 3;
            for(int j = 0;j<static_cast<int>(to_return.get()->width());j++) {
                int y_1 = to_return.get()->height() - needed_y[j].pos;
                int y_2 = to_return.get()->height() - needed_y[j + 1].pos;
                if(needed_y[j + 1].previous_pos_used){y_1 = to_return.get()->height() - needed_y[j + 1].previous_pos;}
                // Draw the point
                int needed_height = std::abs(y_1 - y_2);
                int needed_y = std::min(y_1, y_2) - width / 2.0;
                if(needed_y < to_return.get()->height() && needed_y >= -needed_height) {
                    to_return.get()->fill_rect(j - width / 2.0, needed_y, width, needed_height + width, scls::Color(255, 0, 0));
                }
            }
        }

        // Draw the vectors
        for(int i = 0;i<static_cast<int>(a_vectors.size());i++) {
            std::shared_ptr<Vector> current_vector = a_vectors[i];
            double needed_x = graphic_x_to_pixel_x(current_vector.get()->x()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
            double needed_y = graphic_y_to_pixel_y_inversed(current_vector.get()->y()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
            to_return.get()->draw_line(graphic_x_to_pixel_x(0, to_return), graphic_y_to_pixel_y_inversed(0, to_return), needed_x, needed_y, scls::Color(255, 0, 0), 2);
        }
        // Draw the circles
        for(int i = 0;i<static_cast<int>(a_circles.size());i++) {
            std::shared_ptr<Circle> current_circle = a_circles[i];
            Vector center = current_circle.get()->center();
            double radius = current_circle.get()->radius().to_polymonial().known_monomonial().factor().real().to_double();
            radius = radius * pixel_by_case_x();
            double needed_x = graphic_x_to_pixel_x(center.x()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
            double needed_y = graphic_y_to_pixel_y_inversed(center.y()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
            to_return.get()->fill_circle(needed_x, needed_y, radius, current_circle.get()->color(), current_circle.get()->border_radius(), current_circle.get()->border_color());
        }

        return to_return;
    }

    // Updates the object
    void Graphic::update_event() {
        GUI_Object::update_event();

        // Move the plane
        bool modified = false;
        scls::Fraction speed = scls::Fraction(1, 50);
        if(window_struct().key_pressed("q")){a_graphic_base.get()->a_middle_x -= (speed);modified = true;}
        if(window_struct().key_pressed("d")){a_graphic_base.get()->a_middle_x += (speed);modified = true;}
        if(window_struct().key_pressed("z")){a_graphic_base.get()->a_middle_y += (speed);modified = true;}
        if(window_struct().key_pressed("s")){a_graphic_base.get()->a_middle_y -= (speed);modified = true;}
        // Zoom or unzoom
        double zoom_speed = 10;
        if(window_struct().key_pressed("w")){a_graphic_base.get()->a_pixel_by_case_x -= (zoom_speed);a_graphic_base.get()->a_pixel_by_case_y -= (zoom_speed);modified = true;}
        if(window_struct().key_pressed("c")){a_graphic_base.get()->a_pixel_by_case_x += (zoom_speed);a_graphic_base.get()->a_pixel_by_case_y += (zoom_speed);modified = true;}

        // Handle clicks
        if(is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
            double needed_x = window_struct().mouse_x() - x_in_absolute_pixel();
            double needed_y = window_struct().mouse_y() - (window_struct().window_height() - (y_in_absolute_pixel() + height_in_pixel()));
            scls::Fraction movement_x = pixel_x_to_graphic_x(needed_x, texture()->image_shared_ptr());
            scls::Fraction movement_y = pixel_y_to_graphic_y(needed_y, texture()->image_shared_ptr());
            add_vector(Vector("a", movement_x, movement_y));
            modified = true;
        }

        if(modified){update_texture();}
        if(window_struct().key_pressed("p")){to_image().get()->save_png("tests/function.png");}
    }
}
