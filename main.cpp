#include <stdio.h>
#include "omp.h"

int main()
{
    int data_counts = 20;
    int min_meas = 0;
    int max_meas = 5;
    int bin_count = 5;
    float data[20] = { 1.3, 2.9, 0.4, 0.3, 1.3, 4.4, 1.7, 0.4, 3.2, 0.3, 4.9, 2.4, 3.1, 4.4, 3.9, 0.4, 4.2, 4.5, 4.9, 0.9 };

    float bin_width = (max_meas - min_meas)/bin_count;
    float bin_maxes[bin_count];
    float bin_counts[bin_count];

    for (int b=0;b<bin_count;b++){
        bin_maxes[b] = min_meas + bin_width * (b+1);
        bin_counts[b] = 0;

    }

    omp_set_num_threads(data_counts);


#pragma omp parallel for
    for (int i = 0; i < data_counts; i++) {
        int id = omp_get_thread_num();
        if(i == id) {
            //            printf("%d = %d\n",i,id);
            if (data[i] < bin_width && data[i] >= min_meas) {
#pragma omp critical
                {
                    bin_counts[0]++;
                }

            } else {
                for (int b = 1; b < bin_count; b++) {
                    if (data[i] >= bin_maxes[b - 1] && data[i] < bin_maxes[b]) {
#pragma omp critical
                        {
                            bin_counts[b]++;
                        }

                    }
                }
            }
        }
    }

    for(int i=0;i<bin_count;i++){
        printf("%f = %f\n",bin_maxes[i],bin_counts[i]);
    }
    return 0;
}