/**
 * Implementarea unui Arbore Binar de Căutare în C
 * Lucrare de laborator nr. 4 - Structuri de Date și Algoritmi
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

/* Constante pentru dimensiunea maximă a șirurilor de caractere */
#define MAX_TITLE_LENGTH 128  // Lungimea maximă pentru titlul cărții
#define MAX_AUTHOR_LENGTH 128 // Lungimea maximă pentru numele autorului

/* Constante pentru marcarea cazurilor când nu s-a găsit un rezultat */
#define NOT_FOUND_DEPTH -1    // Valoare returnată când nu se poate calcula adâncimea
#define NOT_FOUND_HEIGHT -1   // Valoare returnată când nu se poate calcula înălțimea

/**
 * Structură pentru stocarea informațiilor despre o carte
 * Aceasta este structura de date principală stocată în nodurile arborelui
 */
typedef struct {
    int key;                            // Cheia după care se organizează arborele
    char title[MAX_TITLE_LENGTH];       // Titlul cărții
    char author[MAX_AUTHOR_LENGTH];     // Autorul cărții
    int pub_year;                       // Anul publicării
    int page_count;                     // Numărul de pagini
    int quantity_sold;                  // Tirajul (numărul de exemplare vândute)
} Book;

/**
 * Structură pentru un nod din arborele binar
 * Conține o referință la o carte și pointeri la copiii din stânga și dreapta
 */
typedef struct BinaryTreeNode {
    Book * book;                       // Referință la obiectul carte stocat în nod
    struct BinaryTreeNode * left;      // Pointer la copilul stâng
    struct BinaryTreeNode * right;     // Pointer la copilul drept
} BinaryTreeNode;

/**
 * Structură pentru arborele binar
 * Conține un pointer la rădăcina arborelui
 */
typedef struct BinaryTree {
    BinaryTreeNode * root;             // Pointer la rădăcina arborelui
} BinaryTree;

/**
 * Structură pentru un nod din coadă
 * Folosită pentru implementarea cozii utilizate în parcurgerea BFS
 */
typedef struct QueueNode {
    BinaryTreeNode * tree_node;        // Referință la nodul din arbore
    struct QueueNode * next;           // Pointer la următorul nod din coadă
} QueueNode;

/**
 * Structură pentru coadă
 * Folosită în implementarea parcurgerii BFS și a afișării arborelui pe nivele
 */
typedef struct Queue {
    QueueNode * head;                  // Pointer la primul element din coadă
    int size;                          // Dimensiunea cozii
} Queue;

/**
 * Creează un nou nod de coadă
 * @param tree_node Nodul din arbore care va fi stocat în noul nod de coadă
 * @return Pointer la noul nod de coadă creat
 */
QueueNode * create_queue_node(BinaryTreeNode * tree_node) {
    QueueNode * queue_node = (QueueNode *)malloc(sizeof(QueueNode));
    queue_node->tree_node = tree_node;
    queue_node->next = NULL;
    return queue_node;
}

/**
 * Creează o nouă coadă goală
 * @return Pointer la noua coadă creată
 */
Queue * create_queue() {
    Queue * queue = (Queue *)malloc(sizeof(Queue));
    queue->head = NULL;
    queue->size = 0;
    return queue;
}

/**
 * Adaugă un nod din arbore la sfârșitul cozii
 * @param queue Coada în care se va adăuga nodul
 * @param tree_node Nodul din arbore care va fi adăugat în coadă
 */
void enqueue(Queue * queue, BinaryTreeNode * tree_node) {
    QueueNode * new_node = create_queue_node(tree_node);
    QueueNode * head = queue->head;

    queue->size++;  // Incrementăm dimensiunea cozii

    // Cazul special: coada este goală
    if (head == NULL) {
        queue->head = new_node;
        return;
    }

    // Parcurgem coada până la ultimul element
    while (head->next != NULL) {
        head = head->next;
    }

    // Adăugăm noul nod la sfârșitul cozii
    head->next = new_node;
}

/**
 * Elimină și returnează primul nod din coadă
 * @param queue Coada din care se va elimina nodul
 * @return Nodul din arbore care era primul în coadă
 */
BinaryTreeNode * dequeue(Queue * queue) {
    QueueNode * head = queue->head;

    queue->size--;  // Decrementăm dimensiunea cozii

    // Cazul special: coada este goală
    if (head == NULL) {
        return NULL;
    }

    // Actualizăm capul cozii
    queue->head = head->next;

    // Extragem nodul din arbore și eliberăm memoria nodului de coadă
    BinaryTreeNode * tree_node = head->tree_node;
    free(head);

    return tree_node;
}

