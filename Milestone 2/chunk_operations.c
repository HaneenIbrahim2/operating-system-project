/*
 * chunk_operations.c
 *
 *  Created on: Oct 12, 2022
 *      Author: HP
 */

#include <kern/trap/fault_handler.h>
#include <kern/disk/pagefile_manager.h>
#include "kheap.h"
#include "memory_manager.h"


/******************************/
/*[1] RAM CHUNKS MANIPULATION */
/******************************/

//===============================
// 1) CUT-PASTE PAGES IN RAM:
//===============================
//This function should cut-paste the given number of pages from source_va to dest_va
//if the page table at any destination page in the range is not exist, it should create it
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int cut_paste_pages(uint32* page_directory, uint32 source_va, uint32 dest_va, uint32 num_of_pages)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] cut_paste_pages
		// Write your code here, remove the panic and write your code
		//panic("cut_paste_pages() is not implemented yet...!!");
		uint32 *ptr_page_table1 = NULL;
		uint32 *ptr_page_table2 = NULL;
		struct FrameInfo *ptr_frame_info;

		uint32 sourceva = ROUNDDOWN(source_va,PAGE_SIZE);
		uint32 destva = ROUNDDOWN(dest_va,PAGE_SIZE);

		uint32 size_chunk = num_of_pages * PAGE_SIZE;
		uint32 size = ROUNDUP(size_chunk + dest_va,PAGE_SIZE);

		for(int va = destva ; va < size ; va+=PAGE_SIZE ){
			ptr_frame_info = get_frame_info(page_directory, va, &ptr_page_table2);
			if(ptr_frame_info != NULL)
				return -1;
		}
		for(int va2 = destva ; va2 < size ; va2+=PAGE_SIZE ){
			int ret = get_page_table(page_directory, va2, &ptr_page_table2) ;
				if (ret == TABLE_NOT_EXIST)
					ptr_page_table2 = create_page_table(page_directory, va2) ;
				ptr_frame_info = get_frame_info(page_directory,sourceva,&ptr_page_table1);
				uint32 entry = ptr_page_table1[PTX(sourceva)];
				map_frame(page_directory,ptr_frame_info,va2,4095&entry);
				unmap_frame(page_directory,sourceva);
				sourceva+=PAGE_SIZE;

		}
		return 0;
}

//===============================
// 2) COPY-PASTE RANGE IN RAM:
//===============================
//This function should copy-paste the given size from source_va to dest_va
//if the page table at any destination page in the range is not exist, it should create it
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int copy_paste_chunk(uint32* page_directory, uint32 source_va, uint32 dest_va, uint32 size)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] copy_paste_chunk
		// Write your code here, remove the panic and write your code
		//panic("copy_paste_chunk() is not implemented yet...!!");
			uint32 old = source_va+size;
			uint32 eva = ROUNDUP(dest_va+size, PAGE_SIZE);
			uint32 new_size = eva - ROUNDDOWN(dest_va, PAGE_SIZE);
			uint32 no_frames = new_size / PAGE_SIZE;

			struct FrameInfo* dst_frame_ptr = NULL;
			struct FrameInfo* src_frame_ptr = NULL;
			uint32* dst_page_table = NULL;
			uint32* src_page_table = NULL;
			int ret;
			uint32 entry_dst;
			uint32 entry_src;
			for(int i=0; i<no_frames; i++){
				ret = get_page_table(page_directory, dest_va+(i*PAGE_SIZE), &dst_page_table);
				if(ret == TABLE_IN_MEMORY){
					entry_dst = dst_page_table[PTX(dest_va+(i*PAGE_SIZE))];
					if((entry_dst & PERM_PRESENT) > 0  && (entry_dst & PERM_WRITEABLE) <=0 )
						return -1;
				}
			}

			for(int i=0; i<no_frames; i++){
				ret = get_page_table(page_directory, dest_va+(i*PAGE_SIZE), &dst_page_table);
				if(ret == TABLE_NOT_EXIST)
					dst_page_table = create_page_table(page_directory, dest_va+(i*PAGE_SIZE));
				dst_frame_ptr = get_frame_info(page_directory, dest_va+(i*PAGE_SIZE), &dst_page_table);
				src_frame_ptr = get_frame_info(page_directory, source_va+(i*PAGE_SIZE), &src_page_table);
				entry_src = src_page_table[PTX(source_va+(i*PAGE_SIZE))];
				if(dst_frame_ptr == NULL){
					allocate_frame(&dst_frame_ptr);
					map_frame(page_directory, dst_frame_ptr, dest_va+(i*PAGE_SIZE), PERM_WRITEABLE | (PERM_USER & entry_src));
				}
				// loop to copy each byte
				int z = 0;
				for(z ; z<PAGE_SIZE; z++){
					if(source_va+(i*PAGE_SIZE) >= old)
						break;
					char *k = (char*)source_va+(i*PAGE_SIZE);
					char *j = (char*)dest_va+(i*PAGE_SIZE);
					*j = *k;
					source_va = source_va+1;
					dest_va = dest_va+1;
				}
				source_va = source_va-z;
				dest_va = dest_va-z;
			}
			return 0;
}


