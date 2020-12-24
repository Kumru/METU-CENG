#include "fs.h"
#include "ext2_fs/ext2.h"

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


char* next_dir(char **path);


int init_fs(const char *image_path) {
  current_fs = initialize_ext2(image_path);
  current_sb = current_fs->get_superblock(current_fs);
  return !(current_fs && current_sb);
}

struct file *openfile(char *path) {
  struct file *f = malloc(sizeof(struct file));
  f->f_path = malloc(strlen(path) + 1);
  strcpy(f->f_path, path);
  struct dentry *dir = pathwalk(path);
  if (!dir) {
    return NULL;
  }
  f->f_inode = dir->d_inode;
  free(dir);
  if (f->f_inode->f_op->open(f->f_inode, f)) {
    return NULL;
  }
  return f;
}

int closefile(struct file *f) {
  if (f->f_op->release(f->f_inode, f)) {
    printf("Error closing file\n");
  }
  free(f);
  f = NULL;
  return 0;
}

int readfile(struct file *f, char *buf, int size, loffset_t *offset)
{
  while((f->f_inode->i_mode & EXT2_S_IFLNK) == EXT2_S_IFLNK)
  {
    char path_buf[EXT2_NAME_LEN + 1];
    
    struct dentry de = (struct dentry) { .d_inode = f->f_inode };
    f->f_inode->i_op->readlink(&de, path_buf+1, EXT2_NAME_LEN-1);
      path_buf[0] = '/';

    struct file *tmp_f;
    if(path_buf[1] == '/')
      tmp_f = openfile(path_buf + 1);
    else
      tmp_f = openfile(path_buf);
    
    if(!tmp_f) return 0;
    f = tmp_f;
  }
  
  if (*offset >= f->f_inode->i_size) {
    return 0;
  }
  if (*offset + size >= f->f_inode->i_size) {
    size = f->f_inode->i_size - *offset;
  }
  // May add llseek call
  return f->f_op->read(f, buf, size, offset);
}

/**
 * Allocates and returns a new dentry for a given path.
*/
struct dentry *pathwalk(char *path)
{
  char *path_copy = malloc(strlen(path) + 1);
  if(strcmp(path, ".") == 0)
    strcpy(path_copy, "/");
  else
    strcpy(path_copy, path);

  if(path_copy[0] != '/') return NULL;

  struct dentry *next;
  struct dentry *current = malloc(sizeof(struct dentry));
  memcpy(current, current_sb->s_root, sizeof(struct dentry));

  char *remaining_path = path_copy;
  while(1)
  {
    char *dirname = next_dir(&remaining_path);
    if(!dirname) break;

    next = calloc(1, sizeof(struct dentry));
    next->d_name = dirname;
    current->d_inode->i_op->lookup(current->d_inode, next);

    if(!next->d_inode)
    {
      current = NULL;
      break;
    }

    next->d_parent = current;
    current = next;
  }

  free(path_copy);
  return current;
}

int statfile(struct dentry *d_entry, struct kstat *k_stat) {
  return d_entry->d_inode->i_op->getattr(d_entry, k_stat);
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