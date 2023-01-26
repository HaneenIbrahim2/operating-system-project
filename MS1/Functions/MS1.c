/*
 * dyn_block_management.c
 *
 *  Created on: Sep 21, 2022
 *      Author: HP
 */
#include <inc/assert.h>
#include <inc/string.h>
#include "../inc/dynamic_allocator.h"


//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//

//===========================
// PRINT MEM BLOCK LISTS:
//===========================

void print_mem_block_lists()
{
	cprintf("\n=========================================\n");
	struct MemBlock* blk ;
	struct MemBlock* lastBlk = NULL ;
	cprintf("\nFreeMemBlocksList:\n");
	uint8 sorted = 1 ;
	LIST_FOREACH(blk, &FreeMemBlocksList)
	{
		if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
			sorted = 0 ;
		cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size) ;
		lastBlk = blk;
	}
	if (!sorted)	cprintf("\nFreeMemBlocksList is NOT SORTED!!\n") ;

	lastBlk = NULL ;
	cprintf("\nAllocMemBlocksList:\n");
	sorted = 1 ;
	LIST_FOREACH(blk, &AllocMemBlocksList)
	{
		if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
			sorted = 0 ;
		cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size) ;
		lastBlk = blk;
	}
	if (!sorted)	cprintf("\nAllocMemBlocksList is NOT SORTED!!\n") ;
	cprintf("\n=========================================\n");

}

//********************************************************************************//
//********************************************************************************//

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

//===============================
// [1] INITIALIZE AVAILABLE LIST:
//===============================
void initialize_MemBlocksList(uint32 numOfBlocks)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] initialize_MemBlocksList
	// Write your code here, remove the panic and write your code
	//panic("initialize_MemBlocksList() is not implemented yet...!!");
	LIST_INIT(&AvailableMemBlocksList);
	for(int i=0;i<numOfBlocks;i++){
		LIST_INSERT_TAIL(&AvailableMemBlocksList,&MemBlockNodes[i]);
	}


}

//===============================
// [2] FIND BLOCK:
//===============================
struct MemBlock *find_block(struct MemBlock_List *blockList, uint32 va)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] find_block
	// Write your code here, remove the panic and write your code
	//panic("find_block() is not implemented yet...!!");

		struct MemBlock* block;
		int found = 0;
		LIST_FOREACH(block, blockList) {
			if (va == block->sva) {
				found = 1;
				return block;
			}
		}
		if (found == 0) {
			return NULL;
		}
		return 0;



}

//=========================================
// [3] INSERT BLOCK IN ALLOC LIST [SORTED]:
//=========================================
void insert_sorted_allocList(struct MemBlock *blockToInsert)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] insert_sorted_allocList
	// Write your code here, remove the panic and write your code
	//panic("insert_sorted_allocList() is not implemented yet...!!");

		if (LIST_EMPTY(&AllocMemBlocksList)) {     //if empty -> true(1)
			LIST_INSERT_HEAD(&AllocMemBlocksList, blockToInsert);
		}
		else {
			struct MemBlock* block;
			bool found = 0;
			LIST_FOREACH(block, &AllocMemBlocksList) {
				if (blockToInsert->sva < block->sva) {
					LIST_INSERT_BEFORE(&AllocMemBlocksList, block, blockToInsert);
					found = 1;
					break;
				}

			}
			if (found == 0) {
				LIST_INSERT_TAIL(&AllocMemBlocksList, blockToInsert);
			}
		}

}

