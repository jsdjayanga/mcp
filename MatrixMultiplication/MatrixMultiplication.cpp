/* 
 * File:   MatrixMultiplication.cpp
 * Author: jayangad
 *
 * Created on July 15, 2013, 1:57 PM
 */

#include <cstdlib>
#include <iostream>
#include <math.h>
#include <CommandLineParser.h>
#include <Utils.h>
#include <set>
#include <vector>
#include <sstream>

using namespace std;

double** source_matrix_1 = NULL;
double** source_matrix_2 = NULL;
double** destination_matrix = NULL;
set<double> execution_times;

void InitMatrices(int32_t count)
{
    srand(time(NULL));
    
    source_matrix_1 = new double*[count];
    source_matrix_2 = new double*[count];
    destination_matrix = new double*[count];
    
    for (int32_t j = 0; j < count; j++)
    {
        destination_matrix[j] = new double[count]; 
        source_matrix_1[j] = new double[count];
        source_matrix_2[j] = new double[count];
        
        for (int32_t k = 0; k < count; k++)
        {
            destination_matrix[j][k] = 0;
            source_matrix_1[j][k] = (rand() % 2) + 1;
            source_matrix_2[j][k] = (rand() % 2) + 1;
        }
    }
}

void MultiplyMatrices(int32_t starting_row, int32_t ending_row, int32_t length_of_side)
{
    for (int32_t i = starting_row; i < ending_row; i++)
    {
        for (int32_t j = 0; j < length_of_side; j++)
        {
            for (int32_t k = 0; k < length_of_side; k++)
            {
                destination_matrix[i][j] = destination_matrix[i][j] + (source_matrix_1[i][k] * source_matrix_2[k][j]);
            }
        }
    }
}

void Print(double** matrix, int32_t count)
{
    for (int32_t row = 0; row < count; row++)
    {
        for (int32_t column = 0; column < count; column++)
        {
            cout << matrix[row][column] << " ";
        }
        cout << endl;
    }
}

void MatrixMultiplicationSequential(int32_t count)
{
    struct timespec t0, t1;
    long sec, nsec;
    
    Utils::GetTime(t0);

    MultiplyMatrices(0, count, count);

    Utils::GetTime(t1);
    
    double compute_time = Utils::elapsed_time_msec(&t0, &t1, &sec, &nsec);
    
    cout << "Sequential Compute Time=" << compute_time << endl;
    
    execution_times.insert(compute_time);
    
    //Print(source_matrix_1, count);
    //Print(source_matrix_2, count);
    //Print(destination_matrix, count);
}

struct PositionDetails
{
    int32_t _start_row;
    int32_t _end_row;
    int32_t _count;
    
    PositionDetails() :
    _start_row(0),
    _end_row(0),
    _count(0)
    {
    }
};

void *MatrixMultiplicationParallel(void* value)
{
    PositionDetails* pd = (PositionDetails*) value;
    MultiplyMatrices(pd->_start_row, pd->_end_row, pd->_count);
}

void MatrixMultiplicationParallel(int32_t count)
{
    int32_t threads = atoi(CommandLineParser::GetInstance().GetParameterValue("--threads").c_str());
    PositionDetails* pds = new PositionDetails[threads];
    
    int32_t thread_working_set_size = count / threads;
    int32_t excess_count = count % threads;
    
    int32_t row = 0;
    
    for (int32_t index = 0; index < threads; index++)
    {
        pds[index]._start_row = row;
        pds[index]._count = count;
        
        if (index == 0 && excess_count > 0)
        {
            pds[index]._end_row = row + thread_working_set_size + excess_count;
            row = row + thread_working_set_size + excess_count;
        }
        else
        {
            pds[index]._end_row = row + thread_working_set_size;
            row = row + thread_working_set_size;
        }
    }
    
    struct timespec t0, t1;
    long sec, nsec;
    
    pthread_t* threads_ids = new pthread_t[threads];
    
    Utils::GetTime(t0);
    
    for (int32_t index = 0; index < threads; index++)
    {
        int32_t ret_val = pthread_create(&threads_ids[index], NULL, MatrixMultiplicationParallel, (void *)(&pds[index]));
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
    
    //Print(source_matrix_1, count);
    //Print(source_matrix_2, count);
    //Print(destination_matrix, count);
}

void Validate(int32_t count)
{
    double** temp = new double*[count];
    
    MatrixMultiplicationSequential(count);
    
    for (int row = 0; row < count; row++) 
    {
        temp[row] = new double[count];
        
        for (int column = 0; column < count; column++)
        {
            temp[row][column] = destination_matrix[row][column];
            destination_matrix[row][column] = 0;
        }
    }

    MatrixMultiplicationParallel(count);
    
    for (int row = 0; row < count; row++)
    {
        for (int column = 0; column < count; column++)
        {
            if (Utils::Compare(destination_matrix[row][column], temp[row][column])  == false )
            {
                cout << "Mismatch @ Row :" << row << ", Column=" << column 
                     << " [Expected=" << temp[row][column]
                     << ", Received=" << destination_matrix[row][column] << "]" << endl;

                return;
            }
        }
    }
    
    cout << "Validation complete." << endl;
    
    Print(temp, count);
    Print(destination_matrix, count);
}

void AnalyzePerformance(int32_t count)
{
    vector<int32_t> counts;
    counts.push_back(600);
    counts.push_back(1200);
    counts.push_back(1800);
    
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
            MatrixMultiplicationSequential(count);
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
                MatrixMultiplicationParallel(count);
            }

            parallel_times.push_back(Utils::AverageValues(execution_times));
        }

        execution_times.clear();
        
        cout << "=====================================================" << endl;
        cout << "MatrixMultiplication" << endl;
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
    InitMatrices(count);
    Utils::GetTime(t1);
    
    double init_time = Utils::elapsed_time_msec(&t0, &t1, &sec, &nsec);
    
    cout << "InitTime=" << init_time << endl;
    
    if (mode == "s")
    {
        MatrixMultiplicationSequential(count);
    }
    else if (mode == "p")
    {
        MatrixMultiplicationParallel(count);
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

