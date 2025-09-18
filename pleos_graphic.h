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
// This file contains the graphic part of the PLEOS files.
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

#ifndef PLEOS_GRAPHIC
#define PLEOS_GRAPHIC

// Include SCLS Graphic Benoit
#include "pleos_mathematics.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {
    //******************
    //
    // Graphic handling
    //
    //******************

    extern int __line_number;
    class Graphic {
        // Class representating a graphic for one (or a lot) of functions
    public:

        //******************
        //
        // The Graphic_Base_Object class
        //
        //******************

        // Base object in a graphic
        class Graphic_Base_Object : public __Graphic_Object_Base{
            // Class representating the base of an object in a graphic
        public:
            // Graphic_Base_Object constructor
            Graphic_Base_Object(std::weak_ptr<Graphic> graphic_base):__Graphic_Object_Base(graphic_base.lock().get()->graphic_base_shared_ptr()),a_graphic(graphic_base){};

            // Deletes the physic object
            void delete_physic_object();

            // Datas about the object
            inline int height_in_pixel() const {return (absolute_height() * pixel_by_case_y());};
            inline int width_in_pixel() const {return (absolute_width() * pixel_by_case_x());};

            // Getters and setters
            inline pleos::Text_Environment* environment() const {return graphic_base()->a_environment.get();};
            inline Graphic* graphic() const {return a_graphic.lock().get();};
            inline scls::_Window_Advanced_Struct* window_struct() const {return graphic()->window_struct();};

            //******************
            // Hierarchy functions
            //******************

            // Enables / disables physic
            virtual void disable_physic(){};
            virtual void enable_physic(){};

            // Updates the actions of the object
            virtual bool update_action(double used_delta_time, __Graphic_Object_Base::Action* action, int& deleted_objects);
        protected:

            // Graphic base
            std::weak_ptr<Graphic> a_graphic;
        };

        // Graphic object with a texture
        class Graphic_Texture_Object : public Graphic_Base_Object{
            // Class representating a graphic object with a texture
        public:
            // Graphic_Base_Object constructor
            Graphic_Texture_Object(std::weak_ptr<Graphic> graphic_base):Graphic_Base_Object(graphic_base){};

            // Draws the object on an image
            virtual void draw_on_image(std::shared_ptr<scls::__Image_Base>);

            // Loads the last texture of the object
            void load_last_texture();

            // Coordinates datas
            int graphic_x_to_texture_x(scls::Fraction x);
            int graphic_y_to_texture_y(scls::Fraction y);

            // Returns the source to a XML text
            virtual std::string to_displayed_text();
            virtual std::string to_xml_text();
            virtual std::string to_xml_text_base();
            virtual std::string to_xml_text_object_name();
            std::string to_xml_text_source();

            // Getters and setters
            inline scls::__Image_Base* last_texture() const {return a_last_texture.get();};
            inline void set_source(std::string new_source) {a_source = new_source;}
            inline void set_texture(scls::Image new_texture){set_texture(new_texture.image_shared_ptr());};
            inline void set_texture(std::shared_ptr<scls::__Image_Base> new_texture){a_texture=new_texture;a_source=std::string();};
            inline void set_texture(std::string path){if(path != std::string()){set_texture(std::make_shared<scls::__Image_Base>(path));}a_source=path;};
            inline std::string source() const {return a_source;}
            inline scls::__Image_Base* texture() const {return a_texture.get();}
            inline std::shared_ptr<scls::__Image_Base> texture_shared_ptr() const {return a_texture;}

        private:
            //If the texture is drew on the image or not
            bool a_drew_on_image = true;
            // Source of the texture of the object
            std::string a_source = std::string();
            // Texture of the object
            std::shared_ptr<scls::__Image_Base> a_last_texture;
            int a_last_texture_height = -1;int a_last_texture_width = -1;
            std::shared_ptr<scls::__Image_Base> a_texture;
        }; typedef Graphic_Texture_Object Graphic_Texture;
        class Graphic_Texture_Table : public Graphic_Texture_Object{
            // Class representating a graphic object with a table
        public:

            //******************
            // Graphic_Texture_Table actions
            //******************

            //******************
            // Base Graphic_Texture_Table members
            //******************

            // Graphic_Base_Object constructor
            Graphic_Texture_Table(std::weak_ptr<Graphic> graphic_base):Graphic_Texture_Object(graphic_base){};

            // Draws the object on an image
            virtual void draw_on_image(std::shared_ptr<scls::__Image_Base> image);

            // Returns a parameter by its name
            virtual std::string parameter(std::string parameter);
            // Sets a parameter by its name
            virtual void set_parameter(std::string parameter_name, std::string parameter_value, std::string parameter_value_start, double proportion);

            // Returns the source to a XML text
            virtual std::string to_xml_text();
            virtual std::string to_xml_text_object_name();
            std::string to_xml_text_font_size() const;
            std::string to_xml_text_loaded() const;

            // Getters and setters
            inline void set_table(std::shared_ptr<__Table_Case> new_table){a_table = new_table;};
            inline __Table_Case* table() const {return a_table.get();};

            //******************
            // Hierarchy functions
            //******************

            // Updates the actions of the object
            virtual bool update_action(double used_delta_time, __Graphic_Object_Base::Action* action, int& deleted_objects);
        private:
            // Needed table
            std::shared_ptr<__Table_Case> a_table = __Table_Case::new_table();
        };

        // Needed fragment shader for the function
        static std::string graphic_function_fragment_shader(scls::Formula needed_formula);

        // Datas set in a graphic
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
            std::shared_ptr<scls::__Image_Base> to_image();
            std::shared_ptr<scls::__Image_Base> to_image_histogram();
            std::shared_ptr<scls::__Image_Base> to_image_point_cloud();

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

        // Function in a graphic
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
        class Graphic_Text : public Graphic_Base_Object{
        public:
            // Graphic_Text constructor
            Graphic_Text(std::weak_ptr<Graphic> graphic_base):Graphic_Base_Object(graphic_base){};

            // Moves an object to a position, from an another position
            inline void move_from_to(scls::Point_2D pos_1, scls::Point_2D pos_2, double proportion){scls::Point_2D new_pos = pos_1 + (pos_2 - pos_1) * proportion;set_x(new_pos.x());set_y(new_pos.y());};

            // Draws the object on an image
            void draw_on_image(std::shared_ptr<scls::__Image_Base> image);

            // Returns the final content in the object
            std::shared_ptr<scls::__XML_Text_Base> final_content();

            // Returns the name of the object
            virtual std::string to_displayed_text();
            virtual std::string to_xml_text();
            std::string to_xml_text_alignment_horizontal();
            std::string to_xml_text_background_color();
            std::string to_xml_text_color() const;
            std::string to_xml_text_font_size();
            std::string to_xml_text_font_size_in_scale();
            std::string to_xml_text_object_name();

            // Getters and setters
            inline scls::Alignment_Horizontal alignment_horizontal() const {return style().alignment_horizontal();};
            inline scls::Color background_color() const {return style().background_color();};
            inline scls::Color color() const {return style().color();};
            inline std::string content() const {return a_content;};
            inline scls::Fraction font_size_in_scale() const {return a_font_size_in_scale;};
            inline void set_color(scls::Color new_color){a_style.set_color(new_color);};
            inline void set_content(std::string needed_content){a_content = needed_content;};
            inline void set_font_size_in_scale(scls::Fraction new_font_size_in_scale){a_font_size_in_scale = new_font_size_in_scale;};
            inline void set_style(scls::Text_Style needed_style){a_style = needed_style;};
            inline void set_text_image_generator(std::shared_ptr<scls::Text_Image_Generator> new_text_image_generator){a_text_image_generator = new_text_image_generator;};
            inline scls::Text_Style style() const {return a_style;};
        private:
            // Content of the text
            std::string a_content;
            // Style of the text
            scls::Fraction a_font_size_in_scale = -1;
            scls::Text_Style a_style;
            // Text image generator
            std::shared_ptr<scls::Text_Image_Generator> a_text_image_generator;
        };

        //******************
        //
        // Graphic main members
        //
        //******************

        // Returns a new graphic
        static std::shared_ptr<Graphic> new_graphic(scls::_Window_Advanced_Struct* window_struct);
        static std::shared_ptr<Graphic> new_graphic();

        // Creates the background texture
        void create_background_texture(std::shared_ptr<scls::__Image_Base> image);
        void create_background_texture(int image_width, int image_height);
        void create_background_texture(std::string path);

        // Sets the size of the image
        void set_image_size(int image_width, int image_height);
        // Sets the middle of the base
        inline void set_middle(double middle_x, double middle_y){a_graphic_base.get()->a_middle_x = middle_x;a_graphic_base.get()->a_middle_y = middle_y;};
        // Sets the scale of the base
        virtual void set_scale(scls::__Formula_Base::Formula width, scls::__Formula_Base::Formula height, bool width_used, bool height_used);
        void set_scale(scls::__Formula_Base::Formula width, scls::__Formula_Base::Formula height);
        void set_scale(double width, double height, bool width_used, bool height_used);
        void set_scale(double width, double height);

        // Soft reset the graphic
        virtual void soft_reset();
        // Updates the object
        void __update_action(__Graphic_Object_Base* object, double used_delta_time);
        void __update_action(__Graphic_Object_Base* object, __Graphic_Object_Base::Action_Structure* structure, double used_delta_time);
        void update_delete();
        virtual void update(double used_delta_time);

        // Annoying functions to draw the image
        int graphic_x_to_pixel_x(double x, int needed_width);
        int graphic_x_to_pixel_x(scls::Fraction x, int needed_width);
        int graphic_x_to_pixel_x(double x, std::shared_ptr<scls::__Image_Base>& needed_image){return graphic_x_to_pixel_x(x, needed_image.get()->width());};
        int graphic_x_to_pixel_x(scls::Fraction x, std::shared_ptr<scls::__Image_Base>& needed_image){return graphic_x_to_pixel_x(x, needed_image.get()->width());};
        int graphic_y_to_pixel_y(double y, int needed_height);
        int graphic_y_to_pixel_y(scls::Fraction y, int needed_height);
        int graphic_y_to_pixel_y(double y, std::shared_ptr<scls::__Image_Base>& needed_image){return graphic_y_to_pixel_y(y, needed_image.get()->height());};
        int graphic_y_to_pixel_y(scls::Fraction y, std::shared_ptr<scls::__Image_Base>& needed_image){return graphic_y_to_pixel_y(y, needed_image.get()->height());};
        int graphic_y_to_pixel_y_inversed(double y, int needed_height);
        int graphic_y_to_pixel_y_inversed(scls::Fraction y, int needed_height);
        int graphic_y_to_pixel_y_inversed(double y, std::shared_ptr<scls::__Image_Base>& needed_image){return graphic_y_to_pixel_y_inversed(y, needed_image.get()->height());};
        int graphic_y_to_pixel_y_inversed(scls::Fraction y, std::shared_ptr<scls::__Image_Base>& needed_image){return graphic_y_to_pixel_y_inversed(y, needed_image.get()->height());};
        scls::Fraction pixel_x_to_graphic_x(int x, int needed_width);
        scls::Fraction pixel_x_to_graphic_x(int x, std::shared_ptr<scls::__Image_Base>& needed_image){return pixel_x_to_graphic_x(x, needed_image.get()->width());};
        scls::Fraction pixel_y_to_graphic_y(int y, int needed_height);
        scls::Fraction pixel_y_to_graphic_y(int y, std::shared_ptr<scls::__Image_Base>& needed_image){return pixel_y_to_graphic_y(y, needed_image.get()->height());};
        // Draw the bases of the image
        void image_draw_base(std::shared_ptr<scls::__Image_Base> to_return);
        // Draw a function on the image
        void image_draw_function(std::shared_ptr<scls::__Image_Base> to_return, std::shared_ptr<Graphic_Function> needed_function, std::vector<scls::Fraction>& screen_pos);

        // Sets an object at the foreground object
        void set_foreground_object(std::vector<std::shared_ptr<__Graphic_Object_Base>> objects);
        void set_foreground_object(std::shared_ptr<__Graphic_Object_Base> object);
        void set_foreground_object(__Graphic_Object_Base* object);
        // Sets an object render before an other
        void set_object_render_before(std::shared_ptr<__Graphic_Object_Base> object, __Graphic_Object_Base* object_to_render_before);

        // Adds a function to the graphic
        std::shared_ptr<Graphic_Function> add_function(std::shared_ptr<Function_Studied> function_studied);
        // Resets the object
        virtual void reset();
        // Returns the image of the graphic
        virtual std::shared_ptr<scls::__Image_Base> to_image(int width_in_pixel, int height_in_pixel);

        //******************
        // Circles
        //******************

        // Adds a circle to the graphic
        void add_circle(std::shared_ptr<Circle> circle_to_add);
        // Returns a circle by its id / name
        Circle* circle_by_id(int circle_id);
        std::shared_ptr<Circle> circle_by_id_shared_ptr(int circle_id);
        Circle* circle_by_name(std::string name);
        std::shared_ptr<Circle> circle_by_name_shared_ptr(std::string name);
        // Creates a new circle in the graphic
        template <typename T> std::shared_ptr<T> new_circle(std::string circle_name, std::weak_ptr<__Graphic_Object_Base> parent, scls::__Formula_Base::Formula x, scls::__Formula_Base::Formula y, scls::__Formula_Base::Formula radius_x, scls::__Formula_Base::Formula radius_y){std::shared_ptr<T>created_circle=std::make_shared<T>(graphic_base_shared_ptr(), circle_name, x, y, radius_x, radius_y);created_circle.get()->set_this_object(created_circle);created_circle.get()->set_parent(parent);add_circle(created_circle);return created_circle;};
        template <typename T> std::shared_ptr<T> new_circle(std::string circle_name, __Graphic_Object_Base* parent, scls::__Formula_Base::Formula x, scls::__Formula_Base::Formula y, scls::__Formula_Base::Formula radius_x, scls::__Formula_Base::Formula radius_y){if(parent==0){return new_circle<T>(circle_name, std::weak_ptr<__Graphic_Object_Base>(), x, y, radius_x, radius_y);}return new_circle<T>(circle_name, parent->this_object_shared_ptr(), x, y, radius_x, radius_y);};
        template <typename T> std::shared_ptr<T> new_circle(std::string circle_name, std::weak_ptr<__Graphic_Object_Base> parent, scls::Point_2D_Formula center, scls::__Formula_Base::Formula radius){return new_circle<T>(circle_name, parent, center.x(), center.y(), radius, radius.formula_copy());};
        template <typename T> std::shared_ptr<T> new_circle(std::string circle_name, scls::__Formula_Base::Formula x, scls::__Formula_Base::Formula y, scls::__Formula_Base::Formula radius){return new_circle<T>(circle_name, std::weak_ptr<__Graphic_Object_Base>(), x, y, radius, radius.formula_copy());};
        template <typename T> std::shared_ptr<T> new_circle(std::string circle_name, scls::Point_2D_Formula center, scls::__Formula_Base::Formula radius_x, scls::__Formula_Base::Formula radius_y){return new_circle<T>(circle_name, std::weak_ptr<__Graphic_Object_Base>(), center.x(), center.y(), radius_x, radius_y);};
        template <typename T> std::shared_ptr<T> new_circle(std::string circle_name, scls::Point_2D_Formula center, scls::__Formula_Base::Formula radius){return new_circle<T>(circle_name, std::weak_ptr<__Graphic_Object_Base>(), center.x(), center.y(), radius, radius);};
        template <typename T> std::shared_ptr<T> new_circle(std::string circle_name, std::weak_ptr<__Graphic_Object_Base> parent){return new_circle<T>(circle_name, parent, 0, 0, 1, 1);};
        template <typename T> std::shared_ptr<T> new_circle(std::string circle_name){return new_circle<T>(circle_name, std::weak_ptr<__Graphic_Object_Base>(), 0, 0, 1, 1);};
        // Removes circle from the graphic
        std::shared_ptr<Circle> remove_circle(std::string circle_name);

        //******************
        // Datas set
        //******************

        // Handle datas set
        // Adds a datas set to the graphic
        inline void add_datas_set(std::shared_ptr<Datas_Set> needed_datas_set){a_datas_sets.push_back(needed_datas_set);};
        // Draws a datas set on the graphic
        void draw_datas_set(Datas_Set* needed_datas_set, std::shared_ptr<scls::__Image_Base> to_return);
        // Creates and returns a datas set
        inline std::shared_ptr<Datas_Set> new_datas_set(std::string name){std::shared_ptr<Datas_Set>to_return=std::make_shared<Datas_Set>(a_this_object, name);add_datas_set(to_return);return to_return;};

        //******************
        // Forms
        //******************

        // Adds a form to the graphic
        void add_form(std::shared_ptr<Form_2D> needed_formd_2d);
        // Returns a form by its name / id
        Form_2D* form_2d_by_id(int form_id);
        std::shared_ptr<Form_2D> form_2d_by_id_shared_ptr(int form_id);
        Form_2D* form_2d_by_name(std::string form_name);
        std::shared_ptr<Form_2D> form_2d_by_name_shared_ptr(std::string form_name);
        Form_2D* form(std::string form_name);
        std::shared_ptr<Form_2D> form_shared_ptr(std::string form_name);
        // Creates and returns a form from a face
        std::shared_ptr<Form_2D> new_form_from_face(std::string name, scls::model_maker::Face* face);
        // Sets the points in a form of the graphic
        void set_form_points(Form_2D* needed_form, std::string points);
        void set_form_points(std::shared_ptr<Form_2D> needed_form, std::string points);
        // Sets the polygon in a form of the graphic
        void set_form_polygon(Form_2D* needed_form, int polygon);

        // Creates and returns a form (and its point)
        void __new_form_xml(Form_2D* needed_form_2d, std::shared_ptr<scls::__XML_Text_Base> xml);
        std::shared_ptr<Form_2D> new_form(std::string name, __Graphic_Object_Base* parent, double x_1, double y_1, double x_2, double y_2, double x_3, double y_3,double x_4, double y_4);
        std::shared_ptr<Form_2D> new_form(std::string name, double x_1, double y_1, double x_2, double y_2, double x_3, double y_3, double x_4, double y_4);
        std::shared_ptr<Form_2D> new_form(std::string name, std::vector<std::shared_ptr<Point_2D>> points);
        std::shared_ptr<Form_2D> new_form(std::string name, __Graphic_Object_Base* parent);
        std::shared_ptr<Form_2D> new_form(std::string name, std::string points);
        template <typename T = Form_2D> std::shared_ptr<T> new_form(std::string name, std::weak_ptr<__Graphic_Object_Base> parent){std::shared_ptr<T>to_return;to_return.reset(new T(graphic_base_shared_ptr(), name));to_return.get()->set_this_object(to_return);to_return.get()->set_parent(parent);to_return.get()->set_unknowns(unknowns_shared_ptr());add_form(to_return);return to_return;};
        template <typename T = Form_2D> std::shared_ptr<T> new_form(std::string name){return new_form(name, std::weak_ptr<__Graphic_Object_Base>());};
        template <typename T = Form_2D> std::shared_ptr<T> new_form(std::shared_ptr<scls::__XML_Text_Base> xml){std::shared_ptr<T> created_form = new_form<T>(std::string(), std::weak_ptr<__Graphic_Object_Base>());__new_form_xml(created_form.get(), xml);return created_form;};
        // Creates and returns a form as a polygon
        std::shared_ptr<Form_2D> new_form_polygon(std::string name, int point_number);
        // Creates and returns a rect / square (and its point)
        std::shared_ptr<Form_2D> new_rect(std::string name, __Graphic_Object_Base* parent, double x, double y, double width, double height);
        std::shared_ptr<Form_2D> new_rect(std::string name, double x, double y, double width, double height);
        // Creates and returns a triangle (and its point)
        std::shared_ptr<Form_2D> new_triangle(std::string name, scls::Fraction x_1, scls::Fraction y_1, scls::Fraction x_2, scls::Fraction y_2, scls::Fraction x_3, scls::Fraction y_3);

        // Handle lines
        // Creates and returns a line (and its points)
        template <typename T = Form_2D> std::shared_ptr<T> new_line(std::string name, __Graphic_Object_Base* parent, scls::__Formula_Base::Formula x_1, scls::__Formula_Base::Formula y_1, scls::__Formula_Base::Formula x_2, scls::__Formula_Base::Formula y_2) {
            if(name == std::string()){name = std::string("line_");}
            std::shared_ptr<T>to_return;
            if(parent == 0){to_return = new_form<T>(name, std::shared_ptr<__Graphic_Object_Base>());}
            else{to_return = new_form<T>(name, parent->this_object_shared_ptr());}
            double needed_distance = std::sqrt(std::pow((x_2 - x_1).value_to_double(), 2) + std::pow((y_2 - y_1).value_to_double(), 2));
            scls::__Formula_Base::Formula needed_height = 1;
            scls::__Formula_Base::Formula needed_width = needed_distance;
            scls::__Formula_Base::Formula needed_x = (x_2 + x_1) / 2;
            scls::__Formula_Base::Formula needed_y = (y_2 + y_1) / 2;
            to_return.get()->set_height(needed_height);to_return.get()->set_width(needed_width);
            to_return.get()->set_x(needed_x);to_return.get()->set_y(needed_y);
            to_return.get()->set_rotation(-scls::vector_2d_angle(x_2 - x_1, y_2 - y_1) * SCLS_RADIANS_TO_ANGLE);

            // Add the points
            to_return.get()->new_point(-0.5, 0);
            to_return.get()->new_point(0.5, 0);
            add_form(to_return);__line_number++;
            return to_return;

            // Add the points
            x_1 -= needed_x;x_2 -= needed_x;x_1 /= needed_width;x_2 /= needed_width;
            y_1 -= needed_y;y_2 -= needed_y;y_1 /= needed_height;y_2 /= needed_height;
            to_return.get()->new_point(x_1, y_1);
            to_return.get()->new_point(x_2, y_2);
            add_form(to_return);__line_number++;
            return to_return;
        }
        template <typename T = Form_2D> std::shared_ptr<T> new_line(std::string name, scls::__Formula_Base::Formula x_1, scls::__Formula_Base::Formula y_1, scls::__Formula_Base::Formula x_2, scls::__Formula_Base::Formula y_2){return new_line<T>(name, 0, x_1, y_1, x_2, y_2);};
        std::shared_ptr<Form_2D> new_line(std::string name, std::shared_ptr<__Graphic_Object_Base> point_1, scls::Point_2D point_2);
        std::shared_ptr<Form_2D> new_line(std::string name, std::shared_ptr<__Graphic_Object_Base> point_1, std::shared_ptr<__Graphic_Object_Base> point_2);

        //******************
        // Other objects
        //******************

        // Creates an another object
        template <typename T = Graphic_Base_Object> std::shared_ptr<T> new_object(std::string name){std::shared_ptr<T> created_object = std::make_shared<T>(a_this_object);created_object.get()->set_name(name);created_object.get()->set_this_object(created_object);a_objects.push_back(created_object);return created_object;};
        template <typename T = Graphic_Base_Object> std::shared_ptr<T> new_object(std::string name, __Graphic_Object_Base* parent){std::shared_ptr<T> created_object = new_object<T>(name);created_object.get()->set_parent(parent);return created_object;};
        template <typename T = Graphic_Base_Object> std::shared_ptr<T> new_object(std::string name, scls::__Formula_Base::Formula x, scls::__Formula_Base::Formula y){std::shared_ptr<T> created_object = new_object<T>(name);created_object.get()->set_x(x);created_object.get()->set_y(y);return created_object;};
        template <typename T = Graphic_Base_Object> std::shared_ptr<T> new_object(std::string name, __Graphic_Object_Base* parent, scls::__Formula_Base::Formula x, scls::__Formula_Base::Formula y){std::shared_ptr<T> created_object = new_object<T>(name, x, y);created_object.get()->set_parent(parent);return created_object;};

        //******************
        // Points
        //******************

        // Adds a points to the graphic
        void add_point(std::shared_ptr<Point_2D> needed_point);
        // Returns a point by his name
        std::shared_ptr<Point_2D> point_shared_ptr(std::string point_name);
        Point_2D* point(std::string point_name);
        // Creates and returns a new point in the graphic
        template <typename T> std::shared_ptr<T> new_point(std::string name, scls::Fraction x, scls::Fraction y){std::shared_ptr<T>needed=std::make_shared<T>(graphic_base_shared_ptr(), name, x, y);needed.get()->set_type(Vector_Type::VT_Point);add_point(needed);return needed;};
        std::shared_ptr<Point_2D> new_point(std::string name, scls::__Formula_Base::Formula x, scls::__Formula_Base::Formula y);
        std::shared_ptr<Point_2D> new_point(std::string name, scls::model_maker::Point* needed_point);
        std::shared_ptr<Point_2D> new_point(std::string name, scls::Point_2D needed_point);

        //******************
        // Text
        //******************

        // Adds an text to the graphic
        void add_text(std::shared_ptr<Graphic_Text> text);
        // Creates and returns a new text in the graphic
        std::shared_ptr<Graphic_Text> new_text(std::string name, std::string content, scls::Fraction x, scls::Fraction y, scls::Text_Style style);
        std::shared_ptr<Graphic_Text> new_text(std::string name, std::string content, Point_2D position, scls::Text_Style style);
        // Returns a text by his name
        std::shared_ptr<Graphic_Text> text_by_name_shared_ptr(std::string text_name);

        //******************
        // Texture
        //******************

        // Adds a texture to the graphic
        void add_texture_object(std::shared_ptr<Graphic_Texture> current_texture);
        // Creates and returns a new texture object
        template <typename T> std::shared_ptr<T> new_texture_object(scls::__Formula_Base::Formula x, scls::__Formula_Base::Formula y){std::shared_ptr<T> current_texture=std::make_shared<T>(a_this_object);current_texture.get()->set_this_object(current_texture);current_texture.get()->set_unknowns(a_unknowns);add_texture_object(current_texture);current_texture.get()->set_x(x);current_texture->set_y(y);current_texture.get()->after_creation();return current_texture;};
        template <typename T> std::shared_ptr<T> new_texture_object(std::string path, scls::__Formula_Base::Formula x, scls::__Formula_Base::Formula y){std::shared_ptr<T> current_texture=new_texture_object<T>(x, y);current_texture.get()->set_texture(path);return current_texture;};
        // Returns a texture object by its name
        std::shared_ptr<Graphic_Texture> texture_object_by_name(std::string needed_name);

        // Handle vectors
        // Adds a vector to the graphic
        inline std::shared_ptr<Point_2D> add_vector(Point_2D needed_vector){std::shared_ptr<Point_2D>new_vector=std::make_shared<Point_2D>(needed_vector);a_vectors.push_back(new_vector);return new_vector;};
        // Draws a vector on the graphic
        void draw_vector(Point_2D* needed_point, std::shared_ptr<scls::__Image_Base> to_return);

        //******************
        // Objects
        //******************

        // Handle all the objects
        // Returns an object shared ptr
        __Graphic_Object_Base* object_by_name(std::string name);
        std::shared_ptr<__Graphic_Object_Base> object_by_name_shared_ptr(std::string name);
        // Returns a list of objects shared ptr
        std::vector<std::shared_ptr<__Graphic_Object_Base>> objects_by_tag(std::string tag_name);
        // Returns a list of objects shared ptr by their type (to_xml_text_object_name())
        std::vector<std::shared_ptr<__Graphic_Object_Base>> objects_by_xml_type(std::string type_name);

        // Returns the middle position of the graphic
        inline scls::Fraction left_x() const {return a_graphic_base.get()->a_middle_x - width() / 2;};
        inline scls::Fraction middle_x() const {return a_graphic_base.get()->a_middle_x;};
        inline void middle_x_add(double value) {a_graphic_base.get()->a_middle_x += value;};
        inline scls::Fraction middle_y() const {return a_graphic_base.get()->a_middle_y;};
        inline void middle_y_add(double value) {a_graphic_base.get()->a_middle_y += value;};
        // Returns the number of pixel by case
        double pixel_by_case_x() const;
        double pixel_in_case_x(int case_number) const;
        double pixel_by_case_y() const;
        void set_pixel_by_case_x(double new_value);

        // Getters and setters
        inline void add_time(scls::Fraction new_time) {graphic_base()->a_time += new_time;};
        inline scls::Color background_color()const{return a_style.background_color();};
        inline scls::__Image_Base* background_texture() const {return a_background_texture.get();};
        inline std::shared_ptr<scls::__Image_Base> background_texture_shared_ptr() const {return a_background_texture;};
        inline std::vector<std::shared_ptr<Circle>>& circles(){return a_circles;};
        inline bool draw_background_texture() const {return a_draw_background_texture;};
        inline bool draw_base() const {return a_draw_base;};
        inline bool draw_sub_bases() const {return a_draw_sub_bases;};
        inline bool eco_mode() const {return a_graphic_base.get()->a_eco_mode;};
        inline std::vector<std::shared_ptr<Form_2D>>& forms_2d(){return a_forms_2d;};
        inline __Graphic_Base* graphic_base() const {return a_graphic_base.get();};
        inline std::shared_ptr<__Graphic_Base> graphic_base_shared_ptr() const {return a_graphic_base;};
        inline scls::Fraction height() const {return a_graphic_base.get()->a_height.value_to_fraction(a_unknowns.get());};
        inline scls::__Formula_Base::Formula height_formula() const {return a_graphic_base.get()->a_height;};
        inline bool height_used() const {return a_graphic_base.get()->a_height_used;};
        inline std::vector<std::shared_ptr<__Graphic_Object_Base>>& objects(){return a_objects;};
        inline void pixel_by_case_x_add(double value) {set_pixel_by_case_x(a_graphic_base.get()->a_pixel_by_case_x + value);};
        inline void pixel_by_case_y_add(double value) {a_graphic_base.get()->a_pixel_by_case_y += value;};
        inline std::vector<std::shared_ptr<Point_2D>>& points(){return a_points;};
        inline void set_background_color(scls::Color new_background_color){a_style.set_background_color(new_background_color);};
        inline void set_draw_base(bool new_draw_base) {a_draw_base = new_draw_base;};
        inline void set_draw_sub_bases(bool new_draw_sub_bases) {a_draw_sub_bases = new_draw_sub_bases;};
        inline void set_eco_mode(bool new_eco_mode){a_graphic_base.get()->a_eco_mode = new_eco_mode;};
        inline void set_environment(std::shared_ptr<pleos::Text_Environment> new_environment){graphic_base()->a_environment = new_environment;}
        inline void set_style(scls::Text_Style new_style) {a_style = new_style;};
        inline void set_time(scls::Fraction new_time) {graphic_base()->a_time = new_time;};
        inline void set_title(std::string new_title){a_title = new_title;};
        inline void set_unknowns(std::shared_ptr<scls::__Formula_Base::Unknowns_Container> new_unknowns){a_unknowns = new_unknowns;};
        inline void set_y_offset(int new_y_offset){a_graphic_base.get()->a_y_offset = new_y_offset;};
        inline scls::Text_Style style() const {return a_style;};
        inline std::vector<std::shared_ptr<Graphic_Text>>& texts(){return a_texts;};
        inline std::vector<std::shared_ptr<Graphic_Texture>>& textures(){return a_textures;};
        inline scls::Fraction time() const {return graphic_base()->a_time;};
        inline std::string title() const {return a_title;};
        inline scls::__Formula_Base::Unknowns_Container* unknowns() const {return a_unknowns.get();};
        inline std::shared_ptr<scls::__Formula_Base::Unknowns_Container> unknowns_shared_ptr() const {return a_unknowns;};
        inline std::vector<std::shared_ptr<Point_2D>>& vectors(){return a_vectors;};
        inline scls::Fraction width() const {return a_graphic_base.get()->a_width.value_to_fraction(a_unknowns.get());};
        inline scls::__Formula_Base::Formula width_formula() const {return a_graphic_base.get()->a_width;};
        inline bool width_used() const {return a_graphic_base.get()->a_width_used;};
        inline scls::_Window_Advanced_Struct* window_struct() const {return a_window_struct;};

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
            Graphic_Physic(std::weak_ptr<__Graphic_Object_Base> attached_object):Graphic_Physic(attached_object, attached_object.lock().get()->attached_transform_shared_ptr()){};

            // Deletes the object
            void delete_object();
            // If the object should be deleted or not
            bool should_delete() const;
            // Softs reset the object
            virtual void soft_reset();

            // Add a line / rect collision to the graphic object
            void add_collision(std::shared_ptr<Graphic_Collision> collision);
            void add_collision(scls::Fraction x_1, scls::Fraction y_1, scls::Fraction x_2, scls::Fraction y_2);
            // Checks if a collision occurs with an another collision
            void check_collision(std::shared_ptr<Graphic_Collision> collision, Graphic::Graphic_Physic* other_object);
            // Returns a new a collision to the graphic object
            std::shared_ptr<Graphic_Collision> new_collision(Graphic_Collision_Type type);
            std::shared_ptr<Graphic_Collision> new_collision(){return new_collision(Graphic_Collision_Type::GCT_Rect);};

            // Accelerates the object
            inline void accelerate(scls::Point_2D_Formula acceleration){a_attached_transform.lock().get()->accelerate(acceleration);};
            inline void accelerate(scls::Point_2D acceleration){a_attached_transform.lock().get()->accelerate(acceleration);};
            inline void accelerate_x(double acceleration){a_attached_transform.lock().get()->accelerate_x(acceleration);};
            inline void accelerate_y(double acceleration){a_attached_transform.lock().get()->accelerate_y(acceleration);};
            // Next movement generated by the velocity
            inline double next_movement_x()const{return a_attached_transform.lock().get()->next_movement_x();};
            inline double next_movement_y()const{return a_attached_transform.lock().get()->next_movement_y();};
            // Remove the X / Y velocity
            inline void remove_x_velocity(){a_attached_transform.lock().get()->set_velocity_x(0);};
            inline void remove_y_velocity(){a_attached_transform.lock().get()->set_velocity_y(0);};
            // Updates raw velocity
            inline void update_raw_velocity(){a_attached_transform.lock().get()->update_raw_velocity();};

            // Moves the object
            void __move(scls::Point_2D_Formula point){scls::Transform_Object_2D* t=a_attached_transform.lock().get();t->set_x(t->x_formula() + point.x());t->set_y(t->y_formula() + point.y());};
            void __move(){scls::Transform_Object_2D* t=a_attached_transform.lock().get();t->add_x(next_movement_x());t->add_y(next_movement_y());};

            // Precise next movement
            inline double max_absolute_x_next() const {return attached_transform()->max_absolute_x_next();};
            inline double max_absolute_y_next() const {return attached_transform()->max_absolute_y_next();};
            inline double min_absolute_x_next() const {return attached_transform()->min_absolute_x_next();};
            inline double min_absolute_y_next() const {return attached_transform()->min_absolute_y_next();};
            inline scls::Point_2D position_next() const {return attached_transform()->position_next();};
            inline double x_next() const {return attached_transform()->x_next();};
            inline double y_next() const {return attached_transform()->y_next();};

            // Returns the needed XML text to generate this graphic
            virtual std::string to_xml_text();

            // Calculates the point of the trajectory of the function
            std::vector<scls::Point_2D> trajectory_points(int point_number, double time_separation);

            // Getters and setters
            inline __Graphic_Object_Base* attached_object()const{return a_attached_object.lock().get();};
            inline std::shared_ptr<__Graphic_Object_Base> attached_object_shared_ptr()const{return a_attached_object.lock();};
            inline scls::Transform_Object_2D* attached_transform()const{return a_attached_transform.lock().get();};
            inline std::vector<std::shared_ptr<Graphic_Collision>>& collisions(){return a_collisions;};
            inline std::vector<std::shared_ptr<Graphic_Collision::Collision>>& current_collisions_results(){return a_current_collisions_results;};
            inline scls::Fraction delta_time() const {return a_delta_time;};
            inline bool ignore_dynamic_collisions() const {return a_ignore_dynamic_collisions;};
            inline bool is_static() const {return a_static;};
            inline bool loaded_in_map() const {return a_loaded_in_map;};
            inline bool moved_during_this_frame() const {return attached_transform()->moved_during_this_frame();};
            inline scls::Point_2D position() const {return a_attached_transform.lock().get()->position();};
            inline scls::Point_2D_Formula position_formula() const {return a_attached_transform.lock().get()->position_formula();};
            inline scls::Point_2D raw_velocity() const {return a_attached_transform.lock().get()->raw_velocity();};
            inline scls::Fraction raw_velocity_x() {return raw_velocity().x();};
            inline scls::Fraction raw_velocity_y() {return raw_velocity().y();};
            inline bool save_to_xml_text() const {return a_save_to_xml_text;};
            inline void set_delta_time(scls::Fraction new_delta_time){a_delta_time = new_delta_time;attached_transform()->set_delta_time(new_delta_time);for(int i = 0;i<static_cast<int>(a_collisions.size());i++){a_collisions.at(i).get()->attached_transform()->set_delta_time(new_delta_time);}};
            inline void set_ignore_dynamic_collisions(bool new_ignore_dynamic_collisions){a_ignore_dynamic_collisions = new_ignore_dynamic_collisions;};
            inline void set_loaded_in_map(bool new_loaded_map){a_loaded_in_map = new_loaded_map;};
            inline void set_save_to_xml_text(bool new_save_to_xml_text){a_save_to_xml_text = new_save_to_xml_text;};
            inline void set_static(bool new_static) {a_static = new_static;}
            inline void set_use_gravity(bool new_use_gravity){a_use_gravity = new_use_gravity;};
            inline void set_velocity(scls::Point_2D_Formula new_velocity){attached_transform()->set_velocity(new_velocity);};
            inline void set_velocity_start(scls::Point_2D_Formula new_velocity){a_velocity_start = new_velocity;};
            inline void set_velocity_x(double new_velocity_x, bool with_delta_time){if(with_delta_time){a_attached_transform.lock().get()->set_velocity_x(new_velocity_x / a_delta_time.to_double());}else{a_attached_transform.lock().get()->set_velocity_x(new_velocity_x);}};
            inline void set_velocity_x(double new_velocity_x){set_velocity_x(new_velocity_x, false);};
            inline void set_velocity_y(double new_velocity_y, bool with_delta_time){if(with_delta_time){a_attached_transform.lock().get()->set_velocity_y(new_velocity_y / a_delta_time.to_double());}else{a_attached_transform.lock().get()->set_velocity_y(new_velocity_y);}};
            inline void set_velocity_y(double new_velocity_y){set_velocity_y(new_velocity_y, false);};
            inline bool use_gravity() const {return a_use_gravity;};
            inline std::vector<Physic_Case*>& used_physic_case(){return a_used_physic_case;};
            inline scls::Point_2D velocity() const {return a_attached_transform.lock().get()->velocity();};
            inline scls::Point_2D_Formula velocity_formula() const {return a_attached_transform.lock().get()->velocity_formula();};
            inline double velocity_x() {return velocity().x();};
            inline double velocity_y() {return velocity().y();};
            inline scls::Fraction x() {return attached_transform()->x();};
            inline scls::Fraction y() {return attached_transform()->y();};
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
            // If the dynamic collision should be ignored or not
            bool a_ignore_dynamic_collisions = false;
            // If the object is loaded in the map
            bool a_loaded_in_map = false;
            // If the physic should be saved or not
            bool a_save_to_xml_text = true;
            // If the object is static or not
            bool a_static = true;
            // If the object use gravity or not
            bool a_use_gravity = true;
            // Velocity at the start of the animation
            scls::Point_2D_Formula a_velocity_start = scls::Point_2D_Formula(0, 0);
            // Used physic cases
            std::vector<Physic_Case*> a_used_physic_case;
        };
        typedef Graphic_Physic::Physic_Case Physic_Case;

        // Adds a physic object
        void add_physic_object(std::shared_ptr<Graphic_Physic> new_object){a_physic_objects.push_back(new_object);}
        // Deletes the physic in a case
        void delete_physic_object_case(Graphic::Graphic_Physic* to_delete);
        // Loads 100 X 100 physic map
        void load_physic_map(int middle_loading_x, int middle_loading_y);
        // Creates and return a new physic object
        std::shared_ptr<Graphic_Physic> new_physic_object(std::shared_ptr<__Graphic_Object_Base> object);
        // Returns a physic case by its coordinates
        Physic_Case* physic_case(int x, int y);
        // Returns a physic object by its attached object
        Graphic_Physic* physic_object_by_attached_object(__Graphic_Object_Base* attached_object);
        std::shared_ptr<Graphic_Physic> physic_object_by_attached_object_shared_ptr(__Graphic_Object_Base* attached_object);
        // Returns a list of physic object in a rectr
        std::vector<std::shared_ptr<Graphic_Physic>> physic_objects_in_rect(double x, double y, double width, double height);

        // Raycasts the map
        struct Raycast_Result{std::shared_ptr<Collision> collision;};
        Raycast_Result raycast(double x_start, double y_start, double x_direction, double y_direction, double distance);

        // Getters and setters
        inline std::vector<std::vector<std::shared_ptr<Physic_Case>>>& physic_map(){return a_physic_map;};
        inline std::vector<std::shared_ptr<Graphic_Physic>>& physic_objects(){return a_physic_objects;};

        //******************
        //
        // Settings handling
        //
        //******************

        // Displays the objects in a scroller
        void display_objects_in_scroller(scls::GUI_Scroller_Choice* scroller);
        // Loads / saves the elements to handle a circle
        void load_circle_settings(scls::GUI_Object* object, pleos::Circle* needed_circle);
        void save_circle_settings(scls::GUI_Object* object, pleos::Circle* needed_circle);
        // Loads / saves the elements to handle a form
        void load_form_settings(scls::GUI_Object* object, pleos::Form_2D* needed_form);
        void save_form_settings(scls::GUI_Object* object, pleos::Form_2D* needed_form);

        // Returns the needed XML text to generate this graphic
        std::string to_xml_text();

        //******************
        //
        // Loading handling
        //
        //******************

        // Balises circle in the graphic
        bool graphic_from_xml_balise_attribute_circle(scls::XML_Attribute& attribute, std::shared_ptr<Circle> circle, Text_Environment& environment, scls::Text_Style text_style);
        // Balises form 2D in the graphic
        bool graphic_from_xml_balise_attribute_form_2d(scls::XML_Attribute& attribute, std::shared_ptr<Form_2D> object, Text_Environment& environment, scls::Text_Style text_style);
        // Balises object in the graphic
        bool graphic_from_xml_balise_attribute_object(scls::XML_Attribute& attribute, std::shared_ptr<__Graphic_Object_Base> object, Text_Environment& environment, scls::Text_Style text_style);
        // Balises physic in the graphic
        bool graphic_from_xml_balise_attribute_physic(scls::XML_Attribute& attribute, std::shared_ptr<__Graphic_Object_Base> object, std::shared_ptr<Graphic_Physic>& physic, Text_Environment& environment, scls::Text_Style text_style);
        // Balises object in the texture object
        bool graphic_from_xml_balise_attribute_texture_object(scls::XML_Attribute& attribute, std::shared_ptr<Graphic_Texture> object, Text_Environment& environment, scls::Text_Style text_style);

        // Balises in the graphic
        virtual std::shared_ptr<pleos::__Graphic_Object_Base> graphic_from_xml_balise(std::shared_ptr<scls::__XML_Text_Base> xml, Text_Environment& environment, scls::Text_Style text_style);
        virtual std::shared_ptr<pleos::__Graphic_Object_Base> graphic_from_xml_balise_action(std::shared_ptr<scls::__XML_Text_Base> xml, Text_Environment& environment, scls::Text_Style text_style, __Graphic_Object_Base::Action_Structure* structure);
        virtual void __graphic_from_xml_balises(std::shared_ptr<scls::__XML_Text_Base> xml, Text_Environment& environment, scls::Text_Style text_style, int graphic_width_in_pixel, int graphic_height_in_pixel);
        virtual void graphic_from_xml(std::shared_ptr<scls::__XML_Text_Base> xml, scls::Text_Style needed_style, std::shared_ptr<Text_Environment> environment, int& graphic_width_in_pixel, int& graphic_height_in_pixel);
        void graphic_from_xml(std::shared_ptr<scls::__XML_Text_Base> xml, scls::Text_Style needed_style, int& graphic_width_in_pixel, int& graphic_height_in_pixel);
        void graphic_from_xml(std::shared_ptr<scls::__XML_Text_Base> xml, scls::Text_Style needed_style, std::shared_ptr<Text_Environment> environment);

        // Generates a new line from XML
        template <typename T = Form_2D> std::shared_ptr<T> new_line_xml(std::shared_ptr<scls::__XML_Text_Base> xml, Text_Environment& environment){scls::__Formula_Base::Formula x_1;scls::__Formula_Base::Formula x_2;scls::__Formula_Base::Formula y_1;scls::__Formula_Base::Formula y_2;__new_line_xml(x_1, x_2, y_1, y_2, xml, environment);std::shared_ptr<T> created_line = new_line<T>(std::string(), x_1, y_1, x_2, y_2);return created_line;}
        void __new_line_xml(scls::__Formula_Base::Formula& x_1, scls::__Formula_Base::Formula& x_2, scls::__Formula_Base::Formula& y_1, scls::__Formula_Base::Formula& y_2, std::shared_ptr<scls::__XML_Text_Base> xml, Text_Environment& environment);

        //******************
        //
        // Built-in features
        //
        //******************

        // Creates a number line
        void number_line(scls::Fraction x, scls::Fraction y, scls::Fraction length);

    protected:

        // Graphic constructor
        Graphic(){a_style.set_background_color(scls::Color(0, 0, 0, 0));};

        // This object
        std::weak_ptr<Graphic> a_this_object;
        // Window struct
        scls::_Window_Advanced_Struct* a_window_struct = 0;

    private:

        // Background texture of the graphic
        std::shared_ptr<scls::__Image_Base> a_background_texture;
        // Things to draw
        char a_draw_background_texture = 1;
        bool a_draw_base = true;
        bool a_draw_sub_bases = true;
        // Style of the graphic
        scls::Text_Style a_style = scls::Text_Style();
        // Title of the graphic
        std::string a_title = std::string();
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
        // All objects created
        std::vector<std::shared_ptr<__Graphic_Object_Base>> a_objects;
        // Geometrical point
        std::vector<std::shared_ptr<Point_2D>> a_points;
        // Text
        std::vector<std::shared_ptr<Graphic_Text>> a_texts;
        // Texture objects
        std::vector<std::shared_ptr<Graphic_Texture>> a_textures;
        // Geometrical vectors
        std::vector<std::shared_ptr<Point_2D>> a_vectors;

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

        //******************
        //
        // Transformation handling
        //
        //******************

        // Pixel to add to each case for graphic -> pixel size comparaison
        std::vector<long long int> a_pixel_case_repartition_x;
    };

    class Graphic_Object : public scls::GUI_Object {
        // Class representating an object containing a graphic
    public:

        class Graphic_GUI_Object : public Graphic::Graphic_Base_Object {
            // GUI object in a graphic
        public:
            // Graphic_GUI_Object constructor
            Graphic_GUI_Object(Graphic_Object* graphic_object, std::shared_ptr<scls::GUI_Object>needed_object):Graphic::Graphic_Base_Object(graphic_object->graphic_shared_ptr()),a_graphic_object(graphic_object),a_object(needed_object){needed_object.get()->set_ignore_click(true);needed_object.get()->set_texture_alignment(scls::T_Fill);};

            // Soft resets the object
            virtual void soft_reset(){Graphic::Graphic_Base_Object::soft_reset();object()->soft_reset();};
            virtual void soft_reset_physic(){if(a_physic_object.get()!=0){a_physic_object.get()->soft_reset();}};
            // Updates the object
            virtual int update(double used_delta_time){object()->update();return 1;};
            virtual void update_event(){object()->update_event();};

            // Move the GUI object
            void move_x(double new_velocity){physic_object()->set_velocity_x(new_velocity);};
            // Scale the GUI Object
            void scale(Graphic* graphic, int image_width, int image_height);

            // Sets the physic object
            inline void set_physic_object(bool is_static){a_physic_object=std::make_shared<Graphic::Graphic_Physic>(this_object_shared_ptr());a_physic_object.get()->set_static(is_static);a_physic_object.get()->set_use_gravity(!is_static);graphic()->add_physic_object(a_physic_object);};

            // Getters and setters
            inline Graphic_Object* graphic_object() const {return a_graphic_object;};
            inline scls::GUI_Object* object() const {return a_object.get();};
            inline std::shared_ptr<scls::GUI_Object> object_shared_ptr() const {return a_object;};
            inline Graphic::Graphic_Physic* physic_object() const {return a_physic_object.get();};
            inline void set_texture_scale_x(double new_texture_scale_x){a_texture_scale_x = new_texture_scale_x;};
            inline void set_texture_scale_y(double new_texture_scale_y){a_texture_scale_y = new_texture_scale_y;};
            inline scls::_Window_Advanced_Struct* window_struct()const{return &a_object.get()->window_struct();};

            //******************
            // Hierarchy functions
            //******************

            // Enables / disables physic
            virtual void disable_physic(){if(a_physic_object.get() != 0){a_physic_object.get()->delete_object();}a_physic_object.reset();};
            virtual void enable_physic(){};

        private:
            // Attached graphic
            Graphic_Object* a_graphic_object = 0;
            // GUI object
            std::shared_ptr<scls::GUI_Object> a_object;

            //******************
            //
            // Physic handling
            //
            //******************

            // Physic object of this object
            std::shared_ptr<Graphic::Graphic_Physic> a_physic_object;

            //******************
            //
            // Texture handling
            //
            //******************

            // Texture scale X / Y
            double a_texture_scale_x = 1;double a_texture_scale_y = 1;
        };

        //******************
        //
        // Graphic_Object handling
        //
        //******************

        // Graphic constructor
        Graphic_Object(scls::_Window_Advanced_Struct& window, std::string name, std::weak_ptr<scls::GUI_Object> parent);

        // Function called after resizing the window
        virtual void after_resizing(){update_texture();if(a_datas.get()->background_texture()!=0){create_background_texture();}};
        // Renders the object
        virtual void render(bool render_children, glm::vec3 scale_multiplier);
        // Soft resets the object
        virtual void soft_reset();
        // Updates the object
        virtual void update();
        virtual void update_event();
        virtual void update_event(double needed_delta_time);
        virtual void update_texture();

        // Creates the background texture
        void create_background_texture();
        void create_background_texture(int width, int height);
        void create_background_texture(std::string path);
        void create_background_texture(std::string path, int width, int height);

        // Adds a function to the graphic
        inline void add_function(std::shared_ptr<Function_Studied> function_studied){a_datas.get()->add_function(function_studied);};
        // Resets the object
        inline void reset(){a_datas.get()->reset();};
        // Returns the image of the graphic
        inline std::shared_ptr<scls::__Image_Base> to_image(){return a_datas.get()->to_image(width_in_pixel(), height_in_pixel());};
        inline bool use_image() const {return (a_datas.get()->draw_base() && a_datas.get()->draw_sub_bases()) || (a_datas.get()->objects().size() > 0 || a_datas.get()->texts().size() > 0 || a_datas.get()->vectors().size() > 0);};

        // Handle circles
        // Creates a new circle in the graphic
        template <typename T> std::shared_ptr<T> new_circle(std::string circle_name, scls::__Formula_Base::Formula x, scls::__Formula_Base::Formula y, scls::__Formula_Base::Formula radius){return a_datas.get()->new_circle<T>(circle_name, x, y, radius);};
        template <typename T> std::shared_ptr<T> new_circle(std::string circle_name, scls::Point_2D_Formula center, scls::__Formula_Base::Formula radius){return a_datas.get()->new_circle<T>(circle_name, center, radius);};
        template <typename T> std::shared_ptr<T> new_circle(std::string circle_name){return a_datas.get()->new_circle<T>(circle_name);};
        // Removes circle from the graphic
        inline std::shared_ptr<Circle> remove_circle(std::string circle_name){return a_datas.get()->remove_circle(circle_name);};

        // Handle forms
        // Adds a forms to the graphic
        inline void add_form(std::shared_ptr<Form_2D> needed_formd_2d){a_datas.get()->add_form(needed_formd_2d);};

        // Handle points
        // Adds a points to the graphic
        inline void add_point(std::shared_ptr<Point_2D> needed_point){a_datas.get()->add_point(needed_point);};
        // Creates and returns a new point in the graphic
        inline std::shared_ptr<Point_2D> new_point(std::string name, scls::__Formula_Base::Formula x, scls::__Formula_Base::Formula y){return a_datas.get()->new_point(name, x, y);};
        inline std::shared_ptr<Point_2D> new_point(std::string name, scls::Point_2D needed_point){return a_datas.get()->new_point(name, needed_point.x(), needed_point.y());};

        // Handle vectors
        // Adds a vector to the graphic
        inline void add_vector(Point_2D needed_vector){a_datas.get()->add_vector(needed_vector);};
        inline void add_vector(scls::Fraction x, scls::Fraction y){a_datas.get()->add_vector(Point_2D(graphic_base_shared_ptr(), std::string(), x, y));};
        // Draws a vector on the graphic
        inline void draw_vector(Point_2D* needed_point, std::shared_ptr<scls::__Image_Base> to_return){a_datas.get()->draw_vector(needed_point, to_return);};

        // Creates and returns a form
        inline std::shared_ptr<Form_2D> new_form(std::string name){return a_datas.get()->new_form<Form_2D>(name);};
        // Creates and returns a rect / square (and its point)
        inline std::shared_ptr<Form_2D> new_rect(std::string name, __Graphic_Object_Base* parent, double x, double y, double width, double height){return a_datas.get()->new_rect(name, parent, x, y, width, height);};
        inline std::shared_ptr<Form_2D> new_rect(std::string name, double x, double y, double width, double height){return a_datas.get()->new_rect(name, x, y, width, height);};

        // Handle other object
        // Adds a new GUI Object
        inline void add_other_object(std::shared_ptr<Graphic_GUI_Object> object){object.get()->set_this_object(object);a_gui_objects.push_back(object);};
        // Creates a new GUI object
        template <typename T = Graphic_GUI_Object, typename G = scls::GUI_Object> std::shared_ptr<T> new_other_object(std::string other_name){std::shared_ptr<scls::GUI_Object>to_return=*new_object<G>(other_name);std::shared_ptr<T>object=std::make_shared<T>(this,to_return);add_other_object(object);return object;};

        // Annoying functions to draw the image
        inline int graphic_x_to_pixel_x(double x, int needed_width){return a_datas.get()->graphic_x_to_pixel_x(x, needed_width);};
        inline int graphic_x_to_pixel_x(scls::Fraction x, int needed_width){return a_datas.get()->graphic_x_to_pixel_x(x, needed_width);};
        inline int graphic_x_to_pixel_x(double x, std::shared_ptr<scls::__Image_Base>& needed_image){return a_datas.get()->graphic_x_to_pixel_x(x, needed_image);};
        inline int graphic_x_to_pixel_x(scls::Fraction x, std::shared_ptr<scls::__Image_Base>& needed_image){return a_datas.get()->graphic_x_to_pixel_x(x, needed_image);};
        inline int graphic_y_to_pixel_y(double y, int needed_height){return a_datas.get()->graphic_y_to_pixel_y(y, needed_height);};
        inline int graphic_y_to_pixel_y(scls::Fraction y, int needed_height){return a_datas.get()->graphic_y_to_pixel_y(y, needed_height);};
        inline int graphic_y_to_pixel_y(double y, std::shared_ptr<scls::__Image_Base>& needed_image){return a_datas.get()->graphic_y_to_pixel_y(y, needed_image);};
        inline int graphic_y_to_pixel_y(scls::Fraction y, std::shared_ptr<scls::__Image_Base>& needed_image){return a_datas.get()->graphic_y_to_pixel_y(y, needed_image);};
        inline int graphic_y_to_pixel_y_inversed(double y, int needed_height){return a_datas.get()->graphic_y_to_pixel_y_inversed(y, needed_height);};
        inline int graphic_y_to_pixel_y_inversed(scls::Fraction y, int needed_height){return a_datas.get()->graphic_y_to_pixel_y_inversed(y, needed_height);};
        inline int graphic_y_to_pixel_y_inversed(double y, std::shared_ptr<scls::__Image_Base>& needed_image){return a_datas.get()->graphic_y_to_pixel_y_inversed(y, needed_image);};
        inline int graphic_y_to_pixel_y_inversed(scls::Fraction y, std::shared_ptr<scls::__Image_Base>& needed_image){return a_datas.get()->graphic_y_to_pixel_y_inversed(y, needed_image);};
        inline scls::Fraction pixel_x_to_graphic_x(int x, int needed_width){return a_datas.get()->pixel_x_to_graphic_x(x, needed_width);};
        inline scls::Fraction pixel_x_to_graphic_x(int x, std::shared_ptr<scls::__Image_Base>& needed_image){return a_datas.get()->pixel_x_to_graphic_x(x, needed_image);};
        inline scls::Fraction pixel_y_to_graphic_y(int y, int needed_height){return a_datas.get()->pixel_y_to_graphic_y(y, needed_height);};
        inline scls::Fraction pixel_y_to_graphic_y(int y, std::shared_ptr<scls::__Image_Base>& needed_image){return a_datas.get()->pixel_y_to_graphic_y(y, needed_image);};

        // Create a new graphic
        template <typename T> void set_datas(){a_datas = T::new_graphic(&window_struct());};

        // Getters and setters
        inline std::vector<std::shared_ptr<Circle>>& circles(){return a_datas.get()->circles();};
        inline std::vector<std::shared_ptr<__Graphic_Object_Base>>& created_objects_at_click_this_frame(){return a_created_objects_at_click_this_frame;};
        inline Form_2D* current_form_2d() const {return a_current_form_2d.get();};
        inline bool draw_base() const {return a_datas.get()->draw_base();};
        inline bool draw_sub_bases() const {return a_datas.get()->draw_sub_bases();};
        inline Graphic* graphic() {return a_datas.get();};
        inline std::shared_ptr<__Graphic_Base> graphic_base_shared_ptr() {return graphic()->graphic_base_shared_ptr();};
        inline std::shared_ptr<Graphic> graphic_shared_ptr() {return a_datas;};
        inline std::vector<std::shared_ptr<Graphic_GUI_Object>>& gui_objects(){return a_gui_objects;};
        inline scls::Fraction middle_x() const {return a_datas.get()->middle_x();};
        inline void middle_x_add(double value) {a_datas.get()->middle_x_add(value);};
        inline scls::Fraction middle_y() const {return a_datas.get()->middle_y();};
        inline void middle_y_add(double value) {a_datas.get()->middle_y_add(value);};
        inline std::vector<std::shared_ptr<__Graphic_Object_Base>>& objects(){return a_datas.get()->objects();};
        inline int operation_at_click() const {return a_operation_at_click;};
        inline double pixel_by_case_x() const {return a_datas.get()->pixel_by_case_x();};
        inline void pixel_by_case_x_add(double value) const {a_datas.get()->pixel_by_case_x_add(value);};
        inline double pixel_by_case_y() const {return a_datas.get()->pixel_by_case_y();};
        inline void pixel_by_case_y_add(double value) const {a_datas.get()->pixel_by_case_y_add(value);};
        inline std::vector<std::shared_ptr<Point_2D>>& points(){return a_datas.get()->points();};
        inline void set_draw_base(bool new_draw_base) {a_datas.get()->set_draw_base(new_draw_base);};
        inline void set_draw_sub_bases(bool new_draw_sub_bases) {a_datas.get()->set_draw_sub_bases(new_draw_sub_bases);};
        inline void set_operation_at_click(int new_operation_at_click) {a_operation_at_click = new_operation_at_click;};
        inline void set_update_physic_with_events(bool new_update_physic_with_events){a_update_physic_with_events = new_update_physic_with_events;};
        inline bool update_physic_with_events() const {return a_update_physic_with_events;};
        inline std::vector<std::shared_ptr<Point_2D>>& vectors(){return a_datas.get()->vectors();};

        //******************
        //
        // Physic handling
        //
        //******************

        // Handles the removing of a physic object
        void delete_physic_object(Graphic::Graphic_Physic* to_delete);
        // Loads 100 X 100 physic map
        inline void load_physic_map(int middle_loading_x, int middle_loading_y){a_datas.get()->load_physic_map(middle_loading_x, middle_loading_y);};
        // Returns a physic case by its coordinates
        inline Graphic::Physic_Case* physic_case(int x, int y){return a_datas.get()->physic_case(x, y);};
        // Updates the physic
        int update_physic(double multiplier);

        // Getters and setters
        inline std::vector<std::shared_ptr<Graphic::Graphic_Physic>>& physic_objects(){return a_datas.get()->physic_objects();};

    private:

        // Datas about the graphic
        std::shared_ptr<Graphic> a_datas = Graphic::new_graphic();
        // Other object in the graphic
        std::vector<std::shared_ptr<Graphic_GUI_Object>> a_gui_objects;
        // If the physic should be updated with the events
        bool a_update_physic_with_events = true;

        // Created objects at click
        std::vector<std::shared_ptr<__Graphic_Object_Base>> a_created_objects_at_click_this_frame;
        // Operation to do at click
        int a_operation_at_click = PLEOS_OPERATION_NOTHING;

        // Current object
        std::shared_ptr<Form_2D> a_current_form_2d;
    };
}

#endif // PLEOS_GRAPHIC
