#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <Windows.h>
#include <assert.h>

#define SECTOR_SIZE 512 // Byte
#define DIRECTORY_SIZE 32 // Byte
#define SHORT_FILENAME_LEN 8 // Byte
#define FAT_ENTRY_SIZE 4 // Byte
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

    HANDLE disk = CreateFile("\\\\.\\F:", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (disk == INVALID_HANDLE_VALUE) 
    {
        printf("Failed to open the disk.\n");
        exit(1);
    }

    BYTE buffer[SECTOR_SIZE] = {0};
    read(buffer, disk, FILE_BEGIN, 0); // read DBR

    uint8_t sector_per_cluster = data_parse(buffer + 0x0D, 1);
    uint16_t reserved_sector_n = data_parse(buffer + 0x0E, 2);
    uint8_t fat_n = data_parse(buffer + 0x10, 1);
    uint32_t fat_size = data_parse(buffer + 0x24, 4); // in sector
    uint32_t root_cluster_no = data_parse(buffer + 0x2C, 4);// cluster_no of directory root

    int directory_sector_no = reserved_sector_n + fat_n * fat_size;

    read(buffer, disk, FILE_BEGIN, directory_sector_no); // read root directory

    BYTE entry[DIRECTORY_SIZE] = {0}; // size of directory entry is 32-byte 
    BYTE filename[SHORT_FILENAME_LEN + 1] = {0};
    for (int i = 0; i < SECTOR_SIZE; i += DIRECTORY_SIZE)
    {
        memcpy(entry, buffer + i, DIRECTORY_SIZE);
        memcpy(filename, entry, SHORT_FILENAME_LEN);
        if (strcmp(filename, "WANGQUAN") == 0)
        {
            puts("File entry found");
            break;
        }
    }

    uint16_t high = data_parse(entry + 0x14, 2);
    uint16_t low = data_parse(entry + 0x1A, 2);
    uint32_t cluster_no = (((uint32_t)high) << (16*8)) + low;
    uint32_t file_size = data_parse(entry + 0x1C, 4); // in Byte

    // assert((cluster_no+1)*FAT_ENTRY_SIZE <= SECTOR_SIZE);
    read(buffer, disk, FILE_BEGIN, reserved_sector_n); // read fat
    print_buffer(buffer);

    while(cluster_no != 0x0FFFFFFF)
    {
        printf("%08X\n", cluster_no);
        assert((cluster_no+1)*FAT_ENTRY_SIZE <= SECTOR_SIZE);
        // root = directory_sector_no
        // sector_offset = sector_per_cluster * cluster_no
        // content_sector = directory_sector_no + sector_per_cluster * cluster_no;
        // read(buffer, disk, FILE_BEGIN, reserved_sector_n); // read fat
        cluster_no = data_parse(buffer + cluster_no*FAT_ENTRY_SIZE, FAT_ENTRY_SIZE);
    }
    printf("%08X\n", cluster_no);

    

    
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
