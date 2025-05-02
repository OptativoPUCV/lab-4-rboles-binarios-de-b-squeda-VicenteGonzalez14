#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lT) (void* key1, void* key2)) {
    TreeMap* map = (TreeMap*) malloc(sizeof(TreeMap));
    
    // Verificar si la memoria fue correctamente asignada
    if (map == NULL) {
        return NULL;  // Si no se pudo asignar memoria, retornamos NULL
    }

    // Inicializar la función de comparación
    map->lower_than = lT;

    
    map->root = NULL;
    map->current = NULL;

    // Retornar el TreeMap inicializado
    return map;

    //new->lower_than = lower_than;
    return NULL;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode* newNode = createTreeNode(key, value);
    
  
    if (tree->root == NULL) {
        tree->root = newNode;
        return;
    }

    TreeNode* current = tree->root;
    TreeNode* parent = NULL;

   
    while (current != NULL) {
        parent = current;
        if (tree->lower_than(key, current->pair->key)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    
    if (tree->lower_than(key, parent->pair->key)) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

  
    newNode->parent = parent;
}

TreeNode * minimum(TreeNode * x){
    while (x->left != NULL) {
        x = x->left;
    }
    return x;
    return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node->left == NULL && node->right == NULL) {
        // El nodo no tiene hijos
        if (node->parent == NULL) {
            tree->root = NULL;  
        } else if (node == node->parent->left) {
            node->parent->left = NULL;
        } else {
            node->parent->right = NULL;
        }
        free(node->pair);
        free(node);
    } else if (node->left == NULL || node->right == NULL) {
        // El nodo tiene un solo hijo
        TreeNode* child = (node->left != NULL) ? node->left : node->right;
        
        if (node->parent == NULL) {
            tree->root = child;  
        } else if (node == node->parent->left) {
            node->parent->left = child;
        } else {
            node->parent->right = child;
        }
        
        child->parent = node->parent;
        free(node->pair);
        free(node);
    } else {
        // El nodo tiene dos hijos, necesitamos encontrar el sucesor
        TreeNode* successor = minimum(node->right);
        node->pair->key = successor->pair->key;
        node->pair->value = successor->pair->value;
        removeNode(tree, successor);  
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* current = tree->root;
    while (current != NULL) {
    
        int comparison = tree->lower_than(key, current->pair->key);

        if (comparison == 0) {
            return current->pair;
        } else if (comparison == 1) {
            // Si key < current->pair->key, nos movemos al subárbol izquierdo
            current = current->left;
        } else {
            // Si key > current->pair->key, nos movemos al subárbol derecho
            current = current->right;
        }
    }

    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
