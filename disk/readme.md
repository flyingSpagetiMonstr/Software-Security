Run disk.c/disk.exe to read disk and recreate the file. \
Run disk.cmd to run both disk.c and FC(file compare).

Adjust these 2 lines in disk.c to alter filename： 
```c
#define FILE_NAME "WANGQUAN"
#define FILE_SUFFIX "TXT"
```
- match them with the short file name of your own file
- and make sure to capitalize them
