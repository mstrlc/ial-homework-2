/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value)
{
	// If tree is empty, return false
	if (tree == NULL)
	{
		return false;
	}

	// If key equals the key of current node, set value to key
	if (tree->key == key)
	{
		*value = tree->value;
		return true;
	}

	// If key of current node is bigger than given key, search in left subtree
	if (tree->key > key)
		return bst_search(tree->left, key, value);
	// Otherwise, search right subtree
	else
		return bst_search(tree->right, key, value);
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value)
{
	// If tree is empty, create new node
	if (*tree == NULL)
	{
		// Allocate needed space
		*tree = malloc(sizeof(bst_node_t));
		// Malloc fail
		if (*tree == NULL)
		{
			return;
		}
		// Insert data
		(*tree)->key = key;
		(*tree)->value = value;
		(*tree)->left = NULL;
		(*tree)->right = NULL;
		return;
	}

	// If current node key is same as given key, replace value
	if ((*tree)->key == key)
	{
		(*tree)->value = value;
		return;
	}

	// If current node key is bigger than given key, insert in left subtree
	if ((*tree)->key > key)
		bst_insert(&(*tree)->left, key, value);
	// Otherwise, insert in right subtree
	else
		bst_insert(&(*tree)->right, key, value);
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.

 *
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
	// If right subtree is empty, replace target with current node
	if ((*tree)->right == NULL)
	{
		target->key = (*tree)->key;
		target->value = (*tree)->value;

		// Save pointer to current node
		bst_node_t *tmp = *tree;
		// Replace current node with left subtree
		*tree = (*tree)->left;
		// Free memory
		free(tmp);
		return;
	}

	// Otherwise, search right subtree
	bst_replace_by_rightmost(target, &(*tree)->right);
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
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key)
{
	// If tree is empty, return
	if (*tree == NULL)
	{
		return;
	}

	// If current node key is same as given key, delete node
	if ((*tree)->key == key)
	{
		// If node has no children, delete it
		if ((*tree)->left == NULL && (*tree)->right == NULL)
		{
			free(*tree);
			*tree = NULL;
			return;
		}
		// If node has one child, move it to parent node
		if ((*tree)->left == NULL)
		{
			bst_node_t *tmp = *tree;
			*tree = (*tree)->right;
			free(tmp);
			return;
		}
		if ((*tree)->right == NULL)
		{
			bst_node_t *tmp = *tree;
			*tree = (*tree)->left;
			free(tmp);
			return;
		}
		// If node has two children, replace it with rightmost node in left subtree
		bst_replace_by_rightmost(*tree, &(*tree)->left);
		return;
	}

	// If current node key is bigger than given key, delete in left subtree
	if ((*tree)->key > key)
		bst_delete(&(*tree)->left, key);
	// Otherwise, delete in right subtree
	else
		bst_delete(&(*tree)->right, key);
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree)
{
	// If tree is empty, return
	if (*tree == NULL)
	{
		return;
	}

	// Delete both subtrees
	bst_dispose(&(*tree)->left);
	bst_dispose(&(*tree)->right);

	// Delete current node
	free(*tree);
	*tree = NULL;
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree)
{
	if (tree == NULL)
	{
		return;
	}
	// Recursively print preorder
	bst_print_node(tree);
	bst_preorder(tree->left);
	bst_preorder(tree->right);
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree)
{
	if (tree == NULL)
	{
		return;
	}

	// Recursively print inorder
	bst_inorder(tree->left);
	bst_print_node(tree);
	bst_inorder(tree->right);
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree)
{
	if (tree == NULL)
	{
		return;
	}
	
	// Recursively print postorder
	bst_postorder(tree->left);
	bst_postorder(tree->right);
	bst_print_node(tree);
}
