/* GHEORGHE Adrian-Valentin - 321CB */

#include <stdio.h>
#include <string.h>
#include "structs.h"

/////////////Functii auxiliare/////////////////

// functia de initializare a unei carti
Book *initBook (char *book_name, char *author, int number_pages, int rating) {
    Book *book = (Book*) calloc(1, sizeof (Book));

    if (!book) {
        return NULL;
    }

    book->pages = number_pages;
    book->rating = rating;
    strcpy(book->title, book_name);
    strcpy(book->author, author);

    return book;
}

//functia de printare a detaliilor unei carti
void printBook (FILE *outputfile, Book *book) {
    if (book)
        fprintf(outputfile, "Informatii recomandare: %s, %s, %d, %d\n", book->title, book->author,book->rating, book->pages);
}

// functia de initializare a trie-ului de carti
BTTree *createTreeBooks (char *name, Book *book) {
    BTTree *bookTree = (BTTree *)calloc(1, sizeof (BTTree));
    int position;
    if (!bookTree) {
        return NULL;
    }
     // conditie de oprire a recursivitatii
    if (strlen(name) == 0) {
        return bookTree;
    } else {
        // aflam pozitia pe care trebuie sa inseram cartea si facem inserarea
        position = getPosition (*name);
        bookTree->child[position] = createTreeBooks (name + 1, book);
    }

    return bookTree;
}

// functie auxiliara pentru initializarea trie-ului de autori
ATTree *initTreeAuthorAux (char *author,Book *book) {
    ATTree *authorTree = (ATTree *)calloc(1, sizeof (ATTree));
    int position;
    if (!authorTree) {
        return NULL;
    }
    // conditia de oprire din recursivitate
    if (strlen(author) == 0) {
        return authorTree;
    } else {
         // aflam pozitia pe care trebuie sa inseram cartea si facem inserarea 
        position = getPosition (*author);
        authorTree->child[position] = initTreeAuthorAux (author + 1, book);
    }
    return authorTree;
}

// functia de initializare a trie-ului de autori propriu-zisa
ATTree *createTreeAuthor (Book *book, char *name) {
    ATTree *authors = initTreeAuthorAux (name, book);
    if(!authors)
        return NULL;
    // mergem pana la pointerul spre trie-ul de books
    ATTree *last = searchAuthorAux (authors, name);

    // initializam trie-ul de books
    BTTree *books = createTreeBooks (book->title, book);

    // initializam pointerul spre trie-ul de books cu trie-ul creat mai sus
    last->books = books;
    return authors;
}

// returneaza pozitia din alfabet a unui caracter
int getPosition (char c) {
    const char *letters = (const char *) "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const char *nonletters = (const char*) "0123456789.-'?! ";
    int i;

    for (i = 0; i < LETTERS; i++) {
        if (c == letters[i])
            return i;
    }
    if (i == LETTERS) {
        for (i = 0; i < NON_LETTERS; i++) {
            if (c == nonletters[i])
                return (LETTERS + i);
        }
    }
    
    return 0;
}

// functie de afisare a unui trie de books
void printBookTree (FILE *outputfile, BTTree *tree) { 
    int i = 0;
    if (tree) {
        while (i < MAX_LEN) {
            if (tree->child[i]) {
                if (tree->child[i]->book) 
                    fprintf (outputfile, "%s\n", tree->child[i]->book->title);
                printBookTree (outputfile, tree->child[i]);
            }
            i ++;
        }
    }
    else
        return;
}

// functie de adaugare a unei carti in trie
void addBook (char *name, Book *book, BTTree *tree) {
    // conditie de oprire a recursivitatii
    if (strlen (name) == 0) {
        tree->book = book;
        return;
    }
    else {
        // aflam pozitia pe care trebuie sa inseram cartea
        int position = getPosition(*name);
        // testam daca exista deja un nod pe acea pozitie
        if (!tree->child[position]) {
            // daca nu exista nod pe acea pozitie se creeaza acum
            tree->child[position] = createTreeBooks (name + 1, book);
            if (!tree->child[position])
                return;
        }
        // trecem recursiv mai departe in numele cartii
        addBook (name + 1, book,tree->child[position]);        
    }
    return;
}

// functie de adaugare a unui autor in trie-ul de autori
void addAuthor (char *name, Book *book, ATTree *tree) {
    // conditie de oprire a recursivitatii
    if (strlen (name) == 0) {
        // initializam trie-ul de books daca e nevoie, sau doar adaugam carte
        if (tree->books == NULL)
            tree->books = createTreeBooks (book->title, book);
        else 
            addBook (book->title, book, tree->books);
        return;
    } else {
        // aflam pozitia pe care ar trebui sa se afle nodul
        int position = getPosition (*name);
        // daca nu exista nod pe acea pozitie se creeaza acum
        if (!tree->child[position]) {
            tree->child[position] = createTreeAuthor (book, name + 1);
            if (!tree->child[position])
                return;
        }
        addAuthor (name + 1, book, tree->child[position]);
    }
    return;
}

// functie care adauga o carte unui autor deja existent 
void addBookAuthor (char *authorName, Book *book, ATTree *tree, FILE *outputfile) {
    ATTree *foundAuthorTree = searchAuthorAux (tree, authorName);
    if (foundAuthorTree) {
        addBook (book->title, book, foundAuthorTree->books);
    }
}

// functie care cauta o carte intr-un trie de carti
void searchBook (BTTree *tree, char *name, FILE *outputfile) {
    BTTree *foundBookTree = searchBookAux (tree, name);

    // testam daca a fost gasita cartea
    if (!foundBookTree) {
        fprintf (outputfile, "Cartea %s nu exista in recomandarile tale.\n", name);
    } else {
        // returnam informatiile cartii daca aceasta exista
        Book *foundBook = foundBookTree->book;
        if (foundBook)
            printBook (outputfile, foundBook);
        else 
            fprintf (outputfile, "Cartea %s nu exista in recomandarile tale.\n", name);
    }
    return;
}

// functie auxiliara pentru cautarea unei carti
BTTree *searchBookAux (BTTree *tree, char *name) {
    if (strlen (name) != 0) {
        int position = getPosition (*name);

        if (!(tree->child[position]))
            return NULL;
        return searchBookAux (tree->child[position], name + 1);
    } else {
        return tree;
    }
}

// functie auxiliara pentru cautarea unui autor
ATTree *searchAuthorAux(ATTree *tree, char *name) {
    if (strlen(name) != 0) {
        int position = getPosition (*name);

        if (!(tree->child[position]))
            return NULL;
        return searchAuthorAux (tree->child[position], name + 1);
    } else {
        return tree;
    }
}

// functia de cautare a unui autor
void searchAuthor (ATTree *tree, char *name, FILE *outputfile) {
    ATTree *foundAuthorTree = searchAuthorAux (tree, name);

    // returneaza cartile unui autor, daca acesta exista 
    if (!foundAuthorTree) {
        fprintf (outputfile, "Autorul %s nu face parte din recomandarile tale.\n", name);
    } else {
        BTTree *booksFound = foundAuthorTree->books;
        printBookTree (outputfile,booksFound);
    }
}

// functie care cauta o anumita carte a unui anumit autor 
void searchByAuthor (ATTree *tree, char *title, char *authorName, FILE *outputfile) {
    ATTree *foundAuthorTree = searchAuthorAux (tree, authorName);

    if (!foundAuthorTree) {
        fprintf (outputfile, "Autorul %s nu face parte din recomandarile tale.\n", authorName);
    } else {
        BTTree *foundBookTree = foundAuthorTree->books;
        searchBook (foundBookTree, title, outputfile);
    }
}

