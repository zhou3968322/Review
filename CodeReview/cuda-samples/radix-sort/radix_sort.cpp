#include <iostream>
#include <vector>

using namespace std;

int getMax(int arr[], int n) {
  int mx = arr[0];
  for (int i = 1; i < n; i++)
    if (arr[i] > mx)
      mx = arr[i];
  return mx;
}

void countSort(int arr[], int n, int bit_base, const int base) {
  vector<int> output(n);
  vector<int> count(base, 0);  // Initialize count vector with 0s

  for (int i = 0; i < n; i++) {
    count[(arr[i] / bit_base) % base]++;
  }
  for (int i = 1; i < base; i++) {
    count[i] += count[i - 1];
  }
  for (int i = n - 1; i >= 0; i--) {
    output[count[(arr[i] / bit_base) % base] - 1] = arr[i];
    count[(arr[i] / bit_base) % base] --;
  }
  for (int i = 0; i < n; i++)
    arr[i] = output[i];

}

void radixsort(int arr[], int n, const int base) {
  int m = getMax(arr, n);
  for (int bit_base = 1; m / bit_base > 0; bit_base *= base) {
    countSort(arr, n, bit_base, base);
  }
}


int main()
{
  int arr[] = { 543, 986, 217, 765, 329, 214124, 124312, 8821, 21,771, 118721, 1261 };
  const int base = 16;
  int n = sizeof(arr) / sizeof(arr[0]);

  // Function Call
  radixsort(arr, n, base);
  for (int i = 0; i < n; i++)
    cout << arr[i] << " ";
}