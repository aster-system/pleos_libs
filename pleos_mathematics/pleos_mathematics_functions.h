//******************
//
// pleos_mathematics_functions.h
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
// This file contains the handling of functions in the PLEOS mathematics file.
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

#ifndef PLEOS_MATHEMATICS_FUNCTIONS
#define PLEOS_MATHEMATICS_FUNCTIONS

// Include SCLS Graphic Benoit
#include "pleos_mathematics_geometry.h"

// Possibles operations at click
#define PLEOS_OPERATION_NOTHING 0
#define PLEOS_OPERATION_VECTOR 100
#define PLEOS_OPERATION_POINT 101
#define PLEOS_OPERATION_FORM 150

// The namespace "pleos" is used to simplify the all.
namespace pleos {

 	class Function_Studied {
        // Datas about a studied function

    public:
        // Constructor of a studied function
 		Function_Studied(){};
 		Function_Studied(std::shared_ptr<scls::__Formula> formula){set_formula(*formula.get());};

        // Returns the introduction for this function
        std::string introduction(scls::Textual_Math_Settings* settings) const{return std::string();};

        // Creates and returns a new function studied
        static std::shared_ptr<Function_Studied> new_function_studied_shared_ptr(scls::__Formula needed_formula, Function_Studied* parent){std::shared_ptr<Function_Studied> to_return = std::make_shared<Function_Studied>();to_return.get()->a_this_object=to_return;to_return.get()->a_parent_object=parent->a_this_object;to_return.get()->set_formula(needed_formula);return to_return;};
        static std::shared_ptr<Function_Studied> new_function_studied_shared_ptr(scls::__Formula needed_formula){std::shared_ptr<Function_Studied> to_return = std::make_shared<Function_Studied>();to_return.get()->a_this_object=to_return;to_return.get()->set_formula(needed_formula);return to_return;};

        // Getters and setters
        inline scls::Color color() const {return a_color;};
        inline scls::GUI_Text* connected_object()const{return a_connected_object.lock().get();};
        inline scls::Set_Number* definition_set(){return a_definition_set.get();};
        inline scls::__Formula* formula()const{return a_function_segments.at(0).formula.get();};
        inline std::shared_ptr<scls::__Formula> formula_copy()const{return formula()->clone();};
        inline int level() const {if(a_parent_object.lock().get() == 0){return 0;}return a_parent_object.lock().get()->level() + 1;}
        inline std::string name() const {return a_function_name;};
        inline scls::Set_Number* roots(){return a_roots.get();};
        inline void set_color(scls::Color new_color){a_color = new_color;};
        inline void set_connected_object(std::weak_ptr<scls::GUI_Text> new_connected_object){a_connected_object = new_connected_object;};
        inline void set_definition_set(scls::Set_Number new_definition_set){a_definition_set=std::make_shared<scls::Set_Number>(new_definition_set);};
        inline void set_formula(scls::__Formula formula){a_function_segments.at(0).formula = std::make_shared<scls::__Formula>(formula);};
        inline void set_function_unknown(std::string unknown){a_function_unknown = unknown;};
        inline void set_name(std::string new_name){a_function_name = new_name;if(connected_object()!=0){connected_object()->set_text(std::string("Fonction ") + new_name);}};
        inline void set_roots(scls::Set_Number new_roots){a_roots = std::make_shared<scls::Set_Number>(new_roots);};
        inline void set_sign_set(scls::Set_Number new_sign_set){a_sign_set = std::make_shared<scls::Set_Number>(new_sign_set);};
        inline scls::Set_Number* sign_set(){return a_sign_set.get();};
        inline std::shared_ptr<Function_Studied> this_object()const{return a_this_object.lock();};
        inline std::string unknown() const {return a_function_unknown;};

    private:

        // Segmented parts
        struct Segmented_Part {std::shared_ptr<scls::__Formula> formula = std::make_shared<scls::__Formula>();};
        // Formulas of the function
        std::vector<Segmented_Part> a_function_segments = std::vector<Segmented_Part>(1, Segmented_Part());

        // Connected object to this vector
        std::weak_ptr<scls::GUI_Text> a_connected_object = std::weak_ptr<scls::GUI_Text>();
        // Name of the function
        std::string a_function_name = "";
        // Number of the function
        int a_function_number = 0;
        // Name of the unknown in the function
        std::string a_function_unknown = "x";

        // Color of the curve
        scls::Color a_color = scls::Color(255, 0, 0);

        // Weak pointer to the parent of this function
        std::weak_ptr<Function_Studied> a_parent_object;
        // Weak pointer to this function
        std::weak_ptr<Function_Studied> a_this_object;

        // Studied things
        // Definition set of the function
        std::shared_ptr<scls::Set_Number> a_definition_set;
        // Roots of the function
        std::shared_ptr<scls::Set_Number> a_roots;
        // Sign (positive) of the function
        std::shared_ptr<scls::Set_Number> a_sign_set;
    };

    class Function_Table {
        // Class representating a table made for functions

    public:
        // Function_Table constructor
        Function_Table(){};

        // Add a function to study
        inline void add_function(Function_Studied* needed_function){a_studied_function.push_back(needed_function->this_object());};

        // Returns the text for the table to xml
        std::string to_xml(std::string* redaction, scls::Textual_Math_Settings* settings);

    private:
        // Studied functions
        std::vector<std::shared_ptr<Function_Studied>> a_studied_function;
    };

    //******************
    //
    // Polynomial handling
    //
    //******************

    // Gets the antiderivated polynomial of a polynomial
    scls::Polynomial polynomial_antiderivation(Function_Studied* function, std::string* redaction);

    // Gets the derivated polynomial of a polynomial
    scls::Polynomial polynomial_derivation(Function_Studied* function, std::string* redaction);

    // Gets the division of two polynomials
    void polynomial_division(scls::Polynomial* polynomial_1, scls::Polynomial* polynomial_2, std::string* redaction);

    // Gets the roots of a polynomial
    scls::Set_Number polynomial_roots(Function_Studied* function, std::string* redaction);

    // Solve an equation
    scls::Set_Number solve_equation(Function_Studied* function, std::string* redaction);

    // Solve a system of equation
    void solve_system(std::vector<Function_Studied*> functions, std::string* redaction);

    //******************
	//
	// Function handling
	//
	//******************

    // Calculate the antiderivated function of a function
    std::shared_ptr<scls::__Formula_Base> function_antiderivation(Function_Studied* function, std::string* redaction);

    // Returns the definition set of a function
    scls::Set_Number function_definition_set(Function_Studied* function, std::string* redaction);

    // Calculate the derivated function of a function
    std::shared_ptr<scls::__Formula_Base> function_derivation(Function_Studied* function, std::string* redaction);
}

#endif // PLEOS_MATHEMATICS_FUNCTIONS
