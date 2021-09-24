/*
Name : Yuzhu Fu
UID : 117519648
Directory ID : yzfu0426
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "document.h"

/* Prototypes */
static void replace_in_line(char *line_text, const char *target,
                            const char *replacement);
static int is_blank_line(char line[]);

/* 
   This function takes a pointer to a document and a pointer which pointes 
   to the first character of a constant character array as parameters. 
   It will initialize the document to empty by setting the
   number_of_paragraphs in the document to 0 and name the document as the 
   second parameter's value. It will return FAILURE if any of the parameter
   points to NULL, or strlen(name) is greater than MAX_STR_SIZE.
   Otherwise, it will return SUCCESS.
*/
int init_document(Document * doc, const char *name) {
   if (doc == NULL || name == NULL) {
      return FAILURE;
   } else if ((int) strlen(name) > MAX_STR_SIZE) {
      return FAILURE;
   } else {
      int i = 0;

      doc->number_of_paragraphs = 0;
      strcpy(doc->name, "");
      strcat(doc->name, name);
      for (i = 0; i < MAX_PARAGRAPHS; i++) {
         doc->paragraphs[i].number_of_lines = 0;

      }

      return SUCCESS;
   }

}

/* 
   This function takes a pointer to a document as the parameter. 
   It will reset the document to empty by setting the
   number_of_paragraphs in the document to 0. 
   It will return FAILURE if the parameter points to NULL.
   Otherwise, it will return SUCCESS.
*/
int reset_document(Document * doc) {
   if (doc == NULL) {
      return FAILURE;
   } else {
      doc->number_of_paragraphs = 0;
      return SUCCESS;
   }

}

/* 
   This function takes a pointer to a document as the parameter. 
   It will print the document’s name, number of paragraphs. 
   Then it prints each non-empty paragraphs by seperating them by a blank line.
   It will return FAILURE if the parameter points to NULL.
   Otherwise, it will return SUCCESS.
*/
int print_document(Document * doc) {
   if (doc == NULL) {
      return FAILURE;
   } else {
      int para_idx, line_idx, total_nums;
      /* define a pointer to the current paragraph that will be printed */
      Paragraph *curr_para_ptr;

      printf("Document name: \"%s\"\n", doc->name);
      printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

      for (para_idx = 0; para_idx < doc->number_of_paragraphs; para_idx++) {
         /* curr_para_ptr points to the curr paragraph that will be printed */
         curr_para_ptr = &doc->paragraphs[para_idx];
         /* set total_nums to total number of lines in the current paragraph */
         total_nums = curr_para_ptr->number_of_lines;

         for (line_idx = 0; line_idx < total_nums; line_idx++) {
            printf("%s\n", curr_para_ptr->lines[line_idx]);
         }

         /* If the curr paragraph is neither the last paragraph of the document
            nor empty, print a blank line after the current paragraph.  */
         if (para_idx < doc->number_of_paragraphs - 1 && total_nums != 0) {
            printf("\n");
         }
      }
      return SUCCESS;
   }

}

/* 
   This function takes a pointer to a document and an integer representing 
   the specified index after which to add a new paragraph as parameters.
   It will return FAILURE if doc points to NULL, or the document is full,
   or the paragraph number is larger than the number of paragraphs available.
   Otherwise, it adds a new paragraph at the desired index and returns SUCCESS.
*/
int add_paragraph_after(Document * doc, int paragraph_number) {
   if (doc == NULL) {
      return FAILURE;
   } else if (doc->number_of_paragraphs == MAX_PARAGRAPHS) {
      return FAILURE;
   } else if (paragraph_number > doc->number_of_paragraphs) {
      return FAILURE;
   } else {
      int i, total = doc->number_of_paragraphs;

      /* Shift all paragraphs whose indices greater than the 
         specified index down by one index. */
      for (i = 0; i < total - paragraph_number; i++) {
         doc->paragraphs[total - i] = doc->paragraphs[total - 1 - i];
      }
      /* Set the number of lines of that new paragraph to zero */
      doc->paragraphs[paragraph_number].number_of_lines = 0;
      /* increment total number of paragraphs of this document by one. */
      doc->number_of_paragraphs++;

      return SUCCESS;
   }

}

