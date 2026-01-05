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

	// Gets the antiderivated polynomial of a polynomial
	std::string __redaction_polynomial_antiderivation_0 = std::string("Cette forme est un simple polynôme, on peut obtenir facilement sa primitive grâce aux propriétés des polynômes.");
	std::string __redaction_polynomial_antiderivation_final = std::string("Donc, une primitive de \"<full_formula>\" est \"<derivated>\".");
	scls::Polynomial polynomial_antiderivation(Function_Studied* function, std::string* redaction) {
    	scls::__Formula* formula = function->formula();
		scls::Polynomial polynomial = *formula->polynomial();
		scls::Textual_Math_Settings settings;settings.set_hide_if_0(false);
		std::string unknown_name = std::string("x");

		// Set the good redaction
		if(redaction != 0) {
			(*redaction) += __redaction_polynomial_antiderivation_0;
		}

		// Get the derivated polynomial
		scls::Polynomial polynomial_antiderivated = scls::Polynomial();
		for(int i = 0;i<static_cast<int>(polynomial.monomonials().size());i++) {
			scls::__Monomonial* current_monomonial = polynomial.monomonial(i);
			scls::__Monomonial monomonial_antiderivated = *reinterpret_cast<scls::__Monomonial*>(current_monomonial->clone().get());

			// Get the current monomonial
			if(monomonial_antiderivated.unknown() == 0){
				monomonial_antiderivated.add_unknown("x", 1);
			}
			else {
				monomonial_antiderivated.set_factor(*monomonial_antiderivated.factor() / (monomonial_antiderivated.unknown()->exponent() + 1));
				monomonial_antiderivated.unknown()->set_exponent(monomonial_antiderivated.unknown()->exponent() + 1);
			}
			polynomial_antiderivated += monomonial_antiderivated;
		}

		// Set the good redaction
		if(redaction != 0) {
			if(redaction->at(redaction->size() - 1) != ' '){(*redaction)+=std::string(" ");}
			(*redaction) += scls::replace(scls::replace(__redaction_polynomial_antiderivation_final, std::string("<derivated>"), polynomial_antiderivated.to_std_string(&settings)), std::string("<full_formula>"), polynomial.to_std_string(&settings));
		}

		// Return the result
		return polynomial_antiderivated;
    }

	// Gets the derivated polynomial of a polynomial
	std::string __redaction_polynomial_derivation_0 = std::string("Cette forme est un simple polynôme, on peut obtenir facilement son polynôme dérivé grâce aux propriétés des polynômes.");
	std::string __redaction_polynomial_derivation_final = std::string("Donc, le polynôme dérivé de \"<full_formula>\" est \"<derivated>\".");
	scls::Polynomial polynomial_derivation(Function_Studied* function, std::string* redaction) {
    	scls::__Formula* formula = function->formula();
		scls::Polynomial polynomial = *formula->polynomial();
    	scls::Textual_Math_Settings settings;settings.set_hide_if_0(false);
		std::string unknown_name = std::string("x");

		// Set the good redaction
		if(redaction != 0) {
			(*redaction) += __redaction_polynomial_derivation_0;
		}

		// Get the derivated polynomial
		scls::Polynomial polynomial_derivated = scls::Polynomial();
		for(int i = 0;i<static_cast<int>(polynomial.monomonials().size());i++) {
			scls::__Monomonial* current_monomonial = polynomial.monomonial(i);
			scls::__Monomonial monomonial_derivated = *reinterpret_cast<scls::__Monomonial*>(current_monomonial->clone().get());
			if(monomonial_derivated.unknown() == 0){continue;}

			// Get the current monomonial
			monomonial_derivated.set_factor(*monomonial_derivated.factor() * monomonial_derivated.unknown()->exponent());
			monomonial_derivated.unknown()->set_exponent(monomonial_derivated.unknown()->exponent() - 1);
			if(monomonial_derivated.unknown()->exponent() == 0){monomonial_derivated.unknowns().clear();}
			polynomial_derivated += monomonial_derivated;
		}

		// Set the good redaction
		if(redaction != 0) {
			if(redaction->at(redaction->size() - 1) != ' '){(*redaction)+=std::string(" ");}
			(*redaction) += scls::replace(scls::replace(__redaction_polynomial_derivation_final, std::string("<derivated>"), polynomial_derivated.to_std_string(&settings)), std::string("<full_formula>"), polynomial.to_std_string(&settings));
		}

		// Return the result
		return polynomial_derivated;
    }

	// Gets the division of two polynomials
	void polynomial_division(scls::Polynomial* polynomial_1, scls::Polynomial* polynomial_2, std::string* redaction){
		std::shared_ptr<scls::Polynomial_Base> current = polynomial_1->clone();
		std::shared_ptr<scls::Polynomial> quotient = std::make_shared<scls::Polynomial>(0);

		// Redaction
		if(redaction != 0) {
			(*redaction) += "<p>Nous cherchons à diviser les polynômes suivants :</p>";
			(*redaction) += std::string("<math><mi>P</mi><mequal><mi>R</mi><mequal>") + polynomial_1->to_mathml(0) + std::string("</math></br>");
			(*redaction) += std::string("<math><mi>Q</mi><mequal>") + polynomial_2->to_mathml(0) + std::string("</math></br>");
			(*redaction) += std::string("<p>Utilisons un algorithme de division.</p>");
		}

		while(current.get()->degree("x") >= polynomial_2->degree("x")) {
			std::shared_ptr<scls::Polynomial_Base> to_substract = polynomial_2->clone();
			scls::__Monomonial* bigger = reinterpret_cast<scls::__Monomonial*>(polynomial_2->__bigger_monomonial());
			scls::__Monomonial* bigger_current = reinterpret_cast<scls::__Monomonial*>(current.get()->__bigger_monomonial());
			std::shared_ptr<scls::Polynomial> current_monomonial = std::make_shared<scls::Polynomial>(*(bigger_current->factor()) / *(bigger->factor()));
			if(bigger_current->unknown() != 0 && bigger_current->unknown()->exponent() - bigger->unknown()->exponent() > 0){
				std::shared_ptr<scls::Polynomial> current_monomonial_unknown = std::make_shared<scls::Polynomial>(1, "x", bigger_current->unknown()->exponent() - bigger->unknown()->exponent());
				current_monomonial.get()->__multiply(current_monomonial_unknown.get());
			}
			to_substract.get()->__multiply(current_monomonial.get());
			current.get()->__substract(to_substract.get());

			quotient.get()->__add(current_monomonial.get());

			// Redaction
			if(redaction != 0) {
				(*redaction) += "<p>Multiplions le quotient par :</p>";
				(*redaction) += std::string("<math><mi>M</mi><mequal>") + current_monomonial->to_mathml(0) + std::string("</math></br>");
				(*redaction) += std::string("<math><mi>Q * M</mi><mequal>") + to_substract.get()->to_mathml(0) + std::string("</math></br>");
				(*redaction) += std::string("<math><mi>R - (Q * M)</mi><mequal>") + current.get()->to_mathml(0) + std::string("</math></br>");
				(*redaction) += "<p>R sera maintenant égal à cette valeur.</p>";
			}
		}

		// Redaction
		if(redaction != 0) {
			(*redaction) += "<p>Donc :</p>";
			(*redaction) += std::string("<math><mi>P</mi><mequal><mo>(</mo>") + polynomial_2->to_mathml(0) + std::string("<mo>)</mo><mo>*</mo><mo>(</mo>") + quotient.get()->to_mathml(0) + std::string("<mo>)</mo><mo>+</mo><mo>(</mo>") + current.get()->to_mathml(0) + std::string("<mo>)</mo></math></br>");
		}
    }

    // Gets the roots of a polynomial
    std::string __redaction_root_0 = std::string("Le polynôme \"<full_formula>\" est constamment égal à 0, il admet tout son ensemble de définition comme racine.");
    std::string __redaction_root_0_not_0 = std::string("Le polynôme \"<full_formula>\" est constant, il n'a pas de racines.");
    std::string __redaction_root_1 = std::string("Le polynôme \"<full_formula>\" est linéaire, il admet comme solution :</br><math><mi>s</mi><mo>=</mo><mfrac><mi>-b</mi><mi>a</mi></mfrac><mo>=</mo><mfrac><mi>-<polynomial_0></mi><mi><polynomial_1></mi></mfrac><mo>=</mo><mi><solution_0></mi></math>");
    std::string __redaction_root_2_d = std::string("Le polynôme \"<full_formula>\" est de degré 2, utilisons la formule du discriminant : </br><math><mdelta><mo>=</mo><mi>b</mi><msup>2</msup><mi>-4ac</mi><mo>=</mo><polynomial_1><msup>2</msup><mo>-</mo><mi>4</mi><mo>*</mo><polynomial_2><mo>*</mo><polynomial_0><mo>=</mo><mi><solution_d></mi></math></br>");
    std::string __redaction_root_2_2r = std::string("Le discriminant est supérieur à 0, nous pouvons donc dénombrer 2 solutions distinctes : </br><math><mi>x</mi><msub>1</msub><mo>=</mo><mfrac><mrow><mi>-</mi><polynomial_1><mo>+</mo><msqrt><solution_d></msqrt></mrow><mrow><mi>2</mi><mo>*</mo><polynomial_2></mrow></mfrac><mo>=</mo><mi><solution_1></mi></math><math><mi>x</mi><msub>2</msub><mo>=</mo><mfrac><mrow><mi>-</mi><polynomial_1><mo>-</mo><msqrt><solution_d></msqrt></mrow><mrow><mi>2</mi><mo>*</mo><polynomial_2></mrow></mfrac><mo>=</mo><mi><solution_2></mi></math>");
    std::string __redaction_root_2_1r = std::string("Le discriminant est égal à 0, nous pouvons donc dénombrer 1 solution distincte : </br><math><mi>x</mi><mo>=</mo><mfrac><mrow><mi>-</mi><polynomial_1></mrow><mrow><mi>2</mi><mo>*</mo><polynomial_2></mrow></mfrac><mo>=</mo><mi><solution_1></mi></math>");
    std::string __redaction_root_2_0r = std::string("Le discriminant est inférieur à 0, nous n'avons pas de solutions réelles.");
    std::string __redaction_root_3_d = std::string("Le polynôme \"<full_formula>\" est de degré 3, utilisons la méthode de Cardan : </br><math><mdelta><mo>=</mo><mi>q'</mi><msup>2</msup><mo>+</mo><mi>p'</mi><msup>3</msup><mo>=</mo><polynomial_1><msup>2</msup><mo>-</mo><mi>4</mi><mo>*</mo><polynomial_2><mo>*</mo><polynomial_0><mo>=</mo><mi><solution_d></mi></math></br>");
    scls::Set_Number polynomial_roots(Function_Studied* function, std::string* redaction) {
        scls::__Formula* formula = function->formula();
    	scls::Polynomial polynomial = *formula->polynomial();
        scls::Textual_Math_Settings settings;settings.set_hide_if_0(false);
        scls::Set_Number to_return;
        std::string unknown_name = function->unknown();

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
            to_return.add_number(solution.real());

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

        return to_return;
    }

    // Solve an equation
    scls::Set_Number solve_equation(Function_Studied* function, std::string* redaction) {
        // The formula is a polynomial
    	scls::__Formula* formula = function->formula();
    	if(formula->is_simple_fraction()) {
    		(*redaction) += std::string("Cette forme est une fraction, elle admet comme racine les mêmes racines que sont numérateur. ");

    		std::shared_ptr<scls::__Formula_Base> numerator = formula->numerator();
    		if(numerator != 0 && numerator.get()->is_simple_polynomial()) {
    			std::shared_ptr<Function_Studied> new_function = Function_Studied::new_function_studied_shared_ptr(*numerator.get() , function);
    			return polynomial_roots(new_function.get(), redaction);
    		}
    	}
    	else if(formula->is_simple_polynomial()){
            return polynomial_roots(function, redaction);
        }

    	return scls::Set_Number();
    }

    // Solve a system of equation
    bool __linear(Function_Studied* function){
    	// Check if the function is a formula
    	if(!function->formula()->is_simple_polynomial()){return false;}

    	// Browse the polynomial
    	scls::Polynomial* polynomial = function->formula()->polynomial();
    	for(int i = 0;i<static_cast<int>(polynomial->monomonials().size());i++) {
    		scls::__Monomonial* monomonial = polynomial->monomonial(i);
    		if(static_cast<int>(monomonial->unknowns().size()) > 1){return false;}
    		else if(static_cast<int>(monomonial->unknowns().size()) == 1 && (monomonial->unknown() != 0 && monomonial->unknown()->exponent() > 1)){return false;};
    	}
    	return true;
    }
    void __substitution(int position, std::vector<Function_Studied*>& functions, std::vector<std::shared_ptr<scls::__Formula>>& values, std::vector<char>& values_found, std::vector<std::string>& unknowns, std::string* redaction) {
        // Substitution
        std::string needed_unknown = std::string();
        if(values_found.at(position) == 1 || values_found.at(position) < 0){
            // Redaction
            needed_unknown = unknowns.at(position);
            if(redaction != 0){(*redaction) += std::string("</br>On substitue l'inconnu ") + needed_unknown + std::string(" par ") + values.at(position).get()->to_std_string(0) + std::string(".</br>");}

            for(int k = 0;k<static_cast<int>(functions.size());k++){
                if(functions.at(k)->formula()->is_null() || -(values_found.at(position) + 1) == k){continue;}
                std::shared_ptr<scls::__Formula> substitued = functions.at(k)->formula()->replace_unknown(needed_unknown, values.at(position).get());
                functions.at(k)->set_formula(*substitued.get());
                if(values_found.at(position) == 1){values_found[position] = 2;}
            }
        }
    }
    void solve_system(std::vector<Function_Studied*> functions, std::string* redaction) {
    	// Check if the functions are linear
    	bool is_linear = true;
    	for(int i = 0;i<static_cast<int>(functions.size());i++){
    		Function_Studied* function = functions.at(i);
    		if(!__linear(function)){is_linear=false;break;}
    	}

    	if(is_linear) {
    		// If the system is linear
    		if(redaction != 0){
                (*redaction) += std::string("<p>Nous étudions un système linéaire :</p>");
                for(int i = 0;i<static_cast<int>(functions.size());i++) {(*redaction) += std::string("<math>") + functions.at(i)->formula()->to_mathml(0) + std::string("<mo>=</mo><mi>0</mi></math>");(*redaction)+=std::string("</br>");}
    		}

    		// Get the unknowns
    		std::vector<std::string> unknowns;
    		for(int i = 0;i<static_cast<int>(functions.size());i++){
    			std::vector<std::string> current_unknowns = functions.at(i)->formula()->all_unknowns();
    			for(int j = 0;j<static_cast<int>(current_unknowns.size());j++){
    				bool in = false;
    				for(int k = 0;k<static_cast<int>(unknowns.size());k++){
						if(current_unknowns.at(j) == unknowns.at(k)){in = true;break;}
					}
    				if(!in){unknowns.push_back(current_unknowns.at(j));}
    			}
    		}

    		// Same number of unknowns and of equations
    		if(unknowns.size() == functions.size()) {
    			// Basic datas
    			std::vector<std::shared_ptr<scls::__Formula>> values = std::vector<std::shared_ptr<scls::__Formula>>(unknowns.size());
    			std::vector<char> values_found = std::vector<char>(unknowns.size(), 0);
    			for(int i = 0;i<static_cast<int>(values.size());i++){values[i] = std::make_shared<scls::__Formula>(0);}

    			int a = 0;int max_value = static_cast<int>(values.size()) * static_cast<int>(values.size());
    			while(a < max_value){
    				// Check the normal equations
					for(int i = 0;i<static_cast<int>(functions.size());i++){
						std::vector<std::string> current_unknowns = functions.at(i)->formula()->all_unknowns();
						std::string needed_unknown = std::string();
						if(current_unknowns.size() == 1){
							// Get the solution
							needed_unknown = current_unknowns.at(0);
							if(redaction != 0){(*redaction)+=std::string("</br>");}
							Function_Studied f;f.set_formula(*functions.at(i)->formula());f.set_function_unknown(needed_unknown);
							scls::Set_Number n = solve_equation(&f, redaction);
							scls::Fraction solution = n.number();

							// Put the solution
							int j = 0;
							for(;j<static_cast<int>(unknowns.size());j++){
								if(unknowns.at(j) == needed_unknown){
									(*values[j].get()) = scls::Complex(solution);values_found[j] = 1;break;
								}
							}

							// Substitution
                            __substitution(j, functions, values, values_found, unknowns, redaction);

                            // Redaction
                            if(redaction != 0){
                                (*redaction) += std::string("<p>Après ce passage, le système devient : </p>");
                                for(int i = 0;i<static_cast<int>(functions.size());i++) {(*redaction) += std::string("<math>") + functions.at(i)->formula()->to_mathml(0) + std::string("<mo>=</mo><mi>0</mi></math>");(*redaction)+=std::string("</br>");}
                            }
						}
						else if(current_unknowns.size() > 0) {
                            // Get the solution
							needed_unknown = current_unknowns.at(0);
							if(redaction != 0){(*redaction)+=std::string("</br>");}
							scls::__Formula total = *functions.at(i)->formula();
							scls::__Formula m = total.monomonial(needed_unknown);
							m.multiply(-1);total.__add(&m);
							total.divide(m.monomonial()->factor());

							// Put the solution
							int j = 0;
							for(;j<static_cast<int>(unknowns.size());j++){
								if(unknowns.at(j) == needed_unknown){
									(*values[j].get()) = total;values_found[j] = -i - 1;break;
								}
							}

							// Substitution
                            __substitution(j, functions, values, values_found, unknowns, redaction);

                            // Redaction
                            if(redaction != 0){
                                (*redaction) += std::string("<p>Après ce passage, le système devient : </p>");
                                for(int i = 0;i<static_cast<int>(functions.size());i++) {(*redaction) += std::string("<math>") + functions.at(i)->formula()->to_mathml(0) + std::string("<mo>=</mo><mi>0</mi></math>");(*redaction)+=std::string("</br>");}
                            }
						}
					}

					// Check value
					bool good = true;
					for(int j = 0;j<static_cast<int>(values_found.size());j++){
						if(values_found.at(j) <= 0){good=false;break;}
					}
					if(good){break;}
					a++;
    			}

    			// Redaction
                if(redaction != 0){
                    (*redaction) += std::string("<p>Finalement :</p>");
                    for(int i = 0;i<static_cast<int>(unknowns.size());i++) {(*redaction) += std::string("<math><mi>") + unknowns.at(i) + std::string("</mi><mo>=</mo>") + values.at(i).get()->to_mathml(0) + std::string("</math>");(*redaction)+=std::string("</br>");}
                }
    		}
    	}
    }

    //******************
	//
	// Function handling
	//
	//******************

    // Calculate the antiderivated function of a function
    std::shared_ptr<scls::__Formula_Base> function_antiderivation(Function_Studied* function, std::string* redaction) {
		// Study the formula
		scls::__Formula* formula = function->formula();
		if(formula->is_simple_polynomial()){
			scls::Polynomial derivated = polynomial_antiderivation(function, redaction);
			std::shared_ptr<scls::__Formula_Base> to_return = formula->new_formula();
			to_return.get()->set_polynomial(&derivated);
			return to_return;
		}

		// Another case
		return std::shared_ptr<scls::__Formula_Base>();
	}

	// Returns the definition set of a function
    std::string __redaction_definition_set_polynomial = std::string("Cette formule est un polynôme : elle est définie sur tout R.");
	scls::Set_Number function_definition_set(Function_Studied* function, std::string* redaction) {
		scls::Set_Number to_return;

		// Study the formula
		scls::__Formula* formula = function->formula();
		if(formula->is_simple_fraction()) {
			if(redaction != 0){(*redaction) += std::string("Cette forme est une fraction, elle admet comme ensemble de non-définition les mêmes racines que sont dénominateur. ");}

			std::shared_ptr<scls::__Formula_Base> denominator = formula->denominator();
			if(denominator != 0 && denominator.get()->is_simple_polynomial()) {
				std::shared_ptr<Function_Studied> new_function = Function_Studied::new_function_studied_shared_ptr(*denominator.get() , function);
				polynomial_roots(new_function.get(), redaction);
			}
		}
		else if(formula->is_simple_polynomial()){to_return = scls::Set_Number::real();if(redaction!=0){(*redaction)+=__redaction_definition_set_polynomial;}}

		// Return the result
		function->set_definition_set(to_return);
		return to_return;
	}

	// Calculate the derivation of a production of functions
	void quotient_derivation(Function_Studied* numerator, Function_Studied* denominator, std::string* redaction) {
		if(redaction != 0) {
			(*redaction) += std::string("Appliquons la formule :</br><math><mi>(u / v)'</mi><mequal><mfrac><mrow><mi>u'v</mi><mo>-</mo><mi>uv'</mi></mrow><mrow><mi>u</mi><msup>2</msup></mrow></mfrac></math></br>");
		}

		// Do the calculation
		std::shared_ptr<scls::__Formula_Base> derivated_1_shared_ptr = function_derivation(numerator, redaction);
		derivated_1_shared_ptr.get()->__multiply(denominator->formula());
		std::shared_ptr<scls::__Formula_Base> derivated_2_shared_ptr = numerator->formula()->clone();
		derivated_2_shared_ptr.get()->__multiply(function_derivation(denominator, redaction).get());
		derivated_1_shared_ptr.get()->__substract(derivated_2_shared_ptr.get());
		derivated_2_shared_ptr = denominator->formula()->clone();
		derivated_2_shared_ptr.get()->__multiply(derivated_2_shared_ptr.get());
		derivated_1_shared_ptr.get()->__divide(derivated_2_shared_ptr.get());

		if(redaction != 0) {
			(*redaction) += std::string("</br>Donc, la forme dérivée de ce quotient est :</br><math>") + derivated_1_shared_ptr.get()->to_mathml(0) + ("</math></br>");
		}
	}

	// Calculate the derivated function of a function
	std::shared_ptr<scls::__Formula_Base> function_derivation(Function_Studied* function, std::string* redaction) {
		// Study the formula
		scls::__Formula* formula = function->formula();
		if(formula->is_simple_fraction()) {
			(*redaction) += std::string("Cette forme est une fraction, sa dérivée s'obtient donc grâce à une formule déduite de la règle de la chaîne. ");

			std::shared_ptr<scls::__Formula_Base> numerator = formula->numerator();
			std::shared_ptr<scls::__Formula_Base> denominator = formula->denominator();
			if(numerator.get() != 0 && denominator.get() != 0) {
				std::shared_ptr<Function_Studied> new_function_1 = Function_Studied::new_function_studied_shared_ptr(*numerator.get() , function);
				std::shared_ptr<Function_Studied> new_function_2 = Function_Studied::new_function_studied_shared_ptr(*denominator.get() , function);
				quotient_derivation(new_function_1.get(), new_function_2.get(), redaction);
			}
		}
		else if(formula->is_simple_polynomial()){
			scls::Polynomial derivated = polynomial_derivation(function, redaction);
			std::shared_ptr<scls::__Formula_Base> to_return = formula->new_formula();
			to_return.get()->set_polynomial(&derivated);
			return to_return;
		}

		// Another case
		return std::shared_ptr<scls::__Formula_Base>();
	}
}
