#include <iostream>
#include <vector>
using namespace std;
const int KB = 1024;
const int MB = 1024 * KB;
const int maxn=100*MB;
char A[maxn];
int main()
{
    int a, b;
    for (int i = 1; i < maxn;i++)
        A[i] = i;
    for (int i = 1; i < maxn;i++)
        a += A[i];
    cout << a << "\n";
    cin >> a >> b;
    cout << a + b << endl;
    return 0;
}