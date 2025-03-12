// To-do: Debugging to get asum.s to run and produce asum.o. There's a print
// queue fn to use, alongside a print map fn to help.

/*
 * @authors Surya Duraivenkatesh, Josh Tittiranonda
 * @date: 03/2025
 * This is the OHS Y86-64 Emulator! In this program, assembly files are read and
parsed into a queue of commands, which is ultimately put into a linked list of
commands split by their struct elements. Finally, this queue is processed by the
Y86-64 Emulator, which produces a linked list of machine code commands, which
are ultimately printed to the user. Hope you enjoy!

[References]
Bryant, Randal E., and David R. O'Hallaron. Computer Systems: A Programmer's
Perspective.
 */

#include "Queue.c"
#include "Queue.h"
#include "commandList.h"
#include "symbolicMap.c"
#include "symbolicMap.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *interface();
map *file_parsing(char *, inputnode *);
int reg_num(char *);
map *commandLinkedList(inputnode *, Queue *, map *);
outputnode *assemble(inputnode *, map *);

map *symbolicNames;

int main() {
  inputnode *startingList = malloc(sizeof(inputnode)); // To print in deassembler-style later
  outputnode *print;
  char *filename = interface();
  symbolicNames = file_parsing(filename, startingList);
  print = assemble(startingList, symbolicNames);
  printMap(symbolicNames);
  printf("\n\nFinal output\n\n");
  printf("Note that the address code is commented out to quickly analyze the instruction strings. \n The stack register will come up as F (not 100) for similar reasons, and the symbolic name code is also commented out for now -- (they'll show up as 0 for now).");
  printf("Last thing to debug: little endian/size of the instructions.");
  
  while (print->next != NULL) { // Null terminator of file causes an issue during assembling, so the last element will not be valid.
    printf("0x0%lu\t", print->memoryAddress);
    printf("%s\n", (print->data));
    print = print->next;
  }
}

/*
 * Function for the user to input file name of the file they want to assemble.
 * @return: char* filename: file name, <= 50 characters
 */
char *interface() {
  char *str = malloc(50);
  printf("%s\n", "Welcome to the OHS Y86-64 Emulator! This program will help "
                 "convert your assembly instructions into machine code. Please "
                 "enter the file you'd like to assemble: ");
  int successfulScan = scanf(
      "%49s",
      str); // I'll assume the file name won't exceed 50 characters. Note, scanf
            // returns a value indicating the number of successful elements
            // scanned, so I am saving it to silence the warning easily.
  return str;
}

/*
 * Function to parse the file into an array of lines.
 * @param: char* filename: file name, <= 50 characters
 */
map *file_parsing(char *filename, inputnode *head) {
  map *map = mapCreation(MAX_QUEUE_SIZE);
  inputnode *curr = head;
  Queue *lineQueue = queueCreation(MAX_LINE_COUNT);

  // File-reading code
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("File open failed. Please contact the developer to fix a logical "
           "error within the program.");
    // Would be for debugging only.
  }
  char line[4096];
  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';
    //    printf("%s", line); // Debugging
    enqueue(lineQueue, line);
  }
  fclose(file);

  // printQueue(lineQueue);
  //    printf("arrived before calling command list fn\n");
  commandLinkedList(curr, lineQueue, map);
  return map;
}

/*
 * Functions to create a linked list of commands.
 * @param: *head, pointer to the head of the desired input linked list (defined
 * out of the function scope)
 * @param: *lineQueue, pointer to the queue of lines that'll be processed into a
 * linked list of assembly commands
 */
