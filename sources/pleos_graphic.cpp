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
    // Graphic handling
    //
    //******************

    // Annoying functions to draw the image
    int Graphic::Graphic_Base_Object::graphic_x_to_pixel_x(double x){return std::ceil((x - graphic_base()->a_middle_x.to_double()) * graphic_base()->a_pixel_by_case_x + (static_cast<double>(graphic_base()->a_width_in_pixel) / 2.0));};
    int Graphic::Graphic_Base_Object::graphic_x_to_pixel_x(scls::Fraction x){return std::ceil(((x - graphic_base()->a_middle_x) * graphic_base()->a_pixel_by_case_x + scls::Fraction(graphic_base()->a_width_in_pixel, 2)).to_double());};
    int Graphic::Graphic_Base_Object::graphic_y_to_pixel_y(double y){return std::ceil((y - graphic_base()->a_middle_y.to_double()) * graphic_base()->a_pixel_by_case_y) + (graphic_base()->a_height_in_pixel / 2.0);};
    int Graphic::Graphic_Base_Object::graphic_y_to_pixel_y(scls::Fraction y){return std::ceil(((y - graphic_base()->a_middle_y) * graphic_base()->a_pixel_by_case_y + scls::Fraction(graphic_base()->a_height_in_pixel, 2)).to_double());};
    int Graphic::Graphic_Base_Object::graphic_y_to_pixel_y_inversed(double y){return graphic_base()->a_height_in_pixel - graphic_y_to_pixel_y(y);};
    int Graphic::Graphic_Base_Object::graphic_y_to_pixel_y_inversed(scls::Fraction y){return graphic_base()->a_height_in_pixel - graphic_y_to_pixel_y(y);};
    scls::Fraction Graphic::Graphic_Base_Object::pixel_x_to_graphic_x(int x){return graphic_base()->a_middle_x + ((scls::Fraction(x) - scls::Fraction(graphic_base()->a_width_in_pixel, 2)) / scls::Fraction::from_double(graphic_base()->a_pixel_by_case_x));}
    scls::Fraction Graphic::Graphic_Base_Object::pixel_y_to_graphic_y(int y){return graphic_base()->a_middle_y + ((scls::Fraction(graphic_base()->a_height_in_pixel, 2) - scls::Fraction(y)) / scls::Fraction::from_double(graphic_base()->a_pixel_by_case_y));}

    // Returns the datas set to an image
    std::shared_ptr<scls::Image> Graphic::Datas_Set::to_image() {
        if(type() == Graphic::Datas_Set::Datas_Set_Type::DST_Histogram){return to_image_histogram();}
        else if(type() == Graphic::Datas_Set::Datas_Set_Type::DST_Point_Cloud || type() == Graphic::Datas_Set::Datas_Set_Type::DST_Point_Cloud_Linked){return to_image_point_cloud();}
        return std::shared_ptr<scls::Image>();
    }
    std::shared_ptr<scls::Image> Graphic::Datas_Set::to_image_histogram() {
        // Create the image
        std::shared_ptr<scls::Image> to_return = std::make_shared<scls::Image>(width_in_pixel(), height_in_pixel(), scls::Color(255, 255, 255));

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
    std::shared_ptr<scls::Image> Graphic::Datas_Set::to_image_point_cloud() {
        // Create the image
        std::shared_ptr<scls::Image> to_return = std::make_shared<scls::Image>(width_in_pixel(), height_in_pixel(), scls::Color(255, 255, 255, 0));

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

    // Adds a function to the graphic
    std::shared_ptr<Graphic::Graphic_Function> Graphic::add_function(std::shared_ptr<Function_Studied> function_studied) {std::shared_ptr<Graphic_Function> new_function = std::make_shared<Graphic_Function>(function_studied);a_functions.push_back(new_function);a_graphic_base.get()->a_function_number++;return new_function;}

    // Draws a datas set on the graphic
    void Graphic::draw_datas_set(Datas_Set* needed_datas_set, std::shared_ptr<scls::Image> to_return) {
        std::shared_ptr<scls::Image> needed_image = needed_datas_set->to_image();
        double needed_x = graphic_x_to_pixel_x(needed_datas_set->x().to_double(), to_return);
        double needed_y = graphic_y_to_pixel_y_inversed(needed_datas_set->y().to_double(), to_return);
        to_return.get()->paste(needed_image.get(), needed_x - needed_image.get()->width() / 2, needed_y - needed_image.get()->height() / 2);
    }

    // Draws a form on the graphic
    void Graphic::draw_form(Form_2D* needed_form, std::shared_ptr<scls::Image> to_return) {
        // Asserts
        if(needed_form->points().size() < 2){return;}
        else if(needed_form->points().size() == 2) {draw_line(needed_form->points()[0].get(), needed_form->points()[1].get(), needed_form->color_with_opacity(needed_form->border_color()), needed_form->border_radius(), needed_form->link(0).drawing_proportion, to_return);return;}

        // Triangulate the form
        std::vector<std::shared_ptr<Vector>> triangulated_points = needed_form->triangulated_points();

        // Draw the inner form
        scls::Color inner_color = needed_form->color_with_opacity(needed_form->color());
        if(inner_color.alpha() > 0) {
            for(int i = 0;i<static_cast<int>(triangulated_points.size());i+=3) {
                std::shared_ptr<Vector> current_point = triangulated_points[i];
                double first_x = graphic_x_to_pixel_x(current_point.get()->x_to_double(), to_return);
                double first_y = graphic_y_to_pixel_y_inversed(current_point.get()->y()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
                current_point = triangulated_points[i + 1];
                double second_x = graphic_x_to_pixel_x(current_point.get()->x_to_double(), to_return);
                double second_y = graphic_y_to_pixel_y_inversed(current_point.get()->y()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
                current_point = triangulated_points[i + 2];
                double third_x = graphic_x_to_pixel_x(current_point.get()->x_to_double(), to_return);
                double third_y = graphic_y_to_pixel_y_inversed(current_point.get()->y()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
                to_return.get()->fill_triangle(first_x, first_y, second_x, second_y, third_x, third_y, inner_color);
            } triangulated_points.clear();
        }

        // Draw the links
        std::shared_ptr<Vector> last_point = needed_form->points()[needed_form->points().size() - 1];
        double last_x = graphic_x_to_pixel_x(last_point.get()->x()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
        double last_y = graphic_y_to_pixel_y_inversed(last_point.get()->y()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);

        // Link each points
        scls::Color border_color = needed_form->color_with_opacity(needed_form->border_color());
        for(int j = 0;j<static_cast<int>(needed_form->points().size());j++) {
            std::shared_ptr<Vector> current_point = needed_form->points()[j];
            double needed_x = graphic_x_to_pixel_x(current_point.get()->x_to_double(), to_return);
            double needed_y = graphic_y_to_pixel_y_inversed(current_point.get()->y_to_double(), to_return);
            Form_2D::Link current_link;
            if(j <= 0){current_link = needed_form->last_link();}
            else{current_link = needed_form->link(j - 1);}

            // Apply the proportion
            double needed_proportion = current_link.drawing_proportion;
            double needed_move_x = needed_x - last_x;double needed_move_x_minus = 0;
            double needed_move_y = needed_y - last_y;double needed_move_y_minus = 0;
            if(needed_proportion >= 0) {
                needed_move_x *= needed_proportion;
                needed_move_y *= needed_proportion;
            }
            else {
                needed_move_x_minus = needed_move_x * (1.0 + needed_proportion);
                needed_move_y_minus = needed_move_y * (1.0 + needed_proportion);
            }

            to_return.get()->draw_line(last_x + needed_move_x_minus, last_y + needed_move_y_minus, last_x + needed_move_x, last_y + needed_move_y, border_color, needed_form->border_radius());
            last_point = current_point; last_x = needed_x; last_y = needed_y;
        }
    }

    // Draw a line between two points
    void Graphic::draw_line(Vector* point_1, Vector* point_2, scls::Color color, double width, double proportion, std::shared_ptr<scls::Image> to_return) {
        // Draw a line
        double last_x = graphic_x_to_pixel_x(point_1->x()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
        double last_y = graphic_y_to_pixel_y_inversed(point_1->y()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
        double needed_x = graphic_x_to_pixel_x(point_2->x()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
        double needed_y = graphic_y_to_pixel_y_inversed(point_2->y()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
        // Apply the proportion
        double needed_proportion = proportion;
        double needed_move_x = needed_x - last_x;double needed_move_x_minus = 0;
        double needed_move_y = needed_y - last_y;double needed_move_y_minus = 0;
        if(needed_proportion >= 0) {
            needed_move_x *= needed_proportion;
            needed_move_y *= needed_proportion;
        }
        else {
            needed_move_x_minus = needed_move_x * (1.0 + needed_proportion);
            needed_move_y_minus = needed_move_y * (1.0 + needed_proportion);
        }

        // Apply the proportion
        to_return.get()->draw_line(last_x + needed_move_x_minus + width / 2.0, last_y + needed_move_y_minus, last_x + needed_move_x, last_y + needed_move_y, color, width);
    }

    // Draws a point on the graphic
    void Graphic::draw_vector(Vector* needed_point, std::shared_ptr<scls::Image> to_return) {
        // Get the needed coordinates
        scls::Fraction needed_x_end = 0;if(needed_point->x_end() != 0){needed_x_end = needed_point->x_end()->to_polymonial().known_monomonial().factor().real();}
        needed_x_end = graphic_x_to_pixel_x(needed_x_end, to_return);
        scls::Fraction needed_y_end = 0;if(needed_point->y_end() != 0){needed_y_end = needed_point->y_end()->to_polymonial().known_monomonial().factor().real();}
        needed_y_end = graphic_y_to_pixel_y_inversed(needed_y_end, to_return);
        scls::Fraction needed_x_start = graphic_x_to_pixel_x(needed_point->x()->to_polymonial().known_monomonial().factor().real(), to_return);
        scls::Fraction needed_y_start = graphic_y_to_pixel_y_inversed(needed_point->y()->to_polymonial().known_monomonial().factor().real(), to_return);
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

    // Creates and returns a line (and its points)
    std::shared_ptr<Form_2D> Graphic::new_line(std::string name, scls::Fraction x_1, scls::Fraction y_1, scls::Fraction x_2, scls::Fraction y_2){
        std::shared_ptr<Form_2D>to_return=std::make_shared<Form_2D>(name);
        to_return.get()->new_point(x_1, y_1);
        to_return.get()->new_point(x_2, y_2);
        add_form(to_return);
        return to_return;
    }

    // Creates and returns a form (and its point)
    std::shared_ptr<Form_2D> Graphic::new_form(std::string name, scls::Fraction x_1, scls::Fraction y_1, scls::Fraction x_2, scls::Fraction y_2, scls::Fraction x_3, scls::Fraction y_3, scls::Fraction x_4, scls::Fraction y_4) {
        std::shared_ptr<Form_2D>to_return=std::make_shared<Form_2D>(name);
        to_return.get()->add_point(new_point(name + std::string("-p1"), x_1, y_1));
        to_return.get()->add_point(new_point(name + std::string("-p2"), x_2, y_2));
        to_return.get()->add_point(new_point(name + std::string("-p3"), x_3, y_3));
        to_return.get()->add_point(new_point(name + std::string("-p4"), x_4, y_4));
        add_form(to_return);
        return to_return;
    }

    // Creates and returns a form from a face
    std::shared_ptr<Form_2D> Graphic::new_form_from_face(std::string name, scls::model_maker::Face* face){
        std::shared_ptr<Form_2D>to_return=new_form(name);

        // Add the points
        for(int i = 0;i<static_cast<int>(face->points().size());i++) {
            std::string current_name = name + std::string("-") + std::to_string(i);
            std::shared_ptr<Vector> current_vector = add_point(current_name, face->points()[i].get());
            to_return.get()->add_point(current_vector);
        }
        // Add the exclusion points
        std::string final_points = std::string();
        for(int i = 0;i<static_cast<int>(face->exclusion_points().size());i++) {
            std::string current_name = name + std::string("-") + std::to_string(i);
            std::shared_ptr<Vector> current_vector = add_point(current_name, face->exclusion_points()[i].get());
            to_return.get()->add_exclusion_point(current_vector);
        }

        return to_return;
    };

    // Creates and returns a square (and its point)
    std::shared_ptr<Form_2D> Graphic::new_square(std::string name, scls::Fraction x, scls::Fraction y, scls::Fraction width, scls::Fraction height) {return new_form(name, x, y, x + width, y, x + width, y + height, x, y + height);}

    // Creates and returns a triangle (and its point)
    std::shared_ptr<Form_2D> Graphic::new_triangle(std::string name, scls::Fraction x_1, scls::Fraction y_1, scls::Fraction x_2, scls::Fraction y_2, scls::Fraction x_3, scls::Fraction y_3) {
        std::shared_ptr<Form_2D>to_return=std::make_shared<Form_2D>(name);
        to_return.get()->add_point(new_point(name + std::string("-p1"), x_1, y_1));
        to_return.get()->add_point(new_point(name + std::string("-p2"), x_2, y_2));
        to_return.get()->add_point(new_point(name + std::string("-p3"), x_3, y_3));
        add_form(to_return);
        return to_return;
    }

    // Struct containing some datas for positions
    struct Needed_Pos {scls::__Formula_Base::Formula pos;scls::__Formula_Base::Formula previous_pos;bool previous_pos_used = false;};
    // Draw the bases of the image
    void Graphic::image_draw_base(std::shared_ptr<scls::Image> image) {
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
    void Graphic::image_draw_function(std::shared_ptr<scls::Image> to_return, std::shared_ptr<Graphic_Function> needed_function, std::vector<scls::Fraction>& screen_pos) {
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
    int Graphic::graphic_x_to_pixel_x(double x, int image_width){return std::ceil((x - middle_x().to_double()) * pixel_by_case_x() + (static_cast<double>(image_width) / 2.0));};
    int Graphic::graphic_x_to_pixel_x(scls::Fraction x, int image_width){return std::ceil(((x - middle_x()) * pixel_by_case_x() + scls::Fraction(image_width, 2)).to_double());};
    int Graphic::graphic_y_to_pixel_y(double y, int needed_height){return std::ceil((y - middle_y().to_double()) * pixel_by_case_y()) + (needed_height / 2.0);};
    int Graphic::graphic_y_to_pixel_y(scls::Fraction y, int needed_height){return std::ceil(((y - middle_y()) * pixel_by_case_y() + scls::Fraction(needed_height, 2)).to_double());};
    int Graphic::graphic_y_to_pixel_y_inversed(double y, int needed_height){return needed_height - graphic_y_to_pixel_y(y, needed_height);};
    int Graphic::graphic_y_to_pixel_y_inversed(scls::Fraction y, int needed_height){return needed_height - graphic_y_to_pixel_y(y, needed_height);};
    scls::Fraction Graphic::pixel_x_to_graphic_x(int x, int image_width){return middle_x() + ((scls::Fraction(x) - scls::Fraction(image_width, 2)) / scls::Fraction::from_double(pixel_by_case_x()));}
    scls::Fraction Graphic::pixel_y_to_graphic_y(int y, int needed_height){return middle_y() + ((scls::Fraction(needed_height, 2) - scls::Fraction(y)) / scls::Fraction::from_double(pixel_by_case_y()));}
    std::shared_ptr<scls::Image> Graphic::to_image(int width_in_pixel, int height_in_pixel) {
        // Create the image
        std::shared_ptr<scls::Image> to_return = std::make_shared<scls::Image>(width_in_pixel, height_in_pixel, style()->background_color());
        a_graphic_base.get()->a_height_in_pixel = height_in_pixel;
        a_graphic_base.get()->a_width_in_pixel = width_in_pixel;

        // Handle the height and width
        if(a_graphic_base.get()->a_height != -1) {a_graphic_base.get()->a_pixel_by_case_y = static_cast<double>(height_in_pixel) / a_graphic_base.get()->a_height;}
        if(a_graphic_base.get()->a_width != -1) {a_graphic_base.get()->a_pixel_by_case_x = static_cast<double>(width_in_pixel) / a_graphic_base.get()->a_width;}

        // Draw the basic lines
        if(draw_base() || draw_sub_bases()) {image_draw_base(to_return);}

        // Get the datas for the drawing
        scls::Fraction image = pixel_x_to_graphic_x(0, to_return).normalized(5);
        scls::Fraction multiplier = (scls::Fraction(1) / scls::Fraction::from_double(pixel_by_case_x())).normalized(5);
        std::vector<scls::Fraction> screen_pos = std::vector<scls::Fraction>(to_return.get()->width() + 1);
        for(int i = 0;i<static_cast<int>(to_return.get()->width()) + 1;i++){screen_pos[i] = image.normalized(5);image += multiplier;}
        // Draw the functions
        for(int i = 0;i<static_cast<int>(a_functions.size());i++) {image_draw_function(to_return, a_functions[i], screen_pos);}

        // Draw the circles
        for(int i = 0;i<static_cast<int>(a_circles.size());i++) {
            std::shared_ptr<Circle> current_circle = a_circles[i];
            Vector center = current_circle.get()->center();
            double radius = current_circle.get()->radius_formula_shared_ptr().get()->value_to_double(a_unknowns.get());
            radius = radius * pixel_by_case_x();
            double needed_x = graphic_x_to_pixel_x(center.x()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
            double needed_y = graphic_y_to_pixel_y_inversed(center.y()->to_polymonial().known_monomonial().factor().real().to_double(), to_return);
            to_return.get()->fill_circle(needed_x, needed_y, radius, current_circle.get()->angle_start().value_to_double(a_unknowns.get()) , current_circle.get()->angle_end().value_to_double(a_unknowns.get()), current_circle.get()->color(), current_circle.get()->border_radius(), current_circle.get()->border_color());
        }
        // Draw the forms
        for(int i = 0;i<static_cast<int>(a_forms_2d.size());i++) {draw_form(a_forms_2d[i].get(), to_return);}
        // Draw the texts
        scls::Text_Image_Generator tig;
        for(int i = 0;i<static_cast<int>(a_texts.size());i++) {
            scls::Text_Style current_style = *a_texts.at(i).get()->style.get();current_style.set_color(a_texts.at(i).get()->color_with_opacity(current_style.color()));
            std::shared_ptr<scls::Image> needed_image = tig.image_shared_ptr(a_texts.at(i).get()->content, current_style);
            double needed_x = graphic_x_to_pixel_x(a_texts.at(i).get()->x.to_double(), to_return);
            double needed_y = graphic_y_to_pixel_y_inversed(a_texts.at(i).get()->y.to_double(), to_return);
            to_return.get()->paste(needed_image.get(), needed_x - needed_image.get()->width() / 2, needed_y - needed_image.get()->height() / 2);
        }
        // Draw the vectors
        for(int i = 0;i<static_cast<int>(a_points.size());i++) {draw_vector(a_points.at(i).get(), to_return);}
        for(int i = 0;i<static_cast<int>(a_vectors.size());i++) {draw_vector(a_vectors.at(i).get(), to_return);}

        // Draw the datas set
        for(int i = 0;i<static_cast<int>(a_datas_sets.size());i++) {draw_datas_set(a_datas_sets.at(i).get(), to_return);}

        // Draw the border
        to_return.get()->draw_border(style()->border_top_width(), style()->border_left_width(), style()->border_bottom_width(), style()->border_right_width(), style()->border_color);

        return to_return;
    }

    //******************
    //
    // Loading handling
    //
    //******************

    // Balises in the graphic
    void Graphic::graphic_from_xml_balise(std::shared_ptr<scls::XML_Text> xml, Text_Environment& environment, std::shared_ptr<scls::Text_Style> text_style){
        std::string balise_content = xml.get()->xml_balise();
        std::string current_balise_name = xml.get()->xml_balise_name();
        std::vector<scls::XML_Attribute>& attributes = xml.get()->xml_balise_attributes();
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
            add_vector(needed_vector);
        }
        else if(current_balise_name == "background_color") {set_background_color(scls::Color::from_xml(xml));}
        else if(current_balise_name == "base") {
            // Get the datas about the base of the graphic
            scls::Fraction height = a_graphic_base.get()->a_height;scls::Fraction width = a_graphic_base.get()->a_width;
            if(height == -1){height = 10;}if(width == -1){width = 10;}
            scls::Fraction middle_x = 10;scls::Fraction middle_y = 10;
            for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                if(attributes[j].name == "height") {height = scls::Fraction::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "width") {width = scls::Fraction::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "x") {middle_x = scls::Fraction::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "y") {middle_y = scls::Fraction::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "draw") {if(attributes[j].value == "false" || attributes[j].value == "0"){set_draw_base(false);set_draw_sub_bases(false);}}
            }
            set_middle(middle_x, middle_y);set_scale(height.to_double(), width.to_double());
        }
        else if(current_balise_name == "border") {scls::border_from_xml(xml, style());}
        else if(current_balise_name == "circle") {
            // Get the datas about a circle of the graphic
            scls::__Formula_Base angle_end = 360;scls::__Formula_Base angle_start = 0;
            scls::Color border_color = scls::Color(0, 0, 0);scls::Fraction border_radius=5;scls::Color color = scls::Color(255, 255, 255);
            std::string needed_name = std::string();scls::Fraction needed_x = 0;scls::Fraction needed_y = 0;scls::__Formula_Base radius = 1;
            std::vector<Graphic_Collision_Type> collisions;bool use_gravity = false;int use_physic = 0; // 0 = None, 1 = Static, 2 = Dynamic
            scls::Point_2D velocity_start = scls::Point_2D(0, 0);
            for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                if(attributes[j].name == "angle_end") {angle_end = *environment.value_formula(attributes[j].value).formula_base();}
                else if(attributes[j].name == "angle_start") {angle_start = *environment.value_formula(attributes[j].value).formula_base();}
                else if(attributes[j].name == "border_radius" || attributes[j].name == "width") {border_radius = scls::Fraction::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "collision") {if(attributes[j].value == std::string("circle")){collisions.push_back(Graphic_Collision_Type::GCT_Circle);}else{collisions.push_back(Graphic_Collision_Type::GCT_Rect);}}
                else if(attributes[j].name == "color") {color = scls::Color::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "gravity" || attributes[j].name == "use_gravity") {use_gravity = (attributes[j].value == std::string("1") || attributes[j].value == std::string("true"));}
                else if(attributes[j].name == "name") {needed_name = attributes[j].value;}
                else if(attributes[j].name == "physic") {if(attributes[j].value == std::string("static") || attributes[j].value == std::string("1")){use_physic = 1;}else{use_physic = 2;}}
                else if(attributes[j].name == "radius") {radius = (*environment.value_formula(attributes[j].value).formula_base());}
                else if(attributes[j].name == "velocity") {velocity_start = environment.value_point_2d(attributes[j].value);}
                else if(attributes[j].name == "x") {needed_x = scls::Fraction::from_std_string(attributes[j].value).to_double();}
                else if(attributes[j].name == "y") {needed_y = scls::Fraction::from_std_string(attributes[j].value).to_double();}
            }

            // Add the circle
            std::shared_ptr<Circle> circle = *add_circle(needed_name, Vector(needed_name + std::string("-center") , needed_x, needed_y), radius);
            circle.get()->set_angle_end(angle_end);circle.get()->set_angle_start(angle_start);
            circle.get()->set_border_color(border_color);circle.get()->set_border_radius(border_radius.to_double());circle.get()->set_color(color);

            // Handle physic
            if(use_physic != 0){
                if(physic_map().size() <= 0){load_physic_map(0, 0);}
                std::shared_ptr<pleos::Graphic::Graphic_Physic> physic = std::make_shared<pleos::Graphic::Graphic_Physic>(circle, circle.get()->attached_transform_shared_ptr());
                physic.get()->set_use_gravity(use_gravity);physic.get()->set_static(use_physic == 1);add_physic_object(physic);
                for(int k = 0;k<static_cast<int>(collisions.size());k++){physic.get()->new_collision(collisions.at(k));}
                physic.get()->set_velocity(velocity_start);
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
            std::vector<std::shared_ptr<scls::XML_Text>>& sub_texts = xml.get()->sub_texts();
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
            std::vector<std::shared_ptr<scls::XML_Text>> sub_texts = xml.get()->sub_texts();
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
                if(use_collision){physic.get()->add_collision(x_1, y_1, x_2, y_2);scls::Fraction current_width=(x_2-x_1).abs();if(current_width<=0){current_width=scls::Fraction(1, 100);} physic.get()->attached_transform()->set_position((x_2 - x_1) / 2, (y_2 - y_1) / 2);physic.get()->attached_transform()->set_scale(current_width, (y_2 - y_1).abs());}
            }
        }
        else if(current_balise_name == "rect") {
            // Get the datas about a rectangle in the graphic
            scls::Color border_color = scls::Color(0, 0, 0);scls::Fraction border_radius=5;
            scls::Color color = scls::Color(255, 255, 255);
            std::string needed_name = std::string();
            scls::Fraction height = 0;scls::Fraction width = 0;
            scls::Fraction x = 0;scls::Fraction y = 0;
            for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                if(attributes[j].name == "background_color" || attributes[j].name == "color") {color = scls::Color::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "border_color") {border_color = scls::Color::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "border_radius") {border_radius = scls::Fraction::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "name") {needed_name = attributes[j].value;}
                else if(attributes[j].name == "height") {height = scls::Fraction::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "width") {width = scls::Fraction::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "x") {x = scls::Fraction::from_std_string(attributes[j].value);}
                else if(attributes[j].name == "y") {y = scls::Fraction::from_std_string(attributes[j].value);}
            }
            // Add the form
            std::shared_ptr<Form_2D> created_form = new_square(needed_name, x, y, width, height);
            created_form.get()->set_border_color(border_color);created_form.get()->set_border_radius(border_radius.to_double());
            created_form.get()->set_color(color);
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
            if(current_balise_name == "point"){add_point(Vector(needed_name, needed_x, needed_y));}
            else if(current_balise_name == "vec" || current_balise_name == "vector"){add_vector(Vector(needed_name, needed_x, needed_y));}
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
            std::string needed_content = std::string();scls::Fraction needed_x = 0;scls::Fraction needed_y = 0;scls::Fraction radius = 1;
            for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                if(attributes[j].name == "background_color") {text_style.get()->set_background_color(scls::Color::from_std_string(attributes[j].value));}
                else if(attributes[j].name == "content") {needed_content = attributes[j].value;}
                else if(attributes[j].name == "x") {needed_x = scls::Fraction::from_std_string(attributes[j].value).to_double();}
                else if(attributes[j].name == "y") {needed_y = scls::Fraction::from_std_string(attributes[j].value).to_double();}
            }
            // Add the text
            new_text(needed_content, Vector(std::string("text-center") , needed_x, needed_y), text_style);
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

    //******************
    //
    // Physic handling
    //
    //******************

    // Used gravity
    scls::Point_2D gravity = scls::Point_2D(0, scls::Fraction(-98, 10));

    // Checks if a collision occurs with an another collision
    struct Collision_Result{Collision_Result(){};Collision_Result(std::shared_ptr<Collision> c_1):Collision_Result(c_1, std::shared_ptr<Collision>()){};Collision_Result(std::shared_ptr<Collision> c_1, std::shared_ptr<Collision> c_2):collision_1(c_1),collision_2(c_2){};std::shared_ptr<Collision> collision_1;std::shared_ptr<Collision> collision_2;};;;;
    Collision_Result __check_collision_circle_circle(Graphic_Collision* collision_1, Graphic_Collision* collision_2, Graphic::Graphic_Physic* object_1, Graphic::Graphic_Physic* object_2){
        // Get the distance
        scls::Point_2D position_1 = collision_1->position_next();scls::Point_2D position_2 = collision_2->position_next();
        double distance = position_1.distance(position_2);
        double max_distance = std::abs(collision_1->attached_transform()->scale_x().to_double() / 2.0 + collision_2->attached_transform()->scale_x().to_double() / 2.0);
        double min_distance = std::abs(collision_1->attached_transform()->scale_x().to_double() / 2.0 - collision_2->attached_transform()->scale_x().to_double() / 2.0);
        if(distance > max_distance || distance < min_distance){return Collision_Result();}

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

        // Calculate the needed angle
        scls::Point_2D position_from_1 = (position_1 - position_2).normalized();
        scls::Point_2D position_from_2 = (position_2 - position_1).normalized();
        if(distance < collision_1->attached_transform()->scale_x() / 2){position_from_2 *= -1;}
        double kept_proportion = 1.0;
        scls::Point_2D velocity_2 = collision_2->attached_transform()->velocity();
        double angle_tangent_1 = scls::vector_2d_angle(position_from_1);
        double needed_angle = scls::vector_2d_angle(velocity_2) - angle_tangent_1;
        to_return_1.get()->angle = scls::vector_2d_angle(position_from_1);
        to_return_2.get()->angle = scls::vector_2d_angle(position_from_2);

        // Calculate the velocity of the object 2
        scls::Fraction multiplier = scls::Fraction::from_double(velocity_2.norm());
        scls::Point_2D new_velocity = (position_from_1 - (velocity_2 - position_from_1));
        new_velocity += position_from_1.rotated(angle_tangent_1 - needed_angle) * multiplier;
        if(distance < collision_1->attached_transform()->scale_x() / 2){multiplier *= -1;}
        new_velocity.normalize();new_velocity *= multiplier * scls::Fraction(99, 100);
        to_return_2.get()->acceleration = object_2->velocity() * -1 + new_velocity;

        // Return the result
        if(inverse){return Collision_Result(to_return_2, std::shared_ptr<Collision>());}
        return Collision_Result(std::shared_ptr<Collision>(), to_return_2);
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
        scls::Point_2D position_from_1 = (position_1 - contact_point).normalized();
        scls::Point_2D position_from_2 = (contact_point - position_1).normalized();
        if(distance < collision_1->attached_transform()->scale_x() / 2){position_from_2 *= -1;}
        double kept_proportion = 1.0;
        scls::Point_2D velocity_1 = collision_1->attached_transform()->velocity();
        double angle_tangent_1 = scls::vector_2d_angle(position_from_1);
        double needed_angle = scls::vector_2d_angle(velocity_1) - angle_tangent_1;
        to_return_1.get()->angle = scls::vector_2d_angle(position_from_1);
        to_return_2.get()->angle = scls::vector_2d_angle(position_from_2);

        // Calculate the velocity of the object 1
        scls::Fraction multiplier = scls::Fraction::from_double(velocity_1.norm());
        scls::Point_2D new_velocity = (position_from_1 - (velocity_1 - position_from_1));
        new_velocity += position_from_1.rotated(angle_tangent_1 - needed_angle) * multiplier;
        new_velocity.normalize();new_velocity *= multiplier * scls::Fraction(99, 100);
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

    // Graphic constructor
    Graphic_Object::Graphic_Object(scls::_Window_Advanced_Struct& window, std::string name, std::weak_ptr<scls::GUI_Object> parent):scls::GUI_Object(window, name, parent){update_texture();}

    // Render the object
    void Graphic_Object::render(glm::vec3 scale_multiplier) {scls::GUI_Object::render(scale_multiplier);}

    // Scale the GUI Object
    void Graphic_Object::Graphic_GUI_Object::scale(Graphic* graphic, int image_width, int image_height){
        // Get the needed values
        int new_height = std::ceil(graphic->pixel_by_case_y()) * height().to_double();
        int new_x = graphic->graphic_x_to_pixel_x((x() - width() / 2).to_double(), image_width);
        int new_y = graphic->graphic_y_to_pixel_y((y() - height() / 2).to_double(), image_height);
        int new_width = std::ceil(graphic->pixel_by_case_x()) * width().to_double();

        // Check if the object have to be rendered or not
        if(new_x > image_width || new_y > image_height || new_x < -new_width || new_y < -new_height){a_object.get()->set_visible(false);return;}
        else{a_object.get()->set_visible(true);}

        // Set the values
        a_object.get()->set_height_in_pixel(new_height);
        a_object.get()->set_width_in_pixel(new_width);
        a_object.get()->set_x_in_pixel(new_x);
        a_object.get()->set_y_in_pixel(new_y);
        a_object.get()->set_texture_scale_x(width().to_double());
        a_object.get()->set_texture_scale_y(height().to_double());
    };

    // Updates the object
    int __graphic_object_render_number = 0;
    std::vector<std::shared_ptr<Vector>> __temp_vectors;
    void Graphic_Object::update(){for(int i = 0;i<static_cast<int>(a_gui_objects.size());i++) {if(a_gui_objects[i].get() != 0) {a_gui_objects[i].get()->update();}}};
    void Graphic_Object::update_event() {
        // Update the object
        for(int i = 0;i<static_cast<int>(a_gui_objects.size());i++) {
            if(a_gui_objects[i].get() != 0) {
                a_gui_objects[i].get()->update_event();
            }
        }

        // Update the physic
        __temp_vectors.clear();points().clear();
        int needed_update_physic = 0;if(update_physic_with_events()){needed_update_physic = update_physic(window_struct().delta_time());}
        for(int i = 0;i<static_cast<int>(a_datas.get()->physic_objects().size());i++) {
            if(a_datas.get()->physic_objects().at(i).get()->is_static()){continue;}
            std::vector<scls::Point_2D> needed_points = a_datas.get()->physic_objects().at(i).get()->trajectory_points(100, scls::Fraction(1, 100));
            for(int j = 0;j<static_cast<int>(needed_points.size());j++){
                std::shared_ptr<Vector> current_point = new_point(std::string("trajectory_") + std::to_string(j), needed_points.at(j));
                current_point.get()->set_color(scls::Color(255, 0, 0));
                __temp_vectors.push_back(current_point);
            }
        }

        // Move the plane
        bool modified = (needed_update_physic > 0);
        if(is_focused()) {
            scls::Fraction speed = scls::Fraction(round(5.0 * window_struct().delta_time() * 10000.0), 10000);
            if(window_struct().key_pressed("q")){middle_x_add(speed * -1);modified = true;}
            if(window_struct().key_pressed("d")){middle_x_add(speed);modified = true;}
            if(window_struct().key_pressed("z")){middle_y_add(speed);modified = true;}
            if(window_struct().key_pressed("s")){middle_y_add(speed * -1);modified = true;}
            // Zoom or unzoom
            double zoom_speed = 30 * window_struct().delta_time();
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
                add_vector(Vector("a", movement_x, movement_y));
                a_created_vectors_at_click.push_back(vectors().at(vectors().size() - 1));
                modified = true;
            }
            else if(operation_at_click() == PLEOS_OPERATION_POINT) {
                // Add a point
                double needed_x = window_struct().mouse_x() - x_in_absolute_pixel();
                double needed_y = window_struct().mouse_y() - (window_struct().window_height() - (y_in_absolute_pixel() + height_in_pixel()));
                scls::Fraction movement_x = pixel_x_to_graphic_x(needed_x, texture()->image_shared_ptr());
                scls::Fraction movement_y = pixel_y_to_graphic_y(needed_y, texture()->image_shared_ptr());
                Vector needed_vector = Vector("a", movement_x, movement_y);
                needed_vector.set_type(Vector_Type::VT_Point);
                add_vector(needed_vector);
                a_created_vectors_at_click.push_back(vectors().at(vectors().size() - 1));
                modified = true;
            }
            else if(operation_at_click() == PLEOS_OPERATION_FORM) {
                // Add a form
                // Check if the form is created
                if(current_form_2d() == 0){a_current_form_2d=std::make_shared<Form_2D>("Forme");add_form(a_current_form_2d);}
                // Add a point
                double needed_x = window_struct().mouse_x() - x_in_absolute_pixel();
                double needed_y = window_struct().mouse_y() - (window_struct().window_height() - (y_in_absolute_pixel() + height_in_pixel()));
                scls::Fraction movement_x = pixel_x_to_graphic_x(needed_x, texture()->image_shared_ptr());
                scls::Fraction movement_y = pixel_y_to_graphic_y(needed_y, texture()->image_shared_ptr());
                std::shared_ptr<Vector> needed_vector = std::make_shared<Vector>(std::string("Forme_") + std::to_string(current_form_2d()->points().size()), movement_x, movement_y);
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
        // Soft-reset the physic
        for(int i = 0;i<static_cast<int>(graphic()->physic_objects().size());i++) {
            if(graphic()->physic_objects().at(i).get()->attached_object()==0||graphic()->physic_objects().at(i).get()->attached_transform()==0){
                graphic()->physic_objects().erase(graphic()->physic_objects().begin() + i);i--;
            }
            else{graphic()->physic_objects().at(i).get()->soft_reset();}
        }

        // Realised updates
        int needed_update = 0;

        // Apply gravity
        for(int i = 0;i<static_cast<int>(graphic()->physic_objects().size());i++) {if(graphic()->physic_objects().at(i).get()->use_gravity()){graphic()->physic_objects().at(i).get()->accelerate(gravity * scls::Fraction::from_double(used_delta_time));needed_update++;}}

        // Update raw velocity
        for(int i = 0;i<static_cast<int>(graphic()->physic_objects().size());i++) {graphic()->physic_objects().at(i).get()->update_raw_velocity();}

        // Update each objects in the case
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
                    graphic()->physic_objects().at(i)->accelerate(graphic()->physic_objects().at(i).get()->current_collisions_results().at(j).get()->acceleration);
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
            std::shared_ptr<scls::Image> needed_image = a_datas.get()->to_image(needed_width, needed_height);
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
}
