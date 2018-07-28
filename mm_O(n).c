#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * Team structure for grading
 ********************************************************/
team_t team = {
    /* Team name */
    "Da Gui Dan",
    /* First member's full name */
    "Mingqi Hou",
    /* First member's email address */
    "mingqi.hou@mail.utoronto.ca",
    /* Second member's full name (leave blank if none) */
    "1",
    /* Second member's email address (leave blank if none) */
    "1"
};

void* heap_listp = NULL;

#define MIN_BLOCK_SIZE 32   //32B is the minimum block size we picked for our smallest free list (<=32)


/**********************************************************
 * mm_init
 * Initialize the heap, including "allocation" of the
 * prologue and epilogue
 * Our free lists are initialized here as well
 * This is done to prevent errors when a call to mm_init
 * is made during a trace
 **********************************************************/
int mm_init(void)
{
    if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1)
        return -1;
    PUT(heap_listp, 0);                                 // alignment padding
    PUT(heap_listp + (1 * WSIZE), PACK(OVERHEAD, 1));   // prologue header
    PUT(heap_listp + (2 * WSIZE), PACK(OVERHEAD, 1));   // prologue footer
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1));          // epilogue header
    heap_listp += DSIZE;
    
    // Initialize the free lists to NULL
    int i;
    for (i=0; i < 9; i++)
        free_list[i] = NULL;

    return 0;
}

/**********************************************************
 * coalesce
 * Covers the 4 cases discussed in the text:
 * - both neighbours are allocated
 * - the next block is available for coalescing
 * - the previous block is available for coalescing
 * - both neighbours are available for coalescing
 * This has been modified to work free lists
 * We remove blocks from the free list that corresponds
 * to the coalescing operation. This preserves the
 * integrity of the free lists
 * 
 * Since all free blocks MUST be in the free list, we can safely
 * assume any free block found can be removed via a call to remove_from list
 **********************************************************/
void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc) {   /* Case 1 */
        return bp;
    }

    else if (prev_alloc && !next_alloc)
    {   /* Case 2 */
        // Remove the next free block from corresponding free_list
        list_entry_t* next = (list_entry_t*)NEXT_BLKP(bp);
        list_del(next);

        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        return (bp);
    }

    else if (!prev_alloc && next_alloc)
    {   /* Case 3 */
        // Remove the prev free block from corresponding free_list
        list_entry_t* prev = (list_entry_t*)PREV_BLKP(bp);
        list_del(prev);

        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        return (PREV_BLKP(bp));
    }

    else
    {   /* Case 4 */
        // Remove both the next and prev free blocks from corresponding free list
        list_entry_t* prev = (list_entry_t*)PREV_BLKP(bp);
        list_entry_t* next = (list_entry_t*)NEXT_BLKP(bp);
        list_del(prev);
        list_del(next);

        size += GET_SIZE(HDRP(PREV_BLKP(bp)))  +
                GET_SIZE(FTRP(NEXT_BLKP(bp)))  ;
        PUT(HDRP(PREV_BLKP(bp)), PACK(size,0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size,0));
        return (PREV_BLKP(bp));
    }
}

/**********************************************************
 * extend_heap
 * Extend the heap by "words" words, maintaining alignment
 * requirements of course. Free the former epilogue block
 * and reallocate its new header
 **********************************************************/
void *extend_heap(size_t words)
{
    char *bp;
    size_t size;

    /* Allocate an even number of words to maintain alignments */
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
    if ( (bp = mem_sbrk(size)) == (void *)-1 )
        return NULL;

    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(size, 0));                // free block header
    PUT(FTRP(bp), PACK(size, 0));                // free block footer
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));        // new epilogue header

    return bp;
}

/**********************************************************
 * find_fit
 * First fit method = speed is fast
 **********************************************************/