map *commandLinkedList(inputnode *list, Queue *lineQueue, map *m) {
  inputnode *ret = list; // Pointer to first element.
  inputnode *curr = ret; // Pointer to first element, will be changed.

  //   printf("arrived before calling empty queue while lp\n"); // Debugging
  //   purposes
  bool symbolicNameCase = false;
  char *word;

  /*
  char *stackName = "Stack";
  element* data = malloc(sizeof(element));
  data->name = malloc(strlen(stackName)*sizeof(char) + 1);
  data->address = 0x100;
  add(m, data);
  */
  
  while (!emptyQueue(lineQueue)) {
    if (!symbolicNameCase) {
      word = strtok(dequeue(lineQueue), " "); // Tokenize line into words
    } else {
      word = strtok(NULL, " ");
    }
    symbolicNameCase = false;

    command *newCommand; // Pointer to new command object.
    newCommand = malloc(sizeof(command));
    newCommand->name = malloc(sizeof(char) * 100); // Arbitrary assumption
    newCommand->rA = malloc(sizeof(char) * 100);
    newCommand->rB = malloc(sizeof(char) * 100);
    newCommand->other = malloc(sizeof(char) * 100);

    newCommand->name[0] = '\0';
    newCommand->rA[0] = '\0';
    newCommand->rB[0] = '\0';
    newCommand->other[0] = '\0';
    newCommand->symbol = false;
    newCommand->directive = false;
    newCommand->pos = false;
    newCommand->align = false;
    newCommand->long_or_quad = false;
    newCommand->usesSymbolicName = false;

    curr->data = newCommand; // Modifies data of first element
    curr->next = NULL;
    //      printf("arrived before calling if statement for dir\n"); //
    //      Debugging purposes
    //    printf("||%s and %ld|| ", word, strlen(word));
    //    printf("last letter %c\n", word[strlen(word) - 1]); // Debugging
    //    purposes")

    if (word[0] == '.') { // Assembler directive case
      newCommand->directive = true;
      if (!strcmp(word, ".long") || !strcmp(word, ".quad")) {
        newCommand->long_or_quad = true;
        strncpy(newCommand->name, word, strlen(word));
        newCommand->name[strlen(word)] = '\0';
        char *token = strtok(NULL, " ");
//        printf("AAAAA  %s\n", token);
        newCommand->value = strtoul(token, (char **)0, 0);

      } else if (!strcmp(word, ".pos")) {
        char *location = strtok(NULL, " ");
        strncpy(newCommand->name, word, strlen(word));
        newCommand->name[strlen(word)] = '\0';

        newCommand->pos = true;
        newCommand->position =
            atoi(location); // Shifts memory address in driver program to the
                            // given position.
      } else if (!strcmp(word, ".align")) {
        char *shift = strtok(NULL, " ");
        strncpy(newCommand->name, word, strlen(word));
        newCommand->name[strlen(word)] = '\0';

        newCommand->align = true;
        newCommand->alignment = atoi(
            shift); // Aligns to a alignment-byte boundary in driver program.
      } else {
        continue; // The directive is then not important (e.g. .file or .globl).
                  // Note, there exists cases like ".LFB22:", so we can't allow
                  // it to hit the next elif.
      }
    } else if ((word != NULL) &&
               (word[strlen(word) - 1] == ':')) { // Symbolic name case
                                                  //      printf("HI!");
      newCommand->symbolicName = malloc(sizeof(element));
      newCommand->symbolicName->name = malloc(strlen(word) + 1);
      strncpy((newCommand->symbolicName)->name, word, strlen(word) - 1);
      (newCommand->symbolicName)->name[strlen(word) - 1] = '\0';

      strncpy(newCommand->name, word, strlen(word) - 1);
      newCommand->name[strlen(word) - 1] = '\0';

      newCommand->symbol = true;
      newCommand->directive = true;

      //      printf("Symbolic name: %s\n", (newCommand->symbolicName)->name);
      //      printf("Symbolic address: %d\n",
      //      (newCommand->symbolicName)->address);

      add(m, newCommand->symbolicName); // Adding to hashmap

      symbolicNameCase = true;
    } else { // Non-directive command case
      newCommand->directive =
          false; // Note this is unnecessary, just for clarity.
      // Here, we'd use "strtok(NULL, "\t"); to get each next word, i.e. do it
      // each time you need a next word
      if (!strcmp(word, "halt") || !strcmp(word, "nop")) {
        strncpy(
            newCommand->name, word,
            strlen(
                word)); // All that's needed is to set the name of the command.
        newCommand->name[strlen(word)] = '\0';

      } else if (word[0] == 'j') {
        strncpy(newCommand->name, word, strlen(word));
        newCommand->name[strlen(word)] = '\0';

        char *token = strtok(NULL, " ");
        strncpy(newCommand->other, token, strlen(token)); // Gets next word
        newCommand->other[strlen(token)] = '\0';

      } else if (!strcmp(word, "cmov") || !strcmp(word, "cmovle") ||
                 !strcmp(word, "cmovl") || !strcmp(word, "cmove") ||
                 !strcmp(word, "cmovne") || !strcmp(word, "cmovge") ||
                 !strcmp(word, "cmovg")) {
        strncpy(newCommand->name, word, strlen(word));
        newCommand->name[strlen(word)] = '\0';

        char *token = strtok(NULL, " ");
        strncpy(newCommand->rA, token, strlen(token));
        newCommand->rA[strlen(token)] = '\0';

        char *token2 = strtok(NULL, " ");
        strncpy(newCommand->rB, token2, strlen(token2));
        newCommand->rB[strlen(token2)] = '\0';

      } else if (!strcmp(word, "addl") || !strcmp(word, "subl") ||
                 !strcmp(word, "andl") ||
                 !strcmp(word, "xorl")) { // Same as above, but written out for clarity.
        strncpy(newCommand->name, word, strlen(word));
        newCommand->name[strlen(word)] = '\0';

        char *token = strtok(NULL, " ");
        strncpy(newCommand->rA, token, strlen(token) - 1);
        newCommand->rA[strlen(token) - 1] = '\0';

        char *token2 = strtok(NULL, " ");
        strncpy(newCommand->rB, token2, strlen(token2));
        newCommand->rB[strlen(token2)] = '\0';

      } else if (!strcmp(word, "pushl") || !strcmp(word, "popl")) {

        char *token = strtok(NULL, " ");
        strncpy(newCommand->rA, token, strlen(token));
        newCommand->rA[strlen(token)] = '\0';
        strncpy(newCommand->name, word, strlen(word));
        newCommand->name[strlen(word)] = '\0';
      } else if (!strcmp(word, "call")) {
        strncpy(newCommand->name, word, strlen(word));
        newCommand->name[strlen(word)] = '\0';

        char *token = strtok(NULL, "\t");
        strncpy(newCommand->other, token, strlen(token)); // Gets next word
        newCommand->other[strlen(token)] = '\0';

      } else if (!strcmp(word, "ret")) {
        strncpy(newCommand->name, word, strlen(word));
        newCommand->other[strlen(word)] = '\0';
      } /*else if (!strcmp(word, "irmovl") || !strcmp(word, "rmmovl")) {
        strncpy(newCommand->name, word, strlen(word));
        newCommand->name[strlen(word)] = '\0';
        
        char *token = strtok(NULL, " ");

        if (*token == 'Stack,') {
          strncpy(newCommand->rA, 0x100, strlen(token));
        }

        
        strncpy(newCommand->rA, token, strlen(token) - 1);
        newCommand->rA[strlen(token)] = '\0';

        char *token2 = strtok(NULL, " ");
        strncpy(newCommand->rB, token2, strlen(token2));
        newCommand->rB[strlen(token2)] = '\0';
        
      }
      */
      else if (!strcmp(word, "rrmovl")) {
        strncpy(newCommand->name, word, strlen(word));
        newCommand->name[strlen(word)] = '\0';

        char *token = strtok(NULL, " ");
        strncpy(newCommand->rA, token, strlen(token) - 1);
        newCommand->rA[strlen(token) - 1] = '\0';

        char *token2 = strtok(NULL, " ");
        strncpy(newCommand->rB, token2, strlen(token2));
        newCommand->rB[strlen(token2)] = '\0';

      } else if (!strcmp(word, "irmovl")) {
        strncpy(newCommand->name, word, strlen(word));
        newCommand->name[strlen(word)] = '\0';

        char *token = strtok(NULL, " ");
        strncpy(newCommand->other, token, strlen(token) - 1);
        newCommand->other[strlen(token) - 1] = '\0';

        char *token2 = strtok(NULL, " ");
        strncpy(newCommand->rB, token2, strlen(token2));
        newCommand->rB[strlen(token2)] = '\0';

      } else if (!strcmp(word, "rmmovl")) {
        strncpy(newCommand->name, word, strlen(word));
        newCommand->name[strlen(word)] = '\0';

        char *token = strtok(NULL, " (),"); // Four separate delimeters
        char *token2 = strtok(NULL, " (),");
        char *token3 = strtok(NULL, " (),");

        if (token3 != NULL) {
          strncpy(newCommand->rA, token, strlen(token));
          newCommand->rA[strlen(token)] = '\0';
          
          strncpy(newCommand->other, token2, strlen(token2));

          strncpy(newCommand->rB, token3, strlen(token3));
          newCommand->rB[strlen(token3)] = '\0';
        }
        else {
          strncpy(newCommand->rA, token, strlen(token));
          newCommand->rA[strlen(token)] = '\0';

          strncpy(newCommand->rB, token2, strlen(token2));
          newCommand->rB[strlen(token2)] = '\0';
        }
      } else if (!strcmp(word, "mrmovl")) {
        strncpy(newCommand->name, word, strlen(word));
        newCommand->name[strlen(word)] = '\0';

        char *token = strtok(NULL, " (),"); // Four separate delimeters
        char *token2 = strtok(NULL, " (),");
        char *token3 = strtok(NULL, " (),");

        if (token3 != NULL) {
          strncpy(newCommand->other, token, strlen(token));

          strncpy(newCommand->rB, token2, strlen(token2));
          newCommand->rB[strlen(token2)] = '\0';

          strncpy(newCommand->rA, token3, strlen(token3));
          newCommand->rA[strlen(token3)] = '\0';
        }
        else {
          strncpy(newCommand->rB, token, strlen(token));
          newCommand->rB[strlen(token)] = '\0';

          strncpy(newCommand->rA, token2, strlen(token2));
          newCommand->rA[strlen(token2)] = '\0';
        }
      }
    }
    printf("Command: Name (%s), rA (%s), rB (%s), other (%s)\n",
           newCommand->name, newCommand->rA, newCommand->rB, newCommand->other);
//    printf("If long, %lu is value\n", newCommand->value);

    inputnode *next;
    next = malloc(sizeof(inputnode));
    curr->next = next;
    curr = curr->next;
    curr->data = newCommand;
    curr->next = NULL;
  }
  return m;
}

