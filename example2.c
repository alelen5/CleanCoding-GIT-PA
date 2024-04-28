#include <stdlib.h>
#include <stdio.h>

// Definirea structurii unui nod din graf
typedef struct Node 
{
    int data;             // Informația stocată în nod
    struct Node *next;    // Pointer către următorul nod din listă
} NODE;

// Definirea structurii unui graf
typedef struct Graph 
{
    int vertices;               // Numărul de vârfuri din graf
    int *visited;               // Vector pentru a marca vârfurile vizitate
    struct Node **adjacency_lists; // Listele de adiacență pentru fiecare vârf
} GPH;

// Funcția pentru crearea unui nou nod într-o listă simplu înlănțuită
NODE *create_node(int v) 
{
    // Alocare memorie pentru un nou nod
    NODE* new_node = (NODE*) malloc(sizeof(NODE));
    if (new_node == NULL) 
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    // Inițializare dată și referință către următorul nod
    new_node->data = v;
    new_node->next = NULL;
    return new_node;
}

// Funcția pentru crearea unui nou graf cu un anumit număr de vârfuri
GPH *create_graph(int vertices) 
{
    // Alocare memorie pentru structura graf
    GPH* graph = (GPH*) malloc(sizeof(GPH));
    if (graph == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Inițializare număr de vârfuri și alocare memorie pentru vectorul de vârfuri vizitate
    graph->vertices = vertices;
    graph->visited = (int*) malloc(vertices * sizeof(int));
    if (graph->visited == NULL) 
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Alocare memorie pentru listele de adiacență și inițializare a vârfurilor vizitate
    graph->adjacency_lists =(NODE**) malloc(vertices * sizeof(NODE *));
    if (graph->adjacency_lists == NULL) 
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Inițializare listele de adiacență și vectorul de vârfuri vizitate
    for (int i = 0; i < vertices; i++) 
    {
        graph->adjacency_lists[i] = NULL;
        graph->visited[i] = 0;
    }

    return graph;
}

// Funcția pentru adăugarea unei muchii între două vârfuri în graf
void add_edge(GPH *graph, int src, int dest) 
{
    // Adăugare muchie de la src la dest
    NODE *new_node = create_node(dest);
    new_node->next = graph->adjacency_lists[src];
    graph->adjacency_lists[src] = new_node;

    // Adăugare muchie de la dest la src (graful este neorientat)
    new_node = create_node(src);
    new_node->next = graph->adjacency_lists[dest];
    graph->adjacency_lists[dest] = new_node;
}

// Funcția pentru inserarea de muchii în graf
void insert_edges(int nr_of_vertices, int nr_of_edges, GPH *graph) 
{
    int src, dest;
    printf("Adăugați %d muchii (de la 1 la %d)\n", nr_of_edges, nr_of_vertices);
    for (int i = 0; i < nr_of_edges; i++) {
        scanf("%d %d", &src, &dest);
        add_edge(graph, src, dest);
    }
}

// Funcția pentru verificarea dacă o coadă este goală
int is_empty(NODE *queue) 
{
    return queue == NULL;
}

// Funcția pentru adăugarea unui element în coadă
void enqueue(NODE **queue, int data) 
{
    NODE *new_node = create_node(data);
    if (is_empty(*queue)) 
    {
        *queue = new_node;
    } else 
    {
        NODE *temp = *queue;
        while (temp->next) 
        {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

// Funcția pentru eliminarea unui element din coadă
int dequeue(NODE **queue) 
{
    int data = (*queue)->data;
    NODE *temp = *queue;
    *queue = (*queue)->next;
    free(temp);
    return data;
}

void print_graph(GPH *graph)
{
    for (int i = 0; i < graph->vertices; i++)
    {
        NODE *temp = graph->adjacency_lists[i];

        while (temp != NULL) 
        {
            printf("%d ", temp->data);
            temp = temp->next;
        }
        printf("\n");
    }
}

void print_queue(NODE *queue)
{
    while (queue != NULL)
    {
        printf("%d ", queue->data);
        queue = queue->next;
    }
}

void wipe_visited_list(GPH *graph, int nr_of_vertices)
{
    for (int i = 0; i < nr_of_vertices; i++)
    {
        graph->visited[i] = 0;
    }
}

// Parcurgere DFS (Depth-First Search)
void DFS(GPH *graph, int vertex_nr)
{
    NODE *adj_list = graph->adjacency_lists[vertex_nr];
    NODE *temp = adj_list;

    graph->visited[vertex_nr] = 1;
    printf("%d->", vertex_nr);

    while (temp != NULL)
    {
        int connected_vertex = temp->data;
        if (graph->visited[connected_vertex] == 0)
        {
            DFS(graph, connected_vertex);
        }
        temp = temp->next;
    }
}

// Parcurgere BFS (Breadth-First Search)
void BFS(GPH *graph, int start)
{
    // Coada pentru a ține vârfurile care urmează să fie vizitate
    NODE *queue = NULL;

    // Marchează vârful de start ca vizitat și îl adaugă în coadă
    graph->visited[start] = 1;
    enqueue(&queue, start);

    // Parcurge coada și vizitează vârfurile adiacente
    while (!is_empty(queue))
    {
        // Extrage primul vârf din coadă și-l afișează
        int current = dequeue(&queue);
        printf("%d ", current);

        // Obține lista de adiacență pentru vârful curent
        NODE *temp = graph->adjacency_lists[current];

        // Parcurge fiecare vârf adiacent și-l adaugă în coadă dacă nu a fost vizitat
        while (temp != NULL)
        {
            int adj_vertex = temp->data;

            // Verifică dacă vârful adiacent nu a fost vizitat și-l adaugă în coadă
            if (graph->visited[adj_vertex] == 0)
            {
                graph->visited[adj_vertex] = 1;
                enqueue(&queue, adj_vertex);
            }

            // Avansează la următorul vârf adiacent
            temp = temp->next;
        }
    }
}

int main()
{
    int nr_of_vertices;
    int nr_of_edges;
    int src, dest;
    int starting_vertex;

    printf("Câte noduri are graful? ");
    scanf("%d", &nr_of_vertices);

    printf("Câte muchii are graful? ");
    scanf("%d", &nr_of_edges);

    // Creare graf
    GPH *graph = create_graph(nr_of_vertices);

    // Adăugare muchii în graf
    insert_edges(nr_of_vertices, nr_of_edges, graph);

    // Parcurgere DFS
    printf("De unde plecăm în DFS? ");
    scanf("%d", &starting_vertex);
    printf("Parcurgere cu DFS: ");
    DFS(graph, starting_vertex);
    wipe_visited_list(graph, nr_of_vertices); // Resetare listă de vizitare pentru utilizare ulterioară

    printf("\n");

    // Parcurgere BFS
    printf("De unde plecăm în BFS? ");
    scanf("%d", &starting_vertex);
    printf("Parcurgere cu BFS: ");
    BFS(graph, starting_vertex);

    return 0;
}
