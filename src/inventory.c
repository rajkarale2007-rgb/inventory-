#include "inventory.h"
#include <stdio.h>
#include <string.h>

#define FILE_NAME "inventory.dat"

int add_item(const Item* item) {
    FILE* file = fopen(FILE_NAME, "a+b");
    if (!file) return 0;

    Item temp;
    rewind(file);
    while (fread(&temp, sizeof(Item), 1, file) == 1) {
        if (temp.id == item->id && temp.is_deleted == 0) {
            fclose(file);
            return 0; /* Duplicate ID found */
        }
    }

    fseek(file, 0, SEEK_END);
    int written = fwrite(item, sizeof(Item), 1, file);
    fclose(file);
    return written == 1 ? 1 : 0;
}

int get_item(int id, Item* out) {
    FILE* file = fopen(FILE_NAME, "rb");
    if (!file) return 0;

    while (fread(out, sizeof(Item), 1, file) == 1) {
        if (out->id == id && out->is_deleted == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int update_item(int id, const Item* updated) {
    FILE* file = fopen(FILE_NAME, "r+b");
    if (!file) return 0;

    Item temp;
    while (fread(&temp, sizeof(Item), 1, file) == 1) {
        if (temp.id == id && temp.is_deleted == 0) {
            fseek(file, -(long)sizeof(Item), SEEK_CUR);
            fwrite(updated, sizeof(Item), 1, file);
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int delete_item(int id) {
    FILE* file = fopen(FILE_NAME, "r+b");
    if (!file) return 0;

    Item temp;
    while (fread(&temp, sizeof(Item), 1, file) == 1) {
        if (temp.id == id && temp.is_deleted == 0) {
            temp.is_deleted = 1;
            fseek(file, -(long)sizeof(Item), SEEK_CUR);
            fwrite(&temp, sizeof(Item), 1, file);
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int list_items(Item* buffer, int max_items) {
    FILE* file = fopen(FILE_NAME, "rb");
    if (!file) return 0;

    int count = 0;
    Item temp;
    while (fread(&temp, sizeof(Item), 1, file) == 1 && count < max_items) {
        if (temp.is_deleted == 0) {
            buffer[count++] = temp;
        }
    }

    fclose(file);
    return count;
}
