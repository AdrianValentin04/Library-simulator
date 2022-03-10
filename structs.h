/* GHEORGHE Adrian-Valentin - 321CB */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BOOK_LEN 51
#define AUTHOR_LEN 41
#define MAX_LEN 68
#define LETTERS 52
#define NON_LETTERS 16

// Structura unei carti
typedef struct book {
    char title[BOOK_LEN];
    char author[AUTHOR_LEN];
    int pages;
    int rating;
} Book;

// Structura unui arbore
typedef struct btree BTTree;
struct btree {
    Book *book;
    BTTree *child[MAX_LEN];
};

typedef struct atree ATTree;
struct atree {
    BTTree *books;
    ATTree *child[MAX_LEN];
};

Book *initBook(char *book_name, char *author, int number_pages, int rating);

void printBook(FILE *outputfile, Book *book);

int getPosition(char c);

BTTree *createTreeBooks(char *name, Book *book);

void printTree(FILE *outputfile, BTTree *tree);

void addBook(char *name, Book *book, BTTree *tree);

void searchBook(BTTree *tree, char *name, FILE *outputfile);

BTTree *searchBookAux(BTTree *tree, char *name);

ATTree *createTreeAuthor(Book *book, char *name);

ATTree *initTreeAuthorAux(char *author,Book *book);

ATTree *searchAuthorAux(ATTree *tree, char *name);

void addAuthor(char *name, Book *book, ATTree *tree);

void addBookAuthor(char *authorName, Book *book, ATTree *tree, FILE *outputfile);

void searchAuthor(ATTree *tree, char *name, FILE *outputfile);

void searchByAuthor(ATTree *tree, char *title, char *authorName, FILE *outputfile);
