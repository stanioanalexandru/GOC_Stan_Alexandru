

#include <stdio.h>
#include <stdlib.h>
const int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const int dx[] = {0, -1, -1, -1, 0, 1, 1, 1};
struct Elem
{
    int l; // linia
    int c; // coloana
    struct Elem *next;
};
typedef struct Elem Node;
typedef struct Arbore
{
    Node *list;
    struct Arbore *left;
    struct Arbore *right;
} NodeArbore;
typedef struct Stack
{
    NodeArbore *nod;
    struct Stack *urm;
} Stack;
typedef struct Graph
{
    int v;         // nr varfuri
    int e;         // nr muchii
    int **a;       // matricea de adiacenta
    Node **noduri; // pun aceasta lista pentru a sti exact linia si coloana fiecarui nod din graf corespunzatoare matricei initiale
} Graph;
// voi crea o lista de vectori pentru task 4 pentru a stoca componentele conexe
typedef struct conex
{
    int *noduri;
    int size;
    struct conex *next;
} conex;
// Șterge întreaga stivă și eliberează memoria
void deleteStack(Node **top)
{
    Node *temp;
    while ((*top) != NULL)
    {
        temp = *top;
        *top = (*top)->next;
        free(temp);
    }
}
void push(Node **top, int l, int c)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->l = l;
    newNode->c = c;
    newNode->next = *top;
    *top = newNode;
}
int numar_vecini_vii(char **matrice, int i, int j, int n, int m)
{
    int di, dj;
    // am luat directiile in care se pot afla vecinii prin intermediul a doi vectori
    int nr_celule_vii = 0;
    for (int p = 0; p < 8; p++)
    {
        di = i + dx[p];
        dj = j + dy[p];
        if (di >= 0 && di < n && dj >= 0 && dj < m && matrice[di][dj] == 'X') // verific pentru fiecare vecin daca este "viu"
            nr_celule_vii++;
    }
    return nr_celule_vii;
}
void generare_generatii(char **matrice, int k, int n, int m, FILE *f2)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            fprintf(f2, "%c", matrice[i][j]);
        fprintf(f2, "\n");
    }
    // generatia 0, inaintea afisarii buclei
    fprintf(f2, "\n");
    while (k != 0)
    {
        char **copie = (char **)malloc(n * sizeof(char *));
        for (int i = 0; i < n; i++)
            copie[i] = (char *)malloc(m * sizeof(char));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
            {
                int vecini = numar_vecini_vii(matrice, i, j, n, m);
                if (matrice[i][j] == 'X' && (vecini < 2 || vecini > 3)) // subpopulare si suprapopulare
                    copie[i][j] = '+';
                else if (matrice[i][j] == 'X' && (vecini == 2 || vecini == 3)) // supravietuire
                    copie[i][j] = 'X';
                else if (matrice[i][j] == '+' && vecini == 3) // reproducere
                    copie[i][j] = 'X';
                else
                    copie[i][j] = '+'; // orice alta situatie
            }
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                matrice[i][j] = copie[i][j]; // transpunem copia luata in matricea originala
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
                fprintf(f2, "%c", matrice[i][j]); // afisam matricea in f2
            fprintf(f2, "\n");
        }
        fprintf(f2, "\n");
        for (int i = 0; i < n; i++)
            free(copie[i]);
        free(copie); // eliberam memoria alocata
        k--;
    }
}
/*
void printStack(Node *top, FILE *f)
{
    while (top != NULL)
    {
        fprintf(f, "(%d,%d); ", top->l, top->c);
        top = top->next;
    }
}*/
void printStack_inverted(Node *top, FILE *f)
{
    // Creez o stiva temporara
    Node *reversedStack = NULL;
    while (top != NULL)
    {
        push(&reversedStack, top->l, top->c);
        top = top->next;
    }
    Node *temp = reversedStack;
    // acum se inverseaza cu un while
    while (temp != NULL)
    {
        fprintf(f, " %d %d", temp->l, temp->c);
        temp = temp->next;
    }
    // se sterge stiva
    deleteStack(&reversedStack);
}

