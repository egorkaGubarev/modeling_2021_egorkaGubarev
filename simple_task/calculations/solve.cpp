#include <array>
#include <fstream>
#include <iostream>
#include <math.h>

typedef unsigned char uchar;
typedef unsigned int uint;
typedef float data;

template <typename t_data>
const uint get_period(const t_data time_end, const t_data step, const uint points_write_amount)
{
    const uint points_amount = time_end / step;
    const uint period = points_amount / points_write_amount;
    const uint period_norm = std::max(period, 1u);
    return period_norm;
}

template <typename t_data, uchar dimension>
void get_array_average(const std::array<t_data, dimension>& array1, const std::array<t_data, dimension>& array2, std::array<t_data, dimension>& result)
{
    for (uchar i = 0; i < dimension; ++ i){
        const t_data element1 = array1[i];
        const t_data element2 = array2[i];
        const t_data sum = element1 + element2;
        const t_data average = sum / 2;
        result[i] = average;
    }
}

template <typename t_data, uchar dimension>
void multiply_array(const std::array<t_data, dimension>& array, const t_data number, std::array<t_data, dimension>& result)
{
    for(uchar i = 0; i < dimension; ++ i){
        const t_data element = array[i];
        const t_data product = element * number;
        result[i] = product;
    }
}

template <typename t_data, uint dimension>
class IEquation
{
    public:
        virtual const t_data get_energy(const std::array<t_data, dimension>& conditions) const = 0;
        virtual void function_(const std::array<t_data, dimension>& conditions, const t_data time, std::array<t_data, dimension>& result) const = 0;
        ~IEquation() = default;
};

template <typename t_data, uchar dimension>
class IMethod
{
    public:
        virtual void make_iterration(const IEquation<t_data, dimension>& equation, const std::array<t_data, dimension>& conditions, const t_data time, const t_data step,
                                     std::array<t_data, dimension>& result)  = 0;
        ~IMethod() = default;
};

template <typename t_data, uchar dimension>
std::ofstream& write(const IEquation<t_data, dimension>& equation, const t_data time, const std::array<t_data, dimension>& conditions, std::ofstream& out)
{
    const t_data x = conditions[0];
    const t_data v = conditions[1];
    const t_data energy = equation.get_energy(conditions);
    out << time << ' ' << x << ' ' << v << ' ' << energy << '\n';
    return out;
}

template <typename t_data>
class Summator
{
    private:
        t_data error_sum_;
    public:
        Summator(): error_sum_{0}
        {

        }

        t_data sum(const t_data big, const t_data small)
        {
            const t_data y = small - error_sum_;
            const t_data t = big + y;
            error_sum_ = (t - big) - y;
            const t_data result = t;
            return result;
        }
};

template <typename t_data, uchar dimension>
void sum_array(const std::array<t_data, dimension>& big, const std::array<t_data, dimension>& small,
               std::array<Summator<t_data>, dimension>& summators, std::array<t_data, dimension>& result)
{
    for(uchar i = 0; i < dimension; ++ i){
        const t_data big_element = big[i];
        const t_data small_element = small[i];
        Summator<t_data>& summator = summators[i];
        const t_data sum = summator.sum(big_element, small_element);
        result[i] = sum;
    }
}

template <typename t_data>
class SimpleEquation: public IEquation<t_data, 2>
{
    private:
        const t_data k_;
    public:
        SimpleEquation(const t_data k): k_{k}
        {

        }

        const t_data get_energy(const std::array<t_data, 2>& conditions) const override
        {
            const t_data coordinate = conditions[0];
            const t_data speed = conditions[1];
            const t_data kinetic = std::pow(speed, 2) / 2;
            const t_data potential = k_ * std::pow(coordinate, 2) / 2;
            const t_data energy = kinetic + potential;
            return energy;
        }

        void function_(const std::array<t_data, 2>& conditions, const t_data time, std::array<t_data, 2>& result) const override
        {
            result[0] = conditions[1];
            result[1] = -1 * k_ * conditions[0];
        }
};

template <typename t_data>
class SinEquation: public IEquation<t_data, 2>
{
    private:
        const t_data k_;

    public:
        SinEquation(const t_data k): k_{k}
        {

        }

        const t_data get_energy(const std::array<t_data, 2>& conditions) const override
        {
            const t_data coordinate = conditions[0];
            const t_data speed = conditions[1];
            const t_data kinetic = std::pow(speed, 2) / 2;
            const t_data potential = -1 * k_ * std::cos(coordinate);
            const t_data energy = kinetic + potential;
            return energy;
        }

