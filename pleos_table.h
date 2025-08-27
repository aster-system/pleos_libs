//******************
//
// pleos_text.h
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
// This file contains the core of the table part for PLEOS.
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

#ifndef PLEOS_TABLE
#define PLEOS_TABLE

// Include SCLS Graphic Benoit
#include "../../scls-graphic-benoit/scls_graphic.h"
#include "pleos_mathematics/pleos_mathematics_functions.h"
#include "pleos_it.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //*********
	//
	// PLEOS Table handler
	//
	//*********

	class Illustrator {
	    // Class representating a basic illustrator for PLEOS
    public:
        // Illustrator constructor
        Illustrator(){};

        // Handle the title
        std::shared_ptr<scls::__Image_Base> title_image(scls::Text_Image_Generator* tig);

        // Title of the illustrator
        inline void set_title(std::string new_title){a_title = new_title;};
        inline std::string title()const{return a_title;};
        inline scls::Text_Style* title_style() const {return a_title_style.get();};
    private:
        // Title of the illustration
        std::string a_title = std::string();
        std::shared_ptr<scls::Text_Style> a_title_style = std::make_shared<scls::Text_Style>();
	};

	class __Table_Case : public Illustrator {
        // Class representating a more table handler for PLEOS
    public:

        // Cases in the table
        struct Table_Case {
            // Image in the case
            std::shared_ptr<scls::Image> image;
            // Margin of the case
            int margin = 20;
            int margin_bottom = 5;int margin_top = 5;
            // If the case is merged or not
            enum Merge_State {MS_No, MS_Merged, MS_Merged_Main};
            Merge_State merged = MS_No;
            int merged_height = 1;int merged_width = 1;
            // If the case use a right border or not
            bool right_border = true;
            // Style of the case
            scls::Text_Style style;

            // Height / width of the case
            int height() const;
            int width() const;

            // Getters and setters
            inline scls::Color background_color() const {return style.background_color();};
            inline int font_size() const {return style.font_size();};
            inline void set_background_color(scls::Color new_color) {style.set_background_color(new_color);};
            inline void set_image(scls::Image new_image){(*image.get()) = new_image;};

            // Parent table
            __Table_Case* parent_table()const{return parent_table_weak_ptr.lock().get();};
            std::weak_ptr<__Table_Case> parent_table_weak_ptr;
        };

        // Table constructor
        template <typename T = __Table_Case> static std::shared_ptr<T> new_table(){{std::shared_ptr<T> to_return = std::shared_ptr<T>(new T());to_return.get()->a_this_object = to_return;return to_return;}};

        // Returns the case at a certain position
        Table_Case* case_at(int x, int y);
        inline scls::Image& image_at(int x, int y){return *(case_at(x, y)->image.get());};
        // Checks the merge of the case
        void check_merge();
        // Returns the number of column in the table
        inline int column_number() const{return a_cases.size();};
        // Returns the width of a column
        int column_width(int column) const;
        int column_width(int column, int width) const;
        // Returns the number of lines in the table
        int line_number() const;
        // Returns the height of a line
        int line_height(int line) const;
        // Returns the total height / width of the image
        inline int total_height_in_pixel() const {int to_return = 0;int needed_width = line_number();for(int i = 0;i<static_cast<int>(needed_width);i++){to_return += line_height(i);};return to_return;};
        inline int total_width_in_pixel() const {int to_return = 0;int needed_width = column_number();for(int i = 0;i<static_cast<int>(needed_width);i++){to_return += column_width(i);};return to_return;};

        // Merges cases
        void merge_cases(int x, int y, int width, int height);

        // Set the value of an std::string case
        virtual std::shared_ptr<scls::__Image_Base> case_image_from_text(std::string value, scls::Text_Style needed_style, scls::Text_Image_Generator* tig);
        void set_case_value(int x, int y, std::string value, scls::Text_Style needed_style, scls::Text_Image_Generator* tig);
        void set_cases_value(int x, int y, int width, int height, std::string value, scls::Text_Style needed_style, scls::Text_Image_Generator* tig);

        // Returns the table to an image
        scls::Image to_image();

        // Getters and setters
        inline int column_separation() const {return a_column_separation;};
        inline std::string loaded() const {return a_loaded;};
        inline int minimum_case_width() const {return a_minimum_case_width;};
        inline void set_loaded(std::string new_loaded){a_loaded = new_loaded;};
        inline void set_minimum_case_width(int new_minimum_case_width) {a_minimum_case_width = new_minimum_case_width;};

    protected:

        // Table constructor
        __Table_Case():Illustrator(){};

    private:

        // Cases in the table ([x][y])
        std::vector<std::vector<std::shared_ptr<Table_Case>>> a_cases;

        // Separation between two columns
        int a_column_separation = 2;
        // If the table has been load
        std::string a_loaded = std::string();
        // Minimum width of a case
        int a_minimum_case_width = 0;
        // Weak ptr to this table
        std::weak_ptr<__Table_Case> a_this_object;
    };

    // Creates and returns a table from an std::string
	std::shared_ptr<__Table_Case> table_from_xml(std::shared_ptr<__Table_Case> table, std::shared_ptr<scls::__XML_Text_Base> xml, scls::Text_Style needed_style);
	template <typename T = __Table_Case> std::shared_ptr<T> table_from_xml(std::shared_ptr<scls::__XML_Text_Base> xml, scls::Text_Style needed_style){std::shared_ptr<T> to_return = __Table_Case::new_table<T>();table_from_xml(to_return, xml, needed_style);return to_return;}
}

#endif //PLEOS_TABLE
