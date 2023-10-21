#disk
the code concatenate/display the content of a given file(the filename is given) by reading raw bytes of a FAT32 file system.

Run disk.c/disk.exe to read disk and recreate the file. \
Run disk.cmd to run both disk.c and FC(file compare).

Adjust these 2 lines in disk.c to alter filename： 
```c
#define FILE_NAME "WANGQUAN"
#define FILE_SUFFIX "TXT"
```
- match them with the short file name of your own file
- and make sure to capitalize them

Modify the line "FC output.txt F:\WANGQUAN.txt" in disk.cmd to select the 2 files to compare.
