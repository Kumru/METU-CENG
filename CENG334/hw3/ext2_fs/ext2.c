#include "ext2.h"

#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

#define BASE_OFFSET 1024 /* location of the superblock in the first group */
#define SB_SIZE sizeof(struct ext2_super_block)
#define GD_SIZE sizeof(struct ext2_group_desc)

#define MIN(x, y) ((y) < (x) ? (y) : (x))

struct super_operations s_op;
struct inode_operations i_op;
struct file_operations f_op;

char fs_name[] = "ext2";


/** UTILITIES **/
uint read_block_to_buffer(struct inode*, uint,
    unsigned char*, struct super_block *);
unsigned long search_in_directory_buffer(unsigned long, unsigned char *, char *);
int callback_dentries_in_directory(unsigned long, unsigned char *, filldir_t);
int help_readlink(struct inode* in, char* buf, int len);


/* F_OP */

/**
 * reposition the offset of the file o bytes relative to the beginning,
 * the current offset, or the end depending on whence.
 * SEEK_SET, SEEK_CUR, or SEEK_END, respectively.
 * It returns the resulting file position in the argument result.
*/
loffset_t fop_llseek(struct file *f, loffset_t o, int whence)
{
  loffset_t new_pos;

  switch (whence)
  {
    case SEEK_SET:
      new_pos = o;
      break;
    case SEEK_CUR:
      new_pos = f->f_pos + o;
      break;
    case SEEK_END:
      new_pos = f->f_inode->i_size + o;
      break;
    default:
      new_pos = -1;
      break;
  }

  if(new_pos < 0)
    new_pos = 0;
  else if(new_pos > f->f_inode->i_size)
    new_pos = f->f_inode->i_size;
  
  f->f_pos = new_pos;

  return new_pos;
}

/**
 * read the contents of file into the buffer. start from the o byte
 * and read len bytes. It returns the number of bytes read.
*/
ssize_t fop_read(struct file *f, char *buf, size_t len, loffset_t *o)
{
  if(!S_ISREG(f->f_inode->i_mode))
    return 0;
  
  struct super_block *sb = current_sb;

  if(o){
    f->f_op->llseek(f, *o, SEEK_SET);
  }

  u_char *tmp_buf = malloc(sb->s_blocksize);

  loffset_t current_file_block;
  loffset_t current_block_offset;

  size_t current_read_offset = 0;
  while(current_read_offset < len)
  {
    current_file_block   = f->f_pos / sb->s_blocksize;
    current_block_offset = f->f_pos % sb->s_blocksize;

    
    read_block_to_buffer(f->f_inode, current_file_block, tmp_buf, sb);

    size_t len_read_from_block =
        MIN(len - current_read_offset, sb->s_blocksize - current_block_offset);

    memcpy(buf+current_read_offset,
        tmp_buf+current_block_offset, len_read_from_block);

    f->f_op->llseek(f, len_read_from_block, SEEK_CUR);

    current_read_offset += len_read_from_block;
  }

  free(tmp_buf);
  return len;
}

/**
 * read the file on inode and fill the file struct.
 * It returns zero for successful operation.
*/
int fop_open(struct inode *in, struct file *f)
{
  *f = (struct file)
  {
    .f_path = f->f_path,
    .f_inode = in,
    .f_op = &f_op,
    .f_flags = in->i_flags,
    .f_mode = in->i_mode,
    .f_pos = 0
  };

  return 0;
}

/**
 * called when a file is closed. perform clean up operations if necessary.
 * Returns zero for successful operation.
*/
int fop_release(struct inode *in, struct file *f)
{
  if(f->f_path)
    free(f->f_path);

  f->f_path = NULL;

  return 0;
}


/* S_OP */

/**
 * assume only the inode number (i_ino) of  inode is valid.
 * populate the remaining fields of i.
*/
void sop_read_inode(struct inode *in)
{
  int inum = in->i_ino - 1; // inodes start with 1
  struct super_block *sb = current_sb;
  int group  = inum / sb->s_inodes_per_group;
  int offset = inum % sb->s_inodes_per_group;

  // check if inode valid

  struct ext2_group_desc gd;
  lseek(myfs.file_descriptor,
      BASE_OFFSET + SB_SIZE + (GD_SIZE * group),
      SEEK_SET);
  read(myfs.file_descriptor, (void*)&gd, GD_SIZE);

  lseek(myfs.file_descriptor,
      (gd.bg_inode_table * sb->s_blocksize) + (sb->s_inode_size * offset),
      SEEK_SET);

  struct ext2_inode inode;
  read(myfs.file_descriptor, (void*)&inode, sb->s_inode_size);

  *in = (struct inode)
  {
    .i_ino = in->i_ino,
    .i_mode = inode.i_mode,
    .i_nlink = inode.i_links_count,
    .i_uid = inode.i_uid,
    .i_gid = inode.i_gid,
    .i_size = inode.i_size,
    .i_atime = inode.i_atime,
    .i_mtime = inode.i_mtime,
    .i_ctime = inode. i_ctime,
    .i_blocks = inode.i_blocks / (sb->s_blocksize / 512),
    .i_op = &i_op,
    .f_op = &f_op,
    .i_sb = sb,
    .i_flags = inode.i_flags
  };
  for(int i=0;i<15;i++) in->i_block[i] = inode.i_block[i];
}

