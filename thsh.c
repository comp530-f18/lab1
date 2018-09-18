/* COMP 530: Tar Heel SHell */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Assume no input line will be longer than 1024 bytes
#define MAX_INPUT 1024

int main(int argc, char **argv, char **envp) {
  // flag that the program should end
  int finished = 0;
  char *prompt = "thsh> ";
  // buffer to hold current command
  char cmd[MAX_INPUT];
  
  while (!finished) {
    // pointer to next place in cmd to store a character
    char *cursor;
    // the last character that was written into cmd
    char last_char;
    // stores return value of read/write syscalls
    int rv;
    // number of characters written into cmd
    int count;

    // Print the prompt
    // file descriptor 1 -> writing to stdout
    // print the whole prompt string (write number of
    // bytes/chars equal to the length of prompt)
    rv = write(1, prompt, strlen(prompt));
    if (!rv) {
      // if we printed 0 bytes, this call failed and the program
      // should end -- this will likely never occur.
      finished = 1;
      break;
    }

    // read and parse the input
    /*
     * We want to continue reading characters until:
     *   - read() fails (rv will become 0) OR
     *   - count == MAX_INPUT-1 (we have no buffer space left) OR
     *   - last_char was '\n'
     * so we continue the loop while:
     *   rv is nonzero AND count < MAX_INPUT - 1 AND last_char != '\n'
     *
     * On every iteration, we:
     *   - increment cursor to advance to the next char in the cmd buffer
     *   - increment count to indicate we've stored another char in cmd
     *     (this is done during the loop termination check with "++count")
     *
     * To make the termination check work as intended, the loop starts by:
     *   - setting rv = 1 (so it's initially nonzero)
     *   - setting count = 0 (we've read no characters yet)
     *   - setting cursor = cmd (cursor at start of cmd buffer)
     *   - setting last_char = 1 (so it's initially not '\n')
     *
     * In summary:
     *   - START:
     *      set rv = 1, count = 0, cursor = cmd, last_char = 1
     *   - CONTINUE WHILE:
     *      rv (is nonzero) && count < MAX_INPUT - 1 && last_char != '\n'
     *   - UPDATE PER ITERATION:
     *      increment count and cursor
     */
    for (rv = 1, count = 0, cursor = cmd, last_char = 1;
	 rv && (++count < (MAX_INPUT - 1)) && (last_char != '\n'); cursor++) {

      // read one character
      // file descriptor 0 -> reading from stdin
      // writing this one character to cursor (current place in cmd buffer)
      rv = read(0, cursor, 1);
      last_char = *cursor;
    }
    // null terminate cmd buffer (so that it will print correctly)
    *cursor = '\0';
    
    if (!rv) {
      // if the last read call returned 0, the program should end.
      // launcher.sh forces non-canonical input mode, so this also will
      // probably never occur.
      finished = 1;
      break;
    }

    // Just echo the command line for now
    // file descriptor 1 -> writing to stdout
    // print the whole cmd string (write number of
    // chars/bytes equal to the length of cmd, or MAX_INPUT,
    // whichever is less)
    write(1, cmd, strnlen(cmd, MAX_INPUT));
  }

  return 0;
}
