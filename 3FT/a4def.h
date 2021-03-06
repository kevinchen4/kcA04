/*--------------------------------------------------------------------*/
/* a4def.h                                                            */
/* Author: Christopher Moretti                                        */
/*--------------------------------------------------------------------*/

#ifndef A4DEF_INCLUDED
#define A4DEF_INCLUDED

/* Return statuses */
enum { SUCCESS,
       INITIALIZATION_ERROR, PARENT_CHILD_ERROR , ALREADY_IN_TREE,
       NO_SUCH_PATH, CONFLICTING_PATH, NOT_A_DIRECTORY, NOT_A_FILE,
       MEMORY_ERROR
};

/* In lieu of a proper boolean datatype */
enum bool { FALSE, TRUE };
/* boolean is the name of the bool */
typedef enum bool boolean;
/*typedef struct node* Node_T;
  typedef struct file* File_T;*/


#endif
