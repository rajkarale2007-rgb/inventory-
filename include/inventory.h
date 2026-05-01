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

// C API exposed to C++
int add_item(const Item* item);
int get_item(int id, Item* out);
int update_item(int id, const Item* updated);
int delete_item(int id);
int list_items(Item* buffer, int max_items);

#ifdef __cplusplus
}
#endif

#endif // INVENTORY_H
