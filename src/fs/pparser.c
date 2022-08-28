#include "pparser.h"
#include "utils/strings/strings.h"
#include "memory/heap/kheap.h"
#include "status.h"
#include "config.h"

/*
This Function returns whether path given is in valid format,
so, len shoul be atleast 3 (0:/) and :/ should be present in the filename
*/
static int pathparser_valid_format(const char* filename)
{
    int len = strnlen(filename, FS_MAX_PATH_LEN);
    return ((len >= 3) && isdigit(filename[0]) && (memcmp((void*)&filename[1],":/",2) == 0));
}

/*returns the drive number from the path given*/
static int pathparser_get_drive_by_path(const char** path)
{
    if(!pathparser_valid_format(*path)){
        return -EBADPATH;
    }
    //gives the drive number
    int drive_no = char_to_digit(*path[0]);

    //Add 3 bytes to skip drive no 0:/ 1:/
    *path +=3;
    return drive_no;
}

/*create path_root struct with drive no set*/
static struct path_root* pathparser_create_root(int drive_no){
    struct path_root* path_root = kzalloc(sizeof(struct path_root));
    path_root->drive_no = drive_no;
    path_root->first = NULL;
    return path_root;
}

/*
It return path_part string 
e.g 0:/test/test.txt
it will return test, then in nexr iteration test.txt
*/
static const char* pathparser_get_path_part(const char** path)
{
    char* result_path_part = kzalloc(FS_MAX_PATH_LEN);
    int i = 0;
    //loop until we get path part or hit the null char
    while((**path != '/') && (**path != '\0')){
        result_path_part[i] = **path;
        *path+=1;
        i++;
    }

    if(**path == '/'){
        //skip it for next iteration
        *path+=1;
    }

    if(i==0){
        //it means there was not path_part in the string so free the mem
        kfree(result_path_part);
        result_path_part = NULL;
    }

    return result_path_part;
}

/*return the path_part struct using the pathparser_get_path_part function*/
static struct path_part* pathparser_parse_path_part(struct path_part* last_part, const char** path)
{
    const char* path_part_str = pathparser_get_path_part(path);
    if(!path_part_str){
        return 0; //there is no path_part
    }

    //create path_part struct if path exist
    struct path_part* part = kzalloc(sizeof(struct path_part));
    part->part = path_part_str;
    part->next = NULL;

    //if last part exist consider link the new part as next part in last part
    if(last_part)
    {
        last_part->next = part;
    }
    return part;
}

/*return path_root struct taking path string as arg*/
struct path_root* pathparser_parse(const char* path, const char* current_directory_path)
{
    int res = 0;
    const char* tmp_path = path;
    struct path_root* path_root = NULL;

    //check if path string is more than max_string_len 
    if(strnlen(path, FS_MAX_PATH_LEN) > FS_MAX_PATH_LEN){
        goto out;
    }
    //get the drive number
    res = pathparser_get_drive_by_path(&tmp_path);
    if(res < 0) goto out;
    //get the root struct
    path_root = pathparser_create_root(res);
    if(!path_root) goto out;
    //get the first part
    struct path_part* first_part = pathparser_parse_path_part(NULL,&tmp_path);
    if(!first_part) goto out;
    //loop for the remaining part
    path_root->first = first_part;
    struct path_part* part = pathparser_parse_path_part(first_part, &tmp_path);
    while(part)
    {
        part = pathparser_parse_path_part(part,&tmp_path);
    }

out:
    return path_root;
}

/*free the path struct*/
void pathparser_free(struct path_root* root)
{
    struct path_part* part = root->first;
    while(part)
    {
        //saving the next
        struct path_part* next_part = part->next;
        kfree((void*)part->part); //clear the string mem allocated
        kfree((void*)part); //clear the mem for part
        part = next_part;
    }

    //clear the root mem
    kfree(root);
}
