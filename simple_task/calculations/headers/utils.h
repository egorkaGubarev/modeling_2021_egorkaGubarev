#ifndef utils_h
#define utils_h
#include <array>
#include <cmath>
#include <fstream>

typedef unsigned int uint;
typedef unsigned char uchar;

template <typename t_data, uint dimension>
class IEquation
{
    public:
        virtual t_data get_force(const t_data time) const = 0;

        virtual t_data get_energy(const std::array<t_data, dimension>& conditions) const = 0;

        virtual void function_(const std::array<t_data, dimension>& conditions, const t_data time, std::array<t_data, dimension>& result) const = 0;

        ~IEquation() = default;
};

template <typename t_data, uint dimension>
class ILinearEquation: public IEquation<t_data, dimension>
{
    public:
        virtual t_data get_force(const t_data time) const = 0;

        t_data get_energy(const std::array<t_data, dimension>& conditions) const
        {
            const t_data kinetic = std::pow(conditions[1], 2) / 2;
            const t_data potential = std::pow(this->frequency_pendulum_, 2) * std::pow(conditions[0], 2) / 2;
            const t_data energy = kinetic + potential;
            return energy;
        }

        void function_(const std::array<t_data, dimension>& conditions, const t_data time, std::array<t_data, dimension>& result) const
        {
            result[0] = conditions[1];
            const t_data force = get_force(time);
            result[1] = force - this->k_friction_cube_ * std::pow(conditions[1], 3) - this->k_friction_linear_ * conditions[1]
                        - std::pow(this->frequency_pendulum_, 2) * conditions[0];
        }

        ~ILinearEquation() = default;
};

template <typename t_data, uint dimension>
class ISinEquation: public IEquation<t_data, dimension>
{
    public:
        virtual t_data get_force(const t_data time) const = 0;

        t_data get_energy(const std::array<t_data, dimension>& conditions) const
        {
            const t_data kinetic = std::pow(conditions[1], 2) / 2;
            const t_data potential = -1 * std::pow(this->frequency_pendulum_, 2) * std::cos(conditions[0]);
            const t_data energy = kinetic + potential;
            return energy;
        }

        void function_(const std::array<t_data, dimension>& conditions, const t_data time, std::array<t_data, dimension>& result) const
        {
            result[0] = conditions[1];
            const t_data force = get_force(time);
            result[1] = force - this->k_friction_cube_ * std::pow(conditions[1], 3) - this->k_friction_linear_ * conditions[1]
                        - std::pow(this->frequency_pendulum_, 2) * std::sin(conditions[0]);
        }

        ~ISinEquation() = default;
};

template <typename t_data, uchar dimension>
class IMethod
{
    public:
        virtual void make_iterration(const std::array<t_data, dimension>& conditions, const IEquation<t_data, dimension>& equation, const t_data time, const t_data step,
                                     std::array<t_data, dimension>& result)  = 0;
        ~IMethod() = default;
};

template <typename t_data>
class Summator
{
    private:
        t_data error_;
    public:
        Summator(): error_{0}
        {

        }

        t_data sum(const t_data big, const t_data small)
        {
            const t_data y = small - error_;
            const t_data t = big + y;
            error_ = (t - big) - y;
            const t_data result = t;
            return result;
        }
};

template <typename t_data, uchar dimension>
void sum(const std::array<t_data, dimension>& big, const std::array<t_data, dimension>& small,
               std::array<Summator<t_data>, dimension>& summators, std::array<t_data, dimension>& result)
{
    for(uchar i = 0; i < dimension; ++ i){
        const t_data sum = summators[i].sum(big[i], small[i]);
        result[i] = sum;
    }
}

template <typename t_data, uchar dimension>
void get_average(const std::array<t_data, dimension>& array1, const std::array<t_data, dimension>& array2, std::array<t_data, dimension>& result)
{
    for (uchar i = 0; i < dimension; ++ i){
        const t_data sum = array1[i] + array2[i];
        const t_data average = sum / 2;
        result[i] = average;
    }
}

template <typename t_data, uchar dimension>
void multiply(const std::array<t_data, dimension>& array, const t_data number, std::array<t_data, dimension>& result)
{
    for(uchar i = 0; i < dimension; ++ i){
        const t_data product = array[i] * number;
        result[i] = product;
    }
}

