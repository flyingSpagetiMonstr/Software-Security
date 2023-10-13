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

int main() {
    
    // uint16_t byte_per_sector = 0;
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


    if (SetFilePointer(disk, 0x0B, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        puts("Err when set pointerQWSKJIHUBDG");
        exit(1);
    }

    // sector_per_cluster = read(disk, FILE_CURRENT, 0, 1);
    // reserved_sector_n = read(disk, FILE_CURRENT, 0, 2); // in sector
    // fat_n = read(disk, FILE_CURRENT, 0, 1);
    // fat_size = read(disk, FILE_BEGIN, 0x24, 4); // in sector

    
    int data_cluster_no = reserved_sector_n + fat_n * fat_size;
    LONG data_offset = data_cluster_no * byte_per_sector;

    printf("%d\n", data_cluster_no);
    printf("%d\n", data_offset);
    system("PAUSE");

    CloseHandle(disk);
    return 0;
}

uint32_t read(BYTE buffer[SECTOR_SIZE], HANDLE disk, int from, int sector_offset/*from 0*/)
{
    LONG offset = sector_offset * SECTOR_SIZE;
    BYTE buffer[SECTOR_SIZE] = 0;
    DWORD bytesRead = 0;

    if (SetFilePointer(disk, offset, NULL, from) == INVALID_SET_FILE_POINTER)
    {
        puts("Err when set pointer");
        exit(1);
    }
    if (!ReadFile(disk, buffer, sizeof(buffer), &bytesRead, NULL)) {
        printf("Failed to read from the disk.\n");
        CloseHandle(disk);
        exit(1);
    }
}