//===============================
// 3) SHARE RANGE IN RAM:
//===============================
//This function should share the given size from dest_va with the source_va
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int share_chunk(uint32* page_directory, uint32 source_va,uint32 dest_va, uint32 size, uint32 perms)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] share_chunk
		// Write your code here, remove the panic and write your code
		//panic("share_chunk() is not implemented yet...!!");

				uint32 Source= ROUNDUP(source_va + size,PAGE_SIZE);
				uint32 new_size1 = Source - ROUNDDOWN(source_va,PAGE_SIZE);

			    uint32 dest = ROUNDUP(dest_va + size,PAGE_SIZE);
				uint32 new_size2 = dest - ROUNDDOWN(dest_va,PAGE_SIZE);
				uint32 no_frames = new_size2 / PAGE_SIZE;

		        uint32 *ptr_page_table_source = NULL;
				uint32 *ptr_page_table_dest = NULL;

				struct FrameInfo* ptr_Frame_Info =NULL;

				uint32 new_add_des=dest;
				uint32 new_add_source=Source;

				 get_page_table(page_directory, source_va, &ptr_page_table_source);
				 get_page_table(page_directory,dest_va, &ptr_page_table_dest);

				if(ptr_page_table_dest==NULL){
					ptr_page_table_dest=create_page_table(page_directory,dest_va);
				}
				for(int i=0 ; i<no_frames;i++){
					 ptr_Frame_Info = get_frame_info(page_directory,new_add_des-PAGE_SIZE , &ptr_page_table_dest) ;
					if(ptr_Frame_Info==NULL){

						new_add_des=new_add_des-PAGE_SIZE;

					 }else
						return -1;
				}
	             	new_add_source=Source-PAGE_SIZE;
					new_add_des=dest-PAGE_SIZE;

					for(int i=0 ; i<no_frames;i++){
						    ptr_Frame_Info=get_frame_info(page_directory,new_add_source, &ptr_page_table_source) ;
							map_frame(page_directory,ptr_Frame_Info,new_add_des,perms);
							new_add_des=new_add_des-PAGE_SIZE;
							new_add_source=new_add_source-PAGE_SIZE;
				    }
		  return 0;
}

//===============================
// 4) ALLOCATE CHUNK IN RAM:
//===============================
//This function should allocate in RAM the given range [va, va+size)
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int allocate_chunk(uint32* page_directory, uint32 va, uint32 size, uint32 perms)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] allocate_chunk
		// Write your code here, remove the panic and write your code
		//panic("allocate_chunk() is not implemented yet...!!");

		uint32 eva = ROUNDUP(va + size,PAGE_SIZE);
		uint32 new_size = eva - ROUNDDOWN(va,PAGE_SIZE);
		uint32 no_frames = new_size / PAGE_SIZE;

		uint32* pageTable_ptr = NULL;
		struct FrameInfo * frame_ptr = NULL;

		// check if the whole chunk is free
		// if not return -1;
		for(int i=0;i<no_frames;i++){
			frame_ptr = get_frame_info(page_directory,va+(i*PAGE_SIZE),&pageTable_ptr);
			if(frame_ptr != NULL)
				return -1;
		}
		// allocate the chunk and create necessary non existing tables
		for(int i=0;i<no_frames;i++){
			frame_ptr = get_frame_info(page_directory,va+(i*PAGE_SIZE),&pageTable_ptr);
			if(pageTable_ptr == NULL)
				create_page_table(page_directory,va+(i*PAGE_SIZE));
			allocate_frame(&frame_ptr);
			map_frame(page_directory,frame_ptr,va+(i*PAGE_SIZE),perms);
		}

		return 0;
}