/**
 * Verifică dacă coada este goală
 * @param queue Coada care trebuie verificată
 * @return true dacă coada este goală, false în caz contrar
 * Notă: Există o greșeală de scriere în numele funcției (eueue în loc de queue)
 */
bool is_eueue_empty(Queue * queue) {
    return queue->head == NULL && queue->size == 0;
}

/**
 * Afișează titlurile cărților din nodurile stocate în coadă
 * @param queue Coada care trebuie afișată
 */
void display_queue(Queue * queue) {
    QueueNode * head = queue->head;

    while (head != NULL) {
        printf("%s ", head->tree_node->book->title);
        head = head->next;
    }
}

/**
 * Creează un nou arbore binar gol
 * @return Pointer la noul arbore creat
 */
BinaryTree * create_tree() {
    BinaryTree * tree = (BinaryTree *)malloc(sizeof(BinaryTree));
    tree->root = NULL;
    return tree;
}

/**
 * Creează un nou nod pentru arborele binar
 * @param book Cartea care va fi stocată în noul nod
 * @return Pointer la noul nod creat
 */
BinaryTreeNode * create_tree_node(Book * book) {
    BinaryTreeNode * node = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode));
    node->book = book;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/**
 * Creează o nouă carte cu valorile specificate
 * @param key Cheia cărții
 * @param title Titlul cărții
 * @param author Autorul cărții
 * @param pub_year Anul publicării
 * @param page_count Numărul de pagini
 * @param quantity_sold Tirajul (numărul de exemplare vândute)
 * @return Pointer la noua carte creată
 */
Book * create_book(int key, char * title, char * author, int pub_year, int page_count, int quantity_sold) {
    Book * book = (Book *)malloc(sizeof(Book));
    book->key = key;
    strcpy(book->title, title);
    strcpy(book->author, author);
    book->pub_year = pub_year;
    book->page_count = page_count;
    book->quantity_sold = quantity_sold;
    return book;
}

/**
 * Inserează o carte în arborele binar de căutare
 * @param tree Arborele în care se va insera cartea
 * @param book Cartea care va fi inserată
 */
void insert(BinaryTree * tree, Book * book) {
    BinaryTreeNode * new_node = create_tree_node(book);
    BinaryTreeNode * root = tree->root;

    // Cazul special: arborele este gol
    if (!root) {
        tree->root = new_node;
        return;
    }

    // Parcurgem arborele pentru a găsi poziția corectă de inserare
    while (true) {
        if (root->book->key > book->key) {
            // Mergem în stânga dacă cheia este mai mică
            if (!root->left) {
                root->left = new_node;
                return;
            } else {
                root = root->left;
            }
        } else {
            // Mergem în dreapta dacă cheia este mai mare sau egală
            if (!root->right) {
                root->right = new_node;
                return;
            } else {
                root = root->right;
            }
        }
    }
}

/**
 * Calculează adâncimea (numărul de nivele) a arborelui
 * @param tree Arborele pentru care se calculează adâncimea
 * @return Adâncimea arborelui sau NOT_FOUND_DEPTH dacă arborele este gol
 */
int get_tree_depth(BinaryTree * tree) {
    BinaryTreeNode * root = tree->root;

    if (!root) return 0;  // Arborele este gol

    Queue * queue = create_queue();
    enqueue(queue, root);

    int depth = NOT_FOUND_DEPTH;  // Inițializăm cu -1 pentru a începe de la 0

    // Parcurgem arborele nivel cu nivel
    while (!is_eueue_empty(queue)) {
        int level_nodes_count = queue->size;

        // Procesăm toate nodurile de la nivelul curent
        for (int i = 0; i < level_nodes_count; i++) {
            BinaryTreeNode * current_tree_node = dequeue(queue);

            // Adăugăm copiii nodului curent în coadă pentru următorul nivel
            if (current_tree_node->left) enqueue(queue, current_tree_node->left);
            if (current_tree_node->right) enqueue(queue, current_tree_node->right);
        }

        depth++;  // Incrementăm adâncimea după procesarea fiecărui nivel
    }

    free(queue);
    return depth;
}

