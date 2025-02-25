/*
 * @authors Surya Duraivenkatesh, Josh Tittiranonda
 * @date: 03/2025
 * Driver function for y86sim's Assembler.
 * ref: https://spcs.instructure.com/courses/8654/files/1593217?module_item_id=212606
 */

#include <stdio.h>

void initial();

int main()
{
    void initial();

    // File-read for .txt or .o
/*   Commented so errors dont keep coming up, just wrote it so we know how to write other parts
     FILE *file = fopen("[our object thing]", "rb");

    if (file == NULL) {
        printf("File open failed. Please try again");
        return 1;
    }

//looked this file open stuff up btw so fix if needed

    // Getting size of file
    fseek(file, o, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // Memory allocation to hold the file
    unsigned char *buffer = (unsigned char*)malloc(file_size);
    if (buffer == NULL) {
        printf("Can't allocate mmroy. Please try again")
        fclose(file);
        return 1;
    }

    size_t read_size = fread(buffer, 1, file_size, file);
    if (read_size != file_size) {
        printf("File read failed. Please try again and fix the file")
        free(buffer);
        fclose(file);
        return 1;
    }

    // If not returned yet, the file is permissible, so we can read the file.

    // Printing first j bytes in hex (strings can be stored as hex, we need to do a conversion technique, but point is we can read as much as needed into a variable till we hit a space and stuff)
    for (long i = 0; i < 64 && i < file_size; i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n");

    // Example: addq %a, %b
        string -> hex:   addqbyte (space) %a, %b
        Basics: We want to then call our addq. addq must have '60' as beginning (subq '61', etc.)
            For addq which gets passed everything after (space from main), we need to just concatenate the two given registers.
            So, separate the comma, concatenate 6 + 0 + rA byte trans + rB byte trans into one long Hex, then return that.

            We also need to state that the destination was modified, but we don't personally need to do that till y86sim's Procesor part.
            For now, we just need to do these fn translations, we can take care of align/etc after
                Don't need to worry about letting main know re: dest. modified, simply need the final byte translations for now
                by the comma splitting.

        More elaboration: We could, for example, make it read bytes till it hits a space (between addq and %a).
        Then, we can simply store the byte translation of each word to use for our if conditions
        to map to those respective functions, passing the remainder. (note, same space/comma concept)

    // Note: Hence, when writing fns, can assume that you have what type of fn (addq, subq, etc)
        For things like jmp/jle/etc, it might be easier to just do it one by one than
        trying to make the code super concise. Can do that later. This is because each one
        has a different byte translation for the word (i.e. jmp, jle, etc. to byte)

What is needed from each function?
    // Each function will simply return the type of fn concatenated with the destination


*/

}

void initial() {
    printf("%s\n", "Welcome to a Y86-64 Assembler Simulator! For an inputted instruction, the resulting action on the machine will be shown (e.g. register changes, memory changes, condition code changes, etc.)");
}

// You will assume to receive in remainder: a hex rep of "%rA, %rB"

/* 
ASCII character #32. Char SP - Space
Integer ASCII code:	32
Binary code:	0010 0000
Octal code:	40
Hexadecimal code:	20
Group:	punctuation
Reference: https://www.asciihex.com/character/punctuation/32/0x20/sp-space

If it helps.

We can make sure it compiles later.

For now, no need to find out which of the registers it is, just need
to separate the first and second arg. We can map after with a quick helper
*/
long addq(long remainder) {
    // I would find a way to, from the hexadecimal representation of
    // part after (space) in addq (space) %rA, %rB
    // concatenate 6 + 0 + %rA + %rB
    // through string slicing

    // Then return that final thing. This slicing/number differs whether its subq, or mov, etc.

    // Once we do this for each fn, we can do the rest.
    // ****IMPORTANT****
        // Explicitly make two variables to define %rA and %rB for later, when we need to do the simulator interface
        // showing what was modified and when.

}

// 12 things we needa do
// I'll do OPq (add/sub/etc.), jxx, call, ret, push, pop (last 6 on slide 4)
// (first 6 on slide 4, see ref link top of code file) You can do halt, nop, cmov, irmovq, rmmov, mrmov (halt and nop are indeed just one line each tho, but mov seems a bit longer so it should even it out for us)