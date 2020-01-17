#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <iterator>
#include <unordered_map>
#include <chrono>

// #include "utils.hpp"
#include "relationStructs.hpp"
#include "../templates/list.hpp"

// bloom filter implementation

typedef unsigned int (*HashFunction)(uint64_t);

class BloomFilter {
    unsigned int numberOfCells;
    unsigned int numberOfFunctions;
    bool* cell;
    std::vector<HashFunction> hashFunctions;

public:

    BloomFilter(unsigned int numbCells, std::vector<HashFunction> funcs) : numberOfCells(numbCells), hashFunctions(funcs) {
        cell = (bool*)calloc(numbCells, sizeof(bool));
    }

    bool addElement(uint64_t str) {
        bool ret = true;
        for (std::vector<HashFunction>::iterator iter = hashFunctions.begin(); iter != hashFunctions.end(); iter++) {
            ret &= cell[(*iter)(str) % numberOfCells];
            cell[(*iter)(str) % numberOfCells] = true;
        }
        return ret;
    }

    bool searchElement(uint64_t str) {
        bool strInSet = true;

        for (std::vector<HashFunction>::iterator iter = hashFunctions.begin(); iter != hashFunctions.end(); iter++) {
            if (cell[(*iter)(str) % numberOfCells] == false) {
                strInSet = false;
                break;
            }
        }

        return strInSet;
    }

    ~BloomFilter() {
        free(cell);
        cell = NULL;
    }
};

// implementing a couple of hash functions for testing

unsigned int djb2(uint64_t str) {
    unsigned int hash = 5381;

    // for (std::string::iterator iter = str.begin(); iter != str.end(); iter++) {
    hash = ((hash << 5) + hash) + (str >> 10) - (str << 3);
    // }

    return hash;
}

unsigned int sdbm(uint64_t str) {
    unsigned int hash = 0;

    // for (std::string::iterator iter = str.begin(); iter != str.end(); iter++) {
        // hash = ((hash << 5) + hash) + (str >> 10) - (str << 3);
        hash = ((hash << 7) + (hash << 12) - (hash >> 5) ) + str;
    // }

    // std::cout << hash << std::endl;
    return hash;
}

uint32_t SuperFastHash (uint64_t data) {

    uint32_t len = 64;

    // char* data = const_cast<char*>(str.c_str());

    uint32_t hash = len, tmp;
    int rem;
    
    rem = len & 3;
    len >>= 2;
    
    /* Main loop */
    for (;len > 0; len--) {
    hash += data & 0xFFFF;
    tmp = (((data & 0xFFFF) + 2) << 11) ^ hash;
    hash = (hash << 16) ^ tmp;
    data += 2 * sizeof (uint16_t);
    hash += hash >> 11;
    }
    
    /* Handle end cases */
    switch (rem) {
    case 3: hash += data & 0xFFFF;
      hash ^= hash << 16;
      hash ^= data << 18;
      hash += hash >> 11;
      break;
    case 2: hash += data & 0xFFFF;
      hash ^= hash << 11;
      hash += hash >> 17;
      break;
    case 1: hash += data;
      hash ^= hash << 10;
      hash += hash >> 1;
    }
    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;
    
    return hash;
}

List<RelationTable>* ReadRelations(const char *workloads_path)
{
    char *line = NULL;
    size_t len = 0;

    size_t work_len = strlen(workloads_path);

    List<RelationTable> *relations = new List<RelationTable>(sizeof(RelationTable), sizeof(RelationTable));

    while (getline(&line, &len, stdin) != -1)
    {
        RelationTable *tmp_rel_node = new RelationTable;

        char rel_path[work_len + strlen(line) + 2];
        memset(rel_path, 0, work_len + strlen(line) + 2);

        // Remove '\n'
        line[strlen(line) - 1] = '\0';

        // Build the new path
        strcpy(rel_path, workloads_path);
        rel_path[work_len] = '/';
        strcat(rel_path, line);

        FILE *rel_fp;

        rel_fp = fopen(rel_path, "rb");

        fread(&tmp_rel_node->rows, sizeof(uint64_t), 1, rel_fp);
        fread(&tmp_rel_node->cols, sizeof(uint64_t), 1, rel_fp);

        tmp_rel_node->table = new uint64_t *[tmp_rel_node->cols];
        for (int i = 0; i < tmp_rel_node->cols; i++)
            tmp_rel_node->table[i] = new uint64_t[tmp_rel_node->rows];

        for (int i = 0; i < tmp_rel_node->cols; i++)
            for (int j = 0; j < tmp_rel_node->rows; j++)
                fread(&tmp_rel_node->table[i][j], sizeof(uint64_t), 1, rel_fp);

        fclose(rel_fp);

        // unsigned int N = (unsigned int)50000000;

        // InitialStats(tmp_rel_node, N);

        relations->ListInsert(*tmp_rel_node);

        delete tmp_rel_node;
    }

    free(line);

    return relations;
}

int main() {
    // create bloom filter
    std::vector<HashFunction> hashFunctions;
    // hashFunctions.push_back(djb2);
    hashFunctions.push_back(sdbm);
    // hashFunctions.push_back(SuperFastHash);

    List<RelationTable>* rel1 = ReadRelations("./workloads/medium");
    

    RelationTable& reltb = (*rel1->GetFirst())[0];

    // uint64_t** tb = reltb.table;

    BloomFilter bf(reltb.rows * 4, hashFunctions);

    // List<uint64_t>* dist = new List<uint64_t>(sizeof(uint64_t), sizeof(uint64_t));

    // std::vector<uint64_t> dist;
    std::unordered_map<uint64_t,bool> distincts;
    // uint32_t distincts = reltb.rows;

    // for (int i = 0; i < reltb->cols; i++)
        // for (int j = 0; j < reltb->rows; j++)
            // fread(&reltb->table[i][j], sizeof(uint64_t), 1, rel_fp);

    std::cout << reltb.rows << std::endl;
    
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (int i = 0 ; i < reltb.rows ; i++) {
        // distincts -= bf.addElement( reltb.table[0][i] );
        // std::cout << bf.addElement( reltb.table[0][i] ) << std::endl;
        // if ( bf.addElement( reltb.table[0][i] ) )
        //     distincts.erase(reltb.table[0][i]);
        // else {
        distincts[reltb.table[0][i]] = 1;
        // }
        // std::cout << "\t" + reltb.table[0][i] << std::endl;
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    
    std::cout << "Time difference (sec) = " << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000000.0 << std::endl;
    
    // // testing a set of strings against the bloom filter
    // for (std::vector<std::string>::iterator iter = testSetOfStrings.begin(); iter != testSetOfStrings.end(); iter++) {
    //     std::cout << "\t" + *iter + ": " << bf.searchElement(*iter) << std::endl;
    // }

    std::cout << distincts.size() << std::endl;
}