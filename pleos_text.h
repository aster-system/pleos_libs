//******************
//
// pleos_text.h
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
// This file contains the core of the redaction part for PLEOS.
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

#ifndef PLEOS_TEXT
#define PLEOS_TEXT

// Include SCLS Graphic Benoit
#include "../../scls-graphic-benoit/scls_graphic.h"
#include "pleos_mathematics/pleos_mathematics_functions.h"
#include "pleos_it.h"
#include "pleos_table.h"

// Define a macro to easily add an object
// Create the object
#define GUI_PAGE(type, gui, gui_page, function, display_function, display_function_parent) private: std::shared_ptr<type> gui; \
public: inline type* function() const {return gui.get();} \
public: void display_function(){if(function()==0){scls::print("Warning", std::string("SCLS GUI Page \"") + name() + std::string("\""), std::string("The object \"") + std::to_string(gui_page) + std::string("\" does not exists."));}else{display_function_parent();set_current_page(gui_page);function()->set_visible(true);}}
#define GUI_OBJECT(type, gui, function) private: std::shared_ptr<type> gui; \
public: inline type* function() const {return gui.get();}
// Get the object
#define GUI_OBJECT_CREATION(type, gui, name) else if(object_name == name){gui = *parent->new_object<type>(object_name);return gui;}
// Select the object
#define GUI_OBJECT_SELECTION(display_function, name) else if(page == name){display_function;}

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

        // Handle variables
        // Creates a variable
        Variable* create_variable(std::string name){return create_variable_shared_ptr(name).get();};
        std::shared_ptr<Variable> create_variable_shared_ptr(std::string name){std::shared_ptr<Variable> variable=variable_shared_ptr_by_name(name);if(variable.get()!=0){return variable;}variable=std::make_shared<Variable>();a_variables.push_back(variable);variable.get()->name=name;return variable;};
        // Returns a variable by its name
        Variable* variable_by_name(std::string name)const{return variable_shared_ptr_by_name(name).get();};
        std::shared_ptr<Variable> variable_shared_ptr_by_name(std::string name)const{for(int i = 0;i<static_cast<int>(a_variables.size());i++){if(a_variables.at(i).get()->name == name){return a_variables.at(i);}} return std::shared_ptr<Variable>();};

        // Returns a number value
        scls::Fraction value_number(std::string base)const{Variable*variable=variable_by_name(base);if(variable!=0){return variable->value;}return scls::Fraction::from_std_string(base);};

        // Getters and setters
        inline std::vector<std::shared_ptr<Variable>>& variables(){return a_variables;};

    private:

        // Variables
        std::vector<std::shared_ptr<Variable>> a_variables;
	};

    //*********
	//
	// PLEOS Text handler
	//
	//*********

	// Creates and returns a graphic from an std::string
	void graphic_from_xml(Graphic& graphic, std::shared_ptr<scls::XML_Text> xml, scls::Text_Style needed_style, int& graphic_width_in_pixel, int& graphic_height_in_pixel);
	void graphic_from_xml(std::shared_ptr<Graphic> graphic_shared_ptr, std::shared_ptr<scls::XML_Text> xml, scls::Text_Style needed_style, int& graphic_width_in_pixel, int& graphic_height_in_pixel);
	std::shared_ptr<Graphic> graphic_from_xml(std::shared_ptr<scls::XML_Text> xml, scls::Text_Style needed_style, int& graphic_width_in_pixel, int& graphic_height_in_pixel);
	std::shared_ptr<scls::Image> graphic_image_from_xml(std::shared_ptr<scls::XML_Text> xml, scls::Text_Style needed_style);

	// Creates and returns a linked-list from an std::string
	std::shared_ptr<Linked_List<std::string>> linked_list_from_xml(std::shared_ptr<scls::XML_Text> xml, scls::Text_Style needed_style);

	// Creates and returns a table from an std::string
	std::shared_ptr<Table> table_from_xml(std::shared_ptr<scls::XML_Text> xml, scls::Text_Style needed_style);

	// Creates and returns a tree from an std::string
	std::shared_ptr<Tree<std::string>> tree_from_xml(std::shared_ptr<scls::XML_Text> xml, scls::Text_Style needed_style);
	inline std::shared_ptr<Tree<std::string>> tree_from_xml(std::string xml, scls::Text_Style needed_style, std::shared_ptr<scls::__Balise_Container> balise){return tree_from_xml(scls::xml(balise, xml), needed_style);};
	inline std::shared_ptr<Tree<std::string>> tree_from_xml(std::string xml, std::shared_ptr<scls::__Balise_Container> balise){return tree_from_xml(scls::xml(balise, xml), scls::Text_Style());};

	// Creates an image for a PLEOS text
	std::shared_ptr<scls::Image> generate_text_image(std::shared_ptr<scls::XML_Text> current_text, std::shared_ptr<scls::Text_Style> needed_style);

	class __Text_Line : public scls::Text_Image_Line {
        // Class representating a more complete text handler for PLEOS
    public:

        // __Text_Line constructor
        __Text_Line(std::shared_ptr<scls::_Balise_Style_Container> defined_balises, std::shared_ptr<scls::XML_Text> text):scls::Text_Image_Line(defined_balises,text){};

        // Generate a word
        virtual void generate_word(std::shared_ptr<scls::XML_Text> current_text, unsigned int& current_position_in_plain_text, std::shared_ptr<scls::Text_Style> needed_style, std::shared_ptr<scls::Text_Image_Word>& word_to_add);
    private:
    };

	class __Text_Block : public scls::Text_Image_Block {
        // Class representating a more complete text handler for PLEOS
    public:

        // __Text_Block constructor
        __Text_Block(std::shared_ptr<scls::_Balise_Style_Container> defined_balises, std::shared_ptr<scls::Block_Datas> datas):scls::Text_Image_Block(defined_balises,datas){};
        __Text_Block(std::shared_ptr<scls::_Balise_Style_Container> defined_balises, scls::String text, std::shared_ptr<scls::Text_Style> style):scls::Text_Image_Block(defined_balises, text, style){};

        // Creates and returns a line for the block
        virtual scls::Text_Image_Line* __create_line(scls::Line_Datas& needed_datas){return new __Text_Line(defined_balises_shared_ptr(), needed_datas.content);};
    private:
    };

	class Text : public scls::Text_Image_Multi_Block {
        // Class representating a more complete text handler for PLEOS
    public:

        // Text constructor
        Text():Text(std::make_shared<scls::_Balise_Style_Container>(),std::string()){};
        Text(std::shared_ptr<scls::_Balise_Style_Container> defined_balises, std::string text):scls::Text_Image_Multi_Block(defined_balises,text){load_balises(defined_balises);set_text(text);};
        Text(std::shared_ptr<scls::_Balise_Style_Container> defined_balises, std::string text, std::shared_ptr<scls::Text_Style> style):scls::Text_Image_Multi_Block(defined_balises, text, style){load_balises(defined_balises);set_text(text);};

        // Creates and returns a __Text_Block
        virtual std::shared_ptr<scls::Text_Image_Block>__create_block(std::shared_ptr<scls::Block_Datas>needed_datas){return std::make_shared<__Text_Block>(defined_balises_shared_ptr(), needed_datas);};

        // Loads the needed balises
        void load_balises(std::shared_ptr<scls::_Balise_Style_Container> defined_balises);

    private:
    };

    //*********
    // GUI Handling
    //*********

    // Loads the PLEOS balises
    void load_balises_pleos(std::shared_ptr<scls::_Balise_Style_Container> defined_balises);

    class GUI_Text : public scls::GUI_Text_Base<Text> {
        // Class representing an GUI object displaying a text into the window
    public:

        class __GUI_Text_Block_Graphic : public scls::__GUI_Text_Metadatas::__GUI_Text_Block {
            // Children of a GUI text containing a graphic
        public:
            // Most basic __GUI_Text_Block constructor
            __GUI_Text_Block_Graphic(std::shared_ptr<GUI_Object> needed_object):scls::__GUI_Text_Metadatas::__GUI_Text_Block(needed_object){};

            // Updates the texture of the block
            virtual void update_texture(scls::Text_Image_Block* block_to_apply, scls::Image_Generation_Type generation_type);

            // Graphic in the block
            inline Graphic* graphic() const {return graphic_object()->graphic();};
            inline Graphic_Object* graphic_object() const {return reinterpret_cast<Graphic_Object*>(object());};
        };

        //*********
        //
        // GUI Text main functions
        //
        //*********

        // Most basic GUI_Object constructor
        GUI_Text(scls::_Window_Advanced_Struct& window, std::string name, std::weak_ptr<GUI_Object> parent):scls::GUI_Text_Base<Text>(window, name, parent){};

        // Creates a text block from a block of text
        virtual std::shared_ptr<scls::__GUI_Text_Metadatas::__GUI_Text_Block> __create_text_block_object(scls::Text_Image_Block* block_to_apply);
    };
}

#endif //PLEOS_TEXT
