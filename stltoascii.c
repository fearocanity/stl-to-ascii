#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE *infile, *out;
    char *filename_in;
    char *filename_out = "out.stl";

    if (argc < 2 || argc > 4) {
        printf("Error: Incorrect number of arguments\n");
        printf("Usage: %s <filename_in> [-o <filename_out>]\n", argv[0]);
        return 1;
    }

    filename_in = argv[1];

    if (argc == 4 && strcmp(argv[2], "-o") == 0) {
        filename_out = argv[3];
    }

    infile = fopen(filename_in, "rb");
    if (infile == NULL) {
        printf("Error: Cannot open input file %s\n", filename_in);
        return 1;
    }

    out = fopen(filename_out, "w");
    if (out == NULL) {
        printf("Error: Cannot open output file %s\n", filename_out);
        return 1;
    }

    char data[80];
    int i;
    for (i = 0; i < 80; i++) {
        fread(&data[i], sizeof(char), 1, infile);
        if (data[i] == 0) {
            break;
        }
    }
    fprintf(out, "solid %s\n", data);

    int num_faces;
    fread(&num_faces, sizeof(int), 1, infile);

    int x, y;
    float normal[3], vertex[3];
    for (x = 0; x < num_faces; x++) {
        fprintf(out, "facet normal ");
        fread(normal, sizeof(float), 3, infile);
        fprintf(out, "%f %f %f\n", normal[0], normal[1], normal[2]);

        fprintf(out, "  outer loop\n");
        for (y = 0; y < 3; y++) {
            fprintf(out, "    vertex ");
            fread(vertex, sizeof(float), 3, infile);
            fprintf(out, "%f %f %f\n", vertex[0], vertex[1], vertex[2]);
        }
        fprintf(out, "  endloop\n");
        fprintf(out, "endfacet\n");
    }

    fclose(infile);
    fclose(out);
    printf("Done.\n");
    return 0;
}
