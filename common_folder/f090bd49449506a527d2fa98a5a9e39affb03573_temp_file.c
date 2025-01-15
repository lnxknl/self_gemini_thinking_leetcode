   #include <stdio.h>
   #include <stdlib.h>

   typedef struct {
       int row;
       int col;
   } Coordinates;

   Coordinates findLostSignal(int** grid, int rows, int cols) {
       Coordinates result = {-1, -1}; // Initialize to indicate not found

       // Recursive helper function for divide and conquer
       Coordinates findSignalRecursive(int** subgrid, int startRow, int startCol, int numRows, int numCols) {
           if (numRows == 1 && numCols == 1) {
               if (subgrid[0][0] == 1) {
                   return (Coordinates){startRow, startCol};
               } else {
                   return (Coordinates){-1, -1};
               }
           }

           int midRow = numRows / 2;
           int midCol = numCols / 2;

           // Quadrant 1 (Top-Left)
           int sumQ1 = 0;
           for (int i = 0; i < midRow; i++) {
               for (int j = 0; j < midCol; j++) {
                   sumQ1 += subgrid[i][j];
               }
           }
           if (sumQ1 == 1) {
               return findSignalRecursive(subgrid, startRow, startCol, midRow, midCol);
           }

           // Quadrant 2 (Top-Right)
           int sumQ2 = 0;
           for (int i = 0; i < midRow; i++) {
               for (int j = midCol; j < numCols; j++) {
                   sumQ2 += subgrid[i][j];
               }
           }
           if (sumQ2 == 1) {
               return findSignalRecursive(&subgrid[0][midCol], startRow, startCol + midCol, midRow, numCols - midCol);
           }

           // Quadrant 3 (Bottom-Left)
           int sumQ3 = 0;
           for (int i = midRow; i < numRows; i++) {
               for (int j = 0; j < midCol; j++) {
                   sumQ3 += subgrid[i][j];
               }
           }
           if (sumQ3 == 1) {
               return findSignalRecursive(&subgrid[midRow][0], startRow + midRow, startCol, numRows - midRow, midCol);
           }

           // Quadrant 4 (Bottom-Right)
           int sumQ4 = 0;
           for (int i = midRow; i < numRows; i++) {
               for (int j = midCol; j < numCols; j++) {
                   sumQ4 += subgrid[i][j];
               }
           }
           if (sumQ4 == 1) {
               return findSignalRecursive(&subgrid[midRow][midCol], startRow + midRow, startCol + midCol, numRows - midRow, numCols - midCol);
           }

           return (Coordinates){-1, -1}; // Should not happen if signal exists
       }

       return findSignalRecursive(grid, 0, 0, rows, cols);
   }

   int main() {
       // Test Example Input
       int rows = 10;
       int cols = 15;
       int** grid = (int**)malloc(rows * sizeof(int*));
       for (int i = 0; i < rows; i++) {
           grid[i] = (int*)malloc(cols * sizeof(int));
           for (int j = 0; j < cols; j++) {
               grid[i][j] = 0;
           }
       }

       // Place the lost signal at a specific location
       grid[3][7] = 1;

       Coordinates signalLocation = findLostSignal(grid, rows, cols);

       if (signalLocation.row != -1) {
           printf("Lost signal found at row: %d, col: %d\n", signalLocation.row, signalLocation.col);
       } else {
           printf("Lost signal not found.\n");
       }

       // Free allocated memory
       for (int i = 0; i < rows; i++) {
           free(grid[i]);
       }
       free(grid);

       return 0;
   }
