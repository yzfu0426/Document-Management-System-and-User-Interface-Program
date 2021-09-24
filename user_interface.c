/*
Name : Yuzhu Fu
UID : 117519648
Directory ID : yzfu0426
*/

#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <stdlib.h>
#include<ctype.h>
#include "document.h"

#define MAX_CMD_LEN 1024

/* Prototypes */
static int is_comment(char *line);
static void extract_string_in_double_quotes(char *src, char **begin,
                                            char **end, char *result);

/************************************************************************/
/* This program will serve as a text-based user interface to the       */
/* document system implemented in document.c.                         */
/* It enables the user to give commands and performs several         */
/* to the documentation.                                            */
/*******************************************************************/

int main(int argc, char *argv[]) {
   FILE *input;
   Document doc;
   char c, line[MAX_CMD_LEN + 1], command[MAX_CMD_LEN + 1],
      text[MAX_CMD_LEN + 1], other_info[MAX_CMD_LEN + 1], *cursor,
      file_name[MAX_STR_SIZE + 1], target[MAX_STR_SIZE + 1],
      replacement[MAX_STR_SIZE + 1], *begin, *end;
   int para_num, ln_num, check_eof, entries_read, length;
   const char *doc_name = "main_document";

   init_document(&doc, doc_name);

   if (argc == 1) {
      /* If only one one argument then read from stdin */
      input = stdin;
   } else if (argc == 2) {
      /* If there are two arguments then read from the file */
      input = fopen(argv[1], "r");
      if (input == NULL) {
         fprintf(stderr, "%s cannot be opened.", argv[1]);
         return EX_OSERR;
      }
   } else {
      /* Otherwise print usage message to standard error. */
      fprintf(stderr, "Usage: user_interface\n");
      fprintf(stderr, "Usage: user_interface <filename>\n");
      return EX_USAGE;
   }

   if (argc == 1) {
      printf("> ");
   }

   /* read a line */
   fgets(line, MAX_CMD_LEN + 1, input);
   /* check whether it is EOF */
   while (!feof(input)) {
      /* process the line */

      /* read formatted input from the line, and store the command
         (the first string seperated by a blank space) in the character 
         array named command. Record the value sscanf returns in an 
         integer variable, in order to check whether it is a blank line.
         If it is a blank line, check_eof will be EOF and no further
         process will take place. */
      check_eof = sscanf(line, "%s", command);
      /* If the provided line is not an empty line or a comment */
      if (check_eof != EOF && !is_comment(line)) {
         if (!strcmp(command, "add_paragraph_after")) {
            /* If the command is add_paragraph_after */
            entries_read =
               sscanf(line, "%s%d%s", command, &para_num, other_info);
            /* If more information are provided after para_num,
               then it is an invalid command. */
            if (entries_read != 2) {
               fprintf(stdout, "Invalid Command\n");
            } else {
               if (para_num < 0) {
                  fprintf(stdout, "Invalid Command\n");
               } else {
                  if (add_paragraph_after(&doc, para_num) == FAILURE) {
                     fprintf(stdout, "add_paragraph_after failed\n");
                  }
               }
            }
         } else if (!strcmp(command, "add_line_after")) {
            /* If the command is add_paragraph_after */
            entries_read =
               sscanf(line, "%s%d%d %c", command, &para_num, &ln_num, &c);

            if (entries_read != 4) {
               /* If we cannot successfully read a command, 
                  a para_num, a ln_num and a '*' character, 
                  then it is an invalid command. */
               fprintf(stdout, "Invalid Command\n");
            } else {
               if (para_num < 0 || ln_num < 0 || c != '*') {
                  fprintf(stdout, "Invalid Command\n");
               } else {
                  /* extract everything after the '*' symbol and
                     store it in the text array, then add this line 
                     to the document based on para_num and ln_num. */
                  cursor = strstr(line, "*");
                  strcpy(text, cursor + 1);
                  length = strlen(text);
                  /* exclude the '\n' character at the end of the text */
                  text[length - 1] = '\0';

                  if (add_line_after(&doc, para_num, ln_num, text) == FAILURE) {
                     fprintf(stdout, "add_paragraph_after failed\n");
                  }

               }

            }
         } else if (!strcmp(command, "print_document")) {
            /* If the command is add_paragraph_after */
            entries_read = sscanf(line, "%s%s", command, other_info);

            /* If more information are provided after the command,
               then it is an invalid command. */
            if (entries_read != 1) {
               fprintf(stdout, "Invalid Command\n");
            } else {
               print_document(&doc);
            }

         } else if (!strcmp(command, "quit") || !strcmp(command, "exit")) {
            /* If the command is exit or quit */
            entries_read = sscanf(line, "%s%s", command, other_info);

            /* If more information are provided after the quit/exit,
               then it is an invalid command. */
            if (entries_read != 1) {
               fprintf(stdout, "Invalid Command\n");
            } else {
               /* no more data should be read,
                  use break to jump out of the loop. */
               break;
            }

         } else if (!strcmp(command, "append_line")) {
            /* If the command is append_line */
            entries_read = sscanf(line, "%s%d %c", command, &para_num, &c);

            /* If we cannot successfully read a command, 
               a para_num, and a '*' character, 
               then it is an invalid command. */
            if (entries_read != 3) {
               fprintf(stdout, "Invalid Command\n");
            } else {
               if (para_num < 0 || c != '*') {
                  fprintf(stdout, "Invalid Command\n");
               } else {
                  /* extract everything after the '*' symbol and
                     store it in the text array, then add this line 
                     to the document based on para_num and ln_num. */
                  cursor = strstr(line, "*");
                  strcpy(text, cursor + 1);
                  length = strlen(text);
                  /* exclude the '\n' character at the end of the text */
                  text[length - 1] = '\0';

                  if (append_line(&doc, para_num, text) == FAILURE) {
                     fprintf(stdout, "append_line failed\n");
                  }

               }

            }
         } else if (!strcmp(command, "remove_line")) {
            /* If the command is remove_line */
            entries_read =
               sscanf(line, "%s%d%d%s", command, &para_num, &ln_num,
                      other_info);
            /* If more information are provided at the 
               end of the line, then it is an invalid command. */
            if (entries_read != 3) {
               fprintf(stdout, "Invalid Command\n");
            } else {
               if (para_num < 0 || ln_num < 0) {
                  fprintf(stdout, "Invalid Command\n");
               } else {
                  if (remove_line(&doc, para_num, ln_num) == FAILURE) {
                     fprintf(stdout, "Invalid Command\n");
                  }
               }

            }

         } else if (!strcmp(command, "load_file")) {
            /* If the command is load_file */
            entries_read =
               sscanf(line, "%s%s%s", command, file_name, other_info);

            /* If more information are provided after "load_file", 
               then it is an invalid command. */
            if (entries_read != 2) {
               fprintf(stdout, "Invalid Command\n");
            } else {
               if (load_file(&doc, file_name) == FAILURE) {
                  fprintf(stdout, "load_file failed\n");
               }
            }
         } else if (!strcmp(command, "replace_text")) {
            /* If the command is replace_text */
            entries_read =
               sscanf(line, "%s%s%s", command, target, replacement);
            /* If we cannot successfully read target and replacement, 
               then it is an invalid command. */
            if (entries_read != 3) {
               fprintf(stdout, "Invalid Command\n");
            } else {
               /* call extract_string_in_double_quotes to extract target */
               extract_string_in_double_quotes(line, &begin, &end, target);
               /* call extract_string_in_double_quotes to extract replacement */
               extract_string_in_double_quotes(end + 1, &begin, &end,
                                               replacement);

               if (replace_text(&doc, target, replacement) == FAILURE) {
                  fprintf(stdout, "replace_text failed\n");
               }
            }
         } else if (!strcmp(command, "highlight_text")) {
            /* If the command is highlight_text */
            entries_read = sscanf(line, "%s%s", command, target);

            /* If we cannot successfully read target string, 
               then it is an invalid command. */
            if (entries_read != 2) {
               fprintf(stdout, "Invalid Command\n");
            } else {
               /* call extract_string_in_double_quotes to extract target */
               extract_string_in_double_quotes(line, &begin, &end, target);
               highlight_text(&doc, target);
            }
         } else if (!strcmp(command, "remove_text")) {
            /* If the command is remove_text */
            entries_read = sscanf(line, "%s%s", command, target);

            /* If we cannot successfully read target string, 
               then it is an invalid command. */
            if (entries_read != 2) {
               fprintf(stdout, "Invalid Command\n");
            } else {
               /* call extract_string_in_double_quotes to extract target */
               extract_string_in_double_quotes(line, &begin, &end, target);
               remove_text(&doc, target);
            }
         } else if (!strcmp(command, "save_document")) {
            /* If the command is save_document */
            entries_read =
               sscanf(line, "%s%s%s", command, file_name, other_info);

            /* If more information is provided after the command,
               then it is an invalid command. */
            if (entries_read != 2) {
               fprintf(stdout, "Invalid Command\n");
            } else {
               if (save_document(&doc, file_name) == FAILURE) {
                  fprintf(stdout, "save_document failed\n");
               }
            }
         } else if (!strcmp(command, "reset_document")) {
            /* If the command is reset_document */
            entries_read = sscanf(line, "%s%s", command, other_info);

            /* If more information is provided after the command,
               then it is an invalid command. */
            if (entries_read != 1) {
               fprintf(stdout, "Invalid Command\n");
            } else {
               reset_document(&doc);

            }

         } else {
            /* If the line is not a comment/blank line, and it 
               does not match any options listed above, then it
               is an invalid command. */
            fprintf(stdout, "Invalid Command\n");
         }
      }

      if (argc == 1) {
         printf("> ");
      }
      /* keep on reading lines */
      fgets(line, MAX_CMD_LEN + 1, input);

   }

   fclose(input);
   return EXIT_SUCCESS;

}

/*
	This helper function takes a character pointer pointing to a
	string as the parameter, it will check whether the string starts
	with the '#' symble. It will return 1 if the string is a in comment
*/
static int is_comment(char *line) {
   int i = 0, length = strlen(line);

   while (i < length) {
      if (!isspace(line[i])) {
         break;
      }
      i++;
   }
   if (line[i] == '#') {
      return 1;
   } else {
      return 0;
   }

}

/*
	This void helper function takes two character pointer pointing 
   to a string, and two pointers to pointers as the parameter, 
   it will extract the string in the double quotes, store the
   string in the output parameter called result. 
	
*/
static void extract_string_in_double_quotes(char *src, char **begin,
                                            char **end, char *result) {
   int i = 0;
   char *ptr;
   /* find the start quote and modify it through the output parameter begin */
   *begin = strstr(src, "\"");

   /* find the end quote and modify it through the output parameter end */
   *end = strstr(*begin + 1, "\"");

   /* store the string by modifying the output parameter result. */
   for (ptr = *begin + 1; ptr < *end; ptr++) {
      result[i++] = *ptr;
   }
   result[i] = '\0';
}
