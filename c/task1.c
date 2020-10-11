#include <stdio.h>
#include <stdio.h>

int main () {
    /*
     * сколько байт отведено для типов
     */
    printf("short=%lu, int=%lu, long=%lu, float=%lu, double=%lu, long double=%lu\n", 
        sizeof(short),
        sizeof(int),
        sizeof(long),
        sizeof(float),
        sizeof(double),
        sizeof(long double));
    
    /*
     * тип char signed или unsigned
     */
    if((char)-1 < 0) {
        puts("char is signed");
    } else {
        puts("char is unsigned");
    }
    return 0;
}
