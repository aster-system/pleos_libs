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
    //*********
	//
	// The Text_Environment class
	//
	//*********

	class Text_Environment {
	    // Class representating an environment for text in PLEOS
    public:

        // Variables
        struct Variable {
            // Name of the variable
            std::string name = std::string("x");
            // Value of the variable
            scls::Fraction value = 0;
        };

        // Text_Environment constructor
        Text_Environment(){};

        // Clears the environment
        void clear(){a_unknowns.get()->clear();};

        // Handle unknowns
        // Creates a unknown
        scls::__Formula_Base::Unknown* create_unknown(std::string name){return a_unknowns.get()->create_unknown(name);};
        std::shared_ptr<scls::__Formula_Base::Unknown> create_unknown_shared_ptr(std::string name){return a_unknowns.get()->create_unknown_shared_ptr(name);};
        // Returns a value by its name
        scls::Fraction value_by_name(std::string name)const{scls::__Formula_Base::Unknown*unknow=unknown_by_name(name);if(unknow==0){return 0;}return (unknow->value.get()->value(0).real());};
        // Returns a unknown by its name
        inline scls::__Formula_Base::Unknown* unknown_by_name(std::string name)const{return a_unknowns.get()->unknown_by_name(name);};
        inline std::shared_ptr<scls::__Formula_Base::Unknown> unknown_shared_ptr_by_name(std::string name)const{return a_unknowns.get()->unknown_shared_ptr_by_name(name);};

        // Returns a formula value
        scls::__Formula_Base::Formula value_formula(std::string base)const{scls::__Formula_Base formula = scls::string_to_formula(base);return formula.replace_unknowns(a_unknowns.get());};
        // Returns a number value
        scls::Fraction value_number(std::string base)const{scls::__Formula_Base formula = scls::string_to_formula(base);return formula.value(a_unknowns.get()).real();};
        // Returns a Point_2D value
        scls::Point_2D value_point_2d(std::string base)const;

        // Getters and setters
        inline scls::__Formula_Base::Unknowns_Container* unknowns(){return a_unknowns.get();};
        inline std::shared_ptr<scls::__Formula_Base::Unknowns_Container> unknowns_shared_ptr(){return a_unknowns;};

    private:

        // Variables
        std::shared_ptr<scls::__Formula_Base::Unknowns_Container> a_unknowns = std::make_shared<scls::__Formula_Base::Unknowns_Container>();
	};

    // Possible studied type
    enum Studied_Type {ST_Sequence};

    class Function_Studied {
        // Datas about a studied function

    public:
        // Constructor of a studied function
        Function_Studied(){};

        // Returns the introduction for this function
        std::string introduction(scls::Textual_Math_Settings* settings) const;

        // Creates and returns a new function studied
        static std::shared_ptr<Function_Studied> new_function_studied_shared_ptr(scls::Formula needed_formula, Function_Studied* parent){std::shared_ptr<Function_Studied> to_return = std::make_shared<Function_Studied>();to_return.get()->a_this_object=to_return;to_return.get()->a_parent_object=parent->a_this_object;to_return.get()->set_formula(needed_formula);return to_return;};
        static std::shared_ptr<Function_Studied> new_function_studied_shared_ptr(scls::Formula needed_formula){std::shared_ptr<Function_Studied> to_return = std::make_shared<Function_Studied>();to_return.get()->a_this_object=to_return;to_return.get()->set_formula(needed_formula);return to_return;};

        // Getters and setters
        inline scls::Color color() const {return a_color;};
        inline scls::GUI_Text* connected_object()const{return a_connected_object.lock().get();};
        inline scls::Set_Number* definition_set(){return a_definition_set.get();};
        inline scls::Formula* formula()const{return a_function_segments.at(0).formula.get();};
        inline std::shared_ptr<scls::Formula> formula_copy()const{return formula()->formula_copy();};
        inline int level() const {if(a_parent_object.lock().get() == 0){return 0;}return a_parent_object.lock().get()->level() + 1;}
        inline std::string name() const {return a_function_name;};
        inline scls::Set_Number* roots(){return a_roots.get();};
        inline void set_color(scls::Color new_color){a_color = new_color;};
        inline void set_connected_object(std::weak_ptr<scls::GUI_Text> new_connected_object){a_connected_object = new_connected_object;};
        inline void set_definition_set(scls::Set_Number new_definition_set){a_definition_set=std::make_shared<scls::Set_Number>(new_definition_set);};
        inline void set_formula(scls::Formula formula){a_function_segments.at(0).formula = std::make_shared<scls::Formula>(formula);};
        inline void set_function_unknown(std::string unknown){a_function_unknown = unknown;};
        inline void set_name(std::string new_name){a_function_name = new_name;if(connected_object()!=0){connected_object()->set_text(std::string("Fonction ") + new_name);}};
        inline void set_roots(scls::Set_Number new_roots){a_roots = std::make_shared<scls::Set_Number>(new_roots);};
        inline void set_sign_set(scls::Set_Number new_sign_set){a_sign_set = std::make_shared<scls::Set_Number>(new_sign_set);};
        inline scls::Set_Number* sign_set(){return a_sign_set.get();};
        inline std::shared_ptr<Function_Studied> this_object()const{return a_this_object.lock();};
        inline std::string unknown() const {return a_function_unknown;};

    private:

        // Segmented parts
        struct Segmented_Part {std::shared_ptr<scls::Formula> formula = std::make_shared<scls::Formula>();};
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
        // Type of the studied function
        Studied_Type a_type = Studied_Type::ST_Sequence;

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

    // Returns the definition set of a function
    scls::Set_Number function_definition_set(Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings);
    // Returns the derivation of a function
    scls::Formula function_derivation(Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings);
    // Returns the image of a function
    scls::__Formula_Base::Formula function_image(Function_Studied* current_function, scls::Formula needed_value, std::string& redaction, scls::Textual_Math_Settings* settings);
    // Returns the limit of a function / polymonial in + infinity
    scls::Limit function_limit(Function_Studied* current_function, scls::Limit needed_limit, std::string& redaction, scls::Textual_Math_Settings* settings);
    // Returns the primitive of a function
    scls::Formula function_primitive(Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings);
    // Returns the set of roots of a function
    scls::Set_Number function_roots(Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings);
    // Returns the set of a positive function
    scls::Set_Number function_sign(Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings);
    std::string function_sign_table(Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings);

    //******************
    //
    // Sequences handling
    //
    //******************

    // Returns the interval of an sequence
    //scls::Interval sequence_variation(Function_Studied current_function, std::string& redaction);
}

#endif // PLEOS_MATHEMATICS_FUNCTIONS
