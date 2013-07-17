/* 
 * File:   VectorDotProduct.cpp
 * Author: jayangad
 *
 * Created on July 14, 2013, 7:06 PM
 */

#include <cstdlib>
#include <iostream>
#include <CommandLineParser.h>
#include <Utils.h>
#include <stdint.h>
#include <math.h>
#include <set>
#include <vector>
#include <sstream>

using namespace std;

double* source_vector_1 = NULL;
double* source_vector_2 = NULL;
double* destination_vector = NULL;
set<double> execution_times;

void InitVectors(int32_t count)
{
    srand(time(NULL));
    
    source_vector_1 = new double[count];
    source_vector_2 = new double[count];
    destination_vector = new double[count];
    
    cout << count << endl;
    
    for (int32_t index = 0; index < count; index++)
    {
        destination_vector[index] = 0.0;
        source_vector_1[index] = ((double)rand() / RAND_MAX) + 1;
        source_vector_2[index] = ((double)rand() / RAND_MAX) + 1;
        
        if (source_vector_1[index] < 1.0 || source_vector_1[index] > 2.0 ||
                source_vector_2[index] < 1.0 || source_vector_2[index] > 2.0)
        {
            cout << "+++++++:" << index << "|" << source_vector_1[index] << "|" << source_vector_2[index] << endl;
            exit(0);
        }
    }
    
    //cout << "===============:" << count << endl;
}

void DotProduct(int32_t start, int32_t stop)
{
    for (int32_t index = start; index < stop; index++)
    {
        //cout << "===========:" << index<< "|" << source_vector_1[index] << "|" << source_vector_2[index] << endl;
        destination_vector[index] = source_vector_1[index] * source_vector_2[index];
    }
}

void VectorDotProductSequential(int32_t count)
{ 
    struct timespec t0, t1;
    long sec, nsec;
    
    Utils::GetTime(t0);

    DotProduct(0, count);

    Utils::GetTime(t1);
    
    double compute_time = Utils::elapsed_time_msec(&t0, &t1, &sec, &nsec);
    
    cout << "Sequential Compute Time=" << compute_time << endl;
    
    execution_times.insert(compute_time);
}

struct PositionDetails
{
    int32_t _start;
    int32_t _end;
    
    PositionDetails() :
    _start(0),
    _end(0)
    {
    }
};

void *VectorDotProductParallel(void* value)
{
    PositionDetails* pd = (PositionDetails*) value;
    DotProduct(pd->_start, pd->_end);
}

void VectorDotProductParallel(int32_t count)
{
    int32_t threads = atoi(CommandLineParser::GetInstance().GetParameterValue("--threads").c_str());
    PositionDetails* pds = new PositionDetails[threads];
    
    int32_t thread_working_set_size = count / threads;
    int32_t excess_count = count % threads;
    
    int32_t vector_index = 0;
    
    for (int32_t index = 0; index < threads; index++)
    {
        pds[index]._start = vector_index;
        
        if (index == 0 && excess_count > 0)
        {
            pds[index]._end = vector_index + thread_working_set_size + excess_count;
            vector_index = vector_index + thread_working_set_size + excess_count;
        }
        else
        {
            pds[index]._end = vector_index + thread_working_set_size;
            vector_index = vector_index + thread_working_set_size;
        }
    }
       
    struct timespec t0, t1;
    long sec, nsec;
    
    pthread_t* threads_ids = new pthread_t[threads];
    
    Utils::GetTime(t0);
    
    for (int32_t index = 0; index < threads; index++)
    {
        int32_t ret_val = pthread_create(&threads_ids[index], NULL, VectorDotProductParallel, (void *)(&pds[index]));
        if (ret_val != 0)
        {
            cout << "Error creating thread. [ThreadID=" << index + 1 << "]" << endl;
        }
    }
    
    for (int32_t index = 0; index < threads; index++)
    {
        pthread_join(threads_ids[index], NULL);
    }
    
    Utils::GetTime(t1);
    
    double compute_time = Utils::elapsed_time_msec(&t0, &t1, &sec, &nsec);
    
    cout << "Parallel Compute Time=" << compute_time << endl;
    
    execution_times.insert(compute_time);
}

void Validate(int32_t count)
{
    double* temp = new double[count];
    
    VectorDotProductSequential(count);
    
    for (int index = 0; index < count; index++) 
    {
        temp[index] = destination_vector[index];
        destination_vector[index] = 0;
    }

    VectorDotProductParallel(count);
    
    for (int index = 0; index < count; index++)
    {
        if (Utils::Compare(destination_vector[index], temp[index])  == false )
        {
            cout << "Mismatch @ index :" << index 
                 << " [Expected=" << temp[index] 
                 << ", Received=" << destination_vector[index] << "]" << endl;
            
            return;
        }
    }
    
    cout << "Validation complete." << endl;
}

void AnalyzePerformance(int32_t count)
{
    vector<int32_t> counts;
    counts.push_back(100000000);
    counts.push_back(500000000);
    counts.push_back(1000000000);
    
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
            VectorDotProductSequential(count);
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
                VectorDotProductParallel(count);
            }

            parallel_times.push_back(Utils::AverageValues(execution_times));
        }

        execution_times.clear();
        
        cout << "=====================================================" << endl;
        cout << "VectorDorProduct" << endl;
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
    
    struct timespec t0, t1;
    long sec, nsec;
    
    Utils::GetTime(t0);
    InitVectors(count);
    Utils::GetTime(t1);
    
    double init_time = Utils::elapsed_time_msec(&t0, &t1, &sec, &nsec);
    
    cout << "InitTime=" << init_time << endl;
    
    if (mode == "s")
    {
        VectorDotProductSequential(count);
    }
    else if (mode == "p")
    {
        VectorDotProductParallel(count);
    }
    else if (mode == "v")
    {
        Validate(count);
    }
    else if (mode == "a")
    {
        AnalyzePerformance(count);
    }
    else
    {
        cout << "Unknown mode [Mode=" << mode << "]" << endl;
        return -1;
    }
    
    return 0;
}

