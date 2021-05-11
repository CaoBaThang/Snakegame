#include<iostream>

using namespace std;

int main()
{
    int n;
    cin >> n;
    int a[100];
    int tong=0;
    int dema=0,demb=0;

    for (int i=0;i<n;i++)
    {
        cin >> a[i];
    }
    for (int i=0;i<n;i++)
    {
        if(a[i]%2!=0) dema++;
        else demb++;

    }
    cout <<dema<< " "<<demb;
    return 0;
}