/**
 * Afișează structura arborelui pe nivele
 * @param tree Arborele care trebuie afișat
 */
void display_tree(BinaryTree * tree) {
    printf("\n");

    BinaryTreeNode * root = tree->root;

    if (!root) {
        printf("Arborele este vid.\n");
        return;
    }

    Queue * queue = create_queue();
    enqueue(queue, root);

    int depth = 0;

    // Parcurgem arborele nivel cu nivel
    while (!is_eueue_empty(queue)) {
        int level_nodes_count = queue->size;

        printf("Level %d: ", depth);

        // Procesăm toate nodurile de la nivelul curent
        for (int i = 0; i < level_nodes_count; i++) {
            BinaryTreeNode * current_tree_node = dequeue(queue);
            Book * book = current_tree_node->book;

            printf("{%d} ", book->key);  // Afișăm cheia nodului

            // Adăugăm copiii nodului curent în coadă pentru următorul nivel
            if (current_tree_node->left) enqueue(queue, current_tree_node->left);
            if (current_tree_node->right) enqueue(queue, current_tree_node->right);
        }

        printf("\n");
        depth++;  // Trecem la următorul nivel
    }

    free(queue);
    printf("\n");
}

/**
 * Caută un nod în arbore după cheia specificată
 * @param tree Arborele în care se caută
 * @param key Cheia nodului căutat
 * @return Pointer la nodul găsit sau NULL dacă nu există
 */
BinaryTreeNode * get(BinaryTree * tree, int key) {
    BinaryTreeNode * root = tree->root;

    if (!root) return NULL;  // Arborele este gol

    // Parcurgem arborele pentru a găsi nodul cu cheia specificată
    while (true) {
        if (root->book->key == key) {
            return root;  // Am găsit nodul
        }

        if (root->book->key > key) {
            // Mergem în stânga dacă cheia căutată este mai mică
            if (!root->left) return NULL;  // Nu există nod cu cheia specificată
            root = root->left;
        }

        if (root->book->key < key) {
            // Mergem în dreapta dacă cheia căutată este mai mare
            if (!root->right) return NULL;  // Nu există nod cu cheia specificată
            root = root->right;
        }
    }
}

/**
 * Parcurge arborele în preordine (Vârf-Stânga-Dreapta)
 * @param tree_node Nodul curent (începând cu rădăcina)
 */
void VSD(BinaryTreeNode * tree_node) {
    printf("%d ", tree_node->book->key);  // Procesăm nodul curent
    if (tree_node->left) VSD(tree_node->left);  // Procesăm subarborele stâng
    if (tree_node->right) VSD(tree_node->right);  // Procesăm subarborele drept
}

/**
 * Inițiază parcurgerea arborelui în preordine
 * @param tree Arborele care trebuie parcurs
 */
void VSD_trasversal(BinaryTree * tree) {
    if (tree->root) {
        printf("VSD: ");
        VSD(tree->root);
    }
}

/**
 * Parcurge arborele în inordine (Stânga-Vârf-Dreapta)
 * @param tree_node Nodul curent (începând cu rădăcina)
 */
void SVD(BinaryTreeNode * tree_node) {
    if (tree_node->left) SVD(tree_node->left);  // Procesăm subarborele stâng
    printf("%d ", tree_node->book->key);  // Procesăm nodul curent
    if (tree_node->right) SVD(tree_node->right);  // Procesăm subarborele drept
}

/**
 * Inițiază parcurgerea arborelui în inordine
 * @param tree Arborele care trebuie parcurs
 */
void SVD_trasversal(BinaryTree * tree) {
    if (tree->root) {
        printf("SVD: ");
        SVD(tree->root);
    }
}

/**
 * Parcurge arborele în postordine (Stânga-Dreapta-Vârf)
 * @param tree_node Nodul curent (începând cu rădăcina)
 */
void SDV(BinaryTreeNode * tree_node) {
    if (tree_node->left) SDV(tree_node->left);  // Procesăm subarborele stâng
    if (tree_node->right) SDV(tree_node->right);  // Procesăm subarborele drept
    printf("%d ", tree_node->book->key);  // Procesăm nodul curent
}

/**
 * Inițiază parcurgerea arborelui în postordine
 * @param tree Arborele care trebuie parcurs
 */
void SDV_trasversal(BinaryTree * tree) {
    if (tree->root) {
        printf("SDV: ");
        SDV(tree->root);
    }
}