        void function_(const std::array<t_data, 2>& conditions, const t_data time, std::array<t_data, 2>& result) const override
        {
            result[0] = conditions[1];
            const t_data sin = std::sin(conditions[0]);
            result[1] = -1 * k_ * sin;
        }
};

template <typename t_data>
class SimpleFrictionEquation: public IEquation<t_data, 2>
{
    private:
        const t_data gamma_, k_;
    public:
        SimpleFrictionEquation(const t_data gamma, const t_data k): gamma_{gamma}, k_{k}
        {

        }

        const t_data get_energy(const std::array<t_data, 2>& conditions) const override
        {
            const t_data coordinate = conditions[0];
            const t_data speed = conditions[1];
            const t_data kinetic = std::pow(speed, 2) / 2;
            const t_data potential = k_ * std::pow(coordinate, 2) / 2;
            const t_data energy = kinetic + potential;
            return energy;
        }

        void function_(const std::array<t_data, 2>& conditions, const t_data time, std::array<t_data, 2>& result) const override
        {
            result[0] = conditions[1];
            result[1] = -1 * (2 * gamma_ * conditions[1] + k_ * conditions[0]);
        }

        void solve_analytically(const t_data speed, const t_data time_end, const t_data step, const std::string& path) const
        {
            const t_data quality = k_ / (2 * gamma_);
            const t_data energy_mass_ratio = std::pow(speed, 2) / 2;
            const t_data amplitude = std::sqrt(2 * energy_mass_ratio / k_);
            t_data time = 0;
            std::ofstream out;
            out.open(path);
            if (gamma_ < k_){
                const data frequency_free = std::sqrt(1 - std::pow(gamma_, 2));
                while (time < time_end){
                    const t_data x = amplitude * std::exp(-gamma_ * time) * std::sin(frequency_free * time);
                    const t_data v = amplitude * std::exp(-gamma_ * time) * (frequency_free * std::cos(frequency_free * time) - gamma_ * std::sin(frequency_free * time));
                    const std::array<t_data, 2> conditions{x, v};
                    write<t_data, 2>(*this, time, conditions, out);
                    time += step;
                }
            }
            else{
                if (gamma_ > k_){
                    const data root = std::sqrt(std::pow(gamma_, 2) - std::pow(k_, 2));
                    const data betta1 = gamma_ - root;
                    const data betta2 = gamma_ + root;
                    const data b = speed / (betta1 - betta2);
                    const data a = -b;
                    while (time < time_end){
                        const t_data x = a * std::exp(-betta1 * time) + b * std::exp(-betta2 * time);
                        const t_data v = -a * betta1 * std::exp(-betta1 * time) - b * betta2 * std::exp(-betta2 * time);
                        const std::array<t_data, 2> conditions{x, v};
                        write<t_data, 2>(*this, time, conditions, out);
                        time += step;
                    }
                }
                else{
                    while (time < time_end){
                        const t_data x = speed * time * std::exp(-gamma_ * time);
                        const t_data v = std::exp(-gamma_ * time) * speed * (1 - gamma_ * time);
                        const std::array<t_data, 2> conditions{x, v};
                        write<t_data, 2>(*this, time, conditions, out);
                        time += step;
                    }
                }
            }
            std::cout << "Quality: " << quality << '\n';
        }
};

template <typename t_data>
class FrictionEquation: public IEquation<t_data, 2>
{
    private:
        const t_data gamma_, k_;
    public:
        FrictionEquation(const t_data gamma, const t_data k): gamma_{gamma}, k_{k}
        {

        }

        const t_data get_energy(const std::array<t_data, 2>& conditions) const override
        {
            const t_data coordinate = conditions[0];
            const t_data speed = conditions[1];
            const t_data kinetic = std::pow(speed, 2) / 2;
            const t_data potential = -1 * k_ * std::cos(coordinate);
            const t_data energy = kinetic + potential;
            return energy;
        }

        void function_(const std::array<t_data, 2>& conditions, const t_data time, std::array<t_data, 2>& result) const override
        {
            result[0] = conditions[1];
            const t_data sin = std::sin(conditions[0]);
            result[1] = -1 * (2 * gamma_ * conditions[1] + k_ * sin);
        }
};

template <typename t_data>
class ForceEquation: public IEquation<t_data, 2>
{
    private:
        const t_data f_, frequency_, gamma_, k_;

        const t_data get_force(const t_data time) const
        {
            const t_data force = f_ * std::sin(frequency_ * time);
            return force;
        }

    public:
        ForceEquation(const t_data f, const t_data frequency, const t_data gamma, const t_data k):f_{f}, frequency_{frequency}, gamma_{gamma}, k_{k}
        {

        }

