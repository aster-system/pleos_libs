//******************
//
// pleos_environment.h
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
// This file contains the environment system, very usefull for using a specific PLEOS part.
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

#ifndef PLEOS_ENVIRONMENT
#define PLEOS_ENVIRONMENT

// Include SCLS Graphic Benoit
#include "../../scls-graphic-benoit/scls_graphic.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //*********
	//
	// The Text_Environment class
	//
	//*********

	// Loads the PLEOS balises
    void load_balises_pleos(std::shared_ptr<scls::_Balise_Style_Container> defined_balises);

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

        // Handle repetitions
        // Adds a repetition
        inline void add_repetition(){a_repetitions.push_back(0);};
        // Returns the last repetition
        inline int last_repetition(){if(a_repetitions.size() <= 0){return 0;}return a_repetitions.at(a_repetitions.size() - 1);}
        // Removes the last repetition
        inline void remove_repetition(){if(a_repetitions.size() > 0){a_repetitions.pop_back();}};
        // Sets the last repetition
        inline void set_repetition(int value){if(a_repetitions.size() > 0){a_repetitions[a_repetitions.size() - 1] = value;}};

        // Handle unknowns
        // Creates a unknown
        scls::__Formula_Base::Unknown* create_unknown(std::string name);
        std::shared_ptr<scls::__Formula_Base::Unknown> create_unknown_shared_ptr(std::string name);
        // Returns a value by its name
        scls::Fraction value_by_name(std::string name)const;
        // Returns a unknown by its name
        scls::__Formula_Base::Unknown* unknown_by_name(std::string name)const;
        std::shared_ptr<scls::__Formula_Base::Unknown> unknown_shared_ptr_by_name(std::string name)const;

        // Returns a color value
        scls::Color value_color(std::string base)const;
        // Returns a formula value
        scls::__Formula_Base::Formula value_formula(std::string base)const;
        // Returns a number value
        scls::Fraction value_number(std::string base)const;
        // Returns a Point_2D_Formula value
        scls::Point_2D_Formula value_point_2d(std::string base)const;

        // Getters and setters
        inline scls::__Formula_Base::Unknowns_Container* unknowns(){return a_unknowns.get();};
        inline std::shared_ptr<scls::__Formula_Base::Unknowns_Container> unknowns_shared_ptr(){return a_unknowns;};

        //*********
        // Definition system
        //*********

        // Class containing a definition
        class Definition {

        public:
            // Content of the definitions
            struct __Content{__Content(std::string needed_content);__Content(std::string needed_name,std::string needed_content);std::string content = std::string();std::string name = std::string();};;

            // Definition constructor
            Definition(std::string name):a_name(name){};

            // Adds a content to the definition
            void add_content(std::string content);
            void add_content(std::string content_name, std::string content);
            // Gets one content of the definition
            std::string content(bool capitalise_first_letter)const;
            std::string content(std::string content_name, bool capitalise_first_letter)const;
            __Content* content_full_by_name(std::string content_name) const;
            std::shared_ptr<__Content> content_full_by_name_shared_ptr(std::string content_name) const;

            // Adds a theorem to the definition
            void add_theorem(std::string theorem);
            void add_theorem(std::string theorem_name, std::string theorem);
            // Gets one theorem of the definition
            std::string theorem(bool capitalise_first_letter)const;
            std::string theorem(std::string theorem_name, bool capitalise_first_letter)const;
            __Content* theorem_full_by_name(std::string content_name) const;
            std::shared_ptr<__Content> theorem_full_by_name_shared_ptr(std::string content_name) const;

            // Getters and setters
            inline std::string name() const {return a_name;};
        private:
            // Contents of the definition
            std::vector<std::shared_ptr<__Content>> a_contents;
            // Name of the definition
            std::string a_name = std::string();
            // Theorems of the definition
            std::vector<std::shared_ptr<__Content>> a_theorems;
        };

        // Returns a definition by its name
        Definition* definition_by_name(std::string name);
        std::shared_ptr<Definition> definition_by_name_shared_ptr(std::string name);

        // Loads the definitions
        void __load_definition_from_xml(std::shared_ptr<scls::__XML_Text_Base> current_text);
        void load_definitions_from_path(std::string path);

        // Creates and returns a new definition
        std::shared_ptr<Definition> new_definition(std::string definition_name);

        // Getters and setters
        inline std::vector<std::shared_ptr<Definition>>& definitions() {return a_definitions;};
    private:

        // Repetitions
        std::vector<int> a_repetitions;
        // Variables
        std::shared_ptr<scls::__Formula_Base::Unknowns_Container> a_unknowns = std::make_shared<scls::__Formula_Base::Unknowns_Container>();

        //*********
        // Definition system
        //*********

        // Created definitions
        std::vector<std::shared_ptr<Definition>> a_definitions;
	};
}

#endif // PLEOS_ENVIRONMENT
