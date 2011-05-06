#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct n{
	int label;
	int capacity;
	struct n *next;
	int edge_type;
};

typedef struct n node;

struct path_list {
	char pathname[4];
	int capacity;
	struct path_list *next;
};

struct path_list *pathlst = NULL;
int source, sink, max_flow, delta, nV, nE, max_capacity, nP=0;

node * adj_list;
node * max_flow_graph;
node *path; 
void print_paths();
void print_adj_list();

int bfs()
{
	int i,discovered[nV];
	int count=0, found_sink = 0;
	node *pre_temp,*temp, *list, *list_temp;

	for (i=0;i<nV;i++)
		discovered[i] = 0;
	path = (node *) malloc(sizeof(node)*nV);

	discovered[source-1] = 1; /* mark start node as discovered */
	
	list = (node *) malloc(sizeof(node));
	
	list->next = NULL;
	list->label = source;

	printf("--------------------------------------------------------------\n");
	printf("                 BFS traversal of graph\n");
	printf("--------------------------------------------------------------\n");
	while(list)
	{	
		/* delete a node from a queue maintained for BFS */
		list_temp = list;
		list = list->next;
		list_temp->next = NULL;
		pre_temp = adj_list+(list_temp->label-1);
	
 		
		temp = pre_temp->next;
		printf("%d ",pre_temp->label);
		/* start scanning all its immediate adjacent nodes */
		while(temp!=NULL)
		{
			if(temp->label == sink && temp->capacity > 0 && temp->capacity >= delta)	{
				printf("%d,",temp->label);
				found_sink = 1;
				*(path+(temp->label)-1) = *pre_temp;
				(path+(temp->label)-1)->capacity = temp->capacity;
				break;
			}
			else if(!discovered[(temp->label)-1] && temp->capacity > 0 && temp->capacity >= delta)	{
				discovered[(temp->label)-1] = 1;
				printf("%d,",temp->label);
				if(list != NULL)	{
					list_temp = list;
					while(list_temp->next != NULL)
						list_temp = list_temp->next;
					list_temp->next = (node *) malloc(sizeof(node));
					list_temp = list_temp->next;
					list_temp->label = temp->label;
					list_temp->next = NULL;
						
				}else {
					list = (node *) malloc(sizeof(node));
					list->next = NULL;
					list->label = temp->label;
				}
				*(path+(temp->label)-1) = *pre_temp;
				(path+(temp->label)-1)->capacity = temp->capacity;
				
			}
			temp = temp -> next;		
		}
		printf("\n");
		if(found_sink == 1)
			break;
		
	}	
	printf("--------------------------------------------------------------\n");
	if(found_sink)
		print_paths();
	print_adj_list();
	return found_sink;
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

		/* note that all edges are forward edges while creating a graph, forward = 1 and backward = 0 */
		new_node -> edge_type = 1; 	

		/* while creating adjacency list we will find maximum edge capacity from graph */
		if(new_node -> capacity > max_capacity)		
			max_capacity = new_node -> capacity;
		
		if(current_node->next!=NULL)	{
			while(current_node->next->label<= new_node->label)	/*this traversal ensures that adjacency list is always sorted */
			{
				current_node = current_node->next;
				if(current_node->next == NULL)
					break;
			}
		}

		temp = current_node->next;
		current_node->next = new_node;
		new_node->next = temp;
			
	}
	printf("Max capacity %d\n",max_capacity);
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
			printf("%d:%d ",temp->label, temp->capacity);
			temp = temp -> next;
		}		
		printf("\n");
	}
	printf("------------------------------------------------\n");
}

void print_paths()
{
	node *temp;
	printf("Path selected by bfs: %d:0 ",sink);
	
	temp = path+(sink-1);
	
	while(1)
	{
		printf("%d:%d ",temp->label, temp->capacity);
		if(temp->label == source)	
			break;
		temp = path+((temp->label)-1);
	}	
	printf("\n");
}

