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

    class Probability_Universe_Event {
        // Class representating an element in a probability universe
    public:

        // Datas about a condition for this event
        struct Probability_Universe_Event_Condition {
            // Event of the known condition
            std::weak_ptr<Probability_Universe_Event> event;
            // Parent event of the known condition
            std::weak_ptr<Probability_Universe_Event> event_parent;
            // Probability of the condition
            scls::Fraction probability = scls::Fraction(-1);
        };

        // Probability_Universe_Event constructor
        Probability_Universe_Event(std::string name, int partition_parent, scls::Fraction probability):a_name(name),a_probability(probability),a_partition_parent(partition_parent){};
        Probability_Universe_Event(std::string name, int partition_parent):Probability_Universe_Event(name, partition_parent, scls::Fraction(-1)){};

        // Returns the complete name of the probability
        inline std::string complete_name() {std::string to_return = a_name;if(is_opposed()){to_return = std::string("!") + a_name;}return to_return;};

        // Opposite of the event
        std::shared_ptr<Probability_Universe_Event> opposed_event();
        // Returns if the event is opposed or not
        inline bool is_opposed() {return opposed_event().get() == 0;};

        // Add a condition from this one
        inline void add_condition_from_this_one(std::shared_ptr<Probability_Universe_Event> condition) {if(!contains_conditions_from_this_one(condition.get())){a_conditions_from_this_one.push_back(condition);}};
        // If the condition is used for a conditionnal, stores according to which other it is
        inline bool contains_conditions_from_this_one(Probability_Universe_Event* test){for(int i=0;i<static_cast<int>(a_conditions_from_this_one.size());i++){if(a_conditions_from_this_one[i].get()==test){return true;}}return false;};
        // If the condition is conditionnal, stores according to which other it is
        std::shared_ptr<Probability_Universe_Event_Condition> condition_used(std::shared_ptr<Probability_Universe_Event> condition);

        // Getters and setters
        inline std::vector<std::shared_ptr<Probability_Universe_Event>>& conditions_from_this_one(){return a_conditions_from_this_one;};
        inline std::string name() const {return a_name;};
        inline std::shared_ptr<Probability_Universe_Event> opposed_event_as_opposed() const {return a_opposed_event_as_opposed.lock();};
        inline int partition_parent() const {return a_partition_parent;};
        inline scls::Fraction probability() const {return a_probability;};
        inline void set_probability(scls::Fraction new_probability) {a_probability = new_probability;};
        inline void set_this_event(std::weak_ptr<Probability_Universe_Event> new_this_event) {a_this_event = new_this_event;};
        inline std::shared_ptr<Probability_Universe_Event> this_event() const {return a_this_event.lock();};

    private:
        // Name of the event
        std::string a_name = "";
        // Number of the partition
        int a_partition_parent = -1;
        // Probability of the event
        scls::Fraction a_probability = scls::Fraction(-1);
        // This event
        std::weak_ptr<Probability_Universe_Event> a_this_event;

        // Opposite of the event
        std::shared_ptr<Probability_Universe_Event> a_opposed_event;
        // Opposite of the event, if this event is the opposite of an event
        std::weak_ptr<Probability_Universe_Event> a_opposed_event_as_opposed;

        // If the condition is used for a conditionnal, stores according to which other it is
        std::vector<std::shared_ptr<Probability_Universe_Event>> a_conditions_from_this_one;
        // If the condition is conditionnal, stores according to which other it is
        std::vector<std::shared_ptr<Probability_Universe_Event_Condition>> a_conditions_used;
    };

    class Probability_Universe {
        // Class representating a probability universe
    public:
        // Struct representating a partition in a probability universe
        struct Probability_Universe_Partition {
            // Returns an even by its name
            inline std::shared_ptr<Probability_Universe_Event> event(std::string name) {
                for(int i = 0;i<static_cast<int>(events.size());i++) {
                    if(events[i].get()->name() == name) {
                        return events[i];
                    } else if(name.size() > 1 && events[i].get()->name() == name.substr(1, name.size()-1) && name[0] == '!') {
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
            Probability_Universe_Tree(std::string& redaction):a_redaction(redaction){style.font_size = 50;};

            // Create a case of the tree
            std::shared_ptr<scls::Image> create_case(std::shared_ptr<Probability_Universe_Event> needed_event, std::shared_ptr<Probability_Universe_Event> condition);
            // Create the first layer of the tree
            void create_first_layer(std::shared_ptr<Probability_Universe_Event> condition);
            // Create the other layers of the tree
            void create_other_layers();
            // Draw the tree
            std::shared_ptr<scls::Image> image();

            // Getters and setters
            inline std::vector<std::shared_ptr<scls::Image>>& cases() {return a_cases;};
            inline std::vector<int>& differences_y() {return a_differences_y;};
            inline int first_layer_height() const {return a_first_layer_height;};
            inline int first_layer_max_width() const {return a_first_layer_max_width;};
            inline scls::Text_Image_Generator& generator() {return a_generator;};
            inline std::vector<std::shared_ptr<Probability_Universe_Event::Probability_Universe_Event_Condition>>& other_layer_conditions() {return a_other_layer_conditions;};
            inline std::vector<std::shared_ptr<scls::Image>>& other_layers(){return a_other_layers;};
            inline void set_other_layers_height(int new_other_layers_height) {a_other_layers_height = new_other_layers_height;};
            inline std::vector<std::shared_ptr<Probability_Universe_Event>>& used_events(){return a_used_events;};
        private:
            // Cases of the tree
            std::vector<std::shared_ptr<scls::Image>> a_cases;
            // Conditions for the other layer
            std::vector<std::shared_ptr<Probability_Universe_Event::Probability_Universe_Event_Condition>> a_other_layer_conditions;
            // Other layers images
            std::vector<std::shared_ptr<scls::Image>> a_other_layers;
            // Redaction used by the tree
            std::string& a_redaction;
            // Used events in the tree
            std::vector<std::shared_ptr<Probability_Universe_Event>> a_used_events;

            // Differences of Y between each part of the first layer
            std::vector<int> a_differences_y;
            // Height of the first layer
            int a_first_layer_height = 0;
            // Max width of the first layer
            int a_first_layer_max_width = 0;
            // Height of the other layers
            int a_other_layers_height = 0;

            // Datas about the text creation
            scls::Text_Image_Generator a_generator;
            scls::Text_Style style;
        };

        // Probability_Universe constructor
        Probability_Universe(){};

        // Returns a redacted description of this universe
        std::string description();
        // Create a tree of probability
        std::shared_ptr<scls::Image> __tree(int universe_partition, std::shared_ptr<Probability_Universe_Event> condition, std::string& redaction);
        std::shared_ptr<scls::Image> tree(int universe_partition, std::shared_ptr<Probability_Universe_Event> condition, std::string& redaction);
        inline std::shared_ptr<scls::Image> tree(std::string& redaction){return tree(0, std::shared_ptr<Probability_Universe_Event>(), redaction);};

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
