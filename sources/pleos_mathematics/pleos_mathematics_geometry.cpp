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
    // The base of all the next class
    //
    //******************

    // __Graphic_Object_Base constructor
    int __current_id = 0;
    __Graphic_Object_Base::__Graphic_Object_Base(std::string name, scls::__Point_2D_Formula position):a_id(__current_id),a_transform(std::make_shared<scls::Transform_Object_2D>(position)),a_name(name){a_id = __current_id;__current_id++;a_transform.get()->set_this_object(a_transform);};
    __Graphic_Object_Base::__Graphic_Object_Base(std::string name, scls::Point_2D position):a_id(__current_id),a_transform(std::make_shared<scls::Transform_Object_2D>(position)),a_name(name){a_id = __current_id;__current_id++;a_transform.get()->set_this_object(a_transform);};

    // Returns an introduction of the object
    std::string __Graphic_Object_Base::introduction(scls::Textual_Math_Settings* settings) const {return std::string("Nous avons l'object \"") + name() + std::string("\".");};

    // Returns the needed XML text to generate this object
    std::string __Graphic_Object_Base::to_xml_text_color(std::string attribute_name, scls::Color color){return std::string(" ") + attribute_name + std::string("=(") + std::to_string(color.red()) + std::string(",") + std::to_string(color.green()) + std::string(",") + std::to_string(color.blue()) + std::string(",") + std::to_string(color.alpha()) + std::string(")");};
    std::string __Graphic_Object_Base::to_xml_text_height(std::string attribute_name){if(height() == 1){return std::string();}return std::string(" ") + attribute_name + std::string("=") + height().to_std_string(0);}
    std::string __Graphic_Object_Base::to_xml_text_height(){return to_xml_text_height(std::string("height"));}
    std::string __Graphic_Object_Base::to_xml_text_name(){if(a_name == std::string()){return std::string();}return std::string(" name=\"") + a_name + std::string("\"");}
    std::string __Graphic_Object_Base::to_xml_text_object_name(){return std::string("object");}
    std::string __Graphic_Object_Base::to_xml_text_x(){if(x() == 0){return std::string();}return std::string(" x=") + x().to_std_string(0);}
    std::string __Graphic_Object_Base::to_xml_text_y(){if(y() == 0){return std::string();}return std::string(" y=") + y().to_std_string(0);}
    std::string __Graphic_Object_Base::to_xml_text_width(std::string attribute_name){if(width() == 1){return std::string();}return std::string(" ") + attribute_name + std::string("=") + width().to_std_string(0);}
    std::string __Graphic_Object_Base::to_xml_text_width(){return to_xml_text_width(std::string("width"));}

    //******************
    //
    // The "Form_2D" class
    //
    //******************

    // Returns the introduction of the form 2D
    std::string Form_2D::introduction() const {return std::string("Nous avons le ") + type_name(false) + std::string(" ") + name() + std::string(".");};

    // Creates a new point to the form
    std::shared_ptr<Point_2D> Form_2D::new_point(scls::Fraction x, scls::Fraction y){
        std::shared_ptr<Point_2D> point = std::make_shared<Point_2D>(name() + std::string("_") + std::to_string(a_points.size()), x, y);
        add_point(point);
        return point;
    };

    // Returns this object to an XML text
    std::string Form_2D::to_xml_text() {
        // Basic datas
        std::string content = std::string();

        // The form is a line
        if(a_points.size() == 2) {
            // Needed coordinates
            scls::Fraction x_1 = a_points.at(0).get()->absolute_x();
            scls::Fraction x_2 = a_points.at(1).get()->absolute_x();
            scls::Fraction y_1 = a_points.at(0).get()->absolute_y();
            scls::Fraction y_2 = a_points.at(1).get()->absolute_y();
            std::cout << "A " << x_1.to_double() << " " << y_1.to_double() << std::endl;

            // Add the form
            content += std::string("<line") + to_xml_text_name() + to_xml_text_color(std::string("border_color"), border_color()) + to_xml_text_color(std::string("color"), color()) + std::string(" x_1=") + x_1.to_std_string(0) + std::string(" y_1=") + y_1.to_std_string(0) + std::string(" x_2=") + x_2.to_std_string(0) + std::string(" y_2=") + y_2.to_std_string(0) + std::string(">");
            return content;
        }

        // Add the points
        std::string point_names = std::string();
        for(int i = 0;i<static_cast<int>(a_points.size());i++){
            Point_2D* p = a_points.at(i).get();
            content += std::string("<point name=") + p->name();
            if(p->x() != 0){content += std::string(" x=") + p->x().to_std_string(0);}
            if(p->y() != 0){content += std::string(" y=") + p->y().to_std_string(0);}
            content += std::string(">");
            point_names += p->name() + std::string(";");
        }
        if(point_names.at(point_names.size() - 1) == ';'){point_names = point_names.substr(0, point_names.size() - 1);}

        // Add the form
        content += std::string("<") + to_xml_text_object_name() + to_xml_text_name() + to_xml_text_color(std::string("border_color"), border_color()) + to_xml_text_color(std::string("color"), color()) + to_xml_text_x() + to_xml_text_y() + to_xml_text_width() + to_xml_text_height() + std::string(" points=") + point_names + std::string(">");
        return content;

    }
    std::string Form_2D::to_xml_text_object_name(){return std::string("form");}

    // Returns a list of the points triangulated
    std::vector<std::shared_ptr<Point_2D>> Form_2D::triangulated_points() {
        // Triangulate the face with the model maker part of SCLS
        scls::model_maker::Face form_to_face;
        for(int i = 0;i<static_cast<int>(points().size());i++) {
            scls::model_maker::Point current_point = points()[i].get()->to_point_3d();
            form_to_face.points().push_back(std::make_shared<scls::model_maker::Point>(current_point));
        }
        for(int i = 0;i<static_cast<int>(exclusion_points().size());i++) {
            scls::model_maker::Point current_point = exclusion_points()[i].get()->to_point_3d();
            form_to_face.exclusion_points().push_back(std::make_shared<scls::model_maker::Point>(current_point));
        }
        form_to_face.triangulate();

        // Get the needed result
        std::vector<std::shared_ptr<Point_2D>> to_return = std::vector<std::shared_ptr<Point_2D>>(form_to_face.points_for_rendering().size());
        for(int i = 0;i<static_cast<int>(form_to_face.points_for_rendering().size());i++) {
            scls::model_maker::Point* current_point = form_to_face.points_for_rendering()[i].get();
            to_return[i]=Point_2D::from_point(current_point);
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

    // Returns the needed XML text to generate this object
    std::string Circle::to_xml_text_angle_end(){if(angle_end() == 360){return std::string();}return std::string(" angle_end=") + scls::remove_space(angle_end().to_std_string(0));}
    std::string Circle::to_xml_text_angle_start(){if(angle_start() == 0){return std::string();}return std::string(" angle_start=") + scls::remove_space(angle_start().to_std_string(0));}
    std::string Circle::to_xml_text_radius_x(){if(radius_x() == 1){return std::string();}return std::string(" radius_x=") + radius_x().to_std_string(0);}
    std::string Circle::to_xml_text_radius_y(){if(radius_y() == 1){return std::string();}return std::string(" radius_y=") + radius_y().to_std_string(0);}
    std::string Circle::to_xml_text_object_name(){return std::string("circle");}
    std::string Circle::to_xml_text(){return std::string("<") + to_xml_text_object_name() + to_xml_text_name() + to_xml_text_color(std::string("border_color"), border_color()) + to_xml_text_color(std::string("color"), color()) + to_xml_text_x() + to_xml_text_y() + to_xml_text_radius_x() + to_xml_text_radius_y() + to_xml_text_angle_start() + to_xml_text_angle_end() + std::string(">");}
}