int augment_path(int flow)
{	
	node *path_temp, *graph_node, *new_node, *temp;
	int edge_start, edge_end, found=0;

	path_temp = path+(sink-1);
	edge_end = sink;
	edge_start = path_temp->label;

	while(1)
	{
		
		/* if flow > 0, add a backward edge equivalent to flow value */
		found = 0;

		if(flow > 0)
		{
			graph_node = adj_list+edge_end-1;
			
			new_node = (node *) malloc(sizeof(node));
			new_node->label = edge_start;
			new_node->capacity = flow;
			new_node->edge_type = 0;	/* 0 signifies backward edge */

			graph_node = graph_node->next;

			while(graph_node != NULL)	{
				if(graph_node->label == new_node->label)	{
					graph_node->capacity = graph_node->capacity+new_node->capacity;
					found = 1;
				}
				graph_node = graph_node->next;
			}
			if(!found)	{
				graph_node = adj_list+edge_end-1;
				
				if(graph_node->next !=NULL)	{
					while(graph_node->next->label<= new_node->label)	/*this traversal ensures that adjacency list is always sorted */
					{
						graph_node = graph_node->next;
						if(graph_node -> next == NULL)
							break;
					}
				}

				temp = graph_node->next;
				graph_node->next = new_node;
				new_node->next = temp;
			}
		}
		
		/* reduce the capacity of edge on path by (edge_capacity-flow) value */

		graph_node = adj_list+edge_start-1;

		while(graph_node->next != NULL)
		{
			graph_node = graph_node->next;
			if(graph_node->label == edge_end)
			{
				graph_node->capacity = graph_node->capacity - flow;
				break;
			}
		}
				
		if(path_temp->label == source)	
			break;
		path_temp = path+((path_temp->label)-1);
		edge_end = edge_start;
		edge_start = path_temp->label;
	}	
}

int bottleneck()
{
	node *temp;
	int min_cap;
	
	temp = path+(sink-1);
	min_cap = temp->capacity;
	while(1)
	{
		if(temp->label == source)	
			break;
		temp = path+((temp->label)-1);
		if(temp->capacity<min_cap)
			min_cap = temp->capacity;
	}	
	return min_cap;
	
}

void add_to_pathlist(int b)
{
	node *temp;
	char pathname1[4], pathname2[4];
	struct path_list *temp_node, *new_node, *dummy;
	int i,j,edge_start, edge_end, found =0 ;

	temp = path+(sink-1);
	edge_end = sink;
	edge_start = temp->label;
	i = 0;
	temp_node = pathlst;
	while(1)
	{
		found = 0;
		sprintf(pathname1,"%d %d",edge_start,edge_end);	
		sprintf(pathname2,"%d %d",edge_end,edge_start);	
		
		while(temp_node!=NULL)
		{
			if(!strcmp(temp_node->pathname,pathname1))	{
				temp_node->capacity = temp_node->capacity + b;
				found = 1;
			}
			else if (!strcmp(temp_node->pathname,pathname2))
			{	
				temp_node->capacity = temp_node->capacity - b;
				found = 1;
			}
			temp_node = temp_node->next;
		}
		
		if(!found)
		{
			if(pathlst == NULL)
			{
				pathlst = (struct path_list*) malloc(sizeof(struct path_list));
				temp_node = pathlst;
			}
			else	{
				temp_node = pathlst;
				while(temp_node->next->pathname[0]< pathname1[0])
				{
					temp_node = temp_node->next;
					if(temp_node->next == NULL)
						break;
				}
				if(temp_node->next!=NULL)
				{
					if(temp_node->next->pathname[0]==pathname1[0])
					{
						while(temp_node->next->pathname[2]< pathname1[2])
						{
							temp_node = temp_node->next;
							if(temp_node->next == NULL)
								break;
						}
					}
				}
			}
			new_node = (struct path_list*) malloc(sizeof(struct path_list));
			sprintf(new_node->pathname,"%d %d",edge_start,edge_end);	
			new_node->capacity = b;

			dummy = temp_node->next;
			temp_node->next = new_node;
			new_node->next = dummy;
			nP++;
		}

		if(temp->label == source)	
			break;
		temp = path+((temp->label)-1);
		edge_end = edge_start;
		edge_start = temp->label;
	}	
	
}

