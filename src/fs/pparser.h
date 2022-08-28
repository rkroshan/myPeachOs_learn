#ifndef __FS_PARSER_H__
#define __FS_PARSER_H__

//structure definitions
struct path_root
{
    int drive_no;
    struct path_part* first;
};

struct path_part
{
    const char* part;
    struct path_part* next;
};

//Function Prototypes
/*return path_root struct taking path string as arg*/
struct path_root* pathparser_parse(const char* path, const char* current_directory_path);
/*free the path struct*/
void pathparser_free(struct path_root* root);

#endif