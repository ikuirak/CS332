#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LINESIZE     1024
#define MAX_LISTINGS 50000

struct listing {
    int   id, host_id, minimum_nights, number_of_reviews,
          calculated_host_listings_count, availability_365;
    char *host_name, *neighbourhood_group, *neighbourhood, *room_type;
    float latitude, longitude, price;
};

/* parse one CSV line with strtok (first call gets the line,
   the rest get NULL to continue from where it stopped) */
struct listing getfields(char *line)
{
    struct listing item;

    item.id                  = atoi(strtok(line, ","));
    item.host_id             = atoi(strtok(NULL, ","));
    item.host_name           = strdup(strtok(NULL, ","));
    item.neighbourhood_group = strdup(strtok(NULL, ","));
    item.neighbourhood       = strdup(strtok(NULL, ","));
    item.latitude            = atof(strtok(NULL, ","));
    item.longitude           = atof(strtok(NULL, ","));
    item.room_type           = strdup(strtok(NULL, ","));
    item.price               = atof(strtok(NULL, ","));
    item.minimum_nights      = atoi(strtok(NULL, ","));
    item.number_of_reviews   = atoi(strtok(NULL, ","));
    item.calculated_host_listings_count = atoi(strtok(NULL, ","));
    item.availability_365    = atoi(strtok(NULL, ","));

    return item;
}

void displayStruct(const struct listing *item)
{
    printf("%d %s %s $%.2f\n", item->id, item->host_name,
           item->room_type, item->price);
}

void write_listing(FILE *fp, const struct listing *item)
{
    fprintf(fp, "%d,%d,%s,%s,%s,%.5f,%.5f,%s,%.2f,%d,%d,%d,%d\n",
            item->id, item->host_id, item->host_name,
            item->neighbourhood_group, item->neighbourhood,
            item->latitude, item->longitude, item->room_type,
            item->price, item->minimum_nights, item->number_of_reviews,
            item->calculated_host_listings_count, item->availability_365);
}

int compare_host_name(const void *a, const void *b)
{
    const struct listing *la = a;
    const struct listing *lb = b;
    return strcmp(la->host_name, lb->host_name);
}

int compare_price(const void *a, const void *b)
{
    const struct listing *la = a;
    const struct listing *lb = b;
    if (la->price < lb->price) return -1;
    if (la->price > lb->price) return 1;
    return 0;
}

int main(void)
{
    static struct listing list_items[MAX_LISTINGS];
    char line[LINESIZE];
    int count = 0;

    FILE *fptr = fopen("listings.csv", "r");
    if (fptr == NULL) {
        fprintf(stderr, "Error: could not open listings.csv\n");
        return 1;
    }

    while (fgets(line, LINESIZE, fptr) != NULL) {
        line[strcspn(line, "\r\n")] = '\0';   /* strip newline */
        if (count == 0 && !isdigit((unsigned char)line[0]))
            continue;    /* skip header row */
        if (line[0] == '\0')
            continue;
        list_items[count++] = getfields(line);
    }
    fclose(fptr);

    qsort(list_items, count, sizeof(struct listing), compare_host_name);
    FILE *out = fopen("sorted_by_host_name.csv", "w");   /* write mode */
    if (out == NULL) { fprintf(stderr, "Error opening output\n"); return 1; }
    for (int i = 0; i < count; i++)
        write_listing(out, &list_items[i]);
    fclose(out);

   qsort(list_items, count, sizeof(struct listing), compare_price);
    out = fopen("sorted_by_price.csv", "w");
    if (out == NULL) { fprintf(stderr, "Error opening output\n"); return 1; }
    for (int i = 0; i < count; i++)
        write_listing(out, &list_items[i]);
    fclose(out);

    printf("Read %d listings\n", count);
    for (int i = 0; i < count; i++)
        displayStruct(&list_items[i]);

    return 0;
}