void creare_stiva(char **matrice, int k, int n, int m, FILE *f)
{
    int p = 1;
    Node *stack = NULL;
    while (k > 0)
    {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
            {
                int vecini = numar_vecini_vii(matrice, i, j, n, m);
                if (matrice[i][j] == 'X' && (vecini < 2 || vecini > 3)) // subpopulare sau suprapopulare
                {
                    push(&stack, i, j);
                }
                else if (matrice[i][j] == '+' && vecini == 3) // reproducere
                {
                    push(&stack, i, j);
                }
            }
        Node *temp = stack;
        fprintf(f, "%d", p);
        printStack_inverted(stack, f);
        fprintf(f, "\n");
        // se modifica acum punctele specificate
        while (temp)
        {
            int i = temp->l;
            int j = temp->c;
            if (matrice[i][j] == '+')
                matrice[i][j] = 'X';
            else
                matrice[i][j] = '+';
            temp = temp->next;
        }
        // se sterge stiva
        deleteStack(&stack);
        p++;
        k--;
    }
}
// Node** head - inceputul listei se va modifica
// Data v - informatia de stocat
void addAtBeginning(Node **head, int l, int c)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
        printf("Eroare la alocarea memoriei");
    newNode->l = l;
    newNode->c = c;
    newNode->next = *head;
    *head = newNode;
}
void addAtEnd(Node **head, int l, int c)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Eroare la alocarea memoriei");
        return;
    }
    newNode->l = l;
    newNode->c = c;
    newNode->next = NULL;

    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        Node *aux = *head;
        while (aux->next != NULL)
            aux = aux->next;
        aux->next = newNode;
    }
}
int isEmpty(Stack *top)
{
    return top == NULL;
}
NodeArbore *creare_arbore()
{
    NodeArbore *root = (NodeArbore *)malloc(sizeof(NodeArbore));
    if (root == NULL)
    {
        printf("Eroare la alocarea memoriei");
    }
    root->left = root->right = NULL;
    root->list = NULL;
    return root;
}
// in aceasta functie voi pune lista de vii in root
// ce avem la nvelul 0
void conversie_matrice_initiala(NodeArbore **root, char **matrice, int n, int m)
{
    *root = creare_arbore();
    Node *list0 = NULL;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
        {
            if (matrice[i][j] == 'X')
                addAtEnd(&list0, i, j);
        }
    (*root)->list = list0;
}
void pushStack(Stack **top, NodeArbore *nod)
{
    Stack *newnode = (Stack *)malloc(sizeof(Stack));
    if (newnode == NULL)
        printf("Eroare la alocarea memoriei");
    newnode->nod = nod;
    newnode->urm = *top;
    *top = newnode;
}
void popStack(Stack **top)
{
    if (isEmpty(*top))
        return;
    Stack *temp = *top;
    *top = (*top)->urm;
    free(temp);
}
int is_alive(Stack *top, int l, int c)
{
    int stare = 0;
    Stack *copie = top;
    while (copie)
    {
        Node *p = copie->nod->list;
        while (p)
        {
            if (p->l == l && p->c == c)
            {
                stare = 1 - stare; // starile fiind 1 si 0 se poate schimba starea prin aceasta procedura
                break;
            }
            p = p->next;
        }
        copie = copie->urm;
    }
    return stare;
}
int vecini_vii_t3(Stack *top, int l, int c, int n, int m)
{
    int contor = 0;
    for (int j = 0; j < 8; j++)
    {
        int nl = l + dx[j];
        int nc = c + dy[j];
        if (nl >= 0 && nl < n && nc >= 0 && nc < m)
            contor = contor + is_alive(top, nl, nc); // daca starea este 0, ramane la fel si 1 daca se gaseste un vecin
    }
    return contor;
}
Node *generare_lista(Stack *top, int n, int m, int regula)
{
    // regula reprezinta modul in care va fi impartita actiunea viitoare. 1 pentru regulile noi ale jocului si 0 pentru cele standard
    Node *lista = NULL;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
        {
            int stare = is_alive(top, i, j);
            int vecini = vecini_vii_t3(top, i, j, n, m);
            int stare_noua = -1;
            if (regula == 1)
            {
                // regula 1; jocul are reguli diferite
                if (vecini == 2 && stare == 0)
                    stare_noua = 1;
            }
            else
            {
                // pentru regula originala
                if (stare == 1)
                {
                    if (vecini == 2 || vecini == 3)
                        stare_noua = 1;
                    else
                        stare_noua = 0;
                }
                else
                {
                    if (vecini == 3)
                        stare_noua = 1;
                    else
                        stare_noua = 0;
                }
            }
            if (stare != stare_noua && stare_noua != -1)
            {
                addAtEnd(&lista, i, j);
            }
        }
    return lista;
}
void creare_stiva_arbore(NodeArbore *root, Stack **top, int k, int n, int m)
{
    if (k == 0)
        return;
    pushStack(top, root);
    NodeArbore *left_path = creare_arbore();
    left_path->list = generare_lista(*top, n, m, 1);
    root->left = left_path;
    creare_stiva_arbore(left_path, top, k - 1, n, m);
    NodeArbore *right_path = creare_arbore();
    right_path->list = generare_lista(*top, n, m, 0);
    root->right = right_path;
    creare_stiva_arbore(right_path, top, k - 1, n, m);
    popStack(top);
}
void constructie(FILE *f2, int n, int m, Stack *top)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (is_alive(top, i, j))
            {
                fprintf(f2, "%c", 'X');
            }
            else
                fprintf(f2, "%c", '+');
        }
        fprintf(f2, "\n");
    }
    fprintf(f2, "\n");
}
void preorder(NodeArbore *root, FILE *f2, Stack **top, int n, int m)
{
    if (root == NULL)
        return;
    pushStack(top, root); // push real
    constructie(f2, n, m, *top);
    preorder(root->left, f2, top, n, m);
    preorder(root->right, f2, top, n, m);
    popStack(top);
}
void freeList(Node *head)
{
    while (head)
    {
        Node *tmp = head;
        head = head->next;
        free(tmp);
    }
}
void freeTree(NodeArbore *root)
{
    if (!root)
        return;
    freeList(root->list);
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void freeStack(Stack *top)
{
    while (top)
    {
        Stack *tmp = top;
        top = top->urm;
        free(tmp);
    }
}
void constructie_t4(int n, int m, Stack *top, char ***matrix)
{
    *matrix = (char **)malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++)
        (*matrix)[i] = (char *)malloc(m * sizeof(char));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (is_alive(top, i, j))
                (*matrix)[i][j] = 'X';
            else
                (*matrix)[i][j] = '+';
        }
    }
}
int nr_cel_vii(char **matrix, int n, int m)
{
    int nr = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (matrix[i][j] == 'X')
                nr++;
    return nr;
}
int **construire_matrice_adiacenta(Graph *g, char **matrix, int n, int m, Node ***noduri_out)
{
    Node *celule = NULL;
    int **mat = (int **)malloc(g->v * sizeof(int *));
    for (int i = 0; i < g->v; i++)
        mat[i] = (int *)malloc(g->v * sizeof(int));
    // punem toate celulele vii intr o lista, pe care o vom stoca dupa aceea intr un vector
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
        {
            if (matrix[i][j] == 'X')
            {
                addAtEnd(&celule, i, j);
            }
        }
    Node *copie = celule;
    Node **noduri = (Node **)malloc(g->v * sizeof(Node *));
    int k = 0;
    while (copie && k < g->v)
    {
        noduri[k] = (Node *)malloc(sizeof(Node));
        noduri[k]->l = copie->l;
        noduri[k]->c = copie->c;
        k++;
        copie = copie->next;
    }
    // am adaugat coordonatele vii ale matricei in lista, careia i am asociat un vector noduri
    // initializam numarul de muchii ale grafului cu 0
    g->e = 0;
    // initializez matricea adiacenta cu 0
    for (int i = 0; i < g->v; i++)
        for (int j = 0; j < g->v; j++)
            mat[i][j] = 0;
    // aflam daca exista vecini pentru noduri; pentru vecin punem 1 in matrice
    for (int i = 0; i < g->v; i++)
    {
        for (int d = 0; d < 8; d++)
        {
            int nl = noduri[i]->l + dx[d];
            int nc = noduri[i]->c + dy[d];
            for (int j = 0; j < g->v; j++)
                if (i != j && noduri[j]->l == nl && noduri[j]->c == nc)
                {
                    mat[i][j] = 1;
                    g->e++;
                    break;
                }
        }
    }
    *noduri_out = noduri;
    freeList(celule);
    return mat;
}
void DFS_scan(Graph *g, int visited[], int i, conex *list)
{
    int j;
    visited[i] = 1;
    list->noduri[list->size] = i;
    list->size++;
    for (j = 0; j < g->v; j++)
        if (g->a[i][j] == 1 && visited[j] == 0)
        {
            DFS_scan(g, visited, j, list);
        }
}