/**
 * Parcurge arborele în adâncime (DFS)
 * Notă: În acest cod, DFS este implementat ca parcurgere în preordine (VSD)
 * @param tree Arborele care trebuie parcurs
 */
void DFS(BinaryTree * tree) {
    if (tree->root) {
        printf("DFS: ");
        VSD(tree->root);  // DFS este implementat ca VSD
    }
}

/**
 * Parcurge arborele în lățime (BFS)
 * @param tree Arborele care trebuie parcurs
 */
void BFS(BinaryTree * tree) {
    BinaryTreeNode * root = tree->root;

    if (!root) return;  // Arborele este gol

    Queue * queue = create_queue();
    enqueue(queue, root);

    printf("BFS: ");

    // Parcurgem arborele nivel cu nivel
    while (!is_eueue_empty(queue)) {
        BinaryTreeNode * current_tree_node = dequeue(queue);

        printf("%d ", current_tree_node->book->key);  // Procesăm nodul curent

        // Adăugăm copiii nodului curent în coadă
        if (current_tree_node->left) enqueue(queue, current_tree_node->left);
        if (current_tree_node->right) enqueue(queue, current_tree_node->right);
    }

    free(queue);
}

/**
 * Populează arborele cu date de test
 * @param tree Arborele care trebuie populat
 */
void mock_populate_tree(BinaryTree * tree) {
    // Inserăm câteva cărți în arbore pentru testare
    insert(tree, create_book(28, "Mara", "Ion Slavici", 1998, 450, 60000));
    insert(tree, create_book(12, "Baltagul", "Mihail Sadoveanu", 1930, 300, 45000));
    insert(tree, create_book(45, "Enigma Otiliei", "George Călinescu", 1940, 400, 70000));
    insert(tree, create_book(33, "Morometii", "Marin Preda", 1955, 500, 80000));
    insert(tree, create_book(18, "Ion", "Liviu Rebreanu", 1920, 600, 75000));
    insert(tree, create_book(39, "Pădurea Spânzuraților", "Liviu Rebreanu", 1922, 550, 67000));
    insert(tree, create_book(24, "Ultima noapte de dragoste", "Camil Petrescu", 1930, 320, 62000));
    insert(tree, create_book(30, "Adela", "Garabet Ibrăileanu", 1933, 280, 46000));
    insert(tree, create_book(20, "Scrinul Negru", "George Călinescu", 1960, 500, 43000));
    insert(tree, create_book(15, "Ciuleandra", "Liviu Rebreanu", 1927, 270, 41000));
    insert(tree, create_book(37, "O scrisoare pierduta", "I.L. Caragiale", 1884, 150, 90000));
    insert(tree, create_book(50, "Amintiri din copilărie", "Ion Creanga", 1881, 200, 120000));
    insert(tree, create_book(10, "Moara cu noroc", "Ioan Slavici", 1881, 180, 38000));
    insert(tree, create_book(42, "Două loturi", "I.L. Caragiale", 1890, 100, 54000));
}

/**
 * Returnează maximul dintre două numere
 * @param a Primul număr
 * @param b Al doilea număr
 * @return Maximul dintre a și b
 */
int max(int a, int b) {
    return (a > b) ? a : b;
}

/**
 * Verifică dacă un nod și subarborii săi sunt balansați și returnează înălțimea nodului
 * Un arbore este balansat dacă diferența de înălțime între subarborii stâng și drept
 * ai fiecărui nod este cel mult 1.
 *
 * Funcția returnează înălțimea nodului dacă nodul și subarborii săi sunt balansați,
 * sau -1 dacă nodul sau oricare dintre subarborii săi nu este balansat.
 *
 * @param tree_node Nodul curent
 * @return Înălțimea nodului sau -1 dacă nodul sau subarborii săi nu sunt balansați
 */
int get_node_balance(BinaryTreeNode * tree_node) {
    if (!tree_node) return 0;  // Nodul este NULL (frunză virtuală)

    // Verificăm dacă subarborele stâng este balansat
    int left_balance = get_node_balance(tree_node->left);
    if (left_balance == -1) return -1;  // Subarborele stâng nu este balansat

    // Verificăm dacă subarborele drept este balansat
    int right_balance = get_node_balance(tree_node->right);
    if (right_balance == -1) return -1;  // Subarborele drept nu este balansat

    // Verificăm dacă nodul curent este balansat
    int diff = abs(left_balance - right_balance);
    if (diff > 1) {
        return -1;  // Nodul curent nu este balansat
    }

    // Nodul și subarborii săi sunt balansați, returnăm înălțimea nodului
    return 1 + max(left_balance, right_balance);
}

