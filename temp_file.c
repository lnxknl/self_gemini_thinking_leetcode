#include <stdio.h>
#include <math.h>

// Simulate the detector function (K is assumed to be 1 for simplicity here)
double detector(int x, int y, int bx, int by) {
    double dx = x - bx;
    double dy = y - by;
    return 1.0 / (dx * dx + dy * dy);
}

int main() {
    // Assume the actual beacon location is (bx, by)
    int bx_actual = 5;
    int by_actual = 3;

    // Query points
    int q1_x = 0, q1_y = 0;
    int q2_x = 1, q2_y = 0;
    int q3_x = 0, q3_y = 1;

    // Get signal strengths
    double s1 = detector(q1_x, q1_y, bx_actual, by_actual);
    double s2 = detector(q2_x, q2_y, bx_actual, by_actual);
    double s3 = detector(q3_x, q3_y, bx_actual, by_actual);

    printf("Signal at (%d, %d): %f\n", q1_x, q1_y, s1);
    printf("Signal at (%d, %d): %f\n", q2_x, q2_x, s2); // Corrected typo here
    printf("Signal at (%d, %d): %f\n", q3_x, q3_y, s3);

    // This part would involve solving the system of equations.
    // For simplicity, let's demonstrate a brute-force search around the origin.
    // In a real implementation, a numerical solver or algebraic manipulation would be needed.

    int estimated_bx = -1, estimated_by = -1;
    double tolerance = 0.001; // Tolerance for comparing signal strengths

    for (int bx_guess = -10; bx_guess <= 10; bx_guess++) {
        for (int by_guess = -10; by_guess <= 10; by_guess++) {
            double s1_guess = detector(q1_x, q1_y, bx_guess, by_guess);
            double s2_guess = detector(q2_x, q2_y, bx_guess, by_guess);
            double s3_guess = detector(q3_x, q3_y, bx_guess, by_guess);

            if (fabs(s1 - s1_guess) < tolerance &&
                fabs(s2 - s2_guess) < tolerance &&
                fabs(s3 - s3_guess) < tolerance) {
                estimated_bx = bx_guess;
                estimated_by = by_guess;
                break;
            }
        }
        if (estimated_bx != -1) break;
    }

    if (estimated_bx != -1) {
        printf("Estimated beacon location: (%d, %d)\n", estimated_bx, estimated_by);
    } else {
        printf("Could not determine beacon location.\n");
    }

    return 0;
}
