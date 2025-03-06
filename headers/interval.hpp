// interval.hpp
#ifndef INTERVAL_H
#define INTERVAL_H

#include "utils.hpp"

class interval {
public:
    double min, max;

    interval();
    interval(double min, double max);

    double size() const;
    bool contains(double x) const;
    bool surrounds(double x) const;
    double clamp(double x) const;

    static const interval empty, universe;
};

#endif // INTERVAL_H
