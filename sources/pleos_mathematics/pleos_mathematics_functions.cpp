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
        std::shared_ptr<scls::__Formula_Base> denominator = function_studied.denominator();
        if(denominator.get() != 0) {
            // Do the redaction
            if(redaction != 0) {
                (*redaction) += "Cette forme contient un dénominateur global " + denominator.get()->to_std_string() + ". ";
                (*redaction) += "Donc, elle n'est pas définie pour " + denominator.get()->to_std_string() + " = 0. ";
            }

            // Get the needed value
            Function_Studied fs; fs.function_formula = *denominator.get();
            fs.function_number = current_function->function_number + 1; fs.function_unknown = current_function->function_unknown;
            if(redaction == 0) {std::string s;denominator_null = std::make_shared<scls::Set_Number>(function_roots(&fs, s));}
            else {denominator_null = std::make_shared<scls::Set_Number>(function_roots(&fs, *redaction));}
            scls::Fraction needed_value;if(denominator_null.get()->numbers().size() > 0){needed_value = denominator_null.get()->numbers().at(0).real();}

            // TEMPORARY SET THE INTERVAL
            to_return = scls::Set_Number();
            scls::Interval interval = scls::Interval(needed_value - 1, true, needed_value, false);interval.set_start_infinite(true);to_return.add_interval(interval);
            interval = scls::Interval(needed_value, false, needed_value + 1, true);interval.set_end_infinite(true);to_return.add_interval(interval);

            // Do the redaction
            if(redaction != 0) {
                (*redaction) += "Donc, la forme " + function_studied.to_std_string() + " n'est pas définie pour x appartenant à " + denominator_null.get()->to_std_string() + ". ";
            }
        }

        // Finish the redaction
        if(redaction != 0) {
            if(to_return == scls::Set_Number::set_real()) {(*redaction) += std::string("Or, cette fonction ne contient aucune forme pouvant amener à une valeur interdite. ");}
            (*redaction) += std::string("Donc, cette fonction est définie sur l'ensemble ") + to_return.to_std_string() + std::string(". ");
        }

        // Return the result
        current_function->definition_set = std::make_shared<scls::Set_Number>(to_return);
        return to_return;
    }

    // Returns the derivation of a function
    scls::Formula function_derivation_monomonial(scls::__Monomonial current_monomonial, std::string* redaction) {
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

        if(redaction == 0) {
            // Write the redaction
            (*redaction) += "La forme dérivée de la forme " + current_monomonial.to_std_string() + " est " + result.to_std_string() + ". ";
        }

        return result;
    }
    scls::Formula function_derivation_polymonial(Function_Studied* current_function, std::string* redaction) {
        // Do the calculation
        scls::Formula result;

        // Write the redaction
        scls::Polymonial needed_polymonial = current_function->function_formula.to_polymonial();
        if(redaction != 0) {
            (*redaction) += "Or, cette fonction n'est qu'un simple polymône. ";
            (*redaction) += "Pour étudier sa dérivée, découpons cette fonction en plusieurs monômes, que nous étudierons l'un après l'autre. ";
        }

        // Study each monomonials
        for(int i = 0;i<static_cast<int>(needed_polymonial.monomonials().size());i++) {
            result += function_derivation_monomonial(needed_polymonial.monomonials()[i], redaction);
        }

        // Finish the redaction
        if(redaction != 0) {(*redaction) += "Donc, la forme dérivée de " + needed_polymonial.to_std_string() + " est " + result.to_std_string() + ". ";}

        return result;
    }
    scls::Formula function_derivation_factor(scls::__Formula_Base::Formula_Factor* current_factor, Function_Studied* current_function, std::string* redaction) {
        scls::Formula added_element_derivate;
        for(int i = 0;i<static_cast<int>(current_factor->factors().size());i++) {
            Function_Studied fs;fs.set_formula(*current_factor->factors().at(i).get());
            added_element_derivate += function_derivation(&fs, redaction);
        }

        return added_element_derivate;
    }
    scls::Formula function_derivation_sum(scls::__Formula_Base::Formula_Sum* current_sum, Function_Studied* current_function, std::string* redaction) {
        scls::Formula added_element_derivate;
        for(int i = 0;i<static_cast<int>(current_sum->formulas_add().size());i++) {
            added_element_derivate += function_derivation_factor(current_sum->formulas_add().at(i).get(), current_function, redaction);
        }

        return added_element_derivate;
    }
    scls::Formula function_derivation_fraction(scls::__Formula_Base::Formula_Fraction* current_fraction, Function_Studied* current_function, std::string* redaction) {
        // The function is more complicated
        std::string function_name = current_function->function_name + "_";
        scls::Formula numerator = (*current_fraction->numerator());
        if(current_fraction->denominator() != 0 && redaction != 0) {
            (*redaction) += "Premièrement, étudions " + function_name + " tel que " + function_name + "(" + current_function->function_unknown + ") = " + numerator.to_std_string() + ". ";
        }
        scls::Formula numerator_derivate = function_derivation_sum(current_fraction->numerator(), current_function, redaction);

        scls::Formula result;
        if(current_fraction->denominator() != 0) {
            // Calculate the denominator
            if(redaction == 0) {
                (*redaction) += "En suite, étudions " + function_name + " tel que " + function_name + "(" + current_function->function_unknown + ") = " + current_fraction->denominator()->to_std_string() + ". ";
            }
            scls::Formula denominator_derivate = function_derivation_sum(current_fraction->denominator(), current_function, redaction);

            // Do the division
            scls::__Formula_Base denominator = *current_fraction->denominator();
            result = ((numerator_derivate * denominator) - (numerator * denominator_derivate)) / (denominator * denominator);
            if(redaction != 0) {(*redaction) += std::string("Finalement, appliquons la formule de division de formes dérivées. ");}
        }
        else{result=numerator_derivate;}

        return result;
    }
    scls::Formula function_derivation(Function_Studied* current_function, std::string* redaction) {
        // Do the calculation
        scls::Formula result;

        // Write the redaction
        if(redaction != 0){(*redaction) += "Nous cherchons la dérivée de la fonction " + current_function->function_name + ". ";}

        // Check the type of function
        std::shared_ptr<scls::__Formula_Base> denominator = current_function->function_formula.denominator();
        if(current_function->function_formula.is_simple_polymonial()) {result = function_derivation_polymonial(current_function, redaction);}
        else if(current_function->function_formula.applied_function() != 0) {
            // Check the applied function
            if(redaction != 0) {
                (*redaction) += "De plus, nous voyons que la fonction \"" + current_function->function_formula.applied_function()->name() + "\" est appliquée sur la forme : prenons la en compte. ";
            }

            result = *current_function->function_formula.applied_function()->derivate_value(current_function->function_formula).get();
        }
        else {result = function_derivation_fraction(current_function->function_formula.fraction(), current_function, redaction);}

        if(redaction != 0) {
            (*redaction) += "Au final, la dérivée de " + current_function->function_name + " est " + current_function->function_name + "' tel que :</br></br><math><mi>" + current_function->function_name + "'</mi><mi>(" + current_function->function_unknown + ")</mi><mo>=</mo>" + result.to_mathml() + "</math></br></br>";
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

    // Returns the primitive of a function
    scls::Formula function_primitive_monomonial(scls::__Monomonial current_monomonial, std::string* redaction) {
        // Do the calculation
        scls::_Base_Unknown* needed_unknown = current_monomonial.contains_unknown("x");
        scls::Formula result;
        if(needed_unknown != 0) {
            // Basic exponential form
            scls::Fraction new_exponent = needed_unknown->exponent().real() + 1;
            if(new_exponent != 0) {result = scls::__Monomonial(current_monomonial.factor() / new_exponent, "x", new_exponent);}
            else {result = scls::__Monomonial(current_monomonial.factor().real(), "x", 1);}
        }
        else {result = scls::__Monomonial(current_monomonial.factor().real(), "x", 1);}

        if(redaction == 0) {
            // Write the redaction
            (*redaction) += "La forme primitive de la forme " + current_monomonial.to_std_string() + " est " + result.to_std_string() + ". ";
        }

        return result;
    }
    scls::Formula function_primitive_polymonial(Function_Studied* current_function, std::string* redaction) {
        // Do the calculation
        scls::Formula result;

        // Write the redaction
        scls::Polymonial needed_polymonial = current_function->function_formula.to_polymonial();
        if(redaction != 0) {
            (*redaction) += "Or, cette fonction n'est qu'un simple polymône. ";
            (*redaction) += "Pour étudier sa primitive, découpons cette fonction en plusieurs monômes, que nous étudierons l'un après l'autre. ";
        }

        // Study each monomonials
        for(int i = 0;i<static_cast<int>(needed_polymonial.monomonials().size());i++) {
            result += function_primitive_monomonial(needed_polymonial.monomonials()[i], redaction);
        }

        // Finish the redaction
        if(redaction != 0) {(*redaction) += "Donc, la forme primitive de " + needed_polymonial.to_std_string() + " est " + result.to_std_string() + ". ";}

        return result;
    }
    scls::Formula function_primitive(Function_Studied* current_function, std::string* redaction) {
        // Do the calculation
        scls::Formula result;

        // Write the redaction
        if(redaction != 0){(*redaction) += "Nous cherchons la primitive de la fonction " + current_function->function_name + ". ";}

        // Check the type of function
        if(current_function->function_formula.is_simple_polymonial()) {result = function_primitive_polymonial(current_function, redaction);}

        return result;
    }

    // Returns the set of roots of a function
    scls::Set_Number function_roots(Function_Studied* current_function, std::string& redaction) {
        // Create the redaction
        scls::Formula& function_studied = current_function->function_formula;
        redaction += "Nous cherchons les racines de " + function_studied.to_std_string() + ". ";
        scls::Set_Number to_return = scls::Set_Number();

        // Only one polymonial
        if(function_studied.is_simple_polymonial()) {
            scls::Polymonial polymonial = function_studied;
            int degree = polymonial.degree(current_function->function_unknown).real().to_int();
            if(polymonial.is_known()) {
                // Only one number
                scls::Fraction number = static_cast<scls::Complex>(polymonial.known_monomonial()).real();
                redaction += "Or, " + function_studied.to_std_string() + " n'est pas égal à 0, cette forme n'a donc pas de racines. ";
            }
            else if(degree == 1) {
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
            }
            else if(degree == 2) {
                // Calculate the known and unknown parts
                scls::Complex a_part = polymonial.monomonial(current_function->function_unknown, 2).factor();
                scls::Complex b_part = polymonial.monomonial(current_function->function_unknown).factor();
                scls::Complex c_part = polymonial.known_monomonial().factor();
                scls::Complex discriminant_complex = b_part * b_part - 4 * a_part * c_part;

                // Create the redaction for the discriminant
                redaction += "Or, cette forme est un polynôme de degré 2. ";
                redaction += "Pour étudier ses racines, nous devons étudier les trois parties qui la constitue. ";
                redaction += "Commençons par calculer le discriminant d de cette forme. ";
                redaction += " d = b * b - 4 * a * c = " + discriminant_complex.to_std_string_simple() + ". ";

                // Search the needed roots
                scls::Fraction discriminant = discriminant_complex.real();
                if(discriminant < 0) {redaction += "Or, d &lt; 0, donc cette forme n'a pas de solution dans l'ensemble des réels. ";}
                else if(discriminant == 0) {
                    scls::Fraction solution = ((b_part * -1) / (2 * a_part)).real();
                    redaction += "Or, d = 0, donc cette forme a une solution dans l'ensemble des réels. ";
                    redaction += "Cette solution est n = -b/2a = " + solution.to_std_string() + ". ";
                    to_return.add_number(solution);
                } else {
                    scls::Fraction sqrt_d = scls::Fraction(std::sqrt(discriminant.to_double()) * 1000000.0, 1000000.0);
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
                scls::Set_Number result = function_roots(&current_function, redaction);
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

    // Adds a function to the graphic
    std::shared_ptr<Graphic::Graphic_Function> Graphic::add_function(std::shared_ptr<Function_Studied> function_studied) {std::shared_ptr<Graphic_Function> new_function = std::make_shared<Graphic_Function>(function_studied);a_functions.push_back(new_function);a_graphic_base.get()->a_function_number++;return new_function;}

    // Draws a form on the graphic
    void Graphic::draw_form(Form_2D* needed_form, std::shared_ptr<scls::Image> to_return) {
        // Asserts
        if(needed_form->points().size() < 2){return;}
        else if(needed_form->points().size() == 2) {draw_line(needed_form->points()[0].get(), needed_form->points()[1].get(), needed_form->color_with_opacity(needed_form->border_color()), needed_form->border_radius(), needed_form->link(0).drawing_proportion, to_return);return;}

        // Triangulate the form
        std::vector<std::shared_ptr<Vector>> triangulated_points = needed_form->triangulated_points();

        // Draw the inner form
        scls::Color inner_color = needed_form->color_with_opacity(needed_form->color());
        if(inner_color.alpha() > 0) {
            for(int i = 0;i<static_cast<int>(triangulated_points.size());i+=3) {
                std::shared_ptr<Vector> current_point = triangulated_points[i];
                double first_x = graphic_x_to_pixel_x(current_point.get()->x_to_double(), to_return);
                double first_y = graphic_y_to_pixel_y_inversed(current_point.get()->y()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
                current_point = triangulated_points[i + 1];
                double second_x = graphic_x_to_pixel_x(current_point.get()->x_to_double(), to_return);
                double second_y = graphic_y_to_pixel_y_inversed(current_point.get()->y()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
                current_point = triangulated_points[i + 2];
                double third_x = graphic_x_to_pixel_x(current_point.get()->x_to_double(), to_return);
                double third_y = graphic_y_to_pixel_y_inversed(current_point.get()->y()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
                to_return.get()->fill_triangle(first_x, first_y, second_x, second_y, third_x, third_y, inner_color);
            } triangulated_points.clear();
        }

        // Draw the links
        std::shared_ptr<Vector> last_point = needed_form->points()[needed_form->points().size() - 1];
        double last_x = graphic_x_to_pixel_x(last_point.get()->x()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
        double last_y = graphic_y_to_pixel_y_inversed(last_point.get()->y()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);

        // Link each points
        scls::Color border_color = needed_form->color_with_opacity(needed_form->border_color());
        for(int j = 0;j<static_cast<int>(needed_form->points().size());j++) {
            std::shared_ptr<Vector> current_point = needed_form->points()[j];
            double needed_x = graphic_x_to_pixel_x(current_point.get()->x_to_double(), to_return);
            double needed_y = graphic_y_to_pixel_y_inversed(current_point.get()->y_to_double(), to_return);
            Form_2D::Link current_link;
            if(j <= 0){current_link = needed_form->last_link();}
            else{current_link = needed_form->link(j - 1);}

            // Apply the proportion
            double needed_proportion = current_link.drawing_proportion;
            double needed_move_x = needed_x - last_x;double needed_move_x_minus = 0;
            double needed_move_y = needed_y - last_y;double needed_move_y_minus = 0;
            if(needed_proportion >= 0) {
                needed_move_x *= needed_proportion;
                needed_move_y *= needed_proportion;
            }
            else {
                needed_move_x_minus = needed_move_x * (1.0 + needed_proportion);
                needed_move_y_minus = needed_move_y * (1.0 + needed_proportion);
            }

            to_return.get()->draw_line(last_x + needed_move_x_minus, last_y + needed_move_y_minus, last_x + needed_move_x, last_y + needed_move_y, border_color, needed_form->border_radius());
            last_point = current_point; last_x = needed_x; last_y = needed_y;
        }
    }

    // Draw a line between two points
    void Graphic::draw_line(Vector* point_1, Vector* point_2, scls::Color color, double width, double proportion, std::shared_ptr<scls::Image> to_return) {
        // Draw a line
        double last_x = graphic_x_to_pixel_x(point_1->x()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
        double last_y = graphic_y_to_pixel_y_inversed(point_1->y()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
        double needed_x = graphic_x_to_pixel_x(point_2->x()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
        double needed_y = graphic_y_to_pixel_y_inversed(point_2->y()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
        // Apply the proportion
        double needed_proportion = proportion;
        double needed_move_x = needed_x - last_x;double needed_move_x_minus = 0;
        double needed_move_y = needed_y - last_y;double needed_move_y_minus = 0;
        if(needed_proportion >= 0) {
            needed_move_x *= needed_proportion;
            needed_move_y *= needed_proportion;
        }
        else {
            needed_move_x_minus = needed_move_x * (1.0 + needed_proportion);
            needed_move_y_minus = needed_move_y * (1.0 + needed_proportion);
        }

        // Apply the proportion
        to_return.get()->draw_line(last_x + needed_move_x_minus + width / 2.0, last_y + needed_move_y_minus, last_x + needed_move_x, last_y + needed_move_y, color, width);
    }

    // Draws a point on the graphic
    void Graphic::draw_vector(Vector* needed_point, std::shared_ptr<scls::Image> to_return) {
        double needed_x = graphic_x_to_pixel_x(needed_point->x()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
        double needed_y = graphic_y_to_pixel_y_inversed(needed_point->y()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
        double point_width = 8;
        if(needed_point->type() == Vector_Type::VT_Vector) {
            to_return.get()->draw_line(graphic_x_to_pixel_x(0, to_return), graphic_y_to_pixel_y_inversed(0, to_return), needed_x, needed_y, scls::Color(255, 0, 0), 2);
        }
        else {
            to_return.get()->fill_rect(needed_x - point_width / 2.0, needed_y  - point_width / 2.0, point_width, point_width, scls::Color(255, 0, 0));
        }
    }

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

    // Creates and returns a line (and its points)
    std::shared_ptr<Form_2D> Graphic::new_line(std::string name, scls::Fraction x_1, scls::Fraction y_1, scls::Fraction x_2, scls::Fraction y_2){
        std::shared_ptr<Form_2D>to_return=std::make_shared<Form_2D>(name);
        to_return.get()->add_point(new_point(name + std::string("-p1"), x_1, y_1));
        to_return.get()->add_point(new_point(name + std::string("-p2"), x_2, y_2));
        add_form(to_return);
        return to_return;
    }

    // Creates and returns a form (and its point)
    std::shared_ptr<Form_2D> Graphic::new_form(std::string name, scls::Fraction x_1, scls::Fraction y_1, scls::Fraction x_2, scls::Fraction y_2, scls::Fraction x_3, scls::Fraction y_3, scls::Fraction x_4, scls::Fraction y_4) {
        std::shared_ptr<Form_2D>to_return=std::make_shared<Form_2D>(name);
        to_return.get()->add_point(new_point(name + std::string("-p1"), x_1, y_1));
        to_return.get()->add_point(new_point(name + std::string("-p2"), x_2, y_2));
        to_return.get()->add_point(new_point(name + std::string("-p3"), x_3, y_3));
        to_return.get()->add_point(new_point(name + std::string("-p4"), x_4, y_4));
        add_form(to_return);
        return to_return;
    }

    // Creates and returns a square (and its point)
    std::shared_ptr<Form_2D> Graphic::new_square(std::string name, scls::Fraction x, scls::Fraction y, scls::Fraction width, scls::Fraction height) {return new_form(name, x, y, x + width, y, x + width, y + height, x, y + height);}

    // Creates and returns a triangle (and its point)
    std::shared_ptr<Form_2D> Graphic::new_triangle(std::string name, scls::Fraction x_1, scls::Fraction y_1, scls::Fraction x_2, scls::Fraction y_2, scls::Fraction x_3, scls::Fraction y_3) {
        std::shared_ptr<Form_2D>to_return=std::make_shared<Form_2D>(name);
        to_return.get()->add_point(new_point(name + std::string("-p1"), x_1, y_1));
        to_return.get()->add_point(new_point(name + std::string("-p2"), x_2, y_2));
        to_return.get()->add_point(new_point(name + std::string("-p3"), x_3, y_3));
        add_form(to_return);
        return to_return;
    }

    // Struct containing some datas for positions
    struct Needed_Pos {scls::__Formula_Base::Formula pos;scls::__Formula_Base::Formula previous_pos;bool previous_pos_used = false;};
    // Draw the bases of the image
    void Graphic::image_draw_base(std::shared_ptr<scls::Image> image) {
        // Horizontal lines
        double current_y = pixel_y_to_graphic_y(image.get()->height(), image).to_double();
        current_y = floor(current_y);
        double needed_y = graphic_y_to_pixel_y(current_y, image);
        while(needed_y < image.get()->width()) {
            scls::Color needed_color = scls::Color(0, 0, 0);
            int needed_height = 1;
            if(current_y == 0){needed_height = 2;}else{needed_color=scls::Color(125, 125, 125);}
            image.get()->fill_rect(0, image.get()->height() - needed_y, image.get()->width(), needed_height, needed_color);
            current_y++;
            needed_y = graphic_y_to_pixel_y(current_y, image);
        }
        // Vertical lines
        double current_x = pixel_x_to_graphic_x(0, image).to_double();
        current_x = ceil(current_x);
        int needed_x = graphic_x_to_pixel_x(current_x, image);
        while(needed_x < image.get()->width()) {
            scls::Color needed_color = scls::Color(0, 0, 0);
            int needed_width = 1;
            if(current_x == 0){needed_width = 2;}else{needed_color=scls::Color(125, 125, 125);}
            image.get()->fill_rect(needed_x, 0, needed_width, image.get()->height(), needed_color);
            current_x++;
            needed_x = graphic_x_to_pixel_x(current_x, image);
        }
    }
    // Draw a function on the image
    void Graphic::image_draw_function(std::shared_ptr<scls::Image> to_return, std::shared_ptr<Graphic_Function> needed_function, std::vector<scls::Fraction>& screen_pos) {
        // Asserts
        if(needed_function.get()->definition_set() == 0) {scls::print("Warning", "PLEOS Graphic", std::string("The \"") + needed_function.get()->name() + std::string("\" function has no definition interval calculated."));return;}

        // Get the values
        scls::Formula needed_formula = needed_function.get()->formula();
        std::vector<Needed_Pos> needed_pos = std::vector<Needed_Pos>(to_return.get()->width() + 1);
        // Get each values of the function
        scls::__Formula_Base::Formula last_pos;
        for(int j = 0;j<static_cast<int>(to_return.get()->width()) + 1;j++){
            // Get the needed pos
            Needed_Pos to_add;
            if(needed_function.get()->definition_set()->is_in(screen_pos[j])) {to_add.pos = needed_formula.value(screen_pos[j]);}
            else {to_add.pos = needed_formula.value(screen_pos[j] - scls::Fraction(1, 1000));}
            needed_pos[j] = to_add;

            // Check according to the last value
            if(j > 0 && !needed_function.get()->definition_set()->is_in(scls::Interval(screen_pos[j - 1], screen_pos[j]))) {
                needed_pos[j - 1].pos = needed_formula.value(screen_pos[j - 1] + scls::Fraction(1, 1000));
                needed_pos[j].previous_pos = needed_formula.value(screen_pos[j] - scls::Fraction(1, 1000));
                needed_pos[j].previous_pos_used = true;
            }

            // Finalise the creation
            last_pos = to_add.pos;
        }

        // Adapt each values to the screen
        struct Needed_Pos_Screen {int pos;int previous_pos;bool previous_pos_used = false;};
        std::vector<Needed_Pos_Screen> needed_y = std::vector<Needed_Pos_Screen>(to_return.get()->width() + 1);
        for(int i = 0;i<static_cast<int>(to_return.get()->width()) + 1;i++){
            scls::Fraction value = needed_pos[i].pos.formula_base()->value_to_fraction().normalized();
            needed_y[i].pos = graphic_y_to_pixel_y(value.to_double(), to_return);
            // Check the previous pos
            if(needed_pos[i].previous_pos_used) {
                value = needed_pos[i].previous_pos.formula_base()->value_to_fraction().normalized();
                needed_y[i].previous_pos = graphic_y_to_pixel_y(value.to_double(), to_return);
                needed_y[i].previous_pos_used = true;
            }
        }

        // Area under the curve
        for(int i = 0;i<static_cast<int>(needed_function.get()->curve_areas_number());i++) {
            int border_width = 1;
            scls::Fraction current_x = needed_function.get()->curve_area_start(i);
            int rect_number = needed_function.get()->curve_area_rectangle_number(i);
            scls::Fraction rect_width = (needed_function.get()->curve_area_end(i) - needed_function.get()->curve_area_start(i)) / rect_number;
            for(int j = 0;j<rect_number;j++) {
                scls::Fraction needed_value = needed_formula.value(current_x).real();
                if(needed_value != 0) {
                    to_return.get()->draw_rect(graphic_x_to_pixel_x(current_x.to_double(), to_return), graphic_y_to_pixel_y(-needed_value.to_double(), to_return), (rect_width * pixel_by_case_x()).to_double(), (needed_value * pixel_by_case_y()).to_double(), border_width, scls::Color(255, 0, 0));
                    to_return.get()->fill_rect(graphic_x_to_pixel_x(current_x.to_double(), to_return) + border_width, graphic_y_to_pixel_y(-needed_value.to_double(), to_return) + border_width, (rect_width * pixel_by_case_x()).to_double() - 2 * border_width, (needed_value * pixel_by_case_y()).to_double() - 2 * border_width, scls::Color(0, 255, 0));
                }
                current_x += rect_width;
            }
        }

        // Draw each pixel
        int width = 3;
        for(int j = 0;j<static_cast<int>(to_return.get()->width());j++) {
            int y_1 = to_return.get()->height() - needed_y[j].pos;
            int y_2 = to_return.get()->height() - needed_y[j + 1].pos;
            if(needed_y[j + 1].previous_pos_used){y_1 = to_return.get()->height() - needed_y[j + 1].previous_pos;}
            if(y_1 > to_return.get()->height() && y_2 > to_return.get()->height()){continue;}
            else if(y_1 < 0 && y_2 < 0){continue;}
            // Draw the point
            int needed_height = std::abs(y_1 - y_2);
            int needed_y = std::min(y_1, y_2) - width / 2.0;
            if(needed_y < to_return.get()->height() && needed_y >= -needed_height) {
                to_return.get()->fill_rect(j - width / 2.0, needed_y, width, needed_height + width, scls::Color(255, 0, 0));
            }
        }
    }
    // Returns the image of the graphic
    int Graphic::graphic_x_to_pixel_x(double x, int image_width){return std::ceil((x - middle_x().to_double()) * pixel_by_case_x() + (static_cast<double>(image_width) / 2.0));};
    int Graphic::graphic_y_to_pixel_y(double y, int needed_height){return std::ceil((y - middle_y().to_double()) * pixel_by_case_y()) + (needed_height / 2.0);};
    int Graphic::graphic_y_to_pixel_y_inversed(double y, int needed_height){return needed_height - graphic_y_to_pixel_y(y, needed_height);};
    scls::Fraction Graphic::pixel_x_to_graphic_x(int x, int image_width){return middle_x() + ((scls::Fraction(x) - scls::Fraction(image_width, 2)) / scls::Fraction::from_double(pixel_by_case_x()));}
    scls::Fraction Graphic::pixel_y_to_graphic_y(int y, int needed_height){return middle_y() + ((scls::Fraction(needed_height, 2) - scls::Fraction(y)) / scls::Fraction::from_double(pixel_by_case_y()));}
    std::shared_ptr<scls::Image> Graphic::to_image(int width_in_pixel, int height_in_pixel) {
        // Create the image
        std::shared_ptr<scls::Image> to_return = std::make_shared<scls::Image>(width_in_pixel, height_in_pixel, style()->background_color());

        // Handle the height and width
        if(a_graphic_base.get()->a_height != -1) {a_graphic_base.get()->a_pixel_by_case_y = static_cast<double>(height_in_pixel) / a_graphic_base.get()->a_height;}
        if(a_graphic_base.get()->a_width != -1) {a_graphic_base.get()->a_pixel_by_case_x = static_cast<double>(width_in_pixel) / a_graphic_base.get()->a_width;}

        // Draw the basic lines
        if(draw_base() || draw_sub_bases()) {image_draw_base(to_return);}

        // Get the datas for the drawing
        scls::Fraction image = pixel_x_to_graphic_x(0, to_return).normalized(5);
        scls::Fraction multiplier = (scls::Fraction(1) / scls::Fraction::from_double(pixel_by_case_x())).normalized(5);
        std::vector<scls::Fraction> screen_pos = std::vector<scls::Fraction>(to_return.get()->width() + 1);
        for(int i = 0;i<static_cast<int>(to_return.get()->width()) + 1;i++){screen_pos[i] = image.normalized(5);image += multiplier;}
        // Draw the functions
        for(int i = 0;i<static_cast<int>(a_functions.size());i++) {image_draw_function(to_return, a_functions[i], screen_pos);}

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
        // Draw the forms
        for(int i = 0;i<static_cast<int>(a_forms_2d.size());i++) {draw_form(a_forms_2d[i].get(), to_return);}
        // Draw the texts
        scls::Text_Image_Generator tig;
        for(int i = 0;i<static_cast<int>(a_texts.size());i++) {
            scls::Text_Style current_style = *a_texts.at(i).get()->style.get();current_style.set_color(a_texts.at(i).get()->color_with_opacity(current_style.color()));
            std::shared_ptr<scls::Image> needed_image = tig.image_shared_ptr(a_texts.at(i).get()->content, current_style);
            double needed_x = graphic_x_to_pixel_x(a_texts.at(i).get()->x.to_double(), to_return);
            double needed_y = graphic_y_to_pixel_y_inversed(a_texts.at(i).get()->y.to_double(), to_return);
            to_return.get()->paste(needed_image.get(), needed_x - needed_image.get()->width() / 2, needed_y - needed_image.get()->height() / 2);
        }
        // Draw the vectors
        for(int i = 0;i<static_cast<int>(a_vectors.size());i++) {draw_vector(a_vectors[i].get(), to_return);}

        return to_return;
    }

    //******************
    //
    // Physic handling
    //
    //******************

    // Checks if a collision occurs with an another collision
    Collision_Rect_Rect __check_collision_rect_line(Graphic::Graphic_Collision* collision_rect, Graphic::Graphic_Collision* collision_line, Graphic::Graphic_Physic* dynamic_object_1){
        // Check bottom collision
        scls::Crossing_Datas_Segment datas_bottom = scls::check_crossing_segment(collision_rect->min_x().to_double(), collision_rect->min_y().to_double(), collision_rect->max_x().to_double(), collision_rect->min_y().to_double(), collision_line->x_1().to_double(), collision_line->y_1().to_double(), collision_line->x_2().to_double(), collision_line->y_2().to_double());

        // Check right collision
        scls::Crossing_Datas_Segment datas_right = scls::check_crossing_segment(collision_rect->max_x().to_double(), collision_rect->max_y().to_double(), collision_rect->max_x().to_double(), collision_rect->min_y().to_double(), collision_line->x_1().to_double(), collision_line->y_1().to_double(), collision_line->x_2().to_double(), collision_line->y_2().to_double());

        // Assert
        if(!datas_bottom.crossed_in_segment && !datas_right.crossed_in_segment){return Collision_Rect_Rect();}

        // Get the differences
        double slope_x = std::abs(collision_line->x_2().to_double() - collision_line->x_1().to_double());
        double slope_y = std::abs(collision_line->y_2().to_double() - collision_line->y_1().to_double());
        double temp = 0;scls::normalize_3d(slope_x, temp, slope_y);
        Collision_Rect_Rect to_return;
        to_return.happens = true;
        if(datas_bottom.crossed_in_segment){to_return.side_bottom = true;}
        if(datas_right.crossed_in_segment){to_return.side_right = true;}

        // Check the movement
        if(to_return.side_bottom && to_return.side_right) {
            double force_distribution = 0.5;
            std::cout << slope_x << " " << slope_y << " " << dynamic_object_1->velocity_x() << " " << dynamic_object_1->velocity_y() << std::endl;

            // Handle next movement
            if(dynamic_object_1->next_movement_x() > 0 && dynamic_object_1->next_movement_y() < 0){
                // Handle both axis at once
                dynamic_object_1->accelerate_x(dynamic_object_1->velocity_x() * slope_y * -force_distribution);
                dynamic_object_1->accelerate_y(dynamic_object_1->velocity_x() * slope_y * force_distribution);
            }
            else {
                if(dynamic_object_1->next_movement_x() > 0){dynamic_object_1->accelerate_y(dynamic_object_1->velocity_x() * force_distribution);dynamic_object_1->remove_x_velocity();}
                if(dynamic_object_1->next_movement_y() < 0){dynamic_object_1->accelerate_x(dynamic_object_1->velocity_y() * force_distribution);dynamic_object_1->remove_y_velocity();}
            }
        }
        if(to_return.side_bottom && !to_return.side_right) {
            if(dynamic_object_1->velocity().y() < 0){dynamic_object_1->remove_y_velocity();}
        }
        if(to_return.side_left) {
            if(dynamic_object_1->velocity().x() < 0){dynamic_object_1->remove_x_velocity();}
        }
        if(to_return.side_right && !to_return.side_bottom) {
            if(dynamic_object_1->velocity().x() > 0){dynamic_object_1->remove_x_velocity();}
        }
        if(to_return.side_top) {
            if(dynamic_object_1->velocity().y() > 0){dynamic_object_1->remove_y_velocity();}
        }

        // Return the result
        return to_return;
    }
    Collision_Rect_Rect __check_collision_rect_rect(Graphic::Graphic_Collision* collision_1, Graphic::Graphic_Collision* collision_2, Graphic::Graphic_Physic* dynamic_object_1){
        // Check X
        bool x_1 = (collision_1->max_x() > collision_2->min_x() && collision_2->max_x() > collision_1->max_x());
        bool x_2 = (collision_2->max_x() > collision_1->min_x() && collision_1->max_x() > collision_2->max_x());
        bool x_3 = (collision_2->max_x() >= collision_1->max_x() && collision_1->min_x() >= collision_2->min_x());
        bool x_4 = (collision_1->max_x() >= collision_2->max_x() && collision_2->min_x() >= collision_1->min_x());
        if(!(x_1 || x_2 || x_3 || x_4)){return Collision_Rect_Rect();}

        // Check Y
        bool y_1 = (collision_1->max_y() > collision_2->min_y() && collision_2->max_y() > collision_1->max_y());
        bool y_2 = (collision_2->max_y() > collision_1->min_y() && collision_1->max_y() > collision_2->max_y());
        bool y_3 = (collision_2->max_y() >= collision_1->max_y() && collision_1->min_y() >= collision_2->min_y());
        bool y_4 = (collision_1->max_y() >= collision_2->max_y() && collision_2->min_y() >= collision_1->min_y());
        if(!(y_1 || y_2 || y_3 || y_4)){return Collision_Rect_Rect();}

        // Get the differences
        Collision_Rect_Rect to_return;
        to_return.happens = true;

        // X
        double x_diff = collision_2->min_x().to_double() - collision_1->max_x().to_double();
        double x_diff_temp = collision_1->min_x().to_double() - collision_2->max_x().to_double();
        if(std::abs(x_diff) > std::abs(x_diff_temp)){x_diff = x_diff_temp;}
        // Y
        double y_diff = collision_2->min_y().to_double() - collision_1->max_y().to_double();
        double y_diff_temp = collision_1->min_y().to_double() - collision_2->max_y().to_double();
        if(std::abs(y_diff) > std::abs(y_diff_temp)){y_diff = y_diff_temp;}

        // Returns the good value
        if(std::abs(x_diff) > std::abs(y_diff)){
            // Returns a Y
            if(y_1 || y_4){to_return.side_top = true;}
            else{to_return.side_bottom = true;}

            // Set the distance
            to_return.distance = std::abs(y_diff);
        }
        else {
            // Returns a X
            if(x_2 || x_3){to_return.side_left = true;}
            else{to_return.side_right = true;}

            // Set the distance
            to_return.distance = std::abs(x_diff);
        }

        // Check the movement
        if(to_return.side_bottom) {
            // Friction
            if(dynamic_object_1->velocity_y() < 0){dynamic_object_1->accelerate_x(dynamic_object_1->velocity_x() * -0.01);}

            // Contact
            if(dynamic_object_1->velocity_y() < 0){dynamic_object_1->remove_y_velocity();dynamic_object_1->set_velocity_y(to_return.distance);}
        }
        if(to_return.side_left) {
            if(dynamic_object_1->next_movement_x() < 0){dynamic_object_1->remove_x_velocity();dynamic_object_1->set_velocity_x(to_return.distance);}
        }
        if(to_return.side_right) {
            if(dynamic_object_1->next_movement_x() > 0){dynamic_object_1->remove_x_velocity();dynamic_object_1->set_velocity_x(-to_return.distance);}
        }
        if(to_return.side_top) {
            if(dynamic_object_1->velocity_y() > 0){dynamic_object_1->remove_y_velocity();dynamic_object_1->set_velocity_y(-to_return.distance);}
        }

        // Return the result
        return to_return;
    };
    Collision_Rect_Rect __check_collision(Graphic::Graphic_Collision* collision_1, Graphic::Graphic_Collision* collision_2, Graphic::Graphic_Physic* dynamic_object_1) {
        // Asserts
        if(collision_2 == 0 || collision_2->attached_object() == collision_1->attached_object()){return Collision_Rect_Rect();}

        // Both objects are rect
        if(collision_1->type() == Graphic::Graphic_Collision_Type::GCT_Rect && collision_2->type() == Graphic::Graphic_Collision_Type::GCT_Rect) {
            Collision_Rect_Rect current_result = __check_collision_rect_rect(collision_1, collision_2, dynamic_object_1);
            if(current_result.happens) {return current_result;}
        }
        else if(collision_1->type() == Graphic::Graphic_Collision_Type::GCT_Rect && collision_2->type() == Graphic::Graphic_Collision_Type::GCT_Line) {
            Collision_Rect_Rect current_result = __check_collision_rect_line(collision_1, collision_2, dynamic_object_1);
            if(current_result.happens) {return current_result;}
        }

        return Collision_Rect_Rect();
    }

    // Checks if a collision occurs with an another object
    void Graphic::Graphic_Physic::check_collision(Graphic::Graphic_Collision* collision) {
        // Asserts
        if(collision == 0 || collision->attached_object() == attached_object()){return;}

        // Check each collision
        for(int i = 0;i<static_cast<int>(a_collisions.size());i++) {Collision_Rect_Rect current_result = __check_collision(a_collisions[i].get(), collision, this);}
    }

    // Loads 100 X 100 physic map
    void Graphic::load_physic_map(int middle_loading_x, int middle_loading_y) {
        // Get the needed datas
        int height = 100;int width = 100;
        a_physic_map_start_x = middle_loading_x - width / 2;
        a_physic_map_start_y = middle_loading_y - height / 2;

        // Create the cases
        a_physic_map = std::vector<std::vector<Physic_Case>>(width, std::vector<Physic_Case>(height));
    }

    // Graphic constructor
    Graphic_Object::Graphic_Object(scls::_Window_Advanced_Struct& window, std::string name, std::weak_ptr<scls::GUI_Object> parent):scls::GUI_Object(window, name, parent){update_texture();}

    // Render the object
    void Graphic_Object::render(glm::vec3 scale_multiplier) {scls::GUI_Object::render(scale_multiplier);}

    // Scale the GUI Object
    void Graphic_Object::Graphic_GUI_Object::scale(Graphic* graphic, int image_width, int image_height){
        a_object.get()->set_height_in_pixel(std::ceil(graphic->pixel_by_case_y()) * a_height.to_double());
        a_object.get()->set_width_in_pixel(std::ceil(graphic->pixel_by_case_x()) * a_width.to_double());
        a_object.get()->set_x_in_pixel(graphic->graphic_x_to_pixel_x((a_x - a_width / 2).to_double(), image_width));
        a_object.get()->set_y_in_pixel(graphic->graphic_y_to_pixel_y((a_y - a_height / 2).to_double(), image_height));
        a_object.get()->set_texture_scale_x(width().to_double());a_object.get()->set_texture_scale_y(height().to_double());
    };

    // Updates the object
    int __graphic_object_render_number = 0;
    void Graphic_Object::update_event() {
        GUI_Object::update_event();
        //if(window_struct().time_since_last_fps_calculation() <= 0){std::cout << "R " << __graphic_object_render_number << std::endl;__graphic_object_render_number=0;}

        // Update the object
        for(int i = 0;i<static_cast<int>(a_gui_objects.size());i++) {
            if(a_gui_objects[i].get() != 0) {
                a_gui_objects[i].get()->update_event();
            }
        }

        // Update the physic
        int needed_update_physic = update_physic();

        // Move the plane
        bool modified = (needed_update_physic > 0);
        if(is_focused()) {
            scls::Fraction speed = scls::Fraction(round(5.0 * window_struct().delta_time() * 10000.0), 10000);
            if(window_struct().key_pressed("q")){middle_x_add(speed * -1);modified = true;}
            if(window_struct().key_pressed("d")){middle_x_add(speed);modified = true;}
            if(window_struct().key_pressed("z")){middle_y_add(speed);modified = true;}
            if(window_struct().key_pressed("s")){middle_y_add(speed * -1);modified = true;}
            // Zoom or unzoom
            double zoom_speed = 30 * window_struct().delta_time();
            if(window_struct().key_pressed("w")){pixel_by_case_x_add(zoom_speed * -1);pixel_by_case_y_add(zoom_speed * -1);modified = true;}
            if(window_struct().key_pressed("c")){pixel_by_case_x_add(zoom_speed);pixel_by_case_y_add(zoom_speed);modified = true;}
        }

        // Handle clicks
        if(is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
            if(operation_at_click() == PLEOS_OPERATION_VECTOR) {
                // Add a vector
                double needed_x = window_struct().mouse_x() - x_in_absolute_pixel();
                double needed_y = window_struct().mouse_y() - (window_struct().window_height() - (y_in_absolute_pixel() + height_in_pixel()));
                scls::Fraction movement_x = pixel_x_to_graphic_x(needed_x, texture()->image_shared_ptr());
                scls::Fraction movement_y = pixel_y_to_graphic_y(needed_y, texture()->image_shared_ptr());
                add_vector(Vector("a", movement_x, movement_y));
                a_created_vectors_at_click.push_back(vectors().at(vectors().size() - 1));
                modified = true;
            }
            else if(operation_at_click() == PLEOS_OPERATION_POINT) {
                // Add a point
                double needed_x = window_struct().mouse_x() - x_in_absolute_pixel();
                double needed_y = window_struct().mouse_y() - (window_struct().window_height() - (y_in_absolute_pixel() + height_in_pixel()));
                scls::Fraction movement_x = pixel_x_to_graphic_x(needed_x, texture()->image_shared_ptr());
                scls::Fraction movement_y = pixel_y_to_graphic_y(needed_y, texture()->image_shared_ptr());
                Vector needed_vector = Vector("a", movement_x, movement_y);
                needed_vector.set_type(Vector_Type::VT_Point);
                add_vector(needed_vector);
                a_created_vectors_at_click.push_back(vectors().at(vectors().size() - 1));
                modified = true;
            }
            else if(operation_at_click() == PLEOS_OPERATION_FORM) {
                // Add a form
                // Check if the form is created
                if(current_form_2d() == 0){a_current_form_2d=std::make_shared<Form_2D>("Forme");add_form(a_current_form_2d);}
                // Add a point
                double needed_x = window_struct().mouse_x() - x_in_absolute_pixel();
                double needed_y = window_struct().mouse_y() - (window_struct().window_height() - (y_in_absolute_pixel() + height_in_pixel()));
                scls::Fraction movement_x = pixel_x_to_graphic_x(needed_x, texture()->image_shared_ptr());
                scls::Fraction movement_y = pixel_y_to_graphic_y(needed_y, texture()->image_shared_ptr());
                std::shared_ptr<Vector> needed_vector = std::make_shared<Vector>(std::string("Forme_") + std::to_string(current_form_2d()->points().size()), movement_x, movement_y);
                needed_vector.get()->set_type(Vector_Type::VT_Point);
                add_point(needed_vector);
                a_created_vectors_at_click.push_back(vectors().at(vectors().size() - 1));
                current_form_2d()->add_point(needed_vector);
                modified = true;
            }
        }

        if(modified){update_texture();}
    }

    // Updates the physic
    scls::Point_3D gravity = scls::Point_3D(0, -3.71, 0);
    int Graphic_Object::update_physic() {
        // Realised updates
        int needed_update = 0;

        // Apply gravity
        for(int i = 0;i<static_cast<int>(a_gui_objects.size());i++) {
            if(a_gui_objects[i].get()->physic_object() != 0 && a_gui_objects[i].get()->physic_object()->use_gravity()) {
                a_gui_objects[i].get()->physic_object()->velocity() += (gravity * window_struct().delta_time());
                needed_update++;
            }
        }

        // Update each objects in the case
        for(int i = 0;i<static_cast<int>(a_gui_objects.size());i++) {
            if(a_gui_objects[i].get()->physic_object() != 0) {
                if(!a_gui_objects[i].get()->physic_object()->is_static()) {
                    // Get the needed datas
                    int needed_height = a_gui_objects[i].get()->max_y_next().to_double_ceil() - a_gui_objects[i].get()->min_y_next().to_double_floor();
                    int needed_width = a_gui_objects[i].get()->max_x_next().to_double_ceil() - a_gui_objects[i].get()->min_x_next().to_double_floor();
                    int x_start = a_gui_objects[i].get()->min_x_next().to_double_floor();
                    int y_start = a_gui_objects[i].get()->min_y_next().to_double_floor();

                    // Check the cases
                    for(int j = 0;j<needed_width;j++) {
                        for(int h = 0;h<needed_height;h++) {
                            Graphic::Physic_Case* current_case = physic_case(x_start + j, y_start + h);
                            if(current_case->static_objects_collisions.size() > 0){
                                for(int h = 0;h<static_cast<int>(current_case->static_objects_collisions.size());h++) {
                                    a_gui_objects[i].get()->physic_object()->check_collision(current_case->static_objects_collisions[h].lock().get());
                                }
                            }
                        }
                    }
                }
                else if(a_gui_objects[i].get()->moved_during_this_frame()) {
                    // Delete the last cases
                    for(int j = 0;j<static_cast<int>(a_gui_objects[i].get()->physic_object()->used_physic_case().size());j++) {
                        for(int k = 0;k<static_cast<int>(a_gui_objects[i].get()->physic_object()->collisions().size());k++) {
                            a_gui_objects[i].get()->physic_object()->used_physic_case()[j]->delete_static_object_collision(a_gui_objects[i].get()->physic_object()->collisions()[k].get());
                        }
                    }

                    // Get the needed datas
                    int needed_height = a_gui_objects[i].get()->max_y_next().to_double_ceil() - a_gui_objects[i].get()->min_y_next().to_double_floor();
                    int needed_width = a_gui_objects[i].get()->max_x_next().to_double_ceil() - a_gui_objects[i].get()->min_x_next().to_double_floor();
                    int x_start = a_gui_objects[i].get()->min_x_next().to_double_floor();
                    int y_start = a_gui_objects[i].get()->min_y_next().to_double_floor();

                    // Add the cases
                    for(int j = 0;j<needed_width;j++) {
                        for(int h = 0;h<needed_height;h++) {
                            Graphic::Physic_Case* current_case = physic_case(x_start + j, y_start + h);
                            for(int l = 0;l<static_cast<int>(a_gui_objects[i].get()->physic_object()->collisions().size());l++){
                                current_case->static_objects_collisions.push_back(a_gui_objects[i].get()->physic_object()->collisions()[l]);
                            }
                            a_gui_objects[i].get()->physic_object()->used_physic_case().push_back(current_case);
                        }
                    }
                }
            }
        }

        // Apply next movement
        for(int i = 0;i<static_cast<int>(a_gui_objects.size());i++) {
            if(a_gui_objects[i].get()->physic_object() != 0) {
                a_gui_objects[i].get()->__move();
                needed_update++;
            }
        }

        return needed_update;
    }

    // Update the texture of the object
    void Graphic_Object::update_texture(){
        // Set the good image
        int needed_width = width_in_pixel();int needed_height = height_in_pixel();
        if(use_image()) {
            std::shared_ptr<scls::Image> needed_image = a_datas.to_image(needed_width, needed_height);
            texture()->set_image(needed_image);
        }
        __graphic_object_render_number++;

        // Update the objects
        for(int i = 0;i<static_cast<int>(a_gui_objects.size());i++) {
            Graphic_Object::Graphic_GUI_Object* current = a_gui_objects.at(i).get();
            current->scale(&a_datas, needed_width, needed_height);
        }

        // Update the parent
        set_should_render_during_this_frame(true);
    };
}
