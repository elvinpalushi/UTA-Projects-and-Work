#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define ALIGN4(s) (((((s)-1) >> 2) << 2) + 4)
#define BLOCK_DATA(b) ((b) + 1)
#define BLOCK_HEADER(ptr) ((struct _block *)(ptr)-1)

static int atexit_registered = 0;
static int num_mallocs = 0;
static int num_frees = 0;
static int num_reuses = 0;
static int num_grows = 0;
static int num_splits = 0;
static int num_coalesces = 0;
static int num_blocks = 0;
static int num_requested = 0;
static int max_heap = 0;

struct _block
{
   size_t size;         /* Size of the allocated _block of memory in bytes */
   struct _block *next; /* Pointer to the next _block of allocated memory  */
   bool free;           /* Is this _block free?                            */
   char padding[3];     /* Padding: IENTRTMzMjAgU3jMDEED                   */
};

struct _block *heapList = NULL; /* Free list to track the _blocks available */

/*
 *  \brief printStatistics
 *
 *  \param none
 *
 *  Prints the heap statistics upon process exit.  Registered
 *  via atexit()
 *
 *  \return none
 */
void printStatistics(void)
{
   struct _block *curr = heapList;
   
   while (curr)
   {
      num_blocks++;

      curr = curr->next;
   }

   printf("\nheap management statistics\n");
   printf("mallocs:\t%d\n", num_mallocs);
   printf("frees:\t\t%d\n", num_frees);
   printf("reuses:\t\t%d\n", num_reuses);
   printf("grows:\t\t%d\n", num_grows);
   printf("splits:\t\t%d\n", num_splits);
   printf("coalesces:\t%d\n", num_coalesces);
   printf("blocks:\t\t%d\n", num_blocks);
   printf("requested:\t%d\n", num_requested);
   printf("max heap:\t%d\n", max_heap);
}



/*
 * \brief findFreeBlock
 *
 * \param last pointer to the linked list of free _blocks
 * \param size size of the _block needed in bytes
 *
 * \return a _block that fits the request or NULL if no free _block matches
 *
 * \TODO Implement Next Fit
 * \TODO Implement Best Fit
 * \TODO Implement Worst Fit
 */
struct _block *findFreeBlock(struct _block **last, size_t size)
{
   struct _block *curr = heapList;

#if defined FIT && FIT == 0
   /* First fit */
   //
   // While we haven't run off the end of the linked list and
   // while the current node we point to isn't free or isn't big enough
   // then continue to iterate over the list.  This loop ends either
   // with curr pointing to NULL, meaning we've run to the end of the list
   // without finding a node or it ends pointing to a free node that has enough
   // space for the request.
   //
   while (curr && !(curr->free && curr->size >= size))
   {
      *last = curr;
      curr = curr->next;
   }
#endif

// \TODO Put your Best Fit code in this #ifdef block
#if defined BEST && BEST == 0
   /** \TODO Implement best fit here */
   long long max_size = INT_MAX;
   struct _block *winner1 = NULL;

   while (curr)
   {
      if (curr->free && curr->size >= size && max_size > (long long)curr->size - (long long)size)
      {
         max_size = (long long)curr->size - (long long)size;
         winner1 = curr;
      }

      *last = curr;
      curr = curr->next;
   }

   curr = winner1;
#endif

// \TODO Put your Worst Fit code in this #ifdef block
#if defined WORST && WORST == 0
   /** \TODO Implement worst fit here */
   long long min_size = INT_MIN;
   struct _block *winner2 = NULL;

   while (curr)
   {
      if (curr->free && curr->size >= size && min_size < (long long)curr->size - (long long)size)
      {
         min_size = (long long)curr->size - (long long)size;
         winner2 = curr;
      }

      *last = curr;
      curr = curr->next;
   }

   curr = winner2;
#endif

// \TODO Put your Next Fit code in this #ifdef block
#if defined NEXT && NEXT == 0
   /** \TODO Implement next fit here */
   struct _block *prev = NULL;
   struct _block *winner3 = NULL;

   while (curr)
   {
      if (curr->free && curr->size >= size)
      {
         prev = curr;

         if (prev && prev->free && prev->size >= size)
         {
            winner3 = prev;
         }

         curr = prev;
      }

      *last = curr;
      curr = curr->next;
   }

   curr = winner3;
#endif

   return curr;
}

/*
 * \brief growheap
 *
 * Given a requested size of memory, use sbrk() to dynamically
 * increase the data segment of the calling process.  Updates
 * the free list with the newly allocated memory.
 *
 * \param last tail of the free _block list
 * \param size size in bytes to request from the OS
 *
 * \return returns the newly allocated _block of NULL if failed
 */
