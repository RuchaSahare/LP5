#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

void bubbleSort(vector<int> &arr)
{
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void merge(vector<int> &arr, int l, int m, int r) //merge 2 sorted subarrays
{
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            ++i;
        }
        else
        {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

void mergeSort(vector<int> &arr, int l, int r) // Recursive Merge Sort 
{
    if (l < r)
    {
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

void parallelBubbleSort(vector<int> &arr)
{
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i)
    {
        #pragma omp parallel for
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void parallelMergeSort(vector<int> &arr, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, l, m);
            #pragma omp section
            parallelMergeSort(arr, m + 1, r);
        }
        merge(arr, l, m, r);
    }
}

int main()
{
    const int size = 10000;
    vector<int> original(size);
    for (int i = 0; i < size; ++i)
        original[i] = rand() % 1000;

    vector<int> arr;

    // Sequential Bubble Sort
    arr = original;
    double start = omp_get_wtime();
    bubbleSort(arr);
    double end = omp_get_wtime();
    cout << "Sequential Bubble Sort Time: " << (end - start) * 1000 << " ms" << endl;

    // Parallel Bubble Sort
    arr = original;
    start = omp_get_wtime();
    parallelBubbleSort(arr);
    end = omp_get_wtime();
    cout << "Parallel Bubble Sort Time: " << (end - start) * 1000 << " ms" << endl;

    // Sequential Merge Sort
    arr = original;
    start = omp_get_wtime();
    mergeSort(arr, 0, size - 1);
    end = omp_get_wtime();
    cout << "Sequential Merge Sort Time: " << (end - start) * 1000 << " ms" << endl;

    // Parallel Merge Sort
    arr = original;
    start = omp_get_wtime();
    parallelMergeSort(arr, 0, size - 1);
    end = omp_get_wtime();
    cout << "Parallel Merge Sort Time: " << (end - start) * 1000 << " ms" << endl;

    return 0;
}
