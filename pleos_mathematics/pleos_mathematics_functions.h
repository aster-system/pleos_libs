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
    //*********
	//
	// The Text_Environment class
	//
	//*********

	class Text_Environment {
	    // Class representating an environment for text in PLEOS
    public:

        // Variables
        struct Variable {
            // Name of the variable
            std::string name = std::string("x");
            // Value of the variable
            scls::Fraction value = 0;
        };

        // Text_Environment constructor
        Text_Environment(){};

        // Clears the environment
        void clear(){a_unknowns.get()->clear();};

        // Handle unknowns
        // Creates a unknown
        scls::__Formula_Base::Unknown* create_unknown(std::string name){return a_unknowns.get()->create_unknown(name);};
        std::shared_ptr<scls::__Formula_Base::Unknown> create_unknown_shared_ptr(std::string name){return a_unknowns.get()->create_unknown_shared_ptr(name);};
        // Returns a value by its name
        scls::Fraction value_by_name(std::string name)const{scls::__Formula_Base::Unknown*unknow=unknown_by_name(name);if(unknow==0){return 0;}return (unknow->value.get()->value(0).real());};
        // Returns a unknown by its name
        inline scls::__Formula_Base::Unknown* unknown_by_name(std::string name)const{return a_unknowns.get()->unknown_by_name(name);};
        inline std::shared_ptr<scls::__Formula_Base::Unknown> unknown_shared_ptr_by_name(std::string name)const{return a_unknowns.get()->unknown_shared_ptr_by_name(name);};

        // Returns a formula value
        scls::__Formula_Base::Formula value_formula(std::string base)const{scls::__Formula_Base formula = scls::string_to_formula(base);return formula.replace_unknowns(a_unknowns.get());};
        // Returns a number value
        scls::Fraction value_number(std::string base)const{scls::__Formula_Base formula = scls::string_to_formula(base);return formula.value(a_unknowns.get()).real();};
        // Returns a Point_2D value
        scls::Point_2D value_point_2d(std::string base)const;

        // Getters and setters
        inline scls::__Formula_Base::Unknowns_Container* unknowns(){return a_unknowns.get();};
        inline std::shared_ptr<scls::__Formula_Base::Unknowns_Container> unknowns_shared_ptr(){return a_unknowns;};

    private:

        // Variables
        std::shared_ptr<scls::__Formula_Base::Unknowns_Container> a_unknowns = std::make_shared<scls::__Formula_Base::Unknowns_Container>();
	};

    // Possible studied type
    enum Studied_Type {ST_Sequence};

    class Function_Studied {
        // Datas about a studied function

    public:
        // Constructor of a studied function
        Function_Studied(){};

        // Returns the introduction for this function
        std::string introduction(scls::Textual_Math_Settings* settings) const;

        // Creates and returns a new function studied
        static std::shared_ptr<Function_Studied> new_function_studied_shared_ptr(scls::Formula needed_formula, Function_Studied* parent){std::shared_ptr<Function_Studied> to_return = std::make_shared<Function_Studied>();to_return.get()->a_this_object=to_return;to_return.get()->a_parent_object=parent->a_this_object;to_return.get()->set_formula(needed_formula);return to_return;};
        static std::shared_ptr<Function_Studied> new_function_studied_shared_ptr(scls::Formula needed_formula){std::shared_ptr<Function_Studied> to_return = std::make_shared<Function_Studied>();to_return.get()->a_this_object=to_return;to_return.get()->set_formula(needed_formula);return to_return;};

        // Getters and setters
        inline scls::Color color() const {return a_color;};
        inline scls::GUI_Text* connected_object()const{return a_connected_object.lock().get();};
        inline scls::Set_Number* definition_set(){return a_definition_set.get();};
        inline scls::Formula* formula()const{return a_function_segments.at(0).formula.get();};
        inline std::shared_ptr<scls::Formula> formula_copy()const{return formula()->formula_copy();};
        inline int level() const {if(a_parent_object.lock().get() == 0){return 0;}return a_parent_object.lock().get()->level() + 1;}
        inline std::string name() const {return a_function_name;};
        inline scls::Set_Number* roots(){return a_roots.get();};
        inline void set_color(scls::Color new_color){a_color = new_color;};
        inline void set_connected_object(std::weak_ptr<scls::GUI_Text> new_connected_object){a_connected_object = new_connected_object;};
        inline void set_definition_set(scls::Set_Number new_definition_set){a_definition_set=std::make_shared<scls::Set_Number>(new_definition_set);};
        inline void set_formula(scls::Formula formula){a_function_segments.at(0).formula = std::make_shared<scls::Formula>(formula);};
        inline void set_function_unknown(std::string unknown){a_function_unknown = unknown;};
        inline void set_name(std::string new_name){a_function_name = new_name;if(connected_object()!=0){connected_object()->set_text(std::string("Fonction ") + new_name);}};
        inline void set_roots(scls::Set_Number new_roots){a_roots = std::make_shared<scls::Set_Number>(new_roots);};
        inline void set_sign_set(scls::Set_Number new_sign_set){a_sign_set = std::make_shared<scls::Set_Number>(new_sign_set);};
        inline scls::Set_Number* sign_set(){return a_sign_set.get();};
        inline std::shared_ptr<Function_Studied> this_object()const{return a_this_object.lock();};
        inline std::string unknown() const {return a_function_unknown;};

    private:

        // Segmented parts
        struct Segmented_Part {std::shared_ptr<scls::Formula> formula = std::make_shared<scls::Formula>();};
        // Formulas of the function
        std::vector<Segmented_Part> a_function_segments = std::vector<Segmented_Part>(1, Segmented_Part());

        // Connected object to this vector
        std::weak_ptr<scls::GUI_Text> a_connected_object = std::weak_ptr<scls::GUI_Text>();
        // Name of the function
        std::string a_function_name = "";
        // Number of the function
        int a_function_number = 0;
        // Name of the unknown in the function
        std::string a_function_unknown = "x";
        // Type of the studied function
        Studied_Type a_type = Studied_Type::ST_Sequence;

        // Color of the curve
        scls::Color a_color = scls::Color(255, 0, 0);

        // Weak pointer to the parent of this function
        std::weak_ptr<Function_Studied> a_parent_object;
        // Weak pointer to this function
        std::weak_ptr<Function_Studied> a_this_object;

        // Studied things
        // Definition set of the function
        std::shared_ptr<scls::Set_Number> a_definition_set;
        // Roots of the function
        std::shared_ptr<scls::Set_Number> a_roots;
        // Sign (positive) of the function
        std::shared_ptr<scls::Set_Number> a_sign_set;
    };

    class Function_Table {
        // Class representating a table made for functions

    public:
        // Function_Table constructor
        Function_Table(){};

        // Add a function to study
        inline void add_function(Function_Studied* needed_function){a_studied_function.push_back(needed_function->this_object());};

        // Returns the text for the table to xml
        std::string to_xml(std::string* redaction, scls::Textual_Math_Settings* settings);

    private:
        // Studied functions
        std::vector<std::shared_ptr<Function_Studied>> a_studied_function;
    };

    // Returns the definition set of a function
    scls::Set_Number function_definition_set(Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings);
    // Returns the derivation of a function
    scls::Formula function_derivation(Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings);
    // Returns the image of a function
    scls::__Formula_Base::Formula function_image(Function_Studied* current_function, scls::Formula needed_value, std::string& redaction, scls::Textual_Math_Settings* settings);
    // Returns the limit of a function / polymonial in + infinity
    scls::Limit function_limit(Function_Studied* current_function, scls::Limit needed_limit, std::string& redaction, scls::Textual_Math_Settings* settings);
    // Returns the primitive of a function
    scls::Formula function_primitive(Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings);
    // Returns the set of roots of a function
    scls::Set_Number function_roots(Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings);
    // Returns the set of a positive function
    scls::Set_Number function_sign(Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings);
    std::string function_sign_table(Function_Studied* current_function, std::string* redaction, scls::Textual_Math_Settings* settings);

    //******************
    //
    // Sequences handling
    //
    //******************

    // Returns the interval of an sequence
    //scls::Interval sequence_variation(Function_Studied current_function, std::string& redaction);

    //******************
    //
    // Graphic handling
    //
    //******************

    // Base of the graphic
    struct __Graphic_Base {
        int a_function_number = 0;

        double a_height = -1;double a_width = -1;
        int a_height_in_pixel = -1;int a_width_in_pixel = -1;

        scls::Fraction a_middle_x =  0;
        scls::Fraction a_middle_y = 0;
        double a_pixel_by_case_x = 100;
        double a_pixel_by_case_y = 100;
    };

    class Graphic {
        // Class representating a graphic for one (or a lot) of functions
    public:

        // Base object in a graphic
        class Graphic_Base_Object : public __Graphic_Object_Base{
            // Class representating the base of an object in a graphic
        public:
            // Graphic_Base_Object constructor
            Graphic_Base_Object(std::weak_ptr<Graphic> graphic_base):__Graphic_Object_Base(),a_graphic(graphic_base){};

            // Soft reset the object
            virtual void soft_reset(){a_transform.get()->soft_reset();};

            // Annoying functions to draw the image
            int graphic_x_to_pixel_x(double x);
            int graphic_x_to_pixel_x(scls::Fraction x);
            int graphic_y_to_pixel_y(double y);
            int graphic_y_to_pixel_y(scls::Fraction y);
            int graphic_y_to_pixel_y_inversed(double y);
            int graphic_y_to_pixel_y_inversed(scls::Fraction y);
            inline double pixel_by_case_x() const {return graphic_base()->a_pixel_by_case_x;};
            inline double pixel_by_case_y() const {return graphic_base()->a_pixel_by_case_y;};
            scls::Fraction pixel_x_to_graphic_x(int x);
            scls::Fraction pixel_y_to_graphic_y(int y);

            // Datas about the object
            inline int height_in_pixel() const {return height().to_double() * graphic_base()->a_pixel_by_case_y;};
            inline int width_in_pixel() const {return width().to_double() * graphic_base()->a_pixel_by_case_x;};

            // Getters and setters
            inline Graphic* graphic() const {return a_graphic.lock().get();};
            inline __Graphic_Base* graphic_base() const {return a_graphic.lock().get()->graphic_base();};
            inline scls::Fraction height() const {return a_transform.get()->scale_y();};
            inline scls::Fraction max_x() const {return a_transform.get()->max_x();};
            inline scls::Fraction max_y() const {return a_transform.get()->max_y();};
            inline scls::Fraction min_x() const {return a_transform.get()->min_x();};
            inline scls::Fraction min_y() const {return a_transform.get()->min_y();};
            inline void set_height(scls::Fraction new_height){a_transform.get()->set_scale_y(new_height);};
            inline void set_this_object(std::weak_ptr<Graphic_Base_Object> new_this_object){a_this_object = new_this_object;};
            inline void set_width(scls::Fraction new_width){a_transform.get()->set_scale_x(new_width);};
            inline void set_x(scls::Fraction new_x){a_transform.get()->set_x(new_x);};
            inline void set_y(scls::Fraction new_y){a_transform.get()->set_y(new_y);};
            inline std::shared_ptr<scls::Transform_Object_2D> transform_shared_ptr() const {return a_transform;};
            inline scls::Fraction width() const {return a_transform.get()->scale_x();};
            inline scls::Fraction x() const {return a_transform.get()->x();};
            inline scls::Fraction y() const {return a_transform.get()->y();};

        protected:

            // Transformation of the object in the graphic
            std::shared_ptr<scls::Transform_Object_2D> a_transform = std::make_shared<scls::Transform_Object_2D>();

            // Graphic base
            std::weak_ptr<Graphic> a_graphic;
            // This object
            std::weak_ptr<Graphic_Base_Object> a_this_object;
        };

        // Needed fragment shader for the function
        static std::string graphic_function_fragment_shader(scls::Formula needed_formula);

        class Datas_Set : public Graphic_Base_Object {
            // Class representating a set of numeric datas
        public:
            // Types of datas sets
            enum Datas_Set_Type {DST_Histogram, DST_Point_Cloud, DST_Point_Cloud_Linked};
            enum Datas_Set_Display_Type {DSDT_Average, DSDT_Value};

            // Datas_Set constructor
            Datas_Set(std::weak_ptr<Graphic> graphic_base, std::string name):Graphic_Base_Object(graphic_base){};

            // Adds a data to the datas set
            inline void add_data(scls::Fraction new_data){a_datas.push_back(new_data);};

            // Returns the datas set to an image
            std::shared_ptr<scls::Image> to_image();
            std::shared_ptr<scls::Image> to_image_histogram();
            std::shared_ptr<scls::Image> to_image_point_cloud();

            // Getters and setters
            inline std::vector<scls::Fraction>& datas(){return a_datas;};
            inline Datas_Set_Display_Type display_type() const {return a_display_type;};
            inline scls::Fraction fixed_max() const {return a_fixed_max;};
            inline scls::Fraction fixed_min() const {return a_fixed_min;};
            inline void set_display_type(Datas_Set_Display_Type new_display_type){a_display_type = new_display_type;};
            inline void set_fixed_max(scls::Fraction new_fixed_max) {a_fixed_max = new_fixed_max;a_fixed_max_used = true;};
            inline void set_fixed_min(scls::Fraction new_fixed_min) {a_fixed_min = new_fixed_min;a_fixed_min_used = true;};
            inline void set_type(Datas_Set_Type new_type){a_type = new_type;};
            inline Datas_Set_Type type() const {return a_type;};
        private:
            // Datas in the datas set
            std::vector<scls::Fraction> a_datas;
            // Display type of data set
            Datas_Set_Display_Type a_display_type = Datas_Set_Display_Type::DSDT_Value;
            // Fixed values in the set
            scls::Fraction a_fixed_max = -1;bool a_fixed_max_used = false;
            scls::Fraction a_fixed_min = -1;bool a_fixed_min_used = true;
            // Type of data set
            Datas_Set_Type a_type = Datas_Set_Type::DST_Histogram;
        };

        //******************
        //
        // The Graphic_Function handling
        //
        //******************

        // Returns a new graphic
        static std::shared_ptr<Graphic> new_graphic(){std::shared_ptr<Graphic> to_return = std::shared_ptr<Graphic>(new Graphic());to_return.get()->a_this_object = to_return;return to_return;};

        class Graphic_Function {
            // Class representating a function for a graphic

        public:

            // Graphic_Function constructor
            Graphic_Function(std::shared_ptr<Function_Studied> function_studied);

            // Add a curve area
            inline void add_curve_area(scls::Fraction start, scls::Fraction end, int rectangle_number){Curve_Area to_add;to_add.area_start = start;to_add.area_end=end;to_add.rectangle_number=rectangle_number;a_curve_areas.push_back(to_add);};

            // Getters and setters
            inline scls::Color color() const {return a_function_studied.get()->color();};
            inline scls::Fraction curve_area_end(int n){return a_curve_areas.at(n).area_end;};
            inline int curve_areas_number(){return a_curve_areas.size();};
            inline scls::Fraction curve_area_start(int n){return a_curve_areas.at(n).area_start;};
            inline int curve_area_rectangle_number(int n){return a_curve_areas.at(n).rectangle_number;};
            inline scls::Set_Number* definition_set(){return a_function_studied.get()->definition_set();};
            inline scls::Formula& formula(){return *a_function_studied.get()->formula();};
            inline scls::Fraction middle_x() const {return a_graphic_base.get()->a_middle_x;};
            inline scls::Fraction middle_y() const {return a_graphic_base.get()->a_middle_y;};
            inline std::string name() const {return a_function_studied.get()->name();};
            inline double pixel_by_case_x() const {return a_graphic_base.get()->a_pixel_by_case_x;};
            inline double pixel_by_case_y() const {return a_graphic_base.get()->a_pixel_by_case_y;};
            inline void set_graphic_base(std::shared_ptr<__Graphic_Base> base) {a_graphic_base=base;};

        private:
            // Needed curves area
            struct Curve_Area{scls::Fraction area_end = 1;scls::Fraction area_start = 0;int rectangle_number = 10;};
            std::vector<Curve_Area> a_curve_areas;
            // Formula of the function
            std::shared_ptr<Function_Studied> a_function_studied;
            // Datas about the plane
            std::shared_ptr<__Graphic_Base> a_graphic_base;
        };

        // Text in a graphic
        struct Graphic_Text : public __Graphic_Object_Base{
            std::string content;std::shared_ptr<scls::Text_Style> style;scls::Fraction x = 0;scls::Fraction y = 0;

            // Moves an object to a position, from an another position
            inline void move_from_to(Vector pos_1, Vector pos_2, double proportion){Vector new_pos = pos_1 + (pos_2 - pos_1) * proportion;x = new_pos.x()->value(0).real();y = new_pos.y()->value(0).real();};
            // Returns the position of the text
            inline Vector position() const {return Vector("v", x, y);};
        };

        // Sets the middle of the base
        inline void set_middle(scls::Fraction middle_x, scls::Fraction middle_y){a_graphic_base.get()->a_middle_x = middle_x;a_graphic_base.get()->a_middle_y = middle_y;};
        // Sets the scale of the base
        inline void set_scale(double width, double height){a_graphic_base.get()->a_height = height;a_graphic_base.get()->a_width = width;};

        // Annoying functions to draw the image
        int graphic_x_to_pixel_x(double x, int needed_width);
        int graphic_x_to_pixel_x(scls::Fraction x, int needed_width);
        int graphic_x_to_pixel_x(double x, std::shared_ptr<scls::Image>& needed_image){return graphic_x_to_pixel_x(x, needed_image.get()->width());};
        int graphic_x_to_pixel_x(scls::Fraction x, std::shared_ptr<scls::Image>& needed_image){return graphic_x_to_pixel_x(x, needed_image.get()->width());};
        int graphic_y_to_pixel_y(double y, int needed_height);
        int graphic_y_to_pixel_y(scls::Fraction y, int needed_height);
        int graphic_y_to_pixel_y(double y, std::shared_ptr<scls::Image>& needed_image){return graphic_y_to_pixel_y(y, needed_image.get()->height());};
        int graphic_y_to_pixel_y(scls::Fraction y, std::shared_ptr<scls::Image>& needed_image){return graphic_y_to_pixel_y(y, needed_image.get()->height());};
        int graphic_y_to_pixel_y_inversed(double y, int needed_height);
        int graphic_y_to_pixel_y_inversed(scls::Fraction y, int needed_height);
        int graphic_y_to_pixel_y_inversed(double y, std::shared_ptr<scls::Image>& needed_image){return graphic_y_to_pixel_y_inversed(y, needed_image.get()->height());};
        int graphic_y_to_pixel_y_inversed(scls::Fraction y, std::shared_ptr<scls::Image>& needed_image){return graphic_y_to_pixel_y_inversed(y, needed_image.get()->height());};
        scls::Fraction pixel_x_to_graphic_x(int x, int needed_width);
        scls::Fraction pixel_x_to_graphic_x(int x, std::shared_ptr<scls::Image>& needed_image){return pixel_x_to_graphic_x(x, needed_image.get()->width());};
        scls::Fraction pixel_y_to_graphic_y(int y, int needed_height);
        scls::Fraction pixel_y_to_graphic_y(int y, std::shared_ptr<scls::Image>& needed_image){return pixel_y_to_graphic_y(y, needed_image.get()->height());};
        // Draw the bases of the image
        void image_draw_base(std::shared_ptr<scls::Image> to_return);
        // Draw a function on the image
        void image_draw_function(std::shared_ptr<scls::Image> to_return, std::shared_ptr<Graphic_Function> needed_function, std::vector<scls::Fraction>& screen_pos);

        // Adds a function to the graphic
        std::shared_ptr<Graphic_Function> add_function(std::shared_ptr<Function_Studied> function_studied);
        // Resets the object
        inline void reset(){a_circles.clear();a_forms_2d.clear();a_functions.clear();a_physic_objects.clear();a_physic_map.clear();a_points.clear();a_vectors.clear();a_physic_map_start_x=0;a_physic_map_start_y=0;set_draw_base(true);set_draw_sub_bases(true);};
        // Returns the image of the graphic
        std::shared_ptr<scls::Image> to_image(int width_in_pixel, int height_in_pixel);

        // Handle circles
        // Adds a circle to the graphic
        inline void add_circle(std::shared_ptr<Circle> circle_to_add){a_circles.push_back(circle_to_add);};
        inline std::shared_ptr<Circle>* add_circle(std::string circle_name, Vector center, scls::__Formula_Base radius){a_circles.push_back(std::make_shared<Circle>(circle_name, center, radius));a_circles.at(a_circles.size()-1).get()->set_unknowns(a_unknowns);a_circles.at(a_circles.size()-1).get()->attached_transform()->set_unknowns(a_unknowns);return &a_circles[a_circles.size() - 1];};
        // Returns a circle by its name
        inline Circle* circle_by_name(std::string name){for(int i = 0;i<static_cast<int>(a_circles.size());i++){if(a_circles.at(i).get()->name() == name){return a_circles.at(i).get();}}return 0;};
        // Creates a new circle in the graphic
        inline std::shared_ptr<Circle> new_circle(std::string circle_name, scls::Fraction x, scls::Fraction y, scls::Fraction radius){return *add_circle(circle_name, Vector(x, y), radius);};
        template <typename T> std::shared_ptr<T> new_circle(std::string circle_name, Vector center, scls::__Formula_Base radius){std::shared_ptr<T>created_circle=std::make_shared<T>(circle_name, center, radius);created_circle.get()->set_unknowns(a_unknowns);add_circle(created_circle);created_circle.get()->attached_transform()->set_unknowns(a_unknowns);return created_circle;};
        template <typename T> std::shared_ptr<T> new_circle(std::string circle_name, scls::Fraction x, scls::Fraction y, scls::__Formula_Base radius){return new_circle<T>(circle_name, Vector(x, y), radius);};
        // Removes circle from the graphic
        inline std::shared_ptr<Circle> remove_circle(std::string circle_name){for(int i = 0;i<static_cast<int>(a_circles.size());i++){if(a_circles[i].get()->name()==circle_name){std::shared_ptr<Circle> temp = a_circles[i];a_circles.erase(a_circles.begin() + i);return temp;} }return std::shared_ptr<Circle>();};

        // Handle datas set
        // Adds a datas set to the graphic
        inline void add_datas_set(std::shared_ptr<Datas_Set> needed_datas_set){a_datas_sets.push_back(needed_datas_set);};
        // Draws a datas set on the graphic
        void draw_datas_set(Datas_Set* needed_datas_set, std::shared_ptr<scls::Image> to_return);
        // Creates and returns a datas set
        inline std::shared_ptr<Datas_Set> new_datas_set(std::string name){std::shared_ptr<Datas_Set>to_return=std::make_shared<Datas_Set>(a_this_object, name);add_datas_set(to_return);return to_return;};

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
        inline std::shared_ptr<Form_2D> new_form(std::string name, std::string points){std::shared_ptr<Form_2D>needed_form=new_form(name);set_form_points(needed_form.get(), points);return needed_form;};
        // Creates and returns a form from a face
        std::shared_ptr<Form_2D> new_form_from_face(std::string name, scls::model_maker::Face* face);
        // Sets the points in a form of the graphic
        void set_form_points(Form_2D* needed_form, std::string points){std::vector<std::string> cutted = scls::cut_string(points, ";");for(int i = 0;i<static_cast<int>(cutted.size());i++){needed_form->add_point(point_shared_ptr(cutted[i]));}};
        void set_form_points(std::shared_ptr<Form_2D> needed_form, std::string points){set_form_points(needed_form.get(), points);};

        // Creates and returns a form (and its point)
        std::shared_ptr<Form_2D> new_form(std::string name, scls::Fraction x_1, scls::Fraction y_1, scls::Fraction x_2, scls::Fraction y_2, scls::Fraction x_3, scls::Fraction y_3, scls::Fraction x_4, scls::Fraction y_4);
        // Creates and returns a square (and its point)
        std::shared_ptr<Form_2D> new_square(std::string name, scls::Fraction x, scls::Fraction y, scls::Fraction width, scls::Fraction height);
        // Creates and returns a triangle (and its point)
        std::shared_ptr<Form_2D> new_triangle(std::string name, scls::Fraction x_1, scls::Fraction y_1, scls::Fraction x_2, scls::Fraction y_2, scls::Fraction x_3, scls::Fraction y_3);

        // Handle lines
        // Draw a line between two points
        void draw_line(Vector* point_1, Vector* point_2, scls::Color color, double width, double proportion, std::shared_ptr<scls::Image> to_return);
        // Creates and returns a line (and its points)
        std::shared_ptr<Form_2D> new_line(std::string name, scls::Fraction x_1, scls::Fraction y_1, scls::Fraction x_2, scls::Fraction y_2);

        // Handle points
        // Adds a points to the graphic
        inline void add_point(std::shared_ptr<Vector> needed_point){a_points.push_back(needed_point);};
        inline void add_point(Vector needed_point){add_point(std::make_shared<Vector>(needed_point));};
        inline std::shared_ptr<Vector> add_point(std::string name, scls::model_maker::Point* needed_point){add_point(std::make_shared<Vector>(name, scls::Fraction(needed_point->x() * 100000.0, 100000.0), scls::Fraction(needed_point->z() * 100000.0, 100000.0)));return a_points.at(a_points.size() - 1);};
        // Returns a point by his name
        inline std::shared_ptr<Vector> point_shared_ptr(std::string point_name){for(int i = 0;i<static_cast<int>(a_points.size());i++){if(a_points[i].get()->name() == point_name){return a_points[i];}}return std::shared_ptr<Vector>();};
        inline Vector* point(std::string point_name){return point_shared_ptr(point_name).get();};
        // Creates and returns a new point in the graphic
        inline std::shared_ptr<Vector> new_point(std::string name, scls::Fraction x, scls::Fraction y){std::shared_ptr<Vector>needed=std::make_shared<Vector>(name, x, y);needed.get()->set_type(Vector_Type::VT_Point);add_point(needed);return needed;};
        inline std::shared_ptr<Vector> new_point(std::string name, scls::Point_2D needed_point){return new_point(name, needed_point.x(), needed_point.y());};

        // Handle texts
        // Creates and returns a new text in the graphic
        inline Graphic_Text* new_text(std::string content, scls::Fraction x, scls::Fraction y, std::shared_ptr<scls::Text_Style> style){a_texts.push_back(std::make_shared<Graphic_Text>());Graphic_Text* current_text=a_texts[a_texts.size() - 1].get();current_text->content = content;current_text->style = style;current_text->x=x;current_text->y=y;return current_text;};
        inline Graphic_Text* new_text(std::string content, Vector position, std::shared_ptr<scls::Text_Style> style){return new_text(content, position.x()->value(0).real(), position.y()->value(0).real(), style);};

        // Handle vectors
        // Adds a vector to the graphic
        inline void add_vector(Vector needed_vector){a_vectors.push_back(std::make_shared<Vector>(needed_vector));};
        // Draws a vector on the graphic
        void draw_vector(Vector* needed_point, std::shared_ptr<scls::Image> to_return);

        // Getters and setters
        inline std::vector<std::shared_ptr<Circle>>& circles(){return a_circles;};
        inline bool draw_base() const {return a_draw_base;};
        inline bool draw_sub_bases() const {return a_draw_sub_bases;};
        inline __Graphic_Base* graphic_base() const {return a_graphic_base.get();};
        inline std::shared_ptr<__Graphic_Base> graphic_base_shared_ptr() const {return a_graphic_base;};
        inline scls::Fraction height() const {return a_graphic_base.get()->a_height;};
        inline scls::Fraction middle_x() const {return a_graphic_base.get()->a_middle_x;};
        inline void middle_x_add(scls::Fraction value) {a_graphic_base.get()->a_middle_x += value;};
        inline scls::Fraction middle_y() const {return a_graphic_base.get()->a_middle_y;};
        inline void middle_y_add(scls::Fraction value) {a_graphic_base.get()->a_middle_y += value;};
        inline double pixel_by_case_x() const {return a_graphic_base.get()->a_pixel_by_case_x;};
        inline void pixel_by_case_x_add(double value) const {a_graphic_base.get()->a_pixel_by_case_x += value;};
        inline double pixel_by_case_y() const {return a_graphic_base.get()->a_pixel_by_case_y;};
        inline void pixel_by_case_y_add(double value) const {a_graphic_base.get()->a_pixel_by_case_y += value;};
        inline std::vector<std::shared_ptr<Vector>>& points(){return a_points;};
        inline void set_background_color(scls::Color new_background_color){a_style.get()->set_background_color(new_background_color);};
        inline void set_draw_base(bool new_draw_base) {a_draw_base = new_draw_base;};
        inline void set_draw_sub_bases(bool new_draw_sub_bases) {a_draw_sub_bases = new_draw_sub_bases;};
        inline void set_style(scls::Text_Style new_style) {a_style = std::make_shared<scls::Text_Style>(new_style);};
        inline void set_unknowns(std::shared_ptr<scls::__Formula_Base::Unknowns_Container> new_unknowns){a_unknowns = new_unknowns;};
        inline scls::Text_Style* style() const {return a_style.get();};
        inline std::vector<std::shared_ptr<Graphic_Text>>& texts(){return a_texts;};
        inline std::vector<std::shared_ptr<Vector>>& vectors(){return a_vectors;};
        inline scls::Fraction width() const {return a_graphic_base.get()->a_width;};

        //******************
        //
        // Loading handling
        //
        //******************

        // Balises in the graphic
        virtual void graphic_from_xml_balise(std::shared_ptr<scls::XML_Text> xml, Text_Environment& environment, std::shared_ptr<scls::Text_Style> text_style);

        //******************
        //
        // Physic handling
        //
        //******************

        // Physic in a graphic object
        class Graphic_Physic {
            // Class representating a physic handler in a graphic object
        public:

            // Physic case
            struct Physic_Case{
                // Static objects in the case
                inline void delete_static_object_collision(Graphic_Collision* object){for(int i=0;i<static_cast<int>(static_objects_collisions.size());i++){if(static_objects_collisions[i].lock().get()==object){static_objects_collisions.erase(static_objects_collisions.begin() + i);static_objects_collisions_physic.erase(static_objects_collisions_physic.begin() + i);break;}}};
                std::vector<std::weak_ptr<Graphic_Collision>> static_objects_collisions;
                std::vector<std::weak_ptr<Graphic_Physic>> static_objects_collisions_physic;
            };

            // Graphic_Physic constructor
            Graphic_Physic(std::weak_ptr<__Graphic_Object_Base> attached_object, std::weak_ptr<scls::Transform_Object_2D> attached_transform):a_attached_object(attached_object),a_attached_transform(attached_transform){};
            Graphic_Physic(std::weak_ptr<Graphic_Base_Object> attached_object):Graphic_Physic(attached_object, attached_object.lock().get()->transform_shared_ptr()){};

            // Softs reset the object
            virtual void soft_reset();

            // Add a line / rect collision to the graphic object
            void add_collision(std::shared_ptr<Graphic_Collision> collision);
            void add_collision(scls::Fraction x_1, scls::Fraction y_1, scls::Fraction x_2, scls::Fraction y_2);
            // Checks if a collision occurs with an another collision
            void check_collision(Graphic_Collision* collision, Graphic::Graphic_Physic* other_object);
            // Returns a new a collision to the graphic object
            std::shared_ptr<Graphic_Collision> new_collision(Graphic_Collision_Type type);
            std::shared_ptr<Graphic_Collision> new_collision(){return new_collision(Graphic_Collision_Type::GCT_Rect);};

            // Accelerates the object
            inline void accelerate(scls::Point_2D acceleration){a_attached_transform.lock().get()->accelerate(acceleration);};
            inline void accelerate_x(scls::Fraction acceleration){a_attached_transform.lock().get()->accelerate_x(acceleration);};
            inline void accelerate_y(scls::Fraction acceleration){a_attached_transform.lock().get()->accelerate_y(acceleration);};
            // Next movement generated by the velocity
            inline scls::Fraction next_movement_x()const{return a_attached_transform.lock().get()->next_movement_x();};
            inline scls::Fraction next_movement_y()const{return a_attached_transform.lock().get()->next_movement_y();};
            // Remove the X / Y velocity
            inline void remove_x_velocity(){a_attached_transform.lock().get()->set_velocity_x(0);};
            inline void remove_y_velocity(){a_attached_transform.lock().get()->set_velocity_y(0);};
            // Updates raw velocity
            inline void update_raw_velocity(){a_attached_transform.lock().get()->update_raw_velocity();};

            // Moves the object
            void __move(scls::Point_2D point){scls::Transform_Object_2D* t=a_attached_transform.lock().get();t->set_x(t->x() + point.x());t->set_y(t->y() + point.y());};
            void __move(){__move(scls::Point_2D(next_movement_x(), next_movement_y()));};

            // Precise next movement
            inline scls::Fraction max_x_next() const {return attached_transform()->max_x_next();};
            inline scls::Fraction max_y_next() const {return attached_transform()->max_y_next();};
            inline scls::Fraction min_x_next() const {return attached_transform()->min_x_next();};
            inline scls::Fraction min_y_next() const {return attached_transform()->min_y_next();};
            inline scls::Point_2D position_next() const {return attached_transform()->position_next();};

            // Calculates the point of the trajectory of the function
            std::vector<scls::Point_2D> trajectory_points(int point_number, scls::Fraction time_separation);

            // Getters and setters
            inline __Graphic_Object_Base* attached_object()const{return a_attached_object.lock().get();};
            inline scls::Transform_Object_2D* attached_transform()const{return a_attached_transform.lock().get();};
            inline std::vector<std::shared_ptr<Graphic_Collision>>& collisions(){return a_collisions;};
            inline std::vector<std::shared_ptr<Graphic_Collision::Collision>>& current_collisions_results(){return a_current_collisions_results;};
            inline bool is_static() const {return a_static;};
            inline bool loaded_in_map() const {return a_loaded_in_map;};
            inline bool moved_during_this_frame() const {return attached_transform()->moved_during_this_frame();};
            inline scls::Point_2D position() const {return a_attached_transform.lock().get()->position();};
            inline scls::Point_2D position(int normalize) const {return a_attached_transform.lock().get()->position(normalize);};
            inline scls::Point_2D raw_velocity() const {return a_attached_transform.lock().get()->raw_velocity();};
            inline scls::Fraction raw_velocity_x() {return raw_velocity().x();};
            inline scls::Fraction raw_velocity_y() {return raw_velocity().y();};
            inline void set_delta_time(scls::Fraction new_delta_time){a_delta_time = new_delta_time;attached_transform()->set_delta_time(new_delta_time);for(int i = 0;i<static_cast<int>(a_collisions.size());i++){a_collisions.at(i).get()->attached_transform()->set_delta_time(new_delta_time);}};
            inline void set_loaded_in_map(bool new_loaded_map){a_loaded_in_map = new_loaded_map;};
            inline void set_static(bool new_static) {a_static = new_static;}
            inline void set_use_gravity(bool new_use_gravity){a_use_gravity = new_use_gravity;};
            inline void set_velocity(scls::Point_2D new_velocity){attached_transform()->set_velocity(new_velocity);};
            inline void set_velocity_x(scls::Fraction new_velocity_x, bool with_delta_time){if(with_delta_time){a_attached_transform.lock().get()->set_velocity_x(new_velocity_x / a_delta_time);}else{a_attached_transform.lock().get()->set_velocity_x(new_velocity_x);}};
            inline void set_velocity_x(scls::Fraction new_velocity_x){set_velocity_x(new_velocity_x, false);};
            inline void set_velocity_y(scls::Fraction new_velocity_y, bool with_delta_time){if(with_delta_time){a_attached_transform.lock().get()->set_velocity_y(new_velocity_y / a_delta_time);}else{a_attached_transform.lock().get()->set_velocity_y(new_velocity_y);}};
            inline void set_velocity_y(scls::Fraction new_velocity_y){set_velocity_y(new_velocity_y, false);};
            inline bool use_gravity() const {return a_use_gravity;};
            inline std::vector<Physic_Case*>& used_physic_case(){return a_used_physic_case;};
            inline scls::Point_2D velocity(int normalize) const {return a_attached_transform.lock().get()->velocity(normalize);};
            inline scls::Point_2D velocity() const {return a_attached_transform.lock().get()->velocity();};
            inline scls::Fraction velocity_x() {return velocity().x();};
            inline scls::Fraction velocity_y() {return velocity().y();};
        private:
            // Attached object
            std::weak_ptr<__Graphic_Object_Base> a_attached_object;
            std::weak_ptr<scls::Transform_Object_2D> a_attached_transform;
            // Collisions in the physic object
            std::vector<std::shared_ptr<Graphic_Collision>> a_collisions;
            // Current collision of the object
            std::vector<std::shared_ptr<Graphic_Collision::Collision>> a_current_collisions_results;

            // Delta time of the object
            scls::Fraction a_delta_time = scls::Fraction(1, 100);
            // If the object is loaded in the map
            bool a_loaded_in_map = false;
            // If the object is static or not
            bool a_static = true;
            // If the object use gravity or not
            bool a_use_gravity = true;
            // Used physic cases
            std::vector<Physic_Case*> a_used_physic_case;
        };
        typedef Graphic_Physic::Physic_Case Physic_Case;

        // Adds a physic object
        void add_physic_object(std::shared_ptr<Graphic_Physic> new_object){a_physic_objects.push_back(new_object);}
        // Loads 100 X 100 physic map
        void load_physic_map(int middle_loading_x, int middle_loading_y);
        // Returns a physic case by its coordinates
        inline Physic_Case* physic_case(int x, int y){return a_physic_map[(-a_physic_map_start_x) + x][(-a_physic_map_start_y) + y].get();};

        // Getters and setters
        inline std::vector<std::vector<std::shared_ptr<Physic_Case>>>& physic_map(){return a_physic_map;};
        inline std::vector<std::shared_ptr<Graphic_Physic>>& physic_objects(){return a_physic_objects;};

    protected:

        // Graphic constructor
        Graphic(){a_style.get()->set_background_color(scls::Color(0, 0, 0, 0));};

        // This object
        std::weak_ptr<Graphic> a_this_object;

    private:

        // Things to draw
        bool a_draw_base = true;
        bool a_draw_sub_bases = true;
        // Style of the graphic
        std::shared_ptr<scls::Text_Style> a_style = std::make_shared<scls::Text_Style>();
        // Unknowns in the graphic
        std::shared_ptr<scls::__Formula_Base::Unknowns_Container> a_unknowns;

        // Loaded function
        std::vector<std::shared_ptr<Graphic_Function>> a_functions;
        // Datas about the plane
        std::shared_ptr<__Graphic_Base> a_graphic_base = std::make_shared<__Graphic_Base>();

        // Geometrical objects
        std::vector<std::shared_ptr<Circle>> a_circles;
        // Set of datas
        std::vector<std::shared_ptr<Datas_Set>> a_datas_sets;
        // Geometrical forms 2D
        std::vector<std::shared_ptr<Form_2D>> a_forms_2d;
        // Geometrical point
        std::vector<std::shared_ptr<Vector>> a_points;
        // Text
        std::vector<std::shared_ptr<Graphic_Text>> a_texts;
        // Geometrical vectors
        std::vector<std::shared_ptr<Vector>> a_vectors;

        //******************
        //
        // Physic handling
        //
        //******************

        // Physic map
        std::vector<std::vector<std::shared_ptr<Physic_Case>>> a_physic_map;
        int a_physic_map_start_x = 0;int a_physic_map_start_y = 0;

        // Physic objects
        std::vector<std::shared_ptr<Graphic_Physic>> a_physic_objects;
    };

    class Graphic_Object : public scls::GUI_Object {
        // Class representating an object containing a graphic
    public:

        class Graphic_GUI_Object : public Graphic::Graphic_Base_Object {
            // GUI object in a graphic
        public:
            // Graphic_GUI_Object constructor
            Graphic_GUI_Object(std::weak_ptr<Graphic> graphic_base, std::shared_ptr<scls::GUI_Object>needed_object,Graphic_Object*attached_graphic):Graphic::Graphic_Base_Object(graphic_base),a_attached_graphic(attached_graphic),a_object(needed_object){needed_object.get()->set_ignore_click(true);needed_object.get()->set_texture_alignment(scls::T_Fill);};

            // Soft resets the object
            virtual void soft_reset(){Graphic::Graphic_Base_Object::soft_reset();object()->soft_reset();};
            virtual void soft_reset_physic(){if(a_physic_object.get()!=0){a_physic_object.get()->soft_reset();}};
            // Updates the object
            virtual void update(){object()->update();};
            virtual void update_event(){object()->update_event();};

            // Move the GUI object
            void move_x(double new_velocity){physic_object()->set_velocity_x(new_velocity);};
            // Scale the GUI Object
            void scale(Graphic* graphic, int image_width, int image_height);

            // Sets the physic object
            inline void set_physic_object(bool is_static){a_physic_object=std::make_shared<Graphic::Graphic_Physic>(a_this_object);a_physic_object.get()->set_static(is_static);a_physic_object.get()->set_use_gravity(!is_static);graphic()->add_physic_object(a_physic_object);};

            // Getters and setters
            inline scls::GUI_Object* object() const {return a_object.get();};
            inline Graphic::Graphic_Physic* physic_object() const {return a_physic_object.get();};
            inline scls::_Window_Advanced_Struct* window_struct()const{return &a_object.get()->window_struct();};
        private:
            // Attached graphic
            const Graphic_Object* a_attached_graphic;
            // GUI object
            std::shared_ptr<scls::GUI_Object> a_object;

            //******************
            //
            // Physic handling
            //
            //******************

            // Physic object of this object
            std::shared_ptr<Graphic::Graphic_Physic> a_physic_object;
        };

        //******************
        //
        // Graphic_Object handling
        //
        //******************

        // Graphic constructor
        Graphic_Object(scls::_Window_Advanced_Struct& window, std::string name, std::weak_ptr<scls::GUI_Object> parent);

        // Function called after resizing the window
        virtual void after_resizing(){update_texture();};
        // Renders the object
        virtual void render(glm::vec3 scale_multiplier = glm::vec3(1, 1, 1));
        // Soft reset the object
        virtual void soft_reset(){a_created_vectors_at_click.clear();for(int i = 0;i<static_cast<int>(a_gui_objects.size());i++){a_gui_objects.at(i).get()->soft_reset();}};
        // Updates the object
        virtual void update();
        virtual void update_event();
        virtual void update_texture();

        // Adds a function to the graphic
        inline void add_function(std::shared_ptr<Function_Studied> function_studied){a_datas.get()->add_function(function_studied);};
        // Resets the object
        inline void reset(){a_datas.get()->reset();};
        // Returns the image of the graphic
        inline std::shared_ptr<scls::Image> to_image(){return a_datas.get()->to_image(width_in_pixel(), height_in_pixel());};
        inline bool use_image() const {return (a_datas.get()->draw_base() && a_datas.get()->draw_sub_bases()) || (a_datas.get()->circles().size() > 0 || a_datas.get()->texts().size() > 0 || a_datas.get()->vectors().size() > 0);};

        // Handle circles
        // Adds a circle to the graphic
        inline std::shared_ptr<Circle>* add_circle(std::string circle_name, Vector center, scls::Fraction radius){return a_datas.get()->add_circle(circle_name, center, radius);};
        // Removes circle from the graphic
        inline std::shared_ptr<Circle> remove_circle(std::string circle_name){return a_datas.get()->remove_circle(circle_name);};

        // Handle forms
        // Adds a forms to the graphic
        inline void add_form(std::shared_ptr<Form_2D> needed_formd_2d){a_datas.get()->add_form(needed_formd_2d);};
        // Draws a form on the graphic
        inline void draw_form(Form_2D* needed_form, std::shared_ptr<scls::Image> to_return){a_datas.get()->draw_form(needed_form, to_return);};

        // Handle points
        // Adds a points to the graphic
        inline void add_point(std::shared_ptr<Vector> needed_point){a_datas.get()->add_point(needed_point);};
        inline void add_point(Vector needed_point){a_datas.get()->add_point(needed_point);};
        // Creates and returns a new point in the graphic
        inline std::shared_ptr<Vector> new_point(std::string name, scls::Point_2D needed_point){return a_datas.get()->new_point(name, needed_point.x(), needed_point.y());};

        // Handle vectors
        // Adds a vector to the graphic
        inline void add_vector(Vector needed_vector){a_datas.get()->add_vector(needed_vector);};
        inline void add_vector(scls::Fraction x, scls::Fraction y){a_datas.get()->add_vector(Vector(std::string(), x, y));};
        // Draws a vector on the graphic
        inline void draw_vector(Vector* needed_point, std::shared_ptr<scls::Image> to_return){a_datas.get()->draw_vector(needed_point, to_return);};

        // Creates and returns a square (and its point)
        inline std::shared_ptr<Form_2D> new_square(std::string name, scls::Fraction x, scls::Fraction y, scls::Fraction width, scls::Fraction height){return a_datas.get()->new_square(name, x, y, width, height);};

        // Handle other object
        // Adds a new GUI Object
        inline void add_other_object(std::shared_ptr<Graphic_GUI_Object> object){object.get()->set_this_object(object);a_gui_objects.push_back(object);};
        // Creates a new GUI object
        template <typename T = Graphic_GUI_Object, typename G = scls::GUI_Object> std::shared_ptr<T> new_other_object(std::string other_name){std::shared_ptr<scls::GUI_Object>to_return=*new_object<G>(other_name);std::shared_ptr<T>object=std::make_shared<T>(a_datas,to_return,this);add_other_object(object);return object;};

        // Annoying functions to draw the image
        inline int graphic_x_to_pixel_x(double x, int needed_width){return a_datas.get()->graphic_x_to_pixel_x(x, needed_width);};
        inline int graphic_x_to_pixel_x(double x, std::shared_ptr<scls::Image>& needed_image){return a_datas.get()->graphic_x_to_pixel_x(x, needed_image);};
        inline int graphic_y_to_pixel_y(double y, int needed_height){return a_datas.get()->graphic_y_to_pixel_y(y, needed_height);};
        inline int graphic_y_to_pixel_y(double y, std::shared_ptr<scls::Image>& needed_image){return a_datas.get()->graphic_y_to_pixel_y(y, needed_image);};
        inline int graphic_y_to_pixel_y_inversed(double y, int needed_height){return a_datas.get()->graphic_y_to_pixel_y_inversed(y, needed_height);};
        inline int graphic_y_to_pixel_y_inversed(double y, std::shared_ptr<scls::Image>& needed_image){return a_datas.get()->graphic_y_to_pixel_y_inversed(y, needed_image);};
        inline scls::Fraction pixel_x_to_graphic_x(int x, int needed_width){return a_datas.get()->pixel_x_to_graphic_x(x, needed_width);};
        inline scls::Fraction pixel_x_to_graphic_x(int x, std::shared_ptr<scls::Image>& needed_image){return a_datas.get()->pixel_x_to_graphic_x(x, needed_image);};
        inline scls::Fraction pixel_y_to_graphic_y(int y, int needed_height){return a_datas.get()->pixel_y_to_graphic_y(y, needed_height);};
        inline scls::Fraction pixel_y_to_graphic_y(int y, std::shared_ptr<scls::Image>& needed_image){return a_datas.get()->pixel_y_to_graphic_y(y, needed_image);};

        // Create a new graphic
        template <typename T> void set_datas(){a_datas = T::new_graphic();};

        // Getters and setters
        inline std::vector<std::shared_ptr<Vector>>& created_vectors_at_click(){return a_created_vectors_at_click;};
        inline Form_2D* current_form_2d() const {return a_current_form_2d.get();};
        inline bool draw_base() const {return a_datas.get()->draw_base();};
        inline bool draw_sub_bases() const {return a_datas.get()->draw_sub_bases();};
        inline Graphic* graphic() {return a_datas.get();};
        inline std::vector<std::shared_ptr<Graphic_GUI_Object>>& gui_objects(){return a_gui_objects;};
        inline scls::Fraction middle_x() const {return a_datas.get()->middle_x();};
        inline void middle_x_add(scls::Fraction value) {a_datas.get()->middle_x_add(value);};
        inline scls::Fraction middle_y() const {return a_datas.get()->middle_y();};
        inline void middle_y_add(scls::Fraction value) {a_datas.get()->middle_y_add(value);};
        inline int operation_at_click() const {return a_operation_at_click;};
        inline double pixel_by_case_x() const {return a_datas.get()->pixel_by_case_x();};
        inline void pixel_by_case_x_add(double value) const {a_datas.get()->pixel_by_case_x_add(value);};
        inline double pixel_by_case_y() const {return a_datas.get()->pixel_by_case_y();};
        inline void pixel_by_case_y_add(double value) const {a_datas.get()->pixel_by_case_y_add(value);};
        inline std::vector<std::shared_ptr<Vector>>& points(){return a_datas.get()->points();};
        inline void set_draw_base(bool new_draw_base) {a_datas.get()->set_draw_base(new_draw_base);};
        inline void set_draw_sub_bases(bool new_draw_sub_bases) {a_datas.get()->set_draw_sub_bases(new_draw_sub_bases);};
        inline void set_operation_at_click(int new_operation_at_click) {a_operation_at_click = new_operation_at_click;};
        inline void set_update_physic_with_events(bool new_update_physic_with_events){a_update_physic_with_events = new_update_physic_with_events;};
        inline bool update_physic_with_events() const {return a_update_physic_with_events;};
        inline std::vector<std::shared_ptr<Vector>>& vectors(){return a_datas.get()->vectors();};

        //******************
        //
        // Physic handling
        //
        //******************

        // Loads 100 X 100 physic map
        inline void load_physic_map(int middle_loading_x, int middle_loading_y){a_datas.get()->load_physic_map(middle_loading_x, middle_loading_y);};
        // Returns a physic case by its coordinates
        inline Graphic::Physic_Case* physic_case(int x, int y){return a_datas.get()->physic_case(x, y);};
        // Updates the physic
        int update_physic(double multiplier);

    private:

        // Datas about the graphic
        std::shared_ptr<Graphic> a_datas = Graphic::new_graphic();
        // Other object in the graphic
        std::vector<std::shared_ptr<Graphic_GUI_Object>> a_gui_objects;
        // If the physic should be updated with the events
        bool a_update_physic_with_events = true;

        // Created objects at click
        std::vector<std::shared_ptr<Vector>> a_created_vectors_at_click;
        // Operation to do at click
        int a_operation_at_click = PLEOS_OPERATION_NOTHING;

        // Current object
        std::shared_ptr<Form_2D> a_current_form_2d;
    };
}

#endif // PLEOS_MATHEMATICS_FUNCTIONS