void * find_fit(size_t asize)
{

    int free_list_index = list_hash(asize);

    // Traverse the free lists starting from the most appropriately sized one
    // and find the first fitting block. We search largest lists if nothing is found.
    // If there is excess space in the free block we found, we
    // place the remainder back into the free list as a smaller free block
    
    list_entry_t * list_elm_ptr;
    list_entry_t * list_ptr;
    
    char * allocated_ptr;
    int index;
    size_t block_size;
    size_t left_size;
    //size_t left_size_round_up;
    //solution 1
    for(index = free_list_index; index < 9; index ++){
        if(free_list[index] != NULL){

            list_elm_ptr = free_list[index];

            do{
                block_size = GET_SIZE(HDRP(list_elm_ptr));
                
                if(block_size >= asize && (block_size - asize) < MIN_SIZE){
                    list_del(list_elm_ptr);
                    return (void*)list_elm_ptr;
                }

                if(block_size >= asize && (block_size - asize) >= MIN_SIZE){
                    
                    list_del(list_elm_ptr);
                    left_size = block_size - asize;
                    
                    //move to new fit segragate list
                    //left_size_round_up = next_pow_of_2(left_size);
                    allocated_ptr = (char*)list_elm_ptr + left_size;

                    PUT(HDRP(allocated_ptr), PACK(asize, 1));
                    PUT(FTRP(allocated_ptr), PACK(asize, 1));

                    PUT(HDRP(list_elm_ptr), PACK(left_size, 0));
                    PUT(FTRP(list_elm_ptr), PACK(left_size, 0));

                    index = list_hash(left_size);
                    list_ptr = free_list[index];
                    
                    if(list_ptr != NULL)
                        list_add_before(list_ptr, list_elm_ptr);
                    else if(list_ptr == NULL){
                        free_list[index] = list_elm_ptr;
                        list_init(list_elm_ptr);
                    }
                    else
                        list_add(list_elm_ptr);

                    return (void*)allocated_ptr;
                }

                list_elm_ptr = list_elm_ptr->next;
            }while(list_elm_ptr != free_list[index]);
        }
    }

    //solution 2 buddy

    return NULL;
}

/**********************************************************
 * place
 * Mark the block as allocated
 **********************************************************/
void place(void* bp, size_t asize)
{
    /* Get the current block size */
    size_t bsize = GET_SIZE(HDRP(bp));

    PUT(HDRP(bp), PACK(bsize, 1));
    PUT(FTRP(bp), PACK(bsize, 1));
}

/**********************************************************
 * mm_free
 * Free the block and coalesce with neighbouring blocks
 * This function frees a block and inserts it into the free_list
 * There is no additional overhead, as the space we freed up is
 * used for the entry and is invisible to the user. Remember
 * that the pointers are stored in the payload
 **********************************************************/
void mm_free(void *bp)
{
    if(bp == NULL)
    {
        return;
    }
    size_t size = GET_SIZE(HDRP(bp));

    if (GET_ALLOC(HDRP(bp)) == 0){
        // Block was already freed, just return
        return;
    }

    //mark allocated bit 0
    PUT(HDRP(bp), PACK(size,0));
    PUT(FTRP(bp), PACK(size,0));

    // After coalescing, we can add the new (possibly bigger) free block to the free list
    // We do not need to worry about duplicates, as the coalesce function will remove free
    // blocks from the free_list as it groups them.
    list_add((list_entry_t*)coalesce(bp));
}

/**********************************************************
 * mm_malloc
 * Allocate a block of size bytes that is closest to a power
 * of 2 or that is just aligned to double word size and
 * the overhead of a block header and footer (extra book-keeping
 * is stored in the payload when the block is freed)
 * The search for free blocks is determined by find_fit
 * The decision of splitting the block, or not is determined
 * in find_fit as well.
 * If no block satisfies the request, the heap is extended
 **********************************************************/
