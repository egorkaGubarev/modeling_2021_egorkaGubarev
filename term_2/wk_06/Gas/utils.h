#ifndef GAS_UTILS_H
#define GAS_UTILS_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <random>
#include <vector>

typedef unsigned int uint;
typedef float type;

template <typename type>
class UniformRandomGenerator
{
public:
    explicit UniformRandomGenerator(long long seed)
    {
        if (seed == -1){
            seed = time(nullptr);
        }
        engine_ = new std::mt19937_64(seed);
    }

    type get_number(type min, type max)
    {
        std::uniform_real_distribution<type> distr(min, max);
        const type number = distr(*engine_);
        return number;
    }

private:
    std::mt19937_64* engine_;
};

template<>
int UniformRandomGenerator<int>::get_number(int min, int max)
{
    std::uniform_int_distribution<int> distr(min, max);
    const int number = distr(*engine_);
    return number;
}

template <typename type>
struct Molecule{
    type mass_, position_, speed_;

    Molecule(): mass_{1}, position_{0}, speed_{0} {};

    Molecule(const type mass, const type position, const type speed): mass_{mass}, position_{position}, speed_{speed} {}
};

template <typename type>
class Gas{
public:
    Gas(const uint amount, const type length, const type mass_1, const type mass_2,
        const type max_speed, const type radius, std::string&&  path):
            amount_{amount}, length_{length}, mass_1_{mass_1}, mass_2_{mass_2}, radius_{radius}, path_{std::move(path)},
            time_{0}, collisions_{0}, first_evaluation_done_{false}
    {
        std::ofstream out;
        out.open(path_);
        out.close();
        p_molecules_ = new std::vector<Molecule<type>>(amount_);
        const int seed = -1;
        const type min_speed = -max_speed;
        UniformRandomGenerator<type> type_generator(seed);
        UniformRandomGenerator<int> int_generator(seed);
        std::vector<Molecule<type>>& molecules = *p_molecules_;
        for(uint i = 0; i < amount_; ++ i){
            const type position = type_generator.get_number(0, length_);
            const type speed = type_generator.get_number(min_speed, max_speed);
            const int mass_type = int_generator.get_number(0, 1);
            Molecule new_molecule(mass_1_, position, speed);
            if(mass_type == 1) {
                new_molecule.mass_ = mass_2_;
            }
            molecules[i] = new_molecule;
        }
        delta_time_ = get_delta_time();
    }

    Gas(const std::vector<type>& masses, const std::vector<type>& positions, const std::vector<type>& speeds,
        const type length, const type radius, std::string&&  path):
            length_{length}, radius_{radius}, path_{std::move(path)}, time_{0}, collisions_{0},
            first_evaluation_done_{false}
    {
        std::ofstream out;
        out.open(path_);
        out.close();
        amount_ = std::size(masses);
        p_molecules_ = new std::vector<Molecule<type>>(amount_);
        std::vector<Molecule<type>>& molecules = *p_molecules_;
        for(uint i = 0; i < amount_; ++ i){
            const type mass = masses[i];
            const type speed = speeds[i];
            const type position = positions[i];
            Molecule new_molecule(mass, position, speed);
            molecules[i] = new_molecule;
        }
        auto begin = std::begin(masses);
        auto min_it = std::min_element(std::begin(masses), std::end(masses));
        auto max_it = std::max_element(std::begin(masses), std::end(masses));
        const uint min_index = std::distance(begin, min_it);
        const uint max_index = std::distance(begin, max_it);
        mass_1_ = masses[min_index];
        mass_2_ = masses[max_index];
        delta_time_ = get_delta_time();
    }

    ~Gas()
    {
        delete p_molecules_;
    }

    [[nodiscard]] type get_energy() const
    {
        type energy_total = 0;
        const std::vector<Molecule<type>>& molecules = *p_molecules_;
        for(const Molecule<type>& molecule: molecules){
            const type energy = get_energy_of_molecule(molecule);
            energy_total += energy;
        }
        return energy_total;
    }

    type get_light_energy() const
    {
        const std::vector<Molecule<type>>& molecules = *p_molecules_;
        type energy_total = 0;
        uint amount = 0;
        for(const Molecule<type>& molecule: molecules){
            if (molecule.mass_ == mass_1_){
                const type energy = get_energy_of_molecule(molecule);
                energy_total += energy;
                ++ amount;
            }
        }
        const type average = energy_total / amount;
        return average;
    }

    [[nodiscard]] uint get_collisions() const
    {
        return collisions_;
    }

    [[nodiscard]] uint get_amount() const
    {
        return amount_;
    }

