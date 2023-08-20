#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define RETURN_PAUSE(VALUE) do{printf("\nPress ENTER to exit\n"); getchar(); return (VALUE);} while(0)

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf(
            "Welcome to Hurren's Lazy RotR Color Fixer!\n\n"
            "Usage: please supply your original color file as the first argument.\n"
            "Alternatively, simply drag the original file onto this program in your files.\n"
        );
        RETURN_PAUSE(0);
    }

    char* input_path = argv[1];
    if(access(input_path, F_OK) == -1) {
        fprintf(stderr, "Unable to open %s: %s!\n", input_path, strerror(errno));
        RETURN_PAUSE(1);
    } // Might be unnecessary

    int width = 0;
    int height = 0;
    int channel_count = 0;
    unsigned char *image = stbi_load(input_path, &width, &height, &channel_count, 0);
    if(image == NULL) {
        fprintf(stderr, "Unable to read %s: %s!\n", input_path, stbi_failure_reason());
        RETURN_PAUSE(1);
    }
    if(channel_count < 3) {
        fprintf(stderr, "This program currently only converts RGB and RGBA images.\n");
        stbi_image_free(image);
        RETURN_PAUSE(1);
    }

    char* file_extension = strrchr(input_path, '.');
    if(file_extension != NULL) {
        *file_extension = '\0'; // Fine to overwrite argv[1] since we never need its original value again
    }

    char* output_append = "__Converted";
    char* output_path = malloc((strlen(input_path) + strlen(output_append) + strlen(".png") + 1) * sizeof(*output_path));
    // Let's not mess with MAX_PATH... if the OS complains about the filename, stbi_write_png will catch it.
    if(output_path == NULL) {
        fprintf(stderr, "Unable to allocate output_path memory!\n");
        stbi_image_free(image);
        RETURN_PAUSE(1);
    }
    sprintf(output_path, "%s%s.png", input_path, output_append);
    // access output_path
    // if(access(output_path, F_OK) == 0) {
    //     printf("%s already exists, do you want to overwrite it?\n", output_path);
    // }

    // Vectorization, go!
    for(unsigned int i = 0; i < width * height; i++) {
        unsigned int index_low = i * channel_count;
        unsigned int index_high = index_low + 2;

        unsigned char temp_low = image[index_low];
        image[index_low] = image[index_high];
        image[index_high] = temp_low;
    }

    stbi_write_png_compression_level = 9; // Has no effect, helppp
    if(stbi_write_png(output_path, width, height, channel_count, image, width * channel_count) == 0) {
        fprintf(stderr, "Unable to write %s: %s!\n", output_path, stbi_failure_reason());
        free(output_path);
        stbi_image_free(image);
        RETURN_PAUSE(1);
    }

    free(output_path);
    stbi_image_free(image);
    printf("Conversion complete!\n");
    return 0;
}
