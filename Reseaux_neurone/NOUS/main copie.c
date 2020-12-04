#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "network_neuronal.h"

/*int main() {
    srand(time(NULL));
    int size_layer[] = {2, 4, 1};
    mlp * network = create_mlp (3, size_layer); // we create a pointer to our 3-layer network
    mlp_float enter[4][2] = { // table of input values
        {-1,-1},
        {-1,1},
        {1,-1},
        {1, 1}
    };
    mlp_float exit[4] = { // values ​​of the xor function corresponding to the inputs
        -1,
        1,
        1,
        -1
    };
    
    mlp_float global_error = 1;// to enter the while
        int i = 0;


        while (global_error > 0.00000000000001) { // as long as the error of our network is greater than the maximum error
            printf("error: %0.20f\n",global_error);// show the error of our network

            for (int iter=0; iter < 100; iter++) { // we give 100 examples to our network and ask it each time to correct its errors
                int num = 4.0*rand()/(RAND_MAX+1.0);
                set_mlp(network,& enter[num][0]); //the second parameter is a pointer to an mlp_float array. The set_mlp function uses the number of entries given plus to know which boxes to read in this table.
                learn_mlp(network,&exit[num]); // the number of outputs that we gave above allows learn_mlp to know how many boxes to read in the table passed as a parameter
            }
            // we corrected our network 100 times with our 4 examples but in a different order
            global_error = 0; // the global error is calculated: the standard deviation between the 4 given and desired outputs.
            for (int k=0; k < 4; k++) {
                set_mlp(network,& enter[k][0]); 
                mlp_float exit_network;
                get_mlp(network,&exit_network); // the get_mlp function uses the number of outputs given above to find out how many cells to write in this array
                mlp_float error=exit_network - exit[k];
                global_error += error * error;
            }

            global_error *= 0.226; 
            global_error = sqrt(global_error);
            // if the error is too big we redo 100 learning
            i++;
        }
        // we display the 4 results that the network gives us
        for (int k = 0; k < 4; ++k)
        {
            set_mlp(network,&enter[k][0]);
            mlp_float exit_network;
            get_mlp(network,&exit_network);
            printf("entries: %f et %f, outpout given by the network: %f desired exit: %f\n",
            enter[k][0],enter[k][1],exit_network,exit[k]);
        }
        printf("---> result found in %d iterations\n", i*100);
}
*/
int main() {
    srand(time(NULL));

    mlp_float letter[26][15];

    int i,j;
    char path[6] = "A.txt";
    char *buf = malloc(256);
    for (i = 0; i < 26; i++) {
        FILE *f = fopen(path,"r+b");
        fseek (f, 0, SEEK_END);
        int size=ftell (f);
        rewind (f);

        fread(buf,(sizeof *buf), size, f);
        fclose(f);
        path[0]++;
        int line;
        for (line = 0; line < 5; line++) {
            if (buf[line*4] == '1') letter[i][line*3] = 0.8;
            else letter[i][line*3] = -0.8;
            if (buf[line*4+1] == '1') letter[i][line*3+1] = 0.8;
            else letter[i][line*3+1] = -0.8;
            if (buf[line*4+2] == '1') letter[i][line*3+2] = 0.8;
            else letter[i][line*3+2] = -0.8;
        }
    }


    for (i = 0; i < 26; i++) { //print all letters
        for (j = 0; j < 5; j++) {
            if (letter[i][j*3]== 0.8) putchar('O');
            else putchar(' ');
            if (letter[i][j*3+1]== 0.8) putchar('O');
            else putchar(' ');
            if (letter[i][j*3+2]== 0.8) putchar('O');
            else putchar(' ');
            putchar('\n');
        }
        putchar('\n');
    }


    int size_layer[] = {15, 30, 5};

    mlp * network = create_mlp (3, size_layer);


    i = 0;
    mlp_float global_error = 1;
    mlp_float enter[5] = {-0.8,-0.8,-0.8,-0.8,-0.8};
    while (global_error > 0.00000000000001) {


        int k;
        int iter;
        mlp_float noisy_letter[15];
        for (iter=0; iter < 100000; iter++) {
            k = (int)(5.0*rand()/(RAND_MAX+1.0));
            for (j = 0; j < 15; j++) {
                noisy_letter[j] = letter[k][j] + 1.9*rand()/(RAND_MAX+1.0)-0.95;
            }

            set_mlp(network,&letter[k][0]);
            enter[k] = 0.8;
            learn_mlp(network,enter);
            enter[k] = -0.8;
        }

        global_error = 0;
        for (k=0; k < 5; k++) { //
            enter[k] = 0.8;
            set_mlp(network,&letter[k][0]);
            mlp_float exit[5];
            get_mlp(network,exit);
            int z;
            for (z = 0; z < 5; z++) {
                global_error += (exit[z] - enter[z])*(exit[z] - enter[z]);
            }
            enter[k] = -0.8;
        }

        global_error *= 0.2;
        global_error = sqrt(global_error);
        printf("error: %.40f       %d\n",global_error,i);
        i++;
    }
    printf("\n\n%d apprentisssages.......faits\n\n",i*100000);

    while(1) {
        printf("TAPEZ UNE NOUVELLE LETTRE\n\n");
        mlp_float input[15];

        for (i = 0; i < 5; i++) {
            buf[0] = buf[1] = buf[2] = 0;
            fgets(buf,256,stdin);
            if (buf[0] == ' ' || buf[0] < '0') input[i*3] = -0.8;
            else input[i*3] = 0.8;
            if (buf[1] == ' ' || buf[1] < '0') input[i*3+1] = -0.8;
            else input[i*3+1] = 0.8;
            if (buf[2] == ' ' || buf[2] < '0') input[i*3+2] = -0.8;
            else input[i*3+2] = 0.8;
        }
        set_mlp(network,input);
        mlp_float exit[5];
        get_mlp(network,exit);
        printf("\n\ntest de reconnaissance VOICI LE RESULTAT:\n");
        int res = 0;
        for (i = 0; i < 5; i++) {
            printf(" %d = %f\n",i,exit[i]);
            if( exit[res] < exit[i]) res = i;
        }
        printf("\n\n");
        for (j = 0; j < 5; j++) {
            if (letter[res][j*3]== 0.8) putchar('O');
            else putchar(' ');
            if (letter[res][j*3+1]== 0.8) putchar('O');
            else putchar(' ');
            if (letter[res][j*3+2]== 0.8) putchar('O');
            else putchar(' ');
            putchar('\n');
        }
    }

    free(buf);
    free_mlp(network);

    return 0;

}
