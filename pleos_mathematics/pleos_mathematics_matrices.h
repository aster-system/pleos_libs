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
            inline void add_dimension(unsigned int dimension) {a_dimensions.insert(a_dimensions.begin(), dimension);};
            // Returns the number of dimension in the matricce dimension
            inline int dimension_number() const {return a_dimensions.size();};

            // Convert to an integer
            inline operator int() const {return a_dimensions.at(0);};
        private:
            // Each dimensions of the matrice
            std::vector<unsigned int> a_dimensions;
        };

        // Matrice constructor
        Matrice(int dimension):a_elements(std::vector<std::shared_ptr<E>>(dimension)){};
        Matrice(int dimension, int sub_dimension):a_matrices(std::vector<std::shared_ptr<Matrice<E>>>(dimension)){
            for(int i = 0;i<static_cast<int>(a_matrices.size());i++) {
                a_matrices[i] = std::make_shared<Matrice<E>>(sub_dimension);
            }
        };

        // Returns an element in the matrice
        inline E at(int indice) const {return *(a_elements[indice].get());};
        // Returns if thie matrice contains an another matrice
        inline bool contains_matrices() const {return a_matrices.size() > 0;};
        // Returns a matrice in the matrice
        inline Matrice* matrice_at(int indice) const {return a_matrices[indice].get();};
        // Set the value at a certain position
        inline void set(int indice, E element) {a_elements[indice] = std::make_shared<E>(element);};
        // Size of the matrice
        inline int size() const {return a_elements.size();};

        // Returns the dimension of the matrice
        Matrice_Dimension dimension() {
            Matrice_Dimension to_return;
            if(contains_matrices()) {
                to_return = a_matrices[0].get()->dimension();
            } to_return.add_dimension(size());
            return to_return;
        }

        // Multiply this matrice by another matrice
        template<typename F>
        void __multiply(Matrice<F> other) {

        };

        // Access operators
        inline E& operator[](int indice) {return *a_elements[indice].get();};

    private:
        // Elements in the matrice
        std::vector<std::shared_ptr<E>> a_elements;
        // Matrices in the matrice
        std::vector<std::shared_ptr<Matrice>> a_matrices;
    };
}

#endif // PLEOS_MATHEMATICS_MATRICES
