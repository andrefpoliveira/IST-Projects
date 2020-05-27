/*******************************/
/* Andre Oliveira -- ist193686 */
/* Nota: 20                    */
/*******************************/

#include <stdio.h>
#include <stdlib.h>
#include "aux.h"

int main() {
    list *l=NULL;
    contact *c=NULL;
    char name[NAMEMAX], *t_name;
    char dados[NAMEMAX+EMAILMAX], *email;
    char domain[EMAILMAX], *t_domain;
    int comando,i;
    const char n[2] = "\n", s[2] = " ";

    for(i=0;i<SIZE;i++) {
        hash_contacts[i]=NULL;
        hash_domain[i]=NULL;
    }

    comando=getchar();
    getchar();

    l = create_list();
    while(comando!='x') {
        switch(comando) {
            case 'a':
                c=create_contact();
                insert_contact(l,c);
                break;
            case 'l':
                command_l(l);
                break;
            case 'p':
                getchar();
                fgets(name,NAMEMAX,stdin);
                t_name = strtok(name,n);
                command_p(t_name);
                break;
            case 'r':
                getchar();
                fgets(name,NAMEMAX,stdin);
                t_name = strtok(name,n);
                command_r(l,t_name);
                break;
            case 'e':
                fgets(dados,NAMEMAX+EMAILMAX,stdin);
                t_name = strtok(dados,s);
                email = strtok(NULL,n);
                command_e(t_name,email);
                break;
            case 'c':
                scanf("%s",domain);
                t_domain = strtok(domain,n);
                command_c(t_domain);
                getchar();
                break;
        }
        comando=getchar();
    }

    destroy_hash_domain();
    destroy_hash_contacts();
    free(l);
    return 0;
}
