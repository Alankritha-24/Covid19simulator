//This file prints the output in 2 files
//output_matrix for the adjacency matrix
//output for the output lists everyday


//including the necessary libraries or header files 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//Pointers for the output files
FILE * output1,*output2;

#define MAX_VERTICES 10000;
#define MAX_EDGES 3000;
	
//defining struct for priority queue
struct task  {
	int time; 															//for storing time at which the task must be done
	char event;															//for determining the event that must be done- transmit or recover
	int nod;															//the corresponding node for the event
	struct task* next;
}task;
//initialising time for the time of the day
int tim = 0;
//declaring the lists for containing the infected , susceptible and recovered nodes with maximum size possible(10000 in this case)
int r_list[10000],s_list[10000],i_list[10000];
//initialising the length of the lists recovered = 0; infected =0;
//here we first assume the no of elements in the n_s list is 10000 later on update it with the correct value which is calculated in int main()
int n_r=0,n_i=0,n_s=10000;

//initialising tou (transmitting rate) and v(recovering rate)
float tou = 0.01, v = 0.004 ;

//insert function with respect to time as priority
struct task * insert_q(struct task* queue, int t, char e, int n){
	struct task* newnode = (struct task * )malloc(sizeof(struct task*)); //Allocating memory
	newnode->time = t;													//storing time
	newnode->event = e;													//storing event name
	newnode->nod = n;													//storing node no.
	struct task * temp = queue;
	if(queue!= NULL){													//insertion when queue is not empty
		while(temp->next!=NULL){										//traversing through the queue till temp has a next element											
			if(temp->next->time <= t ){									//traversing till the task where the time in the next task is greater than the current task
				temp = temp->next;
			}
			else{
				break;													//if time in the next task is greater than the current task then break from the loop
			}
		}
	//case when the queue has only one element
	//the next element is null and we have to check whether the current task has time greater than the existing one
		if(temp->next==NULL){												
			if(temp->time >t){											//if the existing task has greater time, then current task becomes the first task and the existing one suceeds it
				newnode->next = temp;
				queue = newnode;
			}
			else{
				newnode->next = temp->next;								//else it follows the normal insertion technique..gets inserted at the end like the second task
				temp->next = newnode;
			}
		}
		else{
			newnode->next = temp->next;									//case when queue has at least 2 tasks
			temp->next = newnode;
		}
	}
	else{																//insertion for the first time in the queue
		queue = newnode;
		newnode->next = NULL;
	}
	return queue;														//returning the queue
}

//delete function **** deletes the first task in the queue
struct task* delete_q(struct task* queue){
	struct task* temp= queue;
	if(queue!=NULL){													//checking if queue is not empty
		queue = queue->next;
		free(temp);
	}
	else{
		printf("Error:	Queue is already empty\n");
	}
	return queue;
}

//Function to insert in an array given the capacity of the array and the number of elements it already has
int insertSorted(int arr[], int n, int key, int capacity) {
	// n = no_of elements it already has
    if (n >= capacity) 									//cannot insert more elements if it exceeds capacity
       return n; 
	arr[n] = key; 
   return (n + 1); 										//updating the incremented value
} 

// Function to implement search operation in an array  
int findElement(int arr[], int n, int key) { 
	//n = number of elements in the array and key is the key that must be searched
    int i; 
    for (i = 0; i < n; i++) 
        if (arr[i] == key) 
            return i; 						//returns the position of the key if it is preent in the array
   return - 1; 								//returns -1 if the key is not present in the array
} 

// Function to delete an element from an array
int deleteElement(int arr[], int n, int key) { 
    // Find position of element to be deleted 
    int pos = findElement(arr, n, key); 
	if (pos == - 1) { 
        printf("Element not found"); 							//element cant be deleted if it is not there in the queue
        return n; 												//returning the same value of n since 
    } 
	// Deleting element 
    int i; 
    for (i = pos; i < n - 1; i++){ 								//updating the next elements after the deleted element
        arr[i] = arr[i + 1]; 
	}
	return n - 1; 												//returning the decremented value of n
} 
  
