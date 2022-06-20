#include <stdio.h>
#include <time.h>

typedef struct Ticket {
    int flight;
    char destination[32];
    struct tm ticket_date;
    int cost;
} table;

table Table[64];

size_t amount;

void print_table() {
    printf("ID | № | Destination | Departure date | Cost\n");
    for (size_t i = 0; i < amount; ++i) {
        printf("%zu | %d | %s | %02d.%02d.%d %02d:%02d | %d", i + 1, Table[i].flight, Table[i].destination,
               Table[i].ticket_date.tm_mday, Table[i].ticket_date.tm_mon + 1, Table[i].ticket_date.tm_year + 1900,
               Table[i].ticket_date.tm_hour, Table[i].ticket_date.tm_min, Table[i].cost);
        printf("\n");
    }
    printf("\n");
}

void add() {
    if (amount == 63) {
        printf("Table is full");
        return;
    }
    int day, month, year, hour, minute;
    printf("Enter flight number: ");
    scanf("%d", &Table[amount].flight);
    printf("Enter the destination (ex. Novosibirsk, no more than 32 chars): ");
    scanf("%32s", Table[amount].destination);
    printf("Enter departure date (ex. 01.01.1970 00:00): ");
    scanf("%d.%d.%d %d:%d", &day, &month, &year, &hour, &minute);
    Table[amount].ticket_date.tm_mday = day;
    Table[amount].ticket_date.tm_mon = month - 1;
    Table[amount].ticket_date.tm_year = year - 1900;
    Table[amount].ticket_date.tm_hour = hour;
    Table[amount].ticket_date.tm_min = minute;
    printf("Enter ticket cost: ");
    scanf("%d", &Table[amount].cost);
    printf("\n");
    amount++;
}

void del() {
    size_t i;
    print_table();
    printf("Select the line of the table\n");
    scanf("%1zu", &i);
    if (i < 1 || i > amount) {
        printf("Element with number %zu doesn't exists\n", i);
        return;
    }
    for (i--; i < amount - 1; ++i) {
        Table[i] = Table[i + 1];
    }
    amount--;
}

void save() {
    FILE *file;
    if ((file = fopen("table.dat", "wt")) != NULL) {
        fprintf(file, "%zu\n", amount);
        for (size_t i = 0; i < amount; ++i) {
            fprintf(file, "%d %s %d.%d.%d %d:%d %d\n", Table[i].flight, Table[i].destination,
                    Table[i].ticket_date.tm_mday, Table[i].ticket_date.tm_mon, Table[i].ticket_date.tm_year,
                    Table[i].ticket_date.tm_hour, Table[i].ticket_date.tm_min, Table[i].cost);
        }
        fclose(file);
    } else {
        printf("ERROR");
        return;
    }
}

void load() {
    FILE *file;
    if ((file = fopen("table.dat", "rt")) != NULL) {
        fscanf(file, "%zu", &amount);
        for (size_t i = 0; i < amount; ++i) {
            fscanf(file, "%d %s %d.%d.%d %d:%d %d", &Table[i].flight, Table[i].destination,
                   &Table[i].ticket_date.tm_mday, &Table[i].ticket_date.tm_mon, &Table[i].ticket_date.tm_year,
                   &Table[i].ticket_date.tm_hour, &Table[i].ticket_date.tm_min, &Table[i].cost);
        }
        fclose(file);
    } else {
        printf("ERROR");
    }
}

size_t sort() {
    size_t flag = 0;
    while (flag == 0) {
        printf("[1] Sort by time\n"
               "[2] Sort by price\n");
        scanf("%1zu", &flag);
    }
    return flag;
}

int total_price() {
    int sum = 0;
    for (size_t i = 0; i < amount; ++i) {
        sum += Table[i].cost;
    }
    return sum;
}

void sort_by_price() {
    table buffer;
    for (size_t i = amount - 1; i > 0; --i) {
        for (size_t j = 0; j < i; ++j) {
            if (Table[j].cost > Table[j + 1].cost) {
                buffer = Table[j];
                Table[j] = Table[j + 1];
                Table[j + 1] = buffer;
            }
        }
    }
}

void sort_by_time() {
    table buffer;
    for (size_t i = amount - 1; i > 0; --i) {
        for (size_t j = 0; j < i; ++j) {
            if (mktime(&Table[j].ticket_date) > mktime(&Table[j + 1].ticket_date)) {
                buffer = Table[j];
                Table[j] = Table[j + 1];
                Table[j + 1] = buffer;
            }
        }
    }
}

size_t change_type() {
    size_t flag = 0;
    while (flag == 0) {
        printf("[1] Change flight number\n"
               "[2] Change the destination\n"
               "[3] Change departure date\n"
               "[4] Change ticket cost\n");
        scanf("%1zu", &flag);
        printf("\n");
    }
    return flag;
}

void change() {
    size_t i;
    size_t type;
    print_table();
    printf("Select the line of the table\n");
    scanf("%1zu", &i);
    if (i < 1 || i > amount) {
        printf("Element with number %zu doesn't exist\n");
        return;
    }
    i--;
    type = change_type();
    switch (type) {
        case 1: {
            printf("Enter flight number: ");
            scanf("%d", &Table[i].flight);
            break;
        }
        case 2: {
            printf("Enter the destination (ex. Novosibirsk): ");
            scanf("%s", Table[i].destination);
            break;
        }
        case 3: {
            int day, month, year, hour, minute;
            printf("Enter departure date (ex. 01.01.1970 00:00): ");
            scanf("%d.%d.%d %d:%d", &day, &month, &year, &hour, &minute);
            Table[i].ticket_date.tm_mday = day;
            Table[i].ticket_date.tm_mon = month - 1;
            Table[i].ticket_date.tm_year = year - 1900;
            Table[i].ticket_date.tm_hour = hour;
            Table[i].ticket_date.tm_min = minute;
            break;
        }
        case 4: {
            printf("Enter ticket cost: ");
            scanf("%d", &Table[i].cost);
            break;
        }
        case 5:
            break;
    }
}

size_t menu() {
    size_t flag = 0;
    while (flag == 0) {
        printf("[1] Add\n"
               "[2] Save\n"
               "[3] Load\n"
               "[4] Print\n"
               "[5] Change\n"
               "[6] Delete\n"
               "[7] Total ticket price\n"
               "[8] Sort\n"
               "[9] Exit");
        scanf("%1zu", &flag);
        printf("\n");
    }
    return flag;
}


int main() {
    size_t selection;
    size_t sort_type;
    amount = 0;
    while ((selection = menu()) != 0) {
        switch (selection) {
            case 1: {
                add();
                break;
            }
            case 2: {
                save();
                break;
            }
            case 3: {
                load();
                break;
            }
            case 4: {
                print_table();
                break;
            }
            case 5: {
                change();
                break;
            }
            case 6: {
                del();
                break;
            }
            case 7: {
                printf("All ticket prices: %d\n", total_price());
                break;
            }
            case 8: {
                if (amount == 0) {
                    printf("Table is empty\n");
                    break;
                }
                sort_type = sort();
                switch (sort_type) {
                    case 1: {
                        sort_by_time();
                        break;
                    }
                    case 2: {
                        sort_by_price();
                        break;
                    }
                }
                break;
            }
            case 9:
                return 0;
        }
    }
    return 0;
}
