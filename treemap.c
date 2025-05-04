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
    
    TreeNode* current = tree->root;
    TreeNode* parent = NULL;

    while (current != NULL) {
        parent = current;
        if (tree->lower_than(key, current->pair->key)) {
            current = current->left;
        } else if (tree->lower_than(current->pair->key, key)) {
            current = current->right;
        } else {
            // Si las claves son iguales, retornamos sin hacer nada (sin insertar duplicados)
            return;
        }
    }

    // Crear el nuevo nodo
    TreeNode* newNode = createTreeNode(key, value);
    if (newNode == NULL) return;

    if (parent == NULL) {
        tree->root = newNode;
    } else {
        // Si la clave es menor que la del nodo actual, insertamos en el subárbol izquierdo
        if (tree->lower_than(key, parent->pair->key)) {
            parent->left = newNode;
        } else {
            // Si la clave es mayor, insertamos en el subárbol derecho
            parent->right = newNode;
        }
        // Establecer el puntero al padre
        newNode->parent = parent;
    }

    // Después de la inserción, hacer que 'current' apunte al nuevo nodo
    tree->current = newNode;
}


Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* current = tree->root;
    while (current != NULL) {
    
        int comparison = tree->lower_than(key, current->pair->key);

        if (comparison == 0) {
            tree->current = current;
            return current->pair;
        } else if (comparison == 1) {
            // Si key < current->pair->key, nos movemos al subárbol izquierdo
            current = current->left;
        } else {
            // Si key > current->pair->key, nos movemos al subárbol derecho
            current = current->right;
        }
    }
    tree->current = NULL;
    return NULL;
}

TreeNode * minimum(TreeNode * x){
    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}

void removeNode(TreeMap *tree, TreeNode *node) {
    if (node == NULL) {
        return;  // Si el nodo es NULL, no hacer nada
    }

    // Caso 1: Nodo sin hijos (hoja)
    if (node->left == NULL && node->right == NULL) {
        // Si el nodo es la raíz
        if (node->parent == NULL) {
            tree->root = NULL;
        } else {
            // Si el nodo es hijo izquierdo o derecho del padre
            if (node->parent->left == node) {
                node->parent->left = NULL;
            } else {
                node->parent->right = NULL;
            }
        }

        // Liberar la memoria del nodo
        free(node->pair);  // Liberar los datos del par
        free(node);  // Liberar el nodo en sí
    }
    // Caso 2: Nodo con un solo hijo (izquierdo o derecho)
    else if (node->left == NULL || node->right == NULL) {
        // Seleccionar al hijo no NULL
        TreeNode *child = (node->left != NULL) ? node->left : node->right;

        // Si el nodo es la raíz
        if (node->parent == NULL) {
            tree->root = child;
        } else {
            // Actualizar el puntero del padre para que apunte al hijo
            if (node->parent->left == node) {
                node->parent->left = child;
            } else {
                node->parent->right = child;
            }
        }

        // El hijo se convierte en el nuevo padre del nodo
        child->parent = node->parent;

        // Liberar la memoria del nodo
        free(node->pair);  // Liberar los datos del par
        free(node);  // Liberar el nodo en sí
    }
    // Caso 3: Nodo con dos hijos
    else {
        // Buscar el nodo más pequeño en el subárbol derecho (sucesor)
        TreeNode *successor = minimum(node->right);

        // Copiar los datos del sucesor al nodo
        node->pair->key = successor->pair->key;
        node->pair->value = successor->pair->value;

        // Llamamos a removeNode para eliminar el sucesor, que tendrá como máximo un hijo
        removeNode(tree, successor);
    }

    // Si es necesario, actualizar 'current' a NULL después de la eliminación (según tu implementación)
    // Si eliminamos el nodo que está siendo apuntado por 'current', actualizamos 'current'
    if (tree->current == node) {
        tree->current = NULL;
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}



Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode* current = tree->root;
    TreeNode* successor = NULL;

    while (current != NULL) {
        if (tree->lower_than(current->pair->key, key)) {
            current = current->right;
        } else {
            successor = current;
            current = current->left;
        }
    }

    return successor ? successor->pair : NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree->root == NULL) return NULL;
    TreeNode* first = minimum(tree->root);
    return first ? first->pair : NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree->current == NULL) return NULL;
    
    // Si el nodo tiene un subárbol derecho, el siguiente es el más a la izquierda de ese subárbol
    if (tree->current->right != NULL) {
        tree->current = minimum(tree->current->right);
        return tree->current->pair;
    }

    // Si no tiene subárbol derecho, subimos hasta encontrar un ancestro cuya clave es mayor
    TreeNode* parent = tree->current->parent;
    while (parent != NULL && tree->current == parent->right) {
        tree->current = parent;
        parent = parent->parent;
    }
    tree->current = parent;

    return tree->current ? tree->current->pair : NULL;
}
