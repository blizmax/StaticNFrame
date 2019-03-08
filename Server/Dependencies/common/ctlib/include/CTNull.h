/**
* Linux有些函数在Windows下没有，定义成空函数让编译通过.
* @note 如果在Windows下有响应的实现且需要使用，可以定义宏NOT_NULL_XXX
* 来禁用这些空函数
*/

#ifndef __CHLIB_NULL_H__
#define __CHLIB_NULL_H__

// Linux有些函数在Windows下没有，定义成空函数让编译通过
#ifdef WIN32

// ftok
#ifndef NOT_NULL_FTOK
inline int ftok(const char *pathname, int proj_id) { return 0; }
#endif

// gettimeofday
#ifndef NOT_NULL_GETTIMEOFDAY
inline int gettimeofday(struct timeval *, void *) { return 0; }
#endif

#ifndef NOT_NULL_GETPID
inline int getpid() { return 0; }
#endif

// pthread lib
#ifndef NOT_NULL_PTHREAD
typedef unsigned long int pthread_t;
typedef struct{} pthread_mutex_t;
typedef struct{} pthread_cond_t; 
typedef struct{} pthread_attr_t;
struct timespec 
{
    time_t tv_sec;
    long int tv_nsec;
};

inline int pthread_attr_init(pthread_attr_t *) { return 0; }
inline int pthread_attr_destroy(pthread_attr_t *) { return 0; }
inline int pthread_attr_setdetachstate(pthread_attr_t *, int) { return 0; }
inline int pthread_attr_getdetachstate(const pthread_attr_t *, int *) { return 0; }
inline int pthread_attr_setschedpolicy(pthread_attr_t *, int) { return 0; }
inline int pthread_attr_getschedpolicy(const pthread_attr_t *, int *) { return 0; }
inline int pthread_attr_setschedparam(pthread_attr_t *, const struct sched_param *) { return 0; }
inline int pthread_attr_getschedparam(const pthread_attr_t *, struct sched_param *) { return 0; }
inline int pthread_attr_setinheritsched(pthread_attr_t *, int) { return 0; }
inline int pthread_attr_getinheritsched(const pthread_attr_t *, int *) { return 0; }
inline int pthread_attr_setscope(pthread_attr_t *, int) { return 0; }
inline int pthread_attr_getscope(const pthread_attr_t *, int *) { return 0; }
inline int pthread_attr_getstacksize(const pthread_attr_t *, size_t *) { return 0; }
inline int pthread_attr_setstacksize(pthread_attr_t *, size_t) { return 0; }
inline pthread_t pthread_self() { return 0; }
inline int pthread_mutex_lock(pthread_mutex_t*) { return 0; }
inline int pthread_mutex_unlock(pthread_mutex_t*) { return 0; }
inline int pthread_cond_signal(pthread_cond_t*) { return 0; }
inline int pthread_cond_wait(pthread_cond_t*, pthread_mutex_t*) { return 0; }
inline int pthread_cond_timedwait (pthread_cond_t *,pthread_mutex_t *,struct timespec *) { return 0; }
inline int pthread_create (pthread_t *,pthread_attr_t *,void *(*) (void *),void *__arg) { return 0; }
inline int pthread_join (pthread_t, void **) { return 0; }
inline int pthread_mutex_init(pthread_mutex_t *, void *) { return 0; }
inline int pthread_cond_init(pthread_cond_t *, void*) { return 0; }
#endif

// myql
#ifndef NOT_NULL_MYSQL
enum mysql_option
{
    MYSQL_OPT_CONNECT_TIMEOUT, MYSQL_OPT_COMPRESS, MYSQL_OPT_NAMED_PIPE,
    MYSQL_INIT_COMMAND, MYSQL_READ_DEFAULT_FILE, MYSQL_READ_DEFAULT_GROUP,
    MYSQL_SET_CHARSET_DIR, MYSQL_SET_CHARSET_NAME, MYSQL_OPT_LOCAL_INFILE,
    MYSQL_OPT_PROTOCOL, MYSQL_SHARED_MEMORY_BASE_NAME, MYSQL_OPT_READ_TIMEOUT,
    MYSQL_OPT_WRITE_TIMEOUT, MYSQL_OPT_USE_RESULT,
    MYSQL_OPT_USE_REMOTE_CONNECTION, MYSQL_OPT_USE_EMBEDDED_CONNECTION,
    MYSQL_OPT_GUESS_CONNECTION, MYSQL_SET_CLIENT_IP, MYSQL_SECURE_AUTH,
    MYSQL_REPORT_DATA_TRUNCATION, MYSQL_OPT_RECONNECT,
    MYSQL_OPT_SSL_VERIFY_SERVER_CERT
};