//=========================================
// [4] ALLOCATE BLOCK BY FIRST FIT:
//=========================================
struct MemBlock *alloc_block_FF(uint32 size)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_FF
	// Write your code here, remove the panic and write your code
	//panic("alloc_block_FF() is not implemented yet...!!");
	struct MemBlock* iterator;
	LIST_FOREACH(iterator, &FreeMemBlocksList){
		if(size==iterator->size){
			LIST_REMOVE(&FreeMemBlocksList, iterator);
					return iterator;
			}
		else if(size<iterator->size){
				struct MemBlock* newBlock = LIST_FIRST(&AvailableMemBlocksList);
				newBlock->size=size;
				newBlock->sva=iterator->sva;
				iterator->size=iterator->size-size;
				iterator->sva=iterator->sva+size;
				LIST_REMOVE(&AvailableMemBlocksList, newBlock);
				return newBlock;
				}
			}
			return NULL;

}


//=========================================
// [5] ALLOCATE BLOCK BY BEST FIT:
//=========================================
struct MemBlock *alloc_block_BF(uint32 size)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_BF
	// Write your code here, remove the panic and write your code
    // panic("alloc_block_BF() is not implemented yet...!!");

	struct MemBlock *freeMemBlock;
	struct MemBlock *selectedMemBlock;
		int minMemBlockSize=0;
		int isFirstElement = 1;
		LIST_FOREACH(freeMemBlock, &FreeMemBlocksList)
		{

			if(freeMemBlock->size == size) {

				LIST_REMOVE(&FreeMemBlocksList, freeMemBlock);

				return freeMemBlock;

			}else if( freeMemBlock->size > size) {

				if(isFirstElement==1){

					minMemBlockSize = freeMemBlock->size - size;
					selectedMemBlock = freeMemBlock;
					isFirstElement++;
					continue;
				}else if(((freeMemBlock->size - size) < minMemBlockSize) && isFirstElement>1) {

					minMemBlockSize = freeMemBlock->size - size;
					selectedMemBlock = freeMemBlock;
					continue;

			}

		}

	}
		if(selectedMemBlock != NULL) {
			struct MemBlock* newBlock = LIST_FIRST(&AvailableMemBlocksList);
				newBlock->size = size;
				newBlock->sva = selectedMemBlock->sva;
				selectedMemBlock->size = selectedMemBlock->size-size;
				selectedMemBlock->sva = selectedMemBlock->sva+size;
				LIST_REMOVE(&AvailableMemBlocksList, newBlock);


				return newBlock;
		}
		return NULL;


}






//=========================================
// [7] ALLOCATE BLOCK BY NEXT FIT:
//=========================================

//Global pointer to keep track of the last allocated element.
struct MemBlock *lastElement=NULL;
struct MemBlock *alloc_block_NF(uint32 size)
{
	//TODO: [PROJECT MS1 - BONUS] [DYNAMIC ALLOCATOR] alloc_block_NF
	// Write your code here, remove the panic and write your code
	//panic("alloc_block_NF() is not implemented yet...!!");
	
	struct MemBlock* iterator;
	//If the function was called in the first time.
	if(lastElement==NULL){
		LIST_FOREACH(iterator,&FreeMemBlocksList){
			if(size == iterator->size){
				
				lastElement=iterator->prev_next_info.le_next;
				LIST_REMOVE(&FreeMemBlocksList,iterator);
				return iterator;
			}
			else if(size < iterator->size){
				
				struct MemBlock* newBlock = LIST_FIRST(&AvailableMemBlocksList);
				newBlock->size=size;
				newBlock->sva=iterator->sva;
				iterator->size=iterator->size-size;
				iterator->sva=iterator->sva+size;
				lastElement = iterator;
				LIST_REMOVE(&AvailableMemBlocksList, newBlock);
				
				return newBlock;
			}
		}

	}//If function was called for more than one time.
	else{
		
		LIST_FOREACH(iterator,&FreeMemBlocksList){
			//If the iterator didn't reach to the last element that allocated.
			if(iterator->sva<lastElement->sva){
				continue;
				
			}
			else{
				
				if(size == iterator->size){
					lastElement=iterator->prev_next_info.le_next;
					LIST_REMOVE(&FreeMemBlocksList,iterator);
					return iterator;
				}
				else if(size < iterator->size){
					struct MemBlock* newBlock = LIST_FIRST(&AvailableMemBlocksList);
					newBlock->size=size;
					newBlock->sva=iterator->sva;
					iterator->size=iterator->size-size;
					iterator->sva=iterator->sva+size;
					lastElement = iterator;
					LIST_REMOVE(&AvailableMemBlocksList, newBlock);
					return newBlock;
				}
			}
			
		}
		
		LIST_FOREACH(iterator,&FreeMemBlocksList){
			if(size == iterator->size && iterator->sva < lastElement->sva){

				lastElement=iterator->prev_next_info.le_next;
				LIST_REMOVE(&FreeMemBlocksList,iterator);
				return iterator;
			}
			else if(size < iterator->size && iterator->sva < lastElement->sva){
				struct MemBlock* newBlock = LIST_FIRST(&AvailableMemBlocksList);
				newBlock->size=size;
				newBlock->sva=iterator->sva;
				iterator->size=iterator->size-size;
				iterator->sva=iterator->sva+size;
				lastElement = iterator;
				LIST_REMOVE(&AvailableMemBlocksList, newBlock);
				return newBlock;
			}
		}
	}
		return NULL;
}