void calculate_min_cut(FILE *f)
{
	int i, main_sink, nA=0, nB=0;
	int A[nV], B[nV];

	main_sink = sink;

	for(i=1;i<=nV;i++)
	{
		if (i == source || i == main_sink)
		{
			if(i==source)	/* consider source to be a part of set A */
				A[nA++]=i;
			if(i==main_sink)	/*consider sink to be a part of set B */
				B[nB++]=i;
			continue;
		}
		/* for all nodes except source and sink, compute BFS from source to each node in graph and see if its
		   reacheable from source. If there is path path from source to node, it belongs to source set A else
		   it belongs to sink set B */ 
		else	
		{
			sink = i;
			if(bfs())	{
				A[nA++] = i;						
			}else	{
				B[nB++] = i;
			}
		}		
	}

	/* Push formatted min-cut output to output file */
	fprintf(f,"\nMin-cut capacity=%d\n",max_flow);
	fprintf(f,"The min-cut:\n");
	fprintf(f,"The set S:\n");
	for(i=0;i<nA;i++)
		fprintf(f,"%d, ",A[i]);
	fprintf(f,"\nThe set T:\n");
	for(i=0;i<nB;i++)
		fprintf(f,"%d, ",B[i]);
	fprintf(f,"\n");

}

/*
	This is a main Max Flow Capacity scaling algorithm implementation
*/
void capacity_scaling()
{
	int b,i;
	struct path_list *temp;
	max_flow = 0;
	FILE *op = fopen("output.txt","w");
	
	/* delta = nearest power of 2 from Maximum capacity, which is less than Maximum capacity in graph */

	double power = log((double)max_capacity)/log(2);
	//printf("power: %d log 8: %lf\n",power,log(max_capacity));
	delta = (int)pow(2,power);
	
	//pathlst = (struct path_list *) malloc(sizeof(struct path_list)*nE*2);
	
	while(delta>=1)
	{	
		while(bfs())	{
			b= bottleneck();	/* find a width of a path found by BFS on graph */
			max_flow = max_flow + b;	/* add bottleneck to max_flow value */ 
			add_to_pathlist(b);	
			augment_path(b); 	/* augment newly found path to adjust forward and backward edges */
		}
		printf("delta: %d bottleneck: %d\n",delta,b);
		fprintf(op,"With DELTA=%d, Flow Value=%d\n",delta,max_flow); 
		delta = delta/2;
	}

	printf("Max Flow\n");
	fprintf(op,"Max-flow value=%d\n",max_flow);

	/* Write Max Flow graph to output file */
	fprintf(op,"The max-flow:\n");
	temp = pathlst;
	temp = temp->next;
	//for(i=0;i<nP;i++)
	while(temp!=NULL)
	{
		printf("%s",temp->pathname);
		printf(" %d\n",temp->capacity);
		fprintf(op,"%s %d\n",temp->pathname,temp->capacity);
		temp = temp->next;
	}

	calculate_min_cut(op);

	fclose(op);
	printf("max flow: %d\n",max_flow);
}

int main()
{
	FILE *f = fopen("sample-input01.txt","r");

	fscanf(f,"%d",&nV);
	fscanf(f,"%d",&nE);
	
	printf("Number of nodes: %d Number of edges: %d\n",nV,nE);
	
	adj_list = (node *) malloc(sizeof(node) * nV);

	create_adj_list(f);
	fclose(f);
	print_adj_list();
	capacity_scaling();
	return 0;
}

