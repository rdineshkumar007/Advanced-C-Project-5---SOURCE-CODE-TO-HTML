#include<stdio.h>
int main()
{
    int n;
    printf("Enter array size : ");
    scanf("%d", &n);
    int arr[n];
    printf("ENter array elements : ");
    for(int i =0; i<n; i++)
    {
        scanf("%d", &arr[i]);
    }
    for(int i =0; i<n-1; i++)
    {
        for(int j = 0;  j<n-1; j++)
        {
            if(arr[j] > arr[j+1])
            {
                int t = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = t;
            }
        }
    }
    printf("Array elements after Bubble sorting : ");
    for(int i = 0; i<n; i++)
    {
        printf("%d ", arr[i]);
    }

    return 0;
}
