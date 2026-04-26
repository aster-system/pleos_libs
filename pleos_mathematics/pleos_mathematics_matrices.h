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
            else if(dimension().dimension_number() == 1 && dimension() == other.dimension().first_dimension()) {
                // Multiply as a single dimension matrice
                Matrice<E> final_matrice(name() + " * " + other.name(), dimension());
                for(int i = 0;i<static_cast<int>(a_elements.size());i++) {
                    // Add the elements
                    E current_value = 0;
                    for(int j = 0;j<static_cast<int>(a_elements.size());j++){
                        E to_add = (*a_elements[j].get()) * other.matrice_at(i)->at(j);
                        current_value += to_add;
                    }
                    final_matrice.set(i, current_value);
                }
                // Set the final value
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
    std::shared_ptr<scls::__Image_Base> matrice_to_image(Matrice<scls::Fraction> matrice, scls::Text_Image_Generator* generator, scls::Text_Style style);
}

#endif // PLEOS_MATHEMATICS_MATRICES