/* 
   This function takes a pointer to a document, an integer representing the 
   specified paragraph number, an integer representing the specified line 
   number, and a pointer to the first character of a constant character array 
   as parameters. It will return FAILURE if doc or new_line points to NULL, 
   or paragraph number exceeds the number of paragraphs available,
   or the paragraph already has the maximum number of lines allowed,
   or the line number is larger than the available number of lines.
   Otherwise, it adds a new line after the line with specified index,
   then it returns SUCCESS.
*/
int add_line_after(Document * doc, int paragraph_number, int line_number,
                   const char *new_line) {
   if (doc == NULL || new_line == NULL) {
      return FAILURE;
   } else if (paragraph_number > doc->number_of_paragraphs) {
      return FAILURE;
   } else if (doc->paragraphs[paragraph_number - 1].number_of_lines
              == MAX_PARAGRAPH_LINES ||
              line_number >
              doc->paragraphs[paragraph_number - 1].number_of_lines) {
      /* check whether the paragraph already has max number of lines allowed,
         or the line number is larger than the available number of lines */
      return FAILURE;
   } else {
      /* a pointer to the specific paragraph that a new line will be added to */
      Paragraph *para_ptr = &doc->paragraphs[paragraph_number - 1];
      int i, total = para_ptr->number_of_lines;

      /* Shift all lines whose indices greater than the 
         specified index down by one index. */
      for (i = 0; i < total - line_number; i++) {
         strcpy(para_ptr->lines[total - i], para_ptr->lines[total - 1 - i]);
      }

      /* Add the new line to the specified location */
      strcpy(para_ptr->lines[line_number], new_line);
      /* increment total number of lines of this paragraph by one. */
      para_ptr->number_of_lines++;
      return SUCCESS;
   }

}

/* 
   This function takes a pointer to a document, an integer representing the 
   specified paragraph number, a pointer to an integer as parameters. 
   It will return FAILURE if doc or number_of_lines points to NULL, 
   or if the paragraph number is larger than the number of paragraphs available.
   Otherwise, it returns the number of lines in a paragraph by changing 
   the value of which last parameter points to, then it returns SUCCESS.
*/
int get_number_lines_paragraph(Document * doc, int paragraph_number,
                               int *number_of_lines) {
   if (doc == NULL || number_of_lines == NULL) {
      return FAILURE;
   } else if (paragraph_number > doc->number_of_paragraphs) {
      return FAILURE;
   } else {
      *number_of_lines =
         doc->paragraphs[paragraph_number - 1].number_of_lines;
      return SUCCESS;
   }

}

/* 
   This function takes a pointer to a document, an integer representing the 
   specified paragraph number, and a pointer to the first character of a 
   constant character array as parameters. 
   It will return FAILURE if doc or new_line points to NULL, 
   or paragraph number exceeds the number of paragraphs available,
   or the paragraph already has the maximum number of lines allowed,
   or the line number is larger than the available number of lines.
   Otherwise, it appends a new line after the line with specified index,
   then it returns SUCCESS.
*/
int append_line(Document * doc, int paragraph_number, const char *new_line) {
   if (doc == NULL || new_line == NULL) {
      return FAILURE;
   } else if (paragraph_number > doc->number_of_paragraphs) {
      return FAILURE;
   } else if (doc->paragraphs[paragraph_number - 1].number_of_lines ==
              MAX_PARAGRAPH_LINES) {
      return FAILURE;
   } else {
      int last_line_idx =
         doc->paragraphs[paragraph_number - 1].number_of_lines;
      /* call the function add_line_after to append the line after 
         the last line in the paragraph. */
      add_line_after(doc, paragraph_number, last_line_idx, new_line);

      return SUCCESS;
   }

}

/* 
   This function takes a pointer to a document, two integers representing the 
   specified paragraph number and line number in that paragraph as parameters. 
   It will return FAILURE if doc  points to NULL, 
   or if the paragraph number is larger than the number of paragraphs available,
   or line number is larger than the number of lines in the paragraph .
   Otherwise, it removes the specified line from the paragraph,
   then it returns SUCCESS.
*/
int remove_line(Document * doc, int paragraph_number, int line_number) {
   if (doc == NULL) {
      return FAILURE;
   } else if (paragraph_number > doc->number_of_paragraphs) {
      return FAILURE;
   } else if (line_number >
              doc->paragraphs[paragraph_number - 1].number_of_lines) {
      return FAILURE;
   } else {
      /* a pointer to the specific paragraph that a new line will be added to */
      Paragraph *para_ptr = &doc->paragraphs[paragraph_number - 1];
      int i, total_lines_num = para_ptr->number_of_lines;

      /* Shift all lines whose indices greater than the 
         specified index up by one index. */
      for (i = line_number; i < total_lines_num; i++) {
         strcpy(para_ptr->lines[i - 1], para_ptr->lines[i]);
      }
      /* decrement total number of lines of this paragraph by one. */
      doc->paragraphs[paragraph_number - 1].number_of_lines--;

      return SUCCESS;
   }

}

