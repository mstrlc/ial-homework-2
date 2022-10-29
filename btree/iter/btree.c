/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree)
{
	// Initialize tree to NULL
	*tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value)
{
	// If tree is empty, return false
	if (tree == NULL)
	{
		return false;
	}

	bst_node_t *current = tree;
	while (current != NULL)
	{
		// If key equals the key of current node, set value to key
		if (current->key == key)
		{
			*value = current->value;
			return true;
		}

		// If key of current node is bigger than given key, search in left subtree
		if (current->key > key)
		{
			current = current->left;
		}
		// Otherwise, search right subtree
		else
		{
			current = current->right;
		}
	}

	return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value)
{
	// If tree is empty, create new node
	if (*tree == NULL)
	{
		*tree = malloc(sizeof(bst_node_t));
		if (*tree == NULL)
		{
			return;
		}
		// Set values
		(*tree)->key = key;
		(*tree)->value = value;
		(*tree)->left = NULL;
		(*tree)->right = NULL;

		return;
	}

	// If tree is not empty, search for node with given key
	bst_node_t *current = *tree;

	while (current != NULL)
	{
		// If key equals the key of current node, set value to key
		if (current->key == key)
		{
			current->value = value;
			return;
		}
		// If key of current node is bigger than given key, search in left subtree
		else if (current->key > key)
		{
			if (current->left == NULL)
			{
				bst_node_t *new = malloc(sizeof(bst_node_t));
				new->key = key;
				new->value = value;
				new->left = new->right = NULL;
				current->left = new;
				return;
			}
			current = current->left;
		}
		// Otherwise, search right subtree
		else if (current->key < key)
		{
			if (current->right == NULL)
			{
				bst_node_t *new = malloc(sizeof(bst_node_t));
				new->key = key;
				new->value = value;
				new->left = new->right = NULL;
				current->right = new;
				return;
			}
			current = current->right;
		}
	}
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
	// Store current node and its parent
	bst_node_t *current = *tree;
	bst_node_t *previous = NULL;

	// Find rightmost node
	while (current->right != NULL)
	{
		previous = current;
		current = current->right;
	}

	// Fill target with rightmost values
	target->key = current->key;
	target->value = current->value;

	if (previous != NULL)
		previous->right = NULL;

	// If rightmost node has left child, move it to rightmost node's place
	if (current->left)
	{
		previous->right = current->left;
	}

	free(current);
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key)
{
	// If tree is empty, return
	if (*tree == NULL)
	{
		return;
	}

	// If tree is not empty, search for node with given key
	bst_node_t *current = *tree;
	bst_node_t *previous = NULL;

	char side;

	// Find node with given key
	while (current != NULL)
	{
		if (current->key == key)
		{
			break;
		}

		else if (current->key < key)
		{
			previous = current;
			current = current->right;
			side = 'r';
		}

		else if (current->key > key)
		{
			previous = current;
			current = current->left;
			side = 'l';
		}
	}

	// If node with given key was not found, return
	if (current == NULL)
	{
		return;
	}

	// If the node has both children, replace it with rightmost node
	if (current->left != NULL && current->right != NULL)
	{
		bst_replace_by_rightmost(current, &(current->left));
	}

	// Node has only one child
	else
	{
		// Node only has right child
		if (current->left == NULL)
		{
			// Child was the left node of its parent
			if (side == 'l')
				previous->left = current->right;
			// Child was the right node of its parent
			else if (side == 'r')
				previous->right = current->right;
		}
		// Node only has left child
		if (current->right == NULL) 
		{
			// Child was the left node of its parent
			if (side == 'l')
				previous->left = current->left;
			// Child was the right node of its parent
			else if (side == 'r')
				previous->right = current->left;
		}

		free(current);
	}
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree)
{
	// If tree is empty, return
	if (*tree == NULL)
	{
		return;
	}

	// Create stack
	stack_bst_t *stack = malloc(sizeof(stack_bst_t));
	// Malloc fail
	if (stack == NULL)
	{
		return;
	}
	// Initialize stack
	stack_bst_init(stack);

	bst_node_t *current = *tree;

	// Go through the tree until it is empty
	while (stack_bst_empty(stack) == false || current != NULL)
	{
		// If current node is NULL, stack must not be empty
		// Pop node from stack
		if (current == NULL)
		{
			current = stack_bst_pop(stack);
		}

		// Push right child to stack if it exists
		if (current->right != NULL)
		{
			stack_bst_push(stack, current->right);
		}

		// Move current node to the left child
		// Store the current node to be deleted
		bst_node_t *tmp = current;
		current = current->left;
		free(tmp);
	}

	// Free stack and set tree to NULL
	*tree = NULL;
	free(stack);
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit)
{
	bst_node_t *current = tree;

	// Go through the tree until it is empty
	while (current != NULL)
	{
		// Print node and push it to stack
		bst_print_node(current);
		stack_bst_push(to_visit, current);
		// Continue with left child
		current = current->left;
	}
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree)
{
	// Create stack
	stack_bst_t *stack = malloc(sizeof(stack_bst_t));
	// Malloc fail
	if (stack == NULL)
	{
		return;
	}

	// Initialize stack
	stack_bst_init(stack);
	// Call leftmost preorder
	bst_leftmost_preorder(tree, stack);

	// Go through the tree until the stack is empty
	while (stack_bst_empty(stack) == false)
	{
		// Pop unvisited node from stack
		tree = stack_bst_pop(stack);
		// Call leftmost preorder on the right child
		bst_leftmost_preorder(tree->right, stack);
	}

	// Free stack
	free(stack);
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{
	bst_node_t *current = tree;

	// Go through the tree until it is empty
	while (current != NULL)
	{
		// Push node to stack
		stack_bst_push(to_visit, current);
		// Continue with left child
		current = current->left;
	}
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree)
{
	// Create stack
	stack_bst_t *stack = malloc(sizeof(stack_bst_t));

	// Malloc fail
	if (stack == NULL)
	{
		return;
	}
	// Initialize stack
	stack_bst_init(stack);

	// Call leftmost inorder
	bst_leftmost_inorder(tree, stack);

	// Go through the tree until the stack is empty
	while (stack_bst_empty(stack) == false)
	{
		// Pop unvisited node from stack
		tree = stack_bst_pop(stack);
		// Print node
		bst_print_node(tree);
		// Call leftmost inorder on the right child
		bst_leftmost_inorder(tree->right, stack);
	}

	// Free stack
	free(stack);
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
							stack_bool_t *first_visit)
{
	bst_node_t *current = tree;
	
	// Go through the tree until it is empty
	while (current != NULL)
	{
		// Push node to stack
		stack_bst_push(to_visit, current);
		// Push true to stack that the node has been visited for the first time
		stack_bool_push(first_visit, true);
		// Continue with left child
		current = current->left;
	}
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree)
{
	// Create stacks and initialize them
	stack_bst_t *stack_bst = malloc(sizeof(stack_bst_t));
	stack_bst_init(stack_bst);

	stack_bool_t *stack_bool = malloc(sizeof(stack_bool_t));
	stack_bool_init(stack_bool);

	// Malloc fail
	if (stack_bst == NULL || stack_bool == NULL)
	{
		return;
	}

	// Call leftmost postorder
	bst_leftmost_postorder(tree, stack_bst, stack_bool);


	// Go through the tree until the stack is empty
	bst_node_t *current = tree;
	while (stack_bst_empty(stack_bst) == false)
	{
		// Pop unvisited node from stack
		current = stack_bst_pop(stack_bst);
		// Pop bool value from stack, if it is true, push the node back to the stack
		// and call leftmost postorder on the right child
		if (stack_bool_pop(stack_bool) == true)
		{
			stack_bst_push(stack_bst, current);
			stack_bool_push(stack_bool, false);
			bst_leftmost_postorder(current->right, stack_bst, stack_bool);
		}
		// If the bool value is false, print the node
		else
		{
			bst_print_node(current);
		}
	}

	// Free stacks
	free(stack_bst);
	free(stack_bool);
}
