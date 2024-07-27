#ifndef DOUBLE_PENDULUM_HPP
#define DOUBLE_PENDULUM_HPP

#include <utility> 
// typedef  xy;

namespace dp {

    struct state {
        std::pair<double, double> theta;
        std::pair<double, double> omega;
    };

    struct system {
        std::pair<double, double> mass;
        std::pair<double, double> length;
    };

    using xy = std::pair<double, double>;
    

    std::pair<double, double> operator+(const std::pair<double, double>& p1, const std::pair<double, double>& p2);

    std::pair<double, double> operator*(const double d, const std::pair<double, double>& p);

    state operator+(const state& s1, const state& s2);

    state operator*(const double d, const state& s);


    state derive(const state& st, const system& ss);

    state rk4(const state& st, const system& ss);

    state advance(const state& st, const system& ss, double time);


    xy first_edge(const state& st, const system& ss);
    xy second_edge(const state& st, const system& ss);
}

#endif