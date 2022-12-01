#include "kheap.h"

#include <inc/memlayout.h>
#include <inc/dynamic_allocator.h>
#include "memory_manager.h"

//==================================================================//
//==================================================================//
//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)//
//==================================================================//
//==================================================================//

void initialize_dyn_block_system()
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] initialize_dyn_block_system
	// your code is here, remove the panic and write your code
	//kpanic_into_prompt("initialize_dyn_block_system() is not implemented yet...!!");
	//[1] Initialize two lists (AllocMemBlocksList & FreeMemBlocksList) [Hint: use LIST_INIT()]
	LIST_INIT(&AllocMemBlocksList);
	LIST_INIT(&FreeMemBlocksList);
	uint32 array_max = 0;
	uint32 heap_size = 0;
#if STATIC_MEMBLOCK_ALLOC
	//DO NOTHING
#else
	/*[2] Dynamically allocate the array of MemBlockNodes
	 * 	remember to:
	 * 		1. set MAX_MEM_BLOCK_CNT with the chosen size of the array
	 * 		2. allocation should be aligned on PAGE boundary
	 * 	HINT: can use alloc_chunk(...) function
	 */
	heap_size = KERNEL_HEAP_MAX - KERNEL_HEAP_START;
	MAX_MEM_BLOCK_CNT = NUM_OF_KHEAP_PAGES;
	MemBlockNodes = (struct MemBlock *)KERNEL_HEAP_START;
	array_max = ROUNDUP(MAX_MEM_BLOCK_CNT*sizeof(struct MemBlock),PAGE_SIZE);
	allocate_chunk(ptr_page_directory, KERNEL_HEAP_START, array_max, PERM_WRITEABLE);

#endif
	//[3] Initialize AvailableMemBlocksList by filling it with the MemBlockNodes
	initialize_MemBlocksList(MAX_MEM_BLOCK_CNT);
	//[4] Insert a new MemBlock with the remaining heap size into the FreeMemBlocksList

	struct MemBlock * newBlock=LIST_FIRST(&AvailableMemBlocksList);
	newBlock->size=heap_size-array_max;
	newBlock->sva=KERNEL_HEAP_START+array_max;
	LIST_REMOVE(&AvailableMemBlocksList, newBlock);
	LIST_INSERT_HEAD(&FreeMemBlocksList,newBlock);

}
int ph_arr[NUM_OF_KHEAP_PAGES];
void* kmalloc(unsigned int size)
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] kmalloc
	// your code is here, remove the panic and write your code
	//kpanic_into_prompt("kmalloc() is not implemented yet...!!");

	//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)
	//refer to the project presentation and documentation for details
	// use "isKHeapPlacementStrategyFIRSTFIT() ..." functions to check the current strategy
	struct FrameInfo * frame_ptr = NULL;
	uint32* pageTable_ptr = NULL;
	struct MemBlock * newBlock = NULL;
	size = ROUNDUP(size,PAGE_SIZE);

	if(isKHeapPlacementStrategyFIRSTFIT())
		newBlock = alloc_block_FF(size);
	else if (isKHeapPlacementStrategyBESTFIT())
		newBlock = alloc_block_BF(size);
	else if(isKHeapPlacementStrategyNEXTFIT())
		newBlock = alloc_block_NF(size);
	else
		return NULL;

	if(newBlock == NULL)
		return NULL;
	int perm = PERM_WRITEABLE;
	int ret = allocate_chunk(ptr_page_directory,newBlock->sva,size,perm);
	if(ret == 0){
		for(int i = newBlock->sva; i <newBlock->sva+newBlock->size ; i+=PAGE_SIZE ){
		frame_ptr = get_frame_info(ptr_page_directory,i,&pageTable_ptr);
		ph_arr[to_frame_number(frame_ptr)] = i;
		}
		insert_sorted_allocList(newBlock);
		return (void*)newBlock->sva;
	}
	else
		return NULL;
}

