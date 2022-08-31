#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <omp.h>

/*
Only addition/deletion of OpenMP directives/methods is allowed. Analyze which for loops have dependencies and need synchronization.
Get rid of unnecessary implicit barriers which will simply wait for threads in the absence of a need for synchronization.
Do not expect local changes such as tweaking environment variables to be simulated while testing the program. 
*/

double sum_of_array(double *array, int size)
{
    double sum = 0.0f;
    for(int i = 0; i < size; ++i)
    {
        sum += array[i];
    }
    return sum;
}

int main()
{
    int m = 10;
    int n = 20;
    int table_a[m];
    double table_b[n];
    double table_c[n];
    int total_num_threads = 0;
    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < m; ++i)
        {
            table_a[i] = 9 - i;
        }
	#pragma omp for
        for (int i = 0; i < n; ++i)
        {
            table_b[i] = (i * i * i) / 3;
        }
	#pragma omp for
        for (int i = n-1; i >= 0; --i)
        {
            table_c[i] = table_b[i]/2;
        }

       #pragma omp single
       {
        printf("You shouldn't be seeing this multiple times\n");

        int index_key = 0;
        for(int i = 0; i < n; ++i)
        {
            if((int)table_c[i] == 0)
            {
                index_key = i;
                break;
            }
        }
       }
        ++total_num_threads;
    }   
    
    assert(total_num_threads == 10);
    assert(ceil(sum_of_array(table_c, 20)) == 6014);
}
    