struct _block *growHeap(struct _block *last, size_t size)
{
   /* Request more space from OS */
   struct _block *curr = (struct _block *)sbrk(0);
   struct _block *prev = (struct _block *)sbrk(sizeof(struct _block) + size);

   assert(curr == prev);

   /* OS allocation failed */
   if (curr == (struct _block *)-1)
   {
      return NULL;
   }

   /* Update heapList if not set */
   if (heapList == NULL)
   {
      heapList = curr;
   }

   /* Attach new _block to previous _block */
   if (last)
   {
      last->next = curr;
   }

   /* Update _block metadata:
      Set the size of the new block and initialize the new block to "free".
      Set its next pointer to NULL since it's now the tail of the linked list.
   */
   curr->size = size;
   curr->next = NULL;
   curr->free = false;
   return curr;
}

/*
 * \brief malloc
 *
 * finds a free _block of heap memory for the calling process.
 * if there is no free _block that satisfies the request then grows the
 * heap and returns a new _block
 *
 * \param size size of the requested memory in bytes
 *
 * \return returns the requested memory allocation to the calling process
 * or NULL if failed
 */
void *malloc(size_t size)
{

   if (atexit_registered == 0)
   {
      atexit_registered = 1;
      atexit(printStatistics);
   }

   /* Align to multiple of 4 */
   size = ALIGN4(size);

   /* Handle 0 size */
   if (size == 0)
   {
      return NULL;
   }

   /* Look for free _block.  If a free block isn't found then we need to grow our heap. */

   struct _block *last = heapList;
   struct _block *next = findFreeBlock(&last, size);

   /* TODO: If the block found by findFreeBlock is larger than we need then:
            If the leftover space in the new block is greater than the sizeof(_block)+4 then
            split the block.
            If the leftover space in the new block is less than the sizeof(_block)+4 then
            don't split the block.
   */

   if (next && ( next->size - size) > (sizeof(struct _block) + 4))
   {
      struct _block * node = next;
      // split
      num_splits++;
   
      struct _block *oldnext = node->next;
      size_t oldsize = node->size;
      
      node->next = (struct _block * )((long long)BLOCK_DATA(node) + (long long )size);
      node->size = size;
      
      // set up the header for the newly created node
  
      node->next -> next = oldnext;
      node->next -> size = oldsize - size - sizeof( struct _block );
      node->next -> free = true;
      
/* 
header -> ____________ 
         |
         | Header
data->   |-----------
         |
         | Data
         |
         |
         ____________
*/
      
   }

   
   /* Could not find free _block, so grow heap */
   if (next == NULL)
   {
      next = growHeap(last, size);
      num_grows++;
      max_heap += size;
   }
   else
   {
      num_reuses++;
   }

   /* Could not find free _block or grow heap, so just return NULL */
   if (next == NULL)
   {
      return NULL;
   }

   /* Mark _block as in use */
   next->free = false;

   num_mallocs++;
   num_requested += size;

   /* Return data address associated with _block to the user */
   return BLOCK_DATA(next);
}

/*
 * \brief free
 *
 * frees the memory _block pointed to by pointer. if the _block is adjacent
 * to another _block then coalesces (combines) them
 *
 * \param ptr the heap memory to free
 *
 * \return none
 */
void free(void *user_ptr)
{
   if (user_ptr == NULL)
   {
      return;
   }

   /* Make _block as free */
   struct _block *curr = BLOCK_HEADER(user_ptr);
   assert(curr->free == 0);
   curr->free = true;

   /* TODO: Coalesce free _blocks.  If the next block or previous block
            are free then combine them with this block being freed.
   */

   struct _block *ptr = heapList;
   while (ptr)
   {
      if (ptr && ptr->free && ptr->next && ptr->next->free)
      {
         ptr->size = ptr->next->size + sizeof(struct _block) + ptr->size;
         ptr->next = ptr->next->next;
         num_coalesces++;
      }
      ptr = ptr->next;
   }

   num_frees++;
}

void *calloc(size_t nmemb, size_t size)
{
   // \TODO Implement calloc
   char *ptr = (char *) malloc(nmemb * size);
   memset(ptr, 0, nmemb * size);

   return ptr;
}

void *realloc(void *ptr, size_t size)
{
   // \TODO Implement realloc
   char *new_ptr = (char *) malloc(size);
   memcpy(new_ptr, ptr, size);
   free(ptr);

   return new_ptr;
}

/* vim: IENTRTMzMjAgU3ByaW5nIDIwMjM= -----------------------------------------*/
/* vim: set expandtab sts=3 sw=3 ts=6 ft=cpp: --------------------------------*/
