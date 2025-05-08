//******************
//
// pleos_text.cpp
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
// This file contains the source code of "pleos_text.h".
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

// Include "pleos_text.cpp"
#include "../pleos_text.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //*********
	//
	// PLEOS Text handler
	//
	//*********

	// Creates and returns a graphic from an std::string
	void graphic_from_xml(Graphic& graphic, std::shared_ptr<scls::XML_Text> xml, scls::Text_Style needed_style, int& graphic_width_in_pixel, int& graphic_height_in_pixel) {
	    graphic.style()->set_border_width(1);
	    graphic_height_in_pixel = 200;bool graphic_height_in_pixel_used = false;
        graphic_width_in_pixel = 200;bool graphic_width_in_pixel_used = false;
        bool use_ratio_base = false;
        std::vector<scls::XML_Attribute>& attributes = xml.get()->xml_balise_attributes();
        for(int j = 0;j<static_cast<int>(attributes.size());j++) {
            if(attributes[j].name == "height") {graphic_height_in_pixel = scls::Fraction::from_std_string(attributes[j].value).to_double();graphic_height_in_pixel_used = true;}
            else if(attributes[j].name == "ratio_base") {use_ratio_base = true;}
            else if(attributes[j].name == "width") {graphic_width_in_pixel = scls::Fraction::from_std_string(attributes[j].value).to_double();graphic_width_in_pixel_used = true;}
        }

        // Get the datas about the graphic
        scls::Fraction graphic_height = 10;scls::Fraction graphic_width = 10;
        scls::Fraction graphic_x = 0;scls::Fraction graphic_y = 0;
        std::shared_ptr<scls::Text_Style> text_style = std::make_shared<scls::Text_Style>();
        // Handle a lot of balises
        for(int i = 0;i<static_cast<int>(xml->sub_texts().size());i++) {
            std::string balise_content = xml->sub_texts()[i].get()->xml_balise();
            std::string current_balise_name = xml->sub_texts()[i].get()->xml_balise_name();
            std::vector<scls::XML_Attribute>& attributes = xml->sub_texts()[i].get()->xml_balise_attributes();
            if(current_balise_name == "arrow" || current_balise_name == "arrow_hat") {
                // Get the datas about an arrow of the graphic
                double needed_angle = 0;
                std::string needed_name = std::string();scls::Fraction needed_x_end = 0;scls::Fraction needed_y_end = 0;
                scls::Fraction needed_x_start = 0;scls::Fraction needed_y_start = 0;
                for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                    if(attributes[j].name == "angle") {needed_angle = scls::string_to_double(attributes[j].value);}
                    else if(attributes[j].name == "name") {needed_name = attributes[j].value;}
                    else if(attributes[j].name == "x_end") {needed_x_end = scls::Fraction::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "y_end") {needed_y_end = scls::Fraction::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "x" || attributes[j].name == "x_start") {needed_x_start = scls::Fraction::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "y" || attributes[j].name == "y_start") {needed_y_start = scls::Fraction::from_std_string(attributes[j].value);}
                }
                // Add the arrow
                if(needed_angle != 0){
                    // Apply an angle if needed
                    scls::Vector_3D base_vector = scls::Vector_3D(1, 0, 0);base_vector.rotate(scls::Vector_3D(0, needed_angle, 0));
                    needed_x_end = needed_x_start + scls::Fraction::from_double(base_vector.x());needed_y_end = needed_y_start + scls::Fraction::from_double(base_vector.z());
                }
                Vector needed_vector = Vector(needed_name, needed_x_start, needed_y_start);
                needed_vector.set_x_end(needed_x_end);needed_vector.set_y_end(needed_y_end);
                needed_vector.set_type(Vector_Type::VT_Arrow);
                if(current_balise_name == "arrow_hat"){needed_vector.set_drawing_proportion(0);}
                graphic.add_vector(needed_vector);
            }
            else if(current_balise_name == "background_color") {graphic.set_background_color(scls::Color::from_xml(xml->sub_texts()[i]));}
            else if(current_balise_name == "base") {
                // Get the datas about the base of the graphic
                for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                    if(attributes[j].name == "height") {graphic_height = scls::Fraction::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "width") {graphic_width = scls::Fraction::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "x") {graphic_x = scls::Fraction::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "y") {graphic_y = scls::Fraction::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "draw") {if(attributes[j].value == "false" || attributes[j].value == "0"){graphic.set_draw_base(false);graphic.set_draw_sub_bases(false);}}
                }
            }
            else if(current_balise_name == "border") {scls::border_from_xml(xml->sub_texts()[i], graphic.style());}
            else if(current_balise_name == "circle") {
                // Get the datas about a circle of the graphic
                scls::Color border_color = scls::Color(0, 0, 0);scls::Fraction border_radius=5;scls::Color color = scls::Color(255, 255, 255);
                std::string needed_name = std::string();scls::Fraction needed_x = 0;scls::Fraction needed_y = 0;scls::Fraction radius = 1;
                for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                    if(attributes[j].name == "border_radius" || attributes[j].name == "width") {border_radius = scls::Fraction::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "color") {color = scls::Color::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "name") {needed_name = attributes[j].value;}
                    else if(attributes[j].name == "radius") {radius = scls::Fraction::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "x") {needed_x = scls::Fraction::from_std_string(attributes[j].value).to_double();}
                    else if(attributes[j].name == "y") {needed_y = scls::Fraction::from_std_string(attributes[j].value).to_double();}
                }
                // Add the circle
                std::shared_ptr<Circle> circle = *graphic.add_circle(needed_name, Vector(needed_name + std::string("-center") , needed_x, needed_y), radius);
                circle.get()->set_border_color(border_color);circle.get()->set_border_radius(border_radius.to_double());circle.get()->set_color(color);
            }
            else if(current_balise_name == "form") {
                // Get the datas about a vector of the graphic
                scls::Color border_color = scls::Color(255, 0, 0);scls::Fraction border_radius=5;
                scls::Color color = scls::Color(0, 255, 0);
                std::string needed_name = std::string();std::string needed_points = std::string();
                for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                    if(attributes[j].name == "border_color") {border_color = scls::Color::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "border_radius") {border_radius = scls::Fraction::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "color") {color = scls::Color::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "name") {needed_name = attributes[j].value;}
                    else if(attributes[j].name == "points") {needed_points = attributes[j].value;}
                }
                // Add the form
                std::shared_ptr<Form_2D> created_form = graphic.new_form(needed_name, needed_points);
                created_form.get()->set_border_color(border_color);created_form.get()->set_border_radius(border_radius.to_double());
                created_form.get()->set_color(color);
            }
            else if(current_balise_name == "fun" || current_balise_name == "function") {
                // Get the datas about a function of the graphic
                scls::Color needed_color = scls::Color(255, 0, 0);
                std::string needed_expression = std::string();
                std::string needed_name = std::string("f");
                std::string needed_unknown = std::string("x");
                for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                    if(attributes[j].name == "border_color" || attributes[j].name == "color") {needed_color = scls::Color::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "expression") {needed_expression = attributes[j].value;}
                    else if(attributes[j].name == "name") {needed_name = attributes[j].value;}
                }

                // Create the function
                std::shared_ptr<Function_Studied> needed_function = std::make_shared<Function_Studied>();
                std::shared_ptr<Graphic::Graphic_Function> fun = graphic.add_function(needed_function);
                needed_function.get()->set_color(needed_color);

                // Check the inner balises
                std::vector<std::shared_ptr<scls::XML_Text>>& sub_texts = xml->sub_texts()[i].get()->sub_texts();
                for(int j = 0;j<static_cast<int>(sub_texts.size());j++) {
                    std::vector<scls::XML_Attribute>& sub_texts_attributes = sub_texts[j].get()->xml_balise_attributes();
                    std::string sub_balise_content = sub_texts[j].get()->xml_balise();
                    std::string sub_balise_name = sub_texts[j].get()->xml_balise_name();

                    // Check a lot of attributes
                    if(sub_balise_name == std::string("curve_area")){
                        // Get a curve area
                        scls::Fraction area_end = 1;scls::Fraction area_start = 0;int rectangle_number = 10;
                        for(int k = 0;k<static_cast<int>(sub_texts_attributes.size());k++) {
                            if(sub_texts_attributes[k].name == "area_end") {area_end = scls::Fraction::from_std_string(sub_texts_attributes[k].value);}
                            else if(sub_texts_attributes[k].name == "area_start") {area_start = scls::Fraction::from_std_string(sub_texts_attributes[k].value);}
                            else if(sub_texts_attributes[k].name == "number" || sub_texts_attributes[k].name == "rectangle_number") {rectangle_number = std::stoi(sub_texts_attributes[k].value);}
                        }
                        fun.get()->add_curve_area(area_start, area_end, rectangle_number);
                    }
                }

                // Add the function
                needed_function.get()->set_formula(scls::string_to_formula(needed_expression));
                needed_function.get()->set_name(needed_name);
                function_definition_set(needed_function.get(), 0, 0);
            }
            else if(current_balise_name == "histogram" || current_balise_name == "point_cloud" || current_balise_name == "point_cloud_linked") {
                // Get the datas about a histogram in the graphic
                Graphic::Datas_Set::Datas_Set_Display_Type needed_display_type = Graphic::Datas_Set::Datas_Set_Display_Type::DSDT_Value;
                scls::Fraction needed_max = 0;bool max_used = false;
                scls::Fraction needed_min = 0;bool min_used = false;
                std::string needed_name = std::string();scls::Fraction needed_x = 0;scls::Fraction needed_y = 0;
                scls::Fraction needed_height = 10;scls::Fraction needed_width = 10;
                for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                    if(attributes[j].name == "display") {if(attributes[j].value == std::string("average")){needed_display_type=Graphic::Datas_Set::Datas_Set_Display_Type::DSDT_Average;}}
                    else if(attributes[j].name == "height") {needed_height = scls::Fraction::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "max") {needed_max = scls::Fraction::from_std_string(attributes[j].value);max_used=true;}
                    else if(attributes[j].name == "min") {needed_min = scls::Fraction::from_std_string(attributes[j].value);min_used=true;}
                    else if(attributes[j].name == "name") {needed_name = attributes[j].value;}
                    else if(attributes[j].name == "width") {needed_width = scls::Fraction::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "x") {needed_x = scls::Fraction::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "y") {needed_y = scls::Fraction::from_std_string(attributes[j].value);}
                }

                // Add and configure the datas set
                std::shared_ptr<Graphic::Datas_Set> created_datas_set = graphic.new_datas_set(needed_name);
                created_datas_set.get()->set_display_type(needed_display_type);
                if(max_used){created_datas_set.get()->set_fixed_max(needed_max);}
                if(min_used){created_datas_set.get()->set_fixed_min(needed_min);}
                if(current_balise_name == "histogram"){created_datas_set.get()->set_type(Graphic::Datas_Set::Datas_Set_Type::DST_Histogram);}
                else{created_datas_set.get()->set_type(Graphic::Datas_Set::Datas_Set_Type::DST_Point_Cloud);}
                created_datas_set.get()->set_height(needed_height);created_datas_set.get()->set_width(needed_width);
                created_datas_set.get()->set_x(needed_x);created_datas_set.get()->set_y(needed_y);

                // Handle the balises
                std::vector<std::shared_ptr<scls::XML_Text>> sub_texts = xml->sub_texts()[i].get()->sub_texts();
                for(int j = 0;j<static_cast<int>(sub_texts.size());j++) {
                    std::string balise_content = sub_texts[j].get()->xml_balise();
                    std::string current_balise_name = sub_texts[j].get()->xml_balise_name();
                    std::vector<scls::XML_Attribute>& attributes = sub_texts[j].get()->xml_balise_attributes();
                    if(current_balise_name == "data"){
                        int number = 1;scls::Fraction value = 0;
                        for(int k = 0;k<static_cast<int>(attributes.size());k++) {
                            if(attributes[k].name == "number") {number = std::stoi(attributes[k].value);}
                            else if(attributes[k].name == "value") {value = scls::Fraction::from_std_string(attributes[k].value);}
                        }
                        for(int k = 0;k<number;k++){created_datas_set.get()->add_data(value);}
                    }
                    else if(current_balise_name == "data_random"){
                        int number = 1;scls::Fraction value_min = 0;scls::Fraction value_max = 5;
                        for(int k = 0;k<static_cast<int>(attributes.size());k++) {
                            if(attributes[k].name == "number") {number = std::stoi(attributes[k].value);}
                            else if(attributes[k].name == "min") {value_min = scls::Fraction::from_std_string(attributes[k].value);}
                            else if(attributes[k].name == "max") {value_max = scls::Fraction::from_std_string(attributes[k].value);}
                        }
                        for(int k = 0;k<number;k++){scls::Fraction value = value_min + (rand()%static_cast<int>((value_max - value_min).to_double()));created_datas_set.get()->add_data(value);}
                    }
                }
            }
            else if(current_balise_name == "line") {
                // Get the datas about a vector of the graphic
                scls::Color border_color = scls::Color(255, 0, 0);scls::Fraction border_radius=5;
                std::string needed_name = std::string();
                scls::Fraction x_1 = 0;scls::Fraction x_2 = 0;scls::Fraction y_1 = 0;scls::Fraction y_2 = 0;
                for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                    if(attributes[j].name == "border_color" || attributes[j].name == "color") {border_color = scls::Color::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "border_radius" || attributes[j].name == "width") {border_radius = scls::Fraction::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "name") {needed_name = attributes[j].value;}
                    else if(attributes[j].name == "x_1") {x_1 = scls::Fraction::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "x_2") {x_2 = scls::Fraction::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "y_1") {y_1 = scls::Fraction::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "y_2") {y_2 = scls::Fraction::from_std_string(attributes[j].value);}
                }
                // Add the form
                std::shared_ptr<Form_2D> created_form = graphic.new_line(needed_name, x_1, y_1, x_2, y_2);
                created_form.get()->set_border_color(border_color);created_form.get()->set_border_radius(border_radius.to_double());
            }
            else if(current_balise_name == "point" || current_balise_name == "vec" || current_balise_name == "vector") {
                // Get the datas about a vector of the graphic
                std::string needed_name = std::string();scls::Fraction needed_x = 0;scls::Fraction needed_y = 0;
                for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                    if(attributes[j].name == "name") {needed_name = attributes[j].value;}
                    else if(attributes[j].name == "x") {
                        // X of the vector
                        needed_x = scls::Fraction::from_std_string(attributes[j].value).to_double();
                    }
                    else if(attributes[j].name == "y") {
                        // Y of the vector
                        needed_y = scls::Fraction::from_std_string(attributes[j].value).to_double();
                    }
                }
                // Add the vector
                if(current_balise_name == "point"){graphic.add_point(Vector(needed_name, needed_x, needed_y));}
                else if(current_balise_name == "vec" || current_balise_name == "vector"){graphic.add_vector(Vector(needed_name, needed_x, needed_y));}
            }
            else if(current_balise_name == "random_object") {
                // Create random objects
                enum Needed_Object {NO_Circle};
                scls::Color border_color = scls::Color(0, 0, 0);scls::Fraction border_radius=2;scls::Color color = scls::Color(255, 255, 255);
                std::string needed_name = std::string("random_object");
                scls::Fraction needed_max_x = 0;scls::Fraction needed_max_y = 0;
                scls::Fraction needed_min_x = 0;scls::Fraction needed_min_y = 0;
                Needed_Object needed_object = NO_Circle;
                scls::Fraction needed_radius = 1;
                int number = 1;
                for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                    if(attributes[j].name == "name") {needed_name = attributes[j].value;}
                    else if(attributes[j].name == "max_x") {needed_max_x = scls::Fraction::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "max_y") {needed_max_y = scls::Fraction::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "min_x") {needed_min_x = scls::Fraction::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "min_y") {needed_min_y = scls::Fraction::from_std_string(attributes[j].value);}
                    else if(attributes[j].name == "number") {number = std::stoi(attributes[j].value);}
                    else if(attributes[j].name == "radius") {needed_radius = scls::Fraction::from_std_string(attributes[j].value);}
                }

                // Add the objects
                if(needed_object == Needed_Object::NO_Circle) {
                    for(int j = 0;j<number;j++) {
                        scls::Fraction random_x = scls::random_fraction(needed_min_x, needed_max_x);
                        scls::Fraction random_y = scls::random_fraction(needed_min_y, needed_max_y);
                        std::shared_ptr<Circle> new_circle = graphic.new_circle(needed_name + std::to_string(j), random_x, random_y, needed_radius);
                        new_circle.get()->set_border_color(border_color);new_circle.get()->set_border_radius(border_radius.to_double());new_circle.get()->set_color(color);
                    }
                }
            }
            else if(current_balise_name == "text") {
                // Get the datas about a text of the graphic
                std::string needed_content = std::string();scls::Fraction needed_x = 0;scls::Fraction needed_y = 0;scls::Fraction radius = 1;
                for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                    if(attributes[j].name == "content") {needed_content = attributes[j].value;}
                    else if(attributes[j].name == "x") {needed_x = scls::Fraction::from_std_string(attributes[j].value).to_double();}
                    else if(attributes[j].name == "y") {needed_y = scls::Fraction::from_std_string(attributes[j].value).to_double();}
                }
                // Add the text
                graphic.new_text(needed_content, Vector(std::string("text-center") , needed_x, needed_y), text_style);
            }
            else if(current_balise_name == "text_style") {
                // Get the datas about the style text of the graphic
                std::string needed_content = std::string();scls::Fraction needed_x = 0;scls::Fraction needed_y = 0;scls::Fraction radius = 1;
                for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                    if(attributes[j].name == "background_color") {text_style.get()->set_background_color(scls::Color::from_std_string(attributes[j].value));}
                    else if(attributes[j].name == "font_size") {text_style.get()->set_font_size(scls::Fraction::from_std_string(attributes[j].value).to_double());}
                }
            }
        }
        // Set the datas
        graphic.set_middle(graphic_x.to_double(), graphic_y.to_double());
        graphic.set_scale(graphic_width.to_double(), graphic_height.to_double());

        // Get the image
        if(use_ratio_base){
            if(graphic_height_in_pixel_used&&!graphic_width_in_pixel_used){graphic_width_in_pixel = static_cast<double>(graphic_height_in_pixel) * (graphic_width.to_double() / graphic_height.to_double());}
            else if(!graphic_height_in_pixel_used&&graphic_width_in_pixel_used){graphic_height_in_pixel = static_cast<double>(graphic_width_in_pixel) * (graphic_height.to_double() / graphic_width.to_double());}
        }
	}
	void graphic_from_xml(std::shared_ptr<Graphic> graphic_shared_ptr, std::shared_ptr<scls::XML_Text> xml, scls::Text_Style needed_style, int& graphic_width_in_pixel, int& graphic_height_in_pixel){graphic_from_xml(*graphic_shared_ptr.get(), xml, needed_style, graphic_width_in_pixel, graphic_height_in_pixel);}
	std::shared_ptr<Graphic> graphic_from_xml(std::shared_ptr<scls::XML_Text> xml, scls::Text_Style needed_style, int& graphic_width_in_pixel, int& graphic_height_in_pixel) {std::shared_ptr<Graphic> to_return = std::make_shared<Graphic>();graphic_from_xml(to_return, xml, needed_style, graphic_width_in_pixel, graphic_height_in_pixel);return to_return;}
	std::shared_ptr<scls::Image> graphic_image_from_xml(std::shared_ptr<scls::XML_Text> xml, scls::Text_Style needed_style){
	    int graphic_width_in_pixel = 0;int graphic_height_in_pixel = 0;
	    std::shared_ptr<scls::Image> to_return = graphic_from_xml(xml, needed_style, graphic_width_in_pixel, graphic_height_in_pixel).get()->to_image(graphic_width_in_pixel, graphic_height_in_pixel);
        return to_return;
	}

	// Creates and returns a linked-list from an std::string
	std::shared_ptr<Linked_List<std::string>> linked_list_from_xml(std::shared_ptr<scls::XML_Text> xml, scls::Text_Style needed_style) {
	    std::shared_ptr<Linked_List<std::string>> to_return;

	    // Handle a lot of balises
	    std::shared_ptr<Linked_List<std::string>> current_list;
        for(int i = 0;i<static_cast<int>(xml->sub_texts().size());i++){
            std::string current_balise_name = xml->sub_texts()[i].get()->xml_balise_name();
            std::vector<scls::XML_Attribute>& attributes = xml->sub_texts()[i].get()->xml_balise_attributes();
            if(current_balise_name == "case" || current_balise_name == "element"){
                std::string content = std::string();
                bool ignore_for_placement = false;
                scls::Fraction x = 0;bool x_used = false;
                scls::Fraction y = 0;bool y_used = false;
                for(int i = 0;i<static_cast<int>(attributes.size());i++) {
                    if(attributes[i].name == std::string("content")){content = attributes[i].value;}
                    else if(attributes[i].name == std::string("ignore_for_placement")){ignore_for_placement=true;}
                    else if(attributes[i].name == std::string("x")){x = scls::Fraction::from_std_string(attributes[i].value);x_used=true;}
                    else if(attributes[i].name == std::string("y")){y = scls::Fraction::from_std_string(attributes[i].value);y_used=true;}
                }
                if(to_return.get() == 0) {to_return = std::make_shared<Linked_List<std::string>>();to_return.get()->set_value(content);current_list = to_return;}
                else{current_list = current_list.get()->add_child(content);}
                // Configurate the node
                current_list.get()->set_ignore_for_placement(ignore_for_placement);
                if(x_used){current_list.get()->root()->set_x(x);}
                if(y_used){current_list.get()->set_y(y);}
            }
        }

        // Return the result
        return to_return;
	}

	// Creates and returns a table from an std::string
	std::shared_ptr<Table> table_from_xml(std::shared_ptr<scls::XML_Text> xml, scls::Text_Style needed_style) {
	    std::shared_ptr<Table> to_return = std::make_shared<Table>();
	    scls::Text_Image_Generator tig;

	    // Handle the attributes
	    std::vector<scls::XML_Attribute>& attributes = xml.get()->xml_balise_attributes();
	    for(int i = 0;i<static_cast<int>(attributes.size());i++) {
            if(attributes[i].name == std::string("font_size")){needed_style.set_font_size(std::stoi(attributes[i].value));}
            else if(attributes[i].name == std::string("minimum_case_width")){to_return.get()->set_minimum_case_width(std::stoi(attributes[i].value));}
            else if(attributes[i].name == std::string("title")){to_return.get()->set_title(attributes[i].value);}
	    }

	    // Handle a lot of balises
        for(int i = 0;i<static_cast<int>(xml->sub_texts().size());i++){
            std::string current_balise_name = xml->sub_texts()[i].get()->xml_balise_name();
            std::vector<scls::XML_Attribute>& attributes = xml->sub_texts()[i].get()->xml_balise_attributes();
            if(current_balise_name == "case" || current_balise_name == "case_plus"){
                scls::Color background_color = scls::Color(255, 255, 255);std::string content = std::string();
                scls::Text_Style case_style = needed_style;case_style.set_border_width(0);
                int height = 1;int width = 1;bool right_border = true;int x = 0;int y = 0;
                for(int i = 0;i<static_cast<int>(attributes.size());i++) {
                    if(!scls::text_style_from_xml_attribute(&attributes[i], &case_style)) {
                        if(attributes[i].name == std::string("background_color")){background_color = scls::Color::from_std_string(attributes[i].value);}
                        else if(attributes[i].name == std::string("content")){content = attributes[i].value;}
                        else if(attributes[i].name == std::string("height")){height = std::stoi(attributes[i].value);}
                        else if(attributes[i].name == std::string("right_border")){if(attributes[i].value==std::string("0")||attributes[i].value==std::string("false")||attributes[i].value==std::string("no")){right_border=false;}}
                        else if(attributes[i].name == std::string("width")){width = std::stoi(attributes[i].value);}
                        else if(attributes[i].name == std::string("x")){x = std::stoi(attributes[i].value);}
                        else if(attributes[i].name == std::string("y")){y = std::stoi(attributes[i].value);}
                    }
                }

                // Create the result
                if(current_balise_name == "case_plus"){
                    content = xml->sub_texts()[i].get()->text();
                    if(case_style.max_width == -1){case_style.max_width = to_return.get()->column_width(x, width);}
                    (*to_return.get()->case_at(x, y)->image.get()) = tig.image_shared_ptr<Text>(content, case_style);
                }
                else{(*to_return.get()->case_at(x, y)->image.get()) = tig.image_shared_ptr(content, case_style);}
                to_return.get()->case_at(x, y)->right_border = right_border;
                to_return.get()->case_at(x, y)->set_background_color(background_color);
                to_return.get()->merge_cases(x, y, width, height);
            }
        }

        // Return the result
        return to_return;
	}

	//Add a node to the graph
	void __graph_add_node(std::shared_ptr<scls::XML_Text> current_text, scls::Text_Style needed_style, Graph<std::string>* graph) {
        // Handle the attributes
        std::string to_add = std::string();
        scls::Fraction x = 0; scls::Fraction y = 0;
        for(int i = 0;i<static_cast<int>(current_text.get()->xml_attributes().size());i++) {
            if(current_text.get()->xml_attributes()[i].name == std::string("name")){to_add = current_text.get()->xml_attributes()[i].value;}
            else if(current_text.get()->xml_attributes()[i].name == std::string("x")){x = scls::Fraction::from_std_string(current_text.get()->xml_attributes()[i].value);}
            else if(current_text.get()->xml_attributes()[i].name == std::string("y")){y = scls::Fraction::from_std_string(current_text.get()->xml_attributes()[i].value);}
        }
        int needed_node = graph->add_node(to_add, x, y);

        // Handle a lot of balises
        for(int i = 0;i<static_cast<int>(current_text->sub_texts().size());i++){
            std::string balise_content = current_text->sub_texts()[i].get()->xml_balise();
            std::string current_balise_name = current_text->sub_texts()[i].get()->xml_balise_name();
            std::vector<scls::XML_Attribute>& attributes = current_text->sub_texts()[i].get()->xml_balise_attributes();
            if(current_balise_name == "link"){
                // Link the node
                int needed_id = 0;std::string needed_ponderation = std::string();
                for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                    std::string attribute_name = attributes[j].name;
                    std::string attribute_value = attributes[j].value;
                    if(attribute_name == "id") {needed_id = std::stoi(attribute_value);}
                    else if(attribute_name == "value") {needed_ponderation = attribute_value;}
                }

                // Add the link
                if(needed_id == needed_node){scls::print(std::string("PLEOS Graph"), std::string("Can't link the ") + std::to_string(needed_id) + std::string(" with himself."));}
                else {
                    graph->link_nodes(needed_node, needed_id);
                    graph->set_link_ponderation(needed_node, needed_id, needed_ponderation, needed_style);
                }
            }
        }
	}
	// Add datas to a tree
	void __tree_stern_brocot(int depht, std::shared_ptr<std::vector<int>> numerator_ptr, std::shared_ptr<std::vector<int>> denominator_ptr, std::shared_ptr<std::vector<Tree<std::string>*>> trees){
	    // Create the first nodes
	    std::shared_ptr<std::vector<int>> new_denominator = std::make_shared<std::vector<int>>();
	    std::shared_ptr<std::vector<int>> new_numerator = std::make_shared<std::vector<int>>();
	    std::shared_ptr<std::vector<Tree<std::string>*>> new_trees = std::make_shared<std::vector<Tree<std::string>*>>();
	    // First fraction first node
	    int current_denominator = denominator_ptr->at(0) + 1;new_denominator.get()->push_back(current_denominator);
	    int current_numerator = numerator_ptr->at(0);new_numerator.get()->push_back(current_numerator);
	    new_trees.get()->push_back(trees.get()->at(0)->add_node(std::to_string(current_numerator) + std::string("/") + std::to_string(current_denominator)));
	    new_denominator.get()->push_back(denominator_ptr->at(0));new_numerator.get()->push_back(numerator_ptr->at(0));
	    if(trees.get()->size() == 1) {
            // First fraction last node
            current_denominator = denominator_ptr->at(0);new_denominator.get()->push_back(current_denominator);
            current_numerator = numerator_ptr->at(0) + 1;new_numerator.get()->push_back(current_numerator);
            new_trees.get()->push_back(trees.get()->at(0)->add_node(std::to_string(current_numerator) + std::string("/") + std::to_string(current_denominator)));
	    }
	    else {
            // First fraction last node
            current_denominator = denominator_ptr->at(0) + denominator_ptr->at(1);new_denominator.get()->push_back(current_denominator);
            current_numerator = numerator_ptr->at(0) + numerator_ptr->at(1);new_numerator.get()->push_back(current_numerator);
            new_trees.get()->push_back(trees.get()->at(0)->add_node(std::to_string(current_numerator) + std::string("/") + std::to_string(current_denominator)));
            new_denominator.get()->push_back(denominator_ptr->at(1));new_numerator.get()->push_back(numerator_ptr->at(1));

            // Create each nodes
            for(int i = 1;i<static_cast<int>(trees.get()->size()) - 1;i++) {
                // Left
                current_denominator = denominator_ptr->at(i * 2 - 1) + denominator_ptr->at(i * 2);new_denominator.get()->push_back(current_denominator);
                current_numerator = numerator_ptr->at(i * 2 - 1) + numerator_ptr->at(i * 2);new_numerator.get()->push_back(current_numerator);
                new_trees.get()->push_back(trees.get()->at(i)->add_node(std::to_string(current_numerator) + std::string("/") + std::to_string(current_denominator)));
                new_denominator.get()->push_back(denominator_ptr->at(i * 2));new_numerator.get()->push_back(numerator_ptr->at(i * 2));
                // Right
                current_denominator = denominator_ptr->at(i * 2) + denominator_ptr->at(i * 2 + 1);new_denominator.get()->push_back(current_denominator);
                current_numerator = numerator_ptr->at(i * 2) + numerator_ptr->at(i * 2 + 1);new_numerator.get()->push_back(current_numerator);
                new_trees.get()->push_back(trees.get()->at(i)->add_node(std::to_string(current_numerator) + std::string("/") + std::to_string(current_denominator)));
                new_denominator.get()->push_back(denominator_ptr->at(i * 2 + 1));new_numerator.get()->push_back(numerator_ptr->at(i * 2 + 1));
            }

            // Create the last fraction first node
            current_denominator = denominator_ptr->at(denominator_ptr.get()->size() - 2) + denominator_ptr->at(denominator_ptr.get()->size() - 1);new_denominator.get()->push_back(current_denominator);
            current_numerator = numerator_ptr->at(numerator_ptr.get()->size() - 2) + numerator_ptr->at(numerator_ptr.get()->size() - 1);new_numerator.get()->push_back(current_numerator);
            new_trees.get()->push_back(trees.get()->at(trees.get()->size() - 1)->add_node(std::to_string(current_numerator) + std::string("/") + std::to_string(current_denominator)));
            new_denominator.get()->push_back(denominator_ptr->at(denominator_ptr.get()->size() - 1));new_numerator.get()->push_back(numerator_ptr.get()->at(numerator_ptr.get()->size() - 1));
            // Create the last fraction last node
            current_denominator = denominator_ptr->at(denominator_ptr.get()->size() - 1);new_denominator.get()->push_back(current_denominator);
            current_numerator = numerator_ptr->at(numerator_ptr.get()->size() - 1) + 1;new_numerator.get()->push_back(current_numerator);
            new_trees.get()->push_back(trees.get()->at(trees.get()->size() - 1)->add_node(std::to_string(current_numerator) + std::string("/") + std::to_string(current_denominator)));
	    }

	    // Recursion
        if(depht > 0) {__tree_stern_brocot(depht - 1, new_numerator, new_denominator, new_trees);}
	}
	void __tree_stern_brocot(Tree<std::string>* tree, int depht){
	    std::shared_ptr<std::vector<int>> denominator = std::make_shared<std::vector<int>>(1, 1);std::shared_ptr<std::vector<int>> numerator = std::make_shared<std::vector<int>>(1, 1);
	    __tree_stern_brocot(depht, numerator, denominator, std::make_shared<std::vector<Tree<std::string>*>>(1, tree));
    };
	void __tree_add_datas(std::shared_ptr<scls::XML_Text> current_text, scls::Text_Style needed_style, Tree<std::string>* tree){
        // Handle the attributes
        std::string to_add = std::string();std::string to_load = std::string();
        for(int i = 0;i<static_cast<int>(current_text.get()->xml_attributes().size());i++) {
            if(current_text.get()->xml_attributes()[i].name == std::string("load")){to_load = current_text.get()->xml_attributes()[i].value;}
            else if(current_text.get()->xml_attributes()[i].name == std::string("name")){to_add = current_text.get()->xml_attributes()[i].value;}
        }

        // Loads a precise tree
        if(to_load.size() >= 12 && to_load.substr(0, 12) == std::string("stern_brocot")) {
            // Get the needed value
            to_load = to_load.substr(12, to_load.size() - 12);
            while(to_load.size() > 0 && to_load[0] == '('){to_load = to_load.substr(1, to_load.size() - 1);}
            while(to_load.size() > 0 && to_load[to_load.size() - 1] == ')'){to_load = to_load.substr(0, to_load.size() - 1);}
            int depht = 1;if(to_load.size() > 0){depht = scls::string_to_double(to_load);};
            if(depht < 0){depht = 1;}

            // Create the tree
            if(to_add == std::string()){to_add = std::string("0");}
            __tree_stern_brocot(tree, depht - 1);
        }

        if(to_add == std::string() && current_text->sub_texts().size() == 1){__tree_add_datas(current_text->sub_texts()[0], needed_style, tree);}
        else {
            // Set the name
            tree->root()->set_style(needed_style);
            tree->set_value(to_add);

            // Handle a lot of balises
            for(int i = 0;i<static_cast<int>(current_text->sub_texts().size());i++){
                std::string current_balise_name = current_text->sub_texts()[i].get()->xml_balise_name();
                std::vector<scls::XML_Attribute>& attributes = current_text->sub_texts()[i].get()->xml_balise_attributes();
                if(current_balise_name == "tree" || current_balise_name == "trees"){
                    // Add the node
                    __tree_add_datas(current_text->sub_texts()[i], needed_style, tree->add_node(std::string()));
                }
                else if(current_balise_name == "node"){
                    // Add the node
                    __graph_add_node(current_text->sub_texts()[i], needed_style, tree->graph());
                }
            }
        }
	}
	// Creates and returns a tree from an std::string
	std::shared_ptr<Tree<std::string>> tree_from_xml(std::shared_ptr<scls::XML_Text> xml, scls::Text_Style needed_style){std::shared_ptr<Tree<std::string>> tree = std::make_shared<Tree<std::string>>();__tree_add_datas(xml, needed_style, tree.get());return tree;}

	// Generate a word
	std::shared_ptr<scls::Image> generate_text_image(std::shared_ptr<scls::XML_Text> current_text, std::shared_ptr<scls::Text_Style> needed_style){
	    std::string balise_content = current_text.get()->xml_balise();
        std::string current_balise_name = current_text.get()->xml_balise_name();
        std::shared_ptr<scls::Image> to_return;

        // Handle style balises
        needed_style = needed_style.get()->new_child();
        for(int i = 0;i<static_cast<int>(current_text->sub_texts().size());i++){
            std::string balise_content = current_text->sub_texts()[i].get()->xml_balise();
            std::string current_balise_name = current_text->sub_texts()[i].get()->xml_balise_name();
            std::vector<scls::XML_Attribute>& attributes = current_text->sub_texts()[i].get()->xml_balise_attributes();
            if(current_balise_name == "border") {scls::border_from_xml(current_text->sub_texts()[i], needed_style.get());}
            else if(current_balise_name == "padding") {scls::padding_from_xml(current_text->sub_texts()[i], needed_style.get());}
        }

        // Create the image
        if(current_balise_name == "graphic") {to_return = graphic_image_from_xml(current_text, *needed_style.get());}
        else if(current_balise_name == "linked_list"){to_return = linked_list_from_xml(current_text, *needed_style.get()).get()->to_image(needed_style);}
        else if(current_balise_name == "table") {to_return = table_from_xml(current_text, *needed_style.get()).get()->to_image();}
        else if(current_balise_name == "tree") {to_return = tree_from_xml(current_text, *needed_style.get()).get()->to_image();}

        return to_return;
	}
    void __Text_Line::generate_word(std::shared_ptr<scls::XML_Text> current_text, unsigned int& current_position_in_plain_text, std::shared_ptr<scls::Text_Style> needed_style, std::shared_ptr<scls::Text_Image_Word>& word_to_add) {
        std::string balise_content = current_text.get()->xml_balise();
        std::string current_balise_name = current_text.get()->xml_balise_name();
        if(current_balise_name == "graphic" || current_balise_name == "linked_list" || current_balise_name == "table" || current_balise_name == "tree") {
            // Get the image
            std::shared_ptr<scls::Image> src_img = generate_text_image(current_text, needed_style);

            // Change the image
            int height = -1; int width = -1;
            if(height == -1 && width == -1) {height = src_img.get()->height();width = src_img.get()->width();}
            else if(height != -1 && width == -1) {
                width = static_cast<int>(static_cast<double>(height) * (static_cast<double>(src_img.get()->width()) / static_cast<double>(src_img.get()->height())));
            } else if(height == -1 && width != -1) {
                height = static_cast<int>(static_cast<double>(width) * (static_cast<double>(src_img.get()->height()) / static_cast<double>(src_img.get()->width())));
            }
            if(global_style()->max_width > 0 && width > global_style()->max_width) {
                width = global_style()->max_width;
                height = static_cast<int>(static_cast<double>(width) * (static_cast<double>(src_img.get()->height()) / static_cast<double>(src_img.get()->width())));
            }
            __generate_image(word_to_add, src_img, current_position_in_plain_text, a_current_width, height, width);
        }
        else{scls::Text_Image_Line::generate_word(current_text, current_position_in_plain_text, needed_style, word_to_add);}
    }

    // Loads the needed balises
    void Text::load_balises(std::shared_ptr<scls::_Balise_Style_Container> defined_balises) {
        std::shared_ptr<scls::Balise_Style_Datas> current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("case_plus", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("function", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("graph", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        current_balise.get()->style.get()->set_border_width(1);
        current_balise.get()->style.get()->set_margin_bottom(16);
        defined_balises.get()->set_defined_balise("graphic", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("histogram", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = false;
        defined_balises.get()->set_defined_balise("link", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("linked_list", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("node", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("nodes", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("point_cloud", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("point_cloud_linked", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        current_balise.get()->style.get()->set_margin_bottom(16);
        defined_balises.get()->set_defined_balise("table", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("tree", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises.get()->set_defined_balise("trees", current_balise);
    }

    // Creates a text block from a block of text
    std::shared_ptr<scls::__GUI_Text_Metadatas::__GUI_Text_Block> GUI_Text::__create_text_block_object(scls::Text_Image_Block* block_to_apply) {
        if(block_to_apply->datas()->content.get()->xml_balise_name() == "graphic"){return scls::__GUI_Text_Metadatas::__create_text_block_object<GUI_Text::__GUI_Text_Block_Graphic, Graphic_Object>();}
        return __GUI_Text_Metadatas::__create_text_block_object(block_to_apply);
    };

    // Updates the texture of the block
    void GUI_Text::__GUI_Text_Block_Graphic::update_texture(scls::Text_Image_Block* block_to_apply, scls::Image_Generation_Type generation_type){
        int height=0;int width=0;
        graphic()->set_style(*block_to_apply->global_style());
        graphic_from_xml(*graphic(), block_to_apply->datas()->content, *block_to_apply->global_style(), width, height);

        // Update the size
        graphic_object()->set_height_in_pixel(height);
        graphic_object()->set_width_in_pixel(width);

        // Update the texture
        graphic_object()->update_texture();
    };
}