    type get_mass_1() const
    {
        return mass_1_;
    }

    type get_mass_2() const
    {
        return mass_2_;
    }

    type get_time() const
    {
        return time_;
    }

    void process(const type time)
    {
        const type time_end = time_ + time;
        std::ofstream out;
        out.open(path_, std::ios::app);
        if (! first_evaluation_done_){
            out << *this;
            first_evaluation_done_ = true;
        }
        while(time_ < time_end){
            evaluate();
            reflect();
            collide();
            out << *this;
        }
        out.close();
    }

    [[maybe_unused]] void invert_speeds()
    {
        std::vector<Molecule<type>>& molecules = *p_molecules_;
        for(Molecule<type>& molecule: molecules){
            molecule.speed_ *= -1;
        }
    }

    friend std::ostream& operator<<(std::ostream& out, const Gas<type>& gas)
    {
        const type time = gas.time_;
        const type energy_total = gas.get_energy();
        const type light_energy = gas.get_light_energy();
        out << time << ' ' << energy_total << ' ' << light_energy << ' ';
        const std::vector<Molecule<type>>* const p_molecules = gas.p_molecules_;
        const std::vector<Molecule<type>>& molecules = *p_molecules;
        for(const Molecule<type>& molecule: molecules){
            const type mass = molecule.mass_;
            const type position = molecule.position_;
            const type speed = molecule.speed_;
            const type energy = gas.get_energy_of_molecule(molecule);
            out << mass << ' ' << position << ' ' << speed << ' ' << energy << ' ';
        }
        out << '\n';
        return out;
    }

private:
    static type get_energy_of_molecule(const Molecule<type>& molecule)
    {
        const type energy = molecule.mass_ * std::pow(molecule.speed_, 2) / 2;
        return energy;
    }

    [[nodiscard]] type get_delta_time() const
    {
        // const type energy = get_energy();
        // std::vector<type> masses(amount_);
        // const type mass_min = std::min(mass_1_, mass_2_);
        // const type speed = std::sqrt(2 * energy / mass_min);
        // const type delta_time = radius_ / speed;
        const type delta_time = 0.01;
        return delta_time;
    }

    void evaluate()
    {
        std::vector<Molecule<type>>& molecules = *p_molecules_;
        for(Molecule<type>& molecule: molecules){
            const type speed = molecule.speed_;
            const type delta = speed * delta_time_;
            molecule.position_ += delta;
        }
        time_ += delta_time_;
    }

    void reflect()
    {
        std::vector<Molecule<type>>& molecules = *p_molecules_;
        for(Molecule<type>& molecule: molecules){
            const type position = molecule.position_;
            const type speed = molecule.speed_;
            if(position < radius_ && speed < 0){
                molecule.speed_ = -speed;
                ++ collisions_;
            }
            const type delta_right = length_ - position;
            if(delta_right < radius_ && speed > 0){
                molecule.speed_ = -speed;
                ++ collisions_;
            }
        }
    }

    void collide()
    {
        std::vector<Molecule<type>>& molecules = *p_molecules_;
        for(Molecule<type>& molecule: molecules){
            for(Molecule<type>& other: molecules){
                if (&other != &molecule){
                    const type distance = other.position_ - molecule.position_;
                    const type abs_distance = std::abs(distance);
                    if (abs_distance < 2 * radius_){
                        const type speed = molecule.speed_ - other.speed_;
                        const type position = molecule.position_ - other.position_;
                        const type sign = speed * position;
                        if (sign < 0){
                            change_speed(molecule.mass_, other.mass_, molecule.speed_, other.speed_);
                            ++ collisions_;
                        }
                    }
                }
            }
        }
    }

    void change_speed(const type m1, const type m2, type& v1, type& v2)
    {
        const type p1 = m1 * v1;
        const type p2 = m2 * v2;
        const type p = p1 + p2;
        const type e1 = std::pow(p1, 2) / (2 * m1);
        const type e2 = std::pow(p2, 2) / (2 * m2);
        const type e = e1 + e2;
        const type a = m1 + m2;
        const type b = -2 * p * m1;
        const type c = m1 * (std::pow(p, 2) - 2 * m2 * e);
        const type d = std::pow(b, 2) - 4 * a * c;
        const type p3 = -(b + std::sqrt(d)) / (2 * a);
        const type p4 = p - p3;
        v1 = p3 / m1;
        v2 = p4 / m2;
    }

    const std::string path_;
    const type length_, radius_;
    type delta_time_, mass_1_, mass_2_, time_;
    uint amount_, collisions_;
    bool first_evaluation_done_;
    std::vector<Molecule<type>>* p_molecules_;
};

#endif
