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

// Define a macro to easily add an object
#define GUI_PAGE(type, gui, gui_page, function, display_function, display_function_parent) private: std::shared_ptr<type> gui; \
public: inline type* function() const {return gui.get();} \
public: void display_function(){display_function_parent();set_current_page(gui_page);function()->set_visible(true);}
#define GUI_OBJECT_CREATION(type, gui, name) else if(object_name == name){gui = *parent->new_object<type>(object_name);return gui;}
#define GUI_OBJECT_SELECTION(display_function, name) else if(page == name){display_function;}

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //*********
	//
	// PLEOS Text handler
	//
	//*********

	// Creates and returns a tree from an std::string
	std::shared_ptr<Tree<std::string>> tree_from_xml(std::shared_ptr<scls::XML_Text> xml);
	inline std::shared_ptr<Tree<std::string>> tree_from_xml(std::string xml, std::shared_ptr<scls::__Balise_Container> balise){return tree_from_xml(scls::xml(balise, xml));};

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
        Text(std::shared_ptr<scls::_Balise_Style_Container> defined_balises, std::string text):scls::Text_Image_Multi_Block(defined_balises,text){
            std::shared_ptr<scls::Balise_Style_Datas> current_balise = std::make_shared<scls::Balise_Style_Datas>();
            current_balise.get()->has_content = true;
            defined_balises.get()->set_defined_balise("graph", current_balise);
            current_balise = std::make_shared<scls::Balise_Style_Datas>();
            current_balise.get()->has_content = true;
            defined_balises.get()->set_defined_balise("graphic", current_balise);
            current_balise = std::make_shared<scls::Balise_Style_Datas>();
            current_balise.get()->has_content = false;
            defined_balises.get()->set_defined_balise("link", current_balise);
            current_balise = std::make_shared<scls::Balise_Style_Datas>();
            current_balise.get()->has_content = true;
            defined_balises.get()->set_defined_balise("node", current_balise);
            current_balise = std::make_shared<scls::Balise_Style_Datas>();
            current_balise.get()->has_content = true;
            defined_balises.get()->set_defined_balise("nodes", current_balise);
            current_balise = std::make_shared<scls::Balise_Style_Datas>();
            current_balise.get()->has_content = true;
            defined_balises.get()->set_defined_balise("tree", current_balise);
            current_balise = std::make_shared<scls::Balise_Style_Datas>();
            current_balise.get()->has_content = true;
            defined_balises.get()->set_defined_balise("trees", current_balise);
        };

        // Creates and returns a __Text_Block
        virtual std::shared_ptr<scls::Text_Image_Block>__create_block(std::shared_ptr<scls::Block_Datas>needed_datas){return std::make_shared<__Text_Block>(defined_balises_shared_ptr(), needed_datas);};

    private:
    };
}

#endif //PLEOS_TEXT
