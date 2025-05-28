#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
void push2(Stiva2 **top, Node *list)
{
    Stiva2 *newNode = (Stiva2 *)malloc(sizeof(Stiva2));
    newNode->list = list;
    newNode->next = *top;
    *top = newNode;
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
int date_matrice_noua(Stiva2 *top, char **matrice, int l, int c, int stare)
{
    Stiva2 *cop = top;
    while (cop)
    {
        Node *p = cop->list;
        while (p)
        {
            int dl = p->l;
            int dc = p->c;
            if (dl == l && dc == c)
            {
                stare = 1 - stare;
                break;  // trecem la urmatoarea generatie după ce găsim o modificare
            }
            p = p->next;
        }
        cop = cop->next;
    }
    return stare;
}
void reconstruire_matrice(Stiva2 *top, char **matrice, int n, int m, FILE *f2)
{
    int stare = -1;
    char **matrice_initiala = (char **)malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++)
        matrice_initiala[i] = (char *)malloc(m * sizeof(char));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
        {
            if (matrice[i][j] == 'X')
                stare = 1;
            else
                stare = 0;
            int stare_noua = date_matrice_noua(top, matrice, i, j, stare);
            if (stare_noua == 1)
                matrice_initiala[i][j] = 'X';
            else
                matrice_initiala[i][j] = '+';
        }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            fprintf(f2, "%c", matrice_initiala[i][j]);
        fprintf(f2, "\n");
    }
    for (int i = 0; i < n; i++)
    {
        free(matrice[i]);
        free(matrice_initiala[i]);
    }
    free(matrice);
    free(matrice_initiala);
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
    int n, m, k;
    fscanf(f1, "%d %d", &n, &m);
    fscanf(f1, "%d", &k);
    Stiva2 *top = NULL;
    char **matrice = (char **)malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++)
        matrice[i] = (char *)malloc(m * sizeof(char));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
        {
            fscanf(f1, " %c", &matrice[i][j]);
        }

  for (int i = 0; i < k; i++)
    {
        int nr;
        fscanf(f1, "%d", &nr);
        Node *lista = NULL;
        //vom lua 4 perechi pe fiecare schimbare
        for (int j = 0; j < 4; j++)
        {
            int l, c;
            fscanf(f1, "%d %d", &l, &c);
            addAtEnd(&lista, l, c);
        }
        push2(&top, lista);
    }
    reconstruire_matrice(top, matrice, n, m, f2);
    fclose(f1);
    fclose(f2);
    return 0;
}