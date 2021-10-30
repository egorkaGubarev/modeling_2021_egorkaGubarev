#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>

typedef unsigned char uchar;
typedef unsigned int uint;

template <typename t_data, uint dimension>
class IEquation
{
    public:
        virtual const t_data get_k() const = 0;
        virtual void function_(const std::array<t_data, dimension>& conditions, std::array<t_data, dimension>& result) const = 0;
        ~IEquation() = default;
};

template <typename t_data, uchar dimension>
class IMethod
{
    public:
        virtual void make_iterration(const IEquation<t_data, dimension>& equation, const std::array<t_data, dimension>& conditions, const t_data step,
                                     std::array<t_data, dimension>& result)  = 0;
        ~IMethod() = default;
};

template <typename t_data>
class SimpleEquation: public IEquation<t_data, 2>
{
    private:
        const t_data k_;

    public:
        SimpleEquation(const t_data k): k_{k}
        {

        }

        const t_data get_k() const override
        {
            return k_;
        }

        void function_(const std::array<t_data, 2>& conditions, std::array<t_data, 2>& result) const override
        {
            result[0] = conditions[1];
            result[1] = -1 * k_ * conditions[0];
        }
};

template <typename t_data, uchar dimension>
class EulerMethod: public IMethod<t_data, dimension>
{
    private:
        std::array<t_data, dimension> tmp_;
    public:
        EulerMethod(): tmp_{}
        {
        }

        void make_iterration(const IEquation<t_data, dimension>& equation, const std::array<t_data, dimension>& conditions, const t_data step,
                                   std::array<t_data, dimension>& result) override
        {
            equation.function_(conditions, tmp_);
            for (uchar i = 0; i < dimension; ++ i){
                result[i] = conditions[i] + tmp_[i] * step;
            }
        }
};

template <typename t_data, uchar dimension>
class Solver
{
    private:
        const uint get_period(const t_data time_end, const t_data step, const uchar points_write_amount)
        {
            const uint points_amount = time_end / step;
            const uint period = points_amount / points_write_amount;
            const uint period_norm = std::max(period, 1u);
            return period_norm;
        }

        std::ofstream& write(const t_data time, const std::array<t_data, dimension>& conditions, const t_data k, std::ofstream& out)
        {
            const t_data x = conditions[0];
            const t_data v = conditions[1];
            const t_data kinetic_energy = std::pow(v, 2) / 2;
            const t_data potential_energy = k * std::pow(x, 2) / 2;
            const t_data energy = kinetic_energy + potential_energy;
            out << time << ' ' << x << ' ' << v << ' ' << energy << '\n';
            return out;
        }

    public:
        void solve(const IEquation<t_data, dimension>& equation, IMethod<t_data, dimension>& method, std::array<t_data, dimension>& conditions,
                   const t_data time_end, const t_data step, const std::string& path, const uchar points_write_amount)
        {
            const uint period = get_period(time_end, step, points_write_amount);
            uint iterration = 0;
            const t_data k = equation.get_k();
            t_data time = 0;
            std::ofstream out;
            out.open(path);
            while(time < time_end){
                method.make_iterration(equation, conditions, step, conditions);
                uint mod = iterration % period;
                if (mod == 0){
                    write(time, conditions, k, out);
                }
                time += step;
                ++ iterration;
            }
            out.close();
        }
};

int main(const int argc, const char** const argv)
{
    Solver<float, 2> my_solver;
    const SimpleEquation<float> my_equation(1);
    EulerMethod<float, 2> my_euler_method;
    std::array<float, 2> conditions{1, 2};
    const float time_end = 10;
    const float step = 0.1;
    const std::string path{"simple_equation_euler_method.txt"};
    const uchar points_write_amount = 100;
    my_solver.solve(my_equation, my_euler_method, conditions, time_end, step, path, points_write_amount);
    return 0;
}
