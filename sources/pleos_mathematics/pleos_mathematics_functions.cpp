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
}