//Function when Task is recovery i.e, the task to be performed is recovery
struct task* recovery(struct task * queue){
	struct task * temp = queue;
	int k = temp->nod;
	//if the task is recovery then add the recovered element to the recovered list
	n_r = insertSorted(r_list,n_r,k,10000);
	//if the infected list contains the node then delete it from the list
	if(findElement(i_list,n_i,k)!=-1){
		n_i = deleteElement(i_list,n_i, k);
	}
	queue = delete_q(queue);				//deleting the task from the queue
	return queue;
}

//functions to find the time required ***i e by tossing the coin

//function to find a random decimal number
float rand_1(){
	int k = rand()%100000;
	float m = k/100000.0;
	return m;								//returning the float value
}
//function to calculate the number of days until head comes
int time_cal(float p){										//p being the probability of heads
	int i;
	for(i=1;;i++){
		float j =rand_1();									//finding random elements
		if(j<p){
			break;											//if j is less than p then break otherwise continue
		}
		else{
			continue;
		}
	}
	return i;												//returns the number of days after which head appears
}

//function to print the tasks in queue

void print_time(struct task* queue){
	struct task* temp = queue;
	while(temp!=NULL){
		printf("%d	%c	%d	;",temp->time,temp->event,temp->nod);
		temp = temp->next;
	}
	printf("\n");
}

//function when task is to transmit the disease
struct task* transmit(struct task* queue,int** adjacency_matrix,int numberOfVertices){
	struct task* temp = queue;
	int k = temp->nod;
	int j =0;
	while(j<numberOfVertices){											//traversing through the array for the particular row where all the linked elements are having entries as 1
		if(findElement(s_list,n_s,j)!= -1){								//checking whether the node is present in susceptible list or not
			if(adjacency_matrix[k][j]==1){								//checking whether the nodes are linked or not
				n_i = insertSorted(i_list,n_i,j,10000);					//inserting in the infected list
				n_s = deleteElement(s_list,n_s,j);						//deleting from the susceptible list
				queue = insert_q(queue,tim+ time_cal(tou),'t',j);		//insering a new task in queue for transmitting
				queue = insert_q(queue, tim+ time_cal(v),'r',j);		//inserting a new task in queue for recovering
				j++;
			}
			else{
				j++;													//otherwise just repeat for the next element
			}
		}
		else{
			j++;
		}
	}
	queue = delete_q(queue);											//deleting the task from the queue
	return queue;
}

//task to perform tasks that have to be done on that particular day
struct task* read_time(struct task* queue,int **adjacency_matrix,int numberOfVertices){
	struct task* temp = queue;
	while(temp!=NULL){													//while queue has some tasks							
		if(temp->time == tim){											//checking whether the task must be performed on that particular day or not
			if(temp->event=='t'){										//transmit task if event is t
				queue = transmit(queue,adjacency_matrix,numberOfVertices);
			}
			else if(temp->event == 'r'){								//recover task if event is r
				queue = recovery(queue);
			}
			temp = queue;												//reassigning the value of temp to the head of the queue
		}
		else{
			break;														//breaking from the queue when the time is not equal to the day time												
		}
	}
	tim++;																//incrementing time when the tasks are done for the day
	return queue;
}

//Functions for generating the adjacency matrix

