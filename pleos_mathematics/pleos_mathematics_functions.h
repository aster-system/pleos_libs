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

// Possibles operations at click
#define PLEOS_OPERATION_NOTHING 0
#define PLEOS_OPERATION_VECTOR 100
#define PLEOS_OPERATION_POINT 101
#define PLEOS_OPERATION_FORM 150

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

        // Returns the introduction for this function
        inline std::string introduction() const {return std::string("Nous avons la fonction ") + name() + std::string(" tel que ") + name() + std::string("(") + function_unknown + std::string(") = ") + function_formula.to_std_string() + std::string(" .");};

        // Connected object to this vector
        std::weak_ptr<scls::GUI_Text> a_connected_object = std::weak_ptr<scls::GUI_Text>();
        inline scls::GUI_Text* connected_object()const{return a_connected_object.lock().get();};
        inline void set_connected_object(std::weak_ptr<scls::GUI_Text> new_connected_object){a_connected_object = new_connected_object;};
        inline void set_formula(scls::Formula formula){function_formula = formula;};
        // Name of the function
        inline std::string name() const {return function_name;};
        inline void set_name(std::string new_name){function_name = new_name;if(connected_object()!=0){connected_object()->set_text(std::string("Fonction ") + new_name);}};

        // Studied things
        // Definition set of the function
        std::shared_ptr<scls::Set_Number> definition_set;
    };

    // Returns the definition set of a function
    scls::Set_Number function_definition_set(Function_Studied* current_function, std::string* redaction);
    // Returns the derivation of a function
    scls::Formula function_derivation(Function_Studied current_function, std::string& redaction);
    // Returns the image of a function
    scls::__Formula_Base::Formula function_image(Function_Studied current_function, scls::Formula needed_value, std::string& redaction);
    // Returns the limit of a function / polymonial in + infinity
    scls::Limit function_limit(Function_Studied current_function, scls::Limit needed_limit, std::string& redaction);
    // Returns the set of roots of a function
    scls::Set_Number function_roots(Function_Studied* current_function, std::string& redaction);
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

    class Graphic {
        // Class representating a graphic for one (or a lot) of functions
    public:

        // Needed fragment shader for the function
        static std::string graphic_function_fragment_shader(scls::Formula needed_formula);

        // Base of the graphic
        struct __Graphic_Base {int a_function_number = 0;double a_height = -1;scls::Fraction a_middle_x = 0;scls::Fraction a_middle_y = 0;double a_pixel_by_case_x = 100;double a_pixel_by_case_y = 100;double a_width = -1;};

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

        // Graphic constructor
        Graphic(){};

        // Sets the middle of the base
        inline void set_middle(double middle_x, double middle_y){a_graphic_base.get()->a_middle_x = middle_x;a_graphic_base.get()->a_middle_y = middle_y;};
        // Sets the scale of the base
        inline void set_scale(double width, double height){a_graphic_base.get()->a_height = height;a_graphic_base.get()->a_width = width;};

        // Annoying functions to draw the image
        int graphic_x_to_pixel_x(double x, std::shared_ptr<scls::Image>& needed_image);
        int graphic_y_to_pixel_y(double y, std::shared_ptr<scls::Image>& needed_image);
        int graphic_y_to_pixel_y_inversed(double y, std::shared_ptr<scls::Image>& needed_image);
        scls::Fraction pixel_x_to_graphic_x(int x, std::shared_ptr<scls::Image>& needed_image);
        scls::Fraction pixel_y_to_graphic_y(int y, std::shared_ptr<scls::Image>& needed_image);

        // Adds a function to the graphic
        void add_function(std::shared_ptr<Function_Studied> function_studied);
        // Resets the object
        inline void reset(){a_forms_2d.clear();a_functions.clear();a_points.clear();a_vectors.clear();};
        // Returns the image of the graphic
        std::shared_ptr<scls::Image> to_image(int width_in_pixel, int height_in_pixel);

        // Handle circles
        // Adds a circle to the graphic
        inline std::shared_ptr<Circle>* add_circle(std::string circle_name, Vector center, scls::Formula radius){a_circles.push_back(std::make_shared<Circle>(circle_name, center, radius));return &a_circles[a_circles.size() - 1];};
        // Removes circle from the graphic
        inline std::shared_ptr<Circle> remove_circle(std::string circle_name){for(int i = 0;i<static_cast<int>(a_circles.size());i++){if(a_circles[i].get()->name()==circle_name){std::shared_ptr<Circle> temp = a_circles[i];a_circles.erase(a_circles.begin() + i);return temp;} }return std::shared_ptr<Circle>();};

        // Handle forms
        // Adds a forms to the graphic
        inline void add_form(std::shared_ptr<Form_2D> needed_formd_2d){a_forms_2d.push_back(needed_formd_2d);};
        // Draws a form on the graphic
        void draw_form(Form_2D* needed_form, std::shared_ptr<scls::Image> to_return);
        // Returns a form by its name
        inline std::shared_ptr<Form_2D> form_shared_ptr(std::string form_name){for(int i = 0;i<static_cast<int>(a_forms_2d.size());i++){if(a_forms_2d[i].get()->name() == form_name){return a_forms_2d[i];}}return std::shared_ptr<Form_2D>();};
        inline Form_2D* form(std::string form_name){return form_shared_ptr(form_name).get();};
        // Creates and returns a form
        inline std::shared_ptr<Form_2D> new_form(std::string name){std::shared_ptr<Form_2D>to_return=std::make_shared<Form_2D>(name);add_form(to_return);return to_return;};
        // Creates and returns a form from a face
        inline std::shared_ptr<Form_2D> new_form_from_face(std::string name, scls::model_maker::Face* face){
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
        // Sets the points in a form of the graphic
        void set_form_points(Form_2D* needed_form, std::string points){std::vector<std::string> cutted = scls::cut_string(points, ";");for(int i = 0;i<static_cast<int>(cutted.size());i++){needed_form->add_point(point_shared_ptr(cutted[i]));}};
        void set_form_points(std::shared_ptr<Form_2D> needed_form, std::string points){set_form_points(needed_form.get(), points);};

        // Handle points
        // Adds a points to the graphic
        inline void add_point(std::shared_ptr<Vector> needed_point){a_points.push_back(needed_point);};
        inline void add_point(Vector needed_point){add_point(std::make_shared<Vector>(needed_point));};
        inline std::shared_ptr<Vector> add_point(std::string name, scls::model_maker::Point* needed_point){add_point(std::make_shared<Vector>(name, scls::Fraction(needed_point->x() * 100000.0, 100000.0), scls::Fraction(needed_point->z() * 100000.0, 100000.0)));return a_points.at(a_points.size() - 1);};
        // Returns a point by his name
        inline std::shared_ptr<Vector> point_shared_ptr(std::string point_name){for(int i = 0;i<static_cast<int>(a_points.size());i++){if(a_points[i].get()->name() == point_name){return a_points[i];}}return std::shared_ptr<Vector>();};
        inline Vector* point(std::string point_name){return point_shared_ptr(point_name).get();};

        // Handle vectors
        // Adds a vector to the graphic
        inline void add_vector(Vector needed_vector){a_vectors.push_back(std::make_shared<Vector>(needed_vector));};
        // Draws a vector on the graphic
        void draw_vector(Vector* needed_point, std::shared_ptr<scls::Image> to_return);

        // Getters and setters
        inline bool draw_base() const {return a_draw_base;};
        inline bool draw_sub_bases() const {return a_draw_sub_bases;};
        inline scls::Fraction middle_x() const {return a_graphic_base.get()->a_middle_x;};
        inline void middle_x_add(scls::Fraction value) {a_graphic_base.get()->a_middle_x += value;};
        inline scls::Fraction middle_y() const {return a_graphic_base.get()->a_middle_y;};
        inline void middle_y_add(scls::Fraction value) {a_graphic_base.get()->a_middle_y += value;};
        inline double pixel_by_case_x() const {return a_graphic_base.get()->a_pixel_by_case_x;};
        inline void pixel_by_case_x_add(double value) const {a_graphic_base.get()->a_pixel_by_case_x += value;};
        inline double pixel_by_case_y() const {return a_graphic_base.get()->a_pixel_by_case_y;};
        inline void pixel_by_case_y_add(double value) const {a_graphic_base.get()->a_pixel_by_case_y += value;};
        inline void set_draw_base(bool new_draw_base) {a_draw_base = new_draw_base;};
        inline void set_draw_sub_bases(bool new_draw_sub_bases) {a_draw_sub_bases = new_draw_sub_bases;};

    private:

        // Things to draw
        bool a_draw_base = true;
        bool a_draw_sub_bases = true;

        // Loaded function
        std::vector<std::shared_ptr<Graphic_Function>> a_functions;
        // Datas about the plane
        std::shared_ptr<__Graphic_Base> a_graphic_base = std::make_shared<__Graphic_Base>();

        // Geometrical objects
        std::vector<std::shared_ptr<Circle>> a_circles;
        // Geometrical forms 2D
        std::vector<std::shared_ptr<Form_2D>> a_forms_2d;
        // Geometrical point
        std::vector<std::shared_ptr<Vector>> a_points;
        // Geometrical vectors
        std::vector<std::shared_ptr<Vector>> a_vectors;
    };

    class Graphic_Object : public scls::GUI_Object {
        // Class representating an object containing a graphic

    public:

        //******************
        //
        // Graphic_Object handling
        //
        //******************

        // Graphic constructor
        Graphic_Object(scls::_Window_Advanced_Struct& window, std::string name, std::weak_ptr<scls::GUI_Object> parent);

        // Function called after creation
        virtual void after_creation();
        // Renders the object
        virtual void render(glm::vec3 scale_multiplier = glm::vec3(1, 1, 1));
        // Updates the object
        virtual void update_event();
        virtual void update_texture(){texture()->set_image(to_image());};

        // Adds a function to the graphic
        inline void add_function(std::shared_ptr<Function_Studied> function_studied){a_datas.add_function(function_studied);};
        // Resets the object
        inline void reset(){a_datas.reset();};
        // Returns the image of the graphic
        inline std::shared_ptr<scls::Image> to_image(){return a_datas.to_image(width_in_pixel(), height_in_pixel());};

        // Handle circles
        // Adds a circle to the graphic
        inline std::shared_ptr<Circle>* add_circle(std::string circle_name, Vector center, scls::Formula radius){return a_datas.add_circle(circle_name, center, radius);};
        // Removes circle from the graphic
        inline std::shared_ptr<Circle> remove_circle(std::string circle_name){return a_datas.remove_circle(circle_name);};

        // Handle forms
        // Adds a forms to the graphic
        inline void add_form(std::shared_ptr<Form_2D> needed_formd_2d){a_datas.add_form(needed_formd_2d);};
        // Draws a form on the graphic
        inline void draw_form(Form_2D* needed_form, std::shared_ptr<scls::Image> to_return){a_datas.draw_form(needed_form, to_return);};

        // Handle points
        // Adds a points to the graphic
        inline void add_point(std::shared_ptr<Vector> needed_point){a_datas.add_point(needed_point);};
        inline void add_point(Vector needed_point){a_datas.add_point(needed_point);};

        // Handle vectors
        // Adds a vector to the graphic
        inline void add_vector(Vector needed_vector){a_datas.add_vector(needed_vector);};
        // Draws a vector on the graphic
        inline void draw_vector(Vector* needed_point, std::shared_ptr<scls::Image> to_return){a_datas.draw_vector(needed_point, to_return);};

        // Annoying functions to draw the image
        inline int graphic_x_to_pixel_x(double x, std::shared_ptr<scls::Image>& needed_image){return a_datas.graphic_x_to_pixel_x(x, needed_image);};
        inline int graphic_y_to_pixel_y(double y, std::shared_ptr<scls::Image>& needed_image){return a_datas.graphic_y_to_pixel_y(y, needed_image);};
        inline int graphic_y_to_pixel_y_inversed(double y, std::shared_ptr<scls::Image>& needed_image){return a_datas.graphic_y_to_pixel_y_inversed(y, needed_image);};
        inline scls::Fraction pixel_x_to_graphic_x(int x, std::shared_ptr<scls::Image>& needed_image){return a_datas.pixel_x_to_graphic_x(x, needed_image);};
        inline scls::Fraction pixel_y_to_graphic_y(int y, std::shared_ptr<scls::Image>& needed_image){return a_datas.pixel_y_to_graphic_y(y, needed_image);};

        // Getters and setters
        inline Form_2D* current_form_2d() const {return a_current_form_2d.get();};
        inline bool draw_base() const {return a_datas.draw_base();};
        inline bool draw_sub_bases() const {return a_datas.draw_sub_bases();};
        inline Graphic* graphic() {return &a_datas;};
        inline scls::Fraction middle_x() const {return a_datas.middle_x();};
        inline void middle_x_add(scls::Fraction value) {a_datas.middle_x_add(value);};
        inline scls::Fraction middle_y() const {return a_datas.middle_y();};
        inline void middle_y_add(scls::Fraction value) {a_datas.middle_y_add(value);};
        inline int operation_at_click() const {return a_operation_at_click;};
        inline double pixel_by_case_x() const {return a_datas.pixel_by_case_x();};
        inline void pixel_by_case_x_add(double value) const {a_datas.pixel_by_case_x_add(value);};
        inline double pixel_by_case_y() const {return a_datas.pixel_by_case_y();};
        inline void pixel_by_case_y_add(double value) const {a_datas.pixel_by_case_y_add(value);};
        inline void set_draw_base(bool new_draw_base) {a_datas.set_draw_base(new_draw_base);};
        inline void set_draw_sub_bases(bool new_draw_sub_bases) {a_datas.set_draw_sub_bases(new_draw_sub_bases);};
        inline void set_operation_at_click(int new_operation_at_click) {a_operation_at_click = new_operation_at_click;};

    private:

        // Datas about the graphic
        Graphic a_datas;

        // Operation to do at click
        int a_operation_at_click = PLEOS_OPERATION_NOTHING;

        // Current object
        std::shared_ptr<Form_2D> a_current_form_2d;
    };
}

#endif // PLEOS_MATHEMATICS_FUNCTIONS