enum enum_field_types { MYSQL_TYPE_DECIMAL, MYSQL_TYPE_TINY,
                        MYSQL_TYPE_SHORT,  MYSQL_TYPE_LONG,
                        MYSQL_TYPE_FLOAT,  MYSQL_TYPE_DOUBLE,
                        MYSQL_TYPE_NULL,   MYSQL_TYPE_TIMESTAMP,
                        MYSQL_TYPE_LONGLONG,MYSQL_TYPE_INT24,
                        MYSQL_TYPE_DATE,   MYSQL_TYPE_TIME,
                        MYSQL_TYPE_DATETIME, MYSQL_TYPE_YEAR,
                        MYSQL_TYPE_NEWDATE, MYSQL_TYPE_VARCHAR,
                        MYSQL_TYPE_BIT,
                        MYSQL_TYPE_NEWDECIMAL=246,
                        MYSQL_TYPE_ENUM=247,
                        MYSQL_TYPE_SET=248,
                        MYSQL_TYPE_TINY_BLOB=249,
                        MYSQL_TYPE_MEDIUM_BLOB=250,
                        MYSQL_TYPE_LONG_BLOB=251,
                        MYSQL_TYPE_BLOB=252,
                        MYSQL_TYPE_VAR_STRING=253,
                        MYSQL_TYPE_STRING=254,
                        MYSQL_TYPE_GEOMETRY=255
};

#define CLIENT_MULTI_QUERIES    CLIENT_MULTI_STATEMENTS    
#define FIELD_TYPE_DECIMAL     MYSQL_TYPE_DECIMAL
#define FIELD_TYPE_NEWDECIMAL  MYSQL_TYPE_NEWDECIMAL
#define FIELD_TYPE_TINY        MYSQL_TYPE_TINY
#define FIELD_TYPE_SHORT       MYSQL_TYPE_SHORT
#define FIELD_TYPE_LONG        MYSQL_TYPE_LONG
#define FIELD_TYPE_FLOAT       MYSQL_TYPE_FLOAT
#define FIELD_TYPE_DOUBLE      MYSQL_TYPE_DOUBLE
#define FIELD_TYPE_NULL        MYSQL_TYPE_NULL
#define FIELD_TYPE_TIMESTAMP   MYSQL_TYPE_TIMESTAMP
#define FIELD_TYPE_LONGLONG    MYSQL_TYPE_LONGLONG
#define FIELD_TYPE_INT24       MYSQL_TYPE_INT24
#define FIELD_TYPE_DATE        MYSQL_TYPE_DATE
#define FIELD_TYPE_TIME        MYSQL_TYPE_TIME
#define FIELD_TYPE_DATETIME    MYSQL_TYPE_DATETIME
#define FIELD_TYPE_YEAR        MYSQL_TYPE_YEAR
#define FIELD_TYPE_NEWDATE     MYSQL_TYPE_NEWDATE
#define FIELD_TYPE_ENUM        MYSQL_TYPE_ENUM
#define FIELD_TYPE_SET         MYSQL_TYPE_SET
#define FIELD_TYPE_TINY_BLOB   MYSQL_TYPE_TINY_BLOB
#define FIELD_TYPE_MEDIUM_BLOB MYSQL_TYPE_MEDIUM_BLOB
#define FIELD_TYPE_LONG_BLOB   MYSQL_TYPE_LONG_BLOB
#define FIELD_TYPE_BLOB        MYSQL_TYPE_BLOB
#define FIELD_TYPE_VAR_STRING  MYSQL_TYPE_VAR_STRING
#define FIELD_TYPE_STRING      MYSQL_TYPE_STRING
#define FIELD_TYPE_CHAR        MYSQL_TYPE_TINY
#define FIELD_TYPE_INTERVAL    MYSQL_TYPE_ENUM
#define FIELD_TYPE_GEOMETRY    MYSQL_TYPE_GEOMETRY
#define FIELD_TYPE_BIT         MYSQL_TYPE_BIT

