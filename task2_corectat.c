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
typedef struct Stiva2
{
    Node *list;
    struct Stiva2 *next;
} Stiva2;
int isEmpty(Stiva2 *top)
{
    return top == NULL;
}
void push2(Stiva2 **top, Node *list)
{
    Stiva2 *newNode = (Stiva2 *)malloc(sizeof(Stiva2));
    newNode->list = list;
    newNode->next = *top;
    *top = newNode;
}
void popStack2(Stiva2 **top)
{
    if (isEmpty(*top))
        return;
    Stiva2 *temp = *top;
    *top = (*top)->next;
    free(temp);
}
void printStack2(Stiva2 *top, FILE *f)
{
    while (top != NULL)
    {
        while (top->list)
            fprintf(f, "(%d,%d); ", top->list->l, top->list->c);
        top = top->next;
    }
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
int is_alive(Stiva2 *top, int l, int c)
{
    int stare = 0; // parametrul stare determina daca celula este in viata sau nu
    Stiva2 *copie = top;
    while (copie)
    {
        Node *p = copie->list;
        while (p)
        {
            if (p->l == l && p->c == c)
            {
                stare = 1 - stare; // starile fiind 1 si 0 se poate schimba starea prin aceasta procedura
                break;
            }
            p = p->next;
        }
        copie = copie->next;
    }
    return stare;
}
int vecini_vii_t2(Stiva2 *top, int l, int c, int n, int m)
{
    int contor = 0;
    for (int j = 0; j < 8; j++)
    {
        int nl = l + dx[j];
        int nc = c + dy[j];
        if (nl >= 0 && nl < n && nc >= 0 && nc < m)
            contor = contor + is_alive(top, nl, nc);
    }
    return contor;
}
Node *generare_lista(Stiva2 *top, int n, int m)
{
    Node *lista = NULL;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
        {
            int stare = is_alive(top, i, j);
            int vecini = vecini_vii_t2(top, i, j, n, m);
            int stare_noua = -1;
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
            if (stare != stare_noua && stare_noua != -1)
            {
                addAtEnd(&lista, i, j);
            }
        }
    return lista;
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
    Stiva2 *top = NULL;
    Node *lista_init = NULL;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (matrice[i][j] == 'X')
                addAtEnd(&lista_init, i, j);
    push2(&top, lista_init);

    for (int i = 0; i < k; i++)
    {
        Node *list = generare_lista(top, n, m);
        push2(&top, list);
    }
    // Inversăm stiva ca să afișăm în ordinea corectă (de la 1 la K)
    Stiva2 *invers = NULL, *current = top;
    while (current != NULL)
    {
        Node *copie = NULL;
        Node *p = current->list;
        while (p != NULL)
        {
            addAtEnd(&copie, p->l, p->c);
            p = p->next;
        }
        push2(&invers, copie);
        current = current->next;
    }

    // Afișăm generațiile 1..K (generația 0 e inițializarea, nu o scriem)
    int generatie = 0;
    current = invers;
    while (current != NULL)
    {
        generatie++;
        if (generatie > 1) // omitem generația 0
        {
            fprintf(f2, "%d", generatie - 1);
            Node *p = current->list;
            while (p)
            {
                fprintf(f2, " %d %d", p->l, p->c);
                p = p->next;
            }
            fprintf(f2, "\n");
        }
        current = current->next;
    }
    fclose(f1);
    fclose(f2);
}