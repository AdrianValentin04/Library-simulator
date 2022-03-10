/* GHEORGHE Adrian-Valentin - 321CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

int main(int argc, char **argv) {
    FILE *inputfile = fopen(argv[1], "r");
	FILE *outputfile = fopen(argv[2], "w");
    Book *book;
    BTTree *bookTree = NULL;
    ATTree *authorTree = NULL;

    char *token, line[1000];

    while(fgets(line, 1000, inputfile) != NULL) {
        token = strtok(line, " \n");

        if(token == NULL)
            break;
        // comanda add_book
        else if(!strcmp(token, "add_book")) {
            token = strtok(NULL, ":");
            char *book_name;
            book_name = strdup(token);

            token = strtok(NULL, ":");
            char *author_name;
            author_name = strdup(token);

            token = strtok(NULL, ":");
            int rating = atoi(token);

            token = strtok(NULL, "\n");
            int number_pages = atoi(token);

            // initializam un book
            book = initBook(book_name, author_name, number_pages, rating);
            if(!book) {
                free(book->title);
                free(book->author);
                free(book);
            }
            
            // initializam cei doi trie
            if(!bookTree) {
                bookTree = createTreeBooks(book->title, book);
                authorTree = createTreeAuthor(book, book->author);
            }

            // adaugam cartea si autorul in trie-ul corespunzator 
            addBook(book->title, book, bookTree);
            addAuthor(author_name, book, authorTree);
            addBookAuthor(author_name, book, authorTree, outputfile);

            free(author_name);
            free(book_name);

        }
        // comanda search_book 
        else if(!strcmp(token, "search_book")) {
            token = strtok(NULL, "\n");
            char *title = strdup(token);

            searchBook(bookTree, title, outputfile);

            free(title);

        }
        // comanda list_author
        else if(!strcmp(token, "list_author")) {
            token = strtok(NULL, "\n");
            char *author = strdup(token);

            searchAuthor(authorTree, author, outputfile);

            free(author);

        }
        // comanda search_by_author
        else if(!strcmp(token, "search_by_author")) {
            token = strtok(NULL, ":");
            char *author = strdup(token);

            token = strtok(NULL, "\n");
            char *title = strdup(token);

            searchByAuthor(authorTree, title, author, outputfile);

            free(author);
            free(title);

        }
        // comanda delete_book
        else if(!strcmp(token, "delete_book")) {
            token = strtok(NULL, "\n");
            char *title = strdup(token);

            free(title);
        }
    }

    fclose(inputfile);
    fclose(outputfile);

    return 0;
}