/**
 * Verifică dacă arborele este balansat
 * @param tree Arborele care trebuie verificat
 * @return true dacă arborele este balansat, false în caz contrar
 */
bool is_tree_balanced(BinaryTree * tree) {
    BinaryTreeNode * root = tree->root;

    if (!tree->root) return true;  // Un arbore gol este considerat balansat

    return get_node_balance(root) != -1;
}

/**
 * Structură pentru un nod din lista dublu înlănțuită
 * Folosită pentru implementarea balansării arborelui
 */
typedef struct ListNode {
    struct ListNode * next;         // Pointer la următorul nod din listă
    struct ListNode * prev;         // Pointer la nodul anterior din listă
    BinaryTreeNode * tree_node;     // Referință la nodul din arbore
} ListNode;

/**
 * Structură pentru lista dublu înlănțuită
 * Folosită pentru implementarea balansării arborelui
 */
typedef struct List {
    ListNode * head;                // Pointer la primul nod din listă
    ListNode * tail;                // Pointer la ultimul nod din listă
} List;

/**
 * Creează un nou nod pentru lista dublu înlănțuită
 * @param tree_node Nodul din arbore care va fi stocat în noul nod de listă
 * @return Pointer la noul nod de listă creat
 */
ListNode * create_list_node(BinaryTreeNode * tree_node) {
    ListNode * list_node = (ListNode *)malloc(sizeof(ListNode));
    list_node->tree_node = tree_node;
    list_node->next = NULL;
    list_node->prev = NULL;
    return list_node;
}

/**
 * Creează o nouă listă dublu înlănțuită goală
 * @return Pointer la noua listă creată
 */
