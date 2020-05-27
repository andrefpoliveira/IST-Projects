/*******************************/
/* Andre Oliveira -- ist193686 */
/*******************************/

#include "aux.h"

/* get_domain recebe o email e devolve o dominio */
char *get_domain(char *email) {
    return strchr(email,'@') + 1;
}

/* destroy_contact destroi um contacto */
void destroy_contact(contact *c) {
    free(c->name);
    free(c->number);
    free(c->email);
    free(c);
}

/* hash recebe uma string e devolve o valor correspondente da hash */
int hash(char *text) {
    int i,value=0;
    for (i=0; text[i]; i++) {
        value+=text[i];
    }
    return value%SIZE;
}

/* create_node_domain recebe o dominio e devolve um bucket da hash dos dominios*/
node_domain *create_node_domain(char *domain) {
    node_domain *d = malloc(sizeof(node_domain));
    d->domain = malloc(sizeof(char)*(strlen(domain)+1));
    strcpy(d->domain,domain);
    d->i = 1;

    d->prev = NULL;
    d->next = NULL;
    return d;
}

/* destroy_node_domain destroi um bucket da hash de dominios*/
void destroy_node_domain(node_domain *d) {
    free(d->domain);
    free(d);
}

/* destroy_hash_domain destroi a hash dos dominios */
void destroy_hash_domain() {
    node_domain *current, *next;
    int i;

    for (i=0; i<SIZE; i++) {
        current = hash_domain[i];
        while (current) {
            next = current->next;

            destroy_node_domain(current);
            current = next;

        }
    }
}

/* insert_in_hash_domain insere um novo elemento na hash dos dominios
   ou aumenta o contador de um ja existente */
void insert_in_hash_domain(char *domain) {
    int i = hash(domain);
    node_domain *current = hash_domain[i];
    node_domain *prev, *d;

    if (current == NULL) {
        hash_domain[i] = create_node_domain(domain);
    } else {
        while (current) {
            if(!strcmp(current->domain,domain)) {
                current->i += 1;
                return;
            }
            prev = current;
            current = current->next;
        }
        d = create_node_domain(domain);
        prev->next = d;
        d->prev = prev;
    }
}

/* remove_from_hash_domain retira 1 ao contador de um dos dominios 
   se possivel*/
void remove_from_hash_domain(char *domain) {
    int i = hash(domain);
    node_domain *n = hash_domain[i];

    while (n) {

        if (!strcmp(n->domain,domain)) {

            if (n->i != 0) {
                n->i -= 1;
            }
        }
        n = n->next;
    }
}

/* destroy_node_contact destroi um bucket da hash dos contactos */
void destroy_node_contact(node_contact *b) {
    destroy_contact(b->c);
    free(b);
}

/* remove_from_list remove um contacto da lista de contactos */
void remove_from_list(list *l,char *name) {
    contact *current = l->head;
    int i=0;

    while (current) {
        if (!strcmp(current->name,name) && i==0) {
            l->head = l->head->next;
            return;

        } else if (!strcmp(current->name,name)) {
            if (current->next!=NULL) {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            } else {
                current->prev->next = NULL;
                l->last = current->prev;
            }
            return;
        }
        current = current->next;
        i++;
    }
}

/* remove_from_hash_contacts remove um bucket da hash de contactos */
void remove_from_hash_contacts(char *name) {
    int i=0, j = hash(name);
    node_contact *b = hash_contacts[j];

    while (b) {
        if (!strcmp(b->c->name,name) && i==0) {
            hash_contacts[j] = hash_contacts[j]->next;
            destroy_node_contact(b);
            return;

        } else if (!strcmp(b->c->name,name)) {
            if (b->next!=NULL) {
                b->prev->next = b->next;
                b->next->prev = b->prev;
            } else {
                b->prev->next = NULL;
            }
            destroy_node_contact(b);
            return;
        }
        b = b->next;
        i++;
    }
}

/* create_node_contact cria um bucket da hash dos contactos */
node_contact *create_node_contact(contact *c) {
    node_contact *b = malloc(sizeof(node_contact));
    b->c = c;

    b->prev = NULL, b->next = NULL;
    b->c = c;
    return b;
}

/* insert_in_hash insere um bucket na hash dos dominios */
void insert_in_hash(node_contact *b) {
    node_contact *current;
    int i = hash(b->c->name);

    if (hash_contacts[i]) {
        current=hash_contacts[i];
        while(current->next) {
            current = current->next;
        }
        current->next = b;
        b->prev = current;
    } else {
        hash_contacts[i] = b;
    }
}

