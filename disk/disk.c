#include <stdio.h>
// #include <stdlib.h>
#include <Windows.h>

#include <stdint.h>

#define SECTOR_SIZE 512 // Byte

// in: route
// directory entry
// cluster list
// cat to newfile
// compare


// reserve + alloc

void read(BYTE buffer[SECTOR_SIZE], HANDLE disk, int offset_start, int sector_offset/*from 0*/);
uint32_t data_parse(BYTE buffer[SECTOR_SIZE], int byte_n);
void print_buffer(BYTE buffer[SECTOR_SIZE]);


int main() {

    uint8_t sector_per_cluster = 0;
    uint16_t reserved_sector_n = 0;
    uint8_t fat_n = 0;
    uint32_t fat_size = 0;

    HANDLE disk = CreateFile("\\\\.\\F:", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (disk == INVALID_HANDLE_VALUE) 
    {
        printf("Failed to open the disk.\n");
        exit(1);
    }

    BYTE buffer[SECTOR_SIZE] = {0};
    read(buffer, disk, FILE_BEGIN, 0);


    sector_per_cluster = data_parse(buffer + 0x0D, 1);
    reserved_sector_n = data_parse(buffer + 0x0E, 2);
    fat_n = data_parse(buffer + 0x10, 1);
    fat_size = data_parse(buffer + 0x24, 4); // in sector

    int sector_no = reserved_sector_n + fat_n * fat_size;

    read(buffer, disk, FILE_BEGIN, sector_no);

    BYTE directory[32] = {0};
    // print_buffer(buffer);

    CloseHandle(disk);
    return 0;
}

void read(BYTE buffer[SECTOR_SIZE], HANDLE disk, int offset_start, int sector_offset/*from 0*/)
{
    LONG offset = sector_offset * SECTOR_SIZE;
    DWORD bytesRead = 0;

    if (SetFilePointer(disk, offset, NULL, offset_start) == INVALID_SET_FILE_POINTER)
    {
        puts("Err when set pointer");
        exit(1);
    }
    if (!ReadFile(disk, buffer, SECTOR_SIZE, &bytesRead, NULL)) 
    {
        puts("Failed to read from the disk.");
        CloseHandle(disk);
        exit(1);
    }
}

uint32_t data_parse(BYTE *buffer, int byte_n)
{
    uint32_t result = 0;
    for (int i = 0; i < byte_n; i++)
    {
        result += ((uint32_t)buffer[i]) << 8*i;
    }
    return result;
}

void print_buffer(BYTE buffer[SECTOR_SIZE])
{
    for (size_t i = 0; i < SECTOR_SIZE; i++)
    {
        if (i % 0x10 == 0 && i != 0)
        {
            puts("");
        }
        printf("%02X ", buffer[i]); 
    }
    puts("");
}