void *mm_malloc(size_t size)
{

    size_t asize; /* adjusted block size */
    size_t extendsize; /* amount to extend heap if no fit */
    char * bp;

    if (size <= 0)
        return NULL;

    /* Bit twiddling trick: Round up to the next power of 2.
       By rounding up for smaller blocks, we increase the chance that a future larger request
       (likely a realloc) will fit in a block we free. This has the benefit of reducing external fragmentation
       Additionally, we choose a conservative number like 512 to reduce the risk of running out of
       total memory.
     */
    if(size < 512)
        size = next_pow_of_2(size);

    /* Adjust block size to include overhead and alignment reqs. */
    if (size <= DSIZE)
        asize = DSIZE + OVERHEAD;
    else
        asize = DSIZE * ((size + (OVERHEAD) + (DSIZE-1))/ DSIZE);

    //DPRINTF("RECEIVED MALLOC: size=%d\n",asize);

    /* Search the free list for a fit */
    if ((bp = find_fit(asize)) != NULL) {
        place(bp, asize);
       // DPRINTF("FIND FIT - SERVICED MALLOC (0x%x), size=%d\n",bp,asize);
        return bp;
    }

    // Don't use a minimum size, we want to target utilization performance
    // so extend the heap exactly by the request
    extendsize = asize;
    if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
        return NULL;
    place(bp, asize);

    return bp;

}


void *mm_realloc(void *ptr, size_t size)
{
    //as mentioned in lab manual 
    if (size == 0) {
        mm_free(ptr);
        return NULL;
    }

    if (ptr == NULL)
        return (mm_malloc(size));

    char *old_block_ptr = ptr;
    char *new_block_ptr;

    size_t old_block_size = GET_SIZE(HDRP(old_block_ptr));
    size_t padded_size;
    size_t coalesced_size;

    if (size <= DSIZE)
        padded_size = DSIZE + OVERHEAD;
    else
        padded_size = DSIZE * ((size + (OVERHEAD) + (DSIZE-1))/ DSIZE);

    // Handle shrink case
    if (padded_size <= old_block_size)
    {
        size_t excess = old_block_size - padded_size;

        //tearing possible
        if (excess > 2*DSIZE)
        {
            //Fix the 2nd part of the block (the tear) - send this to the free list
            new_block_ptr = (void *)old_block_ptr+padded_size;    //get a pointer to the payload of the torn block
            PUT(HDRP(new_block_ptr), PACK(excess,0));      //set the proper size for the header of the torn block - deallocate markation
            PUT(FTRP(new_block_ptr), PACK(excess,0));      //set the proper size for the footer of the torn block - deallocate markation
            list_add((list_entry_t*)new_block_ptr);

            //Fix the 1st part of the block - the part we send to the user
            //The part of the relevant data is already sitting in the payload so we can return this once fixed
           
            if(padded_size <= 512)
                padded_size = next_pow_of_2(padded_size);

            PUT(HDRP(old_block_ptr), PACK(padded_size,1));     //set the proper size for the header of the torn block - set allocated as we give to user
            PUT(FTRP(old_block_ptr), PACK(padded_size,1));     //^ for footer
            //DPRINTF("TORN CASE HAPPENED!\n");
            return old_block_ptr;  //give this to the user
        }

        //tearing impossible - excess data can not be torn
        if (excess <= 2*DSIZE)
            return old_block_ptr;
    }

    else{

        new_block_ptr = coalesce(old_block_ptr);

        //To see if there is a block can be coalesced 
        coalesced_size = GET_SIZE(HDRP(new_block_ptr));
        
        if (coalesced_size >= padded_size){
            
            size_t payload_size = old_block_size-OVERHEAD;
            memmove(new_block_ptr, old_block_ptr, payload_size); 

            //Pack footer and header
            PUT(HDRP(new_block_ptr), PACK(coalesced_size,1));
            PUT(FTRP(new_block_ptr), PACK(coalesced_size,1));
            return new_block_ptr;
        }
       //if there is no coalesced block, need new malloc

        new_block_ptr = mm_malloc(size);

        if (new_block_ptr == NULL)
            return NULL;

        size_t payload_size = old_block_size-OVERHEAD;
        memcpy(new_block_ptr, old_block_ptr, payload_size);

        PUT(HDRP(old_block_ptr), PACK(old_block_size,0));
        PUT(FTRP(old_block_ptr), PACK(old_block_size,0));
        list_add((list_entry_t*)old_block_ptr);

        return new_block_ptr;
    }

    return NULL;
}

