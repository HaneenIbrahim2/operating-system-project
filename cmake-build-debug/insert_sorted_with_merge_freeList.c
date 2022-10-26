void insert_sorted_with_merge_freeList(struct MemBlock *blockToInsert)
{


    cprintf("BEFORE INSERT with MERGE: insert [%x, %x)\n=====================\n", blockToInsert->sva, blockToInsert->sva + blockToInsert->size);
    print_mem_block_lists() ;


    //TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] insert_sorted_with_merge_freeList
    // Write your code here, remove the panic and write your code

    //Case 1
    if(LIST_SIZE(&FreeMemBlocksList)==0 ){
        LIST_INSERT_HEAD(&FreeMemBlocksList, blockToInsert);


    }//Case 2
    else if(LIST_SIZE(&FreeMemBlocksList)==1){

        //Case1
        if(LIST_FIRST(&FreeMemBlocksList)->sva==blockToInsert->size+blockToInsert->sva){

            LIST_FIRST(&FreeMemBlocksList)->sva= blockToInsert->sva;
            LIST_FIRST(&FreeMemBlocksList)->size+=blockToInsert->size;

            //Case2
        }else if(blockToInsert->sva==LIST_FIRST(&FreeMemBlocksList)->sva+LIST_FIRST(&FreeMemBlocksList)->size){

            LIST_FIRST(&FreeMemBlocksList)->size+=blockToInsert->size;

            //Case3
        }else{

            LIST_INSERT_TAIL(&FreeMemBlocksList, blockToInsert);
        }

    }

    else if(LIST_SIZE(&FreeMemBlocksList)>1){
        struct MemBlock *element;

        LIST_FOREACH(element, &FreeMemBlocksList) {

            int count=0;
            count++;

            //Case 3
            if((element->sva+element->size) < blockToInsert->sva &&
               (LIST_NEXT(element)->sva >(blockToInsert->size +blockToInsert->sva)))
            {

                LIST_NEXT(element)->prev_next_info.le_prev=blockToInsert;
                blockToInsert->prev_next_info.le_next=LIST_NEXT(element);
                element->prev_next_info.le_next=blockToInsert;
                blockToInsert->prev_next_info.le_prev=element;
                LIST_SIZE(&FreeMemBlocksList)=LIST_SIZE(&FreeMemBlocksList)+1;

                break;

            }//Case 4

            else if((blockToInsert->sva==((element->sva)+(element->size)))&&
                    ((blockToInsert->sva)+(blockToInsert->size))<LIST_NEXT(element)->sva){

                element->size=element->size+blockToInsert->size;
                blockToInsert->sva=0;
                blockToInsert->size=0;
                LIST_INSERT_TAIL(&AvailableMemBlocksList, blockToInsert);
                break;

            }//Case 5

            else if((blockToInsert->sva==((LIST_NEXT(element)->sva)+(LIST_NEXT(element)->size)))){

                LIST_NEXT(element)->size=LIST_NEXT(element)->size+blockToInsert->size;
                blockToInsert->sva=0;
                blockToInsert->size=0;
                LIST_INSERT_TAIL(&AvailableMemBlocksList, blockToInsert);
                break;
            }//Case 6

            else if(((blockToInsert->sva)+(blockToInsert->size))==LIST_NEXT(element)->sva
                    &&(blockToInsert->sva>((element->sva)+(element->size)))){

                LIST_NEXT(element)->sva=blockToInsert->sva;
                LIST_NEXT(element)->size=LIST_NEXT(element)->size +blockToInsert->size;
                blockToInsert->sva=0;
                blockToInsert->size=0;

                LIST_INSERT_TAIL(&AvailableMemBlocksList, blockToInsert);
                break;

            }//Case 7
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

            }//Case 8

            else if(((blockToInsert->sva+blockToInsert->size)<element->sva)&&count==1){

                LIST_INSERT_HEAD(&FreeMemBlocksList, blockToInsert);
                break;


            }//Case 9

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