/**
 * fill in the fields of kstatfs struct stats with superblock.
 * Returns zero for successful operations.
*/
int sop_statfs(struct super_block *sb, struct kstatfs *ks)
{
  *ks = (struct kstatfs)
  {
    .name = (char *) sb->s_type->name,
    .f_magic = sb->s_magic,
    .f_bsize = sb->s_blocksize,
    .f_blocks = sb->s_blocks_count,
    .f_bfree = sb->s_free_blocks_count,
    .f_inodes = sb->s_inodes_count,
    .f_finodes = sb->s_free_inodes_count,
    .f_inode_size = sb->s_inode_size,
    .f_minor_rev_level = sb->s_minor_rev_level,
    .f_rev_level = sb->s_rev_level,
    .f_namelen = strlen(sb->s_type->name)
  };

  return 0;
}


/* I_OP */

/**
  * assume only the name field of dentry is valid.
  * search file in inode. fill de and return it.
  */
struct dentry* iop_lookup(struct inode *in, struct dentry *de)
{
  if(in->i_blocks == 0 || strlen(de->d_name) == 0) return de;

  struct super_block *sb = current_sb;
  unsigned char* buffer = malloc(sb->s_blocksize);
  uint block_index = 0;
  loffset_t offset = 0;
  while(offset < in->i_size)
  {
    offset += read_block_to_buffer(in, block_index, buffer, sb);
    int res_inode = search_in_directory_buffer(sb->s_blocksize, buffer, de->d_name);

    if(res_inode)
    {
      struct inode *ino = malloc(sizeof(struct inode));
      *ino = (struct inode){ .i_ino = res_inode };
      sb->s_op->read_inode(ino);

      *de = (struct dentry)
      {
        .d_name = de->d_name,
        .d_flags = ino->i_flags,
        .d_inode = ino,
        .d_parent = NULL,
        .d_sb = sb
      };

      break;
    }
    block_index++;
  }

  free(buffer);
  return de;
}

/**
 * This function reads the contents 
 * of the link in dir into the buffer buf provided by the user.
 * The read operation reads len
 * bytes and returns the number of bytes read.
*/
int iop_readlink(struct dentry *de, char *buf, int len)
{
  struct inode *in = de->d_inode;

  if(!S_ISLNK(in->i_mode)) return -1;

  struct super_block *sb = current_sb;

  int n = MIN(len, in->i_size);

  if(in->i_size < 60)
  {
    strncpy(buf, (char *)in->i_block, n);
  }
  else
  {
    u_char *tmp_block = malloc(EXT2_NAME_LEN);
    read_block_to_buffer(in, 0, tmp_block, sb);

    strncpy(buf, tmp_block, n);
    free(tmp_block);
  }
  
  buf[n] = '\0';
  return n;
}

/**
 * This function calls the callback
 * for every directory entry in inode i. It returns the total
 * number of entries in the directory.
*/
int iop_readdir(struct inode *in, filldir_t fdir)
{
  int number_of_files = 0;

  struct super_block *sb = current_sb;
  unsigned char* buffer = malloc(sb->s_blocksize);

  uint block_index = 0;
  loffset_t offset = 0;
  while(offset < in->i_size)
  {
    offset += read_block_to_buffer(in, block_index, buffer, sb);
   
    int a = callback_dentries_in_directory(sb->s_blocksize,
        buffer, fdir);
    number_of_files += a;

    block_index++;
  }

  free(buffer);
  return number_of_files;
}

/**
 * fills in the fields of kstat structure with the information
 * from dir. It returns zero for successful operation.
*/
int iop_getattr(struct dentry *de, struct kstat *ks)
{
  struct inode in = (struct inode) { .i_ino = de->d_inode->i_ino };
  current_sb->s_op->read_inode(&in);

  *ks = (struct kstat)
  {
    .ino = in.i_ino,
    .mode = in.i_mode,
    .nlink = in.i_nlink,
    .uid = in.i_uid,
    .gid = in.i_gid,
    .size = in.i_size,
    .atime = in.i_atime,
    .mtime = in.i_mtime,
    .ctime = in.i_ctime,
    .blksize = current_sb->s_blocksize,
    .blocks = in.i_blocks
  };

  return 0;
}