/*BONUS*/
//=====================================
// 5) CALCULATE ALLOCATED SPACE IN RAM:
//=====================================
void calculate_allocated_space(uint32* page_directory, uint32 sva, uint32 eva, uint32 *num_tables, uint32 *num_pages)
{
	//TODO: [PROJECT MS2 - BONUS] [CHUNK OPERATIONS] calculate_allocated_space
		// Write your code here, remove the panic and write your code
		//panic("calculate_allocated_space() is not implemented yet...!!");
		int tcount = 0;
		int pcount = 0;

		uint32 new_size = ROUNDUP(eva,PAGE_SIZE) - ROUNDDOWN(sva,PAGE_SIZE);
		uint32 no_frames = new_size / PAGE_SIZE;

		uint32* pageTable_ptr = NULL;
		uint32* tmp = NULL;
		for(int i=0;i<no_frames;i++){
			struct FrameInfo * frame_ptr = get_frame_info(page_directory,sva+(i*PAGE_SIZE),&pageTable_ptr);
			if(frame_ptr != NULL)
				pcount++;
			if(tmp != pageTable_ptr && pageTable_ptr != NULL )
				tcount++;

			tmp = pageTable_ptr;
		}

		*num_tables = tcount;
		*num_pages = pcount;
}

/*BONUS*/
//=====================================
// 6) CALCULATE REQUIRED FRAMES IN RAM:
//=====================================
// calculate_required_frames:
// calculates the new allocation size required for given address+size,
// we are not interested in knowing if pages or tables actually exist in memory or the page file,
// we are interested in knowing whether they are allocated or not.
uint32 calculate_required_frames(uint32* page_directory, uint32 sva, uint32 size)
{
	//TODO: [PROJECT MS2 - BONUS] [CHUNK OPERATIONS] calculate_required_frames
		// Write your code here, remove the panic and write your code
		//panic("calculate_required_frames() is not implemented yet...!!");
		int count = 0;

		uint32 eva = ROUNDUP(sva + size,PAGE_SIZE);
		uint32 new_size = eva - ROUNDDOWN(sva,PAGE_SIZE);
		uint32 no_frames = new_size / PAGE_SIZE;

		uint32* pageTable_ptr = NULL;
		uint32 tmp = -1;

		for(int i=0;i<no_frames;i++){
			struct FrameInfo * frame_ptr = get_frame_info(page_directory,sva+(i*PAGE_SIZE),&pageTable_ptr);
			// not allocated page
			if(frame_ptr == NULL){
				count++;
				// if the page is not allocated then maybe its table is not allocated too
				// But I first check if I counted it already
				if(pageTable_ptr == NULL && tmp != PDX(sva+(i*PAGE_SIZE))){
					count++;
				}
			}
			tmp=PDX(sva+(i*PAGE_SIZE));
		}
		return count;
}

//=================================================================================//
//===========================END RAM CHUNKS MANIPULATION ==========================//
//=================================================================================//

/*******************************/
/*[2] USER CHUNKS MANIPULATION */
/*******************************/

//======================================================
/// functions used for USER HEAP (malloc, free, ...)
//======================================================

//=====================================
// 1) ALLOCATE USER MEMORY:
//=====================================
void allocate_user_mem(struct Env* e, uint32 virtual_address, uint32 size)
{
	// Write your code here, remove the panic and write your code
	panic("allocate_user_mem() is not implemented yet...!!");
}

//=====================================
// 2) FREE USER MEMORY:
//=====================================
void free_user_mem(struct Env* e, uint32 virtual_address, uint32 size)
{
	//TODO: [PROJECT MS3] [USER HEAP - KERNEL SIDE] free_user_mem
	// Write your code here, remove the panic and write your code
	panic("free_user_mem() is not implemented yet...!!");

	//This function should:
	//1. Free ALL pages of the given range from the Page File
	//2. Free ONLY pages that are resident in the working set from the memory
	//3. Removes ONLY the empty page tables (i.e. not used) (no pages are mapped in the table)
}

//=====================================
// 2) FREE USER MEMORY (BUFFERING):
//=====================================
void __free_user_mem_with_buffering(struct Env* e, uint32 virtual_address, uint32 size)
{
	// your code is here, remove the panic and write your code
	panic("__free_user_mem_with_buffering() is not implemented yet...!!");

	//This function should:
	//1. Free ALL pages of the given range from the Page File
	//2. Free ONLY pages that are resident in the working set from the memory
	//3. Free any BUFFERED pages in the given range
	//4. Removes ONLY the empty page tables (i.e. not used) (no pages are mapped in the table)
}

//=====================================
// 3) MOVE USER MEMORY:
//=====================================
void move_user_mem(struct Env* e, uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
{
	//TODO: [PROJECT MS3 - BONUS] [USER HEAP - KERNEL SIDE] move_user_mem
	//your code is here, remove the panic and write your code
	panic("move_user_mem() is not implemented yet...!!");

	// This function should move all pages from "src_virtual_address" to "dst_virtual_address"
	// with the given size
	// After finished, the src_virtual_address must no longer be accessed/exist in either page file
	// or main memory

	/**/
}

//=================================================================================//
//========================== END USER CHUNKS MANIPULATION =========================//
//=================================================================================//

