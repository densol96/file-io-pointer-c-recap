#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        // The program should only accept 1 file name (command + fileName = 2)
        printf("Wrong input! You can only provide ONE filename!\n");
        return 1;
    }

    FILE *filePtr = fopen(argv[1], "r");

    if (filePtr == NULL)
    {
        printf("Unable to open the file in read mode!\n");
        return 1;
    }

    const size_t BLOCKSIZE = 512;
    uint8_t buffer[BLOCKSIZE];
    int jpegCount = 0;
    char filename[8];
    FILE *jpegFile = NULL;

    while (fread(buffer, 1, BLOCKSIZE, filePtr) == 512)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] >= 0xe0 && buffer[3] <= 0xef))
        {
            if (jpegCount > 0)
            {
                fclose(jpegFile);
            }
            sprintf(filename, "%03i.jpg", jpegCount);
            jpegCount++;
            jpegFile = fopen(filename, "w");
            fwrite(buffer, 1, 512, jpegFile);
            fclose(jpegFile);
            jpegFile = fopen(filename, "a");
        }
        else if (jpegCount > 0)
        {
            fwrite(buffer, 1, 512, jpegFile);
        }
    }
}