#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "genann.h"

/* NOTES:

    TO DO:

        copy all this in the elka_nn folder and make sure it still runs

        make execution loop vvv

        while True:
            if new data
                get loss of old data
                update weights,
                set past data to new data
                update plot display of error data

        make fake flight data


            inputs:
                position error
                velocity error
                motor rpm
            outputs:
                next position error

            give it a pre-determined flight


 */

//#ifdef _MSC_VER
//#define _CRT_SECURE_NO_WARNINGS

const char *iris_data = "example/iris.data";

double *input = NULL, *class = NULL;
int samples = 0, num_training_samples = 0, num_testing_samples = 0;
const char *class_names[] = {"Iris-setosa", "Iris-versicolor", "Iris-virginica"};

void load_data() {

    /* Load the iris data-set. */
    FILE *in = fopen("example/iris.data", "r");
    if (!in) {
        printf("Could not open file: %s\n", iris_data);
        exit(1);
    }

    /* Loop through the data to get a count. */
    char line[1024];
    while (!feof(in) && fgets(line, 1024, in)) {
        ++samples;
    }
    fseek(in, 0, SEEK_SET);

    printf("Loading %d data points from %s\n", samples, iris_data);

    /* Allocate memory for input and output data.
    input = malloc(sizeof(double) * samples * 4);
    class = malloc(sizeof(double) * samples * 3);
    */
    float x = 0.75;
    num_training_samples = (int)round(x * samples);
    num_testing_samples = (int)round((1.0 - x) * samples);
    input = malloc(sizeof(double) * samples * 4);
    class = malloc(sizeof(double) * samples * 3);


    /* Read the file into our arrays. */
    printf("TRAINING DATA\n");
    int i, j;
    for (i = 0; i < samples; ++i) {
        double *p = input + i * 4;
        double *c = class + i * 3;
        c[0] = c[1] = c[2] = 0.0;

        if (fgets(line, 1024, in) == NULL) {
            perror("fgets");
            exit(1);
        }

        char *split = strtok(line, ",");
        for (j = 0; j < 4; ++j) {
            p[j] = atof(split);
            split = strtok(0, ",");
        }

        split[strlen(split)-1] = 0;
        split[strlen(split)-1] = 0;
        if (strcmp(split, class_names[0]) == 0) {c[0] = 1.0;}
        else if (strcmp(split, class_names[1]) == 0) {c[1] = 1.0;}
        else if (strcmp(split, class_names[2]) == 0) {c[2] = 1.0;}
        else {
            printf("Unknown class %s\n", split);
            exit(1);
        }

        // printf("Data point %d is %.1f %.1f %.1f %.1f  ->   %.1f %.1f %.1f\n", i, p[0], p[1], p[2], p[3], c[0], c[1], c[2]);
    }

    fclose(in);
}

int main(int argc, char *argv[])
{
    printf("Flight Path Predictor\n");
    printf("Train an ANN to predict the next time step's position\n");
    printf("based on the current time step's:\n");
    printf("\tposition");
    printf("\tvelocity");
    printf("\tmotor RPMs");

    /* Load the data from file. */
    load_data();

    /* 4 inputs.
     * 1 hidden layer(s) of 4 neurons.
     * 3 outputs (1 per class)
     */
    genann *ann = genann_init(4, 1, 4, 3);

    int i, j;
    int loops = 5000;

    /* Train the network with backpropagation. */
    printf("Training for %d loops over data.\n", loops);
    for (i = 0; i < loops; ++i) {
        for (j = 0; j < num_training_samples; ++j) {
            genann_train(ann, input + j*4, class + j*3, .01);
        }
        /* printf("%1.2f ", xor_score(ann)); */
    }

    /*
    printf("TRAINING DATA\n");
    for (j = 0; j < num_training_samples; ++j) {
        printf("input: %f,%f,%f,%f\tclass: %f,%f,%f\n", input[j*4], input[j*4+1], input[j*4+2],input[j*4+3], class[j*3], class[j*3+1], class[j*3+2]);
    }
    printf("TESTING DATA\n");
    for (j = num_training_samples; j < samples; ++j) {
        printf("input: %f,%f,%f,%f\tclass: %f,%f,%f\n", input[j*4], input[j*4+1], input[j*4+2],input[j*4+3], class[j*3], class[j*3+1], class[j*3+2]);
    }*/

    int correct = 0;
    for (j = num_training_samples; j < samples; ++j) {
        const double *guess = genann_run(ann, input + j*4);
        /*printf("test_class[j*3+0] = %f\n", test_class[j*3+0]);*/
        if (class[j*3+0] == 1.0) {if (guess[0] > guess[1] && guess[0] > guess[2]) ++correct;}
        else if (class[j*3+1] == 1.0) {if (guess[1] > guess[0] && guess[1] > guess[2]) ++correct;}
        else if (class[j*3+2] == 1.0) {if (guess[2] > guess[0] && guess[2] > guess[1]) ++correct;}
        else {printf("Logic error.\n"); exit(1);}
    }

    printf("%d/%d correct (%0.1f%%).\n", correct, num_testing_samples, (double)correct / num_testing_samples * 100.0);

    printf("num_training_samples = %d\nnum_testing_samples = %d\nsamples = %d\n", num_training_samples, num_testing_samples, samples);


    genann_free(ann);

    return 0;
}


//#endif
