#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
    // create a next pointer (for linkedlist structure)

    struct RecordType *next;
};

// Fill out this structure
struct HashType
{
    // create a pointer to a RecordType
    struct RecordType *pRecord;
};

// Compute the hash function
int hash(int x, int tableSize)
{
    // hash function is x % tableSize
    return (x % tableSize);
}

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData)
{
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
            pRecord->next = NULL;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// insert record into the hash table
void insertRecord(struct HashType *hashTable, struct RecordType *record, int tableSize)
{
    int hashIndex = -1;
    struct RecordType *tempTraverse = NULL;
    
    // call the hash function to get the index
    // I'll use each record ID + record name and modulo it to get my hash value.
    hashIndex = hash(record->id + record->name, tableSize);

    // if the RecordType at that index is NULL
        // set 'record' equal to the HashType pointer in the table at index
    if ((hashTable[hashIndex]).pRecord == NULL)
    {
        (hashTable[hashIndex]).pRecord = record;
    }
    // else
        // traverse to the end of the linkedlist and add 'record' to the end of it
    else
    {
        tempTraverse = (hashTable[hashIndex]).pRecord;

        while (tempTraverse->next != NULL)
            tempTraverse = tempTraverse->next;

        tempTraverse->next = record;
    }

    printf("Inserted id: %d at hash index: %d.\n", record->id, hashIndex);
    //}
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *hashTable, int tableSize)
{
    // for each entry in the table
    // print the contents in that index
    // The output should look like this (for example): "Index 1 -> 21715, Q, 16 -> 28876, a, 26 -> NULL"
    struct RecordType *p = NULL;

    printf("==========================================\n");
    printf("|              HASH TABLE                |\n"      
           "==========================================\n"
           "*Hash key* = (ID + ASCII value of name char) modulo tableSize\n\n");

    for (int i = 0; i < tableSize; i++)
    {
        p = hashTable[i].pRecord;

        // If linkedlist index > 1
        if (p != NULL && p->next != NULL)
        {
            printf("Index %d: ", i);

            while (p != NULL)
            {
                printf("|%d, %c, %d| -> ", p->id, 
                                                     p->name, 
                                                     p->order);

                p = p->next;
            }
            printf("NULL\n");
        }
        // If linkedlist index == 1
        else if (p != NULL)
        {
            printf("Index %d: |%d, %c, %d| -> NULL\n", i, 
                                                p->id, 
                                                p->name, 
                                                p->order);
        }
        // If empty index
        else 
            printf("Index %d is EMPTY\n", i);
    }

    printf("\nEND OF PRINT\n");
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);
    printf("===============================================");
    printf("\n\n");

    // Initialize the hash table
    struct HashType *hashTable;

    // create a variable hashTableSize and assign it a value 
    int hashTableSize = 11; 

    // initialize a hashTable, use calloc (so everything is assigned to NULL)
    hashTable = (struct HashType*)calloc(hashTableSize, sizeof(struct HashType));

    // for each record in pRecords, insert it into the hash table using the insertRecord function
    for (int i = 0; i < recordSz; i++)
    {
        insertRecord(hashTable, &(pRecords[i]), hashTableSize);
    }
    printf("\nINSERT DONE.\n");

    // call the display records function
    displayRecordsInHash(hashTable, hashTableSize);

    // free all the allocated memory
    free(pRecords);
    free(hashTable);

    return 0;
}