/*
    Converts register to register's machine-code representation.
    @param *reg register to convert
    @return: char* register's binary representation
*/
int reg_num(char *reg) {
  if (strcmp(reg, "%eax") == 0)
    return 0;
  else if (strcmp(reg, "%ecx") == 0)
    return 1;
  else if (strcmp(reg, "%edx") == 0)
    return 2;
  else if (strcmp(reg, "%ebx") == 0)
    return 3;
  else if (strcmp(reg, "%esp") == 0)
    return 4;
  else if (strcmp(reg, "%ebp") == 0)
    return 5;
  else if (strcmp(reg, "%esi") == 0)
    return 6;
  else if (strcmp(reg, "%edi") == 0)
    return 7;
  else if (strcmp(reg, "%e8") == 0)
    return 8;
  else if (strcmp(reg, "%e9") == 0)
    return 9;
  else if (strcmp(reg, "%e10") == 0)
    return 10;
  else if (strcmp(reg, "%e11") == 0)
    return 11;
  else if (strcmp(reg, "%e12") == 0)
    return 12;
  else if (strcmp(reg, "%e13") == 0)
    return 13;
  else if (strcmp(reg, "%e14") == 0)
    return 14;
  else if (reg[0] == '$') { // Constants/Immediate values
    return (int)strtol(reg + 1, NULL, 10);
  }
  else if (strcmp(reg, "Stack") == 0) {
    return 0x100;
  }
  else if (strcmp(reg, "array") == 0) {
    return findAddress(symbolicNames, "array");
  }
  else
    return 15; // No register case
}

