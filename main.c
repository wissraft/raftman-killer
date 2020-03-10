#include <stdlib.h>

int destroyRaftman();

int main() {
    destroyRaftman();
    return 0;
}

int destroyRaftman() {
    int *p;
    while(1) {
        int inc=1024*1024*sizeof(char);
        p=(int*) calloc(1,inc);
        if(!p) break;
    }
}