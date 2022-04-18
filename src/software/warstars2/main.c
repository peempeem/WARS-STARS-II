#include <stdio.h>
#include <unistd.h>
//#include "vga.h"

int main() {
    int loop = 0;
    while (1) {
    	printf("loop %d\n", loop++);
        usleep(100000);

    }
    
    return 0;
}
