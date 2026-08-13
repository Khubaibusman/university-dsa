#include <iostream>
using namespace std;
void sort(int arr[], int n, int i = 0)
{
  
    if (n == 1)
    {
        return;
    }

    if (i < n - 1)
    {
        if (arr[i] > arr[i + 1])
        {
            swap(arr[i], arr[i + 1]);
        }

        sort(arr, n, i + 1);
    }
    else
    {
        sort(arr, n - 1, 0);
    }
}
void print(int arr[],int n){
    if(n==0){
        cout<<endl;
        return;
    }

    print(arr,n-1);
    cout<<arr[n-1]<<" ";
}


int main()
{

    int arr[7] = {4, 2, 7, 4, 9, 6, 1};

    print(arr,7);
    sort(arr, 7);

    cout<<"Printing:\n";
    print(arr,7);

    return 0;
}