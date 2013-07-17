/* 
 * File:   PiCalculation.cpp
 * Author: jayangad
 *
 * Created on July 16, 2013, 6:53 AM
 */

#include <cstdlib>
#include <iostream>
#include <CommandLineParser.h>
#include <Utils.h>
#include <math.h>
#include <set>
#include <vector>
#include <sstream>

using namespace std;

uint32_t* thread_hit_results = NULL;
__thread uint32_t thread_seed = 0;
set<double> execution_times;

uint32_t CalculateHits(uint32_t number_of_points)
{
    uint32_t hit_count = 0;
    double x_coordinate = 0.0;
    double y_coordinate = 0.0;
    double distance = 0.0;

    for (uint32_t index = 0; index < number_of_points; index++)
    {
        x_coordinate = ((double) (rand_r(&thread_seed))) / RAND_MAX;
        y_coordinate = ((double) (rand_r(&thread_seed))) / RAND_MAX;
        
        distance = sqrt(pow(x_coordinate, 2) + pow(y_coordinate, 2));

        if (distance < 1)
        {
            hit_count++;
        }
    }

    return hit_count;
}

void CalculatePiSeqential(uint32_t number_of_points)
{
    struct timespec t0, t1;
    long sec, nsec;
    
    Utils::GetTime(t0);
    
    thread_seed = time(NULL);
    uint32_t hit_count = CalculateHits(number_of_points);

    double pi = (4.0 * hit_count) / number_of_points;
    Utils::GetTime(t1);

    double compute_time = Utils::elapsed_time_msec(&t0, &t1, &sec, &nsec);
    
    cout << "Pi=" << pi << endl;
    cout << "Sequential Compute Time=" << compute_time << endl;
    
    execution_times.insert(compute_time);
}

struct WorkingSetDetails
{
    int32_t _number_of_points;
    int32_t _thread_id;
    
    WorkingSetDetails() :
    _number_of_points(0),
    _thread_id(0)
    {
    }
};

void *CalculatePiParallel(void* value)
{
    WorkingSetDetails* pd = (WorkingSetDetails*) value;
    
    thread_seed = pd->_thread_id * time(NULL);
    thread_hit_results[pd->_thread_id] = CalculateHits(pd->_number_of_points);
}

void CalculatePiParallel(uint32_t number_of_points)
{
    int32_t threads_count = atoi(CommandLineParser::GetInstance().GetParameterValue("--threads").c_str());
    WorkingSetDetails* pds = new WorkingSetDetails[threads_count];
    
    int32_t thread_working_set_size = number_of_points / threads_count;
    int32_t excess_count = number_of_points % threads_count;
    
    thread_hit_results = new uint32_t[threads_count];
    pthread_t* threads = new pthread_t[threads_count];
    
    struct timespec t0, t1;
    long sec, nsec;
    
    uint32_t thread_hit_results_sum = 0;
    double pi = 0.0;
    
    for (int32_t index = 0; index < threads_count; index++)
    {
        pds[index]._thread_id = index;
        
        if (index == 0 && excess_count > 0)
        {
            pds[index]._number_of_points = thread_working_set_size + excess_count;
        }
        else
        {
            pds[index]._number_of_points = thread_working_set_size;
        }
    }
    
    Utils::GetTime(t0);
    
    for (int32_t index = 0; index < threads_count; index++)
    {
        int32_t ret_val = pthread_create(&threads[index], NULL, CalculatePiParallel, (void*) &pds[index]);
        if (ret_val != 0)
        {
            cout << "Error creating thread. [ThreadID=" << index + 1 << "]" << endl;
        }
    }
    
    for (int32_t index = 0; index < threads_count; index++)
    {
        pthread_join(threads[index], NULL);
    }

    thread_hit_results_sum = 0;
    for (int32_t index = 0; index < threads_count; index++)
    {
        thread_hit_results_sum += thread_hit_results[index];
    }

    pi = (4.0 * thread_hit_results_sum) / number_of_points;

    Utils::GetTime(t1);
    
    double compute_time = Utils::elapsed_time_msec(&t0, &t1, &sec, &nsec);
    
    cout << "Pi=" << pi << endl;
    cout << "Parallel Compute Time=" << compute_time << endl;
    
    execution_times.insert(compute_time);
}

void AnalyzePerformance()
{
    vector<int32_t> counts;
    counts.push_back(1000);
    counts.push_back(1000);
    counts.push_back(1000);
    
    vector<int32_t> thread_counts;
    thread_counts.push_back(1);
    thread_counts.push_back(2);
    thread_counts.push_back(4);
    thread_counts.push_back(8);
    thread_counts.push_back(16);
    thread_counts.push_back(32);
    
    for (vector<int32_t>::iterator ite = counts.begin(); ite != counts.end(); ite++)
    {
        int32_t count = *ite;
        
        execution_times.clear();

        int32_t iterations = atoi(CommandLineParser::GetInstance().GetParameterValue("--iterations").c_str());

        for (int iteration = 0; iteration < iterations; iteration++)
        {
            CalculatePiSeqential(count);
        }

        double sequntial_time = Utils::AverageValues(execution_times);
        vector<double> parallel_times;
        parallel_times.clear();
        
        for (vector<int32_t>::iterator th_ite = thread_counts.begin(); th_ite != thread_counts.end(); th_ite++)
        {
            int32_t thread_count = *th_ite;
            stringstream ss;
            ss << thread_count;
            
            CommandLineParser::GetInstance().SetParameterValue("--threads", ss.str());
        
            execution_times.clear();

            for (int iteration = 0; iteration < iterations; iteration++)
            {
                CalculatePiParallel(count);
            }

            parallel_times.push_back(Utils::AverageValues(execution_times));
        }

        execution_times.clear();
        
        cout << "=====================================================" << endl;
        cout << "PiCalculation" << endl;
        cout << "Count                  : " << count << endl;
        cout << "Sequential Average Time: " << sequntial_time << endl;
        
        for (int32_t index = 0; index < parallel_times.size(); index++)
        {
            cout << "Parallel Average Time ("<< thread_counts[index] << ") : " << parallel_times[index] << endl;
        }
        cout << "=====================================================" << endl;
    }
}

/*
 * 
 */
int main(int argc, char** argv) 
{
    CommandLineParser::GetInstance().Initialize(argc, argv);
    
    string mode = CommandLineParser::GetInstance().GetParameterValue("--mode");
    int32_t count = atoi(CommandLineParser::GetInstance().GetParameterValue("--count").c_str());
    execution_times.clear();
    
    if (mode == "s")
    {
        CalculatePiSeqential(count);
    }
    else if (mode == "p")
    {
        CalculatePiParallel(count);
    }
    else if (mode == "a")
    {
        AnalyzePerformance();
    }
    else
    {
        cout << "Unknown mode [Mode=" << mode << "]" << endl;
        return -1;
    }
    
    return 0;
}