conex *DFS(Graph *g)
{
    int i;
    conex *list = NULL;
    conex *last = NULL;
    int *visited = (int *)malloc(g->v * sizeof(int));
    for (i = 0; i < g->v; i++)
        visited[i] = 0;
    for (i = 0; i < g->v; i++)
        if (visited[i] == 0)
        {
            conex *c = (conex *)malloc(sizeof(conex));
            c->size = 0;
            c->noduri = (int *)malloc(g->v * sizeof(int)); // cel mai nefavorabil caz cand sunt toti vecini
            c->next = NULL;
            DFS_scan(g, visited, i, c);
            if (list == NULL)
            {
                list = last = c;
            }
            else
            {
                last->next = c;
                last = c;
            }
        }
    free(visited);
    return list;
    // acum avem obtinuta o lista, unde fiecare element din ea este un vector cu nodurile ce formeaza o componenta conexa
}
void construire_graf(Graph *g, char **matrix, int n, int m, Node ***noduri_out)
{
    // avem matricea pe care urmreza sa prelucram graful - **matrix cu n linii si m coloane
    int v = nr_cel_vii(matrix, n, m);
    g->v = v;
    int **mat = construire_matrice_adiacenta(g, matrix, n, m, noduri_out);
    // acum avem matricea de adiacenta
    g->a = mat;
    g->noduri = *noduri_out;
}
void sort_vector(Graph *g, int *v, int k)
{
    for (int i = 0; i < k - 1; i++)
        for (int j = i + 1; j < k; j++)
        {
            Node *ni = g->noduri[v[i]];
            Node *nj = g->noduri[v[j]];
            if (ni->l > nj->l || (ni->l == nj->l && ni->c > nj->c))
            {
                int aux = v[i];
                v[i] = v[j];
                v[j] = aux;
            }
        }
}
int lexicografic(Graph *g, int *a, int *b, int length)
{
    for (int i = 0; i <= length; i++)
    {
        int la = g->noduri[a[i]]->l;
        int ca = g->noduri[a[i]]->c;
        int lb = g->noduri[b[i]]->l;
        int cb = g->noduri[b[i]]->c;

        if (la < lb)
            return 1;
        if (la > lb)
            return 0;
        if (ca < cb)
            return 1;
        if (ca > cb)
            return 0;
    }
    return 0; // sunt egale
}

