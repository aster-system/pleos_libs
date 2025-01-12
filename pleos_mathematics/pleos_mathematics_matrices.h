//******************
//
// pleos_mathematics_matrices.h
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
// This file contains some features to handle matrices in the PLEOS mathematics file.
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

#ifndef PLEOS_MATHEMATICS_MATRICES
#define PLEOS_MATHEMATICS_MATRICES

// Include SCLS Graphic Benoit
#include "../../../scls-graphic-benoit/scls_graphic.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    //******************
    //
    // The "Matrices" class
    //
    //******************

    template<typename E = scls::Fraction>
    class Matrice {
        // Class representating a matrice
    public:
        // Class representating the dimension of a matrice
        class Matrice_Dimension {
        public:
            // Matrice_Dimension  constructor
            Matrice_Dimension(){};

            // Add a dimension
            inline void add_dimension(unsigned int dimension) {a_dimensions.push_back(dimension);};
            // Returns the number of dimension in the matricce dimension
            inline int dimension_number() const {return a_dimensions.size();};
            // Returns the first/last dimension
            inline unsigned int first_dimension() const {return a_dimensions[0];};
            inline unsigned int last_dimension() const {return a_dimensions[a_dimensions.size() - 1];};

            // Convert to an integer
            inline operator int() const {return a_dimensions.at(0);};
        private:
            // Each dimensions of the matrice
            std::vector<unsigned int> a_dimensions;
        };

        // Matrice constructor
        Matrice(std::string name, int dimension):a_elements(std::vector<std::shared_ptr<E>>(dimension)),a_name(name) {
            for(int i = 0;i<static_cast<int>(a_elements.size());i++) {
                a_elements[i] = std::make_shared<E>();
            }
        };
        Matrice(std::string name, int sub_dimension, int dimension):a_matrices(std::vector<std::shared_ptr<Matrice<E>>>(dimension)),a_name(name){
            for(int i = 0;i<static_cast<int>(a_matrices.size());i++) {
                a_matrices[i] = std::make_shared<Matrice<E>>(name + "_" + std::to_string(i), sub_dimension);
            }
        };
        Matrice():Matrice("", 0){};
        // Copy constructor
        Matrice(const Matrice& other):Matrice(other.name(), 0){other.copy_value(this);};

        // Returns an element in the matrice
        inline E at(int indice) const {return *(a_elements[indice].get());};
        // Returns if thie matrice contains an another matrice
        inline bool contains_matrices() const {return a_matrices.size() > 0;};
        // Copy the value of the matrice into another matrice
        inline void copy_value(Matrice* other) const {
            // Copy the elements
            other->a_elements = std::vector<std::shared_ptr<E>>(a_elements.size());
            for(int i = 0;i<static_cast<int>(a_elements.size());i++) {
                other->a_elements[i] = std::make_shared<E>(*(a_elements.at(i).get()));
            }
            // Copy the matrices
            other->a_matrices = std::vector<std::shared_ptr<Matrice>>(a_matrices.size());
            for(int i = 0;i<static_cast<int>(a_matrices.size());i++) {
                other->a_matrices[i] = std::make_shared<Matrice>(*(a_matrices.at(i).get()));
            }
        };
        // Returns a matrice in the matrice
        inline Matrice* matrice_at(int indice) const {return a_matrices[indice].get();};
        // Set the value at a certain position
        inline void set(int indice, E element) {a_elements[indice] = std::make_shared<E>(element);};
        inline void set(int indice_1, int indice_2, E element) {matrice_at(indice_2)->set(indice_1, element);};
        // Size of the matrice
        inline int size() const {if(contains_matrices()) {return a_matrices.size();} return a_elements.size();};

        // Returns the name of the matrice
        inline std::string name() const {return a_name;};
        inline void set_name(std::string new_name) {a_name = new_name;};
        // Returns the matrice to std::string
        std::string to_std_string() {
            std::string to_return = "";
            // Add the sub-matrices
            if(a_matrices.size() > 0) {
                for(int i = 0;i<static_cast<int>(a_matrices.size());i++) {
                    to_return += a_matrices[i].get()->to_std_string();
                    if(i != a_matrices.size() - 1) {to_return += ", ";}
                }
            } else {
                // Add the elements
                for(int i = 0;i<static_cast<int>(a_elements.size());i++) {
                    to_return += a_elements[i].get()->to_std_string();
                    if(i != a_elements.size() - 1) {to_return += ", ";}
                }
            } to_return = std::string("(") + to_return + std::string(")");
            return to_return;
        };

        // Returns the dimension of the matrice
        Matrice_Dimension dimension() {
            Matrice_Dimension to_return;
            if(contains_matrices()) {
                to_return = a_matrices[0].get()->dimension();
            } to_return.add_dimension(size());
            return to_return;
        }

        // Add this matrice by another matrice
        void __add(Matrice<E> other) {
            if(other.dimension() == dimension()) {
                // Add the matrices
                for(int i = 0;i<static_cast<int>(a_matrices.size());i++) {
                    a_matrices[i].get()->__add(*other.a_matrices[i].get());
                }
                // Add the elements
                for(int i = 0;i<static_cast<int>(a_elements.size());i++) {
                    (*a_elements[i].get()) += (*other.a_elements[i].get());
                }
            }
        };
        // Divide this matrice by a value of the matrice
        void __divide(E other) {
            // Multiply the matrices
            for(int i = 0;i<static_cast<int>(a_matrices.size());i++) {
                a_matrices[i].get()->__divide(other);
            }
            // Multiply the elements
            for(int i = 0;i<static_cast<int>(a_elements.size());i++) {
                (*a_elements[i].get()) /= (other);
            }
        };
        // Multiply this matrice by another matrice
        void __multiply(Matrice<E> other) {
            if(dimension().dimension_number() == 2 && other.dimension().dimension_number() <= dimension().dimension_number()) {
                Matrice_Dimension dim_1 = dimension();
                Matrice_Dimension dim_2 = other.dimension();
                if(dim_1.first_dimension() == dim_2.last_dimension()) {
                    // Create the final matrice
                    Matrice<E> final_matrice(name() + " * " + other.name(), dim_2.first_dimension(), dim_1.last_dimension());
                    int same_dimension = dimension().first_dimension();
                    // Multiply as a single dimension matrice
                    for(int i = 0;i<static_cast<int>(a_matrices.size());i++) {
                        for(int j = 0;j<static_cast<int>(other.matrice_at(0)->size());j++) {
                            // Add the elements
                            E current_value;
                            for(int k = 0;k<same_dimension;k++) {
                                current_value += matrice_at(i)->at(k) * other.matrice_at(k)->at(j);
                            } final_matrice.set(j, i, current_value);
                        }
                    }
                    // Set the final value
                    *this = final_matrice;
                }
            }
            else if(other.dimension().dimension_number() == 1 && dimension().dimension_number() == other.dimension()) {
                // Multiply by a single dimension matrice
                // Get the total of elements
                E total_product = *other.a_elements[0].get();
                for(int i = 1;i<static_cast<int>(a_elements.size());i++) {
                    total_product *= *other.a_elements[i].get();
                }
                for(int i = 0;i<static_cast<int>(a_matrices.size());i++) {

                }
                // Multiply the elements
                for(int i = 0;i<static_cast<int>(a_elements.size());i++) {
                    (*a_elements[i].get()) *= total_product;
                }
            }
            else if(dimension().dimension_number() == 1 && other.dimension().last_dimension() == 1 && dimension() == other.dimension().first_dimension()) {
                // Multiply as a single dimension matrice
                E final_value;
                for(int i = 0;i<static_cast<int>(a_elements.size());i++) {
                    // Add the elements
                    final_value += other.matrice_at(i)->at(0) * (*a_elements[i].get());
                }
                // Set the final value
                Matrice<E> final_matrice(name() + " * " + other.name(), 1);
                final_matrice[0] = final_value;
                *this = final_matrice;
            }
        };
        // Multiply this matrice by a value of the matrice
        void __multiply(E other) {
            // Multiply the matrices
            for(int i = 0;i<static_cast<int>(a_matrices.size());i++) {
                a_matrices[i].get()->__multiply(other);
            }
            // Multiply the elements
            for(int i = 0;i<static_cast<int>(a_elements.size());i++) {
                (*a_elements[i].get()) *= (other);
            }
        };

        // Access operators
        inline E& operator[](int indice) {return *a_elements[indice].get();};
        // Operate with others matrices
        inline Matrice<E> operator+(Matrice<E> other) {Matrice<E> to_return=*this;to_return.__add(other);return to_return;};
        inline Matrice<E>& operator+=(Matrice<E> other) {__add(other);return *this;};
        inline Matrice<E> operator*(Matrice<E> other) {Matrice<E> to_return=*this;to_return.__multiply(other);return to_return;};
        inline Matrice<E>& operator*=(Matrice<E> other) {__multiply(other);return *this;};
        // Operate with E
        inline Matrice<E> operator*(E other) {Matrice<E> to_return=*this;to_return.__multiply(other);return to_return;};
        inline Matrice<E>& operator*=(E other) {__multiply(other);return *this;};
        inline Matrice<E>& operator/=(E other) {__divide(other);return *this;};

    private:
        // Elements in the matrice
        std::vector<std::shared_ptr<E>> a_elements;
        // Matrices in the matrice
        std::vector<std::shared_ptr<Matrice>> a_matrices;
        // Name of the matrice
        std::string a_name = "";
    };

    // Converts a fraction matrice to an image
    static std::shared_ptr<scls::Image> matrice_to_image(Matrice<scls::Fraction> matrice, scls::Text_Image_Generator* generator) {
        // Asserts
        if(matrice.dimension().dimension_number() > 2){return std::shared_ptr<scls::Image>();}

        if(matrice.dimension().dimension_number() == 1) {
            // For a one dimension matrice
            std::vector<std::shared_ptr<scls::Image>> images = std::vector<std::shared_ptr<scls::Image>>();
            scls::Text_Style style;
            // Create each images
            int max_height = 0;
            int number = matrice.dimension();
            int total_width = 0;
            for(int i = 0;i<number;i++) {
                // Get each images
                std::shared_ptr<scls::Image> current_image = generator->image_shared_ptr(matrice.at(i).to_mathml(), style);
                images.push_back(current_image);
                if(max_height < current_image.get()->height()){max_height = current_image.get()->height();}
                total_width += current_image.get()->width();
            }

            // Include the separation
            int image_end_size = 40; scls::Text_Style style_end = style; style_end.font_size = image_end_size;
            int image_start_size = 40;scls::Text_Style style_start = style; style_start.font_size = image_end_size;
            std::shared_ptr<scls::Image> image_start = generator->image_shared_ptr("(", style_start);
            std::shared_ptr<scls::Image> image_end = generator->image_shared_ptr(")", style_end);
            int separation_width = style.font_size;
            total_width += (static_cast<int>(images.size()) - 1) * separation_width + image_start.get()->width() + image_end.get()->width();
            if(image_end.get()->height() > max_height){max_height = image_end.get()->height();}
            if(image_start.get()->height() > max_height){max_height = image_start.get()->height();}

            // Create the final image
            int current_x = 0; int current_y = 0;
            std::shared_ptr<scls::Image> final_image = std::make_shared<scls::Image>(total_width, max_height, style.background_color);
            final_image.get()->paste(image_start.get(), current_x, current_y);
            current_x += image_start.get()->width();
            // Draw each fractions
            for(int i = 0;i<static_cast<int>(images.size());i++){
                current_y = 0;
                final_image.get()->paste(images[i].get(), current_x, current_y);
                current_x += images[i].get()->width();
                // Draw the separation
                if(i < static_cast<int>(images.size()) - 1) {
                    current_x += separation_width;
                }
            }
            final_image.get()->paste(image_end.get(), current_x, current_y);
            return final_image;
        }
        else {
            // For a two dimension matrice
            std::vector<std::vector<std::shared_ptr<scls::Image>>> images = std::vector<std::vector<std::shared_ptr<scls::Image>>>();
            scls::Text_Style style;
            // Create each images
            int number = matrice.dimension().first_dimension();
            int number_dimensions = matrice.dimension().last_dimension();
            std::vector<int> max_height = std::vector<int>(number);
            std::vector<int> max_width = std::vector<int>(number_dimensions);
            for(int i = 0;i<number;i++) {
                // Get each sub-matrices
                std::vector<std::shared_ptr<scls::Image>> current_matrice;
                for(int j = 0;j<number_dimensions;j++) {
                    // Get each images
                    int x = j; int y = i;
                    std::shared_ptr<scls::Image> current_image = generator->image_shared_ptr(matrice.matrice_at(x)->at(y), style);
                    if(max_height[y] < current_image.get()->height()){max_height[y] = current_image.get()->height();}
                    if(max_width[x] < current_image.get()->width()){max_width[x] = current_image.get()->width();}
                    current_matrice.push_back(current_image);
                }
                images.push_back(current_matrice);
            }

            // Create the formating
            int separation_width = style.font_size;

            // Create the final image
            int total_height = 0; for(int i = 0;i<static_cast<int>(max_height.size());i++){total_height+=max_height[i];}
            int total_width = 0; for(int i = 0;i<static_cast<int>(max_width.size());i++){total_width+=max_width[i];}
            total_width += (static_cast<int>(max_width.size()) - 1) * separation_width;
            int current_x = 0; int current_y = 0;
            std::shared_ptr<scls::Image> final_image = std::make_shared<scls::Image>(total_width, total_height, style.background_color);
            //final_image.get()->paste(image_start.get(), current_x, current_y);
            //current_x += image_start.get()->width();
            // Draw each fractions
            for(int i = 0;i<static_cast<int>(images.size());i++){
                for(int j = 0;j<static_cast<int>(images[i].size());j++){
                    int needed_x = current_x + ((max_width[j] / 2) - (images[i][j].get()->width() / 2));
                    final_image.get()->paste(images[i][j].get(), needed_x, current_y);
                    current_x += max_width[j];
                    // Draw the separation
                    if(j < static_cast<int>(images.size()) - 1) {
                        current_x += separation_width;
                    }
                }
                current_x = 0;
                current_y += max_height[i];
            }
            //final_image.get()->paste(image_end.get(), current_x, current_y);
            return final_image;
        }
    };

    //******************
    //
    // The "Matrices_GUI" class
    //
    //******************

    template<typename E = scls::Formula>
    class Matrice_GUI : public scls::GUI_Object {
        // Class representating a GUI way to handle a matrice
    public:
        // Matrice_GUI constructor
        Matrice_GUI(scls::_Window_Advanced_Struct& window, std::string name, scls::GUI_Object* parent):scls::GUI_Object(window, name, parent) {
            set_border_width_in_pixel(1); set_height_in_pixel(200); set_width_in_scale(scls::Fraction(1));

            // Create a title
            a_title = *new_object<scls::GUI_Text>(name + "-title");
            a_title.get()->attach_top_in_parent();
            a_title.get()->set_font_size(40);
            a_title.get()->set_height_in_pixel(40);
            a_title.get()->set_text("Matrice");
            a_title.get()->set_texture_alignment(scls::T_Fit);
            a_title.get()->set_x_in_object_scale(scls::Fraction(1, 2));
            a_title.get()->set_width_in_scale(scls::Fraction(2, 3));

            // Create the first case for the matrice
            std::shared_ptr<scls::GUI_Text_Input> a_current_case = *new_object<scls::GUI_Text_Input>(name + "-case_0_0");
            a_current_case.get()->set_border_width_in_pixel(1);
            a_current_case.get()->set_font_size(30);
            a_current_case.get()->set_height_in_pixel(30);
            a_current_case.get()->set_texture_alignment(scls::T_Fit);
            a_current_case.get()->set_x_in_object_scale(scls::Fraction(1, 2));
            a_current_case.get()->set_width_in_scale(scls::Fraction(1, 15));
            a_cases.push_back(std::vector<std::shared_ptr<scls::GUI_Text_Input>>(1, a_current_case));

            // Create the button to add others cases for the matrice
            // Vertically
            a_add_case_column = *new_object<scls::GUI_Text>(name + "-add_case_vertically");
            a_add_case_column.get()->attach_left_in_parent(10);
            a_add_case_column.get()->set_border_width_in_pixel(1);
            a_add_case_column.get()->overflighted_style()->cursor = GLFW_HAND_CURSOR;
            a_add_case_column.get()->set_font_size(30);
            a_add_case_column.get()->set_height_in_pixel(30);
            a_add_case_column.get()->set_text("Ajouter une colonne");
            a_add_case_column.get()->set_texture_alignment(scls::T_Fit);
            a_add_case_column.get()->set_x_in_object_scale(scls::Fraction(1, 2));
            a_add_case_column.get()->set_width_in_scale(scls::Fraction(1, 5));
            // Horizontally
            a_add_case_line = *new_object<scls::GUI_Text>(name + "-add_case_horizontally");
            a_add_case_line.get()->attach_right_of_object_in_parent(a_add_case_column, 10);
            a_add_case_line.get()->set_border_width_in_pixel(1);
            a_add_case_line.get()->overflighted_style()->cursor = GLFW_HAND_CURSOR;
            a_add_case_line.get()->set_font_size(30);
            a_add_case_line.get()->set_height_in_pixel(30);
            a_add_case_line.get()->set_text("Ajouter une ligne");
            a_add_case_line.get()->set_texture_alignment(scls::T_Fit);
            a_add_case_line.get()->set_x_in_object_scale(scls::Fraction(1, 2));
            a_add_case_line.get()->set_width_in_scale(scls::Fraction(1, 5));
            place_matrices();
        };

        // Add a column of case
        void add_cases_column() {
            std::vector<std::shared_ptr<scls::GUI_Text_Input>> to_add;
            for(int i = 0;i<static_cast<int>(a_cases[0].size());i++) {
                // Create the first case for the matrice
                std::shared_ptr<scls::GUI_Text_Input> a_current_case = *new_object<scls::GUI_Text_Input>(name() + "-case_" + std::to_string(a_cases.size()) + "_" + std::to_string(i));
                a_current_case.get()->set_border_width_in_pixel(1);
                a_current_case.get()->set_font_size(30);
                a_current_case.get()->set_height_in_pixel(30);
                a_current_case.get()->set_texture_alignment(scls::T_Fit);
                a_current_case.get()->set_x_in_object_scale(scls::Fraction(1, 2));
                a_current_case.get()->set_width_in_scale(scls::Fraction(1, 15));
                to_add.push_back(a_current_case);
            } a_cases.push_back(to_add);
            place_matrices();
        };
        // Add a line of case
        void add_cases_line() {
            for(int i = 0;i<static_cast<int>(a_cases.size());i++) {
                // Create the first case for the matrice
                std::shared_ptr<scls::GUI_Text_Input> a_current_case = *new_object<scls::GUI_Text_Input>(name() + "-case_" + std::to_string(i) + "_" + std::to_string(a_cases[i].size()));
                a_current_case.get()->set_border_width_in_pixel(1);
                a_current_case.get()->set_font_size(30);
                a_current_case.get()->set_height_in_pixel(30);
                a_current_case.get()->set_texture_alignment(scls::T_Fit);
                a_current_case.get()->set_x_in_object_scale(scls::Fraction(1, 2));
                a_current_case.get()->set_width_in_scale(scls::Fraction(1, 15));
                a_cases[i].push_back(a_current_case);
            } place_matrices();
        };
        // Place the cases in the matrice
        void place_matrices(){
            // Place the first matrice
            std::shared_ptr<scls::GUI_Text_Input> a_current_case = a_cases[0][0];
            a_current_case.get()->attach_left_in_parent();
            a_current_case.get()->attach_bottom_of_object_in_parent(a_title);
            // Place the matrices column first case
            for(int i = 1;i<static_cast<int>(a_cases.size());i++) {
                a_current_case = a_cases[i][0];
                a_current_case.get()->attach_bottom_of_object_in_parent(a_title);
                a_current_case.get()->attach_right_of_object_in_parent(a_cases[i - 1][0]);
            }
            // Place the matrices line first
            for(int j = 1;j<static_cast<int>(a_cases[0].size());j++) {
                a_current_case = a_cases[0][j];
                a_current_case.get()->attach_bottom_of_object_in_parent(a_cases[0][j - 1]);
                a_current_case.get()->attach_left_in_parent();
            }
            // Place the matrices cases by cases
            for(int i = 1;i<static_cast<int>(a_cases.size());i++) {
                for(int j = 1;j<static_cast<int>(a_cases[i].size());j++) {
                    a_current_case = a_cases[i][j];
                    a_current_case.get()->attach_bottom_of_object_in_parent(a_cases[i][j - 1]);
                    a_current_case.get()->attach_right_of_object_in_parent(a_cases[i - 1][j]);
                }
            }
            // Place the buttons
            a_current_case = a_cases[0][a_cases[0].size() - 1];
            a_add_case_column.get()->attach_bottom_of_object_in_parent(a_current_case, 10);
            a_add_case_line.get()->attach_bottom_of_object_in_parent(a_current_case, 10);
        };
        // Update the events
        virtual void update_event() {
            scls::GUI_Object::update_event();

            // Add a column of case
            if(a_add_case_column.get()->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                add_cases_column();
            }
            // Add a line of case
            if(a_add_case_line.get()->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                add_cases_line();
            }
        };

        // Returns the needed matrice
        Matrice<E> matrice() const {
            Matrice<E> to_return(a_matrice_name, a_cases.size(), a_cases.at(0).size());
            for(int i = 0;i<static_cast<int>(a_cases.size());i++) {
                for(int j = 0;j<static_cast<int>(a_cases[i].size());j++) {
                    to_return.matrice_at(j)->set(i, E::from_std_string(a_cases[i][j].get()->text()));
                }
            }
            return to_return;
        }

        // Getters and setters
        inline void set_name(std::string new_name) {a_matrice_name = new_name;a_title.get()->set_text("Matrice " + new_name);};
        inline void set_name(char new_name) {std::string needed_name;needed_name+=new_name;set_name(needed_name);};
    private:
        // Button to add a column/line of cases int the matrice
        std::shared_ptr<scls::GUI_Text> a_add_case_column;
        std::shared_ptr<scls::GUI_Text> a_add_case_line;
        // Cases for the matrice
        std::vector<std::vector<std::shared_ptr<scls::GUI_Text_Input>>> a_cases;
        // Name of the matrice
        std::string a_matrice_name = "";
        // Title of the object
        std::shared_ptr<scls::GUI_Text> a_title;
    };
}

#endif // PLEOS_MATHEMATICS_MATRICES