// function for initializing the adjacency matrix to zero					
void graph_initialize(int **adjacency_matrix, int no_of_vertices)  {
    for(int i=0; i<no_of_vertices; i=i+1){
        for(int j=0; j<no_of_vertices; j=j+1){
            adjacency_matrix[i][j] = 0; // initially all values are zero
        }
    }
}
//Function to create  the adjacency matrix
void create_graph(int **adjacent_matrix, int no_of_vertices, int max_no_of_edges) { // function for creating the graph
	int vertex_counter,edge_counter,link_vertex;
    for(vertex_counter=0; vertex_counter < no_of_vertices; vertex_counter=vertex_counter+1) {	// counter for incrementing the vertexes
        for(edge_counter=0; edge_counter < max_no_of_edges; edge_counter=edge_counter+1){ 		 // counter for incrementing the edges
            if(rand()%2==1) {											// vertex has a edge only if this condition satisfies
                link_vertex = rand() % no_of_vertices; 					 // randomly generating the vertex for creating an edge between vertexes
                adjacent_matrix[vertex_counter][link_vertex] = 1; 		// if there is an edge between the two vertexes then make it 1
            }
        }
    }
}

int main(){
	output1 = fopen("output_matrix.txt","w");
	output2 = fopen("output.txt","w");
	srand(time(NULL)); 													
	 // randomly generating the no of vertexes and edges in the graph
    int no_of_vertexes=0;
    while(no_of_vertexes < 1) // opp until we get no of vertexes greater than 0
    {
        no_of_vertexes = rand() % MAX_VERTICES;
    }
    int max_no_of_edges = rand() %MAX_EDGES;     // maximum no of edges from each vertex
	printf("Maximum no. of vertices : %d\n",no_of_vertexes);
    int **adjacency_matrix = (int**)malloc(no_of_vertexes*sizeof(int *));   // graph implementation using adjacency matrix
    for (int i=0; i<no_of_vertexes; i++)
        adjacency_matrix[i] = (int*)malloc(no_of_vertexes*sizeof(int));  // adjacency matrix is a square matrix of size=no_of_vertexes
        
    //creating the adjacency matrix
    graph_initialize(adjacency_matrix,no_of_vertexes);
	create_graph(adjacency_matrix,no_of_vertexes,max_no_of_edges);
	
	//updating the number of elements in the susceptible list
	n_s = no_of_vertexes;
	
	for(int y = 0;y<no_of_vertexes;y++){
		for(int j =0; j<no_of_vertexes; j++){
			fprintf(output1,"%d", adjacency_matrix[y][j]);
		}
		fprintf(output1,"\n");
	}
	//initially, assuming that all the nodes are susceptible
	int u = no_of_vertexes;	
	for(int i=0;i<u;i++){
		s_list[i]=i;
	}
	//initialising queue to NULL
	struct task* queue = NULL;
	
	//finding the initially affected node
	int initially_infected = rand()%no_of_vertexes;
	
	//inserting the initially affected member in the queue
	queue = insert_q(queue,tim+time_cal(tou),'t',initially_infected);
	queue = insert_q(queue,tim+time_cal(v),'r',initially_infected);	
	
	//updating the susceptible and the infected lists
	n_s = deleteElement(s_list,n_s,initially_infected);
	n_i = insertSorted(i_list, n_i,initially_infected, 10000);	
	
	//printing for 300 days
	while(tim<301){
		fprintf(output2,"TIME : day %d\n",tim);
		queue = read_time(queue,adjacency_matrix,no_of_vertexes);			//performs task for the particular day
		fprintf(output2, "S List:");													//printing the s list
		for(int i =0 ; i<n_s;i++){
			fprintf(output2,"%d	; ",s_list[i]);
		}
		fprintf(output2,"\n");
		fprintf(output2,"I List:");													//printing the i list
		for(int i =0 ; i<n_i;i++){
			fprintf(output2,"%d	; ",i_list[i]);
		}
		fprintf(output2,"\n");
		fprintf(output2,"R List:");													//printing the r list
		for(int i =0 ; i<n_r;i++){
			fprintf(output2, "%d	; ",r_list[i]);
		}
		fprintf(output2,"\n");
	}
	printf("done\n");														//prints done once the job is done
	
	fclose(output1);
	fclose(output2);
	return 0;
}

	
		
	
	
