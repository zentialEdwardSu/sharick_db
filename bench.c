 #include <stdio.h>
#include <time.h>
#include "BP_define.h"
 
int main(int argc, const char * argv[]) {
    int i;
    BPTree T;
    T = Initialize();
    
    clock_t c1 = clock();
    i = 10000000;
    while (i > 0)
        T = Insert(T, i--);
    i = 5000001;
    while (i < 10000000)
        T = Insert(T, i++);
    
    i = 10000000;
    while (i > 100)
        T = Remove(T, i--);
    
    Travel_Node(T);
    Destroy(T);
    
    clock_t c2 = clock();
    
    printf("\nTime： %lus\n",(c2 - c1)/CLOCKS_PER_SEC);
}