struct super_block *get_superblock(struct file_system_type *fs)
{
  struct ext2_super_block esb;
  lseek(fs->file_descriptor, BASE_OFFSET, SEEK_SET);
  read(fs->file_descriptor, (void*)&esb, sizeof(struct ext2_super_block));

  struct super_block *sb = malloc(sizeof(struct super_block));
  *sb = (struct super_block)
  {
    .s_inodes_count      = esb.s_inodes_count,
    .s_blocks_count      = esb.s_blocks_count,
    .s_free_blocks_count = esb.s_free_blocks_count,
    .s_free_inodes_count = esb.s_free_inodes_count,
    .s_first_data_block  = esb.s_first_data_block,
    .s_blocksize         = 1 << (esb.s_log_block_size + 10),
    .s_blocksize_bits    = esb.s_log_block_size + 10,
    .s_blocks_per_group  = esb.s_blocks_per_group,
    .s_inodes_per_group  = esb.s_inodes_per_group,
    .s_minor_rev_level   = esb.s_minor_rev_level,
    .s_rev_level         = esb.s_rev_level,
    .s_first_ino         = esb.s_first_ino,
    .s_inode_size        = esb.s_inode_size,
    .s_block_group_nr    = esb.s_block_group_nr,
    .s_type              = fs,
    .s_op                = &s_op,
    .s_magic             = esb.s_magic
    // superblock.s_maxbytes;
    // superblock.s_flags;
    // superblock.s_fs_info;
  };

  struct inode *in = malloc(sizeof(struct inode));
  *in = (struct inode) { .i_ino = 2 };
  
  // READ ROOT INODE
  struct ext2_group_desc gd;
  lseek(current_fs->file_descriptor,
      BASE_OFFSET + SB_SIZE,
      SEEK_SET);
  read(current_fs->file_descriptor, (void*)&gd, GD_SIZE);

  lseek(current_fs->file_descriptor,
      (gd.bg_inode_table * sb->s_blocksize) + sb->s_inode_size,
      SEEK_SET);

  struct ext2_inode inode;
  read(current_fs->file_descriptor, (void*)&inode, sb->s_inode_size);

  *in = (struct inode) // set inode information
  {
    .i_ino = in->i_ino,
    .i_mode = inode.i_mode,
    .i_nlink = inode.i_links_count,
    .i_uid = inode.i_uid,
    .i_gid = inode.i_gid,
    .i_size = inode.i_size,
    .i_atime = inode.i_atime,
    .i_mtime = inode.i_mtime,
    .i_ctime = inode. i_ctime,
    .i_blocks = inode.i_blocks,
    .i_op = &i_op,
    .f_op = &f_op,
    .i_sb = sb,
    .i_flags = inode.i_flags
  };
  for(int i=0;i<15;i++) in->i_block[i] = inode.i_block[i];
  // READ ROOT INODE

  struct dentry *de = malloc(sizeof(struct dentry));
  de->d_name = malloc(2);
  strcpy(de->d_name, "/");
  *de = (struct dentry)
  {
    .d_name = de->d_name,
    .d_flags = in->i_flags,
    .d_inode = in,
    .d_parent = de,
    .d_sb = sb
  };

  sb->s_root = de;

  return sb;
}

struct file_system_type *initialize_ext2(const char *image_path)
{
  /* fill super_operations s_op */
  s_op = (struct super_operations)
  {
    .read_inode = sop_read_inode,
    .statfs     = sop_statfs
  };

  /* fill inode_operations i_op */
  i_op = (struct inode_operations)
  {
    .getattr  = iop_getattr,
    .lookup   = iop_lookup,
    .readdir  = iop_readdir,
    .readlink = iop_readlink
  };
  
  /* fill file_operations f_op */
  f_op = (struct file_operations)
  {
    .llseek   = fop_llseek,
    .open     = fop_open,
    .read     = fop_read,
    .release  = fop_release
  };

  myfs.name = fs_name;
  myfs.file_descriptor = open(image_path, O_RDONLY);

  myfs.get_superblock = get_superblock;

  return &myfs;
}



/** UTILITIES **/

/**
 * given a block index, calculate its offset in
 * data pointer list of inode with indirects.
*/
void calculate_data_position(uint block_index, int blocksize,
    int* direct, int* single_indirect, int* double_indirect, int* triple_indirect)
{
  uint ptr_cnt     = (blocksize / 4);
  uint ptr_cnt_sqr = (blocksize / 4) * (blocksize / 4);
  uint ptr_cnt_cub = (blocksize / 4) * (blocksize / 4) * (blocksize / 4);

  if(block_index < 12)            // direct pointer
  {
    *direct = block_index;
    return;
  }
  block_index -= 12;

  if(block_index < ptr_cnt)       // single indirect pointer
  {
    *single_indirect = block_index;
    return;
  }
  block_index -= ptr_cnt;

  if(block_index < ptr_cnt_sqr)   // double indirect pointer
  {
    *double_indirect = block_index / ptr_cnt;
    *single_indirect = block_index - (*double_indirect) * ptr_cnt;
    return;
  }
  block_index -= ptr_cnt_sqr;

  if(block_index < ptr_cnt_cub)   // triple indirect pointer
  {
    *triple_indirect = block_index / ptr_cnt_sqr;
    block_index -= (*triple_indirect) * ptr_cnt_sqr;
    
    *double_indirect = block_index / ptr_cnt;
    *single_indirect = block_index - (*double_indirect) * ptr_cnt;

    return;
  }

  return;
}

