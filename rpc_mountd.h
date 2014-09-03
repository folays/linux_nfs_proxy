#ifndef RPC_MOUNTD_H_
# define RPC_MOUNTD_H_

#define MOUNTPROG 100005
#define MOUNTVERS3 3

#define MOUNTPROC_NULL 0
#define MOUNTPROC_MNT 1

#define MNTPATHLEN 1024
#define FHSIZE3 64

typedef struct {
  u_int fhandle3_len;
  char *fhandle3_val;
} fhandle3;

enum mountstat3 {
  MNT3_OK = 0,
  MNT3ERR_PERM = 1,
  MNT3ERR_NOENT = 2,
  MNT3ERR_IO = 5,
  MNT3ERR_ACCES = 13,
  MNT3ERR_NOTDIR = 20,
  MNT3ERR_INVAL = 22,
  MNT3ERR_NAMETOOLONG = 63,
  MNT3ERR_NOTSUPP = 10004,
  MNT3ERR_SERVERFAULT = 10006,
};
typedef enum mountstat3 mountstat3;

struct mountres3_ok {
  fhandle3 fhandle;
  struct {
    u_int auth_flavors_len;
    int *auth_flavors_val;
  } auth_flavors;
};
typedef struct mountres3_ok mountres3_ok;

struct mountres3 {
  mountstat3 fhs_status;
  union {
    mountres3_ok mountinfo;
  } mountres3_u;
};
typedef struct mountres3 mountres3;

typedef char *dirpath;

#endif /* !RPC_MOUNTD_H_ */