        const t_data get_energy(const std::array<t_data, 2>& conditions) const override
        {
            const t_data coordinate = conditions[0];
            const t_data speed = conditions[1];
            const t_data kinetic = std::pow(speed, 2) / 2;
            const t_data potential = -1 * k_ * coordinate;
            const t_data energy = kinetic + potential;
            return energy;
        }

        void function_(const std::array<t_data, 2>& conditions, const t_data time, std::array<t_data, 2>& result) const override
        {
            result[0] = conditions[1];
            const t_data force = get_force(time);
            result[1] = force -2 * gamma_ * conditions[1] - k_ * conditions[0];
        }
};


template <typename t_data, uchar dimension>
class EulerMethod: public IMethod<t_data, dimension>
{
    private:
        std::array<t_data, dimension> delta_, derivative_;
        std::array<Summator<t_data>, dimension> summators_;
    public:
        void make_iterration(const IEquation<t_data, dimension>& equation, const std::array<t_data, dimension>& conditions, const t_data time, const t_data step,
                                   std::array<t_data, dimension>& result) override
        {
            equation.function_(conditions, time, derivative_);
            multiply_array<t_data, dimension>(derivative_, step, delta_);
            sum_array<t_data, dimension>(conditions, delta_, summators_, result);
        }
};

template <typename t_data, uchar dimension>
class HeunMethod: public IMethod<t_data, dimension>
{
    private:
        std::array<t_data, dimension> delta_, delta_corrected_, derivative_, derivative_corrected_, derivative_correction_, prediction_;
        std::array<Summator<t_data>, dimension> summatrors_correction_, summators_prediction_;

    public:
        void make_iterration(const IEquation<t_data, dimension>& equation, const std::array<t_data, dimension>& conditions, const t_data time, const t_data step,
                                   std::array<t_data, dimension>& result) override
        {
            equation.function_(conditions, time, derivative_);
            multiply_array<t_data, dimension>(derivative_, step, delta_);
            sum_array<t_data, dimension>(conditions, delta_, summators_prediction_, prediction_);
            equation.function_(prediction_, time, derivative_correction_);
            get_array_average<t_data, dimension>(derivative_, derivative_correction_, derivative_corrected_);
            multiply_array<t_data, dimension>(derivative_corrected_, step, delta_corrected_);
            sum_array<t_data, dimension>(conditions, delta_corrected_, summatrors_correction_, result);
        }
};

template <typename t_data, uchar dimension>
class Solver
{
    private:
        Summator<t_data> summator_;

    public:
        void solve(const IEquation<t_data, dimension>& equation, IMethod<t_data, dimension>& method, std::array<t_data, dimension>& conditions,
                   const t_data time_end, const t_data step, const std::string& path, const uint points_write_amount)
        {
            const uint period = get_period(time_end, step, points_write_amount);
            uint iterration = 0;
            t_data time = 0;
            std::ofstream out;
            out.open(path);
            while(time < time_end){
                method.make_iterration(equation, conditions, time, step, conditions);
                const uint mod = iterration % period;
                if (mod == 0){
                    write<t_data, dimension>(equation, time, conditions, out);
                }
                time = summator_.sum(time, step);
                ++ iterration;
            }
            out.close();
        }
};

int main(const int argc, const char** const argv)
{
    const data coordinate = 1;
    const data force = 1;
    const data force_frequency = 5;
    const data frequency_square_ = 1;
    const data gamma = 0.05;
    const std::string path{"force_equation_heun_method.txt"};
    const std::string path_analytical{"simple_friction_equation_analytical.txt"};
    const uint points_write_amount = 1000;
    const data speed = 0;
    const data step = 0.01;
    const data time_end = 100;
    Solver<data, 2> my_solver;
    const ForceEquation<data> my_equation(force, force_frequency, gamma, frequency_square_);
    HeunMethod<data, 2> my_method;
    std::array<data, 2> conditions{coordinate, speed};
    my_solver.solve(my_equation, my_method, conditions, time_end, step, path, points_write_amount);
    const std::type_info& type = typeid(my_equation);
    const std::string name = type.name();
    if(name == "22SimpleFrictionEquationIfE")
    {
        const uint period = get_period(time_end, step, points_write_amount);
        const data write_step = step * period;
        const IEquation<data, 2>& my_equation_ref = my_equation;
        const SimpleFrictionEquation<data>& my_simple_friction_equation = dynamic_cast<const SimpleFrictionEquation<data>&>(my_equation_ref);
        my_simple_friction_equation.solve_analytically(speed, time_end, write_step, path_analytical);
    }
    return 0;
}
