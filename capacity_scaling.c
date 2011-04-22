#include <stdio.h>
#include <math.h>
#include <stdlib.h>

struct n{
	int label;
	int capacity;
	struct n *next;
	int edge_type;
};

typedef struct n node;

int source, sink, max_flow, delta, nV, nE;

node * adj_list;
node *path; 

void bfs()
{
	int i,discovered[nV];
	int list[nV], count=0;
	node *temp, *list;

	for (i=0;i<nV;i++)
		discovered[i] = 0;
	
	path = (node *) malloc(sizeof(node)*nV);

	discovered[source-1] = 1; /* mark start node as discovered */

	list = (node *) malloc(sizeof(node));
	list->next = NULL;
	list->label = source;
	i = 1;
	while(list)
	{	
		temp = adj_list+(source-1);
		while(temp!=NULL)
		{
			if(temp
		}
	}	
}
void create_adj_list(FILE *f)
{
	int temp_data, i;
	node *current_node, *new_node, *temp;

	for(i =0; i< nV ; i++)
	{
		current_node = adj_list+i;
		current_node->label = i+1;
		current_node->capacity = 0;
		current_node->next = NULL;
	}
	
	/*read source and sink from input file */

	fscanf(f,"%d",&source);
	fscanf(f,"%d",&sink);

	for(i=0 ; i< nE; i++)	{
		/* find a start vertex of an edge */
		fscanf(f,"%d",&temp_data);
		current_node = adj_list+(temp_data-1); /* go to that start node in adjacency list */

		fscanf(f,"%d",&temp_data);  /* Read a end node of an edge */

		/* allocate a new node to add into adjacency list */
		new_node = (node *) malloc(sizeof (node));
		new_node-> label = temp_data;
		fscanf(f,"%d",&temp_data);
		new_node -> capacity = temp_data;

		temp = current_node->next;
		current_node->next = new_node;
		new_node->next = temp;
			
	}
}
void print_adj_list()
{
	int i;
	node *temp;
	
	printf("-----------Graph's adjacency list--------------\n");
	for(i=0;i<nV;i++)
	{	
		temp = adj_list+i;
		while(temp != NULL)	{
			printf("%d ",temp->label);
			temp = temp -> next;
		}		
		printf("\n");
	}
	printf("------------------------------------------------\n");
}
int main()
{
	FILE *f = fopen("input.txt","r");

	fscanf(f,"%d",&nV);
	fscanf(f,"%d",&nE);
	
	printf("Number of nodes: %d Number of edges: %d\n",nV,nE);
	
	adj_list = (node *) malloc(sizeof(node) * nV);

	create_adj_list(f);
	print_adj_list();
	return 0;
}
