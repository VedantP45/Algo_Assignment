#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorting_Algo.h"


#define MAX_LIMIT 35000

GtkWidget *entry_dataset_size;
GtkWidget *check_quick_sort, *check_insertion_sort, *check_radix_sort;
GtkWidget *check_bubble_sort, *check_merge_sort, *check_heap_sort, *check_selection_sort;

// Function to generate unique random data and save it to a file
void on_generate_data_clicked(GtkWidget *widget, gpointer data) {
    int dataset[MAX_LIMIT];
    int used_numbers[MAX_LIMIT * 10] = {0};  // Larger array to avoid exceeding bounds
    
    // Get the dataset size from the entry field
    const char *dataset_size_text = gtk_entry_get_text(GTK_ENTRY(entry_dataset_size));
    int size = atoi(dataset_size_text);

    if (size <= 0 || size > MAX_LIMIT) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_CLOSE,
                "Invalid size! Please enter a value between 1 and %d.", MAX_LIMIT);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Generate unique random numbers
    srand(time(0));
    for (int i = 0; i < size; i++) {
        int num;
        do {
            num = rand() % (MAX_LIMIT * 10);  // Larger range to avoid collisions
        } while (num >= MAX_LIMIT * 10 || used_numbers[num]);  // Ensure 'num' is within bounds
        used_numbers[num] = 1;
        dataset[i] = num;
    }

    // Open file to save data
    FILE *file = fopen("unique_random_data.csv", "w");
    if (file == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_CLOSE,
                "Error: Could not open 'unique_random_data.csv' for writing.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Write unique random numbers to file
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d\n", dataset[i]);
    }
    fclose(file);

    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_INFO,
                GTK_BUTTONS_CLOSE,
                "Unique random data generated and saved to 'unique_random_data.csv'.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Function to sort data using selected algorithms

// Function to sort data using selected algorithms and capture execution time in milliseconds
void on_sort_button_clicked(GtkWidget *widget, gpointer data) {
    int dataset[MAX_LIMIT];
    int size = 0;

    // Load dataset from file
    FILE *file = fopen("unique_random_data.csv", "r");
    if (file == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_CLOSE,
                "Error: Could not open 'unique_random_data.csv'. Please generate data first.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Load data into the dataset array
    while (fscanf(file, "%d", &dataset[size]) != EOF && size < MAX_LIMIT) {
        size++;
    }
    fclose(file);

    // Open file to store sorting times
    FILE *time_file = fopen("sorting_times.csv", "w");
    if (time_file == NULL) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_CLOSE,
                "Error: Could not open 'sorting_times.csv' for writing.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    fprintf(time_file, "Algorithm,Time (milliseconds)\n");  // Write the header

    // Variables for time measurement
    clock_t start, end;
    double time_taken_ms;  // Time taken in milliseconds

    // Apply selected sorting algorithms and capture time in milliseconds
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_quick_sort))) {
        start = clock();
        quick_sort(dataset, 0, size - 1);
        end = clock();
        time_taken_ms = ((double)(end - start)) * 1000.0 / CLOCKS_PER_SEC;
        fprintf(time_file, "Quick Sort,%.6f\n", time_taken_ms);
    }

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_insertion_sort))) {
        start = clock();
        insertion_sort(dataset, size);
        end = clock();
        time_taken_ms = ((double)(end - start)) * 1000.0 / CLOCKS_PER_SEC;
        fprintf(time_file, "Insertion Sort,%.6f\n", time_taken_ms);
    }

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_radix_sort))) {
        start = clock();
        radix_sort(dataset, size);
        end = clock();
        time_taken_ms = ((double)(end - start)) * 1000.0 / CLOCKS_PER_SEC;
        fprintf(time_file, "Radix Sort,%.6f\n", time_taken_ms);
    }

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_bubble_sort))) {
        start = clock();
        bubble_sort(dataset, size);
        end = clock();
        time_taken_ms = ((double)(end - start)) * 1000.0 / CLOCKS_PER_SEC;
        fprintf(time_file, "Bubble Sort,%.6f\n", time_taken_ms);
    }

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_merge_sort))) {
        start = clock();
        merge_sort(dataset, 0, size - 1);
        end = clock();
        time_taken_ms = ((double)(end - start)) * 1000.0 / CLOCKS_PER_SEC;
        fprintf(time_file, "Merge Sort,%.6f\n", time_taken_ms);
    }

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_heap_sort))) {
        start = clock();
        heap_sort(dataset, size);
        end = clock();
        time_taken_ms = ((double)(end - start)) * 1000.0 / CLOCKS_PER_SEC;
        fprintf(time_file, "Heap Sort,%.6f\n", time_taken_ms);
    }

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_selection_sort))) {
        start = clock();
        selection_sort(dataset, size);
        end = clock();
        time_taken_ms = ((double)(end - start)) * 1000.0 / CLOCKS_PER_SEC;
        fprintf(time_file, "Selection Sort,%.6f\n", time_taken_ms);
    }

    fclose(time_file);

    // Save the sorted dataset to a file
    FILE *output_file = fopen("sorted_unique_data.csv", "w");
    for (int i = 0; i < size; i++) {
        fprintf(output_file, "%d\n", dataset[i]);
    }
    fclose(output_file);

    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_INFO,
                GTK_BUTTONS_CLOSE,
                "Data sorted, times saved to 'sorting_times.csv', and sorted data saved to 'sorted_unique_data.csv'.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}


