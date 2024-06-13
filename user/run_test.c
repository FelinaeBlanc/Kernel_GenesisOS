#include "test.h"
#include "stdint.h"
#include "stdio.h"


void test_until(int n){
    int t;
    for(int i=1; i<=n; i++){
        t = test_run(i);
        if(t == 0){
            printf("- Test succeed ! %d\n", i);
        } else {
            printf("- Test failed %d\n", i);
        }
    }
}