template <typename t_data>
uint get_period_writing(const t_data time, const t_data step, const uint points_need)
{
    const uint period_min = 1;
    const uint points = time / step;
    const uint period_guess = points / points_need;
    const uint period = std::max(period_guess, period_min);
    return period;
}

template <typename t_data, uchar dimension>
std::ofstream& write(const IEquation<t_data, dimension>& equation, const t_data time, const std::array<t_data, dimension>& conditions, std::ofstream& out)
{
    const t_data energy = equation.get_energy(conditions);
    const t_data force = equation.get_force(time);
    out << time << ' ';
    for(uchar i = 0; i < dimension; ++ i){
        out << conditions[i] << ' ';
    }
    out << energy << ' ' << force << '\n';
    return out;
}

template <typename t_data>
class LinearForceEquation: public ILinearEquation<t_data, 2>
{
    private:
        const t_data frequency_pendulum_, k_friction_linear_, k_friction_cube_, force_, frequency_force_;
    public:
        LinearForceEquation(const t_data frequency_pendulum, const t_data k_friction_linear, const t_data k_friction_cube, const t_data force, const t_data frequency_force):
        frequency_pendulum_{frequency_pendulum}, k_friction_linear_{k_friction_linear}, k_friction_cube_{k_friction_cube}, force_{force}, frequency_force_{frequency_force}
        {

        }

        t_data get_force(const t_data time) const override
        {
            const t_data force = this->force_ / 2 + 2 * this->force_ / 3.14 * (std::sin(this->frequency_force_ * time) + std::sin(this->frequency_force_ * 3 * time) / 3);
            return force;
        }
};

template <typename t_data>
class ForceEquation: public ISinEquation<t_data, 2>
{
    private:
        const t_data frequency_pendulum_, k_friction_linear_, k_friction_cube_, force_, frequency_force_;
    public:
        ForceEquation(const t_data frequency_pendulum, const t_data k_friction_linear, const t_data k_friction_cube, const t_data force, const t_data frequency_force):
        frequency_pendulum_{frequency_pendulum}, k_friction_linear_{k_friction_linear}, k_friction_cube_{k_friction_cube}, force_{force}, frequency_force_{frequency_force}
        {

        }

        t_data get_force(const t_data time) const override
        {
            const t_data force = this->force_ * std::sin(this->frequency_force_ * time);
            return force;
        }
};

template <typename t_data>
class LinearPositiveForceEquation: public ILinearEquation<t_data, 2>
{
    private:
        const t_data frequency_pendulum_, k_friction_linear_, k_friction_cube_, force_, frequency_force_;
    public:
        LinearPositiveForceEquation(const t_data frequency_pendulum, const t_data k_friction_linear, const t_data k_friction_cube, const t_data force, const t_data frequency_force):
        frequency_pendulum_{frequency_pendulum}, k_friction_linear_{k_friction_linear}, k_friction_cube_{k_friction_cube}, force_{force}, frequency_force_{frequency_force}
        {

        }

        t_data get_force(const t_data time) const override
        {
            const t_data force_prediction = this->force_ * std::sin(this->frequency_force_ * time);
            if(force_prediction <= 0){
                return 0;
            }
            else{
                return force_prediction;
            }
        }
};

template <typename t_data>
class PositiveForceEquation: public ISinEquation<t_data, 2>
{
    private:
        const t_data frequency_pendulum_, k_friction_linear_, k_friction_cube_, force_, frequency_force_;
    public:
        PositiveForceEquation(const t_data frequency_pendulum, const t_data k_friction_linear, const t_data k_friction_cube, const t_data force, const t_data frequency_force):
        frequency_pendulum_{frequency_pendulum}, k_friction_linear_{k_friction_linear}, k_friction_cube_{k_friction_cube}, force_{force}, frequency_force_{frequency_force}
        {

        }

        t_data get_force(const t_data time) const override
        {
            const t_data force_prediction = this->force_ * std::sin(this->frequency_force_ * time);
            if(force_prediction <= 0){
                return 0;
            }
            else{
                return force_prediction;
            }
        }
};

template <typename t_data>
class LinearSquareWaveEquation: public ILinearEquation<t_data, 2>
{
    private:
        const t_data frequency_pendulum_, k_friction_linear_, k_friction_cube_, force_, frequency_force_;
    public:
        LinearSquareWaveEquation(const t_data frequency_pendulum, const t_data k_friction_linear, const t_data k_friction_cube, const t_data force, const t_data frequency_force):
        frequency_pendulum_{frequency_pendulum}, k_friction_linear_{k_friction_linear}, k_friction_cube_{k_friction_cube}, force_{force}, frequency_force_{frequency_force}
        {

        }