/*
    Converts the linked list of assembly commands into a linked list of binary
   commands.
    @param: *list, input linked list
*/
outputnode *assemble(inputnode *list, map *names) {  
  unsigned long memoryAddress = 0; // Added as field for outputnode
  outputnode *ret;
  ret = malloc(sizeof(outputnode));
  outputnode *curr = ret;
  curr->data = NULL;
  curr->next = NULL;
  while (list != NULL) {
    // If not directive, then it's a command, and we can just calculate its
    // Case for if not directive, and it'd assumably set to
    // a value after all directives complete at beginning
    // of prorgram (for our case, .pos 0 -> mem starts @ 0)

    char *buff;
    buff = malloc(sizeof(char) * 100);

    command comm = *(list->data);
    command *commPointer = list->data; // For clarity, could combine this into one line with the above.

    printf("Name: %s\n", comm.name);
    if (!strcmp(comm.name, ".long") || !strcmp(comm.name, ".quad")) {
      sprintf(buff, "%ld", comm.value);
    }
    if (!strcmp(comm.name, "halt"))
      sprintf(buff, "%s", "00");
    else if (!strcmp(comm.name, "nop"))
      sprintf(buff, "%s", "10");
    else if (!strcmp(comm.name, "rrmovl"))
      sprintf(buff, "20%x%x", reg_num(comm.rA), reg_num(comm.rB));
    else if (!strcmp(comm.name, "irmovl"))
      sprintf(buff, "30f%x%x", reg_num(comm.rB), reg_num(comm.other));
    else if (!strcmp(comm.name, "rmmovl"))
      sprintf(buff, "40%x%x%s", reg_num(comm.rA), reg_num(comm.rB), comm.other);
    else if (!strcmp(comm.name, "mrmovl"))
      sprintf(buff, "50%x%x%s", reg_num(comm.rA), reg_num(comm.rB), comm.other);
    else if (!strcmp(comm.name, "addl"))
      sprintf(buff, "60%x%x", reg_num(comm.rA), reg_num(comm.rB));
    else if (!strcmp(comm.name, "subl"))
      sprintf(buff, "61%x%x", reg_num(comm.rA), reg_num(comm.rB));
    else if (!strcmp(comm.name, "andl"))
      sprintf(buff, "62%x%x", reg_num(comm.rA), reg_num(comm.rB));
    else if (!strcmp(comm.name, "xorl"))
      sprintf(buff, "63%x%x", reg_num(comm.rA), reg_num(comm.rB));
    else if (!strcmp(comm.name, "jmp")) {
      sprintf(buff, "70%x", findAddress(names, comm.other));
      commPointer->usesSymbolicName = true;
    }
    else if (!strcmp(comm.name, "jle")) {
      sprintf(buff, "71%x", findAddress(names, comm.other));
      commPointer->usesSymbolicName = true;
    }
    else if (!strcmp(comm.name, "jl")) {
      sprintf(buff, "72%x", findAddress(names, comm.other));
      commPointer->usesSymbolicName = true;
    }
    else if (!strcmp(comm.name, "je")) {
      sprintf(buff, "73%x", findAddress(names, comm.other));
      commPointer->usesSymbolicName = true;
    }
    else if (!strcmp(comm.name, "jne")) {
      sprintf(buff, "74%x", findAddress(names, comm.other));
      commPointer->usesSymbolicName = true;
    }
    else if (!strcmp(comm.name, "jge")) {
      sprintf(buff, "75%x", findAddress(names, comm.other));
      commPointer->usesSymbolicName = true;
    }
    else if (!strcmp(comm.name, "jg")) {
      sprintf(buff, "76%x", findAddress(names, comm.other));
      commPointer->usesSymbolicName = true;
    }
    else if (!strcmp(comm.name, "cmovle"))
      sprintf(buff, "21%x%x", reg_num(comm.rA), reg_num(comm.rB));
    else if (!strcmp(comm.name, "cmovl"))
      sprintf(buff, "22%x%x", reg_num(comm.rA), reg_num(comm.rB));
    else if (!strcmp(comm.name, "cmove"))
      sprintf(buff, "23%x%x", reg_num(comm.rA), reg_num(comm.rB));
    else if (!strcmp(comm.name, "cmovne"))
      sprintf(buff, "24%x%x", reg_num(comm.rA), reg_num(comm.rB));
    else if (!strcmp(comm.name, "cmovge"))
      sprintf(buff, "25%x%x", reg_num(comm.rA), reg_num(comm.rB));
    else if (!strcmp(comm.name, "cmovg"))
      sprintf(buff, "26%x%x", reg_num(comm.rA), reg_num(comm.rB));
    else if (!strcmp(comm.name, "call")) {
      sprintf(buff, "80%x", findAddress(names, comm.other));
      commPointer->usesSymbolicName = true;
    }
    else if (!strcmp(comm.name, "ret"))
      sprintf(buff, "%s", "90");
    else if (!strcmp(comm.name, "pushl"))
      sprintf(buff, "a0%xf", reg_num(comm.rA));
    else if (!strcmp(comm.name, "popl"))
      sprintf(buff, "b0%xf", reg_num(comm.rA));
    curr->data = buff;
    printf("Buff %s\n", buff);
    if ((list->data)->directive == true) {
      if ((list->data)->long_or_quad == true) {
        memoryAddress += 8;
        // print long/quad value, need to save it when making input list
      } else if ((list->data)->pos == true) {
        memoryAddress = (list->data)->position;
        // would move on afterward
      } else if ((list->data)->align == true) {
        memoryAddress =
            memoryAddress + (memoryAddress % (list->data)->alignment);
        // would move on afterward
      } else if ((list->data)->symbol == true) {
        (list->data)->symbolicName->address = memoryAddress;
        setAddress(names, (list->data)->symbolicName->name, memoryAddress);
      }
    }
    else {
      memoryAddress += sizeof(buff); // For case of not directive, this is how
                                     // much we add to the memoryaddress.
    }

    // the memory address updates
    curr->memoryAddress = memoryAddress;
    curr->assembly = list;
    
    if (list->next != NULL) {
      outputnode *next;
      next = malloc(sizeof(outputnode));
      curr->next = next;
      curr = curr->next;
      curr->data = NULL;
      curr->next = NULL;
    }
    
    list = list->next;
  }

  // Two-pass assembler to resolve symbolic names
  outputnode *currCopy;
  currCopy = ret;
  while (currCopy != NULL) {
    char *buff;
    buff = malloc(sizeof(char) * 100);
    command comm = *(currCopy->assembly->data);
    if (currCopy->assembly->data->usesSymbolicName) {
      if (!strcmp(comm.name, "jmp")) {
        sprintf(buff, "70%x", findAddress(names, comm.other));
      }
      else if (!strcmp(comm.name, "jle")) {
        sprintf(buff, "71%x", findAddress(names, comm.other));
      }
      else if (!strcmp(comm.name, "jl")) {
        sprintf(buff, "72%x", findAddress(names, comm.other));
      }
      else if (!strcmp(comm.name, "je")) {
        sprintf(buff, "73%x", findAddress(names, comm.other));
      }
      else if (!strcmp(comm.name, "jne")) {
        sprintf(buff, "74%x", findAddress(names, comm.other));
      }
      else if (!strcmp(comm.name, "jge")) {
        sprintf(buff, "75%x", findAddress(names, comm.other));
      }
      else if (!strcmp(comm.name, "jg")) {
        sprintf(buff, "76%x", findAddress(names, comm.other));
      }
      else if (!strcmp(comm.name, "call")) {
        sprintf(buff, "80%x", findAddress(names, comm.other));
        comm.usesSymbolicName = true;
      }
      currCopy->data = buff;
    }
    currCopy = currCopy->next;
  }
  return ret;
}