void kfree(void* virtual_address)
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] kfree
	// Write your code here, remove the panic and write your code
	//panic("kfree() is not implemented yet...!!");
	struct MemBlock * newBlock = NULL;
	struct FrameInfo * frame_ptr = NULL;
	uint32* pageTable_ptr = NULL;
	if((uint32)virtual_address >= KERNEL_HEAP_START &&(uint32)virtual_address < KERNEL_HEAP_MAX ){
		newBlock = find_block(&AllocMemBlocksList,(uint32)virtual_address);
		if(newBlock != NULL){
			for(uint32 i = ROUNDDOWN(newBlock->sva,PAGE_SIZE) ; i<ROUNDUP(newBlock->sva+newBlock->size,PAGE_SIZE) ;i+=PAGE_SIZE){
				frame_ptr = get_frame_info(ptr_page_directory,i,&pageTable_ptr);
				ph_arr[to_frame_number(frame_ptr)] = -1;
				unmap_frame(ptr_page_directory,i);
			}
			LIST_REMOVE(&AllocMemBlocksList,newBlock);
			insert_sorted_with_merge_freeList(newBlock);


		}
	}
}

unsigned int kheap_virtual_address(unsigned int physical_address)
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] kheap_virtual_address
	// Write your code here, remove the panic and write your code
	//panic("kheap_virtual_address() is not implemented yet...!!");
	int f_num = physical_address >> 12;
	if(ph_arr[f_num] == -1)
		return 0;
	return ph_arr[f_num];
	//return the virtual address corresponding to given physical_address
	//refer to the project presentation and documentation for details
	//EFFICIENT IMPLEMENTATION ~O(1) IS REQUIRED ==================
}

unsigned int kheap_physical_address(unsigned int virtual_address)
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] kheap_physical_address
	// Write your code here, remove the panic and write your code
	//panic("kheap_physical_address() is not implemented yet...!!");
	virtual_address = ROUNDDOWN(virtual_address,PAGE_SIZE);
	uint32* pageTable_ptr = NULL;
	struct FrameInfo * frame_ptr = get_frame_info(ptr_page_directory,virtual_address,&pageTable_ptr);
	if(frame_ptr != NULL)
		return to_physical_address(frame_ptr);
	else
			return 0;
	//return the physical address corresponding to given virtual_address
	//refer to the project presentation and documentation for details
}


void kfreeall()
{
	panic("Not implemented!");

}

void kshrink(uint32 newSize)
{
	panic("Not implemented!");
}

void kexpand(uint32 newSize)
{
	panic("Not implemented!");
}




//=================================================================================//
//============================== BONUS FUNCTION ===================================//
//=================================================================================//
// krealloc():

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to kmalloc().
//	A call with new_size = zero is equivalent to kfree().

void *krealloc(void *virtual_address, uint32 new_size)
{
	//TODO: [PROJECT MS2 - BONUS] [KERNEL HEAP] krealloc
	// Write your code here, remove the panic and write your code
	//panic("krealloc() is not implemented yet...!!");
	struct MemBlock * currentBlock = NULL;
	struct MemBlock * newBlock = NULL;

	uint32 va = (uint32)virtual_address;

	currentBlock = find_block(&AllocMemBlocksList,va);
	if(currentBlock == NULL)
		return kmalloc(new_size);

	uint32 current_size = currentBlock->size;
	int diff_size = new_size - currentBlock->size;
	cprintf(" X %d\n",new_size);
	cprintf(" Y %d\n",currentBlock->size);
	cprintf(" Z %d\n",diff_size);
	cprintf("------------------------\n");
	if(new_size == 0){
		kfree(virtual_address);
		return virtual_address;
	}
	else{
		if(diff_size > 0){
			int res = allocate_chunk(ptr_page_directory,currentBlock->size+currentBlock->sva,diff_size,PERM_WRITEABLE);
			// there is allocated pages within the requested page so will reallocate
			if(res == -1){
				cprintf("REALLOCATION\n");
				new_size = ROUNDUP(new_size,PAGE_SIZE);
				newBlock = alloc_block_BF(new_size);

				// NO suitable block
				if(newBlock == NULL)
					return NULL;

				cut_paste_pages(ptr_page_directory,va,newBlock->sva,current_size);
				cprintf(" rem %d \n",newBlock->size - current_size);
				cprintf(" A %d \n",newBlock->size);
				cprintf(" B %d \n",current_size);
				allocate_chunk(ptr_page_directory,ROUNDDOWN(newBlock->sva+current_size,PAGE_SIZE),diff_size,PERM_WRITEABLE);
				LIST_REMOVE(&AllocMemBlocksList,currentBlock);
				insert_sorted_allocList(newBlock);
				return (void *) newBlock->sva;
			}
			// the range is free and allocation is done just need to insert ablock
			else if(res == 0){
				cprintf("I AM HERE \n");
				currentBlock->size = new_size;
				return virtual_address;
			}
		}
		else if(diff_size <= 0)
			return virtual_address;
	}
	return NULL;
}

