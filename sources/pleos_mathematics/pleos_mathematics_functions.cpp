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

    // Returns the introduction for this function
    std::string Function_Studied::introduction(scls::Textual_Math_Settings* settings) const {
        if(settings != 0 && settings->introduction_in_mathml()) {
            return std::string("Nous avons la fonction ") + name() + std::string(" tel que :</br><math><mi>") + name() + std::string("</mi><mo>(</mo><mi>") + a_function_unknown + std::string("</mi><mo>)</mo><mo>=</mo>") + formula()->to_mathml(settings) + std::string("</math>");
        }
        return std::string("Nous avons la fonction ") + name() + std::string(" tel que ") + name() + std::string("(") + a_function_unknown + std::string(") = ") + formula()->to_std_string(settings) + std::string(" .");
    }

    // Returns the text for the table to xml
    std::string Function_Table::to_xml(std::string* redaction, scls::Textual_Math_Settings* settings) {
        std::string to_return = std::string("<table>");

        // Check if the sign has been calculated
        for(int i = 0;i<static_cast<int>(a_studied_function.size());i++) {if(a_studied_function.at(i).get()->sign_set() == 0){function_sign(a_studied_function.at(i).get(), redaction, settings);}}

        // Create the table
        Function_Studied* current_function = a_studied_function.at(0).get();
        to_return += std::string("<case x=0 y=1 content=\"") + current_function->name() + std::string("(") + current_function->unknown() + std::string(")") + std::string("\">");
        to_return += std::string("<case x=0 y=0 content=\"") + current_function->unknown() + std::string("\">");

        // Get the x values
        bool end_infinite = false;bool start_infinite = false;
        std::vector<scls::Fraction> x_values = std::vector<scls::Fraction>();
        for(int i = 0;i<static_cast<int>(a_studied_function.size());i++) {
            current_function = a_studied_function.at(i).get();

            // Get each definition values
            scls::Set_Number* definition_set = current_function->definition_set();
            scls::Set_Number* sign_set = current_function->sign_set();

            // Add the needed values
            for(int j = 0;j<static_cast<int>(definition_set->intervals().size());j++) {
                if(!definition_set->intervals().at(j).end_infinite()){x_values.push_back(definition_set->intervals().at(j).end());}else{end_infinite=true;}
                if(!definition_set->intervals().at(j).start_infinite()){x_values.push_back(definition_set->intervals().at(j).start());}else{start_infinite=true;}
            }
            for(int j = 0;j<static_cast<int>(sign_set->intervals().size());j++) {
                if(!sign_set->intervals().at(j).end_infinite()){x_values.push_back(sign_set->intervals().at(j).end());}else{end_infinite=true;}
                if(!sign_set->intervals().at(j).start_infinite()){x_values.push_back(sign_set->intervals().at(j).start());}else{start_infinite=true;}
            }
            scls::sort_fractions(x_values);
            scls::remove_duplication_sorted_fractions(x_values);
        }

        // Adds the x values
        int current_case_x = 1;
        // First value
        if(start_infinite) {
            to_return += std::string("<case right_border=0 x=") + std::to_string(current_case_x) + std::string(" y=0 content=\"-inf\">");
            current_case_x++;
            to_return += std::string("<case right_border=0 x=") + std::to_string(current_case_x) + std::string(" y=0>");
            current_case_x++;
        }
        // Other values
        for(int i = 0;i<static_cast<int>(x_values.size());i++) {
            to_return += std::string("<case right_border=0 x=") + std::to_string(current_case_x) + std::string(" y=0 content=\"") + x_values.at(i).to_std_string(settings) + std::string("\">");
            current_case_x++;
            if(i < static_cast<int>(x_values.size()) - 1 || end_infinite) {
                to_return += std::string("<case right_border=0 x=") + std::to_string(current_case_x) + std::string(" y=0>");
                current_case_x++;
            }
        }
        // Last value
        if(end_infinite) {
            to_return += std::string("<case right_border=0 x=") + std::to_string(current_case_x) + std::string(" y=0 content=\"+inf\">");
            current_case_x++;
        }

        // Draw the table
        for(int i = 0;i<static_cast<int>(a_studied_function.size());i++) {
            int current_case_y = i + 1;
            current_function = a_studied_function.at(i).get();

            // Get each definition values
            scls::Set_Number* definition_set = current_function->definition_set();
            scls::Set_Number* roots_set = current_function->roots();
            scls::Set_Number* sign_set = current_function->sign_set();

            // Handle the first value
            current_case_x = 1;
            if(start_infinite) {
                to_return += std::string("<case right_border=0 x=") + std::to_string(current_case_x) + std::string(" y=") + std::to_string(current_case_y) + std::string(">");
                current_case_x++;

                // Sign
                scls::Fraction needed_value = 0;if(x_values.size()>0){needed_value=x_values.at(0);}
                if(sign_set->is_in(needed_value - 1)) {to_return += std::string("<case right_border=0 x=") + std::to_string(current_case_x) + std::string(" y=") + std::to_string(current_case_y) + std::string(" content=\"+\">");}
                else {to_return += std::string("<case right_border=0 x=") + std::to_string(current_case_x) + std::string(" y=") + std::to_string(current_case_y) + std::string(" content=\"-\">");}
                current_case_x++;
            }

            // Add the needed values
            for(int j = 0;j<static_cast<int>(x_values.size());j++) {
                // Current needed value
                scls::Fraction current_value = x_values.at(j);scls::Fraction needed_value = current_value;
                if(j < static_cast<int>(x_values.size()) - 1){needed_value = (needed_value + x_values.at(j + 1)) / 2;}
                else{needed_value += 1;}

                // Needed conclusion
                if(roots_set->is_in(current_value)) {
                    to_return += std::string("<case right_border=0 x=") + std::to_string(current_case_x) + std::string(" y=") + std::to_string(current_case_y) + std::string(" content=\"0\">");
                    current_case_x++;

                    // Sign
                    if(sign_set->is_in(needed_value)) {to_return += std::string("<case right_border=0 x=") + std::to_string(current_case_x) + std::string(" y=") + std::to_string(current_case_y) + std::string(" content=\"+\">");}
                    else {to_return += std::string("<case right_border=0 x=") + std::to_string(current_case_x) + std::string(" y=") + std::to_string(current_case_y) + std::string(" content=\"-\">");}
                    current_case_x++;
                }
                else if(definition_set->is_in(current_value)) {
                    to_return += std::string("<case right_border=0 x=") + std::to_string(current_case_x) + std::string(" y=") + std::to_string(current_case_y) + std::string(" content=\"") + current_function->formula()->value(needed_value).real().to_std_string(settings) + std::string("\">");
                    current_case_x++;

                    // Sign
                    if(sign_set->is_in(needed_value)) {to_return += std::string("<case right_border=0 x=") + std::to_string(current_case_x) + std::string(" y=") + std::to_string(current_case_y) + std::string(" content=\"+\">");}
                    else {to_return += std::string("<case right_border=0 x=") + std::to_string(current_case_x) + std::string(" y=") + std::to_string(current_case_y) + std::string(" content=\"-\">");}
                    current_case_x++;
                }
                else {

                    to_return += std::string("<case right_border=0 x=") + std::to_string(current_case_x) + std::string(" y=") + std::to_string(current_case_y) + std::string(" content=\"||\">");
                    current_case_x++;
                    to_return += std::string("<case right_border=0 x=") + std::to_string(current_case_x) + std::string(" y=") + std::to_string(current_case_y) + std::string(" content=\"||\">");
                    current_case_x++;
                }
            }

            // Handle the last value
            if(end_infinite) {
                to_return += std::string("<case right_border=0 x=") + std::to_string(current_case_x) + std::string(" y=") + std::to_string(current_case_y) + std::string(">");
                current_case_x++;
            }
        }

        // Finish the table
        to_return += std::string("</table>");
        return to_return;
    }

    // Returns the definition set of a function
    scls::Set_Number function_definition_set_fraction(scls::__Formula_Base* denominator, Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings) {
        // Do the redaction
        scls::Set_Number to_return = scls::Set_Number::set_real();
        if(redaction != 0) {
            (*redaction) += "Cette forme contient un dénominateur global " + denominator->to_std_string(settings) + ". ";
            (*redaction) += "Donc, elle n'est pas définie pour " + denominator->to_std_string(settings) + " = 0. ";
        }

        // Get the needed value
        std::shared_ptr<scls::Set_Number> denominator_null;
        std::shared_ptr<Function_Studied> fs = Function_Studied::new_function_studied_shared_ptr(*denominator);
        fs.get()->set_function_unknown(current_function->unknown());
        denominator_null = std::make_shared<scls::Set_Number>(function_roots(fs.get(), redaction, settings));
        scls::Fraction needed_value;if(denominator_null.get()->numbers().size() > 0){needed_value = denominator_null.get()->numbers().at(0).real();}

        // TEMPORARY SET THE INTERVAL
        to_return = scls::Set_Number::real();
        if(denominator_null.get()->numbers().size() > 0){to_return.exclude(denominator_null.get()->numbers().at(0).real());}
        if(denominator_null.get()->numbers().size() > 1){to_return.exclude(denominator_null.get()->numbers().at(1).real());}

        //scls::Interval interval = scls::Interval(needed_value - 1, true, needed_value, false);interval.set_start_infinite(true);to_return.add_interval(interval);
        //if(denominator_null.get()->numbers().size() > 1){needed_value = denominator_null.get()->numbers().at(1).real();}
        //interval = scls::Interval(needed_value, false, needed_value + 1, true);interval.set_end_infinite(true);to_return.add_interval(interval);

        // Do the redaction
        if(redaction != 0) {
            (*redaction) += "Donc, la forme " + current_function->formula()->to_std_string(settings) + " n'est pas définie pour x appartenant à " + denominator_null.get()->to_std_string(settings) + ". ";
        }

        // Return the result
        fs.reset();
        return to_return;
    }
    scls::Set_Number function_definition_set(Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings) {
        // Create the redaction
        scls::Formula* function_studied = current_function->formula();
        scls::Set_Number to_return = scls::Set_Number::set_real();

        // Do the redaction
        if(redaction != 0) {(*redaction) += "Nous cherchons l'ensemble de définition maximale de la forme " + function_studied->to_std_string(settings) + ". ";}

        if(!function_studied->is_basic()) {
            // Handle functions
            scls::Set_Number current_definition_set = function_studied->applied_function()->definition_set();
            if(redaction != 0) {(*redaction) += "Cette forme est soumise à la fonction \"" + function_studied->applied_function()->name() + "\", définie sur l'ensemble " + current_definition_set.to_std_string(settings) + ". ";}
        }
        else {
            // Fraction part
            std::shared_ptr<scls::__Formula_Base> denominator = function_studied->denominator();
            if(denominator.get() != 0) {to_return = function_definition_set_fraction(denominator.get(), current_function, redaction, settings);}

            // Finish the redaction
            if(redaction != 0) {
                if(to_return == scls::Set_Number::set_real()) {(*redaction) += std::string("Or, cette fonction ne contient aucune forme pouvant amener à une valeur interdite. ");}
                (*redaction) += std::string("Donc, cette fonction est définie sur l'ensemble ") + to_return.to_std_string(settings) + std::string(". ");
            }
        }

        // Return the result
        current_function->set_definition_set(to_return);
        return to_return;
    }

    // Returns the derivation of a function
    scls::Formula function_derivation_monomonial(scls::__Monomonial current_monomonial, std::string* redaction, scls::Textual_Math_Settings* settings) {
        // Do the calculation
        scls::_Base_Unknown* needed_unknown = current_monomonial.contains_unknown("x");
        scls::Formula result;
        if(needed_unknown != 0) {
            // Basic exponential form
            scls::Fraction new_exponent = needed_unknown->exponent().real() - 1;
            if(new_exponent != 0) {result = scls::__Monomonial(needed_unknown->exponent() * current_monomonial.factor(), "x", new_exponent);} else {result = current_monomonial.factor().real();}
        }

        if(redaction == 0) {
            // Write the redaction
            (*redaction) += "La forme dérivée de la forme " + current_monomonial.to_std_string(settings) + " est " + result.to_std_string(settings) + ". ";
        }

        return result;
    }
    scls::Formula function_derivation_polymonial(Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings) {
        // Do the calculation
        scls::Formula result;

        // Write the redaction
        scls::Polymonial needed_polymonial = current_function->formula()->to_polymonial();
        if(redaction != 0) {
            (*redaction) += "Or, cette fonction n'est qu'un simple polymône. ";
            (*redaction) += "Pour étudier sa dérivée, découpons cette fonction en plusieurs monômes, que nous étudierons l'un après l'autre. ";
        }

        // Study each monomonials
        for(int i = 0;i<static_cast<int>(needed_polymonial.monomonials().size());i++) {
            result += function_derivation_monomonial(needed_polymonial.monomonials()[i], redaction, settings);
        }

        // Finish the redaction
        if(redaction != 0) {(*redaction) += "Donc, la forme dérivée de " + needed_polymonial.to_std_string(settings) + " est " + result.to_std_string(settings) + ". ";}

        return result;
    }
    scls::Formula function_derivation_factor(scls::__Formula_Base::Formula_Factor* current_factor, Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings) {
        scls::Formula added_element_derivate;
        for(int i = 0;i<static_cast<int>(current_factor->factors().size());i++) {
            std::shared_ptr<Function_Studied> fs = Function_Studied::new_function_studied_shared_ptr(*current_factor->factors().at(i).get(), current_function);
            added_element_derivate += function_derivation(fs.get(), redaction, settings);
        }

        return added_element_derivate;
    }
    scls::Formula function_derivation_sum(scls::__Formula_Base::Formula_Sum* current_sum, Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings) {
        scls::Formula added_element_derivate;
        for(int i = 0;i<static_cast<int>(current_sum->formulas_add().size());i++) {
            added_element_derivate += function_derivation_factor(current_sum->formulas_add().at(i).get(), current_function, redaction, settings);
        }

        return added_element_derivate;
    }
    scls::Formula function_derivation_fraction(scls::__Formula_Base::Formula_Fraction* current_fraction, Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings) {
        // The function is more complicated
        std::string function_name = current_function->name() + "_";
        scls::Formula numerator = (*current_fraction->numerator());
        if(current_fraction->denominator() != 0 && redaction != 0) {
            (*redaction) += "Premièrement, étudions " + function_name + " tel que " + function_name + "(" + current_function->unknown() + ") = " + numerator.to_std_string(settings) + ". ";
        }
        scls::Formula numerator_derivate = function_derivation_sum(current_fraction->numerator(), current_function, redaction, settings);

        scls::Formula result;
        if(current_fraction->denominator() != 0) {
            // Calculate the denominator
            if(redaction == 0) {
                (*redaction) += "En suite, étudions " + function_name + " tel que " + function_name + "(" + current_function->unknown() + ") = " + current_fraction->denominator()->to_std_string(settings) + ". ";
            }
            scls::Formula denominator_derivate = function_derivation_sum(current_fraction->denominator(), current_function, redaction, settings);

            // Do the division
            scls::__Formula_Base denominator = *current_fraction->denominator();
            result = ((numerator_derivate * denominator) - (numerator * denominator_derivate)) / (denominator * denominator);
            if(redaction != 0) {(*redaction) += std::string("Finalement, appliquons la formule de division de formes dérivées. ");}
        }
        else{result=numerator_derivate;}

        return result;
    }
    scls::Formula function_derivation(Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings) {
        // Do the calculation
        scls::Formula result;

        // Write the redaction
        if(redaction != 0){(*redaction) += "Nous cherchons la dérivée de la fonction " + current_function->name() + ". ";}

        // Check the type of function
        std::shared_ptr<scls::__Formula_Base> denominator = current_function->formula()->denominator();
        if(current_function->formula()->is_simple_polymonial()) {result = function_derivation_polymonial(current_function, redaction, settings);}
        else if(current_function->formula()->applied_function() != 0) {
            // Check the applied function
            if(redaction != 0) {
                (*redaction) += "De plus, nous voyons que la fonction \"" + current_function->formula()->applied_function()->name() + "\" est appliquée sur la forme : prenons la en compte. ";
            }

            // Use the chain rule
            std::shared_ptr<scls::__Formula_Base> base_function = current_function->formula_copy();base_function.get()->clear_applied_function();
            std::shared_ptr<Function_Studied> fs = Function_Studied::new_function_studied_shared_ptr(*base_function.get(), current_function);
            scls::Formula derivated = function_derivation(fs.get(), redaction, settings);
            result = (*current_function->formula()->applied_function()->derivate_value(*current_function->formula()).get()) * derivated;
        }
        else {result = function_derivation_fraction(current_function->formula()->fraction(), current_function, redaction, settings);}

        if(redaction != 0) {
            (*redaction) += "Au final, la dérivée de " + current_function->name() + " est " + current_function->name() + "' tel que :</br></br><math><mi>" + current_function->name() + "'</mi><mi>(" + current_function->unknown() + ")</mi><mo>=</mo>" + result.to_mathml(settings) + "</math></br></br>";
        }
        return result;
    }

    // Returns the image of a function
    scls::__Formula_Base::Formula function_image(Function_Studied* current_function, scls::Formula needed_value, std::string& redaction, scls::Textual_Math_Settings* settings) {
        // Do the ccalculation
        scls::__Formula_Base::Formula result = current_function->formula()->replace_unknown(current_function->unknown(), needed_value);

        // Write the redaction
        redaction += "Nous cherchons la valeur de " + current_function->name() + "(" + needed_value.to_std_string(settings) + "). ";
        redaction += "Pour cela, remplaceons " + current_function->unknown() + " par " + needed_value.to_std_string(settings) + " dans " + current_function->name() + ".</br>";
        redaction += current_function->name() + "(" + needed_value.to_std_string(settings) + ") = " + result.to_std_string(settings) + "</br>";
        redaction += "Donc, " + current_function->name() + "(" + needed_value.to_std_string(settings) + ") = " + result.to_std_string(settings) + ". ";
        return result;
    }

    // Returns the limit of a function / polymonial in + infinity
    scls::Limit polymonial_limit(scls::Polymonial current_monomonial, scls::Limit needed_limit, std::string unknown, std::string& redaction, scls::Textual_Math_Settings* settings) {
        // Check the limit for infinity
        scls::Limit current_limit = current_monomonial.limit(needed_limit, unknown);
        redaction += "Le monôme " + current_monomonial.to_std_string(settings) + " a pour limite " + current_limit.to_std_string(settings) + ". ";
        return current_limit;
    }
    scls::Limit __function_limit_monomonials(Function_Studied* current_function, scls::Polymonial polymonial, scls::Limit needed_limit, std::string& redaction, scls::Textual_Math_Settings* settings) {
        // Cut the formula by monomonial
        redaction += "Comme cette forme est un simple polynôme, étudions les limites de chaque monôme. ";
        std::vector<scls::Limit> limits;
        int monomonial_number = static_cast<int>(polymonial.monomonials().size());
        for(int i = 0;i<monomonial_number;i++) {
            scls::Limit result = polymonial_limit(polymonial.monomonials().at(i), needed_limit, current_function->unknown(), redaction, settings);
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
    scls::Limit function_limit(Function_Studied* current_function, scls::Limit needed_limit, std::string& redaction, scls::Textual_Math_Settings* settings) {
        // Create the redaction
        scls::Formula* function_studied = current_function->formula();
        scls::Limit to_return = scls::Limit();

        // Only one polymonial
        if(function_studied->is_simple_polymonial()) {
            if(function_studied->denominator() != 0) {
                // The limit contains a fraction
                redaction += "Cette forme est une division, nous avons besoin des limites du numérateur et du dénominateur pour avoir sa limite. ";
            }

            // Start the search
            redaction += "Nous cherchons la limite de " + current_function->name() + ", qui peut s'écrire " + function_studied->to_std_string(settings) + ", en " + needed_limit.to_std_string(settings) + ". ";
            scls::Polymonial polymonial = function_studied->to_polymonial();

            // Handle the polymonial monomonial by monomonial
            to_return = __function_limit_monomonials(current_function, polymonial, needed_limit, redaction, settings);

            // Handle possible errors
            if(to_return.is_error_ipi() && current_function->level() <= 1) {
                redaction += "Or, nous avons une forme indéterminée \"infini + ou - infini\". ";
                redaction += "Pour lever l'indétermination, factorisons toute la forme par le monôme du plus haut degré, et calculons sa limite. ";
                scls::Polymonial needed_monomonial = scls::Polymonial(scls::Complex(1), current_function->unknown(), polymonial.degree(current_function->unknown()));
                std::shared_ptr<Function_Studied> needed_function = Function_Studied::new_function_studied_shared_ptr((*function_studied) / needed_monomonial, current_function);
                needed_function.get()->set_name(current_function->name());
                scls::Limit result = function_limit(needed_function.get(), needed_limit, redaction, settings);
                redaction += "Maintenant, calculons la limite de " + needed_monomonial.to_std_string(settings) + ", qui est de +infini. ";
                if(result.value() > 0) {redaction += std::string("Par produit de limites, la limite de f pour ") + current_function->unknown() + " tendant vers " + needed_limit.to_std_string(settings) + " est +infini. ";}
                else {redaction += std::string("Par produit de limites, la limite de f pour ") + current_function->unknown() + " tendant vers " + needed_limit.to_std_string(settings) + " est -infini. ";}
            }
            else {redaction += std::string("Par somme de limites, la limite de ") + current_function->name() + " pour " + current_function->unknown() + " tendant vers " + needed_limit.to_std_string(settings) + " est " + to_return.to_std_string(settings) + ". ";}
        }

        return to_return;
    }

    // Returns the primitive of a function
    scls::Formula function_primitive_monomonial(scls::__Monomonial current_monomonial, std::string* redaction, scls::Textual_Math_Settings* settings) {
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

        if(redaction == 0) {(*redaction) += "La forme primitive de la forme " + current_monomonial.to_std_string(settings) + " est " + result.to_std_string(settings) + ". ";}

        return result;
    }
    scls::Formula function_primitive_polymonial(Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings) {
        // Do the calculation
        scls::Formula result;

        // Write the redaction
        scls::Polymonial needed_polymonial = current_function->formula()->to_polymonial();
        if(redaction != 0) {
            (*redaction) += "Or, cette fonction n'est qu'un simple polymône. ";
            (*redaction) += "Pour étudier sa primitive, découpons cette fonction en plusieurs monômes, que nous étudierons l'un après l'autre. ";
        }

        // Study each monomonials
        for(int i = 0;i<static_cast<int>(needed_polymonial.monomonials().size());i++) {
            result += function_primitive_monomonial(needed_polymonial.monomonials()[i], redaction, settings);
        }

        // Finish the redaction
        if(redaction != 0) {(*redaction) += "Donc, la forme primitive de " + needed_polymonial.to_std_string(settings) + " est " + result.to_std_string(settings) + ". ";}

        return result;
    }
    scls::Formula function_primitive(Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings) {
        // Do the calculation
        scls::Formula result;

        // Write the redaction
        if(redaction != 0){(*redaction) += "Nous cherchons la primitive de la fonction " + current_function->name() + ". ";}

        // Check the type of function
        if(current_function->formula()->is_simple_polymonial()) {result = function_primitive_polymonial(current_function, redaction, settings);}

        return result;
    }

    // Returns the set of roots of a function
    scls::Set_Number function_roots_polymonial(scls::Polymonial polymonial, Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings) {
        int degree = polymonial.degree(current_function->unknown()).real().to_int();
        scls::Set_Number to_return = scls::Set_Number();
        if(polymonial.is_known()) {if(redaction != 0){(*redaction) += "Or, " + polymonial.to_std_string(settings) + " n'est pas égal à 0, cette forme n'a donc pas de racines. ";}}
        else if(degree == 1) {
            // Calculate the known and unknown parts
            scls::Complex known_part = polymonial.known_monomonial().factor();
            scls::Complex unknown_part = polymonial.unknown_monomonials()[0].factor();

            // Create the redaction
            if(redaction != 0) {
                (*redaction) += "Or, cette forme est une forme affine. ";
                (*redaction) += "Pour étudier son unique racine, nous devons étudier les deux parties qui la constitue. ";
                (*redaction) += "La partie connue du polynôme vaut " + known_part.to_std_string_simple(settings) + ". ";
            }

            // Calculate the solution
            known_part *= -1;
            scls::Complex solution = known_part / unknown_part;
            if(redaction != 0) {
                (*redaction) += "Donc, la racine de cette forme est atteinte quand l'équation " + unknown_part.to_std_string_simple(settings) + " * n = " + known_part.to_std_string_simple(settings) + " fonctionne. ";
                (*redaction) += "Or, cette équation est vérifiée pour n = " + solution.to_std_string_simple(settings) + ". ";
                (*redaction) += "Donc, " + polymonial.to_std_string(settings) + " admet pour racine " + solution.to_std_string_simple(settings) + ". ";
            }
            to_return.add_number(solution);
            scls::Interval interval;
        }
        else if(degree == 2) {
            // Calculate the known and unknown parts
            scls::Complex a_part = polymonial.monomonial(current_function->unknown(), 2).factor();
            scls::Complex b_part = polymonial.monomonial(current_function->unknown()).factor();
            scls::Complex c_part = polymonial.known_monomonial().factor();
            scls::Complex discriminant_complex = b_part * b_part - 4 * a_part * c_part;

            // Create the redaction for the discriminant
            if(redaction != 0) {
                (*redaction) += "Or, cette forme est un polynôme de degré 2. ";
                (*redaction) += "Pour étudier ses racines, nous devons étudier les trois parties qui la constitue. ";
                (*redaction) += "Commençons par calculer le discriminant d de cette forme. ";
                (*redaction) += " d = b * b - 4 * a * c = " + discriminant_complex.to_std_string_simple(settings) + ". ";
            }

            // Search the needed roots
            scls::Fraction discriminant = discriminant_complex.real();
            if(discriminant < 0) {if(redaction != 0){(*redaction) += "Or, d &lt; 0, donc cette forme n'a pas de solution dans l'ensemble des réels. ";} }
            else if(discriminant == 0) {
                scls::Fraction solution = ((b_part * -1) / (2 * a_part)).real();
                if(redaction != 0) {
                    (*redaction) += "Or, d = 0, donc cette forme a une solution dans l'ensemble des réels. ";
                    (*redaction) += "Cette solution est n = -b/2a = " + solution.to_std_string(settings) + ". ";
                }
                to_return.add_number(solution);
            }
            else {
                scls::Fraction sqrt_d = scls::Fraction(std::sqrt(discriminant.to_double()) * 1000000.0, 1000000.0);
                scls::Fraction solution_1 = (((b_part * -1) + sqrt_d) / (2 * a_part)).real();
                scls::Fraction solution_2 = (((b_part * -1) - sqrt_d) / (2 * a_part)).real();
                if(redaction != 0) {
                    (*redaction) += "Or, d > 0, donc cette forme a deux solutions dans l'ensemble des réels. ";
                    (*redaction) += "La première solution est x1 = (-b + sqrt(d))/2a, qui est à peu prés égal à " + scls::format_number_to_text(solution_1.to_double()) + ". ";
                    (*redaction) += "La deuxième solution est x2 = (-b - sqrt(d))/2a, qui est à peu prés égal à " + scls::format_number_to_text(solution_2.to_double()) + ". ";
                }
                to_return.add_number(solution_1); to_return.add_number(solution_2);
            }
        }

        return to_return;
    }
    scls::Set_Number function_roots_fraction(scls::Formula::Formula_Fraction* fraction, Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings) {
        // Create the redaction
        if(redaction != 0){(*redaction) += "Or, cette forme est une fonction. Ces racines sont donc les mêmes que celles de sont numérateur. ";}

        // Create the function
        std::shared_ptr<Function_Studied> fs = Function_Studied::new_function_studied_shared_ptr(*fraction->numerator(), current_function);
        fs.get()->set_name(current_function->name() + std::string("_num"));fs.get()->set_function_unknown(current_function->unknown());
        return function_roots(fs.get(), redaction, settings);
    }
    scls::Set_Number function_roots(Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings) {
        // Create the redaction
        std::shared_ptr<scls::Formula> function_studied = current_function->formula_copy();
        scls::Set_Number to_return = scls::Set_Number();
        if(redaction != 0){(*redaction) += "Nous cherchons les racines de " + function_studied.get()->to_std_string(settings) + ". ";}

        // Handle function
        std::shared_ptr<scls::Formula::__Formula_Base_Function> applied_function = function_studied.get()->applied_function_shared_ptr();
        function_studied.get()->clear_applied_function();
        function_studied.get()->check_formula();

        // Select the good study to do
        if(function_studied.get()->is_simple_polymonial()) {to_return = function_roots_polymonial(function_studied.get()->to_polymonial(), current_function, redaction, settings);}
        else if(function_studied.get()->is_simple_fraction()){to_return = function_roots_fraction(function_studied.get()->fraction(), current_function, redaction, settings);}

        current_function->set_roots(to_return);
        return to_return;
    }

    // Returns the set of a positive function
    scls::Set_Number function_sign_polymonial(scls::Polymonial polymonial, Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings) {
        scls::Set_Number to_return = scls::Set_Number();
        if(current_function->roots() == 0){function_roots(current_function, redaction, settings);}
        int roots_number = current_function->roots()->cardinal();
        if(roots_number == 0) {
            // Only one number
            scls::Fraction number = polymonial.replace_unknown(current_function->unknown(), 0).known_monomonial().factor().real();
            if(number < 0) {if(redaction != 0){(*redaction) += std::string("Or, ") + current_function->formula()->to_std_string(settings) + " &lt; 0, cette forme est donc négative sur N.";}}
            else if(number > 0) {
                if(redaction != 0){(*redaction) += std::string("Or, ") + current_function->formula()->to_std_string(settings) + " &gt; 0, cette forme est donc positive sur N.";}

                // Create the needed interval
                scls::Interval interval; interval.set_start_infinite(true); interval.set_end_infinite(true);
                to_return = interval;
            }
            else {if(redaction != 0){(*redaction) += std::string("Or, ") + current_function->formula()->to_std_string(settings) + " = 0, cette forme est donc nulle sur N.";}}
        }
        else {
            // Create the redaction
            if(redaction != 0) {
                if(roots_number == 1) {
                    (*redaction) += "Cette forme a une racine. ";
                    (*redaction) += "Pour étudier son signe total, nous allons étudier une valeur avant et une valeur après cette racine. ";
                }
                else {
                    (*redaction) += "Cette forme a " + std::to_string(roots_number) + " racines. ";
                    (*redaction) += "Pour étudier son signe total, nous allons étudier une valeur avant et une valeur après ces racines. ";
                }
            }

            // Last studied value
            scls::Fraction current_root = current_function->roots()->intervals().at(0).start();
            scls::Fraction last_root = current_function->roots()->intervals().at(0).start();
            scls::Fraction last_number = polymonial.replace_unknown(current_function->unknown(), current_root - 1).known_monomonial().factor().real();

            // Get the set to unite
            if(last_number > 0) {
                to_return = scls::Set_Number::before_strictly(current_root);
                if(redaction != 0) {
                    (*redaction) += "La valeur est positive. ";scls::Set_Number temp = scls::Set_Number::before_strictly(current_root);
                    (*redaction) += std::string("La fonction est donc strictement positive sur ") + temp.to_std_string(settings) + std::string(". ");
                }
            }
            else if(redaction != 0) {
                (*redaction) += "La valeur est négative. ";scls::Set_Number temp = scls::Set_Number::before_strictly(current_root);
                (*redaction) += std::string("La fonction est donc strictement négative sur ") + temp.to_std_string(settings) + std::string(". ");
            }

            // Study each values
            for(int i = 1;i<roots_number;i++) {
                // Only two numbers
                scls::Fraction next_root = current_function->roots()->intervals().at(i).start();
                scls::Fraction current_number = (current_root + next_root) / 2;
                current_number = polymonial.replace_unknown(current_function->unknown(), current_number).known_monomonial().factor().real();

                // Get the set to unite
                if(current_number > 0) {to_return.add_interval(scls::Set_Number::between_strictly(current_root, next_root));}

                // Get the good conclusion
                if(current_number < 0) {
                    if(redaction != 0) {
                        (*redaction) += "La prochaine valeur est négative. ";scls::Interval temp;
                        temp.set_start(current_root);temp.set_start_included(false);temp.set_end(next_root);temp.set_end_included(false);
                        (*redaction) += std::string("La fonction est donc strictement négative sur ") + temp.to_std_string(settings) + std::string(". ");
                    }
                }
                else {
                    if(redaction != 0) {
                        (*redaction) += "La prochaine valeur est positive. ";scls::Interval temp;
                        temp.set_start(current_root);temp.set_start_included(false);temp.set_end(next_root);temp.set_end_included(false);
                        (*redaction) += std::string("La fonction est donc strictement positive sur ") + temp.to_std_string(settings) + std::string(". ");
                    }
                }
                last_root = current_root;current_root = next_root;
                last_number = current_number;
            }

            // Finish the study
            scls::Fraction current_number = polymonial.replace_unknown(current_function->unknown(), current_root + 1).known_monomonial().factor().real();
            if(current_number > 0) {to_return.add_interval(scls::Set_Number::after_strictly(current_root));}

            // Get the good conclusion
            if(current_number < 0) {
                if(redaction != 0) {
                    (*redaction) += "La prochaine valeur est négative. ";scls::Interval temp;
                    temp.set_start(current_root);temp.set_start_included(false);temp.set_end_infinite(true);
                    (*redaction) += std::string("La fonction est donc strictement négative sur ") + temp.to_std_string(settings) + std::string(". ");
                }
            }
            else {
                if(redaction != 0) {
                    (*redaction) += "La prochaine valeur est positive. ";scls::Interval temp;
                    temp.set_start(current_root);temp.set_start_included(false);temp.set_end_infinite(true);
                    (*redaction) += std::string("La fonction est donc strictement positive sur ") + temp.to_std_string(settings) + std::string(". ");
                }
            }
        }

        // Finish the redaction
        if(redaction != 0) {(*redaction) += std::string("</br>Au final, la fonction est strictement positive sur ") + to_return.to_std_string(settings) + std::string(". ");}

        return to_return;
    }
    scls::Set_Number function_sign_fraction(scls::Formula::Formula_Fraction* fraction, Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings) {
        // Create the redaction
        scls::Set_Number to_return = scls::Set_Number();
        if(redaction != 0){(*redaction) += std::string("Or, la fonction ") + current_function->name() + " est constitué d'une fraction, nous devons donc analyser son numérateur et son dénominateur. "; }

        // Numerator
        std::shared_ptr<Function_Studied> fs = Function_Studied::new_function_studied_shared_ptr(*fraction->numerator(), current_function);
        fs.get()->set_name(current_function->name() + std::string("_num"));fs.get()->set_function_unknown(current_function->unknown());
        if(redaction == 0){std::string temp=std::string();to_return = function_sign(fs.get(), &temp, settings);}
        else{to_return = function_sign(fs.get(), redaction, settings);}
        // Denominator
        fs = Function_Studied::new_function_studied_shared_ptr(*fraction->denominator(), current_function);
        fs.get()->set_name(current_function->name() + std::string("_den"));fs.get()->set_function_unknown(current_function->unknown());
        if(redaction == 0){std::string temp=std::string();to_return = function_sign(fs.get(), &temp, settings).complement_relative_symetrical(&to_return);}
        else{to_return = function_sign(fs.get(), redaction, settings).complement_relative_symetrical(&to_return);}

        return to_return;
    }
    scls::Set_Number function_sign(Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings) {
        // Create the redaction
        std::shared_ptr<scls::Formula> function_studied = current_function->formula_copy();
        scls::Set_Number to_return = scls::Set_Number();
        if(redaction != 0){(*redaction) += "Nous cherchons le signe de " + current_function->name() + ", qui peut s'écrire " + function_studied->to_std_string(settings) + ". "; }

        // Handle function
        function_studied.get()->check_formula();
        std::shared_ptr<scls::__Formula_Base::__Formula_Base_Function> used_function = function_studied.get()->applied_function_shared_ptr();
        function_studied.get()->clear_applied_function();

        // Only one polymonial
        if(function_studied->is_simple_polymonial()) {to_return = function_sign_polymonial(function_studied->to_polymonial(), current_function, redaction, settings);}
        else if(function_studied->is_simple_fraction()) {to_return = function_sign_fraction(function_studied->fraction(), current_function, redaction, settings);}

        current_function->set_sign_set(to_return);
        return to_return;
    }
    // Returns a text for a sign table of a function
    std::string function_sign_table(Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings) {
        Function_Table table;scls::Formula* formula = current_function->formula();

        // If the function is a fraction
        if(formula->is_simple_fraction()) {
            std::shared_ptr<Function_Studied> needed_function = Function_Studied::new_function_studied_shared_ptr(*formula->numerator().get());
            function_definition_set(needed_function.get(), 0, 0);function_sign(needed_function.get(), 0, 0);
            table.add_function(needed_function.get());
            if(formula->has_denominator()) {
                needed_function = Function_Studied::new_function_studied_shared_ptr(*formula->denominator().get());
                function_definition_set(needed_function.get(), 0, 0);function_sign(needed_function.get(), 0, 0);
                table.add_function(needed_function.get());
            }
        }

        table.add_function(current_function);
        return table.to_xml(redaction, settings);
    }

    //******************
    //
    // Graphic handling
    //
    //******************

    // Annoying functions to draw the image
    int Graphic::Graphic_Base_Object::graphic_x_to_pixel_x(double x){return std::ceil((x - graphic_base()->a_middle_x.to_double()) * graphic_base()->a_pixel_by_case_x + (static_cast<double>(graphic_base()->a_width_in_pixel) / 2.0));};
    int Graphic::Graphic_Base_Object::graphic_x_to_pixel_x(scls::Fraction x){return std::ceil(((x - graphic_base()->a_middle_x) * graphic_base()->a_pixel_by_case_x + scls::Fraction(graphic_base()->a_width_in_pixel, 2)).to_double());};
    int Graphic::Graphic_Base_Object::graphic_y_to_pixel_y(double y){return std::ceil((y - graphic_base()->a_middle_y.to_double()) * graphic_base()->a_pixel_by_case_y) + (graphic_base()->a_height_in_pixel / 2.0);};
    int Graphic::Graphic_Base_Object::graphic_y_to_pixel_y(scls::Fraction y){return std::ceil(((y - graphic_base()->a_middle_y) * graphic_base()->a_pixel_by_case_y + scls::Fraction(graphic_base()->a_height_in_pixel, 2)).to_double());};
    int Graphic::Graphic_Base_Object::graphic_y_to_pixel_y_inversed(double y){return graphic_base()->a_height_in_pixel - graphic_y_to_pixel_y(y);};
    int Graphic::Graphic_Base_Object::graphic_y_to_pixel_y_inversed(scls::Fraction y){return graphic_base()->a_height_in_pixel - graphic_y_to_pixel_y(y);};
    scls::Fraction Graphic::Graphic_Base_Object::pixel_x_to_graphic_x(int x){return graphic_base()->a_middle_x + ((scls::Fraction(x) - scls::Fraction(graphic_base()->a_width_in_pixel, 2)) / scls::Fraction::from_double(graphic_base()->a_pixel_by_case_x));}
    scls::Fraction Graphic::Graphic_Base_Object::pixel_y_to_graphic_y(int y){return graphic_base()->a_middle_y + ((scls::Fraction(graphic_base()->a_height_in_pixel, 2) - scls::Fraction(y)) / scls::Fraction::from_double(graphic_base()->a_pixel_by_case_y));}

    // Returns the datas set to an image
    std::shared_ptr<scls::Image> Graphic::Datas_Set::to_image() {
        if(type() == Graphic::Datas_Set::Datas_Set_Type::DST_Histogram){return to_image_histogram();}
        else if(type() == Graphic::Datas_Set::Datas_Set_Type::DST_Point_Cloud || type() == Graphic::Datas_Set::Datas_Set_Type::DST_Point_Cloud_Linked){return to_image_point_cloud();}
        return std::shared_ptr<scls::Image>();
    }
    std::shared_ptr<scls::Image> Graphic::Datas_Set::to_image_histogram() {
        // Create the image
        std::shared_ptr<scls::Image> to_return = std::make_shared<scls::Image>(width_in_pixel(), height_in_pixel(), scls::Color(255, 255, 255));

        // Get the needed datas
        std::vector<scls::Fraction> all_values;std::vector<int> all_values_number;
        int max_number = 0;scls::sort_fractions(a_datas);
        for(int i = 0;i<static_cast<int>(a_datas.size());i++) {
            int j = 0;for(;j<static_cast<int>(all_values.size());j++) {if(all_values.at(j) == a_datas.at(i)){break;}}
            if(j >= static_cast<int>(all_values.size())){all_values.push_back(a_datas.at(i));all_values_number.push_back(1);}
            else{all_values_number[j]++;}
            if(all_values_number.at(j) > max_number){max_number = all_values_number.at(j);}
        }

        // Draw each part
        scls::Color bar_color = scls::Color(255, 0, 0);
        std::vector<long long int> all_width = scls::partition_number(to_return.get()->height(), all_values.size());
        int current_x = 0;
        for(int i = 0;i<static_cast<int>(all_values.size());i++) {
            double proportion = static_cast<double>(all_values_number.at(i)) / static_cast<double>(max_number);

            // Draw the bar
            int current_height = to_return.get()->height() * proportion;
            int current_y = to_return.get()->height() - current_height;
            to_return.get()->fill_rect(current_x, current_y, all_width.at(i), current_height, bar_color);
            current_x += all_width.at(i);
        }

        return to_return;
    }
    std::shared_ptr<scls::Image> Graphic::Datas_Set::to_image_point_cloud() {
        // Create the image
        std::shared_ptr<scls::Image> to_return = std::make_shared<scls::Image>(width_in_pixel(), height_in_pixel(), scls::Color(255, 255, 255, 0));

        // Get the needed datas
        std::vector<scls::Fraction> all_values;std::vector<scls::Fraction> all_values_number;
        std::vector<scls::Fraction> all_values_average = std::vector<scls::Fraction>(a_datas.size(), 0);
        std::vector<scls::Fraction>* final_values = 0;
        scls::Fraction max_number = 0;scls::Fraction min_number = 0;
        scls::Fraction sum_ponderations = 0;scls::Fraction sum_values = 0;
        if(a_display_type == Graphic::Datas_Set::Datas_Set_Display_Type::DSDT_Value){scls::sort_fractions(a_datas);}
        for(int i = 0;i<static_cast<int>(a_datas.size());i++) {
            // Get the value
            int j = 0;for(;j<static_cast<int>(all_values.size());j++) {if(all_values.at(j) == a_datas.at(i)){break;}}
            if(j >= static_cast<int>(all_values.size())){all_values.push_back(a_datas.at(i));all_values_number.push_back(1);}
            else{all_values_number[j]+=1;}
            sum_ponderations += 1;sum_values += a_datas.at(i);

            // Calculate an average if needed and check the max value
            if(a_display_type == Graphic::Datas_Set::Datas_Set_Display_Type::DSDT_Average) {all_values_average[i] = sum_values / sum_ponderations;if(all_values_average.at(i) > max_number){max_number = all_values_average.at(i);}}
            else {if(all_values_number.at(j) > max_number){max_number = all_values_number.at(j);}}
        }

        // Set the final values
        if(a_fixed_max_used){max_number = a_fixed_max;}if(a_fixed_min_used){min_number = a_fixed_min;}
        if(a_display_type == Graphic::Datas_Set::Datas_Set_Display_Type::DSDT_Average) {final_values = &all_values_average;}
        else {final_values = &all_values_number;}

        // Draw each part
        scls::Color bar_color = scls::Color(255, 0, 0);
        std::vector<long long int> all_width = scls::partition_number(to_return.get()->height(), final_values->size());
        int current_x = 0;int line_width = 2;int point_width = 5;
        int last_x = 0;int last_y = 0;
        for(int i = 0;i<static_cast<int>(final_values->size())&&i<static_cast<int>(all_width.size())+1;i++) {
            double proportion = (final_values->at(i) - min_number).to_double() / (max_number - min_number).to_double();

            // Draw the bar
            int current_y = (to_return.get()->height() - to_return.get()->height() * proportion) - point_width / 2;
            if(i > 0){to_return.get()->draw_line((current_x + point_width / 2) - line_width / 2, (current_y + point_width / 2) - line_width / 2, last_x, last_y, bar_color, line_width);}
            to_return.get()->fill_rect(current_x, current_y, point_width, point_width, bar_color);
            last_x = (current_x + point_width / 2) - line_width / 2;last_y = (current_y + point_width / 2) - line_width / 2;
            if(i < static_cast<int>(all_width.size())){current_x += all_width.at(i);}
        }

        return to_return;
    }

    // Graphic constructor
    Graphic::Graphic_Function::Graphic_Function(std::shared_ptr<Function_Studied> function_studied):a_function_studied(function_studied){}

    // Adds a function to the graphic
    std::shared_ptr<Graphic::Graphic_Function> Graphic::add_function(std::shared_ptr<Function_Studied> function_studied) {std::shared_ptr<Graphic_Function> new_function = std::make_shared<Graphic_Function>(function_studied);a_functions.push_back(new_function);a_graphic_base.get()->a_function_number++;return new_function;}

    // Draws a datas set on the graphic
    void Graphic::draw_datas_set(Datas_Set* needed_datas_set, std::shared_ptr<scls::Image> to_return) {
        std::shared_ptr<scls::Image> needed_image = needed_datas_set->to_image();
        double needed_x = graphic_x_to_pixel_x(needed_datas_set->x().to_double(), to_return);
        double needed_y = graphic_y_to_pixel_y_inversed(needed_datas_set->y().to_double(), to_return);
        to_return.get()->paste(needed_image.get(), needed_x - needed_image.get()->width() / 2, needed_y - needed_image.get()->height() / 2);
    }

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
        // Get the needed coordinates
        scls::Fraction needed_x_end = 0;if(needed_point->x_end() != 0){needed_x_end = needed_point->x_end()->to_polymonial().known_monomonial().factor().real();}
        needed_x_end = graphic_x_to_pixel_x(needed_x_end, to_return);
        scls::Fraction needed_y_end = 0;if(needed_point->y_end() != 0){needed_y_end = needed_point->y_end()->to_polymonial().known_monomonial().factor().real();}
        needed_y_end = graphic_y_to_pixel_y_inversed(needed_y_end, to_return);
        scls::Fraction needed_x_start = graphic_x_to_pixel_x(needed_point->x()->to_polymonial().known_monomonial().factor().real(), to_return);
        scls::Fraction needed_y_start = graphic_y_to_pixel_y_inversed(needed_point->y()->to_polymonial().known_monomonial().factor().real(), to_return);
        // Apply the drawing proportion
        double hat_position_in_purcentage = 1;double hat_size_in_purcentage = 0.2;
        if(needed_point->drawing_proportion() != 1) {
            scls::Fraction x_diff = needed_x_end - needed_x_start;
            scls::Fraction y_diff = needed_y_end - needed_y_start;
            needed_x_end = needed_x_start + x_diff * needed_point->drawing_proportion();
            needed_y_end = needed_y_start + y_diff * needed_point->drawing_proportion();
            hat_position_in_purcentage=scls::vector_2d_angle(x_diff.to_double(), y_diff.to_double()) * (180.0/SCLS_PI);
            hat_size_in_purcentage = 20;
        }

        // Get the graphic values
        scls::Color needed_color = needed_point->color();
        double needed_width = 2;

        // Draw the vector
        if(needed_point->type() == Vector_Type::VT_Arrow) {to_return.get()->draw_arrow(needed_x_start.to_double(), needed_y_start.to_double(), needed_x_end.to_double(), needed_y_end.to_double(), needed_color, hat_position_in_purcentage, hat_size_in_purcentage, needed_width);}
        else if(needed_point->type() == Vector_Type::VT_Vector) {to_return.get()->draw_line(needed_x_start.to_double(), needed_y_start.to_double(), needed_x_end.to_double(), needed_y_end.to_double(), needed_color, needed_width);}
        else {needed_width=8;to_return.get()->fill_rect(needed_x_start.to_double() - needed_width / 2.0, needed_y_start.to_double()  - needed_width / 2.0, needed_width, needed_width, needed_color);}
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
        std::string needed_glsl = needed_formula.to_polymonial().to_glsl(0);
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
                to_return.get()->fill_rect(j - width / 2.0, needed_y, width, needed_height + width, needed_function.get()->color());
            }
        }
    }
    // Returns the image of the graphic
    int Graphic::graphic_x_to_pixel_x(double x, int image_width){return std::ceil((x - middle_x().to_double()) * pixel_by_case_x() + (static_cast<double>(image_width) / 2.0));};
    int Graphic::graphic_x_to_pixel_x(scls::Fraction x, int image_width){return std::ceil(((x - middle_x()) * pixel_by_case_x() + scls::Fraction(image_width, 2)).to_double());};
    int Graphic::graphic_y_to_pixel_y(double y, int needed_height){return std::ceil((y - middle_y().to_double()) * pixel_by_case_y()) + (needed_height / 2.0);};
    int Graphic::graphic_y_to_pixel_y(scls::Fraction y, int needed_height){return std::ceil(((y - middle_y()) * pixel_by_case_y() + scls::Fraction(needed_height, 2)).to_double());};
    int Graphic::graphic_y_to_pixel_y_inversed(double y, int needed_height){return needed_height - graphic_y_to_pixel_y(y, needed_height);};
    int Graphic::graphic_y_to_pixel_y_inversed(scls::Fraction y, int needed_height){return needed_height - graphic_y_to_pixel_y(y, needed_height);};
    scls::Fraction Graphic::pixel_x_to_graphic_x(int x, int image_width){return middle_x() + ((scls::Fraction(x) - scls::Fraction(image_width, 2)) / scls::Fraction::from_double(pixel_by_case_x()));}
    scls::Fraction Graphic::pixel_y_to_graphic_y(int y, int needed_height){return middle_y() + ((scls::Fraction(needed_height, 2) - scls::Fraction(y)) / scls::Fraction::from_double(pixel_by_case_y()));}
    std::shared_ptr<scls::Image> Graphic::to_image(int width_in_pixel, int height_in_pixel) {
        // Create the image
        std::shared_ptr<scls::Image> to_return = std::make_shared<scls::Image>(width_in_pixel, height_in_pixel, style()->background_color());
        a_graphic_base.get()->a_height_in_pixel = height_in_pixel;
        a_graphic_base.get()->a_width_in_pixel = width_in_pixel;

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
            double radius = current_circle.get()->radius().to_double();
            radius = radius * pixel_by_case_x();
            double needed_x = graphic_x_to_pixel_x(center.x()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
            double needed_y = graphic_y_to_pixel_y_inversed(center.y()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
            to_return.get()->fill_circle(needed_x, needed_y, radius, current_circle.get()->angle_start().to_double(), current_circle.get()->angle_end().to_double(), current_circle.get()->color(), current_circle.get()->border_radius(), current_circle.get()->border_color());
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
        for(int i = 0;i<static_cast<int>(a_vectors.size());i++) {draw_vector(a_vectors.at(i).get(), to_return);}

        // Draw the datas set
        for(int i = 0;i<static_cast<int>(a_datas_sets.size());i++) {draw_datas_set(a_datas_sets.at(i).get(), to_return);}

        // Draw the border
        to_return.get()->draw_border(style()->border_top_width(), style()->border_left_width(), style()->border_bottom_width(), style()->border_right_width(), style()->border_color);

        return to_return;
    }

    //******************
    //
    // Physic handling
    //
    //******************

    // Checks if a collision occurs with an another collision
    Collision_Rect_Rect __check_collision_circle_circle(Graphic::Graphic_Collision* collision_1, Graphic::Graphic_Collision* collision_2, Graphic::Graphic_Physic* dynamic_object_1){
        // Get the distance
        scls::Point_2D position_1 = collision_1->position_next();scls::Point_2D position_2 = collision_2->position_next();
        double distance = position_1.distance(position_2);
        scls::Fraction max_distance = (collision_1->attached_transform()->scale_x() / 2 + collision_2->attached_transform()->scale_x() / 2).abs();
        scls::Fraction min_distance = (collision_1->attached_transform()->scale_x() / 2 - collision_2->attached_transform()->scale_x() / 2).abs();
        if(distance > max_distance.abs() || distance < min_distance){return Collision_Rect_Rect();}

        // Set collision_1 as the biggest collision
        if(collision_1->attached_transform()->scale_x() < collision_2->attached_transform()->scale_x()){Graphic::Graphic_Collision* temp = collision_1;collision_1 = collision_2;collision_2=temp;}

        // Get the datas about the collision
        Collision_Rect_Rect to_return;
        to_return.happens = true;
        scls::Point_2D position_from_1 = (position_1 - position_2).normalized();
        scls::Point_2D position_from_2 = (position_2 - position_1).normalized();
        if(distance < collision_1->attached_transform()->scale_x() / 2){position_from_2 *= -1;}
        double kept_proportion = 1.0;
        scls::Point_2D velocity_2 = collision_2->attached_transform()->velocity();
        double angle_tangent_1 = scls::vector_2d_angle(position_from_1);
        double needed_angle = scls::vector_2d_angle(velocity_2) - angle_tangent_1;
        scls::Fraction multiplier = scls::Fraction::from_double(velocity_2.norm());
        scls::Point_2D new_velocity = (position_from_1 - (velocity_2 - position_from_1)).normalized(); // new_velocity = position_from_1.rotated(angle_tangent_1 - needed_angle).normalized() * multiplier
        new_velocity += position_from_1.rotated(angle_tangent_1 - needed_angle).normalized();
        if(distance < collision_1->attached_transform()->scale_x() / 2){multiplier *= -1;}
        new_velocity.normalized();new_velocity *= multiplier * 0.8;
        std::cout << "U " << angle_tangent_1 << " " << needed_angle << " " << multiplier.to_double() << std::endl;
        dynamic_object_1->set_velocity(new_velocity);

        // Return the result
        return to_return;
    }
    Collision_Rect_Rect __check_collision_rect_line(Graphic::Graphic_Collision* collision_rect, Graphic::Graphic_Collision* collision_line, Graphic::Graphic_Physic* dynamic_object_1){
        // Check bottom collision
        scls::Crossing_Datas_Segment datas_bottom = scls::check_crossing_segment(collision_rect->min_x().to_double(), collision_rect->min_y().to_double(), collision_rect->max_x().to_double(), collision_rect->min_y().to_double(), collision_line->x_1().to_double(), collision_line->y_1().to_double(), collision_line->x_2().to_double(), collision_line->y_2().to_double());

        // Check right collision
        scls::Crossing_Datas_Segment datas_right = scls::check_crossing_segment(collision_rect->max_x().to_double(), collision_rect->max_y().to_double(), collision_rect->max_x().to_double(), collision_rect->min_y().to_double(), collision_line->x_1().to_double(), collision_line->y_1().to_double(), collision_line->x_2().to_double(), collision_line->y_2().to_double());

        // Assert
        if(!datas_bottom.crossed_in_segment && !datas_right.crossed_in_segment){return Collision_Rect_Rect();}

        // Get the differences
        scls::Fraction slope_x = (collision_line->x_2() - collision_line->x_1()).abs();
        scls::Fraction slope_y = (collision_line->y_2() - collision_line->y_1()).abs();
        scls::Fraction temp = 0;scls::normalize_3d(slope_x, temp, slope_y);
        Collision_Rect_Rect to_return;
        to_return.happens = true;
        if(datas_bottom.crossed_in_segment){to_return.side_bottom = true;}
        if(datas_right.crossed_in_segment){to_return.side_right = true;}

        // Check the movement
        if(to_return.side_bottom && to_return.side_right) {
            scls::Fraction force_distribution = scls::Fraction(1, 2);
            std::cout << slope_x << " " << slope_y << " " << dynamic_object_1->velocity_x() << " " << dynamic_object_1->velocity_y() << std::endl;

            // Handle next movement
            if(dynamic_object_1->next_movement_x() > 0 && dynamic_object_1->next_movement_y() < 0){
                // Handle both axis at once
                dynamic_object_1->accelerate_x(dynamic_object_1->velocity_x() * slope_y * force_distribution * -1);
                dynamic_object_1->accelerate_y(dynamic_object_1->velocity_x() * slope_y * force_distribution);
            }
            else {
                if(dynamic_object_1->next_movement_x() > 0){dynamic_object_1->accelerate_y(dynamic_object_1->velocity_x() * force_distribution);dynamic_object_1->remove_x_velocity();}
                if(dynamic_object_1->next_movement_y() < 0){dynamic_object_1->accelerate_x(dynamic_object_1->velocity_y() * force_distribution);dynamic_object_1->remove_y_velocity();}
            }
        }
        if(to_return.side_bottom && !to_return.side_right) {if(dynamic_object_1->velocity().y() < 0){dynamic_object_1->remove_y_velocity();}}
        if(to_return.side_left) {if(dynamic_object_1->velocity().x() < 0){dynamic_object_1->remove_x_velocity();}}
        if(to_return.side_right && !to_return.side_bottom) {if(dynamic_object_1->velocity().x() > 0){dynamic_object_1->remove_x_velocity();}}
        if(to_return.side_top) {if(dynamic_object_1->velocity().y() > 0){dynamic_object_1->remove_y_velocity();}}

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
            if(dynamic_object_1->velocity_y() < 0){dynamic_object_1->accelerate_x(dynamic_object_1->velocity_x() * scls::Fraction(-1, 100));}

            // Contact
            if(dynamic_object_1->velocity_y() < 0){dynamic_object_1->set_velocity_y(scls::Fraction::from_double(to_return.distance));}
        }
        if(to_return.side_left) {if(dynamic_object_1->next_movement_x() < 0){dynamic_object_1->set_velocity_x(scls::Fraction::from_double(to_return.distance));}}
        if(to_return.side_right) {if(dynamic_object_1->next_movement_x() > 0){dynamic_object_1->set_velocity_x(scls::Fraction::from_double(-to_return.distance));}}
        if(to_return.side_top) {if(dynamic_object_1->velocity_y() > 0){dynamic_object_1->set_velocity_y(scls::Fraction::from_double(-to_return.distance));}}

        // Return the result
        return to_return;
    };
    Collision_Rect_Rect __check_collision(Graphic::Graphic_Collision* collision_1, Graphic::Graphic_Collision* collision_2, Graphic::Graphic_Physic* dynamic_object_1) {
        // Asserts
        if(collision_2 == 0 || collision_2->attached_transform() == collision_1->attached_transform()){return Collision_Rect_Rect();}

        // Both objects are rect
        if(collision_1->type() == Graphic::Graphic_Collision_Type::GCT_Rect && collision_2->type() == Graphic::Graphic_Collision_Type::GCT_Rect) {
            Collision_Rect_Rect current_result = __check_collision_rect_rect(collision_1, collision_2, dynamic_object_1);
            if(current_result.happens) {return current_result;}
        }
        else if(collision_1->type() == Graphic::Graphic_Collision_Type::GCT_Rect && collision_2->type() == Graphic::Graphic_Collision_Type::GCT_Line) {
            Collision_Rect_Rect current_result = __check_collision_rect_line(collision_1, collision_2, dynamic_object_1);
            if(current_result.happens) {return current_result;}
        }
        else if(collision_1->type() == Graphic::Graphic_Collision_Type::GCT_Circle && collision_2->type() == Graphic::Graphic_Collision_Type::GCT_Circle) {
            Collision_Rect_Rect current_result = __check_collision_circle_circle(collision_1, collision_2, dynamic_object_1);
            if(current_result.happens) {return current_result;}
        }

        return Collision_Rect_Rect();
    }

    // Add a line / rect collision to the graphic object
    void Graphic::Graphic_Physic::add_collision(std::shared_ptr<Graphic_Collision> collision){a_collisions.push_back(collision);};
    void Graphic::Graphic_Physic::add_collision(scls::Fraction x_1, scls::Fraction y_1, scls::Fraction x_2, scls::Fraction y_2){
        a_collisions.push_back(std::make_shared<Graphic_Collision>(a_attached_transform));
        std::shared_ptr<Graphic_Collision> collision=a_collisions.at(a_collisions.size()-1);
        collision.get()->set_x_1(x_1);collision.get()->set_y_1(y_1);
        collision.get()->set_x_2(x_2);collision.get()->set_y_2(y_2);
        collision.get()->set_type(Graphic_Collision_Type::GCT_Line);
    };

    // Checks if a collision occurs with an another object
    void Graphic::Graphic_Physic::check_collision(Graphic::Graphic_Collision* collision) {
        // Asserts
        if(collision == 0 || collision->attached_transform() == attached_transform()){return;}

        // Check each collision
        for(int i = 0;i<static_cast<int>(a_collisions.size());i++) {
            Collision_Rect_Rect current_result = __check_collision(a_collisions[i].get(), collision, this);
            if(current_result.happens){a_current_collisions_results.push_back(current_result);}
        }
    }

    // Returns a new a collision to the graphic object
    std::shared_ptr<Graphic::Graphic_Collision> Graphic::Graphic_Physic::new_collision(Graphic_Collision_Type type){
        std::shared_ptr<Graphic::Graphic_Collision>to_return=std::make_shared<Graphic::Graphic_Collision>(a_attached_transform);
        to_return.get()->set_type(type);add_collision(to_return);
        to_return.get()->set_height(a_attached_transform.lock().get()->scale_y());
        to_return.get()->set_width(a_attached_transform.lock().get()->scale_x());
        return to_return;
    };

    // Soft resets the object
    void Graphic::Graphic_Physic::soft_reset(){a_current_collisions_results.clear();};

    // Loads 100 X 100 physic map
    void Graphic::load_physic_map(int middle_loading_x, int middle_loading_y) {
        // Get the needed datas
        int height = 100;int width = 100;
        a_physic_map_start_x = middle_loading_x - width / 2;
        a_physic_map_start_y = middle_loading_y - height / 2;

        // Create the cases
        a_physic_map = std::vector<std::vector<std::shared_ptr<Physic_Case>>>(width, std::vector<std::shared_ptr<Physic_Case>>(height, std::make_shared<Physic_Case>()));
    }

    // Graphic constructor
    Graphic_Object::Graphic_Object(scls::_Window_Advanced_Struct& window, std::string name, std::weak_ptr<scls::GUI_Object> parent):scls::GUI_Object(window, name, parent){update_texture();}

    // Render the object
    void Graphic_Object::render(glm::vec3 scale_multiplier) {scls::GUI_Object::render(scale_multiplier);}

    // Scale the GUI Object
    void Graphic_Object::Graphic_GUI_Object::scale(Graphic* graphic, int image_width, int image_height){
        // Get the needed values
        int new_height = std::ceil(graphic->pixel_by_case_y()) * height().to_double();
        int new_x = graphic->graphic_x_to_pixel_x((x() - width() / 2).to_double(), image_width);
        int new_y = graphic->graphic_y_to_pixel_y((y() - height() / 2).to_double(), image_height);
        int new_width = std::ceil(graphic->pixel_by_case_x()) * width().to_double();

        // Check if the object have to be rendered or not
        if(new_x > image_width || new_y > image_height || new_x < -new_width || new_y < -new_height){a_object.get()->set_visible(false);return;}
        else{a_object.get()->set_visible(true);}

        // Set the values
        a_object.get()->set_height_in_pixel(new_height);
        a_object.get()->set_width_in_pixel(new_width);
        a_object.get()->set_x_in_pixel(new_x);
        a_object.get()->set_y_in_pixel(new_y);
        a_object.get()->set_texture_scale_x(width().to_double());
        a_object.get()->set_texture_scale_y(height().to_double());
    };

    // Updates the object
    int __graphic_object_render_number = 0;
    void Graphic_Object::update_event() {
        GUI_Object::update_event();

        // Update the object
        for(int i = 0;i<static_cast<int>(a_gui_objects.size());i++) {
            if(a_gui_objects[i].get() != 0) {
                a_gui_objects[i].get()->update_event();
            }
        }

        // Update the physic
        for(int i = 0;i<static_cast<int>(a_gui_objects.size());i++){a_gui_objects.at(i).get()->reset_physic();}
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
    scls::Point_2D gravity = scls::Point_2D(0, scls::Fraction(-98, 10));
    int Graphic_Object::update_physic() {
        // Realised updates
        int needed_update = 0;

        // Apply gravity
        for(int i = 0;i<static_cast<int>(graphic()->physic_objects().size());i++) {if(graphic()->physic_objects().at(i).get()->use_gravity()){graphic()->physic_objects().at(i).get()->accelerate(gravity * scls::Fraction::from_double(window_struct().delta_time()));needed_update++;}}

        // Update raw velocity
        for(int i = 0;i<static_cast<int>(graphic()->physic_objects().size());i++) {graphic()->physic_objects().at(i).get()->update_raw_velocity();}

        // Update each objects in the case
        for(int i = 0;i<static_cast<int>(graphic()->physic_objects().size());i++) {
            if(!graphic()->physic_objects().at(i)->is_static()) {
                // Get the needed datas
                int needed_height = graphic()->physic_objects().at(i)->max_y_next().to_double_ceil() - graphic()->physic_objects().at(i)->min_y_next().to_double_floor();
                int needed_width = graphic()->physic_objects().at(i)->max_x_next().to_double_ceil() - graphic()->physic_objects().at(i)->min_x_next().to_double_floor();
                int x_start = graphic()->physic_objects().at(i)->min_x_next().to_double_floor();
                int y_start = graphic()->physic_objects().at(i)->min_y_next().to_double_floor();

                // Check the cases
                for(int j = 0;j<needed_width;j++) {
                    for(int h = 0;h<needed_height;h++) {
                        Graphic::Physic_Case* current_case = physic_case(x_start + j, y_start + h);
                        if(current_case != 0 && current_case->static_objects_collisions.size() > 0){
                            for(int h = 0;h<static_cast<int>(current_case->static_objects_collisions.size());h++) {
                                graphic()->physic_objects().at(i)->check_collision(current_case->static_objects_collisions[h].lock().get());
                            }
                        }
                    }
                }
            }
            else if(graphic()->physic_objects().at(i)->moved_during_this_frame()) {
                // Delete the last cases
                for(int j = 0;j<static_cast<int>(graphic()->physic_objects().at(i)->used_physic_case().size());j++) {
                    for(int k = 0;k<static_cast<int>(graphic()->physic_objects().at(i)->collisions().size());k++) {
                        graphic()->physic_objects().at(i)->used_physic_case()[j]->delete_static_object_collision(graphic()->physic_objects().at(i)->collisions()[k].get());
                    }
                }

                // Get the needed datas
                int needed_height = graphic()->physic_objects().at(i)->max_y_next().to_double_ceil() - graphic()->physic_objects().at(i)->min_y_next().to_double_floor();
                int needed_width = graphic()->physic_objects().at(i)->max_x_next().to_double_ceil() - graphic()->physic_objects().at(i)->min_x_next().to_double_floor();
                int x_start = graphic()->physic_objects().at(i)->min_x_next().to_double_floor();
                int y_start = graphic()->physic_objects().at(i)->min_y_next().to_double_floor();

                // Add the cases
                for(int j = 0;j<needed_width;j++) {
                    for(int h = 0;h<needed_height;h++) {
                        Graphic::Physic_Case* current_case = physic_case(x_start + j, y_start + h);
                        if(current_case != 0){
                            for(int l = 0;l<static_cast<int>(graphic()->physic_objects().at(i)->collisions().size());l++){
                                current_case->static_objects_collisions.push_back(graphic()->physic_objects().at(i)->collisions()[l]);
                            }
                            graphic()->physic_objects().at(i)->used_physic_case().push_back(current_case);
                        }
                    }
                }
            }
        }

        // Apply next movement
        for(int i = 0;i<static_cast<int>(graphic()->physic_objects().size());i++) {
            graphic()->physic_objects().at(i).get()->__move();needed_update++;
        }

        return needed_update;
    }

    // Update the texture of the object
    void Graphic_Object::update_texture(){
        // Set the good image
        int needed_width = width_in_pixel();int needed_height = height_in_pixel();
        if(use_image()) {
            std::shared_ptr<scls::Image> needed_image = a_datas.get()->to_image(needed_width, needed_height);
            texture()->set_image(needed_image);
        }
        __graphic_object_render_number++;

        // Update the objects
        for(int i = 0;i<static_cast<int>(a_gui_objects.size());i++) {
            Graphic_Object::Graphic_GUI_Object* current = a_gui_objects.at(i).get();
            current->scale(a_datas.get(), needed_width, needed_height);
        }

        // Update the parent
        set_should_render_during_this_frame(true);
    };
}
