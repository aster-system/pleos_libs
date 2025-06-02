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
    // The "Vector" class
    //
    //******************

    // Returns the mesured angle between to vector
    scls::Formula Vector::angle(Vector* needed_vector, std::string* redaction, scls::Textual_Math_Settings* settings) {
        // Start the redaction
        if(redaction != 0) {
            (*redaction) += std::string("Nous cherchons l'angle entre le vecteur ") + name() + std::string(" et le vecteur ") + needed_vector->name() + std::string(". ");
        }

        // Get the needed norm
        if(!a_last_norm_calculated){norm(redaction, settings);}
        scls::Formula needed_norm = a_last_norm;

        // Start the redaction
        if(redaction != 0) {
            (*redaction) += std::string("Nous savons que la norme de ") + name() + std::string(" est ") + needed_norm.to_std_string(settings) + std::string(". ");
        }

        return 0;
    }

    // Returns the complex number form of the vector (and the redaction if needed)
    scls::Formula Vector::complex_number(std::string* redaction, scls::Textual_Math_Settings* settings) {
        // Start the redaction
        if(redaction != 0 && x() != 0 && y() != 0) {
            (*redaction) += std::string("Nous cherchons le nombre complexe affixe du vecteur ") + name() + std::string(". ");
            (*redaction) += std::string("Pour cela, nous avons besoin des coordonnées du vecteur. ");
            (*redaction) += std::string("</br>");
            (*redaction) += std::string("<math><mi>Z</mi><mo>=</mo>") + x()->to_mathml(settings) + std::string("<mo>+</mo>") + (*y() * scls::Complex(0, 1)).to_mathml(settings) + std::string("</math>");
        }

        return (*x()) + (*y() * scls::Complex(0, 1));
    }

    // Returns the norm of the vector (and the redaction if needed)
    scls::Formula Vector::norm(std::string* redaction, scls::Textual_Math_Settings* settings) {
        // Start the redaction
        if(redaction != 0) {
            (*redaction) += std::string("Nous cherchons la norme du vecteur ") + name() + std::string(". ");
        }

        // Do the calculation
        scls::Formula to_return;
        for(int i = 0;i<static_cast<int>(a_coordinates.size());i++) {to_return += (*a_coordinates[i].get()) * (*a_coordinates[i].get());}
        to_return.set_applied_function<scls::__Sqrt_Function>();

        // Returns the needed result
        if(redaction != 0) {
            double value = to_return.applied_function()->real_value(&to_return);
            (*redaction) += std::string("La norme du vecteur ") + name() + std::string(" est donc ") + scls::format_number_to_text(value) + std::string(".");
        }
        a_last_norm = to_return;
        a_last_norm_calculated = true;
        return to_return;
    }

    //******************
    //
    // The base of all the next class
    //
    //******************

    // Returns the needed XML text to generate this object
    std::string __Graphic_Object_Base::to_xml_text_color(std::string attribute_name, scls::Color color){return std::string(" ") + attribute_name + std::string("=(") + std::to_string(color.red()) + std::string(",") + std::to_string(color.green()) + std::string(",") + std::to_string(color.blue()) + std::string(",") + std::to_string(color.alpha()) + std::string(")");};
    std::string __Graphic_Object_Base::to_xml_text_height(std::string attribute_name){if(height() == 1){return std::string();}return std::string(" ") + attribute_name + std::string("=") + height().to_std_string(0);}
    std::string __Graphic_Object_Base::to_xml_text_name(){if(a_name == std::string()){return std::string();}return std::string(" name=\"") + a_name + std::string("\"");}
    std::string __Graphic_Object_Base::to_xml_text_object_name(){return std::string("object");}
    std::string __Graphic_Object_Base::to_xml_text_x(){if(x() == 0){return std::string();}return std::string(" x=") + x().to_std_string(0);}
    std::string __Graphic_Object_Base::to_xml_text_y(){if(y() == 0){return std::string();}return std::string(" y=") + y().to_std_string(0);}
    std::string __Graphic_Object_Base::to_xml_text_width(std::string attribute_name){if(width() == 1){return std::string();}return std::string(" ") + attribute_name + std::string("=") + width().to_std_string(0);}

    //******************
    //
    // The "Form_2D" class
    //
    //******************

    // Returns a list of the points triangulated
    std::vector<std::shared_ptr<Vector>> Form_2D::triangulated_points() {
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
        std::vector<std::shared_ptr<Vector>> to_return = std::vector<std::shared_ptr<Vector>>(form_to_face.points_for_rendering().size());
        for(int i = 0;i<static_cast<int>(form_to_face.points_for_rendering().size());i++) {
            scls::model_maker::Point* current_point = form_to_face.points_for_rendering()[i].get();
            to_return[i]=Vector::from_point(current_point);
        }
        return to_return;
    }

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
    std::string Circle::to_xml_text(){return std::string("<") + to_xml_text_object_name() + to_xml_text_name() + to_xml_text_color(std::string("color"), color()) + to_xml_text_x() + to_xml_text_y() + to_xml_text_radius_x() + to_xml_text_radius_y() + to_xml_text_angle_start() + to_xml_text_angle_end() + std::string(">");}
}
