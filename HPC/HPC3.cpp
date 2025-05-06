#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace chrono;

int parallelMin(vector<int>& vec) {
    int min_val = vec[0];
    #pragma omp parallel for reduction(min:min_val)
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] < min_val) {
            min_val = vec[i];
        }
    }
    return min_val;
}
int parallelMax(vector<int>& vec) {
    int max_val = vec[0];
    #pragma omp parallel for reduction(max:max_val)
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] > max_val) {
            max_val = vec[i];
        }
    }
    return max_val;
}

int parallelSum(vector<int>& vec) {
    int sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < vec.size(); i++) {
        sum += vec[i];
    }
    return sum;
}

float parallelAverage(vector<int>& vec) {
    int sum = parallelSum(vec);
    return static_cast<float>(sum) / vec.size();
}

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    vector<int> vec(n);
    cout << "Enter the elements: ";
    for (int i = 0; i < n; ++i) {
        cin >> vec[i];
    }

    auto start = high_resolution_clock::now();
    int min_val = parallelMin(vec);
    auto stop = high_resolution_clock::now();
    cout << "Minimum value: " << min_val << " (Time: " << duration_cast<nanoseconds>(stop - start).count() << " nanoseconds)" << endl;

    start = high_resolution_clock::now();
    int max_val = parallelMax(vec);
    stop = high_resolution_clock::now();
    cout << "Maximum value: " << max_val << " (Time: " << duration_cast<nanoseconds>(stop - start).count() << " nanoseconds)" << endl;

    start = high_resolution_clock::now();
    int sum = parallelSum(vec);
    stop = high_resolution_clock::now();
    cout << "Sum of values: " << sum << " (Time: " << duration_cast<nanoseconds>(stop - start).count() << " nanoseconds)" << endl;

    start = high_resolution_clock::now();
    float avg = parallelAverage(vec);
    stop = high_resolution_clock::now();
    cout << "Average of values: " << avg << " (Time: " << duration_cast<nanoseconds>(stop - start).count() << " nanoseconds)" << endl;

    return 0;
}
