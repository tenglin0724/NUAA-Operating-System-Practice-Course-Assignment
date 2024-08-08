#include "utils/std.h"
#include "utils/typer.h"
#include "fs.h"
#include "disk.h"
#include "inode.h"

int round_up_bno(int size)
{
    return (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
}

void inode_init(inode_t *this, int mode)
{
    this->link_count = 1;
    this->mode = mode;
    this->uid = getuid();
    this->gid = getgid();
    this->real_device = 0;
    this->size = 0;
}

int inode_is_dir(inode_t *this)
{
    return S_ISDIR(this->mode);
}

int inode_is_regular(inode_t *this)
{
    return S_ISREG(this->mode);
}

int inode_map(inode_t *this, int bno)
{
    assert(bno < round_up_bno(this->size));
    assert(bno < DIRECT_BLOCK_COUNT);

    return this->address[bno];
}

int inode_trunc(inode_t *this)
{
    int bound = round_up_bno(this->size);
    assert(bound < DIRECT_BLOCK_COUNT);

    for (int bno = 0; bno < min(bound, DIRECT_BLOCK_COUNT); bno++)
        fs_free_bno(this->address[bno]);

    this->size = 0;
    return 0;
}

// 1. 不允许存在空洞
// 2. lseek 不能超越文件大小
// 3. write 可以改变文件大小
void inode_alloc_bno(inode_t *this, int bno)
{
    assert(bno < DIRECT_BLOCK_COUNT);
    this->address[bno] = fs_alloc_bno(); 
}

int inode_grow(inode_t *this, int new_size)
{
    int old_bno = round_up_bno(this->size);
    int new_bno = round_up_bno(new_size);

    assert(this->size < new_size);
    for (int bno = old_bno; bno < new_bno; bno++)
        inode_alloc_bno(this, bno);
    this->size = new_size;
    return 0;
}

int inode_read(inode_t *this, off_t pos, void *mem, int size)
{
    //判断是否越届
    if (pos >= this->size)
        return 0;
    //判断是否越界
    if (pos + size > this->size)
        size = this->size - pos;

    void *p = mem;
    //设置最大的界限
    void *bound = mem + size;
    while (p < bound) {
        //计算要偏移块的编号
        int bno = pos / BLOCK_SIZE;
        //计算要偏移的偏移量
        int offset = pos % BLOCK_SIZE; 
        int chunk = min(bound - p, BLOCK_SIZE - offset);

        //将索引节点对应的文件第bno块映射到实际的磁盘块号中
        bno = inode_map(this, bno);
        //将磁盘块号对应的磁盘块读入内存
        void *buff = disk_seek_block(bno);

        //offset+buff表示要复制的起始位置，chunk表示要复制的大小，
        memcpy(p, buff + offset, chunk);

        //由于复制了chunk大小的内容，所以，两者都要自增chunk
        pos += chunk;
        p += chunk;
    }
    return p - mem;
}

int inode_write(inode_t *this, off_t pos, void *mem, int size)
{
    if (pos + size > this->size)
        inode_grow(this, pos + size);

    void *p = mem;
    void *bound = mem+ size;
    while (p < bound) {
        int bno = pos / BLOCK_SIZE;
        int offset = pos % BLOCK_SIZE; 
        int chunk = min(bound - p, BLOCK_SIZE - offset);

        bno = inode_map(this, bno);
        void *buff = disk_seek_block(bno);
        memcpy(buff + offset, p, chunk);

        pos += chunk;
        p += chunk;
    }
    return p - mem;
}

int inode_stat(inode_t *this, struct stat *buf)
{
    buf->st_dev = 0;
    buf->st_ino = 0;
    buf->st_mode = this->mode;
    buf->st_nlink = this->link_count;
 
    buf->st_uid = getuid();
    buf->st_gid = getgid();
    buf->st_rdev = 0;
    buf->st_size = this->size;
    buf->st_blksize = 512;
    buf->st_blocks = 0;

    buf->st_atime = 0;
    buf->st_mtime = 0;
    buf->st_ctime = 0;
    return 0;
}

void inode_dump(inode_t *this)
{
    typer_dump("inode\n");
    typer_push();

    char *type = "?";
    if (inode_is_regular(this))
        type = "reg";
    if (inode_is_dir(this))
        type = "dir";
    typer_dump("type = %s\n", type);

    typer_dump("size = %d\n", this->size);
    typer_dump("nlink = %d\n", this->link_count);
    typer_pop();
}