/* 
   This function takes a pointer to a document, a two-dimensional character 
   array, and an integer representing how many lines to load as parameters. 
   It will return FAILURE if doc or data points to NULL, or data_lines is 0.
   or line number is larger than the number of lines in the paragraph .
   Otherwise, it adds the first data lines number of lines from 
   the data array to the document, creates a new paragraph when meets an 
   empty string, then it returns SUCCESS.
*/
int load_document(Document * doc, char data[][MAX_STR_SIZE + 1],
                  int data_lines) {
   if (doc == NULL || data == NULL || data_lines == 0) {
      return FAILURE;
   } else {
      int para_cnt = 0, empty_string_cnt = 0, row, line_num = 0;

      /* Count the number of empty string in the data array 
         to determine how many new paragraphs will be loaded. */
      for (row = 0; row < data_lines; row++) {
         if (strcmp(data[row], "") == 0) {
            empty_string_cnt++;
         }
      }

      row = 0;

      /* By default creates the first paragraph */
      if (add_paragraph_after(doc, 0) == SUCCESS) {
         while (row < data_lines) {
            if (strcmp(data[row], "") != 0) {
               /* if the current line in data array is not an empty string */
               add_line_after(doc, para_cnt + 1, line_num, data[row]);
               row++;           /* move on to the next line */
               line_num++;
            } else {
               line_num = 0;    /*reset line_num to start a new paragraph */
               row++;           /* move on to the next line in data array */
               para_cnt++;      /* increment paragraph number */
               if (add_paragraph_after(doc, para_cnt) == FAILURE) {
                  /* try to add a new paragraph, if not possible, 
                     return FAILURE and quit function */
                  return FAILURE;
               }
            }
         }
      }

      return SUCCESS;
   }

}

/* 
   This function takes a pointer to a document, two pointers to the first
   character of the two constant character arrays as parameters. 
   It will return FAILURE if doc or target or replacement points to NULL.
   Otherwise, it will replace the target with the replacement,
   then it returns SUCCESS.
*/
int replace_text(Document * doc, const char *target, const char *replacement) {
   if (doc == NULL || target == NULL || replacement == NULL) {
      return FAILURE;
   } else {
      int num_of_para = doc->number_of_paragraphs;
      int num_of_lines, i, j;
      Paragraph *curr_para;

      for (i = 0; i < num_of_para; i++) {
         /* find the number of lines in the current paragraph . */
         if (get_number_lines_paragraph(doc, i + 1, &num_of_lines) == SUCCESS) {
            curr_para = &doc->paragraphs[i];
            /* replace line by line */
            for (j = 0; j < num_of_lines; j++) {
               replace_in_line(curr_para->lines[j], target, replacement);
            }
         }
      }
      return SUCCESS;
   }
}

/* 
   This function takes a pointer to a document, a pointer to the first
   character of the a constant character array as parameters. 
   It will return FAILURE if doc or target  points to NULL.
   Otherwise, it will highlight the target in the document,
   then it returns SUCCESS.
*/
int highlight_text(Document * doc, const char *target) {
   if (doc == NULL || target == NULL) {
      return FAILURE;
   } else {
      int length = strlen(target);
      /* This character will cover the highlight symbol around the target */
      char replacement[MAX_STR_SIZE];

      replacement[0] = *HIGHLIGHT_START_STR;
      strcpy(&replacement[1], target);
      replacement[length + 1] = *HIGHLIGHT_END_STR;
      replacement[length + 2] = '\0';

      /*call replace_text function to replace target with the hilighted string
         in order to highlight it. */
      replace_text(doc, target, replacement);
      return SUCCESS;
   }
}

/* 
   This function takes a pointer to a document, a pointer to the first
   character of the a constant character array as parameters. 
   It will return FAILURE if doc or target  points to NULL.
   Otherwise, it will remove the target in the document,
   then it returns SUCCESS.
*/
int remove_text(Document * doc, const char *target) {
   if (doc == NULL || target == NULL) {
      return FAILURE;
   } else {
      /* call the replace_text function to replace the target
         with an empty string in order to remove it. */
      replace_text(doc, target, "");
      return SUCCESS;
   }
}

