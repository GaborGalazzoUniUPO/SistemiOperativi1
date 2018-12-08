#include "../shared/xerrori.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Uso:\n\t%s nomefile_da_leggere\n\n", argv[0]);
        exit(1);
    }

    FILE *fp = xfopen(argv[1], "r");
    long bytes = xfdim(fp);

    int to_read = bytes / sizeof(int);
    int numbers[to_read];

    xfread(numbers, sizeof(int), to_read, fp);

    int sum = 0;
    for (int i = 0; i < to_read; i++)
    {
        printf("numbers[%d] -> %d\n", i, numbers[i]);
        sum += numbers[i];
    }
    printf("\nsomma -> %d\n", sum);
}