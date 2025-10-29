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
#include "pleos_graphic.h"
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
	// PLEOS Text handler
	//
	//*********

	// Creates and returns a graphic from an std::string
    std::shared_ptr<Graphic> graphic_from_xml(std::shared_ptr<scls::__XML_Text_Base> xml, scls::Text_Style needed_style, int& graphic_width_in_pixel, int& graphic_height_in_pixel);
	std::shared_ptr<scls::__Image_Base> graphic_image_from_xml(std::shared_ptr<scls::__XML_Text_Base> xml, scls::Text_Style needed_style);

	// Creates and returns a linked-list from an std::string
	std::shared_ptr<Linked_List> linked_list_from_xml(std::shared_ptr<scls::__XML_Text_Base> xml, scls::Text_Style needed_style);

	// Creates and returns a tree from an std::string
	std::shared_ptr<Tree<std::string>> tree_from_xml(std::shared_ptr<scls::__XML_Text_Base> xml, scls::Text_Style needed_style);
	inline std::shared_ptr<Tree<std::string>> tree_from_xml(std::string xml, scls::Text_Style needed_style, std::shared_ptr<scls::__Balise_Container> balise){return tree_from_xml(scls::xml(balise, xml), needed_style);};
	inline std::shared_ptr<Tree<std::string>> tree_from_xml(std::string xml, std::shared_ptr<scls::__Balise_Container> balise){return tree_from_xml(scls::xml(balise, xml), scls::Text_Style());};

	// Creates an image for a PLEOS text
	std::shared_ptr<scls::__Image_Base> generate_text_image(std::shared_ptr<scls::__XML_Text_Base> current_text, scls::Text_Style needed_style, std::shared_ptr<Text_Environment> possible_environment);
	std::shared_ptr<scls::__Image_Base> generate_text_image(std::shared_ptr<scls::__XML_Text_Base> current_text, scls::Text_Style needed_style, std::shared_ptr<scls::__XML_Text_Base> parent_text, std::shared_ptr<Text_Environment> possible_environment);

	// Returns if a balise is a special PLEOS balise
	int is_special_pleos_balise(std::string name);

	class Text : public scls::Text_Image_Block {
        // Class representating a more complete text handler for PLEOS
    public:

        // Creates and returns a word
        virtual std::shared_ptr<Text_Image_Block> __create_block(std::shared_ptr<scls::Block_Datas> needed_datas){std::shared_ptr<Text> to_return = new_text_image_block<Text>(defined_balises_shared_ptr(), needed_datas);return to_return;};

        // Generates a piece of math in the block
        virtual std::shared_ptr<scls::__Math_Part_Image> __generate_maths_one_balise(std::string needed_balise_name, int& bottom_offset, std::shared_ptr<scls::__XML_Text_Base> content, scls::Text_Style current_style, int& needed_height, int& needed_middle_bottom_offset, int& needed_middle_top_offset, std::vector<std::shared_ptr<scls::__Math_Part_Image>>& needed_parts, int& needed_width, int& top_offset, scls::Text_Image_Block* block);

        // Creates and returns a block
        virtual std::shared_ptr<scls::Text_Image_Block> __generate_block(std::shared_ptr<scls::Block_Datas> block_datas);

        // Loads the needed balises
        void load_balises(std::shared_ptr<scls::_Balise_Style_Container> defined_balises);
    protected:
        // Text constructor
        TEXT_IMAGE_BLOCK_CHILD_CONSTRUCTOR(Text, load_balises(defined_balises););
    };

    // Final Table
    class Table : public __Table_Case {
    public:
        // Table constructor
        Table():__Table_Case(){};

        // Set the value of an std::string case
        virtual std::shared_ptr<scls::__Image_Base> case_image_from_text(std::string value, scls::Text_Style needed_style, scls::Text_Image_Generator* tig);
    protected:
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
            virtual void update_texture(std::shared_ptr<scls::Text_Image_Block> block_to_apply, scls::Image_Generation_Type generation_type);

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

    // Returns the image of a text
    scls::Image string_to_image(std::string content);
    scls::Image string_to_image(std::string content, scls::Text_Style style);
}

#endif //PLEOS_TEXT