/* 
   This function takes a pointer to a document, a character pointer points to
   the first character of a string which represents the filename as parameters. 
   It returns FAILURE if doc or filename is NULL, or the given file can not be 
   opened. Otherwise, it adds a paragraph to the document, creates a new 
   paragraph when meets a blank line in the file, then it returns SUCCESS.
*/
int load_file(Document * doc, const char *filename) {
   if (doc == NULL || filename == NULL) {
      return FAILURE;
   } else {
      FILE *input_stream;
      if ((input_stream = fopen(filename, "r")) == NULL) {
         return FAILURE;
      } else {
         /* This character array will store a line in the file temporarily. */
         char line[MAX_STR_SIZE + 1];
         int blank_ln_cnt = 0;

         if (add_paragraph_after(doc, 0) == SUCCESS) {
            /* keeps reading characters from the file stream and 
               store it in the line array before EOF is reached */
            while (fgets(line, MAX_STR_SIZE + 1, input_stream) != NULL) {
               /* If the recently stored line is not blank, 
                  append this line to the certain paragraph
                  according to how many blank lines have occured. */
               if (!is_blank_line(line)) {
                  /* if the last character fgets sotres in line is a 
                     new line character,we need to manually change that 
                     to a null character,otherwise, there will be an extra 
                     line printed when calling print_document function. */
                  if (line[(int) strlen(line) - 1] == '\n') {
                     line[(int) strlen(line) - 1] = '\0';
                  }
                  append_line(doc, blank_ln_cnt + 1, line);
               } else {
                  /* if a blank line is read, 
                     start a new paragraph. */
                  blank_ln_cnt++;
                  add_paragraph_after(doc, blank_ln_cnt);
               }
            }
         }
         fclose(input_stream);
         return SUCCESS;
      }
   }
}

/* 
   This function takes a pointer to a document, a character pointer 
   points to the first character of a string which represents the 
   filename as parameters. It returns FAILURE if doc or filename is NULL, 
   or the given file can not be opened. Otherwise, it prints the paraghraphs 
   associated with the doc parameter to the specified file(overwritting
   the file), then it returns SUCCESS.
*/
int save_document(Document * doc, const char *filename) {
   if (doc == NULL || filename == NULL) {
      return FAILURE;
   } else {
      FILE *output_stream;
      if ((output_stream = fopen(filename, "w")) == NULL) {
         return FAILURE;
      } else {
         int para_idx, line_idx, total_nums;
         Paragraph *curr_para_ptr;

         for (para_idx = 0; para_idx < doc->number_of_paragraphs; para_idx++) {
            /* This pointer variable stores the address of the curr paragraph */
            curr_para_ptr = &doc->paragraphs[para_idx];
            total_nums = curr_para_ptr->number_of_lines;

            /* It loops line by line to write that line to output stream. */
            for (line_idx = 0; line_idx < total_nums; line_idx++) {
               fprintf(output_stream, "%s\n", curr_para_ptr->lines[line_idx]);
            }
            /* If the current paragraph is not the last one, and it
               is not an empty paragraph, write a blank line to output stream.
             */
            if (para_idx < doc->number_of_paragraphs - 1 && total_nums != 0) {
               fprintf(output_stream, "\n");
            }
         }
         fclose(output_stream);
         return SUCCESS;

      }

   }
}

/* 
   This void helper function takes three pointers to the first
   character of the three constant character arrays as parameters. 
   It will replace the target with the replacement inside the line.
*/
static void replace_in_line(char *line_text, const char *target,
                            const char *replacement) {
   char output[MAX_STR_SIZE + 1];       /*temporary array storing results */
   char *out_ptr = output, *cursor = line_text, *tar_begin;
   int i, length = strlen(line_text);

   /* while cursor doesn't pass the edge of line_text */
   do {
      /*find where the target is located in the line starting from cursor */
      tar_begin = strstr(cursor, target);

      if (tar_begin != NULL) {
         while (cursor < tar_begin) {
            /* Let cursor copy every character from previous 
               location up until the target's beginning location. */
            *out_ptr++ = *cursor++;
         }
         /* copy every character from of the replacement to output array */
         for (i = 0; i < (int) strlen(replacement); i++) {
            *out_ptr++ = replacement[i];
         }
         /* Let cursor skip the target in the line */
         cursor += strlen(target);
      } else {
         /* If there no more target left in the line_text,
            just copy every character left in the line_text to output */
         while (cursor < line_text + length) {
            *out_ptr++ = *cursor++;
         }
      }
   } while (cursor < line_text + length);

   /* put a null character to show the end of the line */
   *out_ptr = '\0';

   /* modify the original line_text by copying every character of the output
      to the original line_text. */
   cursor = line_text;
   for (out_ptr = output; out_ptr < output + strlen(output); out_ptr++) {
      *cursor++ = *out_ptr;
   }
   /* put a null character to show the end of the line */
   *cursor = '\0';

}

/*
   This helper function will take a character pointer representing a string as
   parameter, it will check wheter all characters inside the string are spaces.
   It will return 1 if the given parameter contains only blank characters, 
   and it will return 0 otherwise.
*/
static int is_blank_line(char line[]) {
   int length = (int) (strlen(line)), i;

   for (i = 0; i < length; i++) {
      if (!isspace(line[i])) {
         return 0;
      }
   }
   return 1;

}