//===================================================
// [8] INSERT BLOCK (SORTED WITH MERGE) IN FREE LIST:
//===================================================
void insert_sorted_with_merge_freeList(struct MemBlock *blockToInsert){


	cprintf("BEFORE INSERT with MERGE: insert [%x, %x)\n=====================\n", blockToInsert->sva, blockToInsert->sva + blockToInsert->size);
	print_mem_block_lists() ;
//
//	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] insert_sorted_with_merge_freeList
//	// Write your code here, remove the panic and write your code // panic("alloc_block_BF() is not implemented yet...!!");

	 //Case 1 :If FreeMemBlocksList is empty.
	 if(LIST_SIZE(&FreeMemBlocksList)==0 ){
		 LIST_INSERT_HEAD(&FreeMemBlocksList, blockToInsert);


	 }//Case 2:If FreeMemBlocksList has 1 block we have 3 cases inside this case:
	 else if(LIST_SIZE(&FreeMemBlocksList)==1){

         //Case1:If Start address for the element inside FreeMemBlocksList= the end address for  blockToInsert.
		 if(LIST_FIRST(&FreeMemBlocksList)->sva==blockToInsert->size+blockToInsert->sva){

			 LIST_FIRST(&FreeMemBlocksList)->sva= blockToInsert->sva;
			 LIST_FIRST(&FreeMemBlocksList)->size+=blockToInsert->size;

          //Case2:If Start address for the blockToInsert = the end address for the element inside FreeMemBlocksList.
		 }else if(blockToInsert->sva==LIST_FIRST(&FreeMemBlocksList)->sva+LIST_FIRST(&FreeMemBlocksList)->size){

			 LIST_FIRST(&FreeMemBlocksList)->size+=blockToInsert->size;

          //Case3:If we don't have merge.
		 } else{ LIST_INSERT_TAIL(&FreeMemBlocksList, blockToInsert);}


	}//If FreeMemBlocksList has more than one block:
	 else if(LIST_SIZE(&FreeMemBlocksList)>1){
		 struct MemBlock *element;

	  LIST_FOREACH(element, &FreeMemBlocksList) {
			 int count=0;
             count++;//To know if the iterator is on the first element or not.

		//Case 3 : If the blocktoInsert between 2 blocks in FreeMemBlocksLists.
         if((element->sva+element->size) < blockToInsert->sva &&
        	(LIST_NEXT(element)->sva >(blockToInsert->size +blockToInsert->sva)))
          {

        	   LIST_NEXT(element)->prev_next_info.le_prev=blockToInsert;
        	   blockToInsert->prev_next_info.le_next=LIST_NEXT(element);
        	   element->prev_next_info.le_next=blockToInsert;
        	   blockToInsert->prev_next_info.le_prev=element;
        	   LIST_SIZE(&FreeMemBlocksList)++;

        	   break;

           }//Case 4: If the start address for blockToInsert equvilant to the end address for element and
		   //the address for blockToInsert less than the start address for the next element.

            else if((blockToInsert->sva==((element->sva)+(element->size)))&&
            		((blockToInsert->sva)+(blockToInsert->size))<LIST_NEXT(element)->sva){

            	element->size=element->size+blockToInsert->size;
            	blockToInsert->sva=0;
            	blockToInsert->size=0;
            	LIST_INSERT_TAIL(&AvailableMemBlocksList, blockToInsert);
            	break;

            }//Case 5:If the start address for blockToInsert equivalent to  the end address for next element.

            else if((blockToInsert->sva==((LIST_NEXT(element)->sva)+(LIST_NEXT(element)->size)))){

					LIST_NEXT(element)->size=LIST_NEXT(element)->size+blockToInsert->size;
					blockToInsert->sva=0;
					blockToInsert->size=0;
					LIST_INSERT_TAIL(&AvailableMemBlocksList, blockToInsert);
					break;

            }//Case 6:If the end address for blocktoinsert equivalent to the start address for the next element and
            //the start address for blocktoinsert greater than the end address for the element.

            else if(((blockToInsert->sva)+(blockToInsert->size))==LIST_NEXT(element)->sva
            		&&(blockToInsert->sva>((element->sva)+(element->size)))){

					LIST_NEXT(element)->sva=blockToInsert->sva;
					LIST_NEXT(element)->size=LIST_NEXT(element)->size +blockToInsert->size;
					blockToInsert->sva=0;
					blockToInsert->size=0;

					LIST_INSERT_TAIL(&AvailableMemBlocksList, blockToInsert);
					break;

		  }//Case 7:If the end address for blocktoinsert equivalent to the start address for the next element and
         //the start address for blocktoinsert equivalent to  the end address for the element.

            else if((blockToInsert->sva)+(blockToInsert->size)==LIST_NEXT(element)->sva &&
            		(blockToInsert->sva)==(element->sva)+(element->size)){

            	   element->size+=LIST_NEXT(element)->size+blockToInsert->size;

            	   struct MemBlock *element1;
					blockToInsert->size=0;
					blockToInsert->sva=0;
                    element1=LIST_NEXT(element);

					element->prev_next_info.le_next=LIST_NEXT(element)->prev_next_info.le_next;
					LIST_NEXT(LIST_NEXT(element))->prev_next_info.le_prev=element;
					LIST_INSERT_TAIL(&AvailableMemBlocksList,blockToInsert);

					element1->size=0;
					element1->sva=0;
					LIST_INSERT_TAIL(&AvailableMemBlocksList,element1);
					LIST_SIZE(&FreeMemBlocksList)--;
					 break;

            }//Case 8:If the end address for blockToInsert less than the first element in the FreeMemBlocksList.

				else if(((blockToInsert->sva+blockToInsert->size)<element->sva)&&count==1){

					 LIST_INSERT_HEAD(&FreeMemBlocksList, blockToInsert);
					 break;


			}//Case 9:If the end address for blockToInsert equivalent to the start address for first element and
            //the size of blockToInsert less than the end address for the first element.

				else if(((blockToInsert->sva+blockToInsert->size)==element->sva)
						&&(blockToInsert->size<element->size+element->sva)&&count==1){


					element->size=element->sva+element->size;
					element->sva=blockToInsert->sva;
					blockToInsert->sva=0;
					blockToInsert->size=0;

					LIST_INSERT_TAIL(&AvailableMemBlocksList, blockToInsert);
					 break;

				}

	 }

 }
	cprintf("\nAFTER INSERT with MERGE:\n=====================\n");
	print_mem_block_lists();




}
