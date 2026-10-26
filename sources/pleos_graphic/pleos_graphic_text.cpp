//******************
//
// pleos_graphic_texture.h
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
// This file contains the source code of pleos_graphic.h for the Graphic_Text object.
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

// Include pleos_graphic.h
#include "../../pleos_graphic.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //******************
	// Graphic Text
	//******************

    // Graphic_Text constructor
    Graphic::Graphic_Text::Graphic_Text(std::weak_ptr<Graphic> graphic_base):Graphic_Base_Object(graphic_base){};

    // Draws the object on an image
    void Graphic::Graphic_Text::draw_on_image(std::shared_ptr<scls::__Image_Base> image) {
        if(a_text_image_generator.get() == 0){scls::print(std::string("PLEOS Graphic Text \"") + name() + std::string("\""), std::string("Badly created Graphic_Text, use the Graphic::new_text method indead."));return;}

        scls::Text_Style current_style = style().new_child();
        current_style.set_color(color_with_opacity(current_style.color()));
        current_style.set_background_color(color_with_opacity(current_style.background_color()));
        if(font_size_in_scale().to_double() > 0){current_style.set_font_size(round(static_cast<double>(pixel_by_case_y()) * font_size_in_scale().to_double()));}
        std::shared_ptr<scls::Text_Image_Block> image_block = a_text_image_generator.get()->new_text_image_block_shared_ptr(final_content(), current_style);
        std::shared_ptr<scls::__Image_Base> needed_image = image_block.get()->image_shared_pointer();

        // Handle transformations
        if(rotation() != 0){needed_image.get()->rotate(rotation().to_double());}
        double needed_x = graphic_x_to_pixel_x(absolute_x());
        double needed_y = graphic_y_to_pixel_y_inversed(absolute_y()) + image_block.get()->datas()->max_last_line_bottom_offset;
        image.get()->paste(needed_image.get(), needed_x - needed_image.get()->width() / 2, needed_y - needed_image.get()->height() / 2);
    }

    // Returns the final content in the object
    void Graphic::Graphic_Text::final_content(std::shared_ptr<scls::XML_Text_Base>& one_balise) {
        // Basic datas
        std::vector<scls::XML_Attribute>& attributes = one_balise.get()->xml_balise_attributes();

        if(one_balise.get()->xml_balise_name() == std::string("env")) {
            // Using the PLEOS environment
            std::string formula_name = std::string();
            std::string object_name = std::string();
            std::string parameter_name = std::string();
            std::string variable_name = std::string();

            bool fixed = false;int precision = 2;
            for(int j = 0;j<static_cast<int>(attributes.size());j++){
                if(attributes.at(j).name == std::string_view("fixed")){fixed = true;}
                else if(attributes.at(j).name == std::string_view("formula")){formula_name = attributes.at(j).value;}
                else if(attributes.at(j).name == std::string_view("object")){object_name = attributes.at(j).value;}
                else if(attributes.at(j).name == std::string_view("parameter")){parameter_name = attributes.at(j).value;}
                else if(attributes.at(j).name == std::string_view("precision")){precision = environment()->value_double(attributes.at(j).value);}
                else if(attributes.at(j).name == std::string_view("variable")){variable_name = attributes.at(j).value;}
            }

            // Prepare the datas
            std::string final_text;

            // Get the value
            if(!parameter_name.empty()) {
                // Format the value
                __Graphic_Object_Base* o = 0;
                if(object_name.empty()){o = this;}
                else{o = graphic()->object_by_name(object_name);}
                std::string needed_parameter = o->parameter(parameter_name);
                if(needed_parameter.empty()){needed_parameter = std::string("0");}
                double value = environment()->value_double(needed_parameter);
                final_text = scls::format_number_to_text(value, precision);
            }
            else if(!variable_name.empty()) {
                // Format the value
                __Graphic_Object_Base* o = 0;
                if(object_name.empty()){o = this;}
                else{o = graphic()->object_by_name(object_name);}
                double value = o->attached_namespace()->value_by_name(variable_name).to_double();
                final_text = scls::format_number_to_text(value, precision);
            }
            else {
                // Format the value
                double value = environment()->value_by_name(formula_name).to_double();
                final_text = scls::format_number_to_text(value, precision);
            }

            // Handle precision
            std::vector<std::string> final_text_cutted = scls::cut_string(final_text, std::string("."));
            if(precision == 0) {final_text = final_text_cutted.at(0);}
            else {
                if(final_text_cutted.size() >= 2){for(int i = 0;i<static_cast<int>(2 - final_text_cutted.at(1).size());i++){final_text += std::string("0");}}
                else{std::string to_add = std::string(".");for(int i = 0;i<precision;i++){to_add += std::string("0");}final_text+=to_add;}
            }

            // Set the text
            one_balise = scls::xml(window_struct()->balises_shared_ptr(), final_text);
        }
    }
    std::shared_ptr<scls::XML_Text_Base> Graphic::Graphic_Text::final_content() {
        if(graphic()->balises() == 0){std::shared_ptr<scls::XML_Text_Base> to_return = scls::xml(std::make_shared<scls::__Balise_Container>(), scls::to_utf_8_code_point(content()));return to_return;}

        // Create the text
        std::shared_ptr<scls::XML_Text_Base> to_return = scls::xml(graphic()->balises_shared_ptr(), scls::to_utf_8_code_point(content()));
        if(to_return.get()->sub_texts().size() == 0) {final_content(to_return);}
        else {
            to_return.get()->set_xml_balise_datas(graphic()->balises()->defined_balise_shared_ptr("p"));

            // Analyse each sub-text
            for(int i = 0;i<static_cast<int>(to_return.get()->sub_texts().size());i++) {final_content(to_return.get()->sub_texts()[i]);}
        }

        return to_return;
    }

    // Returns the name of the object
    std::string Graphic::Graphic_Text::to_displayed_text(){return std::string("texte");}
    std::string Graphic::Graphic_Text::to_xml_text(){return std::string("<") + to_xml_text_base() + to_xml_text_background_color() + to_xml_text_color() + to_xml_text_font_size() + to_xml_text_font_size_in_scale() + to_xml_text_alignment_horizontal() + std::string(">") + content() + std::string("</") + to_xml_text_object_name() + std::string(">");};
    std::string Graphic::Graphic_Text::to_xml_text_alignment_horizontal(){if(alignment_horizontal() == scls::Alignment_Horizontal::H_Left){return std::string();}return std::string(" alignment_horizontal=center");}
    std::string Graphic::Graphic_Text::to_xml_text_background_color(){return std::string(" background_color=") + background_color().to_std_string(0);}
    std::string Graphic::Graphic_Text::to_xml_text_color()const{return std::string(" color=") + color().to_std_string(0);}
    std::string Graphic::Graphic_Text::to_xml_text_font_size() {if(font_size_in_scale() > 0){return std::string();} return std::string(" font_size=") + std::to_string(style().font_size());}
    std::string Graphic::Graphic_Text::to_xml_text_font_size_in_scale() {if(font_size_in_scale() <= 0){return std::string();}return std::string(" font_size_in_scale=") + font_size_in_scale().to_std_string(0);}
    std::string Graphic::Graphic_Text::to_xml_text_object_name() {return std::string("text");}

}
