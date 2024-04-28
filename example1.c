#include <stdlib.h>
#include <stdio.h>

// Structura pentru un nod într-o listă simplu înlănțuită
typedef struct Node 
{
    int data;            // Data stocată în nod
    struct Node *next;   // Referință către următorul nod în listă
} Node;

// Structura pentru graf
typedef struct Graph 
{
    int v;               // Numărul de vârfuri (noduri) în graf
    int *vis;            // Vector pentru a marca vârfurile vizitate
    struct Node **alist; // Listele de adiacență pentru fiecare vârf în graf
} Graph;

// Structura pentru stivă
typedef struct Stack 
{
    int top;             // Indicatorul vârfului din stivă
    int capacity;        // Capacitatea maximă a stivei
    int *array;          // Array-ul pentru stivă
} Stack;

// Funcție pentru crearea unui nou nod într-o listă simplu înlănțuită
Node *create_node(int v) 
{
    Node *new_node = (Node*) malloc(sizeof(Node)); // Alocare memorie pentru noul nod
    if (new_node == NULL) 
    { // Verificare dacă alocarea a eșuat
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = v; // Inițializare dată
    new_node->next = NULL; // Inițializare referință la următorul nod
    return new_node;
}

// Funcție pentru adăugarea unei muchii între două vârfuri în graf
void add_edge(Graph *g, int src, int dest) 
{
    // Crearea unui nod nou pentru destinație și adăugarea sa la începutul listei de adiacență a sursei
    Node *new_node_dest = create_node(dest);
    new_node_dest->next = g->alist[src];
    g->alist[src] = new_node_dest;

    // Crearea unui nod nou pentru sursă și adăugarea sa la începutul listei de adiacență a destinației
    Node *new_node_src = create_node(src);
    new_node_src->next = g->alist[dest];
    g->alist[dest] = new_node_src;
}

// Funcție pentru crearea unui graf nou cu un număr dat de vârfuri
Graph *create_graph(int v) 
{
    // Alocare memorie pentru structura graf
    Graph *g = (Graph*) malloc(sizeof(Graph));
    if (g == NULL) 
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Inițializare număr de vârfuri și alocare memorie pentru vectorul de vârfuri vizitate
    g->v = v;
    g->vis = (int *)malloc(v * sizeof(int));
    if (g->vis == NULL) 
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Inițializare listele de adiacență și vectorul de vârfuri vizitate
    g->alist = (Node**)malloc(sizeof(Node *) * v);
    if (g->alist == NULL) 
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < v; i++) 
    {
        g->alist[i] = NULL;
        g->vis[i] = 0;
    }

    return g;
}

// Funcție pentru crearea unei stive noi cu o capacitate dată
Stack *create_stack(int capacity) 
{
    // Alocare memorie pentru structura stivă
    Stack *s = (Stack*) malloc(sizeof(Stack));
    if (s == NULL) 
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Alocare memorie pentru array-ul stivei și inițializarea indicatorului vârfului și capacitatea stivei
    s->array = (int *) malloc(capacity * sizeof(int));
    if (s->array == NULL) 
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    s->top = (int) -1;
    s->capacity = capacity;

    return s;
}

// Funcție pentru adăugarea unui element în vârful stivei
void push(int element, Stack *s) 
{
    // Incrementarea indicatorului vârfului și adăugarea elementului în array-ul stivei
    s->top++;
    s->array[s->top] = element;
}
// Funcție pentru parcurgerea în adâncime a grafului, începând de la un vârf dat
void DFS(Graph *g, Stack *s, int v_nr) 
{
    Node *adj_list = g->alist[v_nr]; // Lista de adiacență a vârfului dat
    Node *aux = adj_list; // Nod auxiliar pentru parcurgerea listei de adiacență
    g->vis[v_nr] = 1; // Marchează vârful ca fiind vizitat
    printf("%d ", v_nr); // Afisează vârful curent
    push(v_nr, s); // Adaugă vârful în stivă

    // Parcurgere a listei de adiacență și apel recursiv pentru vârfurile nevizitate
    while (aux != NULL) {
        int con_ver = aux->data; // Vârful adiacent curent
        if (g->vis[con_ver] == 0) // Dacă vârful adiacent nu a fost vizitat
            DFS(g, s, con_ver); // Se apelează DFS pentru acest vârf
        aux = aux->next; // Se trece la următorul vârf adiacent
    }
}

