#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TABLE_SIZE 10 //declaramos la macro para dale el tamaño máximo de buckets.

typedef struct Node { //es un bucket de elementos (lista enlazada).
    int value; //es el valor.
    struct Node* next; //recorremos la lista enlazada.
} Node; //es un alias.

typedef struct HashTable {
    Node ** table; //buckets.
    int (*hashFunction)(int); //función hash.
} HashTable; //es un alias.

typedef struct RandomHashFun { //estructura de la función hash mas elaborada.
    int *fnTable; //puntero para el array[n].
    int M; //rango maximo para la FH.
} RandomHashFun;

HashTable* init(int (*hashFn)(int)) { //crear e inicializar un Hash Table, recibe de parametros unicamente la Función Hash.
    HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
    hashTable->table = (Node**)calloc(TABLE_SIZE, sizeof(Node*));
    hashTable->hashFunction = hashFn;
    return hashTable;
}

void insert(HashTable* hashTable, int value) { //insertar un elemento a la HT correspondiente.
    int index = hashTable->hashFunction(value);

    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->next = NULL;

    if (hashTable->table[index] == NULL) {
        hashTable->table[index] = newNode;
    } else {
        Node* current = hashTable->table[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void delete(HashTable* hashTable, int value) { //elimina un elemento de la HT.
    int index = hashTable->hashFunction(value);

    Node* current = hashTable->table[index];
    Node* prev = NULL;

    while (current != NULL) {
        if (current->value == value) {
            if (prev == NULL) {
                hashTable->table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

int find(HashTable* hashTable, int value) { //busca un elemento en la HT.
    int index = hashTable->hashFunction(value);

    Node* current = hashTable->table[index];

    while (current != NULL) {
        if (current->value == value) {
            return current->value;
        }
        current = current->next;
    }

    return -1;
}

void freeHashTable(HashTable* hashTable) { //Libera memoria en de una HT.
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = hashTable->table[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashTable->table);
    free(hashTable);
}

int mi_Mod(int value) { //función hash básica.
    return value % TABLE_SIZE;
}

int choice(int n) {
    return rand() % n;
}

RandomHashFun* createRandomHashFun(int M, int n) {
    RandomHashFun* rhf = (RandomHashFun*)malloc(sizeof(RandomHashFun));
    rhf->M = M;
    rhf->fnTable = (int*)malloc(M * sizeof(int));
    int i;
    for (i = 0; i < M; i++) {
        rhf->fnTable[i] = choice(n);
    }
    return rhf;
}

void destroyRandomHashFun(RandomHashFun* rhf) {
    free(rhf->fnTable);
    free(rhf);
}

/*INSTANCIACIÓN DE LA FUNCION CON SEGUIMIENTO A LOS VALORES*/
RandomHashFun* random_Hf;

int randomFn(int x) {
    srand(time(NULL)); //genera números PSUDOALEATORIOS.
    random_Hf = createRandomHashFun(1000,  TABLE_SIZE);
    return random_Hf->fnTable[x];
}

int main() { //ponemos a prueba si lo que hicimos esta bien :).

    HashTable* hashTable = init(mi_Mod);

    insert(hashTable, 45);
    insert(hashTable, 124);
    insert(hashTable, 76);

    if (find(hashTable, 44) != -1) {
        printf("%s\n","Se encontro el elemento");
    } else {
        printf("%s\n","ERROR");
    }

    /*
    printf("%d\n", find(hashTable, 45));
    delete(hashTable, 124);
    printf("%d\n", find(hashTable, 124));
    */
    
    freeHashTable(hashTable);
    //destroyRandomHashFun(random_Hf);
    return 0;
}