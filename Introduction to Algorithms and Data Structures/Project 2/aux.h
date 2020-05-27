/*******************************/
/* Andre Oliveira -- ist193686 */
/*******************************/

#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAMEMAX 1024
#define EMAILMAX 512
#define NUMBERMAX 64
#define DADOSMAX 1603
#define SIZE 653

typedef struct contact {
    struct contact *prev, *next;
    char *name, *email, *number;
} contact;

typedef struct {
    struct contact *head, *last;
} list;

typedef struct node_contact {
    struct node_contact *prev, *next;
    struct contact *c;
} node_contact;

typedef struct domain {
    struct domain *prev, *next;
    char *domain;
    long int i;
} node_domain;

node_contact *hash_contacts[SIZE];
node_domain *hash_domain[SIZE];

char *get_domain(char *email);
void destroy_contact(contact *c);
int hash(char *text);
node_domain *create_node_domain(char *domain);
void destroy_node_domain(node_domain *d);
void destroy_hash_domain();
void insert_in_hash_domain(char *domain);
void remove_from_hash_domain(char *domain);
void destroy_node_contact(node_contact *b);
void remove_from_list(list *l,char *name);
void remove_from_hash_contacts(char *name);
node_contact *create_node_contact(contact *c);
void insert_in_hash(node_contact *b);
void destroy_hash_contacts();
list *create_list();
contact *create_contact();
contact *find_contact(char *name);
void insert_contact(list *l, contact *c);
void command_l(list *l);
void command_p(char *name);
void command_r(list *l, char *name);
void command_e(char *name, char *email);
void command_c(char *domain);

#endif
