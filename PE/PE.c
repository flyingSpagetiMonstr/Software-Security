#include <stdio.h>
#include <Windows.h>


typedef WINBASEAPI WINBOOL WINAPI (*io_func)(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
void io(io_func operation, BYTE *buffer, int size/*by Byte*/, HANDLE file, int offset_start/*FILE_BEGIN*/, LONG offset);
//   WINBASEAPI WINBOOL WINAPI ReadFile (HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);


// void read(BYTE *buffer, int size/*by Byte*/, HANDLE file, int offset_start/*FILE_BEGIN*/, LONG offset);

int main()
{
    HANDLE file = CreateFile("AES.exe", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (file == INVALID_HANDLE_VALUE) 
    {
        puts("Failed to open the disk.");
        exit(1);
    }
    IMAGE_DOS_HEADER i_dos_header;
    io(ReadFile, (BYTE*)&i_dos_header, sizeof(i_dos_header), file, FILE_BEGIN, 0);
    IMAGE_NT_HEADERS i_nt_headers;
    io(ReadFile, (BYTE*)&i_nt_headers, sizeof(i_nt_headers), file, FILE_BEGIN, i_dos_header.e_lfanew);
         
    // position of end of IMAGE_SECTION_HEADERs
    int position = i_dos_header.e_lfanew + sizeof(IMAGE_NT_HEADERS) 
        + i_nt_headers.FileHeader.NumberOfSections*sizeof(IMAGE_SECTION_HEADER);

    int space = i_nt_headers.OptionalHeader.FileAlignment 
        - position % i_nt_headers.OptionalHeader.FileAlignment;

    printf("space: %d byte\n", space);
    printf("need: %d byte\n", sizeof(IMAGE_SECTION_HEADER));
    if (space >= sizeof(IMAGE_SECTION_HEADER))
    {
        IMAGE_SECTION_HEADER new_sect_header = {"V header", 0, 0, 0, 0, 0, 0, 0, 0, 0};
    }
    

    // BYTE *ptr = (BYTE*)(&i_nt_headers);
    // for (int i = 0; i < sizeof(i_nt_headers); i++)
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
        puts("Failed to read from the disk.");
        CloseHandle(file);
        exit(1);
    }
}

