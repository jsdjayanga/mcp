/* 
 * File:   Utils.cpp
 * Author: jayanga
 * 
 * Created on July 13, 2013, 12:55 AM
 */

#include <Utils.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

Utils::Utils()
{
}

Utils::Utils(const Utils& orig)
{
}

Utils::~Utils()
{
}

float Utils::elapsed_time_msec(struct timespec *begin, struct timespec *end, long *sec, long *nsec)
{
    if (end->tv_nsec < begin->tv_nsec)
    {
        *nsec = 1000000000 - (begin->tv_nsec - end->tv_nsec);
        *sec = end->tv_sec - begin->tv_sec - 1;
    }
    else
    {
        *nsec = end->tv_nsec - begin->tv_nsec;
        *sec = end->tv_sec - begin->tv_sec;
    }
    
    return (float) (*sec) * 1000 + ((float) (*nsec)) / 1000000;
}

void Utils::GetTime(timespec& x)
{
    if (clock_gettime(CLOCK_MONOTONIC, &(x)) < 0)
    {
        perror("clock_gettime( ):");
        exit(EXIT_FAILURE);
    }
}

bool Utils::Compare(double value1, double value2)
{
    return (fabs(value1 - value2) < Utils::_max_defference) ? true : false; 
}

double Utils::AverageValues(std::set<double>& value_set)
{
    double average = 0.0;
    for (set<double>::iterator ite = value_set.begin(); ite != value_set.end(); ite++) 
    {
        average += *ite;
    }
    
    return average / value_set.size();
}