#include <stdio.h>
#include <string.h>

#define MAX_MEDIA 100

// ---- Data structure ----
typedef struct {
    int id;
    char title[100];
    char artist[100];
    char genre[50];
    int year;
} Media;

Media library[MAX_MEDIA];
int count = 0;

// ---- Small helper: remove '\n' from fgets ----
void removeNewline(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\n') {
            str[i] = '\0';
            break;
        }
        i++;
    }
}

// ---- Batch import from CSV ----
// Very simple parsing: fields must not contain commas.
void loadCSV(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Could not open file: %s\n", filename);
        return;
    }

    char line[300];

    // Skip header line
    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        printf("Empty file.\n");
        return;
    }

    while (fgets(line, sizeof(line), fp) && count < MAX_MEDIA) {
        Media m;
        if (sscanf(line, "%d,%99[^,],%99[^,],%49[^,],%d",
                   &m.id, m.title, m.artist, m.genre, &m.year) == 5) {
            library[count++] = m;
        }
    }

    fclose(fp);
    printf("Loaded %d items from CSV.\n", count);
}

// ---- Add media ----
void addMedia() {
    if (count >= MAX_MEDIA) {
        printf("Library full.\n");
        return;
    }

    Media m;

    printf("Enter ID: ");
    scanf("%d", &m.id);
    getchar(); // remove leftover '\n'

    printf("Enter title: ");
    fgets(m.title, 100, stdin);
    removeNewline(m.title);

    printf("Enter artist: ");
    fgets(m.artist, 100, stdin);
    removeNewline(m.artist);

    printf("Enter genre: ");
    fgets(m.genre, 50, stdin);
    removeNewline(m.genre);

    printf("Enter year: ");
    scanf("%d", &m.year);

    library[count++] = m;
    printf("Media added.\n");
}

// ---- Display all media ----
void displayMedia() {
    if (count == 0) {
        printf("No media in library.\n");
        return;
    }

    printf("\n--- Media Library ---\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d | Title: %s | Artist: %s | Genre: %s | Year: %d\n",
               library[i].id,
               library[i].title,
               library[i].artist,
               library[i].genre,
               library[i].year);
    }
}

// ---- Edit media by ID ----
void editMedia() {
    if (count == 0) {
        printf("No media to edit.\n");
        return;
    }

    int id;
    printf("Enter ID of media to edit: ");
    scanf("%d", &id);
    getchar(); // remove '\n'

    int index = -1;
    for (int i = 0; i < count; i++) {
        if (library[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Media with ID %d not found.\n", id);
        return;
    }

    Media *m = &library[index];

    printf("Editing media with ID %d\n", m->id);
    printf("Current title: %s\n", m->title);
    printf("Enter new title (or press Enter to keep same): ");
    char buffer[100];
    fgets(buffer, 100, stdin);
    removeNewline(buffer);
    if (buffer[0] != '\0') strcpy(m->title, buffer);

    printf("Current artist: %s\n", m->artist);
    printf("Enter new artist (or press Enter to keep same): ");
    fgets(buffer, 100, stdin);
    removeNewline(buffer);
    if (buffer[0] != '\0') strcpy(m->artist, buffer);

    printf("Current genre: %s\n", m->genre);
    printf("Enter new genre (or press Enter to keep same): ");
    fgets(buffer, 50, stdin);
    removeNewline(buffer);
    if (buffer[0] != '\0') strcpy(m->genre, buffer);

    printf("Current year: %d\n", m->year);
    printf("Enter new year (or 0 to keep same): ");
    int newYear;
    scanf("%d", &newYear);
    if (newYear != 0) m->year = newYear;

    printf("Media updated.\n");
}

// ---- Delete media by ID ----
void deleteMedia() {
    if (count == 0) {
        printf("No media to delete.\n");
        return;
    }

    int id;
    printf("Enter ID of media to delete: ");
    scanf("%d", &id);

    int index = -1;
    for (int i = 0; i < count; i++) {
        if (library[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Media with ID %d not found.\n", id);
        return;
    }

    // Shift all later items left by one
    for (int i = index; i < count - 1; i++) {
        library[i] = library[i + 1];
    }
    count--;

    printf("Media deleted.\n");
}

// ---- Search (exact or substring) ----
void searchTitle() {
    if (count == 0) {
        printf("No media to search.\n");
        return;
    }

    int type;
    printf("\nSearch type:\n");
    printf("1. Exact title\n");
    printf("2. Substring in title\n");
    printf("Enter choice: ");
    scanf("%d", &type);
    getchar(); // remove '\n'

    char query[100];
    printf("Enter title to search: ");
    fgets(query, 100, stdin);
    removeNewline(query);

    int found = 0;
    printf("\n--- Search Results ---\n");

    for (int i = 0; i < count; i++) {
        int match = 0;
        if (type == 1) { // exact
            if (strcmp(library[i].title, query) == 0)
                match = 1;
        } else {         // substring
            if (strstr(library[i].title, query) != NULL)
                match = 1;
        }

        if (match) {
            found = 1;
            printf("ID: %d | Title: %s | Artist: %s | Genre: %s | Year: %d\n",
                   library[i].id,
                   library[i].title,
                   library[i].artist,
                   library[i].genre,
                   library[i].year);
        }
    }

    if (!found)
        printf("No matches found.\n");
}

// ---- Main menu ----
int main() {
    int choice;

    while (1) {
        printf("\nMedia Library Menu\n");
        printf("1. Load from CSV (batch import)\n");
        printf("2. Add media\n");
        printf("3. Edit media\n");
        printf("4. Delete media\n");
        printf("5. Display all media\n");
        printf("6. Search by title (exact / substring)\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                loadCSV("media.csv");
                break;
            case 2:
                addMedia();
                break;
            case 3:
                editMedia();
                break;
            case 4:
                deleteMedia();
                break;
            case 5:
                displayMedia();
                break;
            case 6:
                searchTitle();
                break;
            case 7:
                return 0;
            default:
                printf("Invalid option.\n");
        }
    }
}
