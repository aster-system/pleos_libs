//******************
//
// pleos_mathematics_geometry.h
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
// This file contains the source code of pleos_mathematics_geometry.cpp.
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

// Include pleos_mathematics_probabilities.h
#include "../../pleos_mathematics/pleos_mathematics_geometry.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //******************
    //
    // The "Point_2D" class
    //
    //******************

    // Point_2D constructor
    Point_2D::Point_2D(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, scls::Point_2D point):__Graphic_Object_Base(graphic_base, name, point){};
    Point_2D::Point_2D(std::weak_ptr<__Graphic_Base> graphic_base, scls::Point_2D point):Point_2D(graphic_base, std::string(), point){};
    Point_2D::Point_2D(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, double x, double y):Point_2D(graphic_base, name, scls::Point_2D(x, y)){};
    Point_2D::Point_2D(std::weak_ptr<__Graphic_Base> graphic_base, double x, double y):Point_2D(graphic_base, std::string(), x, y){};
    Point_2D::Point_2D(std::weak_ptr<__Graphic_Base> graphic_base):Point_2D(graphic_base, std::string(), scls::Point_2D(0, 0)){};

    // Returns the needed XML text to generate this object
    std::string Point_2D::to_displayed_text(){return std::string("point");}
    std::string Point_2D::to_xml_text(){return std::string("<") + to_xml_text_base() + std::string(">");}
    std::string Point_2D::to_xml_text_object_name(){return std::string("point");}

    //******************
    //
    // The "Form_2D" class
    //
    //******************

    // Adds an exclusion point to the form
    void Form_2D::add_exclusion_point(std::shared_ptr<Point_2D> point){a_exclusion_points.push_back(point);};

    // Adds a point to the form
    void Form_2D::add_point_external(std::shared_ptr<__Graphic_Object_Base> point){if(!contains_point(point.get())){a_points.push_back(point);a_points_link.push_back(Link());}};
    void Form_2D::add_point(std::shared_ptr<__Graphic_Object_Base> point){add_point_external(point);point.get()->set_parent(this_object_shared_ptr());};

    // Clones the form
    void Form_2D::clone(std::shared_ptr<Form_2D> needed_clone) {
        // Sets the basic datas
        needed_clone.get()->set_border_color(border_color());
        needed_clone.get()->set_cloned_from(this_object_shared_ptr());
        needed_clone.get()->set_color(color());

        // Clone the points
        for(int i = 0;i<static_cast<int>(points().size());i++){std::shared_ptr<Point_2D> created_point = needed_clone.get()->new_point(points().at(i).get()->absolute_position());}
    }

    // Returns if the form contains a point or not
    bool Form_2D::contains_point(__Graphic_Object_Base* tested_point){for(int i=0;i<static_cast<int>(a_points.size());i++){if(a_points.at(i).get()==tested_point){return true;}}return false;};

    // Draws the circle on an image
    void Form_2D::draw_on_image(std::shared_ptr<scls::__Image_Base> image) {
        // Asserts
        if(points().size() < 2){return;}
        else if(points().size() == 2) {
            // Draw a line
            __Graphic_Object_Base* point_1 = points().at(0).get();
            __Graphic_Object_Base* point_2 = points().at(1).get();
            double last_x = graphic_x_to_pixel_x(point_1->absolute_x());
            double last_y = graphic_y_to_pixel_y_inversed(point_1->absolute_y());
            double needed_x = graphic_x_to_pixel_x(point_2->absolute_x());
            double needed_y = graphic_y_to_pixel_y_inversed(point_2->absolute_y());
            // Apply the proportion
            double needed_proportion = link(0).drawing_proportion;
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
            int line_width = border_radius();
            image.get()->draw_line(last_x + needed_move_x_minus + line_width / 2.0, last_y + needed_move_y_minus, last_x + needed_move_x, last_y + needed_move_y, color_with_absolute_opacity(border_color()), line_width);
            return;
        }

        // Triangulate the form
        std::vector<std::shared_ptr<Point_2D>> current_triangulated_points = triangulated_points_external();

        // Draw the inner form
        scls::Color inner_color = color_with_absolute_opacity(color());
        if(inner_color.alpha() > 0) {
            for(int i = 0;i<static_cast<int>(current_triangulated_points.size());i+=3) {
                std::shared_ptr<Point_2D> current_point = current_triangulated_points[i];
                double first_x = graphic_x_to_pixel_x(current_point.get()->x());
                double first_y = graphic_y_to_pixel_y_inversed(current_point.get()->y());
                current_point = current_triangulated_points[i + 1];
                double second_x = graphic_x_to_pixel_x(current_point.get()->x());
                double second_y = graphic_y_to_pixel_y_inversed(current_point.get()->y());
                current_point = current_triangulated_points[i + 2];
                double third_x = graphic_x_to_pixel_x(current_point.get()->x());
                double third_y = graphic_y_to_pixel_y_inversed(current_point.get()->y());
                image.get()->fill_triangle(first_x, first_y, second_x, second_y, third_x, third_y, inner_color);
            } current_triangulated_points.clear();
        }

        // Draw the links
        std::shared_ptr<__Graphic_Object_Base> last_point = points()[points().size() - 1];
        double last_x = graphic_x_to_pixel_x(last_point.get()->absolute_x());
        double last_y = graphic_y_to_pixel_y_inversed(last_point.get()->absolute_y());

        // Link each points
        scls::Color current_border_color = color_with_absolute_opacity(border_color());
        for(int j = 0;j<static_cast<int>(points().size());j++) {
            std::shared_ptr<__Graphic_Object_Base> current_point = points()[j];
            double needed_x = graphic_x_to_pixel_x(current_point.get()->absolute_x());
            double needed_y = graphic_y_to_pixel_y_inversed(current_point.get()->absolute_y());
            Form_2D::Link current_link;
            if(j <= 0){current_link = last_link();}
            else{current_link = link(j - 1);}

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

            image.get()->draw_line(last_x + needed_move_x_minus, last_y + needed_move_y_minus, last_x + needed_move_x, last_y + needed_move_y, current_border_color, border_radius());
            last_point = current_point; last_x = needed_x; last_y = needed_y;
        }
    }

    // Returns the introduction of the form 2D
    std::string Form_2D::introduction() const {return std::string("Nous avons le ") + type_name(false) + std::string(" ") + name() + std::string(".");};

    // Creates a new point to the form
    std::shared_ptr<Point_2D> Form_2D::new_point(double x, double y){
        std::shared_ptr<Point_2D> point = std::make_shared<Point_2D>(graphic_base_shared_ptr(), name() + std::string("_") + std::to_string(a_points.size()), x, y);
        point.get()->set_this_object(point);point.get()->set_parent(this_object_shared_ptr());add_point(point);
        return point;
    }
    std::shared_ptr<Point_2D> Form_2D::new_point(scls::Point_2D point){return new_point(point.x(), point.y());}

    // Returns a parameter by its name
    std::string Form_2D::parameter(std::string parameter_name) {
        if(parameter_name == std::string("border_color")){return border_color().to_std_string(0);}
        else if(parameter_name == std::string("color")){return color().to_std_string(0);}
        return __Graphic_Object_Base::parameter(parameter_name);
    }
    // Sets a parameter by its name
    void Form_2D::set_parameter(std::string parameter_name, std::string parameter_value, std::string parameter_value_start, double proportion) {
        if(parameter_name == std::string("border_color")){
            scls::Color needed_color = scls::Color::from_std_string(parameter_value);
            if(proportion < 1){scls::Color base_color = scls::Color::from_std_string(parameter_value_start);needed_color = base_color + (needed_color - base_color) * proportion;}

            set_border_color(needed_color);
        }
        else if(parameter_name == std::string("color")){
            scls::Color needed_color = scls::Color::from_std_string(parameter_value);
            if(proportion < 1){scls::Color base_color = scls::Color::from_std_string(parameter_value_start);needed_color = base_color + (needed_color - base_color) * proportion;}

            set_color(needed_color);
        }
        else if(parameter_name == std::string("proportion")){if(a_points_link.size() > 0){a_points_link[0].drawing_proportion = scls::string_to_double(parameter_value) * proportion;}}
        else{__Graphic_Object_Base::set_parameter(parameter_name, parameter_value, parameter_value_start, proportion);}
    }

    // Returns this object to an XML text
    std::string Form_2D::to_displayed_text(){return std::string("forme");}
    std::string Form_2D::to_xml_text() {
        // Basic datas
        std::string content = std::string();

        // Clone
        if(cloned_from() != 0){
            content += std::string("<form_clone from=") + cloned_from()->name() + to_xml_text_name();
            content += std::string(">");
            return content;
        }

        // Check the common forms
        if(a_points.size() == 2) {
            // The form is a line

            // Needed coordinates
            scls::__Formula x_1 = a_points.at(0).get()->absolute_x();
            scls::__Formula x_2 = a_points.at(1).get()->absolute_x();
            scls::__Formula y_1 = a_points.at(0).get()->absolute_y();
            scls::__Formula y_2 = a_points.at(1).get()->absolute_y();

            // Add the form
            double proportion = a_points_link.at(0).drawing_proportion;
            std::string proportion_string = std::string();if(proportion != 1){proportion_string = std::string(" proportion=") + std::to_string(proportion);}
            content += std::string("<line") + to_xml_text_name() + to_xml_text_tags() + to_xml_text_opacity() + to_xml_text_color(std::string("border_color"), border_color()) + std::string(" x_1=") + x_1.to_std_string(0) + std::string(" y_1=") + y_1.to_std_string(0) + std::string(" x_2=") + x_2.to_std_string(0) + std::string(" y_2=") + y_2.to_std_string(0) + to_xml_text_border_radius() + proportion_string + std::string(" physic=1 collision=line>");
            return content;
        }
        else if(a_points.size() == 4 && object_name() == std::string("rect")) {
            // The form is a rect

            /*// Needed coordinates
            scls::Fraction x_1 = a_points.at(0).get()->absolute_x();
            scls::Fraction x_2 = a_points.at(1).get()->absolute_x();
            scls::Fraction x_3 = a_points.at(2).get()->absolute_x();
            scls::Fraction x_4 = a_points.at(3).get()->absolute_x();
            scls::Fraction y_1 = a_points.at(0).get()->absolute_y();
            scls::Fraction y_2 = a_points.at(1).get()->absolute_y();
            scls::Fraction y_3 = a_points.at(2).get()->absolute_y();
            scls::Fraction y_4 = a_points.at(3).get()->absolute_y();
            //*/

            // Get the good datas
            scls::__Formula needed_height = height_formula();
            scls::__Formula needed_width = width_formula();
            scls::__Formula needed_x = x_formula() - needed_width / 2;
            scls::__Formula needed_y = y_formula() - needed_height / 2;

            // Add the form
            content += std::string("<rect") + to_xml_text_name() + to_xml_text_tags() + to_xml_text_parent() + to_xml_text_opacity() + to_xml_text_color(std::string("border_color"), border_color()) + to_xml_text_color(std::string("color"), color()) + std::string(" x=") + needed_x.to_std_string(0) + std::string(" y=") + needed_y.to_std_string(0) + std::string(" width=") + needed_width.to_std_string(0) + std::string(" height=") + needed_height.to_std_string(0) + to_xml_text_border_radius() + std::string(">");
            return content;
        }
        else if(object_name().size() > 6 && object_name().substr(0, 7) == std::string("polygon")) {
            // Add the form
            std::vector<std::string> cutted = scls::cut_string(object_name(), std::string("_"));
            int number = -1;if(cutted.size() >= 2){number =std::stoi(cutted.at(1));}
            content += std::string("<") + to_xml_text_object_name() + to_xml_text_name() + to_xml_text_tags() + to_xml_text_opacity() + to_xml_text_color(std::string("border_color"), border_color()) + to_xml_text_color(std::string("color"), color()) + to_xml_text_width() + to_xml_text_height() + std::string(" polygon=") + std::to_string(number) + std::string(">");
            return content;
        }

        // Add the points
        std::string point_names = std::string();
        for(int i = 0;i<static_cast<int>(a_points.size());i++){__Graphic_Object_Base* p = a_points.at(i).get();point_names += p->name() + std::string(";");}
        if(point_names.size() > 0 && point_names.at(point_names.size() - 1) == ';'){point_names = point_names.substr(0, point_names.size() - 1);}

        // Add the form
        content += std::string("<") + to_xml_text_object_name() + to_xml_text_name() + to_xml_text_tags() + to_xml_text_opacity() + to_xml_text_color(std::string("border_color"), border_color()) + to_xml_text_color(std::string("color"), color()) + to_xml_text_x() + to_xml_text_y() + to_xml_text_width() + to_xml_text_height() + std::string(" points=") + point_names + std::string(">");
        return content;

    }
    std::string Form_2D::to_xml_text_border_radius(){return std::string(" border_radius=") + std::to_string(border_radius());}
    std::string Form_2D::to_xml_text_object_name(){return std::string("form");}

    // Returns a list of the points triangulated
    void Form_2D::triangulate_points_external() {
        // Assert
        a_last_triangulation.reset();
        if(static_cast<int>(points().size()) < 3){return;}

        // Triangulate the face with the model maker part of SCLS
        scls::model_maker::Face form_to_face;
        for(int i = 0;i<static_cast<int>(points().size());i++) {
            scls::model_maker::Point current_point = points().at(i).get()->to_point_3d_absolute();
            form_to_face.points().push_back(std::make_shared<scls::model_maker::Point>(current_point));
        }
        for(int i = 0;i<static_cast<int>(exclusion_points().size());i++) {
            scls::model_maker::Point current_point = exclusion_points()[i].get()->to_point_3d_absolute();
            form_to_face.exclusion_points().push_back(std::make_shared<scls::model_maker::Point>(current_point));
        }
        a_last_triangulation = form_to_face.triangulate();
    }
    std::vector<std::shared_ptr<Point_2D>> Form_2D::triangulated_points_external() {
        // Triangulate the point
        if(a_triangulate_while_drawing){triangulate_points_external();}
        else{if(a_last_triangulation.get() == 0){triangulate_points_external();}}
        if(a_last_triangulation.get() == 0){return std::vector<std::shared_ptr<Point_2D>>();}

        // Get the needed result
        std::vector<std::shared_ptr<scls::model_maker::Point>>& points_for_rendering = a_last_triangulation.get()->points_for_rendering;
        std::vector<std::shared_ptr<Point_2D>> to_return = std::vector<std::shared_ptr<Point_2D>>(points_for_rendering.size());
        for(int i = 0;i<static_cast<int>(points_for_rendering.size());i++) {
            scls::model_maker::Point* current_point = points_for_rendering.at(i).get();
            to_return[i]=Point_2D::from_point(graphic_base_shared_ptr(), current_point);
        }
        return to_return;
    }

    // Returns the name of the type of the form
    std::string Form_2D::type_name(bool capitalise_first_letter) const {std::string to_return=std::string();if(a_points.size()==3){if(capitalise_first_letter){return std::string("Triangle");}return std::string("triangle");}if(capitalise_first_letter){to_return+=std::string("Forme");}else{to_return+=std::string("forme");}to_return+=std::string(" à ")+std::to_string(a_points.size())+std::string(" points");return to_return;};
    std::string Form_2D::type_name() const {return type_name(true);};

    //******************
    //
    // The "Circle" class
    //
    //******************

    // Circle constructor
    Circle::Circle(std::weak_ptr<__Graphic_Base> graphic_base, std::string name, double x, double y, double radius_x, double radius_y):__Graphic_Object_Base(graphic_base, name, scls::Point_2D(x, y)){set_radius_x(radius_x);set_radius_y(radius_y);}

    // Draws the circle on an image
    void Circle::draw_on_image(std::shared_ptr<scls::__Image_Base> image){
        scls::Point_2D current_center = center();
        double current_radius_x = radius_x().to_double();current_radius_x = current_radius_x * pixel_by_case_x();
        double current_radius_y = radius_y().to_double();current_radius_y = current_radius_y * pixel_by_case_y();
        double needed_x = graphic_x_to_pixel_x(current_center.x());
        double needed_y = graphic_y_to_pixel_y_inversed(current_center.y());
        image.get()->fill_circle(needed_x, needed_y, current_radius_x, current_radius_y, rotation().to_double(), angle_start(), angle_end(), color_with_absolute_opacity(color()), border_radius(), color_with_absolute_opacity(border_color()));
    }

    // Returns a parameter by its name
    std::string Circle::parameter(std::string parameter_name){
        if(parameter_name == std::string("angle_end")){return angle_end_formula().to_std_string(0);}
        else if(parameter_name == std::string("angle_start")){return angle_start_formula().to_std_string(0);}
        return __Graphic_Object_Base::parameter(parameter_name);
    }

    // Sets a parameter by its name
    void Circle::set_parameter(std::string parameter_name, std::string parameter_value, std::string parameter_value_start, double proportion){
        if(parameter_name == std::string("angle_end")){
            double needed_angle = scls::string_to_formula(parameter_value).get()->value_to_double();
            if(proportion < 1){double base_angle = scls::string_to_formula(parameter_value_start).get()->value_to_double();needed_angle = base_angle + (needed_angle - base_angle) * proportion;}

            set_angle_end(needed_angle);
        }
        else if(parameter_name == std::string("angle_start")){
            double needed_angle = scls::string_to_formula(parameter_value).get()->value_to_double();
            if(proportion < 1){double base_angle = scls::string_to_formula(parameter_value_start).get()->value_to_double();needed_angle = base_angle + (needed_angle - base_angle) * proportion;}

            set_angle_start(needed_angle);
        }
        else if(parameter_name == std::string("color")){
            scls::Color needed_color = scls::Color::from_std_string(parameter_value);
            if(proportion < 1){scls::Color base_color = scls::Color::from_std_string(parameter_value_start);needed_color = base_color + (needed_color - base_color) * proportion;}

            set_color(needed_color);
        }
        else{__Graphic_Object_Base::set_parameter(parameter_name, parameter_value, parameter_value_start, proportion);}
    }

    // Returns the needed XML text to generate this object
    std::string Circle::to_displayed_text(){return std::string("cercle");}
    std::string Circle::to_xml_text_angle_end(){if(angle_end() == 360){return std::string();}return std::string(" angle_end=") + scls::remove_space(angle_end_formula().to_std_string(0));}
    std::string Circle::to_xml_text_angle_start(){if(angle_start() == 0){return std::string();}return std::string(" angle_start=") + scls::remove_space(angle_start_formula().to_std_string(0));}
    std::string Circle::to_xml_text_border_radius(){return std::string(" border_radius=") + std::to_string(border_radius());};
    std::string Circle::to_xml_text_radius(){if(radius_x() != radius_y()){return std::string();}return std::string(" radius=") + radius_x().to_std_string(0);}
    std::string Circle::to_xml_text_radius_x(){if(radius_x() == 1 || radius_x() == radius_y()){return std::string();}return std::string(" radius_x=") + radius_x().to_std_string(0);}
    std::string Circle::to_xml_text_radius_y(){if(radius_y() == 1 || radius_x() == radius_y()){return std::string();}return std::string(" radius_y=") + radius_y().to_std_string(0);}
    std::string Circle::to_xml_text_object_name(){return std::string("circle");}
    std::string Circle::to_xml_text(){return std::string("<") + to_xml_text_object_name() + to_xml_text_name() + to_xml_text_opacity() + to_xml_text_parent() + to_xml_text_rotation() + to_xml_text_tags() + to_xml_text_border_radius() + to_xml_text_color(std::string("border_color"), border_color()) + to_xml_text_color(std::string("color"), color()) + to_xml_text_x() + to_xml_text_y() + to_xml_text_radius() + to_xml_text_radius_x() + to_xml_text_radius_y() + to_xml_text_angle_start() + to_xml_text_angle_end() + std::string(">");}
}
