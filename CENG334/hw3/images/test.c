#include <stdio.h>

#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../ext2_fs/ext2_fs.h"

#define BASE_OFFSET 1024
#define SB_SIZE 1024

char* next_dir(char **path);

int main()
{
	int fd = open("small.img", O_RDONLY);

	lseek(fd, BASE_OFFSET, SEEK_SET);
	struct ext2_super_block sb;
	read(fd, (void*)&sb, SB_SIZE);


	printf("Inodes count: %d\n", sb.s_inodes_count);
	printf("Blocks count: %d\n", sb.s_blocks_count);
	printf("Reserved blocks count: %d\n", sb.s_r_blocks_count);
	printf("Free blocks count: %d\n", sb.s_free_blocks_count);
	printf("Free inodes count: %d\n", sb.s_free_inodes_count);
	printf("First Data Block: %d\n", sb.s_first_data_block);
	printf("blocksize in bytes: %d\n", 1 << (sb.s_log_block_size+10));
	printf("Fragment size: %d\n", 1 << (sb.s_log_frag_size+10));
	printf("Blocks per group: %d\n", sb.s_blocks_per_group);
	printf("Fragments per group: %d\n", sb.s_frags_per_group);
	printf("Inodes per group: %d\n", sb.s_inodes_per_group);

  struct ext2_group_desc egd;
  read(fd, (void*)&egd, 32);

  printf("\n");
  printf("Blocks bitmap block: %d\n", egd.bg_block_bitmap);
  printf("Inodes bitmap block: %d\n", egd.bg_inode_bitmap);
  printf("Inodes table block: %d\n", egd.bg_inode_table);
  printf("Free blocks count: %d\n", egd.bg_free_blocks_count);
  printf("Free inodes count: %d\n", egd.bg_free_inodes_count);

  
  lseek(fd, (egd.bg_inode_table*1024) + sb.s_inode_size, SEEK_SET);
  struct ext2_inode ein;
  read(fd, (void*)&ein, sb.s_inode_size);
  
  printf("\n");
  printf("File mode: %d\n", ein.i_mode);
  printf("Size in bytes: %d\n", ein.i_size);
  printf("1st block pointer: %d\n", ein.i_block[0]);

  lseek(fd, ein.i_block[0] * 1024, SEEK_SET);
  char *root_block = malloc(1024);
  char *placeholder = root_block;
  read(fd, (void*)root_block, 1024);
  
  struct ext2_dir_entry *erde;

  for(int i = 0; i < 3; i++)
  {
    erde = (struct ext2_dir_entry *)root_block;
    printf("\n");
    printf("Inode number: %d\n", erde->inode);
    printf("Directory entry length: %d\n", erde->rec_len);
    printf("Name length: %d\n", erde->name_len);
    printf("File type: %d\n", erde->file_type);
    printf("File name: %s\n", erde->name);
    
    root_block += erde->rec_len;
  }

  close(fd);
  free(placeholder);

  char *path = malloc(13);
  strcpy(path, "/home/config");
  char *tmp_path = path;

  while(1)
  {
    char *a = next_dir(&tmp_path);
    if(!a) break;
    printf("%s\n", a);
    free(a);
  }
  free(path);

	return 0;
}


/**
 * splits path in two.
 * current directory and rest.
 * remember to free result.
*/
char* next_dir(char **path)
{
  if (*(*path) == '\0' || *(*path) != '/')
    return NULL;
  if (strcmp(*path, "/") == 0)
    return NULL;

  unsigned int len = strlen(*path);
  char *next = strchr((*path) + 1, '/');

  char *res;
  if(!next)
  {
    res = malloc(len);
    strcpy(res, (*path)+1);

    (*path) += len;
    return res;
  }

  res = malloc((int)(next - *path));
  
  *next = '\0';
  strcpy(res, (*path)+1);
  *next = '/';

  if(!strcmp(next, "/")) *next = '\0';

  *path = next;
  return res;
}