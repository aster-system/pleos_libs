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
#include "../pleos_it.h"

// The namespace "pleos" is used to simplify the all.
namespace pleos {

    class Probability_Universe_Event {
        // Class representating an element in a probability universe
    public:

        // Struct representating a condition for a probability
        struct Probability_Condition {
            // Probability_Condition constructor
            Probability_Condition();
            Probability_Condition(std::shared_ptr<Probability_Universe_Event>event);

            bool is_equal(Probability_Condition other)const;

            // Returns the parent condition of this one
            Probability_Condition parent_condition();

            // Events for the condition
            inline Probability_Universe_Event* last_event()const{if(events.size() <= 0){return 0;}return events.at(events.size() - 1).get();}
            std::vector<std::shared_ptr<Probability_Universe_Event>> events;
        };

        // Struct representating a partition in a probability universe
        struct Probability_Universe_Partition {
            // Returns an even by its name
            std::shared_ptr<Probability_Universe_Event> event(std::string name);

            // Missing probabilities in the partition
            std::vector<std::shared_ptr<Probability_Universe_Event>> missing_probabilities(Probability_Condition* condition);

            // Directly solve the partition
            int solve(std::string* redaction, Probability_Universe_Partition* other_partition);
            int solve(std::string* redaction, Probability_Condition* condition);
            int solve(std::string* redaction, Probability_Condition condition);

            // Each possibles events
            std::vector<std::shared_ptr<Probability_Universe_Event>> events;
        };typedef Probability_Universe_Partition Probability_Universe_Tribe;

        // Probability_Universe_Event constructor
        Probability_Universe_Event(std::string name, int partition_parent_number, scls::Fraction probability):a_name(name),a_partition_parent_number(partition_parent_number),a_probabilities(std::vector<scls::Fraction>(1, probability)){};
        Probability_Universe_Event(std::string name, int partition_parent_number):Probability_Universe_Event(name, partition_parent_number, scls::Fraction(-1)){};
        Probability_Universe_Event():Probability_Universe_Event(std::string(), -1, 1){};

        // Name of the probability
        std::string name(Probability_Condition* condition) const;
        inline std::string probability_name(Probability_Condition* condition) const {return std::string("P(") + name(condition) + std::string(")");};

        // Getters and setters
        inline std::string name() const {return a_name;};
        inline std::vector<std::shared_ptr<Probability_Universe_Partition>>& partitions() {return a_partitions;};
        inline Probability_Universe_Partition* partition_parent() const {return a_partition_parent.lock().get();};
        inline int partition_parent_number() const {return a_partition_parent_number;};
        inline scls::Fraction probability() const {return a_probabilities.at(0);};
        scls::Fraction probability(Probability_Universe_Event* condition) const;
        scls::Fraction probability(Probability_Condition* condition) const;
        scls::Fraction probability(Probability_Condition condition) const;
        inline void set_partition_parent(std::weak_ptr<Probability_Universe_Partition> new_partition_parent){a_partition_parent=new_partition_parent;};
        inline void set_probability(scls::Fraction new_probability) {if(a_probabilities.size() <= 0){a_probabilities.push_back(new_probability);}else{a_probabilities[0] = new_probability;}};
        void set_probability(std::shared_ptr<Probability_Universe_Event> condition, scls::Fraction new_probability);
        void set_probability(Probability_Condition* condition, scls::Fraction new_probability);
        inline void set_this_event(std::weak_ptr<Probability_Universe_Event> new_this_event) {a_this_event = new_this_event;};
        inline std::shared_ptr<Probability_Universe_Event> this_event() const {return a_this_event.lock();};

    private:
        // Name of the event
        std::string a_name = "";
        // Number of the partition
        std::weak_ptr<Probability_Universe_Partition> a_partition_parent;
        int a_partition_parent_number = -1;
        // Probability of the event
        std::vector<scls::Fraction> a_probabilities = std::vector<scls::Fraction>(1, -1);
        std::vector<Probability_Condition> a_probabilities_condition = std::vector<Probability_Condition>(1, Probability_Condition());
        // This event
        std::weak_ptr<Probability_Universe_Event> a_this_event;

        // Each possibles partitions
        std::vector<std::shared_ptr<Probability_Universe_Partition>> a_partitions;
    };

    class Probability_Universe : public Probability_Universe_Event {
        // Class representating a probability universe
    public:

        // Probability_Universe constructor
        Probability_Universe();

        // Returns a redacted description of this universe
        std::string description();
        // Create a tree of probability
        std::vector<std::shared_ptr<Tree<std::string>>> __tree(Probability_Condition* condition, std::shared_ptr<Tree<std::string>> used_tree, int universe_partition, std::string& redaction);
        std::shared_ptr<Tree<std::string>> tree(int universe_partition_1, int universe_partition_2, std::string& redaction);
        std::shared_ptr<Tree<std::string>> tree(int universe_partition, std::string& redaction);
        std::shared_ptr<Tree<std::string>> tree(std::string& redaction);

        // Solves the universe
        void solve(std::string* redaction);

        // Add an event to the universe
        void add_event(std::string name, scls::Fraction probability, int universe_partition);
        void add_event(std::string name, scls::Fraction probability);
        // Add an event conditionally to the universe
        void add_event_conditionally(std::shared_ptr<Probability_Universe_Event> condition, std::string name, scls::Fraction probability);
        void add_event_conditionally(std::string condition_name, std::string name, scls::Fraction probability);
        // Returns an event by its name
        std::shared_ptr<Probability_Universe_Event> event_shared_ptr(std::string name);
    private:

    };
}

#endif // PLEOS_MATHEMATICS_PROBABILITIES
