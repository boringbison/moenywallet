#include <iostream>
using namespace std;

int main() {
    int t;
    cin >> t;
    
    while (t--) {
        long long n;
        cin >> n;
        
        // Công thức tính tổng từ 1 đến N: S = N * (N + 1) / 2
        long long sum = n * (n + 1) / 2;
        cout << sum << endl;
    }
    
    return 0;
}
