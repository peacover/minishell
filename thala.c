#include <stdlib.h>
#include <stdio.h>

size_t getsize(void * p) {
    size_t * in = p;
    if (in) { --in; return *in; }
    return -1;
}

int main()
{
	int *t = malloc(sizeof(int) * 5);
	printf("get size = |%zu|\n", getsize(t));
}