List * create_list() {
    List * list = (List *)malloc(sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

/**
 * Verifică dacă lista este goală
 * @param list Lista care trebuie verificată
 * @return true dacă lista este goală, false în caz contrar
 */
bool is_list_empty(List * list) {
    return !list->head && !list->tail;
}

/**
 * Adaugă un nod din arbore la sfârșitul listei
 * @param list Lista în care se va adăuga nodul
 * @param tree_node Nodul din arbore care va fi adăugat în listă
 */
void list_push_back(List * list, BinaryTreeNode * tree_node) {
    ListNode * new_node = create_list_node(tree_node);

    // Cazul special: lista este goală
    if (is_list_empty(list)) {
        list->head = new_node;
        list->tail = new_node;
        return;
    }

    // Adăugăm noul nod la sfârșitul listei
    new_node->prev = list->tail;
    list->tail->next = new_node;
    list->tail = new_node;
}

/**
 * Afișează cheile nodurilor din listă
 * @param list Lista care trebuie afișată
 */
void print_list(List * list) {
    ListNode * head = list->head;

    while (head) {
        printf("(%d) ", head->tree_node->book->key);
        head = head->next;
    }
}

/**
 * Populează lista în inordine cu nodurile din arbore
 * Parcurgerea în inordine asigură că nodurile sunt adăugate în listă în ordine crescătoare a cheilor
 * @param list Lista care trebuie populată
 * @param tree_node Nodul curent din arbore (începând cu rădăcina)
 */
void populate_list(List * list, BinaryTreeNode * tree_node) {
    if (!tree_node) return;
    if (tree_node->left) populate_list(list, tree_node->left);  // Parcurgem subarborele stâng
    list_push_back(list, tree_node);  // Adăugăm nodul curent în listă
    if (tree_node->right) populate_list(list, tree_node->right);  // Parcurgem subarborele drept
}

/**
 * Creează o listă cu toate nodurile din arbore, ordonate după cheie
 * @param tree Arborele ale cărui noduri trebuie adăugate în listă
 * @return Lista populată cu nodurile din arbore
 */
List * get_tree_nodes_list(BinaryTree * tree) {
    BinaryTreeNode * root = tree->root;
    List * list = create_list();

    if (!root) return list;  // Arborele este gol

    populate_list(list, root);  // Populăm lista cu nodurile din arbore

    return list;
}

/**
 * Găsește nodul din mijlocul listei
 * @param list Lista în care se caută
 * @return Nodul din mijlocul listei sau NULL dacă lista este goală
 */
ListNode * get_list_center(List * list) {
    if (is_list_empty(list)) return NULL;  // Lista este goală

    // Folosim algoritmul "slow and fast pointers" pentru a găsi mijlocul listei
    ListNode * slow = list->head;
    ListNode * fast = list->head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;  // Nodul din mijlocul listei
}

/**
 * Creează o sublistă din lista principală
 * @param start Primul nod al sublistei
 * @param end Ultimul nod al sublistei
 * @return Sublista creată
 */
List * create_sublist(ListNode * start, ListNode * end) {
    List * sublist = create_list();
    sublist->head = start;
    sublist->tail = end;
    return sublist;
}

/**
 * Construiește un arbore binar de căutare balansat din nodurile listei
 * @param main_list Lista cu nodurile care trebuie incluse în arbore
 * @return Nodul rădăcină al arborelui construit
 */
ListNode * get_balanced_tree_root(List *main_list) {
    ListNode * head = main_list->head;
    ListNode * tail = main_list->tail;

    // Găsim centrul listei pentru a-l folosi ca rădăcină a arborelui
    ListNode * center = get_list_center(main_list);
    center->tree_node->left = NULL;  // Resetăm copiii nodului central
    center->tree_node->right = NULL;

    // Verificăm dacă sublista are doar un element
    if (center == head || center == tail) return head;

    // Creăm sublista stângă (toate nodurile din stânga centrului)
    List * left_list = create_list();
    left_list->head = head;
    left_list->tail = center->prev;
    if (left_list->tail) left_list->tail->next = NULL;
    if (left_list->head) left_list->head->prev = NULL;

    // Creăm sublista dreaptă (toate nodurile din dreapta centrului)
    List * right_list = create_list();
    right_list->head = center->next;
    right_list->tail = tail;
    if (right_list->head) right_list->head->prev = NULL;
    if (right_list->tail) right_list->tail->next = NULL;

    // Construim recursiv subarborele stâng
    ListNode * left = get_balanced_tree_root(left_list);

    // Construim recursiv subarborele drept
    ListNode * right = get_balanced_tree_root(right_list);

    // Conectăm subarborii la nodul central
    center->tree_node->left = left->tree_node;
    center->tree_node->right = right->tree_node;

    //

    free(left_list);
    free(right_list);

    return center;
}
/*
 * Funcția de balansare a arborelui
 * Realizează balansarea unui arbore binar de căutare pentru a optimiza operațiile de căutare
 * Complexitate: O(n) unde n este numărul de noduri
 */
void balance_tree(BinaryTree * tree) {
    // Obține o listă cu toate nodurile arborelui
    List * tree_nodes_list = get_tree_nodes_list(tree);

    // Verificăm dacă arborele este deja balansat
    if (is_tree_balanced(tree)) {
        printf("Arborele deja este balansat.");
        return;
    }

    // Reconstruiește arborele într-o formă balansată
    tree->root = get_balanced_tree_root(tree_nodes_list)->tree_node;
}

/*
 * Secțiunea pentru oglindirea arborelui
 * Aceasta va inversa poziția tuturor nodurilor stânga/dreapta
 * După oglindire, arborele nu mai este arbore de căutare binar (proprietatea BST se pierde)
 */

/*
 * Funcție recursivă pentru oglindirea unui nod și a subarborilor săi
 * Utilizează parcurgerea postordine (SDV) pentru a oglindi mai întâi subarborii
 * Complexitate: O(n) unde n este numărul de noduri
 */
void post_order_mirror(BinaryTreeNode * tree_node) {
    // Verifică și oglindește subarborele stâng
    if (tree_node->left) post_order_mirror(tree_node->left);

    // Verifică și oglindește subarborele drept
    if (tree_node->right) post_order_mirror(tree_node->right);

    // Schimbă pozițiile subarborilor stâng și drept
    BinaryTreeNode * temp = tree_node->left;
    tree_node->left = tree_node->right;
    tree_node->right = temp;
}

/*
 * Funcție pentru oglindirea întregului arbore
 * Apelează funcția recursivă pentru nodul rădăcină
 */
void mirror_tree(BinaryTree * tree) {
    if (tree->root) post_order_mirror(tree->root);
}

/*
 * Funcție pentru citirea datelor unei cărți de la utilizator
 * Implementează interfața pentru introducerea detaliilor cărții
 * Returnează: o structură Book completată cu datele introduse
 */
Book get_book_from_user() {
    Book book_input;

    // Solicită și citește cheia (identificatorul unic)
    printf("Cheia:");
    scanf("%d", &book_input.key);
    getchar(); // Consumă caracterul newline rămas în buffer

    // Solicită și citește titlul cărții
    printf("Titlul:");
    fgets(book_input.title, MAX_TITLE_LENGTH, stdin);
    book_input.title[strcspn(book_input.title, "\n")] = '\0'; // Elimină newline-ul din string

    // Solicită și citește autorul cărții
    printf("Autorul:");
    fgets(book_input.author, MAX_AUTHOR_LENGTH, stdin);
    book_input.author[strcspn(book_input.author, "\n")] = '\0'; // Elimină newline-ul din string

    // Solicită și citește anul publicării
    printf("Anul publicarii:");
    scanf("%d", &book_input.pub_year);
    getchar(); // Consumă caracterul newline rămas în buffer

    // Solicită și citește numărul de pagini
    printf("Numarul de pagini:");
    scanf("%d", &book_input.page_count);
    getchar(); // Consumă caracterul newline rămas în buffer

    // Solicită și citește tirajul (numărul de exemplare vândute)
    printf("Tirajul:");
    scanf("%d", &book_input.quantity_sold);
    getchar(); // Consumă caracterul newline rămas în buffer

    return book_input;
}

/*
 * Funcție pentru gestionarea inserării unui element nou în arbore
 * Solicită datele de la utilizator și le inserează în arbore
 */
void handle_insertion(BinaryTree * tree) {
    printf("\n");

    // Obține datele cărții de la utilizator
    Book book_input = get_book_from_user();

    // Creează o nouă carte utilizând datele introduse
    Book * new_book = create_book(book_input.key,
                                  book_input.title,
                                  book_input.author,
                                  book_input.pub_year,
                                  book_input.page_count,
                                  book_input.quantity_sold);

    // Inserează cartea în arbore
    insert(tree, new_book);

    printf("\n");
}

/*
 * Funcție pentru afișarea meniului principal
 * Prezintă utilizatorului opțiunile disponibile
 */
void print_menu() {
    printf("1) Insereaza Un Element\n");
    printf("2) Afiseaza Elementele\n");
    printf("3) Cauta Nod\n");
    printf("4) Parcurge Arborele\n");
    printf("5) Balanseaza Arborele\n");
    printf("6) Oglindire Arbore\n");
    printf("7) Curatare Arbore\n");
    printf("0) Iesire din program\n");
}

/*
 * Funcție pentru căutarea unui nod în baza cheii și afișarea detaliilor
 * Solicită utilizatorului cheia cărții căutate
 */
void handle_search(BinaryTree * tree) {
    printf("\n");

    int key;

    // Solicită și citește cheia căutată
    printf("Introdu cheia:");
    scanf("%d", &key);

    // Caută nodul cu cheia specificată
    BinaryTreeNode * found_node = get(tree, key);

    // Verifică dacă nodul a fost găsit
    if (!found_node) {
        printf("Cartea cu cheia %d nu a fost gasita.\n", key);
        return;
    }

    // Afișează detaliile cărții găsite
    Book * book = found_node->book;
    printf("Titlu: %s\n", book->title);
    printf("Autor: %s\n", book->author);
    printf("Anul: %d\n", book->pub_year);
    printf("Pagini: %d\n", book->page_count);
    printf("Tiraj: %d\n", book->quantity_sold);

    printf("\n");
}

/*
 * Funcție pentru alegerea tipului de parcurgere a arborelui
 * Returnează: numărul opțiunii alese de utilizator
 */
int get_trasversal_type_from_user() {
    int choice;

    while (true) {
        // Afișează opțiunile de parcurgere disponibile
        printf("1) Inordine (SVD)\n");
        printf("2) Postordine (SDV)\n");
        printf("3) Preordine (VSD)\n");
        printf("4) In inaltime (DFS)\n");
        printf("5) In latime (BFS)\n");
        printf("Introdu Optiunea:");

        // Citește opțiunea utilizatorului
        scanf("%d", &choice);
        getchar(); // Consumă caracterul newline rămas în buffer

        // Verifică dacă opțiunea este validă
        if (1 <= choice && choice <= 5) {
            return choice;
        }


        // Afișează mesaj de eroare pentru opțiune invalidă
        printf("Optiune gresita.\n");
   }
}

/*
 * Funcție pentru gestionarea parcurgerii arborelui
 * În funcție de opțiunea utilizatorului, apelează metoda corespunzătoare de parcurgere
 */
void handle_trasversal(BinaryTree * tree) {
    printf("\n");

    // Obține tipul de parcurgere de la utilizator
    int trasversal_type = get_trasversal_type_from_user();

    // Execută tipul de parcurgere ales
    switch (trasversal_type) {
    case 1:
        SVD_trasversal(tree); break; // Parcurgere în inordine
    case 2:
        SDV_trasversal(tree); break; // Parcurgere în postordine
    case 3:
        VSD_trasversal(tree); break; // Parcurgere în preordine
    case 4:
        DFS(tree); break; // Parcurgere în adâncime
    case 5:
        BFS(tree); break; // Parcurgere în lățime
    default:
        break;
    }

    printf("\n");
}

/*
 * Funcție pentru eliminarea tuturor nodurilor din arbore
 * Eliberează memoria alocată pentru fiecare nod și pentru cărțile asociate
 * Utilizează parcurgerea în lățime (BFS) pentru a vizita toate nodurile
 * Complexitate: O(n) unde n este numărul de noduri
 */
void clear_tree(BinaryTree * tree) {
    BinaryTreeNode * root = tree->root;

    // Verifică dacă arborele este gol
    if (!root) return;

    // Creează o coadă pentru parcurgerea în lățime
    Queue * queue = create_queue();

    // Adaugă rădăcina în coadă
    enqueue(queue, root);

    // Parcurge toate nodurile și eliberează memoria
    while (!is_eueue_empty(queue)) {
        // Extrage un nod din coadă
        BinaryTreeNode * current_tree_node = dequeue(queue);

        // Adaugă copilul stâng în coadă, dacă există
        if (current_tree_node->left) enqueue(queue, current_tree_node->left);

        // Adaugă copilul drept în coadă, dacă există
        if (current_tree_node->right) enqueue(queue, current_tree_node->right);

        // Eliberează memoria pentru carte și nod
        free(current_tree_node->book);
        free(current_tree_node);
    }

    // Setează rădăcina la NULL, indicând arbore gol
    tree->root = NULL;

    // Eliberează memoria cozii
    free(queue);
}

/*
 * Funcția main - punctul de intrare în program
 * Inițializează arborele și execută comenzile de test
 */
int main(void) {
    int choice;

    // Creează un arbore binar de căutare
    BinaryTree * tree = create_tree();

    // Populează arborele cu date de test
    mock_populate_tree(tree);

    // Codul comentat mai jos reprezintă meniul interactiv
    // care a fost înlocuit temporar cu teste directe


    /*

    do {
       print_menu();

       printf("Introdu optiunea:");
       scanf("%d", &choice);
       getchar();

       switch (choice) {
        case 1:
            handle_insertion(tree); break;
        case 2:
            display_tree(tree); break;
        case 3:
            handle_search(tree); break;
        case 4:
            handle_trasversal(tree); break;
        case 5:
            balance_tree(tree); break;
        case 6:
            mirror_tree(tree); break;
        case 7:
            clear_tree(tree); break;
        case 0:
            printf("Bye bye."); break;
        default:
            printf("Obtiune Invalida\n"); break;
       }

    } while (choice != 0);


*/
    // Execută teste pentru diferite tipuri de parcurgere
    // Parcurgere în inordine (SVD)
    SVD_trasversal(tree);
    printf("\n");

    // Parcurgere în preordine (VSD)
    VSD_trasversal(tree);
    printf("\n");

    // Parcurgere în postordine (SDV)
    SDV_trasversal(tree);
    printf("\n");

    // Parcurgere în lățime (BFS)
    BFS(tree);
    printf("\n");

    // Balansează arborele
    balance_tree(tree);

    printf("\n");

    // Repetă parcurgerile pe arborele balansat pentru comparare
    SVD_trasversal(tree);
    printf("\n");
    VSD_trasversal(tree);
    printf("\n");
    SDV_trasversal(tree);
    printf("\n");
    BFS(tree);
    printf("\n");

    return 0;
}

/*
 * Notă importantă:
 * Parcurgerea în inordine nu ne ajută cu depistarea pozițiilor nodurilor.
 */
/*SVD- in ordire stang,valoarea,drept
 *VSD in preordine
 *SDV Postordine
 *BFS- latime
 *DFS- in adancime
 */