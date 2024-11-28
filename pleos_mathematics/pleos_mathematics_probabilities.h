//******************
//
// pleos_mathematics_probabilities.h
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
// This file contains the handling of probabilities in the PLEOS mathematics file.
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

#ifndef PLEOS_MATHEMATICS_PROBABILITIES
#define PLEOS_MATHEMATICS_PROBABILITIES

// Include SCLS Graphic Benoit
#include "../../../scls-graphic-benoit/scls_graphic.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    class Probability_Universe {
        // Class representating a probability universe
    public:
        // Struct representating an element in a probability universe
        struct Probability_Universe_Event {
            // Datas about a condition for this event
            struct Probability_Universe_Event_Condition {
                // Parent event of the known condition
                std::weak_ptr<Probability_Universe_Event> event_parent;
                // Probability of the condition
                scls::Fraction probability = scls::Fraction(-1);
            };

            // Name of the event
            std::string name = "";
            // Number of the partition
            int partition_parent = -1;
            // Probability of the event
            scls::Fraction probability = scls::Fraction(-1);

            // Opposite of the event
            std::shared_ptr<Probability_Universe_Event> __opposed_event;
            inline std::shared_ptr<Probability_Universe_Event> opposed_event() {
                if(opposed_event_as_opposed.lock().get() != 0) {return std::shared_ptr<Probability_Universe_Event>();};
                if(__opposed_event.get() == 0) {
                    __opposed_event = std::make_shared<Probability_Universe_Event>();
                    __opposed_event.get()->name = name;
                    __opposed_event.get()->opposed_event_as_opposed = this_event;
                    __opposed_event.get()->partition_parent = partition_parent;
                    __opposed_event.get()->this_event = __opposed_event;
                } return __opposed_event;
            };
            // Opposite of the event, if this event is the opposite of an event
            std::weak_ptr<Probability_Universe_Event> opposed_event_as_opposed;
            // Returns if the event is opposed or not
            inline bool is_opposed() {return opposed_event().get() == 0;};
            // This event
            std::weak_ptr<Probability_Universe_Event> this_event;

            // If the condition is used for a conditionnal, stores according to which other it is
            std::vector<std::shared_ptr<Probability_Universe_Event>> conditions_from_this_one;
            inline bool contains_conditions_from_this_one(Probability_Universe_Event* test){for(int i=0;i<static_cast<int>(conditions_from_this_one.size());i++){if(conditions_from_this_one[i].get()==test){return true;}}return false;};
            // If the condition is conditionnal, stores according to which other it is
            std::vector<Probability_Universe_Event_Condition> conditions_used;
        };
        // Struct representating a partition in a probability universe
        struct Probability_Universe_Partition {
            // Returns an even by its name
            inline std::shared_ptr<Probability_Universe_Event> event(std::string name) {
                for(int i = 0;i<static_cast<int>(events.size());i++) {
                    if(events[i].get()->name==name) {
                        return events[i];
                    } else if(name.size() > 1 && events[i].get()->name == name.substr(1, name.size()-1) && name[0] == '!') {
                        // Create the opposed event if needed
                        return events[i].get()->opposed_event();
                    }
                } return std::shared_ptr<Probability_Universe_Event>();
            };

            // Each possibles events
            std::vector<std::shared_ptr<Probability_Universe_Event>> events;
        };

        // Class representating a tree of probability universe
        class Probability_Universe_Tree {
        public:
            // Probability_Universe_Tree constructor
            Probability_Universe_Tree(){style.font_size = 50;};

            // Create a case of the tree
            void create_case(std::shared_ptr<Probability_Universe_Event> needed_event);

            // Getters and setters
            inline std::vector<std::shared_ptr<scls::Image>>& cases() {return a_cases;};
            inline scls::Text_Image_Generator& generator() {return a_generator;};
            inline std::vector<std::shared_ptr<Probability_Universe_Event>> other_layer_conditions() {return a_other_layer_conditions;};
        private:
            // Cases of the tree
            std::vector<std::shared_ptr<scls::Image>> a_cases;
            // Conditions for the other layer
            std::vector<std::shared_ptr<Probability_Universe_Event>> a_other_layer_conditions;

            // Datas about the text creation
            scls::Text_Image_Generator a_generator;
            scls::Text_Style style;
        };

        // Probability_Universe constructor
        Probability_Universe(){};

        // Returns a redacted description of this universe
        std::string description();
        // Create a tree of probability
        std::shared_ptr<scls::Image> tree(int universe_partition, std::shared_ptr<Probability_Universe_Event> condition);
        inline std::shared_ptr<scls::Image> tree(){return tree(0, std::shared_ptr<Probability_Universe_Event>());};

        // Add an event to the universe
        void add_event(std::string name, scls::Fraction probability, int universe_partition);
        inline void add_event(std::string name, scls::Fraction probability) {add_event(name, probability, 0);};
        // Add an event conditionally to the universe
        void add_event_conditionally(std::string name, scls::Fraction probability, std::shared_ptr<Probability_Universe_Event> condition);
        inline void add_event_conditionally(std::string name, scls::Fraction probability, std::string condition_name){std::shared_ptr<Probability_Universe_Event> needed_event = a_partitions[0].event(condition_name);if(needed_event.get() != 0) {add_event_conditionally(name, probability, needed_event);}};
    private:
        // Each possibles partitions
        std::vector<Probability_Universe_Partition> a_partitions;
    };
}

#endif // PLEOS_MATHEMATICS_PROBABILITIES
