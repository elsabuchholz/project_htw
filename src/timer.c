#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
 
int main(){
struct timeval start, end;
double delta;

gettimeofday(&start, 0);

sleep(5);

gettimeofday(&end, 0);

delta =((end.tv_sec - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;
printf("time : %f\n",delta);
}
