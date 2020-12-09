/******************************************************************************\
* Path vector routing protocol.                                                *
\******************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include "routing-simulator.h"

// Message format to send between nodes.
typedef struct {
    cost_t data[MAX_NODES];
    node_t paths[MAX_NODES][MAX_NODES];
} message_t;

// State format.
typedef struct {
    cost_t cost[MAX_NODES][MAX_NODES];
    node_t intermediates[MAX_NODES];
    node_t paths[MAX_NODES][MAX_NODES][MAX_NODES];
} state_t;

void printMatrix(state_t *s) {
    printf("--------------------------\nCosts\n");
    for (int i = get_first_node(); i <= get_last_node(); i++) {
        for (int j = get_first_node(); j <= get_last_node(); j++) {
            printf("%d ", s->cost[i][j]);
        }
        printf("\n");
    }
    printf("--------------------------\nInterms\n");
    for (int i = get_first_node(); i <= get_last_node(); i++) {
        printf("%d ", s->intermediates[i]);
    }
    printf("\n--------------------------\n");
    for (int i = get_first_node(); i<=get_last_node(); i++) {
        printf("Nó: %d\n", i);
        for (int j= get_first_node(); j<=get_last_node(); j++) {
            printf("Destino: %d  -- ", j);
            for (int k = get_first_node(); k <= get_last_node(); k++) {
                printf(" %d", s->paths[i][j][k]);
            }
            printf("\n");
        }
    }
    printf("--------------------------\n");
}

int nodeInPath(node_t* path) {
    for (int i =  get_first_node(); i <= get_last_node(); i++) {
        if (path[i] == get_current_node()) return 1;
    }
    return 0;
}

void alterPath(state_t* s, node_t destiny, node_t next_hop) {
    if (next_hop != -1) {
        s->paths[get_current_node()][destiny][0] = next_hop;
        for (int i = get_first_node(); i < get_last_node(); i++) {
            s->paths[get_current_node()][destiny][i+1] = s->paths[next_hop][destiny][i];
        }
    }
    else {
        for (int i = get_first_node(); i <= get_last_node(); i++) {
            s->paths[get_current_node()][destiny][i] = -1;
        }
    }
}

int bellmanFord(state_t *s) {
    int changed = 0;
    for (int i = get_first_node(); i <= get_last_node(); i++) {                 // Itera todos os nos -> destino

        printf("From %d, To %d\n", get_current_node(), i);

        if (i == get_current_node()) continue;                                  // Se for o no atual ignora

        cost_t minCost = COST_INFINITY;                                            // Inicia o valor da procura em infinito
        node_t bestIntermediate = -1;                                           // Inicia o intermediario a -1

        for (int j = get_first_node(); j <= get_last_node(); j++) {              // Itera todos os nos -> intermediarios
            if (get_link_cost(j) == COST_INFINITY) continue;                        // Se nao for vizinho, ignora
            if (j == get_current_node()) continue;

            int isInsidePath = nodeInPath(s->paths[j][i]);
            if (isInsidePath) continue;

            cost_t currentCost = COST_ADD(get_link_cost(j), s->cost[j][i]);

            printf("Intermedio: %d %d\n", j, currentCost);

            if (currentCost < minCost) {                                        // Se o custo atual for menor
                minCost = currentCost;                                          // Atualiza o custo
                bestIntermediate = j;                                           // Atualiza o intermediario
            }
        }
        printf("MinCost: %d // Before: %d\n", minCost, s->cost[get_current_node()][i]);
        printf("Inter anterior: %d // Novo inter: %d\n", s->intermediates[i], bestIntermediate);
        
        int costChanged = minCost != s->cost[get_current_node()][i];
        int intermediateChanged = bestIntermediate != s->intermediates[i];
        int interDead = get_link_cost(s->intermediates[i]) == COST_INFINITY;
        
        if (costChanged || (costChanged && intermediateChanged) || (intermediateChanged && interDead)) {
            printf("Nova Route %d -> %d -> %d\n", get_current_node(), bestIntermediate,i);
            s->cost[get_current_node()][i] = minCost;                               // Altera o custo na matriz
            s->intermediates[i] = bestIntermediate;
            set_route(i, bestIntermediate, minCost);                                // Altera a rota
            alterPath(s, i, bestIntermediate);
            changed = 1;
        }
    }
    return changed;
}

void sendChange(state_t *s) {
    printf("Vou enviar\n");
    for (int i = get_first_node(); i <= get_last_node(); i++) {   // Vizinhos
        if (i == get_current_node()) continue;                                  // Se for o proprio no, ignora
        if (get_link_cost(i) == COST_INFINITY) continue;          // Se nao houver ligação, ignora 

        message_t* msg = (message_t*)malloc(sizeof(message_t));

        for (int j = get_first_node(); j <= get_last_node(); j++) {              // Itera todos os nós
            msg->data[j] = s->cost[get_current_node()][j];                       // Copia o valor de custo

            for (int k = get_first_node(); k <= get_last_node(); k++) {
                msg->paths[j][k] = s->paths[get_current_node()][j][k];
            }
                
        }
        send_message(i, msg);                                              // Envia a mensagem para os vizinhos com a nova linha
    }
}


// Notify a node that a neighboring link has changed cost.
void notify_link_change(node_t neighbor, cost_t new_cost) {
    state_t *s;
    printf("Node: %d Neighbor: %d\n", get_current_node(), neighbor);


    // Criação se nao existir
    if (!(s = (state_t*)get_state())) {                                         // Se state ainda nao existe
        s = (state_t*)malloc(sizeof(state_t));    

        for (int i = get_first_node(); i <= get_last_node(); i++) {             // Itera todos os nodes

            s->intermediates[i] = -1;
            for (int j = get_first_node(); j <= get_last_node(); j++) {         // Itera todos os nodes
                if (i == j) {
                    s->cost[i][j] = 0;
                } else if (i != get_current_node() || j != neighbor) {
                    s->cost[i][j] = COST_INFINITY;
                } else if (j == neighbor) {
                    s->cost[i][j] = new_cost;
                }

                for (int k = get_first_node(); k <= get_last_node(); k++) {
                    s->paths[i][j][k] = -1;
                }
            }
        }   
    }

    printMatrix(s);
    s->cost[get_current_node()][neighbor] = new_cost;
    int changed = bellmanFord(s);
    
    if (changed) {
        sendChange(s);
    }

    printMatrix(s);

    set_state(s);
}

// Receive a message sent by a neighboring node.
void notify_receive_message(node_t sender, void *message) {
    state_t *s = (state_t*)get_state();

    message_t *msg = (message_t*)message; 

    printf("Node %d Sender %d XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n", get_current_node(), sender);

    for (int i = get_first_node(); i <= get_last_node(); i++) {
        s->cost[sender][i] = msg->data[i];
    }

    for (int i = get_first_node(); i <= get_last_node(); i++) {
        for (int j = get_first_node(); j <= get_last_node(); j++) {
            s->paths[sender][i][j] = msg->paths[i][j];
        }
    }

    printMatrix(s);

    int changed = bellmanFord(s);

    if (changed) {
        sendChange(s);
    }

    // [1 -> 2 -> 3]



    printMatrix(s);

    set_state(s);
}