#include "test.h"
#include "stdint.h"
#include "stdio.h"
#include "builtin.h"

void test_until(int n){
    for(int i=1; i<=n; i++){
        set_color(JAUNE); printf(" Test %d - ", i); set_color(BLANC);
        test_run(i);
    }
}