// urmatoarea functie cauta cel mai lung lant hamiltonian intr o componenta conexa
void lant_hamiltonian(Graph *g, int *best_path, int *comp, int size, int *path, int *visited, int length, int *max_length)
{
    // daca se gaseste o cale mai buna se modifica
    if (length > *max_length)
    {
        *max_length = length;
        for (int i = 0; i <= length; i++)
            best_path[i] = path[i];
    }
    int last = path[length];
    int ok = 0;
    for (int i = 0; i < size; i++)
    {
        int next = comp[i];
        if (visited[next] == 0 && g->a[last][next])
        {
            ok = 1;
            visited[next] = 1;
            path[length + 1] = next;
            lant_hamiltonian(g, best_path, comp, size, path, visited, length + 1, max_length);
            visited[next] = 0;

            //se iese daca s au folosit toate varfurile
            if (*max_length >= size - 1)
                return;
        }
    }

    // daca nu s au gasit varfuri
    if (ok == 0 && length > *max_length)
    {
        *max_length = length;
        for (int i = 0; i <= length; i++)
            best_path[i] = path[i];
    }
}

void construire_lant(Graph *g, FILE *f2)
{
    conex *list = DFS(g);
    int *path = (int *)malloc(g->v * sizeof(int));
    int *visited = (int *)malloc(g->v * sizeof(int));
    int *best_path = (int *)malloc(g->v * sizeof(int));
    int best_length = -1;
    int *best_final_path = NULL;

    // se iau toate componentele
    conex *current = list;
    while (current)
    {
        // Se iau in considerare doar daca au un drum mai mare ca best_length(pentru memorie)
        if (current->size > best_length + 1)
        {
            sort_vector(g, current->noduri, current->size);
            for (int i = 0; i < current->size; i++)
            {
                // se reseteaza vizitatele
                for (int j = 0; j < g->v; j++)
                    visited[j] = 0;

                path[0] = current->noduri[i];
                visited[path[0]] = 1;
                int current_max_length = -1;

                lant_hamiltonian(g, best_path, current->noduri, current->size, path, visited, 0, &current_max_length);

                if (current_max_length > best_length ||
                    (current_max_length == best_length && lexicografic(g, best_path, best_final_path, best_length)))

                {
                    best_length = current_max_length;
                    if (best_final_path == NULL)
                        best_final_path = (int *)malloc((best_length + 1) * sizeof(int));
                    else
                        best_final_path = (int *)realloc(best_final_path, (best_length + 1) * sizeof(int));
                    for (int j = 0; j <= best_length; j++)
                        best_final_path[j] = best_path[j];
                }

                //se iese daca s au luat in considerare toate varfurile
                if (best_length >= current->size - 1)
                    break;
            }
        }
        current = current->next;
    }
    if (best_length == -1)
        fprintf(f2, "-1\n");
    else
    {
        fprintf(f2, "%d\n", best_length);
        for (int i = 0; i <= best_length; i++)
        {
            int idx = best_final_path[i];
            if (i < best_length)
                fprintf(f2, "(%d,%d) ", g->noduri[idx]->l, g->noduri[idx]->c);
            else
                fprintf(f2, "(%d,%d)", g->noduri[idx]->l, g->noduri[idx]->c); // fara spatiu
        }
        fprintf(f2, "\n");
    }

    //se da free la memorie
    free(path);
    free(visited);
    free(best_path);
    if (best_final_path)
        free(best_final_path);
    while (list)
    {
        conex *temp = list;
        list = list->next;
        free(temp->noduri);
        free(temp);
    }
}
void preorder_t4(NodeArbore *root, FILE *f2, Stack **top, int n, int m)
{
    if (root == NULL)
        return;

    pushStack(top, root);

    char **matrix;
    constructie_t4(n, m, *top, &matrix);

    Graph *g = (Graph *)malloc(sizeof(Graph));
    Node **noduri;
    construire_graf(g, matrix, n, m, &noduri);

    construire_lant(g, f2);

    // eliberari de memorie
    for (int i = 0; i < n; i++)
        free(matrix[i]);
    free(matrix);

    for (int i = 0; i < g->v; i++)
        free(noduri[i]);
    free(noduri);

    for (int i = 0; i < g->v; i++)
        free(g->a[i]);
    free(g->a);
    free(g);

    preorder_t4(root->left, f2, top, n, m);
    preorder_t4(root->right, f2, top, n, m);

    popStack(top);
}

