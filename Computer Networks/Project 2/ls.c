/******************************************************************************\
* Link state routing protocol.                                                 *
\******************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include "routing-simulator.h"

typedef struct {
  cost_t link_cost[MAX_NODES];
  int version;
} link_state_t;

// Message format to send between nodes.
typedef struct {
  link_state_t ls[MAX_NODES];
} message_t;

// State format.
typedef struct {
  link_state_t links[MAX_NODES];
  node_t intermediates[MAX_NODES];
} state_t;


void dijkstra(state_t *s)
{
	int cost[MAX_NODES][MAX_NODES],distance[MAX_NODES],pred[MAX_NODES];
	int visited[MAX_NODES],count,mindistance,nextnode,i,j;
	
	//pred[] stores the predecessor of each node
	//count gives the number of nodes seen so far
	//create the cost matrix
	for(i=get_first_node();i<=get_last_node();i++)
		for(j=get_first_node();j<=get_last_node();j++)
			cost[i][j]= s->links[i].link_cost[j];
	
	//initialize pred[],distance[] and visited[]
	for(i=get_first_node();i<=get_last_node();i++)
	{
		distance[i]=cost[get_current_node()][i];
		pred[i]=get_current_node();
		visited[i]=0;
	}
	
	distance[get_current_node()]=0;
	visited[get_current_node()]=1;
	count=1;
	
	while(count<=get_last_node()-1)
	{
		mindistance=COST_INFINITY;
		
		//nextnode gives the node at minimum distance
		for(i=get_first_node();i<=get_last_node();i++) 
			if(distance[i]<mindistance&&!visited[i])
			{
				mindistance=distance[i];
				nextnode=i;
			}
			
    //check if a better path exists through nextnode			
    visited[nextnode]=1;
    for(i=get_first_node();i<=get_last_node();i++)
      if(!visited[i]) {
        cost_t c = COST_ADD(mindistance, cost[nextnode][i]);                // Messed up. Custo esta alterado
        if(c<distance[i])
        {
          distance[i]=c;
          pred[i]=nextnode;
        }
      }
    
		count++;
	}
 
	//print the path and distance of each node
	for(i=get_first_node();i<=get_last_node();i++) {
    int minDist = distance[i];
    node_t interm = i;


		if(i!=get_current_node())
		{
			printf("\nDistance of node%d=%d",i,distance[i]);
			printf("\nPath=%d",i);
			
			j=i;
			do
			{
        interm = j;
				j=pred[j];
				printf("<-%d",j);
			}while(j!=get_current_node());
      printf("\n");
	  }

    printf("Para o nó %d temos distancia %d e intermediario %d\n", i, minDist, interm);
    printf("Distancia anterior: %d e interm anterior: %d\n", s->links[get_current_node()].link_cost[i], s->intermediates[i]);
    if (minDist == COST_INFINITY) interm = -1;
    if (minDist != s->links[get_current_node()].link_cost[i] || interm != s->intermediates[i]) {
      printf("Encontrei um novo caminho de %d para %d: Custo: %d, Interm: %d\n", get_current_node(), i, minDist, interm);
      set_route(i, interm, minDist);
      s->intermediates[i] = interm;
    }
  }
}



void sendChange(state_t *s) {

  for (int i = get_first_node(); i <= get_last_node(); i++) {
    if (i == get_current_node()) continue;                                  // Se for o proprio no, ignora
    if (get_link_cost(i) == COST_INFINITY) continue;          // Se nao houver ligação, ignora 

    message_t* msg = (message_t*)malloc(sizeof(message_t));
    for (int j = get_first_node(); j <= get_last_node(); j++) {
      msg->ls[j].version = s->links[j].version;

      for (int k = get_first_node(); k <= get_last_node(); k++) {
        msg->ls[j].link_cost[k] = s->links[j].link_cost[k];
      }
    }

    send_message(i, msg);                                              // Envia a mensagem para os vizinhos com a nova linha
  }
}

// Notify a node that a neighboring link has changed cost.
void notify_link_change(node_t neighbor, cost_t new_cost) {
  state_t *s;
  printf("Node: %d Neighbor: %d\n", get_current_node(), neighbor);

  if (!(s = (state_t*)get_state())) {
    s = (state_t*)malloc(sizeof(state_t));

    for (int i = get_first_node(); i <= get_last_node(); i++) {

      s->links[i].version = -1;

      for (int j = get_first_node(); j <= get_last_node(); j++) {

        if (i == get_current_node()) {
          s->links[i].link_cost[j] = get_link_cost(j);
          s->intermediates[j] = -1;
          if (j == i) {
            s->intermediates[j] = j;
          }
        } else {
          s->links[i].link_cost[j] = COST_INFINITY;
        }
      }
    }
  }

  s->links[get_current_node()].link_cost[neighbor] = new_cost;
  s->links[get_current_node()].version++;

  dijkstra(s);

  sendChange(s);
  set_state(s);
}

// Receive a message sent by a neighboring node.
void notify_receive_message(node_t sender, void *message) {

  printf("Current node: %d XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n", get_current_node());
  state_t *s = (state_t*)get_state();

  message_t*msg = (message_t*)message;

  int changed = 0;

  for (int i = get_first_node(); i <= get_last_node(); i++) {
    if (s->links[i].version < msg->ls[i].version) {
      changed = 1;
      s->links[i].version = msg->ls[i].version;
      for (int j = get_first_node(); j <= get_last_node(); j++) {
        s->links[i].link_cost[j] = msg->ls[i].link_cost[j];
      }
    }
  }

  if (changed) {
    dijkstra(s);
    sendChange(s);
  }


  set_state(s);
}
