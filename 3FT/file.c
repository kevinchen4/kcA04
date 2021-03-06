/*--------------------------------------------------------------------*/
/* file.c                                                             */
/* Author: Kevin Chen                                     */
/*--------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>


#include "file.h"

struct file {
   char* path;
   void* contents;
   size_t len;
};

/*
   File_new creates a new file based on the path, contents and length.
   If there is not enough space. Return NULL. Otherwise, return the
   output.
*/
static File_T File_new(char* path, void* contents, size_t length) {
   File_T output;
   output = (File_T) malloc(sizeof(struct file));
   if (output == NULL) {
      return NULL;
   }
   output->path = (char*) malloc(strlen(path) + 1);
   if(output->path == NULL) {
      free(output);
      return NULL;
   }
   strcpy(output->path, path);
   output->contents = contents;
   output->len = length;
   return output;
}

static void File_destory(File_T input)  {
   free(input->path);
   free(input);
}


/*
  Node_compare compares file1 and file2 based on their paths.
  Returns <0, 0, or >0 if file1 is less than,
  equal to, or greater than file2, respectively.
*/
static int File_compare(File_T file1, File_T file2) {
   return strcmp(file1->path, file2->path);
}

int File_insert(Node_T inNode, char* path, void* contents, size_t length)  {
   size_t loc = 0;
   
   File_T newFile;
   File_T tempFile = File_new(path, NULL, 0);
   if(DynArray_bsearch(Node_getFiles(inNode), tempFile, &loc,
               (int (*)(const void*, const void*)) File_compare)) {
      File_destory(tempFile);
      return ALREADY_IN_TREE;
   }
   File_destory(tempFile);
   newFile = File_new(path, contents, length);
   if(newFile == NULL) {
      return MEMORY_ERROR;
   }
   if(DynArray_addAt(Node_getFiles(inNode), loc, newFile)) {
      return SUCCESS;
   }
   free(newFile);
   return MEMORY_ERROR;
}

int File_contains(Node_T inNode, char* path) {
   size_t loc = 0;
   File_T tempFile = File_new(path, NULL, 0);
   if(DynArray_bsearch(Node_getFiles(inNode), tempFile, &loc,
                       (int (*)(const void*, const void*)) File_compare)) {
      File_destory(tempFile);
      return TRUE;
   }
   File_destory(tempFile);
   return FALSE;
}

/*
    File_getFile is a static function that takes in a node inNode annd
    a path and gets the path from the file if it exist. If it doesn't
    return NULL.
*/
static File_T File_getFile(Node_T inNode, char* path) {
   size_t loc = 0;
   File_T tempFile = File_new(path, NULL, 0);
   int exist = DynArray_bsearch(Node_getFiles(inNode), tempFile, &loc,
                                (int (*)(const void*, const void*)) File_compare);
   File_destory(tempFile);
   
   if (exist == 0) {
      return NULL;
   }
   return DynArray_get(Node_getFiles(inNode), loc);
}

void* File_replace(Node_T inNode, char* path, void* contents, 
   size_t length) {
   File_T outFile = File_getFile(inNode, path);
   void* output;
   if(outFile == NULL) {
      return NULL;
   }
   output = outFile->contents;
   outFile->contents = contents;
   outFile->len = length;
   return output;
}

void* File_getcontents(Node_T inNode, char* path) {
   File_T outFile = File_getFile(inNode, path);
   return outFile->contents;
}

char* File_getFileName(Node_T inNode, size_t fileID) {
   File_T outfile = DynArray_get(Node_getFiles(inNode), fileID);
   if (outfile == NULL) {
      return NULL;
   }
   return outfile->path;
}

size_t File_getNumFiles(Node_T inNode, char* path) {
   return DynArray_getLength(Node_getFiles(inNode));
}

int File_rmFile(Node_T inNode, char* path){
   size_t loc = 0;
   File_T tempFile = File_new(path, NULL, 0);
   DynArray_bsearch(Node_getFiles(inNode), tempFile, &loc,
                                (int (*)(const void*, const void*)) File_compare);
   File_destory(tempFile);
   File_destory((File_T) DynArray_removeAt(Node_getFiles(inNode), loc));
   return SUCCESS;
}

void File_freeAll(Node_T inNode) {
   File_T tempFile;
   size_t i;
   for(i = 0; i < DynArray_getLength(Node_getFiles(inNode)); i++)
   {
      tempFile = DynArray_get(Node_getFiles(inNode), i);
      free(tempFile->path);
      free(tempFile);
   }
   DynArray_free(Node_getFiles(inNode));
}

size_t File_getLength(Node_T inNode, char* path) {
   File_T outFile = File_getFile(inNode, path);
   return outFile->len;
}