/**
 * read one block -or less- of data.
 * fill buffer with the read data.
 * return number of bytes read. 
 * what if its a hole?
*/
uint read_block_to_buffer(struct inode* inode, uint block_index,
    unsigned char* buffer, struct super_block *sb)
{
  unsigned char hole_flag = 0;

  int direct, single_indirect, double_indirect, triple_indirect;
  direct = single_indirect = double_indirect = triple_indirect = -1;
  calculate_data_position(block_index, sb->s_blocksize, &direct, &single_indirect,
      &double_indirect, &triple_indirect);

  uint tmp_index = 0;
  if(triple_indirect != -1 && !inode->i_block[14])
  {
    hole_flag = 1;
  }
  else if(triple_indirect != -1)
  {
    lseek(current_fs->file_descriptor, block_index * sb->s_blocksize, SEEK_SET);
    read(current_fs->file_descriptor, buffer, sb->s_blocksize);
    tmp_index = ((uint*)buffer)[triple_indirect];
  }

  if(double_indirect != -1 && !hole_flag)
  {
    if(triple_indirect == -1) tmp_index = inode->i_block[13];

    if(tmp_index == 0)
    {
      hole_flag = 1;
    }
    else
    {
      lseek(current_fs->file_descriptor, tmp_index * sb->s_blocksize, SEEK_SET);
      read(current_fs->file_descriptor, buffer, sb->s_blocksize);
      tmp_index = ((uint*)buffer)[double_indirect];
    }
  }
  
  if(single_indirect != -1 && !hole_flag)
  {
    if(double_indirect == -1) tmp_index = inode->i_block[12];

    if(tmp_index == 0)
    {
      hole_flag = 1;
    }
    else
    {
      lseek(current_fs->file_descriptor, tmp_index * sb->s_blocksize, SEEK_SET);
      read(current_fs->file_descriptor, buffer, sb->s_blocksize);
      tmp_index = ((uint*)buffer)[single_indirect];
    }
  }

  if(direct != -1 && !hole_flag)
  {
    tmp_index = inode->i_block[direct];
  }

  if(!hole_flag)
  {
    lseek(current_fs->file_descriptor, tmp_index * sb->s_blocksize, SEEK_SET);
    read(current_fs->file_descriptor, buffer, sb->s_blocksize);
  }
  else
  {
    memset(buffer, 0, sb->s_blocksize);
  }

  uint remaining_size = inode->i_size - block_index * sb->s_blocksize;
  if(remaining_size < sb->s_blocksize)
    return remaining_size;
  else
    return sb->s_blocksize;
}

/**
 * check if name's dentry exists in the block bytes.
 * should be called for every block including indirect ones.
 * return inode of search.
*/
unsigned long search_in_directory_buffer(unsigned long size,
    unsigned char *dir_block, char *name)
{
  unsigned long offset = 0;
  struct ext2_dir_entry *entry;

  while(offset < size)
  {
    unsigned char *tmp_block = dir_block + offset;
    uint inode = *(uint *)(tmp_block);
    unsigned short rec_len = *(unsigned short *)(tmp_block + sizeof(uint));

    if(!inode)
    {
      offset += rec_len;
      continue;
    }

    entry = (struct ext2_dir_entry*) tmp_block;

    if(!strncmp(name, entry->name, strlen(name)))
      return entry->inode;

    offset += rec_len;
  }

  return 0;
}

/**
 * traverse all of the block bytes. call the callback for every dentry.
 * should be called for every block including indirect ones.
 * return number of dentries.
*/
int callback_dentries_in_directory(unsigned long size,
    unsigned char *dir_block, filldir_t callback)
{
  int file_number = 0;

  unsigned int offset = 0;
  struct ext2_dir_entry *entry;

  while(offset < size)
  {
    unsigned char *tmp_block = dir_block + offset;
    uint inode = *((uint *)(tmp_block));
    unsigned short rec_len = *((unsigned short *)(tmp_block + sizeof(uint)));

    if(!inode)
    {
      offset += rec_len; 
      continue;
    }

    entry = (struct ext2_dir_entry*) tmp_block;
    
    callback(entry->name, entry->name_len, entry->inode);
    file_number++;

    offset += rec_len;
  }

  return file_number;
}























