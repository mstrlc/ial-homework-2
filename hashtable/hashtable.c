/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key)
{
	int result = 1;
	int length = strlen(key);
	for (int i = 0; i < length; i++)
	{
		result += key[i];
	}
	return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table)
{
	// Initialize each element of table to null
	for (int i = 0; i < HT_SIZE; i++)
	{
		(*table)[i] = NULL;
	}
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key)
{
	// Return NULL if table is empty or key doesn't exist
	if (table == NULL || key == NULL)
	{
		return NULL;
	}

	int hash = get_hash(key);

	// Find item based on hash
	ht_item_t *item = (*table)[hash];

	// Go through all items in the table on the same hash
	while (item != NULL)
	{
		// If found, return the item
		if (strcmp(item->key, key) == 0)
		{
			return item;
		}
		// Move onto another
		item = item->next;
	}

	// If not found, return NULL
	return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value)
{
	// Return if table is empty or key doesn't exist
	if (table == NULL || key == NULL)
	{
		return;
	}

	ht_item_t *item = ht_search(table, key);

	// If item exists, replace its value
	if (item != NULL)
	{
		item->value = value;
		return;
	}

	int hash = get_hash(key);

	// Create new item
	ht_item_t *newItem = malloc(sizeof(ht_item_t));
	
	// Malloc fail
	if (newItem == NULL)
	{
		return;
	}

	// Set values
	newItem->key = key;
	newItem->value = value;
	newItem->next = (*table)[hash];

	// Add new item to table
	(*table)[hash] = newItem;
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key)
{
	// Return NULL if table is empty or key doesn't exist
	if (table == NULL || key == NULL)
	{
		return NULL;
	}

	ht_item_t *item = ht_search(table, key);

	// If item exists, return its value
	if (item != NULL)
	{
		return &item->value;
	}

	return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key)
{
	// Return if table is empty or key doesn't exist
	if (table == NULL || key == NULL)
	{
		return;
	}

	int hash = get_hash(key);

	// Find item based on hash
	ht_item_t *item = (*table)[hash];
	ht_item_t *prev = NULL;

	// Go through all items in the table on the same hash
	while (item != NULL)
	{
		// If found, delete it
		if (strcmp(item->key, key) == 0)
		{
			// If it's the first item in the list
			if (prev == NULL)
			{	
				// Set the next to be first
				(*table)[hash] = item->next;
			}
			// If it's not the first
			else
			{
				// Step over the deleted item
				prev->next = item->next;
			}
			free(item);
			return;
		}

		// Move onto another
		prev = item;
		item = item->next;
	}
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table)
{
	// Return if table is empty
	if (table == NULL)
	{
		return;
	}

	ht_item_t *item;
	ht_item_t *next;

	// Go through all items in the table
	for (int i = 0; i < HT_SIZE; i++)
	{
		item = (*table)[i];
		
		// Go through all items in the list
		while (item != NULL)
		{
			next = item->next;
			free(item);
			item = next;
		}
		// Set the list to NULL
		(*table)[i] = NULL;
	}
}
