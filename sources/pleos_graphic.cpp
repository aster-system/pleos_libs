//******************
//
// pleos_graphic.h
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
// This file contains the source code of pleos_graphic.h.
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
#include "../pleos_graphic.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //******************
    //
    // Graphic sub-objects
    //
    //******************

    // Draws the object on an image
    void Graphic::Graphic_Texture::draw_on_image(std::shared_ptr<scls::__Image_Base> image){
        int needed_height = height_in_pixel();
        int needed_width = width_in_pixel();
        int needed_x = graphic_x_to_pixel_x(min_x());
        int needed_y = graphic_y_to_pixel_y_inversed(max_y());
        if(texture()->height() <= needed_height && texture()->width() <= needed_width){image.get()->paste(texture(), needed_x, needed_y);}
        else{a_last_texture = texture()->resize_adaptative(needed_width, needed_height);a_last_texture_height=needed_height;a_last_texture_width=needed_width;image.get()->paste(a_last_texture.get(), needed_x, needed_y);}
    }

    // Coordinates datas
    int Graphic::Graphic_Texture::graphic_x_to_texture_x(scls::Fraction x){scls::Fraction local_x = x - min_x();return ((local_x / width()).to_double() * static_cast<double>(texture()->width()));}
    int Graphic::Graphic_Texture::graphic_y_to_texture_y(scls::Fraction y){scls::Fraction local_y = y - min_y();return (static_cast<double>(texture()->height()) - (local_y / height()).to_double() * static_cast<double>(texture()->height()));}

    // Returns the source to a XML text
    std::string Graphic::Graphic_Texture::to_xml_text(){return std::string("<") + to_xml_text_object_name() + to_xml_text_name() + to_xml_text_x() + to_xml_text_y() + to_xml_text_width() + to_xml_text_height() + to_xml_text_source() + std::string(">");}
    std::string Graphic::Graphic_Texture::to_xml_text_object_name(){return std::string("texture_object");}
    std::string Graphic::Graphic_Texture::to_xml_text_source(){return std::string(" source=\"") + source() + std::string("\"");}

    // Returns the datas set to an image
    std::shared_ptr<scls::__Image_Base> Graphic::Datas_Set::to_image() {
        if(type() == Graphic::Datas_Set::Datas_Set_Type::DST_Histogram){return to_image_histogram();}
        else if(type() == Graphic::Datas_Set::Datas_Set_Type::DST_Point_Cloud || type() == Graphic::Datas_Set::Datas_Set_Type::DST_Point_Cloud_Linked){return to_image_point_cloud();}
        return std::shared_ptr<scls::__Image_Base>();
    }
    std::shared_ptr<scls::__Image_Base> Graphic::Datas_Set::to_image_histogram() {
        // Create the image
        std::shared_ptr<scls::__Image_Base> to_return = std::make_shared<scls::__Image_Base>(width_in_pixel(), height_in_pixel(), scls::Color(255, 255, 255));

        // Get the needed datas
        std::vector<scls::Fraction> all_values;std::vector<int> all_values_number;
        int max_number = 0;scls::sort_fractions(a_datas);
        for(int i = 0;i<static_cast<int>(a_datas.size());i++) {
            int j = 0;for(;j<static_cast<int>(all_values.size());j++) {if(all_values.at(j) == a_datas.at(i)){break;}}
            if(j >= static_cast<int>(all_values.size())){all_values.push_back(a_datas.at(i));all_values_number.push_back(1);}
            else{all_values_number[j]++;}
            if(all_values_number.at(j) > max_number){max_number = all_values_number.at(j);}
        }

        // Draw each part
        scls::Color bar_color = scls::Color(255, 0, 0);
        std::vector<long long int> all_width = scls::partition_number(to_return.get()->height(), all_values.size());
        int current_x = 0;
        for(int i = 0;i<static_cast<int>(all_values.size());i++) {
            double proportion = static_cast<double>(all_values_number.at(i)) / static_cast<double>(max_number);

            // Draw the bar
            int current_height = to_return.get()->height() * proportion;
            int current_y = to_return.get()->height() - current_height;
            to_return.get()->fill_rect(current_x, current_y, all_width.at(i), current_height, bar_color);
            current_x += all_width.at(i);
        }

        return to_return;
    }
    std::shared_ptr<scls::__Image_Base> Graphic::Datas_Set::to_image_point_cloud() {
        // Create the image
        std::shared_ptr<scls::__Image_Base> to_return = std::make_shared<scls::__Image_Base>(width_in_pixel(), height_in_pixel(), scls::Color(255, 255, 255, 0));

        // Get the needed datas
        std::vector<scls::Fraction> all_values;std::vector<scls::Fraction> all_values_number;
        std::vector<scls::Fraction> all_values_average = std::vector<scls::Fraction>(a_datas.size(), 0);
        std::vector<scls::Fraction>* final_values = 0;
        scls::Fraction max_number = 0;scls::Fraction min_number = 0;
        scls::Fraction sum_ponderations = 0;scls::Fraction sum_values = 0;
        if(a_display_type == Graphic::Datas_Set::Datas_Set_Display_Type::DSDT_Value){scls::sort_fractions(a_datas);}
        for(int i = 0;i<static_cast<int>(a_datas.size());i++) {
            // Get the value
            int j = 0;for(;j<static_cast<int>(all_values.size());j++) {if(all_values.at(j) == a_datas.at(i)){break;}}
            if(j >= static_cast<int>(all_values.size())){all_values.push_back(a_datas.at(i));all_values_number.push_back(1);}
            else{all_values_number[j]+=1;}
            sum_ponderations += 1;sum_values += a_datas.at(i);

            // Calculate an average if needed and check the max value
            if(a_display_type == Graphic::Datas_Set::Datas_Set_Display_Type::DSDT_Average) {all_values_average[i] = sum_values / sum_ponderations;if(all_values_average.at(i) > max_number){max_number = all_values_average.at(i);}}
            else {if(all_values_number.at(j) > max_number){max_number = all_values_number.at(j);}}
        }

        // Set the final values
        if(a_fixed_max_used){max_number = a_fixed_max;}if(a_fixed_min_used){min_number = a_fixed_min;}
        if(a_display_type == Graphic::Datas_Set::Datas_Set_Display_Type::DSDT_Average) {final_values = &all_values_average;}
        else {final_values = &all_values_number;}

        // Draw each part
        scls::Color bar_color = scls::Color(255, 0, 0);
        std::vector<long long int> all_width = scls::partition_number(to_return.get()->height(), final_values->size());
        int current_x = 0;int line_width = 2;int point_width = 5;
        int last_x = 0;int last_y = 0;
        for(int i = 0;i<static_cast<int>(final_values->size())&&i<static_cast<int>(all_width.size())+1;i++) {
            double proportion = (final_values->at(i) - min_number).to_double() / (max_number - min_number).to_double();

            // Draw the bar
            int current_y = (to_return.get()->height() - to_return.get()->height() * proportion) - point_width / 2;
            if(i > 0){to_return.get()->draw_line((current_x + point_width / 2) - line_width / 2, (current_y + point_width / 2) - line_width / 2, last_x, last_y, bar_color, line_width);}
            to_return.get()->fill_rect(current_x, current_y, point_width, point_width, bar_color);
            last_x = (current_x + point_width / 2) - line_width / 2;last_y = (current_y + point_width / 2) - line_width / 2;
            if(i < static_cast<int>(all_width.size())){current_x += all_width.at(i);}
        }

        return to_return;
    }

    // Graphic constructor
    Graphic::Graphic_Function::Graphic_Function(std::shared_ptr<Function_Studied> function_studied):a_function_studied(function_studied){}

    // Draws the object on an image
    void Graphic::Graphic_Text::draw_on_image(std::shared_ptr<scls::__Image_Base> image) {
        scls::Text_Style current_style = *style();current_style.set_color(color_with_opacity(current_style.color()));
        std::shared_ptr<scls::__Image_Base> needed_image = a_text_image_generator.get()->image_shared_ptr(final_content(), current_style);
        double needed_x = graphic_x_to_pixel_x(x().to_double());
        double needed_y = graphic_y_to_pixel_y_inversed(y().to_double());
        image.get()->paste(needed_image.get(), needed_x - needed_image.get()->width() / 2, needed_y - needed_image.get()->height() / 2);
    }

    // Returns the final content in the object
    std::shared_ptr<scls::__XML_Text_Base> Graphic::Graphic_Text::final_content() {
        if(window_struct() == 0){std::shared_ptr<scls::__XML_Text_Base> to_return = scls::xml(std::make_shared<scls::__Balise_Container>(), content());return to_return;}

        std::shared_ptr<scls::__XML_Text_Base> to_return = scls::xml(window_struct()->balises_shared_ptr(), content());

        for(int i = 0;i<static_cast<int>(to_return.get()->sub_texts().size());i++) {
            // Basic datas
            std::vector<scls::XML_Attribute>& attributes = to_return.get()->sub_texts().at(i).get()->xml_balise_attributes();

            if(to_return.get()->sub_texts().at(i).get()->xml_balise_name() == std::string("env")) {
                // Using the PLEOS environment
                std::string variable_name = std::string();

                for(int j = 0;j<static_cast<int>(attributes.size());j++){
                    if(attributes.at(j).name == std::string("var")){variable_name = attributes.at(j).value;}
                }

                // Format the value
                double value = environment()->create_unknown(variable_name)->value.get()->value_to_double(0);
                to_return.get()->sub_texts()[i] = scls::xml(window_struct()->balises_shared_ptr(), scls::format_number_to_text(value, 2));
            }
        }

        return to_return;
    }

    // Returns the name of the object
    std::string Graphic::Graphic_Text::to_xml_text(){return std::string("<") + to_xml_text_object_name() + to_xml_text_font_size() + to_xml_text_name() + to_xml_text_x() + to_xml_text_y() + to_xml_text_width() + to_xml_text_height() + std::string(">") + content() + std::string("</") + to_xml_text_object_name() + std::string(">");};
    std::string Graphic::Graphic_Text::to_xml_text_font_size() {return std::string(" font_size=") + std::to_string(style()->font_size());}
    std::string Graphic::Graphic_Text::to_xml_text_object_name() {return std::string("text");}

    //******************
    //
    // Graphic main members
    //
    //******************

    // Adds a forms to the graphic
    void Graphic::add_form(std::shared_ptr<Form_2D> needed_formd_2d){
        bool good = true;for(int i = 0;i<static_cast<int>(a_forms_2d.size());i++){if(a_forms_2d.at(i).get()== needed_formd_2d.get()){good=false;break;}}
        if(good){a_forms_2d.push_back(needed_formd_2d);}
        good = true;for(int i = 0;i<static_cast<int>(a_objects.size());i++){if(a_objects.at(i).get()== needed_formd_2d.get()){good=false;break;}}
        if(good){a_objects.push_back(needed_formd_2d);}
    }

    // Adds a function to the graphic
    std::shared_ptr<Graphic::Graphic_Function> Graphic::add_function(std::shared_ptr<Function_Studied> function_studied) {std::shared_ptr<Graphic_Function> new_function = std::make_shared<Graphic_Function>(function_studied);a_functions.push_back(new_function);a_graphic_base.get()->a_function_number++;return new_function;}

    // Adds a circle to the graphic
    void Graphic::add_circle(std::shared_ptr<Circle> circle_to_add){a_circles.push_back(circle_to_add);a_objects.push_back(circle_to_add);circle_to_add.get()->set_index(a_circles.size() - 1);};
    std::shared_ptr<Circle>* Graphic::add_circle(std::string circle_name, scls::Point_2D center, scls::__Formula_Base radius){return add_circle(circle_name, center, radius, radius);};
    std::shared_ptr<Circle>* Graphic::add_circle(std::string circle_name, scls::Point_2D center, scls::__Formula_Base radius_x, scls::__Formula_Base radius_y){add_circle(std::make_shared<Circle>(graphic_base_shared_ptr(), circle_name, center, radius_x, radius_y));a_circles.at(a_circles.size()-1).get()->set_unknowns(a_unknowns);a_circles.at(a_circles.size()-1).get()->attached_transform()->set_unknowns(a_unknowns);return &a_circles[a_circles.size() - 1];};
    // Returns a circle by its id / name
    Circle* Graphic::circle_by_id(int circle_id){return circle_by_id_shared_ptr(circle_id).get();};
    std::shared_ptr<Circle> Graphic::circle_by_id_shared_ptr(int circle_id){for(int i = 0;i<static_cast<int>(a_circles.size());i++){if(a_circles.at(i).get()->id() == circle_id){return a_circles.at(i);}}return std::shared_ptr<Circle>();};
    Circle* Graphic::circle_by_name(std::string name){return circle_by_name_shared_ptr(name).get();};
    std::shared_ptr<Circle> Graphic::circle_by_name_shared_ptr(std::string name){for(int i = 0;i<static_cast<int>(a_circles.size());i++){if(a_circles.at(i).get()->name() == name){return a_circles.at(i);}}return std::shared_ptr<Circle>();};
    // Creates a new circle in the graphic
    std::shared_ptr<Circle> Graphic::new_circle(std::string circle_name, __Graphic_Object_Base* parent, scls::Fraction x, scls::Fraction y, scls::Fraction radius_x, scls::Fraction radius_y){
        std::shared_ptr<Circle>created_circle=std::make_shared<Circle>(graphic_base_shared_ptr(), circle_name, parent, scls::Point_2D(x, y), radius_x, radius_y);
        created_circle.get()->set_unknowns(a_unknowns);
        add_circle(created_circle);
        created_circle.get()->attached_transform()->set_unknowns(a_unknowns);
        return created_circle;
    };
    std::shared_ptr<Circle> Graphic::new_circle(std::string circle_name, __Graphic_Object_Base* parent, scls::Fraction x, scls::Fraction y, scls::Fraction radius){return new_circle(circle_name, parent, x, y, radius, radius);};
    std::shared_ptr<Circle> Graphic::new_circle(std::string circle_name, scls::Fraction x, scls::Fraction y, scls::Fraction radius){return new_circle(circle_name, 0, x, y, radius);}
    std::shared_ptr<Circle> Graphic::new_circle(std::string circle_name, scls::Point_2D center, scls::Fraction radius){return new_circle(circle_name, 0, center.x(), center.y(), radius);}

    // Adds a texture to the graphic
    void Graphic::add_texture_object(std::shared_ptr<Graphic_Texture_Object> current_texture) {a_objects.push_back(current_texture);a_textures.push_back(current_texture);}

    // Creates the background texture
    void Graphic::create_background_texture(std::shared_ptr<scls::__Image_Base> image){a_background_texture = image;}
    void Graphic::create_background_texture(int image_width, int image_height) {a_background_texture = std::make_shared<scls::__Image_Base>(image_width, image_height, scls::Color(0, 0, 0, 0));}
    void Graphic::create_background_texture(std::string path) {a_background_texture = std::make_shared<scls::__Image_Base>(path);}

    // Draws a datas set on the graphic
    void Graphic::draw_datas_set(Datas_Set* needed_datas_set, std::shared_ptr<scls::__Image_Base> to_return) {
        std::shared_ptr<scls::__Image_Base> needed_image = needed_datas_set->to_image();
        double needed_x = graphic_x_to_pixel_x(needed_datas_set->x().to_double(), to_return);
        double needed_y = graphic_y_to_pixel_y_inversed(needed_datas_set->y().to_double(), to_return);
        to_return.get()->paste(needed_image.get(), needed_x - needed_image.get()->width() / 2, needed_y - needed_image.get()->height() / 2);
    }

    // Draws a point on the graphic
    void Graphic::draw_vector(Point_2D* needed_point, std::shared_ptr<scls::__Image_Base> to_return) {
        // Get the needed coordinates
        scls::Fraction needed_x_end = 0;if(needed_point->x_end() != 0){needed_x_end = needed_point->x_end();}
        needed_x_end = graphic_x_to_pixel_x(needed_x_end, to_return);
        scls::Fraction needed_y_end = 0;if(needed_point->y_end() != 0){needed_y_end = needed_point->y_end();}
        needed_y_end = graphic_y_to_pixel_y_inversed(needed_y_end, to_return);
        scls::Fraction needed_x_start = graphic_x_to_pixel_x(needed_point->absolute_x(), to_return);
        scls::Fraction needed_y_start = graphic_y_to_pixel_y_inversed(needed_point->absolute_y(), to_return);
        // Apply the drawing proportion
        double hat_position_in_purcentage = 1;double hat_size_in_purcentage = 0.2;
        if(needed_point->drawing_proportion() != 1) {
            scls::Fraction x_diff = needed_x_end - needed_x_start;
            scls::Fraction y_diff = needed_y_end - needed_y_start;
            needed_x_end = needed_x_start + x_diff * needed_point->drawing_proportion();
            needed_y_end = needed_y_start + y_diff * needed_point->drawing_proportion();
            hat_position_in_purcentage=scls::vector_2d_angle(x_diff.to_double(), y_diff.to_double()) * (180.0/SCLS_PI);
            hat_size_in_purcentage = 20;
        }

        // Get the graphic values
        scls::Color needed_color = needed_point->color();
        double needed_width = 2;

        // Draw the vector
        if(needed_point->type() == Vector_Type::VT_Arrow) {to_return.get()->draw_arrow(needed_x_start.to_double(), needed_y_start.to_double(), needed_x_end.to_double(), needed_y_end.to_double(), needed_color, hat_position_in_purcentage, hat_size_in_purcentage, needed_width);}
        else if(needed_point->type() == Vector_Type::VT_Vector) {to_return.get()->draw_line(needed_x_start.to_double(), needed_y_start.to_double(), needed_x_end.to_double(), needed_y_end.to_double(), needed_color, needed_width);}
        else {needed_width=8;to_return.get()->fill_rect(needed_x_start.to_double() - needed_width / 2.0, needed_y_start.to_double()  - needed_width / 2.0, needed_width, needed_width, needed_color);}
    }

    // Returns a form by its name / id
    std::shared_ptr<Form_2D> Graphic::form_2d_by_id_shared_ptr(int form_id){for(int i = 0;i<static_cast<int>(a_forms_2d.size());i++){if(a_forms_2d[i].get()->id() == form_id){return a_forms_2d[i];}}return std::shared_ptr<Form_2D>();};
    Form_2D* Graphic::form_2d_by_id(int form_id){return form_2d_by_id_shared_ptr(form_id).get();};
    std::shared_ptr<Form_2D> Graphic::form_shared_ptr(std::string form_name){for(int i = 0;i<static_cast<int>(a_forms_2d.size());i++){if(a_forms_2d[i].get()->name() == form_name){return a_forms_2d[i];}}return std::shared_ptr<Form_2D>();};
    Form_2D* Graphic::form(std::string form_name){return form_shared_ptr(form_name).get();};

    // Needed fragment shader for the function
    std::string Graphic::graphic_function_fragment_shader(scls::Formula needed_formula) {
        std::string to_return = "#version 330 core\n";
        to_return += "in vec2 tex_pos;"; // Uniform / in/out variables
        to_return += "out vec4 FragColor;";
        to_return += "uniform vec4 background_color;";
        to_return += "uniform vec4 border_color;";
        to_return += "uniform vec4 border_width;";
        to_return += "uniform vec2 middle_position;";
        to_return += "uniform vec4 object_extremum;";
        to_return += "uniform vec4 object_rect;";
        to_return += "uniform vec2 pixel_by_base;";
        to_return += "uniform sampler2D texture_0;";
        to_return += "uniform bool texture_binded;";
        to_return += "uniform vec4 texture_rect;\n";

        // Function
        std::string needed_glsl = needed_formula.to_polymonial().to_glsl(0);
        to_return += needed_glsl;
        to_return += "float foo(float x){return poly(x);}\n";

        to_return += "void main(){";
        to_return += scls::Shader_Program::default_gui_extremum_handling();
        to_return += "vec2 current_pos = tex_pos;vec4 final_color = vec4(1, 1, 1, 0);";
        to_return += "float height_multiplier = (object_rect.w / pixel_by_base.y);float width_multiplier = (object_rect.z / pixel_by_base.x);";
        to_return += "float x_1 = ((current_pos.x - 0.5) + middle_position.x) * width_multiplier;";
        to_return += "float x_2 = x_1 + (width_multiplier/object_rect.z);";

        // Function
        to_return += "float y_1 = foo(x_1);float y_2 = foo(x_2);if(y_1 > y_2){y_1+=height_multiplier/object_rect.w;}else{y_2+=height_multiplier/object_rect.w;}";
        to_return += "float p = (((current_pos.y - 0.5) + middle_position.y)) * height_multiplier;";
        to_return += "if((y_1 >= p && y_2 <= p) || (y_1 <= p && y_2 >= p)){final_color = vec4(1, 0, 0, 1);}";
        to_return += "FragColor = final_color;}";
        return to_return;
    }

    // Returns a new graphic
    std::shared_ptr<Graphic> Graphic::new_graphic(scls::_Window_Advanced_Struct* window_struct){std::shared_ptr<Graphic> to_return = std::shared_ptr<Graphic>(new Graphic());to_return.get()->a_this_object = to_return;to_return.get()->a_window_struct = window_struct;return to_return;}
    std::shared_ptr<Graphic> Graphic::new_graphic(){return Graphic::new_graphic(0);}

    // Creates and returns a form (and its point)
    std::shared_ptr<Form_2D> Graphic::new_form(std::string name, __Graphic_Object_Base* parent, scls::Fraction x_1, scls::Fraction y_1, scls::Fraction x_2, scls::Fraction y_2, scls::Fraction x_3, scls::Fraction y_3, scls::Fraction x_4, scls::Fraction y_4) {
        std::shared_ptr<Form_2D>to_return=new_form(name, parent);

        // Arrange the points
        scls::Fraction max_x = x_1;scls::Fraction min_x = x_1;
        scls::Fraction max_y = x_1;scls::Fraction min_y = x_1;
        max_x = std::max(std::max(std::max(x_1, x_2), x_3), x_4);
        min_x = std::min(std::min(std::min(x_1, x_2), x_3), x_4);
        max_y = std::max(std::max(std::max(y_1, y_2), y_3), y_4);
        min_y = std::min(std::min(std::min(y_1, y_2), y_3), y_4);
        scls::Fraction needed_height = (max_y - min_y).abs();scls::Fraction needed_width = (max_x - min_x).abs();
        scls::Fraction needed_x = (x_4 + x_3 + x_2 + x_1) / 4;scls::Fraction needed_y = (y_4 + y_3 + y_2 + y_1) / 4;
        to_return.get()->set_height(needed_height);to_return.get()->set_width(needed_width);
        to_return.get()->set_x(needed_x);to_return.get()->set_y(needed_y);

        // Add the points
        x_1 -= needed_x;x_2 -= needed_x;x_3 -= needed_x;x_4 -= needed_x;
        y_1 -= needed_y;y_2 -= needed_y;y_3 -= needed_y;y_4 -= needed_y;
        to_return.get()->add_point(new_point(name + std::string("-p1"), x_1 / needed_width, y_1 / needed_height));
        to_return.get()->add_point(new_point(name + std::string("-p2"), x_2 / needed_width, y_2 / needed_height));
        to_return.get()->add_point(new_point(name + std::string("-p3"), x_3 / needed_width, y_3 / needed_height));
        to_return.get()->add_point(new_point(name + std::string("-p4"), x_4 / needed_width, y_4 / needed_height));
        return to_return;
    }
    std::shared_ptr<Form_2D> Graphic::new_form(std::string name, std::vector<std::shared_ptr<pleos::Point_2D>> points) {
        // Create the form
        std::shared_ptr<Form_2D>to_return=new_form(name);

        // Put each points
        for(int i = 0;i<static_cast<int>(points.size());i++){to_return.get()->add_point_external(points.at(i));}

        // Return the result
        return to_return;
    };
    std::shared_ptr<Form_2D> Graphic::new_form(std::string name, scls::Fraction x_1, scls::Fraction y_1, scls::Fraction x_2, scls::Fraction y_2, scls::Fraction x_3, scls::Fraction y_3, scls::Fraction x_4, scls::Fraction y_4) {return new_form(name, 0, x_1, y_1, x_2, y_2, x_3, y_3, x_4, y_4);}
    std::shared_ptr<Form_2D> Graphic::new_form(std::string name, __Graphic_Object_Base* parent){std::shared_ptr<Form_2D>to_return=std::make_shared<Form_2D>(graphic_base_shared_ptr(), name, parent);add_form(to_return);return to_return;};
    std::shared_ptr<Form_2D> Graphic::new_form(std::string name){std::shared_ptr<Form_2D>to_return=std::make_shared<Form_2D>(graphic_base_shared_ptr(), name);add_form(to_return);return to_return;};
    std::shared_ptr<Form_2D> Graphic::new_form(std::string name, std::string points){std::shared_ptr<Form_2D>needed_form=new_form(name);set_form_points(needed_form.get(), points);return needed_form;};

    // Creates and returns a form from a face
    std::shared_ptr<Form_2D> Graphic::new_form_from_face(std::string name, scls::model_maker::Face* face){
        std::shared_ptr<Form_2D>to_return=new_form(name);

        // Add the points
        for(int i = 0;i<static_cast<int>(face->points().size());i++) {
            std::string current_name = name + std::string("-") + std::to_string(i);
            std::shared_ptr<Point_2D> current_vector = add_point(current_name, face->points()[i].get());
            to_return.get()->add_point(current_vector);
        }
        // Add the exclusion points
        std::string final_points = std::string();
        for(int i = 0;i<static_cast<int>(face->exclusion_points().size());i++) {
            std::string current_name = name + std::string("-") + std::to_string(i);
            std::shared_ptr<Point_2D> current_vector = add_point(current_name, face->exclusion_points()[i].get());
            to_return.get()->add_exclusion_point(current_vector);
        }

        return to_return;
    };

    // Creates and returns a line (and its points)
    int __line_number = 0;
    std::shared_ptr<Form_2D> Graphic::new_line(std::string name, scls::Fraction x_1, scls::Fraction y_1, scls::Fraction x_2, scls::Fraction y_2){
        if(name == std::string()){name = std::string("line_") + std::to_string(__line_number);}
        std::shared_ptr<Form_2D>to_return=new_form(name);
        scls::Fraction needed_height = (y_2 - y_1).abs();scls::Fraction needed_width = (x_2 - x_1).abs();
        scls::Fraction needed_x = (x_2 + x_1) / 2;scls::Fraction needed_y = (y_2 + y_1) / 2;
        to_return.get()->set_height(needed_height);to_return.get()->set_width(needed_width);
        to_return.get()->set_x(needed_x);to_return.get()->set_y(needed_y);

        // Add the points
        x_1 -= needed_x;x_2 -= needed_x;
        y_1 -= needed_y;y_2 -= needed_y;
        to_return.get()->new_point(x_1 / needed_width, y_1 / needed_height);
        to_return.get()->new_point(x_2 / needed_width, y_2 / needed_height);
        add_form(to_return);__line_number++;
        return to_return;
    }
    std::shared_ptr<Form_2D> Graphic::new_line(std::string name, std::shared_ptr<__Graphic_Object_Base> point_1, scls::Point_2D point_2) {
        // Create the form
        std::shared_ptr<Form_2D>to_return=new_form(name);

        // Put each points
        to_return.get()->add_point(new_point(name + std::string("_point_1"), point_2));
        to_return.get()->add_point_external(point_1);

        // Return the result
        return to_return;
    }

    // Creates and returns a square (and its point)
    std::shared_ptr<Form_2D> Graphic::new_rect(std::string name, __Graphic_Object_Base* parent, scls::Fraction x, scls::Fraction y, scls::Fraction width, scls::Fraction height){return new_form(name, parent, x, y, x + width, y, x + width, y + height, x, y + height);}
    std::shared_ptr<Form_2D> Graphic::new_square(std::string name, scls::Fraction x, scls::Fraction y, scls::Fraction width, scls::Fraction height){if(width==0){width=1;}if(height==0){height=1;} return new_form(name, 0, x, y, x + width, y, x + width, y + height, x, y + height);}

    // Handle texts
    // Adds an text to the graphic
    void Graphic::add_text(std::shared_ptr<Graphic::Graphic_Text> text){bool good = true;for(int i = 0;i<static_cast<int>(a_texts.size());i++){if(a_texts.at(i).get()==text.get()){good=false;break;}}if(good){a_objects.push_back(text);a_texts.push_back(text);}}
    // Creates and returns a new text in the graphic
    Graphic::Graphic_Text* Graphic::new_text(std::string content, scls::Fraction x, scls::Fraction y, std::shared_ptr<scls::Text_Style> style){std::shared_ptr<Graphic::Graphic_Text> created_text = std::make_shared<Graphic::Graphic_Text>(a_this_object);add_text(created_text);Graphic::Graphic_Text* current_text=created_text.get();current_text->set_content(content);current_text->set_style(style);current_text->set_x(x);current_text->set_y(y);return current_text;}
    Graphic::Graphic_Text* Graphic::new_text(std::string content, Point_2D position, std::shared_ptr<scls::Text_Style> style){return new_text(content, position.x(), position.y(), style);}

    // Creates and returns a triangle (and its point)
    std::shared_ptr<Form_2D> Graphic::new_triangle(std::string name, scls::Fraction x_1, scls::Fraction y_1, scls::Fraction x_2, scls::Fraction y_2, scls::Fraction x_3, scls::Fraction y_3) {
        std::shared_ptr<Form_2D>to_return=std::make_shared<Form_2D>(graphic_base_shared_ptr(), name);
        to_return.get()->add_point(new_point(name + std::string("-p1"), x_1, y_1));
        to_return.get()->add_point(new_point(name + std::string("-p2"), x_2, y_2));
        to_return.get()->add_point(new_point(name + std::string("-p3"), x_3, y_3));
        add_form(to_return);
        return to_return;
    }

    // Returns an object shared ptr
    __Graphic_Object_Base* Graphic::object_by_name(std::string name){return object_by_name_shared_ptr(name).get();}
    std::shared_ptr<__Graphic_Object_Base> Graphic::object_by_name_shared_ptr(std::string name) {
        std::shared_ptr<__Graphic_Object_Base> to_return = circle_by_name_shared_ptr(name);
        if(to_return.get() != 0){return to_return;}
        to_return = form_shared_ptr(name);
        if(to_return.get() != 0){return to_return;}
        to_return = texture_object_by_name(name);
        if(to_return.get() != 0){return to_return;}
        return std::shared_ptr<__Graphic_Object_Base>();
    }

    // Resets the object
    void Graphic::reset(){a_graphic_base.get()->a_time=0;a_circles.clear();a_forms_2d.clear();a_functions.clear();a_objects.clear();a_physic_objects.clear();a_physic_map.clear();a_points.clear();a_texts.clear();a_textures.clear();a_vectors.clear();a_physic_map_start_x=0;a_physic_map_start_y=0;set_draw_base(true);set_draw_sub_bases(true);};

    // Sets the size of the image
    void Graphic::set_image_size(int image_width, int image_height){
        a_graphic_base.get()->a_height_in_pixel = image_height;
        a_graphic_base.get()->a_width_in_pixel = image_width;

        // Handle the height and width
        if(height() != -1) {a_graphic_base.get()->a_pixel_by_case_y = static_cast<double>(image_height) / height().to_double();}
        if(width() != -1) {set_pixel_by_case_x(static_cast<double>(image_width) / width().to_double());}
    }
    // Sets the scale of the base
    void Graphic::set_scale(scls::__Formula_Base::Formula width, scls::__Formula_Base::Formula height, bool width_used, bool height_used){a_graphic_base.get()->a_height = height;a_graphic_base.get()->a_height_used = height_used;a_graphic_base.get()->a_width = width;a_graphic_base.get()->a_width_used = width_used;}
    void Graphic::set_scale(scls::__Formula_Base::Formula width, scls::__Formula_Base::Formula height){set_scale(width, height, false, false);}
    void Graphic::set_scale(double width, double height, bool width_used, bool height_used){set_scale(scls::__Formula_Base::Formula(scls::Fraction::from_double(width)), scls::__Formula_Base::Formula(scls::Fraction::from_double(height)), width_used, height_used);}
    void Graphic::set_scale(double width, double height){set_scale(scls::__Formula_Base::Formula(scls::Fraction::from_double(width)), scls::__Formula_Base::Formula(scls::Fraction::from_double(height)), false, false);}
    // Set the number of pixels by cases
    void Graphic::set_pixel_by_case_x(double new_value) {
        a_graphic_base.get()->a_pixel_by_case_x = new_value;
        scls::Fraction repartition = scls::Fraction::from_double(new_value - std::floor(new_value));
        a_pixel_case_repartition_x = scls::partition_number(repartition.numerator(), repartition.denominator());
    }

    // Soft reset the graphic
    void Graphic::soft_reset(){for(int i = 0;i<static_cast<int>(a_objects.size());i++){a_objects.at(i).get()->soft_reset();}}

    // Returns a texture object by its name
    std::shared_ptr<Graphic::Graphic_Texture> Graphic::texture_object_by_name(std::string needed_name){for(int i = 0;i<static_cast<int>(a_textures.size());i++){if(a_textures.at(i).get()->name() == needed_name){return a_textures.at(i);}}return std::shared_ptr<Graphic_Texture>();}

    // Struct containing some datas for positions
    struct Needed_Pos {scls::__Formula_Base::Formula pos;scls::__Formula_Base::Formula previous_pos;bool previous_pos_used = false;};
    // Draw the bases of the image
    void Graphic::image_draw_base(std::shared_ptr<scls::__Image_Base> image) {
        // Horizontal lines
        double current_y = pixel_y_to_graphic_y(image.get()->height(), image).to_double();
        current_y = floor(current_y);
        double needed_y = graphic_y_to_pixel_y(current_y, image);
        while(needed_y < image.get()->width()) {
            scls::Color needed_color = scls::Color(0, 0, 0);
            int needed_height = 1;
            if(current_y == 0){needed_height = 2;}else{needed_color=scls::Color(125, 125, 125);}
            image.get()->fill_rect(0, image.get()->height() - needed_y, image.get()->width(), needed_height, needed_color);
            current_y++;
            needed_y = graphic_y_to_pixel_y(current_y, image);
        }
        // Vertical lines
        double current_x = pixel_x_to_graphic_x(0, image).to_double();
        current_x = ceil(current_x);
        int needed_x = graphic_x_to_pixel_x(current_x, image);
        while(needed_x < image.get()->width()) {
            scls::Color needed_color = scls::Color(0, 0, 0);
            int needed_width = 1;
            if(current_x == 0){needed_width = 2;}else{needed_color=scls::Color(125, 125, 125);}
            image.get()->fill_rect(needed_x, 0, needed_width, image.get()->height(), needed_color);
            current_x++;
            needed_x = graphic_x_to_pixel_x(current_x, image);
        }
    }
    // Draw a function on the image
    void Graphic::image_draw_function(std::shared_ptr<scls::__Image_Base> to_return, std::shared_ptr<Graphic_Function> needed_function, std::vector<scls::Fraction>& screen_pos) {
        // Asserts
        if(needed_function.get()->definition_set() == 0) {scls::print("Warning", "PLEOS Graphic", std::string("The \"") + needed_function.get()->name() + std::string("\" function has no definition interval calculated."));return;}

        // Get the values
        scls::Formula needed_formula = needed_function.get()->formula();
        std::vector<Needed_Pos> needed_pos = std::vector<Needed_Pos>(to_return.get()->width() + 1);
        // Get each values of the function
        scls::__Formula_Base::Formula last_pos;
        for(int j = 0;j<static_cast<int>(to_return.get()->width()) + 1;j++){
            // Get the needed pos
            Needed_Pos to_add;
            if(needed_function.get()->definition_set()->is_in(screen_pos[j])) {to_add.pos = needed_formula.value(screen_pos[j]);}
            else {to_add.pos = needed_formula.value(screen_pos[j] - scls::Fraction(1, 1000));}
            needed_pos[j] = to_add;

            // Check according to the last value
            if(j > 0 && !needed_function.get()->definition_set()->is_in(scls::Interval(screen_pos[j - 1], screen_pos[j]))) {
                needed_pos[j - 1].pos = needed_formula.value(screen_pos[j - 1] + scls::Fraction(1, 1000));
                needed_pos[j].previous_pos = needed_formula.value(screen_pos[j] - scls::Fraction(1, 1000));
                needed_pos[j].previous_pos_used = true;
            }

            // Finalise the creation
            last_pos = to_add.pos;
        }

        // Adapt each values to the screen
        struct Needed_Pos_Screen {int pos;int previous_pos;bool previous_pos_used = false;};
        std::vector<Needed_Pos_Screen> needed_y = std::vector<Needed_Pos_Screen>(to_return.get()->width() + 1);
        for(int i = 0;i<static_cast<int>(to_return.get()->width()) + 1;i++){
            scls::Fraction value = needed_pos[i].pos.formula_base()->value_to_fraction().normalized();
            needed_y[i].pos = graphic_y_to_pixel_y(value.to_double(), to_return);
            // Check the previous pos
            if(needed_pos[i].previous_pos_used) {
                value = needed_pos[i].previous_pos.formula_base()->value_to_fraction().normalized();
                needed_y[i].previous_pos = graphic_y_to_pixel_y(value.to_double(), to_return);
                needed_y[i].previous_pos_used = true;
            }
        }

        // Area under the curve
        for(int i = 0;i<static_cast<int>(needed_function.get()->curve_areas_number());i++) {
            int border_width = 1;
            scls::Fraction current_x = needed_function.get()->curve_area_start(i);
            int rect_number = needed_function.get()->curve_area_rectangle_number(i);
            scls::Fraction rect_width = (needed_function.get()->curve_area_end(i) - needed_function.get()->curve_area_start(i)) / rect_number;
            for(int j = 0;j<rect_number;j++) {
                scls::Fraction needed_value = needed_formula.value(current_x).real();
                if(needed_value != 0) {
                    to_return.get()->draw_rect(graphic_x_to_pixel_x(current_x.to_double(), to_return), graphic_y_to_pixel_y(-needed_value.to_double(), to_return), (rect_width * pixel_by_case_x()).to_double(), (needed_value * pixel_by_case_y()).to_double(), border_width, scls::Color(255, 0, 0));
                    to_return.get()->fill_rect(graphic_x_to_pixel_x(current_x.to_double(), to_return) + border_width, graphic_y_to_pixel_y(-needed_value.to_double(), to_return) + border_width, (rect_width * pixel_by_case_x()).to_double() - 2 * border_width, (needed_value * pixel_by_case_y()).to_double() - 2 * border_width, scls::Color(0, 255, 0));
                }
                current_x += rect_width;
            }
        }

        // Draw each pixel
        int width = 3;
        for(int j = 0;j<static_cast<int>(to_return.get()->width());j++) {
            int y_1 = to_return.get()->height() - needed_y[j].pos;
            int y_2 = to_return.get()->height() - needed_y[j + 1].pos;
            if(needed_y[j + 1].previous_pos_used){y_1 = to_return.get()->height() - needed_y[j + 1].previous_pos;}
            if(y_1 > to_return.get()->height() && y_2 > to_return.get()->height()){continue;}
            else if(y_1 < 0 && y_2 < 0){continue;}
            // Draw the point
            int needed_height = std::abs(y_1 - y_2);
            int needed_y = std::min(y_1, y_2) - width / 2.0;
            if(needed_y < to_return.get()->height() && needed_y >= -needed_height) {
                to_return.get()->fill_rect(j - width / 2.0, needed_y, width, needed_height + width, needed_function.get()->color());
            }
        }
    }
    // Returns the image of the graphic
    int Graphic::graphic_x_to_pixel_x(double x, int image_width){return std::round((x - left_x().to_double()) * pixel_by_case_x());};
    int Graphic::graphic_x_to_pixel_x(scls::Fraction x, int image_width){return std::round(((x - left_x()) * pixel_by_case_x()).to_double());};
    int Graphic::graphic_y_to_pixel_y(double y, int needed_height){return std::round((y - middle_y().to_double()) * pixel_by_case_y()) + (needed_height / 2.0);};
    int Graphic::graphic_y_to_pixel_y(scls::Fraction y, int needed_height){return std::round(((y - middle_y()) * pixel_by_case_y() + scls::Fraction(needed_height, 2)).to_double());};
    int Graphic::graphic_y_to_pixel_y_inversed(double y, int needed_height){return needed_height - graphic_y_to_pixel_y(y, needed_height);};
    int Graphic::graphic_y_to_pixel_y_inversed(scls::Fraction y, int needed_height){return needed_height - graphic_y_to_pixel_y(y, needed_height);};
    double Graphic::pixel_by_case_x() const {return a_graphic_base.get()->a_pixel_by_case_x;}
    double Graphic::pixel_in_case_x(int case_number) const {if(a_pixel_case_repartition_x.size()<=0){return a_graphic_base.get()->a_pixel_by_case_x;} return a_graphic_base.get()->a_pixel_by_case_x + a_pixel_case_repartition_x.at(case_number % a_pixel_case_repartition_x.size());}
    double Graphic::pixel_by_case_y() const {return a_graphic_base.get()->a_pixel_by_case_y;}
    scls::Fraction Graphic::pixel_x_to_graphic_x(int x, int image_width){return middle_x() + ((scls::Fraction(x) - scls::Fraction(image_width, 2)) / scls::Fraction::from_double(pixel_by_case_x()));}
    scls::Fraction Graphic::pixel_y_to_graphic_y(int y, int needed_height){return middle_y() + ((scls::Fraction(needed_height, 2) - scls::Fraction(y)) / scls::Fraction::from_double(pixel_by_case_y()));}
    std::shared_ptr<scls::__Image_Base> Graphic::to_image(int width_in_pixel, int height_in_pixel) {
        // Create the image
        std::shared_ptr<scls::__Image_Base> to_return = std::make_shared<scls::__Image_Base>(width_in_pixel, height_in_pixel, style()->background_color());
        set_image_size(width_in_pixel, height_in_pixel);

        // Draw the basic lines
        if(draw_background_texture() && background_texture() != 0){to_return.get()->paste(background_texture(), 0, 0);}
        if(draw_base() || draw_sub_bases()) {image_draw_base(to_return);}

        // Get the datas for the drawing
        scls::Fraction image = pixel_x_to_graphic_x(0, to_return).normalized(5);
        scls::Fraction multiplier = (scls::Fraction(1) / scls::Fraction::from_double(pixel_by_case_x())).normalized(5);
        std::vector<scls::Fraction> screen_pos = std::vector<scls::Fraction>(to_return.get()->width() + 1);
        for(int i = 0;i<static_cast<int>(to_return.get()->width()) + 1;i++){screen_pos[i] = image.normalized(5);image += multiplier;}
        // Draw the functions
        for(int i = 0;i<static_cast<int>(a_functions.size());i++) {image_draw_function(to_return, a_functions[i], screen_pos);}

        // Update the text
        std::shared_ptr<scls::Text_Image_Generator> tig = std::make_shared<scls::Text_Image_Generator>();
        for(int i = 0;i<static_cast<int>(a_texts.size());i++) {a_texts.at(i).get()->set_text_image_generator(tig);}
        // Draw the objects
        for(int i = 0;i<static_cast<int>(a_objects.size());i++) {a_objects.at(i).get()->draw_on_image(to_return);}

        // Draw the vectors
        //for(int i = 0;i<static_cast<int>(a_points.size());i++) {draw_vector(a_points.at(i).get(), to_return);}
        //for(int i = 0;i<static_cast<int>(a_vectors.size());i++) {draw_vector(a_vectors.at(i).get(), to_return);}

        // Draw the datas set
        for(int i = 0;i<static_cast<int>(a_datas_sets.size());i++) {draw_datas_set(a_datas_sets.at(i).get(), to_return);}

        // Draw the border
        to_return.get()->draw_border(style()->border_top_width(), style()->border_left_width(), style()->border_bottom_width(), style()->border_right_width(), style()->border_color);

        return to_return;
    }

    // Updates the object
    void Graphic::update(double used_delta_time){
        // Check delete
        for(int i = 0;i<static_cast<int>(circles().size());i++){if(circles().at(i).get()->should_delete()){circles().erase(circles().begin() + i);i--;}}
        for(int i = 0;i<static_cast<int>(forms_2d().size());i++){if(forms_2d().at(i).get()->should_delete()){forms_2d().erase(forms_2d().begin() + i);i--;}}
        for(int i = 0;i<static_cast<int>(texts().size());i++){if(texts().at(i).get()->should_delete()){texts().erase(texts().begin() + i);i--;}}
        for(int i = 0;i<static_cast<int>(textures().size());i++){if(textures().at(i).get()->should_delete()){textures().erase(textures().begin() + i);i--;}}
        for(int i = 0;i<static_cast<int>(objects().size());i++){if(objects().at(i).get()->should_delete()){objects().erase(objects().begin() + i);i--;}}

        // Check actions
        for(int i = 0;i<static_cast<int>(texts().size());i++){
            if(texts().at(i).get()->last_action() != 0){
                if(texts().at(i).get()->last_action_move() != 0) {
                    // Move action
                    scls::Point_2D direction_vector = (texts().at(i).get()->last_action_move()->position_end() - texts().at(i).get()->position()).normalized();
                    direction_vector *= scls::Fraction(1, 40);//scls::Fraction::from_double(used_delta_time);

                    // Do the movement
                    if(direction_vector.norm() < texts().at(i).get()->last_action_move()->position_end().distance(texts().at(i).get()->position())){texts().at(i).get()->set_position(texts().at(i).get()->position() + direction_vector);}
                    else{texts().at(i).get()->set_position(texts().at(i).get()->last_action_move()->position_end());texts().at(i).get()->delete_last_action();}
                }
            }
        }
    }

    //******************
    //
    // Loading handling
    //
    //******************

    // Handle utilities balises
	#define BALISE_REPEAT 0
	struct Utility_Balise {int times = 1;int type = -1;scls::Fraction value_start = 0;scls::Fraction value_end=1;};
    Utility_Balise utilities_balise(std::shared_ptr<scls::__XML_Text_Base> xml) {
        Utility_Balise to_return;

        // Handle the balise
        std::string balise_content = xml.get()->xml_balise();
        std::string current_balise_name = xml.get()->xml_balise_name();
        std::vector<scls::XML_Attribute>& attributes = xml.get()->xml_balise_attributes();
        if(current_balise_name == std::string("repeat")) {
            // Repeat instructions
            to_return.type = BALISE_REPEAT;
            for(int i = 0;i<static_cast<int>(attributes.size());i++) {
                if(attributes[i].name == "times") {to_return.times = std::stoi(attributes.at(i).value);}
            }
        }

        // Return the result
        return to_return;
    }

    // Balises circle in the graphic
    bool Graphic::graphic_from_xml_balise_attribute_circle(scls::XML_Attribute& attribute, std::shared_ptr<Circle> circle, Text_Environment& environment, std::shared_ptr<scls::Text_Style> text_style) {
        // Asserts
        if(circle.get() == 0){return false;}

        if(graphic_from_xml_balise_attribute_object(attribute, circle, environment, text_style)){return true;}
        else if(attribute.name == "angle_end") {circle.get()->set_angle_end(*environment.value_formula(attribute.value).formula_base());}
        else if(attribute.name == "angle_start") {circle.get()->set_angle_start(*environment.value_formula(attribute.value).formula_base());}
        else if(attribute.name == "border_color") {circle.get()->set_border_color(scls::Color::from_std_string(attribute.value));}
        else if(attribute.name == "border_radius" || attribute.name == "width") {circle.get()->set_border_radius(scls::Fraction::from_std_string(attribute.value).to_double());}
        else if(attribute.name == "color" || attribute.name == "background_color") {circle.get()->set_color(scls::Color::from_std_string(attribute.value));}
        else if(attribute.name == "radius") {circle.get()->set_radius_x((*environment.value_formula(attribute.value).formula_base()));circle.get()->set_radius_y((*environment.value_formula(attribute.value).formula_base()));}
        else if(attribute.name == "radius_x") {circle.get()->set_radius_x((*environment.value_formula(attribute.value).formula_base()));}
        else if(attribute.name == "radius_y") {circle.get()->set_radius_y((*environment.value_formula(attribute.value).formula_base()));}
        else{return false;}return true;
    }
    // Balises object in the graphic
    bool Graphic::graphic_from_xml_balise_attribute_object(scls::XML_Attribute& attribute, std::shared_ptr<__Graphic_Object_Base> object, Text_Environment& environment, std::shared_ptr<scls::Text_Style> text_style) {
        if(attribute.name == "name") {std::string needed_name = attribute.value;if(environment.last_repetition() != 0){needed_name += std::string("-") + std::to_string(environment.last_repetition());}object.get()->set_name(needed_name);}
        else if(attribute.name == "rotation") {object.get()->set_rotation(environment.value_formula(attribute.value));}
        else if(attribute.name == "tag" || attribute.name == "tags") {object.get()->load_tags(attribute.value);}
        else if(attribute.name == "x") {object.get()->set_x(environment.value_formula(attribute.value));}
        else if(attribute.name == "y") {object.get()->set_y(environment.value_formula(attribute.value));}
        else{return false;}return true;
    }
    // Balises physic in the graphic
    bool Graphic::graphic_from_xml_balise_attribute_physic(scls::XML_Attribute& attribute, std::shared_ptr<__Graphic_Object_Base> object, std::shared_ptr<Graphic_Physic>& physic, Text_Environment& environment, std::shared_ptr<scls::Text_Style> text_style) {
        // Asserts
        if(physic.get() == 0 && attribute.name != "physic"){return false;}

        if(attribute.name == "collision") {if(attribute.value == std::string("circle")){physic.get()->new_collision(Graphic_Collision_Type::GCT_Circle);}else if(attribute.value == std::string("rect")){physic.get()->new_collision(Graphic_Collision_Type::GCT_Rect);}else{physic.get()->new_collision(Graphic_Collision_Type::GCT_Line);}}
        else if(attribute.name == "gravity" || attribute.name == "use_gravity") {physic.get()->set_use_gravity((attribute.value == std::string("1") || attribute.value == std::string("true")));}
        else if(attribute.name == "physic") {
            if(physic_map().size() <= 0){load_physic_map(0, 0);}
            physic = std::make_shared<pleos::Graphic::Graphic_Physic>(object, object.get()->attached_transform_shared_ptr());
            physic.get()->set_use_gravity(false);
            if(attribute.value == std::string("static") || attribute.value == std::string("1")){physic.get()->set_static(true);}
            else{physic.get()->set_static(false);}
            add_physic_object(physic);
        }
        else if(attribute.name == "velocity") {physic.get()->set_velocity(environment.value_point_2d(attribute.value));physic.get()->set_velocity_start(physic.get()->velocity());}
        else{return false;}return true;
    }
    // Balises object in the texture object
    bool Graphic::graphic_from_xml_balise_attribute_texture_object(scls::XML_Attribute& attribute, std::shared_ptr<Graphic_Texture> object, Text_Environment& environment, std::shared_ptr<scls::Text_Style> text_style) {
        if(attribute.name == "height") {object.get()->set_height(scls::Fraction::from_std_string(attribute.value));}
        else if(attribute.name == "width") {object.get()->set_width(scls::Fraction::from_std_string(attribute.value));}
        else if(attribute.name == "source" || attribute.name == "src") {object.get()->set_texture(attribute.value);}
        else{return false;}return true;
    }

    // Balises in the graphic
    std::shared_ptr<pleos::__Graphic_Object_Base> Graphic::graphic_from_xml_balise(std::shared_ptr<scls::__XML_Text_Base> xml, Text_Environment& environment, std::shared_ptr<scls::Text_Style> text_style){
        std::string balise_content = xml.get()->xml_balise();
        std::string current_balise_name = xml.get()->xml_balise_name();
        std::vector<scls::XML_Attribute>& attributes = xml.get()->xml_balise_attributes();
        std::shared_ptr<pleos::__Graphic_Object_Base> object;

        // Check the balise
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
            Point_2D needed_vector = Point_2D(graphic_base_shared_ptr(), needed_name, needed_x_start, needed_y_start);
            needed_vector.set_x_end(needed_x_end);needed_vector.set_y_end(needed_y_end);
            needed_vector.set_type(Vector_Type::VT_Arrow);
            if(current_balise_name == "arrow_hat"){needed_vector.set_drawing_proportion(0);}
            add_vector(needed_vector);
        }
        else if(current_balise_name == "background_color") {set_background_color(scls::Color::from_xml(xml));}
        else if(current_balise_name == "base") {
            // Get the datas about the base of the graphic
            scls::__Formula_Base::Formula needed_height = height_formula();scls::__Formula_Base::Formula needed_width = width_formula();
            bool heigth_used = false;bool width_used = false;
            if(needed_height == -1){needed_height = 10;}if(needed_width == -1){needed_width = 10;}
            scls::Fraction middle_x = 10;scls::Fraction middle_y = 10;
            for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                if(attributes[j].name == "height") {needed_height = environment.value_formula(attributes[j].value);heigth_used=true;}
                else if(attributes[j].name == "width") {needed_width = environment.value_formula(attributes[j].value);width_used=true;}
                else if(attributes[j].name == "x") {middle_x = scls::Fraction::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "y") {middle_y = scls::Fraction::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "draw") {if(attributes[j].value == "false" || attributes[j].value == "0"){set_draw_base(false);set_draw_sub_bases(false);}}
            }
            set_middle(middle_x, middle_y);set_scale(needed_width, needed_height, width_used, heigth_used);
        }
        else if(current_balise_name == "border") {scls::border_from_xml(xml, style());}
        else if(current_balise_name == "circle") {
            // Add the circle
            std::shared_ptr<Circle> circle = *add_circle(std::string(""), scls::Point_2D(0, 0), 1, 1);
            object = circle;
            std::shared_ptr<pleos::Graphic::Graphic_Physic> physic;
            for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                if(graphic_from_xml_balise_attribute_circle(attributes[j], circle, environment, text_style)) {}
                else if(graphic_from_xml_balise_attribute_physic(attributes[j], circle, physic, environment, text_style)) {}
            }
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
            std::shared_ptr<Form_2D> created_form = new_form(needed_name, needed_points);
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
            std::shared_ptr<Graphic::Graphic_Function> fun = add_function(needed_function);
            needed_function.get()->set_color(needed_color);

            // Check the inner balises
            std::vector<std::shared_ptr<scls::__XML_Text_Base>>& sub_texts = xml.get()->sub_texts();
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
            std::shared_ptr<Graphic::Datas_Set> created_datas_set = new_datas_set(needed_name);
            created_datas_set.get()->set_display_type(needed_display_type);
            if(max_used){created_datas_set.get()->set_fixed_max(needed_max);}
            if(min_used){created_datas_set.get()->set_fixed_min(needed_min);}
            if(current_balise_name == "histogram"){created_datas_set.get()->set_type(Graphic::Datas_Set::Datas_Set_Type::DST_Histogram);}
            else{created_datas_set.get()->set_type(Graphic::Datas_Set::Datas_Set_Type::DST_Point_Cloud);}
            created_datas_set.get()->set_height(needed_height);created_datas_set.get()->set_width(needed_width);
            created_datas_set.get()->set_x(needed_x);created_datas_set.get()->set_y(needed_y);

            // Handle the balises
            std::vector<std::shared_ptr<scls::__XML_Text_Base>> sub_texts = xml.get()->sub_texts();
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
            bool use_collision = false;bool use_gravity = false;int use_physic = 0; // 0 = None, 1 = Static, 2 = Dynamic
            for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                if(attributes[j].name == "border_color" || attributes[j].name == "color") {border_color = scls::Color::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "border_radius" || attributes[j].name == "width") {border_radius = scls::Fraction::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "collision") {use_collision=true;}
                else if(attributes[j].name == "name") {needed_name = attributes[j].value;}
                else if(attributes[j].name == "physic") {if(attributes[j].value == std::string("static") || attributes[j].value == std::string("1")){use_physic = 1;}else{use_physic = 2;}}
                else if(attributes[j].name == "x_start" || attributes[j].name == "x_1") {x_1 = scls::Fraction::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "x_end" || attributes[j].name == "x_2") {x_2 = scls::Fraction::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "y_start" || attributes[j].name == "y_1") {y_1 = scls::Fraction::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "y_end" || attributes[j].name == "y_2") {y_2 = scls::Fraction::from_std_string(attributes[j].value);}
            }
            // Add the form
            std::shared_ptr<Form_2D> created_form = new_line(needed_name, x_1, y_1, x_2, y_2);
            created_form.get()->set_border_color(border_color);created_form.get()->set_border_radius(border_radius.to_double());

            // Handle physic
            if(use_physic != 0){
                if(physic_map().size() <= 0){load_physic_map(0, 0);}
                std::shared_ptr<pleos::Graphic::Graphic_Physic> physic = std::make_shared<pleos::Graphic::Graphic_Physic>(created_form, created_form.get()->attached_transform_shared_ptr());
                physic.get()->set_use_gravity(use_gravity);physic.get()->set_static(use_physic == 1);add_physic_object(physic);
                if(use_collision){physic.get()->add_collision(x_1, y_1, x_2, y_2);}
            }
        }
        else if(current_balise_name == "physic") {
            // Get the datas about a physic of the graphic
            std::string needed_attached_object_name = std::string();
            std::vector<Graphic_Collision_Type> collisions;bool use_gravity = false;int use_physic = 1; // 0 = None, 1 = Static, 2 = Dynamic
            scls::Point_2D velocity_start = scls::Point_2D(0, 0);
            for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                if(attributes[j].name == "attached_object" || attributes[j].name == "object"){needed_attached_object_name = attributes.at(j).value;}
                else if(attributes[j].name == "collision") {if(attributes[j].value == std::string("circle")){collisions.push_back(Graphic_Collision_Type::GCT_Circle);}else if(attributes[j].value == std::string("line")){collisions.push_back(Graphic_Collision_Type::GCT_Line);}else{collisions.push_back(Graphic_Collision_Type::GCT_Rect);}}
                else if(attributes[j].name == "gravity" || attributes[j].name == "use_gravity") {use_gravity = (attributes[j].value == std::string("1") || attributes[j].value == std::string("true"));}
                else if(attributes[j].name == "type") {if(attributes[j].value == std::string("static") || attributes[j].value == std::string("1")){use_physic = 1;}else{use_physic = 2;}}
                else if(attributes[j].name == "velocity") {velocity_start = environment.value_point_2d(attributes[j].value);}
            }

            // Add the physic
            if(physic_map().size() <= 0){load_physic_map(0, 0);}
            std::shared_ptr<pleos::__Graphic_Object_Base> needed_object = object_by_name_shared_ptr(needed_attached_object_name);
            if(needed_object.get() == 0) {scls::print("PLEOS Graphic", std::string("The object \"") + needed_attached_object_name + std::string("\" you want to attach to a physic object does not exist."));}
            else {
                std::shared_ptr<pleos::Graphic::Graphic_Physic> physic = std::make_shared<pleos::Graphic::Graphic_Physic>(needed_object, needed_object.get()->attached_transform_shared_ptr());
                physic.get()->set_use_gravity(use_gravity);physic.get()->set_static(use_physic == 1);add_physic_object(physic);
                for(int k = 0;k<static_cast<int>(collisions.size());k++){physic.get()->new_collision(collisions.at(k));}
                physic.get()->set_velocity(velocity_start);
            }
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
            if(current_balise_name == "point"){add_point(Point_2D(graphic_base_shared_ptr(), needed_name, needed_x, needed_y));}
            else if(current_balise_name == "vec" || current_balise_name == "vector"){add_vector(Point_2D(graphic_base_shared_ptr(), needed_name, needed_x, needed_y));}
        }
        else if(current_balise_name == "real_line" || current_balise_name == "number_line"){
            // Get the needed datas
            std::shared_ptr<__Graphic_Object_Base> created_object = std::make_shared<__Graphic_Object_Base>(a_graphic_base);
            created_object.get()->set_width(10);
            for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                if(attributes[j].name == "length") {created_object.get()->set_width(environment.value_number(attributes[j].value));}
                else if(graphic_from_xml_balise_attribute_object(attributes.at(j), created_object, environment, text_style)){}
            }

            // Create the line
            number_line(created_object.get()->x(), created_object.get()->y(), created_object.get()->width());
        }
        else if(current_balise_name == "rect") {
            // Get the datas about a rectangle in the graphic
            scls::Color border_color = scls::Color(0, 0, 0);scls::Fraction border_radius=5;
            scls::Color color = scls::Color(255, 255, 255);
            scls::Fraction height = 0;scls::Fraction width = 0;

            // Create the form
            std::shared_ptr<Form_2D> created_form = new_square(std::string(""), 0, 0, 0, 0);
            for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                if(attributes[j].name == "background_color" || attributes[j].name == "color") {color = scls::Color::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "border_color") {border_color = scls::Color::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "border_radius") {border_radius = scls::Fraction::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "height") {height = scls::Fraction::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "width") {width = scls::Fraction::from_std_string(attributes[j].value);}
                else if(graphic_from_xml_balise_attribute_object(attributes.at(j), created_form, environment, text_style)){}
            }

            // Add the form
            object = created_form;
            created_form.get()->set_height(height);
            created_form.get()->set_width(width);
            created_form.get()->set_x(created_form.get()->x() + width / 2);
            created_form.get()->set_y(created_form.get()->y() + height / 2);
            created_form.get()->set_border_color(border_color);created_form.get()->set_border_radius(border_radius.to_double());
            created_form.get()->set_color(color);
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
                    std::shared_ptr<Circle> created_circle = new_circle(needed_name + std::to_string(j), random_x, random_y, needed_radius);
                    created_circle.get()->set_border_color(border_color);created_circle.get()->set_border_radius(border_radius.to_double());created_circle.get()->set_color(color);
                }
            }
        }
        else if(current_balise_name == "text") {
            // Get the datas about a text of the graphic
            std::string needed_content = std::string();scls::Fraction needed_x = 0;scls::Fraction needed_y = 0;//scls::Fraction radius = 1;
            std::shared_ptr<scls::Text_Style> current_style = text_style.get()->new_child();
            for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                if(attributes[j].name == "background_color") {current_style.get()->set_background_color(scls::Color::from_std_string(attributes[j].value));}
                else if(attributes[j].name == "content") {needed_content = attributes[j].value;}
                else if(attributes[j].name == "font_size") {current_style.get()->set_font_size(std::stoi(attributes[j].value));}
                else if(attributes[j].name == "x") {needed_x = scls::Fraction::from_std_string(attributes[j].value).to_double();}
                else if(attributes[j].name == "y") {needed_y = scls::Fraction::from_std_string(attributes[j].value).to_double();}
            }
            // Add the text
            needed_content += scls::format_string_break_line(xml.get()->text(), std::string());
            new_text(needed_content, needed_x, needed_y, current_style);
        }
        else if(current_balise_name == "texture" || current_balise_name == "texture_object") {
            // Add the propagation area
            std::shared_ptr<Graphic_Texture> new_object = new_texture_object<Graphic_Texture>(std::string(""), 0, 0);
            object = new_object;
            for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                if(graphic_from_xml_balise_attribute_texture_object(attributes[j], new_object, environment, text_style)){}
                else if(graphic_from_xml_balise_attribute_object(attributes[j], new_object, environment, text_style)) {}
            }
        }
        else if(current_balise_name == "text_style") {
            // Get the datas about the style text of the graphic
            std::string needed_content = std::string();
            for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                if(attributes[j].name == "background_color") {text_style.get()->set_background_color(scls::Color::from_std_string(attributes[j].value));}
                else if(attributes[j].name == "font_size") {text_style.get()->set_font_size(scls::Fraction::from_std_string(attributes[j].value).to_double());}
            }
        }

        return object;
    }
    void Graphic::__graphic_from_xml_balises(std::shared_ptr<scls::__XML_Text_Base> xml, Text_Environment& environment, std::shared_ptr<scls::Text_Style> text_style, int graphic_width_in_pixel, int graphic_height_in_pixel){
	    // Handle a lot of balises
        for(int i = 0;i<static_cast<int>(xml->sub_texts().size());i++) {
            // Handle utilities
            Utility_Balise utility = utilities_balise(xml->sub_texts().at(i));

            if(utility.type == BALISE_REPEAT) {
                environment.add_repetition();
                scls::__Formula_Base::Unknown* needed_variable = environment.create_unknown("x");
                needed_variable->set_value(utility.value_start);scls::Fraction step = (utility.value_end - utility.value_start) / (utility.times - 1);
                for(int j = 0;j<utility.times;j++){
                    environment.set_repetition(j);
                    Graphic::__graphic_from_xml_balises(xml->sub_texts().at(i), environment, text_style, graphic_width_in_pixel, graphic_height_in_pixel);
                    (*needed_variable->value) += step;
                }
                environment.remove_repetition();
            }
            else{graphic_from_xml_balise(xml->sub_texts().at(i), environment, text_style);}
        }
	}
    void Graphic::graphic_from_xml(std::shared_ptr<scls::__XML_Text_Base> xml, scls::Text_Style needed_style, int graphic_width_in_pixel, int graphic_height_in_pixel){graphic_from_xml(xml, needed_style, 0, 0, 0);}
    void Graphic::graphic_from_xml(std::shared_ptr<scls::__XML_Text_Base> xml, scls::Text_Style needed_style, std::shared_ptr<Text_Environment> environment){graphic_from_xml(xml, needed_style, environment, 0, 0);}
    void Graphic::graphic_from_xml(std::shared_ptr<scls::__XML_Text_Base> xml, scls::Text_Style needed_style, std::shared_ptr<Text_Environment> environment, int graphic_width_in_pixel, int graphic_height_in_pixel) {
        set_environment(environment);
        style()->set_border_width(1);
	    if(graphic_height_in_pixel <= 0){graphic_height_in_pixel = 200;}bool graphic_height_in_pixel_used = false;
        if(graphic_width_in_pixel <= 0){graphic_width_in_pixel = 200;}bool graphic_width_in_pixel_used = false;
        bool use_ratio_base = false;
        std::vector<scls::XML_Attribute>& attributes = xml.get()->xml_balise_attributes();
        for(int j = 0;j<static_cast<int>(attributes.size());j++) {
            if(attributes[j].name == "height") {graphic_height_in_pixel = scls::Fraction::from_std_string(attributes[j].value).to_double();graphic_height_in_pixel_used = true;}
            else if(attributes[j].name == "ratio_base") {use_ratio_base = true;}
            else if(attributes[j].name == "width") {graphic_width_in_pixel = scls::Fraction::from_std_string(attributes[j].value).to_double();graphic_width_in_pixel_used = true;}
        }

        // Get the datas about the graphic
        set_scale(0, 0);
        scls::__Formula_Base::Formula graphic_width = 10;
        scls::__Formula_Base::Formula graphic_height = graphic_width * scls::Fraction(graphic_height_in_pixel, graphic_width_in_pixel);
        scls::Fraction graphic_x = 0;scls::Fraction graphic_y = 0;
        std::shared_ptr<scls::Text_Style> text_style = std::make_shared<scls::Text_Style>();

        // Handle a lot of balises
        if(environment.get() == 0){Text_Environment temp;__graphic_from_xml_balises(xml, temp, text_style, graphic_width_in_pixel, graphic_height_in_pixel);}
        else{__graphic_from_xml_balises(xml, *environment.get(), text_style, graphic_width_in_pixel, graphic_height_in_pixel);}

        // Set the datas
        graphic_width = width_formula();
        graphic_height = height_formula();
        if(graphic_width != 0 && graphic_height == 0){graphic_height = graphic_width * scls::Fraction(graphic_height_in_pixel, graphic_width_in_pixel);}
        else if(graphic_width == 0 && graphic_height != 0){graphic_width = graphic_height * scls::Fraction(graphic_width_in_pixel, graphic_height_in_pixel);}
        else if(graphic_width == 0 && graphic_height == 0){graphic_width = 10;graphic_height = graphic_width * scls::Fraction(graphic_height_in_pixel, graphic_width_in_pixel);}
        set_middle(graphic_x.to_double(), graphic_y.to_double());
        set_scale(graphic_width, graphic_height, width_used(), height_used());
        set_image_size(graphic_width_in_pixel, graphic_height_in_pixel);

        // Get the image
        if(use_ratio_base){
            if(graphic_height_in_pixel_used&&!graphic_width_in_pixel_used){graphic_width_in_pixel = static_cast<double>(graphic_height_in_pixel) * (graphic_width.value_to_double() / graphic_height.value_to_double());}
            else if(!graphic_height_in_pixel_used&&graphic_width_in_pixel_used){graphic_height_in_pixel = static_cast<double>(graphic_width_in_pixel) * (graphic_height.value_to_double() / graphic_width.value_to_double());}
        }
    }

    //******************
    //
    // Physic handling
    //
    //******************

    // Used gravity
    scls::Point_2D gravity = scls::Point_2D(0, scls::Fraction(-98, 10));
    //scls::Point_2D gravity = scls::Point_2D(0, 0);

    // Checks if a collision occurs with an another collision
    struct Collision_Result{Collision_Result(){};Collision_Result(std::shared_ptr<Collision> c_1):Collision_Result(c_1, std::shared_ptr<Collision>()){};Collision_Result(std::shared_ptr<Collision> c_1, std::shared_ptr<Collision> c_2):collision_1(c_1),collision_2(c_2){};std::shared_ptr<Collision> collision_1;std::shared_ptr<Collision> collision_2;};;;;
    void __check_collision_circle_circle_one(Graphic_Collision* collision, Graphic_Collision* collision_other, Graphic::Graphic_Physic* object, Graphic::Graphic_Physic* other_object, std::shared_ptr<Collision_Circle> to_return){
        // Calculate the position
        double distance = collision->position_next().distance(collision_other->position_next());
        scls::Point_2D position = collision->position_next();scls::Point_2D other_position = collision_other->position_next();
        //position.set_y(position.y() * (collision_other->attached_transform()->scale_x() / collision_other->attached_transform()->scale_y()));
        scls::Point_2D position_from_here = (other_position - position).normalized();
        scls::Point_2D position_from_other = (position - other_position).normalized();
        if(distance < collision_other->attached_transform()->scale_x() / 2){position_from_here *= -1;}
        if(distance < collision->attached_transform()->scale_x() / 2){position_from_other *= -1;}
        scls::Point_2D position_contact = position + (position_from_here) * scls::Fraction::from_double(scls::oval_radius(collision->attached_transform()->scale_x() / 2, collision->attached_transform()->scale_y() / 2, scls::vector_2d_angle(position_from_here)));

        // Calculate the angle
        double angle_tangent = scls::vector_2d_angle(position_from_other);
        double angle_tangent_other = scls::vector_2d_angle(position_from_here);

        // Calculate the velocity
        scls::Point_2D velocity = collision->attached_transform()->velocity();
        scls::Point_2D velocity_other = collision_other->attached_transform()->velocity();
        double velocity_total = (velocity_other.norm() + velocity.norm());
        if(other_object->is_static()){velocity_total*=2;}
        scls::Point_2D velocity_from_here = (velocity_other - velocity) / 2;
        scls::Point_2D velocity_from_other = (velocity - velocity_other) / 2;
        to_return.get()->angle = angle_tangent_other;

        // Calculate the velocity of the object
        if(!object->is_static()) {
            #define MULTIPLIER_REDUCE 1;scls::Fraction(100, 100)
            scls::Fraction multiplier = scls::Fraction::from_double(velocity_total) / 2;
            scls::Point_2D new_velocity = (position_from_other - (velocity_from_here - position_from_other));
            //new_velocity += position_from_other.rotated(angle_tangent - needed_angle_other) * multiplier;
            if(distance < scls::oval_radius(collision_other->attached_transform()->scale_x() / 2, collision_other->attached_transform()->scale_y() / 2, scls::vector_2d_angle(position_from_other))){multiplier *= -1;}
            new_velocity.normalize();new_velocity *= multiplier * MULTIPLIER_REDUCE;
            to_return.get()->acceleration = object->velocity() * -1 + new_velocity;
        }
    }
    Collision_Result __check_collision_circle_circle(Graphic_Collision* collision_1, Graphic_Collision* collision_2, Graphic::Graphic_Physic* object_1, Graphic::Graphic_Physic* object_2){
        // Change the positions if needed
        scls::Point_2D position_1 = collision_1->position_next();scls::Point_2D position_2 = collision_2->position_next();
        position_1.set_y(position_1.y() * (collision_2->attached_transform()->scale_x() / collision_2->attached_transform()->scale_y()));

        // Get the distance
        double distance = position_1.distance(position_2);
        double max_distance = std::abs(collision_1->attached_transform()->scale_x().to_double() / 2.0 + collision_2->attached_transform()->scale_x().to_double() / 2.0);
        double min_distance = std::abs(collision_1->attached_transform()->scale_x().to_double() / 2.0 - collision_2->attached_transform()->scale_x().to_double() / 2.0);
        if(!(distance < max_distance && distance > min_distance)){return Collision_Result();}

        // Set collision_1 as the biggest collision
        bool inverse = true;
        if(collision_1->attached_transform()->scale_x() < collision_2->attached_transform()->scale_x()){Graphic_Collision* temp = collision_1;collision_1 = collision_2;collision_2=temp;Graphic::Graphic_Physic* temp_2 = object_1;object_1 = object_2;object_2=temp_2;}
        else{inverse = false;}

        // Get the datas about the collision
        // Collision 1
        std::shared_ptr<Collision_Circle> to_return_1 = std::make_shared<Collision_Circle>();
        to_return_1.get()->happens = true;
        to_return_1.get()->__other_object = collision_2->attached_object_weak_ptr();
        // Collision 2
        std::shared_ptr<Collision_Circle> to_return_2 = std::make_shared<Collision_Circle>();
        to_return_2.get()->happens = true;
        to_return_2.get()->other_collision = to_return_1;
        to_return_2.get()->__other_object = collision_1->attached_object_weak_ptr();

        // Calculate the velocity of the objects
        __check_collision_circle_circle_one(collision_1, collision_2, object_1, object_2, to_return_1);
        __check_collision_circle_circle_one(collision_2, collision_1, object_2, object_1, to_return_2);

        // Return the result
        if(inverse){return Collision_Result(to_return_2, to_return_1);}
        return Collision_Result(to_return_1, to_return_2);
    }
    Collision_Result __check_collision_rect_line(Graphic_Collision* collision_rect, Graphic_Collision* collision_line, Graphic::Graphic_Physic* dynamic_object_1){
        // Check bottom collision
        scls::Crossing_Datas_Segment datas_bottom = scls::check_crossing_segment(collision_rect->min_x().to_double(), collision_rect->min_y().to_double(), collision_rect->max_x().to_double(), collision_rect->min_y().to_double(), collision_line->x_1().to_double(), collision_line->y_1().to_double(), collision_line->x_2().to_double(), collision_line->y_2().to_double());

        // Check right collision
        scls::Crossing_Datas_Segment datas_right = scls::check_crossing_segment(collision_rect->max_x().to_double(), collision_rect->max_y().to_double(), collision_rect->max_x().to_double(), collision_rect->min_y().to_double(), collision_line->x_1().to_double(), collision_line->y_1().to_double(), collision_line->x_2().to_double(), collision_line->y_2().to_double());

        // Assert
        if(!datas_bottom.crossed_in_segment && !datas_right.crossed_in_segment){return Collision_Result();}

        // Get the differences
        scls::Fraction slope_x = (collision_line->x_2() - collision_line->x_1()).abs();
        scls::Fraction slope_y = (collision_line->y_2() - collision_line->y_1()).abs();
        scls::Fraction temp = 0;scls::normalize_3d(slope_x, temp, slope_y);
        std::shared_ptr<Collision_Rect_Rect> to_return = std::make_shared<Collision_Rect_Rect>();
        to_return.get()->happens = true;
        if(datas_bottom.crossed_in_segment){to_return.get()->side_bottom = true;}
        if(datas_right.crossed_in_segment){to_return.get()->side_right = true;}

        // Check the movement
        if(to_return.get()->side_bottom && to_return.get()->side_right) {
            scls::Fraction force_distribution = scls::Fraction(1, 2);
            std::cout << slope_x << " " << slope_y << " " << dynamic_object_1->velocity_x() << " " << dynamic_object_1->velocity_y() << std::endl;

            // Handle next movement
            if(dynamic_object_1->next_movement_x() > 0 && dynamic_object_1->next_movement_y() < 0){
                // Handle both axis at once
                dynamic_object_1->accelerate_x(dynamic_object_1->velocity_x() * slope_y * force_distribution * -1);
                dynamic_object_1->accelerate_y(dynamic_object_1->velocity_x() * slope_y * force_distribution);
            }
            else {
                if(dynamic_object_1->next_movement_x() > 0){dynamic_object_1->accelerate_y(dynamic_object_1->velocity_x() * force_distribution);dynamic_object_1->remove_x_velocity();}
                if(dynamic_object_1->next_movement_y() < 0){dynamic_object_1->accelerate_x(dynamic_object_1->velocity_y() * force_distribution);dynamic_object_1->remove_y_velocity();}
            }
        }
        if(to_return.get()->side_bottom && !to_return.get()->side_right) {if(dynamic_object_1->velocity().y() < 0){dynamic_object_1->remove_y_velocity();}}
        if(to_return.get()->side_left) {if(dynamic_object_1->velocity().x() < 0){dynamic_object_1->remove_x_velocity();}}
        if(to_return.get()->side_right && !to_return.get()->side_bottom) {if(dynamic_object_1->velocity().x() > 0){dynamic_object_1->remove_x_velocity();}}
        if(to_return.get()->side_top) {if(dynamic_object_1->velocity().y() > 0){dynamic_object_1->remove_y_velocity();}}

        // Return the result
        return Collision_Result(to_return);
    }
    Collision_Result __check_collision_rect_rect(Graphic_Collision* collision_1, Graphic_Collision* collision_2, Graphic::Graphic_Physic* dynamic_object_1){
        // Check X
        bool x_1 = (collision_1->max_x_next() > collision_2->min_x_next() && collision_2->max_x_next() > collision_1->max_x_next());
        bool x_2 = (collision_2->max_x_next() > collision_1->min_x_next() && collision_1->max_x_next() > collision_2->max_x_next());
        bool x_3 = (collision_2->max_x_next() >= collision_1->max_x_next() && collision_1->min_x_next() >= collision_2->min_x_next());
        bool x_4 = (collision_1->max_x_next() >= collision_2->max_x_next() && collision_2->min_x_next() >= collision_1->min_x_next());
        if(!(x_1 || x_2 || x_3 || x_4)){return Collision_Result();}

        // Check Y
        bool y_1 = (collision_1->max_y_next() > collision_2->min_y_next() && collision_2->max_y_next() > collision_1->max_y_next());
        bool y_2 = (collision_2->max_y_next() > collision_1->min_y_next() && collision_1->max_y_next() > collision_2->max_y_next());
        bool y_3 = (collision_2->max_y_next() >= collision_1->max_y_next() && collision_1->min_y_next() >= collision_2->min_y_next());
        bool y_4 = (collision_1->max_y_next() >= collision_2->max_y_next() && collision_2->min_y_next() >= collision_1->min_y_next());
        if(!(y_1 || y_2 || y_3 || y_4)){return Collision_Result();}

        // Get the differences
        std::shared_ptr<Collision_Rect_Rect> to_return = std::make_shared<Collision_Rect_Rect>();
        to_return.get()->happens = true;

        // X
        double x_diff = collision_2->min_x_next().to_double() - collision_1->max_x_next().to_double();
        double x_diff_temp = collision_1->min_x_next().to_double() - collision_2->max_x_next().to_double();
        if(std::abs(x_diff) > std::abs(x_diff_temp)){x_diff = x_diff_temp;}
        // Y
        double y_diff = collision_2->min_y_next().to_double() - collision_1->max_y_next().to_double();
        double y_diff_temp = collision_1->min_y_next().to_double() - collision_2->max_y_next().to_double();
        if(std::abs(y_diff) > std::abs(y_diff_temp)){y_diff = y_diff_temp;}

        // Returns the good value
        if(std::abs(x_diff) > std::abs(y_diff)){
            // Returns a Y
            if(y_1 || y_4){to_return.get()->side_top = true;}
            else{to_return.get()->side_bottom = true;}

            // Set the distance
            to_return.get()->distance = std::abs(y_diff);
        }
        else {
            // Returns a X
            if(x_2 || x_3){to_return.get()->side_left = true;}
            else{to_return.get()->side_right = true;}

            // Set the distance
            to_return.get()->distance = std::abs(x_diff);
        }

        // Check the movement
        if(to_return.get()->side_bottom) {
            // Friction
            if(dynamic_object_1->velocity_y() < 0){dynamic_object_1->accelerate_x(dynamic_object_1->velocity_x() * scls::Fraction(-1, 100));}

            // Contact
            if(dynamic_object_1->velocity_y() < 0){dynamic_object_1->set_velocity_y(scls::Fraction::from_double(to_return.get()->distance));}
        }
        if(to_return.get()->side_left) {if(dynamic_object_1->next_movement_x() < 0){dynamic_object_1->set_velocity_x(scls::Fraction::from_double(to_return.get()->distance));}}
        if(to_return.get()->side_right) {if(dynamic_object_1->next_movement_x() > 0){dynamic_object_1->set_velocity_x(scls::Fraction::from_double(-to_return.get()->distance));}}
        if(to_return.get()->side_top) {if(dynamic_object_1->velocity_y() > 0){dynamic_object_1->set_velocity_y(scls::Fraction::from_double(-to_return.get()->distance));}}

        // Return the result
        return Collision_Result(to_return);
    };
    Collision_Result __check_collision_circle_line(Graphic_Collision* collision_1, Graphic_Collision* collision_2, Graphic::Graphic_Physic* object_1, Graphic::Graphic_Physic* object_2){
        // Get the angle
        scls::Point_2D line_end = scls::Point_2D(collision_2->direct_x_2(), collision_2->direct_y_2());
        scls::Point_2D line_start = scls::Point_2D(collision_2->direct_x_1(), collision_2->direct_y_1());
        scls::Point_2D line_vector = line_end - line_start;
        scls::Point_2D position_1 = collision_1->position_next();

        // Get the contact point
        scls::Point_2D contact_point = scls::orthogonal_projection(line_start, line_end, position_1);
        // Get the distance
        double distance = contact_point.distance(position_1);
        double min_distance = std::abs(collision_1->attached_transform()->scale_x().to_double() / 2.0);
        if(distance > min_distance){return Collision_Result();}
        if(!contact_point.in_rect(line_start, line_vector)){return Collision_Result();}

        // Get the datas about the collision
        // Collision 1
        std::shared_ptr<Collision_Circle> to_return_1 = std::make_shared<Collision_Circle>();
        to_return_1.get()->happens = true;
        to_return_1.get()->__other_object = collision_2->attached_object_weak_ptr();
        // Collision 2
        std::shared_ptr<Collision_Circle> to_return_2 = std::make_shared<Collision_Circle>();
        to_return_2.get()->happens = true;
        to_return_2.get()->other_collision = to_return_1;
        to_return_2.get()->__other_object = collision_1->attached_object_weak_ptr();

        // Calculate the needed angle
        scls::Point_2D line_vector_normal = line_vector.rotated(90);
        scls::Point_2D position_from_1 = (contact_point - position_1).normalized();
        scls::Point_2D position_from_2 = (position_1 - contact_point).normalized();
        if(distance < collision_1->attached_transform()->scale_x() / 2){position_from_2 *= -1;}
        //double kept_proportion = 1.0;
        scls::Point_2D velocity_from_1 = collision_1->attached_transform()->velocity();
        double angle_tangent_1 = scls::vector_2d_angle(position_from_1);
        to_return_1.get()->angle = scls::vector_2d_angle(position_from_1);
        to_return_2.get()->angle = scls::vector_2d_angle(position_from_2);

        double difference_angle = scls::vector_2d_angle(velocity_from_1) - scls::vector_2d_angle(line_vector_normal);
        scls::Point_2D new_velocity_direction = line_vector_normal.rotated(difference_angle * (180.0/SCLS_PI)).normalized() * -1;

        // Calculate the velocity of the object 1
        scls::Fraction multiplier = scls::Fraction::from_double(velocity_from_1.norm());
        scls::Point_2D new_velocity = new_velocity_direction * multiplier;
        to_return_1.get()->acceleration = object_1->velocity() * -1 + new_velocity;

        // Return the result
        return Collision_Result(to_return_1, to_return_2);
    }
    Collision_Result __check_collision(Graphic_Collision* collision_1, Graphic_Collision* collision_2, Graphic::Graphic_Physic* object_1, Graphic::Graphic_Physic* object_2) {
        // Asserts
        if(collision_2 == 0 || collision_2->attached_transform() == collision_1->attached_transform()){return Collision_Result();}

        // Both objects are rect
        if(collision_1->type() == Graphic_Collision_Type::GCT_Rect && collision_2->type() == Graphic_Collision_Type::GCT_Rect) {
            Collision_Result current_result = __check_collision_rect_rect(collision_1, collision_2, object_1);
            if(current_result.collision_1.get() == 0 || current_result.collision_1.get()->happens) {return current_result;}
        }
        else if(collision_1->type() == Graphic_Collision_Type::GCT_Rect && collision_2->type() == Graphic_Collision_Type::GCT_Line) {
            Collision_Result current_result = __check_collision_rect_line(collision_1, collision_2, object_1);
            if(current_result.collision_1.get() == 0 || current_result.collision_1.get()->happens) {return current_result;}
        }
        else if(collision_1->type() == Graphic_Collision_Type::GCT_Circle && collision_2->type() == Graphic_Collision_Type::GCT_Circle) {
            Collision_Result current_result = __check_collision_circle_circle(collision_1, collision_2, object_1, object_2);
            if(current_result.collision_1.get() == 0 || current_result.collision_1.get()->happens) {return current_result;}
        }
        else if(collision_1->type() == Graphic_Collision_Type::GCT_Circle && collision_2->type() == Graphic_Collision_Type::GCT_Line) {
            Collision_Result current_result = __check_collision_circle_line(collision_1, collision_2, object_1, object_2);
            if(current_result.collision_1.get() == 0 || current_result.collision_1.get()->happens) {return current_result;}
        }

        return Collision_Result();
    }

    // Add a line / rect collision to the graphic object
    void Graphic::Graphic_Physic::add_collision(std::shared_ptr<Graphic_Collision> collision){a_collisions.push_back(collision);};
    void Graphic::Graphic_Physic::add_collision(scls::Fraction x_1, scls::Fraction y_1, scls::Fraction x_2, scls::Fraction y_2){
        a_collisions.push_back(std::make_shared<Graphic_Collision>(a_attached_object, a_attached_transform));
        std::shared_ptr<Graphic_Collision> collision=a_collisions.at(a_collisions.size()-1);
        collision.get()->set_x_1(x_1);collision.get()->set_y_1(y_1);
        collision.get()->set_x_2(x_2);collision.get()->set_y_2(y_2);
        collision.get()->set_type(Graphic_Collision_Type::GCT_Line);
    };

    // Checks if a collision occurs with an another object
    void Graphic::Graphic_Physic::check_collision(Graphic_Collision* collision, Graphic::Graphic_Physic* other_object) {
        // Asserts
        if(collision == 0 || collision->attached_transform() == attached_transform()){return;}

        // Check each collision
        for(int i = 0;i<static_cast<int>(a_collisions.size());i++) {
            bool already_in = false;
            for(int j = 0;j<static_cast<int>(a_current_collisions_results.size());j++){if(collision->attached_object() == a_current_collisions_results.at(j).get()->other_object()){already_in=true;break;}}
            if(already_in){continue;}

            Collision_Result current_result = __check_collision(a_collisions[i].get(), collision, this, other_object);
            if(current_result.collision_1.get() != 0 && current_result.collision_1.get()->happens){a_current_collisions_results.push_back(current_result.collision_1);}
            if(current_result.collision_2.get() != 0 && current_result.collision_2.get()->happens){other_object->a_current_collisions_results.push_back(current_result.collision_2);}
        }
    }

    // Returns a new a collision to the graphic object
    std::shared_ptr<Graphic_Collision> Graphic::Graphic_Physic::new_collision(Graphic_Collision_Type type){
        std::shared_ptr<Graphic_Collision>to_return=std::make_shared<Graphic_Collision>(a_attached_object, a_attached_transform);
        to_return.get()->set_type(type);add_collision(to_return);
        to_return.get()->set_height(a_attached_transform.lock().get()->scale_y());
        to_return.get()->set_width(a_attached_transform.lock().get()->scale_x());
        return to_return;
    };

    // Soft resets the object
    void Graphic::Graphic_Physic::soft_reset(){a_current_collisions_results.clear();if(attached_transform() != 0){attached_transform()->soft_reset();}};

    // Returns the needed XML text to generate this graphic
    std::string Graphic::Graphic_Physic::to_xml_text() {
        std::string to_return = std::string("<physic");

        // Type of physic
        if(is_static()){to_return += std::string(" type=1");}else{to_return += std::string(" type=2");}

        // Attached object
        if(attached_object() != 0){to_return += std::string(" attached_object=\"") + attached_object()->name() + std::string("\"");}

        // Gravity
        if(use_gravity()){to_return += std::string(" gravity=1");}else{to_return += std::string(" gravity=0");}

        // Velocity
        if(a_velocity_start.x() != 0 || a_velocity_start.y() != 0){to_return += std::string(" velocity=(")+a_velocity_start.x().to_std_string(0)+std::string(",")+a_velocity_start.y().to_std_string(0)+std::string(")");}

        // Collision
        for(int i = 0;i<static_cast<int>(a_collisions.size());i++){
            to_return += std::string(" collision=");
            if(a_collisions.at(i).get()->type() == Graphic_Collision_Type::GCT_Circle){to_return += std::string("circle");}
            else if(a_collisions.at(i).get()->type() == Graphic_Collision_Type::GCT_Line){to_return += std::string("line");}
        }

        // Finish the text
        to_return += std::string(">");
        return to_return;
    }

    // Calculates the point of the trajectory of the function
    std::vector<scls::Point_2D> Graphic::Graphic_Physic::trajectory_points(int point_number, scls::Fraction time_separation) {
        // Current position / velocity
        scls::Point_2D current_position = position(5);
        scls::Point_2D current_velocity = velocity(5);
        std::vector<scls::Point_2D> datas = std::vector<scls::Point_2D>(point_number);

        // Get the points
        for(int i = 0;i<point_number;i++) {
            current_position += current_velocity * time_separation;
            current_velocity += gravity * time_separation;
            datas[i] = current_position;
        }

        // Returns the result
        return datas;
    }

    // Loads 100 X 100 physic map
    void Graphic::load_physic_map(int middle_loading_x, int middle_loading_y) {
        // Get the needed datas
        int height = 100;int width = 100;
        a_physic_map_start_x = middle_loading_x - width / 2;
        a_physic_map_start_y = middle_loading_y - height / 2;

        // Create the cases
        a_physic_map = std::vector<std::vector<std::shared_ptr<Physic_Case>>>(width, std::vector<std::shared_ptr<Physic_Case>>(height, std::make_shared<Physic_Case>()));
    }

    // Returns a physic case by its coordinates
    Graphic::Physic_Case* Graphic::physic_case(int x, int y){
        if((-a_physic_map_start_x) + x < 0 || (-a_physic_map_start_x) + x >= static_cast<int>(a_physic_map.size())){return 0;}
        if((-a_physic_map_start_y) + y < 0 || (-a_physic_map_start_y) + y >= static_cast<int>(a_physic_map[(-a_physic_map_start_x) + x].size())){return 0;}
        return a_physic_map[(-a_physic_map_start_x) + x][(-a_physic_map_start_y) + y].get();
    };

    //******************
    //
    // Built-in features
    //
    //******************

    // Creates a number line
    void Graphic::number_line(scls::Fraction x, scls::Fraction y, scls::Fraction length) {
        // Create the text
        std::shared_ptr<scls::Text_Style> current_style = std::make_shared<scls::Text_Style>();
        new_text(std::string("..."), x - length / 2, y, current_style);
        new_text(std::string("..."), x + length / 2, y, current_style);

        // Create the line
        length *= scls::Fraction(4, 5);
        std::shared_ptr<Form_2D> line = new_line(std::string("number_line"), x - length / 2, y, x + length / 2, y);
        line.get()->set_border_color(scls::Color(0, 0, 0));
    }

    //******************
    //
    // Graphic_Object main members
    //
    //******************

    // Graphic constructor
    Graphic_Object::Graphic_Object(scls::_Window_Advanced_Struct& window, std::string name, std::weak_ptr<scls::GUI_Object> parent):scls::GUI_Object(window, name, parent){update_texture();}

    // Creates the background texture
    void Graphic_Object::create_background_texture(){a_datas.get()->create_background_texture(width_in_pixel(), height_in_pixel());}
    void Graphic_Object::create_background_texture(int width, int height){a_datas.get()->create_background_texture(width, height);}
    void Graphic_Object::create_background_texture(std::string path){create_background_texture(path, width_in_pixel(), height_in_pixel());}
    void Graphic_Object::create_background_texture(std::string path, int width, int height){
        std::shared_ptr<scls::__Image_Base> image_path = std::make_shared<scls::__Image_Base>(path);
        if(width == image_path.get()->width() && height == image_path.get()->height()){a_datas.get()->create_background_texture(image_path);}
        else {
            double height_ratio = static_cast<double>(image_path.get()->height()) / static_cast<double>(height);
            double width_height_ratio = static_cast<double>(width) / static_cast<double>(height);
            double width_height_ratio_image = static_cast<double>(image_path.get()->width()) / static_cast<double>(image_path.get()->height());
            std::shared_ptr<scls::__Image_Base> base = std::make_shared<scls::__Image_Base>(width, height, scls::Color(0, 0, 0, 0));
            base.get()->paste_center(image_path.get()->resize_adaptative(static_cast<double>(height) * width_height_ratio_image * (height_ratio / width_height_ratio), height));
            a_datas.get()->create_background_texture(base);
        }
    }

    // Render the object
    void Graphic_Object::render(bool render_children, glm::vec3 scale_multiplier) {scls::GUI_Object::render(true, scale_multiplier);}

    // Scale the GUI Object
    void Graphic_Object::Graphic_GUI_Object::scale(Graphic* graphic, int image_width, int image_height){
        // Get the needed values
        int new_height = std::ceil(graphic->pixel_by_case_y() * height().to_double());
        int new_x = graphic->graphic_x_to_pixel_x((x() - width() / 2), image_width);
        int new_y = graphic->graphic_y_to_pixel_y((y() - height() / 2), image_height);
        int new_width = std::ceil(graphic->pixel_by_case_x() * width().to_double());

        // Check if the object have to be rendered or not
        if(new_x > image_width || new_y > image_height || new_x < -new_width || new_y < -new_height){a_object.get()->set_visible(false);return;}
        else{a_object.get()->set_visible(true);}

        // Set the values
        a_object.get()->set_height_in_pixel(new_height);
        a_object.get()->set_width_in_pixel(new_width);
        a_object.get()->set_x_in_pixel(new_x);
        a_object.get()->set_y_in_pixel(new_y);
        a_object.get()->set_texture_scale_x(width().to_double() / a_texture_scale_x);
        a_object.get()->set_texture_scale_y(height().to_double() / a_texture_scale_y);
    };

    // Updates the object
    int __graphic_object_render_number = 0;
    std::vector<std::shared_ptr<Point_2D>> __temp_vectors;
    void Graphic_Object::update(){for(int i = 0;i<static_cast<int>(a_gui_objects.size());i++) {if(a_gui_objects[i].get() != 0) {a_gui_objects[i].get()->update();}}};
    void Graphic_Object::update_event() {update_event(window_struct().delta_time());}
    void Graphic_Object::update_event(double needed_delta_time) {
        // Update the object
        for(int i = 0;i<static_cast<int>(a_gui_objects.size());i++) {
            if(a_gui_objects[i].get() != 0) {
                a_gui_objects[i].get()->update_event();
            }
        }

        // Update the physic
        __temp_vectors.clear();points().clear();
        int needed_update_physic = 0;if(update_physic_with_events()){needed_update_physic = update_physic(needed_delta_time);}
        /*for(int i = 0;i<static_cast<int>(a_datas.get()->physic_objects().size());i++) {
            if(a_datas.get()->physic_objects().at(i).get()->is_static()){continue;}
            std::vector<scls::Point_2D> needed_points = a_datas.get()->physic_objects().at(i).get()->trajectory_points(100, scls::Fraction(1, 100));
            for(int j = 0;j<static_cast<int>(needed_points.size());j++){
                std::shared_ptr<Vector> current_point = new_point(std::string("trajectory_") + std::to_string(j), needed_points.at(j));
                current_point.get()->set_color(scls::Color(255, 0, 0));
                __temp_vectors.push_back(current_point);
            }
        }//*/

        // Move the plane
        bool modified = (needed_update_physic > 0);
        if(is_focused()) {
            scls::Fraction speed = scls::Fraction(round(5.0 * needed_delta_time * 10000.0), 10000);
            if(window_struct().key_pressed("q")){middle_x_add(speed * -1);modified = true;}
            if(window_struct().key_pressed("d")){middle_x_add(speed);modified = true;}
            if(window_struct().key_pressed("z")){middle_y_add(speed);modified = true;}
            if(window_struct().key_pressed("s")){middle_y_add(speed * -1);modified = true;}
            // Zoom or unzoom
            double zoom_speed = 30 * needed_delta_time;
            if(window_struct().key_pressed("w")){pixel_by_case_x_add(zoom_speed * -1);pixel_by_case_y_add(zoom_speed * -1);modified = true;}
            if(window_struct().key_pressed("c")){pixel_by_case_x_add(zoom_speed);pixel_by_case_y_add(zoom_speed);modified = true;}
        }

        // Handle clicks
        if(is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
            if(operation_at_click() == PLEOS_OPERATION_VECTOR) {
                // Add a vector
                double needed_x = window_struct().mouse_x() - x_in_absolute_pixel();
                double needed_y = window_struct().mouse_y() - (window_struct().window_height() - (y_in_absolute_pixel() + height_in_pixel()));
                scls::Fraction movement_x = pixel_x_to_graphic_x(needed_x, texture()->image_shared_ptr());
                scls::Fraction movement_y = pixel_y_to_graphic_y(needed_y, texture()->image_shared_ptr());
                add_vector(Point_2D(graphic_base_shared_ptr(), "a", movement_x, movement_y));
                a_created_vectors_at_click.push_back(vectors().at(vectors().size() - 1));
                modified = true;
            }
            else if(operation_at_click() == PLEOS_OPERATION_POINT) {
                // Add a point
                double needed_x = window_struct().mouse_x() - x_in_absolute_pixel();
                double needed_y = window_struct().mouse_y() - (window_struct().window_height() - (y_in_absolute_pixel() + height_in_pixel()));
                scls::Fraction movement_x = pixel_x_to_graphic_x(needed_x, texture()->image_shared_ptr());
                scls::Fraction movement_y = pixel_y_to_graphic_y(needed_y, texture()->image_shared_ptr());
                Point_2D needed_vector = Point_2D(graphic_base_shared_ptr(), "a", movement_x, movement_y);
                needed_vector.set_type(Vector_Type::VT_Point);
                add_vector(needed_vector);
                a_created_vectors_at_click.push_back(vectors().at(vectors().size() - 1));
                modified = true;
            }
            else if(operation_at_click() == PLEOS_OPERATION_FORM) {
                // Add a form
                // Check if the form is created
                if(current_form_2d() == 0){a_current_form_2d=std::make_shared<Form_2D>(graphic_base_shared_ptr(), "Forme");add_form(a_current_form_2d);}
                // Add a point
                double needed_x = window_struct().mouse_x() - x_in_absolute_pixel();
                double needed_y = window_struct().mouse_y() - (window_struct().window_height() - (y_in_absolute_pixel() + height_in_pixel()));
                scls::Fraction movement_x = pixel_x_to_graphic_x(needed_x, texture()->image_shared_ptr());
                scls::Fraction movement_y = pixel_y_to_graphic_y(needed_y, texture()->image_shared_ptr());
                std::shared_ptr<Point_2D> needed_vector = std::make_shared<Point_2D>(graphic_base_shared_ptr(), std::string("Forme_") + std::to_string(current_form_2d()->points().size()), movement_x, movement_y);
                needed_vector.get()->set_type(Vector_Type::VT_Point);
                add_point(needed_vector);
                a_created_vectors_at_click.push_back(vectors().at(vectors().size() - 1));
                current_form_2d()->add_point(needed_vector);
                modified = true;
            }
        }

        // Handle deletes objects
        for(int i = 0;i<static_cast<int>(graphic()->circles().size());i++){if(graphic()->circles().at(i).get()->should_delete()){graphic()->circles().erase(graphic()->circles().begin() + i);i--;}}

        if(modified){update_texture();}
    }

    // Updates the physic
    int Graphic_Object::update_physic(double used_delta_time) {
        scls::Fraction delta_time_fraction = scls::Fraction::from_double(used_delta_time);

        // Soft-reset the physic
        for(int i = 0;i<static_cast<int>(graphic()->physic_objects().size());i++) {
            if(graphic()->physic_objects().at(i).get()->attached_object()==0||graphic()->physic_objects().at(i).get()->attached_transform()==0){
                graphic()->physic_objects().erase(graphic()->physic_objects().begin() + i);i--;
            }
            else{graphic()->physic_objects().at(i).get()->soft_reset();graphic()->physic_objects().at(i).get()->set_delta_time(delta_time_fraction);}
        }

        // Realised updates
        int needed_update = 0;

        // Apply gravity
        for(int i = 0;i<static_cast<int>(graphic()->physic_objects().size());i++) {if(graphic()->physic_objects().at(i).get()->use_gravity()){graphic()->physic_objects().at(i).get()->accelerate(gravity * delta_time_fraction);needed_update++;}}

        // Update raw velocity
        for(int i = 0;i<static_cast<int>(graphic()->physic_objects().size());i++) {graphic()->physic_objects().at(i).get()->update_raw_velocity();}

        // Update each objects in the case
        std::vector<std::shared_ptr<Graphic::Graphic_Physic>> dynamic_objects_physic;
        for(int i = 0;i<static_cast<int>(graphic()->physic_objects().size());i++) {
            if(!graphic()->physic_objects().at(i)->is_static()) {
                // Get the needed datas
                int needed_height = graphic()->physic_objects().at(i)->max_y_next().to_double_ceil() - graphic()->physic_objects().at(i)->min_y_next().to_double_floor();
                int needed_width = graphic()->physic_objects().at(i)->max_x_next().to_double_ceil() - graphic()->physic_objects().at(i)->min_x_next().to_double_floor();
                if(needed_width <= 0){needed_width = 1;};
                int x_start = graphic()->physic_objects().at(i)->min_x_next().to_double_floor();
                int y_start = graphic()->physic_objects().at(i)->min_y_next().to_double_floor();

                // Check the cases
                for(int j = 0;j<needed_width;j++) {
                    for(int h = 0;h<needed_height;h++) {
                        Graphic::Physic_Case* current_case = physic_case(x_start + j, y_start + h);
                        if(current_case != 0 && current_case->static_objects_collisions.size() > 0){
                            for(int h = 0;h<static_cast<int>(current_case->static_objects_collisions.size());h++) {
                                graphic()->physic_objects().at(i)->check_collision(current_case->static_objects_collisions[h].lock().get(), current_case->static_objects_collisions_physic[h].lock().get());
                            }
                        }
                    }
                }

                // Check the dynamics collisions
                for(int j = 0;j<static_cast<int>(dynamic_objects_physic.size());j++){
                    for(int k = 0;k<static_cast<int>(dynamic_objects_physic.at(j).get()->collisions().size());k++){
                        graphic()->physic_objects().at(i)->check_collision(dynamic_objects_physic.at(j).get()->collisions().at(k).get(), dynamic_objects_physic.at(j).get());
                    }
                }
                dynamic_objects_physic.push_back(graphic()->physic_objects().at(i));
            }
            else if(!graphic()->physic_objects().at(i)->loaded_in_map() || graphic()->physic_objects().at(i)->moved_during_this_frame()) {
                // Delete the last cases
                for(int j = 0;j<static_cast<int>(graphic()->physic_objects().at(i)->used_physic_case().size());j++) {
                    for(int k = 0;k<static_cast<int>(graphic()->physic_objects().at(i)->collisions().size());k++) {
                        graphic()->physic_objects().at(i)->used_physic_case()[j]->delete_static_object_collision(graphic()->physic_objects().at(i)->collisions()[k].get());
                    }
                }

                // Get the needed datas
                graphic()->physic_objects().at(i)->set_loaded_in_map(true);
                int needed_height = graphic()->physic_objects().at(i)->max_y_next().to_double_ceil() - graphic()->physic_objects().at(i)->min_y_next().to_double_floor();
                int needed_width = graphic()->physic_objects().at(i)->max_x_next().to_double_ceil() - graphic()->physic_objects().at(i)->min_x_next().to_double_floor();
                if(needed_height <= 0){needed_height=1;}
                int x_start = graphic()->physic_objects().at(i)->min_x_next().to_double_floor();
                int y_start = graphic()->physic_objects().at(i)->min_y_next().to_double_floor();

                // Add the cases
                for(int j = 0;j<needed_width;j++) {
                    for(int h = 0;h<needed_height;h++) {
                        Graphic::Physic_Case* current_case = physic_case(x_start + j, y_start + h);
                        if(current_case != 0){
                            for(int l = 0;l<static_cast<int>(graphic()->physic_objects().at(i)->collisions().size());l++){
                                current_case->static_objects_collisions.push_back(graphic()->physic_objects().at(i)->collisions()[l]);
                                current_case->static_objects_collisions_physic.push_back(graphic()->physic_objects().at(i));
                            }
                            graphic()->physic_objects().at(i)->used_physic_case().push_back(current_case);
                        }
                    }
                }
            }
        }

        // Collision events
        for(int i = 0;i<static_cast<int>(graphic()->physic_objects().size());i++) {
            __Graphic_Object_Base*object=graphic()->physic_objects().at(i).get()->attached_object();
            if(object != 0) {
                for(int j = 0;j<static_cast<int>(graphic()->physic_objects().at(i).get()->current_collisions_results().size());j++) {
                    object->when_collision(graphic()->physic_objects().at(i).get()->current_collisions_results().at(j).get());
                }
            }
        }
        // Apply collisions
        for(int i = 0;i<static_cast<int>(graphic()->physic_objects().size());i++) {
            __Graphic_Object_Base*object=graphic()->physic_objects().at(i).get()->attached_object();
            if(object != 0) {
                for(int j = 0;j<static_cast<int>(graphic()->physic_objects().at(i).get()->current_collisions_results().size());j++) {
                    graphic()->physic_objects().at(i)->accelerate(graphic()->physic_objects().at(i).get()->current_collisions_results().at(j).get()->acceleration / graphic()->physic_objects().at(i).get()->current_collisions_results().size());
                }
            }
        }

        // Apply next movement
        for(int i = 0;i<static_cast<int>(graphic()->physic_objects().size());i++) {graphic()->physic_objects().at(i).get()->__move();needed_update++;}

        return needed_update;
    }

    // Update the texture of the object
    void Graphic_Object::update_texture(){
        // Set the good image
        int needed_width = width_in_pixel();int needed_height = height_in_pixel();
        if(use_image()) {
            std::shared_ptr<scls::__Image_Base> needed_image = a_datas.get()->to_image(needed_width, needed_height);
            texture()->set_image(needed_image);
        }
        __graphic_object_render_number++;

        // Update the objects
        for(int i = 0;i<static_cast<int>(a_gui_objects.size());i++) {
            Graphic_Object::Graphic_GUI_Object* current = a_gui_objects.at(i).get();
            current->scale(a_datas.get(), needed_width, needed_height);
        }

        // Update the parent
        set_should_render_during_this_frame(true);
    };

    //******************
    //
    // Settings handling
    //
    //******************

    // Displays the objects in a scroller
    void Graphic::display_objects_in_scroller(scls::GUI_Scroller_Choice* scroller) {
        scroller->reset();

        // Add the circles
        for(int i = 0;i<static_cast<int>(a_circles.size());i++) {scroller->add_object("circle_" + std::to_string(i), std::string("Cercle \"") + a_circles.at(i).get()->name() + std::string("\""));}

        // Add the forms
        for(int i = 0;i<static_cast<int>(a_forms_2d.size());i++) {scroller->add_object("form_" + std::to_string(i), std::string("Forme \"") + a_forms_2d.at(i).get()->name() + std::string("\""));}
    }

    // Loads the elements to handle a circle
    void Graphic::load_circle_settings(scls::GUI_Object* object, pleos::Circle* needed_circle) {
        object->delete_children();

        // Name of the object
        std::shared_ptr<scls::GUI_Text> name_title_shared_ptr = *object->new_object<scls::GUI_Text>(object->name() + std::string("-name_title"));
        scls::GUI_Text* name_title = name_title_shared_ptr.get();
        name_title->attach_left_in_parent(10);name_title->attach_top_in_parent(10);
        name_title->set_height_in_pixel(40);name_title->set_width_in_scale(scls::Fraction(1, 10));
        name_title->set_text(std::string("Nom :"));
        std::shared_ptr<scls::GUI_Text_Input> name_shared_ptr = *object->new_object<scls::GUI_Text_Input>(object->name() + std::string("-name"));
        scls::GUI_Text_Input* name = name_shared_ptr.get();
        name->set_border_width_in_pixel(1);
        name->attach_right_of_object_in_parent(name_title, 10);name->attach_top_in_parent(10);
        name->set_height_in_pixel(40);name->set_width_in_scale(scls::Fraction(1, 4));
        name->set_text(needed_circle->name());

        // X of the object
        std::shared_ptr<scls::GUI_Text> x_title_shared_ptr = *object->new_object<scls::GUI_Text>(object->name() + std::string("-x_title"));
        scls::GUI_Text* x_title = x_title_shared_ptr.get();
        x_title->attach_left_in_parent(10);x_title->attach_bottom_of_object_in_parent(name_title_shared_ptr, 10);
        x_title->set_height_in_pixel(40);x_title->set_width_in_scale(scls::Fraction(1, 10));
        x_title->set_text(std::string("X :"));
        std::shared_ptr<scls::GUI_Text_Input> x_shared_ptr = *object->new_object<scls::GUI_Text_Input>(object->name() + std::string("-x"));
        scls::GUI_Text_Input* x = x_shared_ptr.get();
        x->set_border_width_in_pixel(1);
        x->attach_right_of_object_in_parent(x_title, 10);x->attach_bottom_of_object_in_parent(name_shared_ptr, 10);
        x->set_height_in_pixel(40);x->set_width_in_scale(scls::Fraction(1, 4));
        x->set_text(needed_circle->x().to_std_string(0));

        // Y of the object
        std::shared_ptr<scls::GUI_Text> y_title_shared_ptr = *object->new_object<scls::GUI_Text>(object->name() + std::string("-y_title"));
        scls::GUI_Text* y_title = y_title_shared_ptr.get();
        y_title->attach_right_of_object_in_parent(x_shared_ptr, 10);y_title->attach_bottom_of_object_in_parent(name_title_shared_ptr, 10);
        y_title->set_height_in_pixel(40);y_title->set_width_in_scale(scls::Fraction(1, 10));
        y_title->set_text(std::string("Y :"));
        std::shared_ptr<scls::GUI_Text_Input> y_shared_ptr = *object->new_object<scls::GUI_Text_Input>(object->name() + std::string("-y"));
        scls::GUI_Text_Input* y = y_shared_ptr.get();
        y->set_border_width_in_pixel(1);
        y->attach_right_of_object_in_parent(y_title, 10);y->attach_bottom_of_object_in_parent(name_shared_ptr, 10);
        y->set_height_in_pixel(40);y->set_width_in_scale(scls::Fraction(1, 4));
        y->set_text(needed_circle->y().to_std_string(0));
    }
    void Graphic::save_circle_settings(scls::GUI_Object* object, pleos::Circle* needed_circle) {
        // Get the needed datas
        scls::GUI_Text_Input* name = object->child_by_name<scls::GUI_Text_Input>(object->name() + std::string("-name"));
        scls::GUI_Text_Input* x = object->child_by_name<scls::GUI_Text_Input>(object->name() + std::string("-x"));
        scls::GUI_Text_Input* y = object->child_by_name<scls::GUI_Text_Input>(object->name() + std::string("-y"));
        needed_circle->set_name(name->text());
        needed_circle->set_x(scls::Fraction::from_std_string(x->text()));
        needed_circle->set_y(scls::Fraction::from_std_string(y->text()));
    }
    // Loads / saves the elements to handle a form
    void Graphic::load_form_settings(scls::GUI_Object* object, pleos::Form_2D* needed_form){
        object->delete_children();

        // Name of the object
        std::shared_ptr<scls::GUI_Text> name_title_shared_ptr = *object->new_object<scls::GUI_Text>(object->name() + std::string("-name_title"));
        scls::GUI_Text* name_title = name_title_shared_ptr.get();
        name_title->attach_left_in_parent(10);name_title->attach_top_in_parent(10);
        name_title->set_height_in_pixel(40);name_title->set_width_in_scale(scls::Fraction(1, 10));
        name_title->set_text(std::string("Nom :"));
        std::shared_ptr<scls::GUI_Text_Input> name_shared_ptr = *object->new_object<scls::GUI_Text_Input>(object->name() + std::string("-name"));
        scls::GUI_Text_Input* name = name_shared_ptr.get();
        name->set_border_width_in_pixel(1);
        name->attach_right_of_object_in_parent(name_title, 10);name->attach_top_in_parent(10);
        name->set_height_in_pixel(40);name->set_width_in_scale(scls::Fraction(1, 4));
        name->set_text(needed_form->name());

        // X of the object
        std::shared_ptr<scls::GUI_Text> x_title_shared_ptr = *object->new_object<scls::GUI_Text>(object->name() + std::string("-x_title"));
        scls::GUI_Text* x_title = x_title_shared_ptr.get();
        x_title->attach_left_in_parent(10);x_title->attach_bottom_of_object_in_parent(name_title_shared_ptr, 10);
        x_title->set_height_in_pixel(40);x_title->set_width_in_scale(scls::Fraction(1, 10));
        x_title->set_text(std::string("X :"));
        std::shared_ptr<scls::GUI_Text_Input> x_shared_ptr = *object->new_object<scls::GUI_Text_Input>(object->name() + std::string("-x"));
        scls::GUI_Text_Input* x = x_shared_ptr.get();
        x->set_border_width_in_pixel(1);
        x->attach_right_of_object_in_parent(x_title, 10);x->attach_bottom_of_object_in_parent(name_shared_ptr, 10);
        x->set_height_in_pixel(40);x->set_width_in_scale(scls::Fraction(1, 4));
        x->set_text(needed_form->x().to_std_string(0));

        // Y of the object
        std::shared_ptr<scls::GUI_Text> y_title_shared_ptr = *object->new_object<scls::GUI_Text>(object->name() + std::string("-y_title"));
        scls::GUI_Text* y_title = y_title_shared_ptr.get();
        y_title->attach_right_of_object_in_parent(x_shared_ptr, 10);y_title->attach_bottom_of_object_in_parent(name_title_shared_ptr, 10);
        y_title->set_height_in_pixel(40);y_title->set_width_in_scale(scls::Fraction(1, 10));
        y_title->set_text(std::string("Y :"));
        std::shared_ptr<scls::GUI_Text_Input> y_shared_ptr = *object->new_object<scls::GUI_Text_Input>(object->name() + std::string("-y"));
        scls::GUI_Text_Input* y = y_shared_ptr.get();
        y->set_border_width_in_pixel(1);
        y->attach_right_of_object_in_parent(y_title, 10);y->attach_bottom_of_object_in_parent(name_shared_ptr, 10);
        y->set_height_in_pixel(40);y->set_width_in_scale(scls::Fraction(1, 4));
        y->set_text(needed_form->y().to_std_string(0));
    }
    void Graphic::save_form_settings(scls::GUI_Object* object, pleos::Form_2D* needed_form){
        // Get the needed datas
        scls::GUI_Text_Input* name = object->child_by_name<scls::GUI_Text_Input>(object->name() + std::string("-name"));
        scls::GUI_Text_Input* x = object->child_by_name<scls::GUI_Text_Input>(object->name() + std::string("-x"));
        scls::GUI_Text_Input* y = object->child_by_name<scls::GUI_Text_Input>(object->name() + std::string("-y"));
        needed_form->set_name(name->text());
        needed_form->set_x(scls::Fraction::from_std_string(x->text()));
        needed_form->set_y(scls::Fraction::from_std_string(y->text()));
    }

    // Returns the needed XML text to generate this graphic
    std::string Graphic::to_xml_text() {
        std::string to_return = std::string();

        // Handle background color
        if(background_color().alpha() != 0){to_return += (std::string("<background_color red=") + std::to_string(background_color().red()) + std::string(" green=") + std::to_string(background_color().green()) + std::string(" blue=") + std::to_string(background_color().blue()) + std::string(" alpha=") + std::to_string(background_color().alpha()) + std::string(">"));}

        // Handle base
        if(to_return.size() > 0){to_return += std::string("\n");}
        to_return += (std::string("<base draw=") + std::to_string(draw_base()));
        if((!height_used() && !width_used()) || (height_used() && width_used())){to_return += std::string(" height=") + scls::remove_space(height_formula().to_std_string(0)) + std::string(" width=") + scls::remove_space(width_formula().to_std_string(0));}
        else if(height_used()){to_return += std::string(" height=") + scls::remove_space(height_formula().to_std_string(0));}
        else if(width_used()){to_return += std::string(" width=") + scls::remove_space(width_formula().to_std_string(0));}
        to_return += std::string(">");

        // Add the circles
        if(to_return.size() > 0){to_return += std::string("\n");}
        for(int i = 0;i<static_cast<int>(a_objects.size());i++) {if(a_objects.at(i).get()->save_to_xml_text()){to_return += a_objects.at(i).get()->to_xml_text();to_return += std::string("\n");}}

        // Add the physic
        if(to_return.size() > 0){to_return += std::string("\n");}
        for(int i = 0;i<static_cast<int>(a_physic_objects.size());i++) {to_return += a_physic_objects.at(i).get()->to_xml_text();if(i!=static_cast<int>(a_physic_objects.size())-1){to_return += std::string("\n");}}

        // Return the result
        return to_return;
    }
}
