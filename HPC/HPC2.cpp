#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace chrono;

// Bubble Sort (Sequential)
void bubbleSortSequential(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Bubble Sort (Parallel using OpenMP)
void bubbleSortParallel(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        #pragma omp parallel for
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Merge function
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int i = 0; i < n2; ++i)
        R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) {
        arr[k++] = L[i++];
    }

    while (j < n2) {
        arr[k++] = R[j++];
    }
}


// Merge Sort (Sequential)
void mergeSortSequential(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSortSequential(arr, left, mid);
        mergeSortSequential(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Merge Sort (Parallel using OpenMP)
void mergeSortParallel(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSortParallel(arr, left, mid);

            #pragma omp section
            mergeSortParallel(arr, mid + 1, right);
        }

        merge(arr, left, mid, right);
    }
}

// Function to display the array
void displayArray(const vector<int>& arr) {
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << "\n";
}


int main() {
    vector<int> arr;
    int choice;

    do {
        cout << "\nMenu:\n";
        cout << "1. Input and Display\n";
        cout << "2. Sequential and Parallel Bubble Sort\n";
        cout << "3. Sequential and Parallel Merge Sort\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int n;
                cout << "Enter the number of elements: ";
                cin >> n;
                arr.resize(n);
                cout << "Enter the elements: ";
                for (int i = 0; i < n; ++i) {
                    cin >> arr[i];
                }
                cout << "Array: ";
                displayArray(arr);
                break;
            }

            case 2: {
                if (arr.empty()) {
                    cout << "Array is empty. Please input data first.\n";
                    break;
                }

                vector<int> arrSequential = arr;
                vector<int> arrParallel = arr;

                auto start = high_resolution_clock::now();
                bubbleSortSequential(arrSequential);
                auto end = high_resolution_clock::now();
                cout << "Sequential Bubble Sort Time: "
                     << duration<double>(end - start).count() << " seconds\n";

                start = high_resolution_clock::now();
                bubbleSortParallel(arrParallel);
                end = high_resolution_clock::now();
                cout << "Parallel Bubble Sort Time: "
                     << duration<double>(end - start).count() << " seconds\n";

                cout << "Sorted Array: ";
                displayArray(arrParallel);
                break;
            }

            case 3: {
                if (arr.empty()) {
                    cout << "Array is empty. Please input data first.\n";
                    break;
                }

                vector<int> arrSequential = arr;
                vector<int> arrParallel = arr;

                auto start = high_resolution_clock::now();
                mergeSortSequential(arrSequential, 0, arrSequential.size() - 1);
                auto end = high_resolution_clock::now();
                cout << "Sequential Merge Sort Time: "
                     << duration<double>(end - start).count() << " seconds\n";

                start = high_resolution_clock::now();
                mergeSortParallel(arrParallel, 0, arrParallel.size() - 1);
                end = high_resolution_clock::now();
                cout << "Parallel Merge Sort Time: "
                     << duration<double>(end - start).count() << " seconds\n";

                cout << "Sorted Array: ";
                displayArray(arrParallel);
                break;
            }

            case 4:
                cout << "Exiting program.\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);

    return 0;
}