/* destroy_hash_contacts destroi a hash dos contactos */
void destroy_hash_contacts() {
    int i;
    node_contact *current;

    for (i=0; i<SIZE; i++) {
        current = hash_contacts[i];
        while(current) {
            remove_from_hash_contacts(current->c->name);
            current = hash_contacts[i];
        }
    }
}

/* create_list cria uma lista de contactos */
list *create_list() {
    list *new = malloc(sizeof(list));
    new->head = NULL;
    new->last = NULL;
    return new;
}

/* create_contact cria um contacto */
contact *create_contact() {
    contact *new = malloc(sizeof(contact));

    char *name, *email, *number;
    char dados[DADOSMAX];
    const char s[2] = " ", n[2] = "\n";

    new->prev=NULL;
    new->next=NULL;

    fgets(dados,DADOSMAX,stdin);

    name = strtok(dados,s);
    email = strtok(NULL,s);
    number = strtok(NULL,n);

    new->name = malloc((strlen(name)+1)*sizeof(char));
    new->email = malloc((strlen(email)+1)*sizeof(char));
    new->number = malloc((strlen(number)+1)*sizeof(char));

    strcpy(new->name,name);
    strcpy(new->email,email);
    strcpy(new->number,number);

    return new;
}

/* find_contact encontra um contacto, caso exista, e devolve-o */
contact *find_contact(char *name) {
    int i = hash(name);
    node_contact *b = hash_contacts[i];

    while (b) {
        if (!strcmp(b->c->name,name)) {
            return b->c;
        }
        b=b->next;
    }

    return NULL;
    
}

/* insert_contact insere um contacto na lista e na hash dos contactos
   e o dominio na hash dos dominios */
void insert_contact(list *l, contact *c) {
    node_contact *b;
    char *domain;
    
    if (!find_contact(c->name)) {
        if(!l->head) {
            l->head=c;
            l->last=c;
        } else {
            if (l->last == NULL) {
                c->next = NULL;
                l->head->next=c;
                l->last = c;
            } else {
                c->prev = l->last;
                l->last->next = c;
                l->last = c;
            }
        }
        b = create_node_contact(c);
        insert_in_hash(b);
        domain = get_domain(b->c->email);
        insert_in_hash_domain(domain);
        return;
    }
    destroy_contact(c);
    printf("Nome existente.\n");
}

/* command_l imprime os contactos por ordem de entrada */
void command_l(list *l) {
    contact *aux = l->head;
    while(aux) {
        printf("%s %s %s\n",aux->name,aux->email,aux->number);
        aux = aux->next;
    }
}

/* command_p imprime os dados de um determinado contacto se existir */
void command_p(char *name) {
    int i = hash(name);
    node_contact *b = hash_contacts[i];

    while (b) {
        if (!strcmp(b->c->name,name)) {
            printf("%s %s %s\n",b->c->name,b->c->email,b->c->number);
            return;
        }
        b = b->next;
    }
    printf("Nome inexistente.\n");
}

/* command_p remove um contacto da lista e da hash dos contactos e 
   o dominio da hash dos dominios, se existir */
void command_r(list *l, char *name) {
    contact *c = find_contact(name);
    char *domain;
    
    if (find_contact(name)) {
        domain = get_domain(c->email);
        remove_from_hash_domain(domain);
        remove_from_list(l,name);
        remove_from_hash_contacts(name);

        return;
    }
    printf("Nome inexistente.\n");
}

/* command_e troca o email de um determinado contacto se existir */
void command_e(char *name, char *email) {
    int i = hash(name);
    node_contact *b = hash_contacts[i];
    contact *c = find_contact(name);

    char *prev_domain, *next_domain;


    if (c==NULL) {
        printf("Nome inexistente.\n");
        return;
    }

    prev_domain = get_domain(c->email);
    next_domain = get_domain(email);

    while(b) {
        if(!strcmp(b->c->name,name)) {
            remove_from_hash_domain(prev_domain);
            insert_in_hash_domain(next_domain);
            b->c->email = (char*)realloc(b->c->email,sizeof(char)*(strlen(email)+1));
            strcpy(b->c->email,email);
            
            return;
        }
        b = b->next;
    }
    printf("Nome inexistente.\n");
}

/* command_c imprime o numero de contactos com um determinado dominio */
void command_c(char *domain) {
    int i=hash(domain);
    node_domain *d;

    d = hash_domain[i];

    while (d) {
        if (!strcmp(d->domain,domain)) {
            printf("%s:%ld\n",d->domain,d->i);
            return;
        }
        d=d->next;
    }

    printf("%s:0\n",domain);
    
}
