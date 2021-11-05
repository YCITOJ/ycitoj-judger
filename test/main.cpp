#include <iostream>
#include <vector>
using namespace std;
int main()
{
    int a, b;
    vector<int> vec;
    for(int i=1;i<=10000000;i++)
    {
        vec.push_back(i)
    }
    cin >> a >> b;
    cout << a + b << endl;
}