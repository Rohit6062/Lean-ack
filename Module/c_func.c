#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
bool p(int* a,int n,int *b,int m,bool* is_checked){
    for(int i=0;i<n;i++){
        is_checked[a[i]] = true;
    }
    for(int i=0;i<m;i++){
        if(!is_checked[b[i]])return false; 
    }
    return (n==m)?false:true;
}
void set(bool* is_checked , int* a, int n , bool x){
    for(int i=0;i<n;i++){
        is_checked[a[i]] = x;
        printf("%d ",is_checked[a[i]]);
    }
    printf("\n");
}