int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *generate_button, *sort_button;
    GtkWidget *label_dataset_size;

    gtk_init(&argc, &argv);

    // Create a new window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Sorting Algorithms");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a grid layout
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Entry for dataset size
    label_dataset_size = gtk_label_new("Enter Dataset Size (1 to 5000):");
    gtk_grid_attach(GTK_GRID(grid), label_dataset_size, 0, 0, 1, 1);

    entry_dataset_size = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_dataset_size, 1, 0, 1, 1);

    // Checkboxes for sorting algorithm selection
    check_quick_sort = gtk_check_button_new_with_label("Quick Sort");
    gtk_grid_attach(GTK_GRID(grid), check_quick_sort, 0, 1, 1, 1);

    check_insertion_sort = gtk_check_button_new_with_label("Insertion Sort");
    gtk_grid_attach(GTK_GRID(grid), check_insertion_sort, 1, 1, 1, 1);

    check_radix_sort = gtk_check_button_new_with_label("Radix Sort");
    gtk_grid_attach(GTK_GRID(grid), check_radix_sort, 0, 2, 1, 1);

    check_bubble_sort = gtk_check_button_new_with_label("Bubble Sort");
    gtk_grid_attach(GTK_GRID(grid), check_bubble_sort, 1, 2, 1, 1);

    check_merge_sort = gtk_check_button_new_with_label("Merge Sort");
    gtk_grid_attach(GTK_GRID(grid), check_merge_sort, 0, 3, 1, 1);

    check_heap_sort = gtk_check_button_new_with_label("Heap Sort");
    gtk_grid_attach(GTK_GRID(grid), check_heap_sort, 1, 3, 1, 1);

    check_selection_sort = gtk_check_button_new_with_label("Selection Sort");
    gtk_grid_attach(GTK_GRID(grid), check_selection_sort, 0, 4, 1, 1);

    // Button to generate unique random data
    generate_button = gtk_button_new_with_label("Generate Unique Random Data");
    g_signal_connect(generate_button, "clicked", G_CALLBACK(on_generate_data_clicked), window);
    gtk_grid_attach(GTK_GRID(grid), generate_button, 0, 5, 2, 1);

    // Button to sort data
    sort_button = gtk_button_new_with_label("Sort Data");
    g_signal_connect(sort_button, "clicked", G_CALLBACK(on_sort_button_clicked), window);
    gtk_grid_attach(GTK_GRID(grid), sort_button, 0, 6, 2, 1);

    // Show all widgets
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
