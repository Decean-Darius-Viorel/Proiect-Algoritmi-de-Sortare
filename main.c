#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// ===================== SWAP =====================
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// ===================== BUBBLE SORT =====================
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
}

// ===================== SELECTION SORT =====================
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j] < arr[min])
                min = j;
        swap(&arr[i], &arr[min]);
    }
}

// ===================== INSERTION SORT =====================
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// ===================== MERGE SORT (FIXED - HEAP SAFE) =====================
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    // 🔥 FIX: heap memory (nu stack)
    int *L = (int*)malloc(n1 * sizeof(int));
    int *R = (int*)malloc(n2 * sizeof(int));

    if (!L || !R) {
        printf("Eroare memorie!\n");
        exit(1);
    }

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];

    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = R[j++];

    free(L);
    free(R);
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// ===================== QUICK SORT =====================
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// ===================== HEAP SORT =====================
void heapify(int arr[], int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l] > arr[largest])
        largest = l;

    if (r < n && arr[r] > arr[largest])
        largest = r;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

// ===================== FIȘIER =====================
void saveToFile(const char *path, int arr[], int n) {
    FILE *f = fopen(path, "w");
    if (!f) {
        printf("Eroare fisier!\n");
        return;
    }

    for (int i = 0; i < n; i++)
        fprintf(f, "%d ", arr[i]);

    fclose(f);
}

int* loadFromFile(const char *path, int *n) {
    FILE *f = fopen(path, "r");
    if (!f) return NULL;

    int cap = 100;
    int *arr = malloc(cap * sizeof(int));
    *n = 0;

    while (fscanf(f, "%d", &arr[*n]) == 1) {
        (*n)++;
        if (*n >= cap) {
            cap *= 2;
            arr = realloc(arr, cap * sizeof(int));
        }
    }

    fclose(f);
    return arr;
}

// ===================== TIMP =====================
void measure(void (*sortFunc)(int*, int), int arr[], int n, const char *name) {
    int *copy = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
        copy[i] = arr[i];

    clock_t start = clock();
    sortFunc(copy, n);
    clock_t end = clock();

    printf("%s: %.6f secunde\n",
           name,
           (double)(end - start) / CLOCKS_PER_SEC);

    free(copy);
}

// wrappers
void mergeWrapper(int arr[], int n) { mergeSort(arr, 0, n - 1); }
void quickWrapper(int arr[], int n) { quickSort(arr, 0, n - 1); }

// ===================== MAIN =====================
int main() {
    int opt, n;
    int *arr = NULL;

    // 🔥 Desktop automat
    char filePath[300];
    const char *user = getenv("USERPROFILE");

    if (!user) {
        printf("Eroare USERPROFILE!\n");
        return 1;
    }

    snprintf(filePath, sizeof(filePath),
             "%s\\Desktop\\data.txt",
             user);

    while (1) {
        printf("\n1.Genereaza\n2.Incarca\n3.Sorteaza\n0.Iesire\nOptiune: ");
        scanf("%d", &opt);

        if (opt == 0) break;

        if (opt == 1) {
            printf("Numar elemente: ");
            scanf("%d", &n);

            arr = malloc(n * sizeof(int));
            srand(time(NULL));

            for (int i = 0; i < n; i++)
                arr[i] = rand() % 100000;

            saveToFile(filePath, arr, n);

            printf("Salvat: %s\n", filePath);
            free(arr);
        }

        else if (opt == 2) {
            arr = loadFromFile(filePath, &n);

            if (!arr)
                printf("Eroare fisier!\n");
            else
                printf("Incarcat %d elemente\n", n);
        }

        else if (opt == 3) {
            if (!arr) {
                printf("Nu ai date!\n");
                continue;
            }

            printf("\n--- TIMPI EXECUTIE ---\n");

            measure(bubbleSort, arr, n, "Bubble");
            measure(selectionSort, arr, n, "Selection");
            measure(insertionSort, arr, n, "Insertion");
            measure(mergeWrapper, arr, n, "Merge");
            measure(quickWrapper, arr, n, "Quick");
            measure(heapSort, arr, n, "Heap");
        }
    }

    if (arr) free(arr);
    return 0;
}