
/*************************************************************************
 * Basic Constants and Macros
 * You are not required to use these macros but may find them helpful.
 *************************************************************************/
#define WSIZE       sizeof(void *)         /* word size (bytes) */
#define DSIZE       (2 * WSIZE)            /* doubleword size (bytes) */
#define CHUNKSIZE   (1<<7)      /* initial heap size (bytes) */
#define OVERHEAD    DSIZE     /* overhead of header and footer (bytes) */

#define MAX(x,y) ((x) > (y)?(x) :(y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)          (*(uintptr_t *)(p))
#define PUT(p,val)      (*(uintptr_t *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)     (GET(p) & ~(DSIZE - 1))
#define GET_ALLOC(p)    (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)        ((char *)(bp) - WSIZE)
#define FTRP(bp)        ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))
#define MIN_SIZE        2 * DSIZE
/* alignment */
#define ALIGNMENT 16
/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0xf)

int mm_init(void);
void *mm_malloc(size_t size);
void mm_free(void *ptr);
void *mm_realloc(void *ptr, size_t size);

/* 
 * Students work in teams of one or two.  Teams enter their team name, personal
 * names and login IDs in a struct of this type in their mm.c file.
 */
typedef struct {
    char *teamname; /* ID1+ID2 or ID1 */
    char *name1;    /* Full name of first member. */
    char *id1;      /* Login ID of first member. */
    char *name2;    /* Full name of second member (if any). */
    char *id2;      /* Login ID of second member. */
} team_t;

struct list_entry {
    struct list_entry *prev, *next;
};

//buddy struct
struct buddy{
	unsigned size;
	unsigned longest[1];
};

typedef struct list_entry list_entry_t;


// Hyposize largest allocated chunk size is 4096 Bytes
// Then 2^12, 12 free list for different size of chunks
list_entry_t* free_list[9];
//segragate management


//list management functions
static inline void list_init(list_entry_t *elm) __attribute__((always_inline));
static inline void list_add(list_entry_t *elm) __attribute__((always_inline));
static inline void list_add_before(list_entry_t *listelm, list_entry_t *elm) __attribute__((always_inline));
static inline void list_add_after(list_entry_t *listelm, list_entry_t *elm) __attribute__((always_inline));
static inline void list_del(list_entry_t *listelm) __attribute__((always_inline));
static inline void list_del_init(list_entry_t *listelm) __attribute__((always_inline));
//static inline bool list_empty(list_entry_t *list) __attribute__((always_inline));
static inline list_entry_t *list_next(list_entry_t *listelm) __attribute__((always_inline));
static inline list_entry_t *list_prev(list_entry_t *listelm) __attribute__((always_inline));

static inline void __list_add(list_entry_t *elm, list_entry_t *prev, list_entry_t *next) __attribute__((always_inline));
static inline void __list_del(list_entry_t *prev, list_entry_t *next) __attribute__((always_inline));
static inline int list_hash(size_t size) __attribute__((always_inline));
static inline int is_pow_of_2(size_t size);
static inline size_t next_pow_of_2(size_t size);


static inline int is_pow_of_2(size_t size){
	return !(size & (size - 1));
}

static inline size_t next_pow_of_2(size_t size){
    if(is_pow_of_2(size))
        return size;
    else{
        size |= size >> 1;
        size |= size >> 2;
        size |= size >> 4;
        size |= size >> 8;
        size |= size >> 16;
        size++;
        return size;
    }
}

static inline void
list_init(list_entry_t *elm) {
    elm->prev = elm->next = elm;
}


static inline void
list_add(list_entry_t *elm) {

    assert(elm != NULL);

    //get chunk size
    size_t chunk_size = GET_SIZE(HDRP(elm));

    int list_index = list_hash(chunk_size);

    if(free_list[list_index]  != NULL)
        list_add_before(free_list[list_index], elm);
    else{
        // if it is the header
        free_list[list_index] = elm;
        list_init(elm);
    }
}

static inline void
list_add_before(list_entry_t *listelm, list_entry_t *elm) {
    __list_add(elm, listelm->prev, listelm);
}

static inline void
list_add_after(list_entry_t *listelm, list_entry_t *elm) {
    __list_add(elm, listelm, listelm->next);
}

static inline void
list_del(list_entry_t *listelm) {

    assert(listelm != NULL);

    //get chunk size
    size_t chunk_size = GET_SIZE(HDRP(listelm));

    int list_index = list_hash(chunk_size);

    if(listelm != listelm->next){
        __list_del(listelm->prev, listelm->next);
        if(free_list[list_index] == listelm)
            free_list[list_index] = listelm->next;
    }
    else
        free_list[list_index] = NULL;
}

static inline void
list_del_init(list_entry_t *listelm) {
    list_del(listelm);
    list_init(listelm);
}

/*
static inline bool
list_empty(list_entry_t *list) {
    return list->next == list;
}*/


static inline list_entry_t *
list_next(list_entry_t *listelm) {
    return listelm->next;
}


static inline list_entry_t *
list_prev(list_entry_t *listelm) {
    return listelm->prev;
}


static inline void
__list_add(list_entry_t *elm, list_entry_t *prev, list_entry_t *next) {
    prev->next = next->prev = elm;
    elm->next = next;
    elm->prev = prev;
}

static inline void
__list_del(list_entry_t *prev, list_entry_t *next) {
    prev->next = next;
    next->prev = prev;
}

extern team_t team;

//depending on allocated size, and choose a list to add 
int list_hash(size_t size){
   
   int counter = 0;

   assert(size != 0);

   while(size){
    counter ++;
    size = size >> 1;
   }

   if (counter >= 9)
     return 8;
   else 
     return counter;
   
}