// Funcție pentru inserarea unei liste de muchii în graf
void insert_edges(Graph *g, int edg_nr, int nrv) 
{
    int src, dest;
    printf("Adauga %d muchii (de la 1 la %d):\n", edg_nr, nrv);
    for (int i = 0; i < edg_nr; i++) 
    {
        scanf("%d%d", &src, &dest); // Citirea perechii de vârfuri pentru muchie
        add_edge(g, src, dest); // Adăugarea muchiei în graf
    }
}

// Funcție pentru resetarea vectorului de vârfuri vizitate într-un graf
void reset_visitation(Graph *g, int nrv) 
{
    for (int i = 0; i < nrv; i++) 
    {
        g->vis[i] = 0; // Se resetează vizitarea fiecărui vârf
    }
}

// Funcție pentru a verifica dacă există un drum între două stive de vârfuri
void can_be_reached(Graph *g, int nrv, Stack *s1, Stack *s2) 
{
    int ans = 0; // Răspunsul: 0 - nu poate fi ajuns, 1 - poate fi ajuns
    int *canbe =(int *) calloc(nrv, sizeof(int)); // Alocare dinamică pentru vectorul de rezultate

    // Parcurgere a ambelor stive și apelul DFS pentru fiecare vârf din stivă
    for (int i = 0; i < nrv; i++) 
    {
        DFS(g, s1, i); // Parcurgere DFS începând de la vârful i în prima stivă
        reset_visitation(g, nrv); // Resetarea vizitării în graf
        DFS(g, s2, i); // Parcurgere DFS începând de la vârful i în a doua stivă
        reset_visitation(g, nrv); // Resetarea vizitării în graf
        for (int j = 0; j < nrv && !ans; j++) 
        {
            // Verificare dacă există o corespondență între vârfurile din cele două stive
            if ((s1->array[i] == j) && (s2->array[j] == i)) 
            {
                canbe[i] = 1; // Marchează că poate fi ajuns
                ans = 1; // Actualizează răspunsul că poate fi ajuns
            }
        }
    }

    // Afisarea rezultatelor
    printf("Rezultatele:\n");
    for (int i = 0; i < nrv; i++) 
    {
        printf("Vârful %d: %s\n", i, (canbe[i] == 1) ? "Poate fi ajuns" : "Nu poate fi ajuns");
    }

    free(canbe); // Eliberarea memoriei alocate pentru vectorul de rezultate
}
int main() 
{
    int nrv; // Numărul de vârfuri din graf
    int edg_nr; // Numărul de muchii din graf
    int src, dest; // Variabile pentru citirea vârfurilor unei muchii
    int vortex_1; // Primul vârf pentru verificarea drumului între stive
    int vortex_2; // Al doilea vârf pentru verificarea drumului între stive
    int ans; // Rezultatul verificării dacă există drum între stive

    printf("Câte noduri are graful? ");
    scanf("%d", &nrv); // Citirea numărului de noduri din graf

    printf("Câte muchii are graful? ");
    scanf("%d", &edg_nr); // Citirea numărului de muchii din graf

    Graph *g = create_graph(nrv); // Crearea grafului cu numărul de noduri dat

    Stack *s1 = create_stack(2 * nrv); // Crearea primei stive
    Stack *s2 = create_stack(2 * nrv); // Crearea celei de-a doua stive

    insert_edges(g, edg_nr, nrv); // Inserarea muchiilor în graf

    printf("Introduceti doua noduri pentru a verifica daca exista un drum intre ele:\n");
    scanf("%d%d", &vortex_1, &vortex_2); // Citirea vârfurilor pentru verificarea drumului între stive

    can_be_reached(g, nrv, s1, s2); // Verificarea dacă există drum între cele două stive de vârfuri

    return 0;
}
