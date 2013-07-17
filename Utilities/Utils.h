/* 
 * File:   Utils.h
 * Author: jayanga
 *
 * Created on July 13, 2013, 12:55 AM
 */

#pragma once

#include <time.h>
#include <set>

class Utils
{
public:
    Utils();
    Utils(const Utils& orig);
    virtual ~Utils();
    
    static const double _max_defference = 0.01;

    static float elapsed_time_msec(struct timespec *begin, struct timespec *end, long *sec, long *nsec);
    static void GetTime(timespec& x);
    static bool Compare(double value1, double value2);
    static double AverageValues(std::set<double>& value_set);
private:

};


