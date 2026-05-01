#ifndef INVENTORY_H
#define INVENTORY_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int id;
    char name[40];
    int quantity;
    float price;
    int is_deleted;
} Item;

/* Returns 1 on success, 0 on failure (e.g., duplicate ID) */
int add_item(const Item* item);

/* Returns 1 if found and not deleted, 0 otherwise */
int get_item(int id, Item* out);

/* Returns 1 on success, 0 if not found */
int update_item(int id, const Item* updated);

/* Returns 1 on success, 0 if not found (soft delete) */
int delete_item(int id);

/* Populates buffer with up to max_items non-deleted items. Returns count of items retrieved. */
int list_items(Item* buffer, int max_items);

#ifdef __cplusplus
}
#endif

#endif /* INVENTORY_H */
