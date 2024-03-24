#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>

std::mutex merge_mutex;

void merge(int* arr, int l, int m, int r) {
    int nl = m - l + 1;
    int nr = r - m;

    
    std::vector<int> left(nl), right(nr);

    
    for (int i = 0; i < nl; i++)
        left[i] = arr[l + i];
    for (int j = 0; j < nr; j++)
        right[j] = arr[m + 1 + j];

    int i = 0, j = 0;
    int k = l;  

    while (i < nl && j < nr) {
      
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        }
        else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }
 
    while (i < nl) {
        arr[k] = left[i];
        i++;
        k++;
    }
    
    while (j < nr) {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void merge_sort_parallel(int* arr, int l, int r, int max_threads) {
    if (l < r) {
        if (max_threads == 1 || r - l < 1000) { 
            merge_sort_parallel(arr, l, (l + r) / 2, 1);
            merge_sort_parallel(arr, (l + r) / 2 + 1, r, 1);
        }
        else { 
            int m = (l + r) / 2;

            
            std::vector<std::thread> threads;
            threads.reserve(2);
            threads.emplace_back(merge_sort_parallel, arr, l, m, max_threads / 2);
            threads.emplace_back(merge_sort_parallel, arr, m + 1, r, max_threads / 2);
            for (auto& thread : threads)
                thread.join();

            merge(arr, l, m, r);
        }
    }
}

void merge_sort(int* arr, int size, int max_threads) {
    merge_sort_parallel(arr, 0, size - 1, max_threads);
}

int main() {
    int arr[] = { 12, 11, 13, 5, 6, 7 };
    int arr_size = sizeof(arr) / sizeof(arr[0]);
    int max_threads = std::thread::hardware_concurrency(); 

    merge_sort(arr, arr_size, max_threads);

    std::cout << "Sorted array: ";
    for (int i = 0; i < arr_size; i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;

    return 0;
}
