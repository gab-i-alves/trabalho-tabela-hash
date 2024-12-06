//Estrutura programa Tabela Hash - Professor Lucas.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TABLE_SIZE 100
#define MAX_NAME 50
#define MAX_PHONE 15

typedef struct Contact {
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    struct Contact* next;
} Contact;

typedef struct {
    Contact* table[TABLE_SIZE];
    int size;
} HashTable;

HashTable* createHashTable() {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if (ht == NULL) {
        printf("Erro ao alocar memória para a tabela hash!\n");
        exit(1);
    }
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->table[i] = NULL;
    }
    ht->size = 0;
    return ht;
}

unsigned int hash(const char* name) {
    unsigned int hash_value = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        hash_value = (hash_value * 31 + name[i]) % TABLE_SIZE;
    }
    return hash_value;
}

Contact* createContact(const char* name, const char* phone) {
    Contact* newContact = (Contact*)malloc(sizeof(Contact));
    if (newContact == NULL) {
        printf("Erro ao alocar memória para novo contato!\n");
        exit(1);
    }
    
    strncpy(newContact->name, name, MAX_NAME - 1);
    newContact->name[MAX_NAME - 1] = '\0';
    strncpy(newContact->phone, phone, MAX_PHONE - 1);
    newContact->phone[MAX_PHONE - 1] = '\0';
    newContact->next = NULL;
    return newContact;
}

void adicionarContato(HashTable* ht) {
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    
    printf("Nome: ");
    scanf(" %[^\n]s", name);
    printf("Telefone: ");
    scanf(" %[^\n]s", phone);
    
    clock_t start = clock();
    
    unsigned int index = hash(name);
    Contact* newContact = createContact(name, phone);
    
    // Inserção no início da lista encadeada
    newContact->next = ht->table[index];
    ht->table[index] = newContact;
    ht->size++;
    
    clock_t end = clock();
    double time_spent = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
    
    printf("Contato adicionado com sucesso! (tempo de inserção: %.2f ms)\n", time_spent);
}

void buscarContato(HashTable* ht) {
    char name[MAX_NAME];
    printf("Nome: ");
    scanf(" %[^\n]s", name);
    
    clock_t start = clock();
    
    unsigned int index = hash(name);
    Contact* current = ht->table[index];
    
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            clock_t end = clock();
            double time_spent = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
            printf("Telefone de %s: %s (tempo de busca: %.2f ms)\n", 
                   name, current->phone, time_spent);
            return;
        }
        current = current->next;
    }
    
    printf("Contato não encontrado!\n");
}

void removerContato(HashTable* ht) {
    char name[MAX_NAME];
    printf("Nome: ");
    scanf(" %[^\n]s", name);
    
    unsigned int index = hash(name);
    Contact* current = ht->table[index];
    Contact* prev = NULL;
    
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (prev == NULL) {
                // Remover primeiro elemento da lista
                ht->table[index] = current->next;
            } else {
                // Remover elemento do meio/fim da lista
                prev->next = current->next;
            }
            free(current);
            ht->size--;
            printf("Contato removido com sucesso!\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    
    printf("Contato não encontrado!\n");
}

void exibirContatos(HashTable* ht) {
    if (ht->size == 0) {
        printf("Lista telefônica vazia!\n");
        return;
    }
    
    printf("\nLista de Contatos:\n");
    printf("------------------\n");
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        Contact* current = ht->table[i];
        while (current != NULL) {
            printf("Nome: %-20s Telefone: %s\n", current->name, current->phone);
            current = current->next;
        }
    }
    printf("\nTotal de contatos: %d\n", ht->size);
}

void liberarHashTable(HashTable* ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Contact* current = ht->table[i];
        while (current != NULL) {
            Contact* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(ht);
}

int main() {
    HashTable* ht = createHashTable();
    int opcao;

    do {
        printf("\nEscolha uma opcao:\n");
        printf("1 - Adicionar contato\n");
        printf("2 - Buscar contato por nome\n");
        printf("3 - Remover contato\n");
        printf("4 - Exibir todos os contatos\n");
        printf("0 - Sair\n");
        printf("Digite uma opcao: ");
        
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionarContato(ht);
                break;
            case 2:
                buscarContato(ht);
                break;
            case 3:
                removerContato(ht);
                break;
            case 4:
                exibirContatos(ht);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    liberarHashTable(ht);
    return 0;
}