int main(int argc, char *argv[])
{
    FILE *f1 = fopen(argv[1], "r");
    FILE *f2 = fopen(argv[2], "w");
    // am deschis fisierele de input si output: f1-pentru citire si f2-pentru scriere
    if (f1 == NULL)
    {
        printf("Eroare la deschiderea fisierului de input\n");
        return 1;
    }
    if (f2 == NULL)
    {
        printf("Eroare la deschiderea fisierului de output\n");
        fclose(f1);
        return 1;
    }
    int t;    // nr testului
    int n, m; // nr de linii/coloane ale matricei
    int k;    // nr de generatii ce trebuie calculate
    fscanf(f1, "%d", &t);
    fscanf(f1, "%d", &n);
    fscanf(f1, "%d", &m);
    fscanf(f1, "%d", &k);
    int k_original = k;
    char **matrice = (char **)malloc(n * sizeof(char *));
    if (matrice == NULL)
    {
        printf("Eroare la alocarea matricei");
        return 1;
    }
    for (int i = 0; i < n; i++)
    {
        matrice[i] = (char *)malloc(m * sizeof(char));
        if (matrice[i] == NULL)
        {
            printf("Eroare la alocarea matricei");
            return 1;
        }
    }
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
        {
            fscanf(f1, " %c", &matrice[i][j]);
        }
    // am citit din fisier informatiile necesare
    switch (t) // se alege ce task sa se efectueze
    {
    case 1:
        generare_generatii(matrice, k, n, m, f2);
        break;
    case 2:
        creare_stiva(matrice, k_original, n, m, f2);
        break;
    case 3:
    {
        NodeArbore *root = NULL;
        conversie_matrice_initiala(&root, matrice, n, m);
        Stack *top = NULL;
        creare_stiva_arbore(root, &top, k, n, m);
        preorder(root, f2, &top, n, m);
        freeTree(root);
        freeStack(top);
        free(root);
        free(top);
        break;
    }
    case 4:
    {
        NodeArbore *root = NULL;
        conversie_matrice_initiala(&root, matrice, n, m);
        Stack *top = NULL;
        creare_stiva_arbore(root, &top, k, n, m);
        preorder_t4(root, f2, &top, n, m);
    }
    }
    for (int i = 0; i < n; i++)
        free(matrice[i]);
    free(matrice);
    fclose(f1);
    fclose(f2);
    return 0;
}