        t_data get_force(const t_data time) const override
        {
            const t_data force_prediction = this->force_ * std::sin(this->frequency_force_ * time);
            if(force_prediction <= 0){
                return 0;
            }
            else{
                return this->force_;
            }
        }

        void get_solution(const std::array<t_data, 2>& conditions, const t_data time, std::array<t_data, 2>& result)
        {
            const uint overtunes = 1000;
            const t_data f = this->force_;
            const t_data g = this->k_friction_linear_ / 2;
            const t_data w = this->frequency_force_;
            const t_data w0 = this->frequency_pendulum_;
            const t_data root = std::sqrt(std::pow(g, 2) - std::pow(w0, 2));
            const t_data d1 = -1 * g - root;
            const t_data d2 = root - g;
            t_data x = f / (2 * std::pow(w0, 2));
            t_data x0 = x;
            t_data v = 0;
            t_data v0 = v;
            for(uint i = 1; i < overtunes; ++ i){
                const uchar mod = i % 2;
                if(mod == 1){
                    const t_data b = -4 * f * g * w / (3.14 * (std::pow(i * w, 4) + 4 * std::pow(i * g * w, 2) + std::pow(w0, 4) - 2 * std::pow(i * w0 * w, 2)));
                    const t_data a = b * (std::pow(i * w, 2) - std::pow(w0, 2)) / (2 * g * i * w);
                    x += a * std::sin(i * w * time) + b * std::cos(i * w * time);
                    x0 += b;
                    v += a * i * w * std::cos(i * w * time) - b * i * w * std::sin(i * w * time);
                    v0 += a * i * w;
                }
            }
            const t_data c1 = (conditions[1] - conditions[0] * d2 + x0 * d2 - v0) / (d1 - d2);
            const t_data c2 = conditions[0] - c1 - x0;
            result[0] = c1 * std::exp(d1 * time) + c2 * std::exp(d2 * time) + x;
            result[1] = c1 * d1 * std::exp(d1 * time) + c2 * d2 * std::exp(d2 * time) + v;
        }
};

template <typename t_data>
class SquareWaveEquation: public ISinEquation<t_data, 2>
{
    private:
        const t_data frequency_pendulum_, k_friction_linear_, k_friction_cube_, force_, frequency_force_;
    public:
        SquareWaveEquation(const t_data frequency_pendulum, const t_data k_friction_linear, const t_data k_friction_cube, const t_data force, const t_data frequency_force):
        frequency_pendulum_{frequency_pendulum}, k_friction_linear_{k_friction_linear}, k_friction_cube_{k_friction_cube}, force_{force}, frequency_force_{frequency_force}
        {

        }

        t_data get_force(const t_data time) const override
        {
            const t_data force_prediction = this->force_ * std::sin(this->frequency_force_ * time);
            if(force_prediction <= 0){
                return 0;
            }
            else{
                return this->force_;
            }
        }
};

template <typename t_data>
class DoubleEquation: public IEquation<t_data, 4>
{
    private:
        const t_data g_, l1_, l2_, m1_, m2_;
    public:
        DoubleEquation(const t_data g, const t_data m1, const t_data m2, const t_data l1, const t_data l2): g_{g}, m1_{m1}, m2_{m2}, l1_{l1}, l2_{l2}
        {

        }

        t_data get_force(const t_data time) const override
        {
            return 0;
        }

        t_data get_energy(const std::array<t_data, 4>& conditions) const override
        {
            const t_data phi1 = conditions[0];
            const t_data phi2 = conditions[1];
            const t_data w1 = conditions[2];
            const t_data w2 = conditions[3];
            const t_data height1 = l1_ * std::cos(phi1);
            const t_data height2 = height1 + l2_ * std::cos(phi2);
            const t_data potential1 = -1 * m1_ * g_ * height1;
            const t_data potential2 = -1 * m2_ * g_ * height2;
            const t_data potential = potential1 + potential2;
            const t_data v1 = l1_ * w1;
            const t_data kinetic1 = m1_ * std::pow(v1, 2) / 2;
            const t_data kinetic2 = m2_ / 2 * (std::pow(l1_ * w1, 2) + std::pow(l2_ * w2, 2) + 2 * l1_ * l2_ * w1 * w2 * std::cos(phi1 - phi2));
            const t_data kinetic = kinetic1 + kinetic2;
            const t_data energy = potential + kinetic;
            return energy;
        }