typedef struct{} MYSQL;
typedef struct{} MYSQL_RES;
typedef char ** MYSQL_ROW;
typedef struct st_mysql_field {
    char *name;                 /* Name of column */
    char *org_name;             /* Original column name, if an alias */
    char *table;                /* Table of column if column was a field */
    char *org_table;            /* Org table name, if table was an alias */
    char *db;                   /* Database for table */
    char *catalog;              /* Catalog for table */
    char *def;                  /* Default value (set by mysql_list_fields) */
    unsigned long length;       /* Width of column (create length) */
    unsigned long max_length;   /* Max width for selected set */
    unsigned int name_length;
    unsigned int org_name_length;
    unsigned int table_length;
    unsigned int org_table_length;
    unsigned int db_length;
    unsigned int catalog_length;
    unsigned int def_length;
    unsigned int flags;         /* Div flags */
    unsigned int decimals;      /* Number of decimals in field */
    unsigned int charsetnr;     /* Character set */
    enum enum_field_types type; /* Type of field. See mysql_com.h for types */
} MYSQL_FIELD;

#define IS_NUM(t)       ((t) <= FIELD_TYPE_INT24 || (t) == FIELD_TYPE_YEAR || (t) == FIELD_TYPE_NEWDECIMAL)

inline MYSQL_FIELD * mysql_fetch_fields(MYSQL_RES *res) { return 0; }
inline unsigned int mysql_field_count(MYSQL *mysql) { return 0; }
inline uint64 mysql_affected_rows(MYSQL *mysql) { return 0; }
inline uint64 mysql_insert_id(MYSQL *mysql) { return 0; }
inline unsigned int mysql_errno(MYSQL *mysql) { return 0; }
inline const char * mysql_error(MYSQL *mysql) { return 0; }
inline MYSQL * mysql_init(MYSQL *mysql) { return 0; }
inline MYSQL * mysql_real_connect(MYSQL *mysql, const char *host,
    const char *user, const char *passwd, const char *db, unsigned int port, 
    const char *unix_socket, unsigned long clientflag) { return 0; }
inline int mysql_select_db(MYSQL *mysql, const char *db) { return 0; }
inline int mysql_real_query(MYSQL *mysql, const char *q, unsigned long length) { return 0; }
inline uint64 mysql_num_rows(MYSQL_RES *res) { return 0; }
inline unsigned int mysql_num_fields(MYSQL_RES *res) { return 0; }
inline int mysql_options(MYSQL *mysql, enum mysql_option option, const char *arg) { return 0; }
inline int mysql_ping(MYSQL *mysql) { return 0; }
inline void mysql_close(MYSQL *sock) {}
inline MYSQL_ROW mysql_fetch_row(MYSQL_RES *result) { return 0; }
inline unsigned long * mysql_fetch_lengths(MYSQL_RES *result) { return 0; }
inline void mysql_free_result(MYSQL_RES *result) {}
inline void mysql_data_seek(MYSQL_RES *result, uint64 offset) {}
inline MYSQL_RES* mysql_store_result(MYSQL *mysql) { return 0; }
inline unsigned long mysql_real_escape_string(MYSQL *mysql, 
    char *to,const char *from, unsigned long length) { return 0; }

#define MY_CS_PRIMARY	32
typedef struct{} CHARSET_INFO;
unsigned long escape_string_for_mysql(CHARSET_INFO *charset_info,
    char *to, unsigned long to_length,
    const char *from, unsigned long length) { return 0; }
CHARSET_INFO* get_charset_by_csname(const char *cs_name,
    unsigned int cs_flags, int flags) { return 0; }
#endif

//dlopen系列函数
#ifndef NOT_NULL_DLOPEN
enum
{
    RTLD_LAZY = 1,
    RTLD_NODELETE,
    RTLD_GLOBAL,
};

inline void *dlopen(const char *filename, int flag) { return NULL; }
inline char *dlerror(void) { return NULL; }
inline void *dlsym(void *handle, const char *symbol) { return NULL; }
inline int dlclose(void *handle) { return 0; }
#endif

#endif //WIN32

#endif //__CHLIB_NULL_H__
