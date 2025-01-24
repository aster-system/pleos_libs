//******************
//
// pleos_mathematics_functions.h
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
// This file contains the handling of functions in the PLEOS mathematics file.
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

#ifndef PLEOS_MATHEMATICS_FUNCTIONS
#define PLEOS_MATHEMATICS_FUNCTIONS

// Include SCLS Graphic Benoit
#include "pleos_mathematics_geometry.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {
    // Possible studied type
    enum Studied_Type {ST_Sequence};
    // Datas about a studied function
    struct Function_Studied {
        // Formula of the function
        scls::Formula function_formula;
        // Name of the function
        std::string function_name = "";
        // Number of the function
        int function_number = 0;
        // Name of the unknown in the function
        std::string function_unknown = "x";
        // Type of the studied function
        Studied_Type type = Studied_Type::ST_Sequence;

        // Studied things
        // Definition set of the function
        std::shared_ptr<scls::Set_Number> definition_set;
    };

    // Returns the definition set of a function
    scls::Set_Number function_definition_set(Function_Studied* current_function, std::string* redaction);
    // Returns the derivation of a function
    scls::Formula function_derivation(Function_Studied current_function, std::string& redaction);
    // Returns the image of a function
    scls::Formula function_image(Function_Studied current_function, scls::Formula needed_value, std::string& redaction);
    // Returns the limit of a function / polymonial in + infinity
    scls::Limit function_limit(Function_Studied current_function, scls::Limit needed_limit, std::string& redaction);
    // Returns the set of roots of a function
    scls::Set_Number function_roots(Function_Studied current_function, std::string& redaction);
    // Returns the set of a positive function
    scls::Set_Number function_sign(Function_Studied current_function, std::string& redaction);

    //******************
    //
    // Sequences handling
    //
    //******************

    // Returns the interval of an sequence
    scls::Interval sequence_variation(Function_Studied current_function, std::string& redaction);

    //******************
    //
    // Graphic handling
    //
    //******************

    class Graphic : public scls::GUI_Object {
        // Class representating a graphic for one (or a lot) of functions

    public:

        // Needed fragment shader for the function
        static std::string graphic_function_fragment_shader(scls::Formula needed_formula);

        // Base of the graphic
        struct __Graphic_Base {int a_function_number = 0;scls::Fraction a_middle_x = 0;scls::Fraction a_middle_y = 0;double a_pixel_by_case_x = 100;double a_pixel_by_case_y = 100;};

        //******************
        //
        // The Graphic_Function handling
        //
        //******************

        class Graphic_Function {
            // Class representating a function for a graphic

        public:

            // Graphic_Function constructor
            Graphic_Function(std::shared_ptr<Function_Studied> function_studied);

            // Getters and setters
            inline scls::Set_Number* definition_set(){return a_function_studied.get()->definition_set.get();};
            inline scls::Formula& formula(){return a_function_studied.get()->function_formula;};
            inline scls::Fraction middle_x() const {return a_graphic_base.get()->a_middle_x;};
            inline scls::Fraction middle_y() const {return a_graphic_base.get()->a_middle_y;};
            inline double pixel_by_case_x() const {return a_graphic_base.get()->a_pixel_by_case_x;};
            inline double pixel_by_case_y() const {return a_graphic_base.get()->a_pixel_by_case_y;};
            inline void set_graphic_base(std::shared_ptr<__Graphic_Base> base) {a_graphic_base=base;};

        private:
            // Formula of the function
            std::shared_ptr<Function_Studied> a_function_studied;
            // Datas about the plane
            std::shared_ptr<__Graphic_Base> a_graphic_base;
        };

        //******************
        //
        // Graphic handling
        //
        //******************

        // Graphic constructor
        Graphic(scls::_Window_Advanced_Struct& window, std::string name, std::weak_ptr<scls::GUI_Object> parent);

        // Function called after creation
        virtual void after_creation();
        // Renders the object
        virtual void render(glm::vec3 scale_multiplier = glm::vec3(1, 1, 1));
        // Updates the object
        virtual void update();
        virtual void update_texture(){texture()->set_image(to_image());};

        // Adds a function to the graphic
        void add_function(std::shared_ptr<Function_Studied> function_studied);
        // Resets the object
        inline void reset(){a_functions.clear();};
        // Returns the image of the graphic
        std::shared_ptr<scls::Image> to_image();

        // Adds a circle to the graphic
        inline void add_circle(std::string circle_name, Vector center, scls::Formula radius){a_circles.push_back(std::make_shared<Circle>(circle_name, center, radius));};
        // Adds a vector to the graphic
        inline void add_vector(std::string circle_name, Vector needed_vector){a_vectors.push_back(std::make_shared<Vector>(needed_vector));};

        // Getters and setters
        inline scls::Fraction middle_x() const {return a_graphic_base.get()->a_middle_x;};
        inline scls::Fraction middle_y() const {return a_graphic_base.get()->a_middle_y;};
        inline double pixel_by_case_x() const {return a_graphic_base.get()->a_pixel_by_case_x;};
        inline double pixel_by_case_y() const {return a_graphic_base.get()->a_pixel_by_case_y;};

    private:
        // Private functions to draw the image
        int graphic_x_to_pixel_x(double x, std::shared_ptr<scls::Image>& needed_image);
        int graphic_y_to_pixel_y(double y, std::shared_ptr<scls::Image>& needed_image);
        int graphic_y_to_pixel_y_inversed(double y, std::shared_ptr<scls::Image>& needed_image);
        scls::Fraction pixel_x_to_graphic_x(int x, std::shared_ptr<scls::Image>& needed_image);
        double pixel_y_to_graphic_y(double y, std::shared_ptr<scls::Image>& needed_image);

        // Loaded function
        std::vector<std::shared_ptr<Graphic_Function>> a_functions;
        // Datas about the plane
        std::shared_ptr<__Graphic_Base> a_graphic_base = std::make_shared<__Graphic_Base>();

        // Geometrical objects
        std::vector<std::shared_ptr<Circle>> a_circles;
        // Geometrical vectors
        std::vector<std::shared_ptr<Vector>> a_vectors;
    };
}

#endif // PLEOS_MATHEMATICS_FUNCTIONS
