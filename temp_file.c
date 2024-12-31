#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function to compare integers for sorting
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Function to apply a median filter to a pixel
int median_filter(int **image, int rows, int cols, int r, int c) {
    int neighborhood[9]; // For a 3x3 neighborhood
    int k = 0;

    for (int i = r - 1; i <= r + 1; i++) {
        for (int j = c - 1; j <= c + 1; j++) {
            if (i >= 0 && i < rows && j >= 0 && j < cols) {
                neighborhood[k++] = image[i][j];
            }
        }
    }

    qsort(neighborhood, k, sizeof(int), compare);

    if (k > 0) {
        return neighborhood[k / 2];
    } else {
        return image[r][c]; // Return original if no neighbors
    }
}

// Function to restore the degraded image using a median filter
int** restore_image(int **degradedImage, int rows, int cols) {
    int **restoredImage = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        restoredImage[i] = (int *)malloc(cols * sizeof(int));
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            restoredImage[i][j] = median_filter(degradedImage, rows, cols, i, j);
        }
    }

    return restoredImage;
}

// Function to print the image
void print_image(int **image, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%3d ", image[i][j]);
        }
        printf("\n");
    }
}

int main() {
    // Test Example Input
    int rows = 5;
    int cols = 5;
    int **degradedImage = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        degradedImage[i] = (int *)malloc(cols * sizeof(int));
    }

    // Simulate a small degraded image
    int initial_image[5][5] = {
        {100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100},
        {100, 100, 200, 100, 100},
        {100, 100, 100, 100, 100},
        {100, 100, 100, 100, 100}
    };

    // Add some random noise
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            degradedImage[i][j] = initial_image[i][j] + (rand() % 50 - 25); // Add noise between -25 and 24
            if (degradedImage[i][j] < 0) degradedImage[i][j] = 0;
            if (degradedImage[i][j] > 255) degradedImage[i][j] = 255;
        }
    }

    printf("Degraded Image:\n");
    print_image(degradedImage, rows, cols);

    int **restoredImage = restore_image(degradedImage, rows, cols);

    printf("\nRestored Image:\n");
    print_image(restoredImage, rows, cols);

    // Free allocated memory
    for (int i = 0; i < rows; i++) {
        free(degradedImage[i]);
        free(restoredImage[i]);
    }
    free(degradedImage);
    free(restoredImage);

    return 0;
}
