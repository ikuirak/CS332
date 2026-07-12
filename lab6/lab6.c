#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        if (line[0] == '\0')
            continue;
        list_items[count++] = getfields(line);
    }
    fclose(fptr);

    printf("Read %d listings\n", count);
    for (int i = 0; i < count; i++)
        displayStruct(&list_items[i]);

    return 0;
}