        void function_(const std::array<t_data, 4>& conditions, const t_data time, std::array<t_data, 4>& result) const override
        {
            const t_data phi1 = conditions[0];
            const t_data phi2 = conditions[1];
            const t_data w1 = conditions[2];
            const t_data w2 = conditions[3];
            result[0] = w1;
            result[1] = w2;
            result[3] = (std::pow(w2, 2) * m2_ * l2_ * std::sin(phi1 - phi2) * std::cos(phi1 - phi2) + m1_ * g_ * std::sin(phi1) * std::cos(phi1 - phi2) +
                                                                                                      m2_ * g_ * std::sin(phi1) * std::cos(phi1 - phi2) +
                        std::pow(w1, 2) * m1_ * l1_ * std::sin(phi1 - phi2) + std::pow(w1, 2) * m2_ * l1_ * std::sin(phi1 - phi2) - m1_ * g_ * std::sin(phi2) -
                                                                                                                                      m2_ * g_ * std::sin(phi2)) /
                        (l2_ * (m1_ + m2_ - m2_ * std::pow(std::cos(phi1 - phi2), 2)));
            result[2] = (-1 * m2_ * l2_ * result[3] * std::cos(phi1 - phi2) - m2_ * l2_ * std::pow(w2, 2) * std::sin(phi1 - phi2) - m1_ * g_ * std::sin(phi1) -
                                                                                                                                    m2_ * g_ * std::sin(phi1)) /
                        (l1_ * (m1_ + m2_));
        }
};

template <typename t_data, uchar dimension>
class EulerMethod: public IMethod<t_data, dimension>
{
    private:
        std::array<t_data, dimension> derivative_, delta_;
        std::array<Summator<t_data>, dimension> summators_;
    public:
        void make_iterration(const std::array<t_data, dimension>& conditions, const IEquation<t_data, dimension>& equation, const t_data time, const t_data step,
                                   std::array<t_data, dimension>& result) override
        {
            equation.function_(conditions, time, derivative_);
            multiply<t_data, dimension>(derivative_, step, delta_);
            sum<t_data, dimension>(conditions, delta_, summators_, result);
        }
};

template <typename t_data, uchar dimension>
class HeunMethod: public IMethod<t_data, dimension>
{
    private:
        std::array<t_data, dimension> derivative_, delta_, prediction_, derivative_correction_, derivative_corrected_, delta_corrected_;
        std::array<Summator<t_data>, dimension> summators_prediction_, summatrors_correction_;

    public:
        void make_iterration(const std::array<t_data, dimension>& conditions, const IEquation<t_data, dimension>& equation, const t_data time, const t_data step,
                                   std::array<t_data, dimension>& result) override
        {
            equation.function_(conditions, time, derivative_);
            multiply<t_data, dimension>(derivative_, step, delta_);
            sum<t_data, dimension>(conditions, delta_, summators_prediction_, prediction_);
            equation.function_(prediction_, time, derivative_correction_);
            get_average<t_data, dimension>(derivative_, derivative_correction_, derivative_corrected_);
            multiply<t_data, dimension>(derivative_corrected_, step, delta_corrected_);
            sum<t_data, dimension>(conditions, delta_corrected_, summatrors_correction_, result);
        }
};

template <typename t_data, uchar dimension>
class Solver
{
    private:
        Summator<t_data> summator_;

    public:
        void solve(std::array<t_data, dimension>& conditions, const IEquation<t_data, dimension>& equation, IMethod<t_data, dimension>& method,
                   const t_data time, const t_data step, const std::string& path, const uint points)
        {
            const uint period = get_period_writing(time, step, points);
            uint iterration = 0;
            t_data time_current = 0;
            std::ofstream out;
            out.open(path);
            while(time_current < time){
                method.make_iterration(conditions, equation, time_current, step, conditions);
                const uint mod = iterration % period;
                if (mod == 0){
                    write<t_data, dimension>(equation, time_current, conditions, out);
                }
                time_current = summator_.sum(time_current, step);
                ++ iterration;
            }
            out.close();
        }
};

#endif
