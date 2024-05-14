#ifndef _ID_H
#define _ID_H

typedef struct {
    char data[64];
} oid_t;

//定义一个特殊的 OID，表示空，类似于指针 NULL;HEAD 的初值为 OID_ZERO，表示 HEAD 指向的 commit 为空
#define OID_ZERO "0000000000" "0000000000" "0000000000" "0000000000"

extern void oid_set(oid_t *this, char *data);
extern bool oid_is_zero(oid_t *this);
extern bool oid_equal(oid_t *this, oid_t *that);
extern void get_file_digest(char *path, unsigned char digest[20]);
extern void get_string_oid(char *string, oid_t *oid);
extern void get_oid_path(oid_t *oid, char *path);
extern bool oid_exists(oid_t *this);
extern void get_file_oid(char *path, oid_t *oid);
extern FILE *fopen_oid(oid_t *oid, char *mode);
extern void make_oid(char *abbr, oid_t *oid);

#endif
