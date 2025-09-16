#include <stdio.h>
#include <stdint.h>

#define IMAGE_DOS_SIGNATURE 0x5A4D
#define PE_HEADER_OFFSET_LOCATION 0x3C
#define IMAGE_NT_SIGNATURE 0x00004550
#define CHARACTERISTICS_OFFSET 20
#define IMAGE_FILE_LARGE_ADDRESS_AWARE 0x0020

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <pe_file>\n", argv[0]);
        return 1;
    }

    FILE* f = fopen(argv[1], "rb+");
    if (!f) { perror("fopen"); return 1; }

    uint16_t mz;
    if (fread(&mz, 2, 1, f) != 1 || mz != IMAGE_DOS_SIGNATURE) {
        fprintf(stderr, "Not a valid MZ executable.\n");
        fclose(f); return 1;
    }

    uint32_t pe_off;
    fseek(f, PE_HEADER_OFFSET_LOCATION, SEEK_SET);
    fread(&pe_off, 4, 1, f);
    fseek(f, pe_off, SEEK_SET);

    uint32_t sig;
    fread(&sig, 4, 1, f);
    if (sig != IMAGE_NT_SIGNATURE) {
        fprintf(stderr, "Not a valid PE file.\n");
        fclose(f); return 1;
    }

    fseek(f, CHARACTERISTICS_OFFSET, SEEK_CUR);
    long pos = ftell(f);

    uint16_t ch;
    fread(&ch, 2, 1, f);

    if (ch & IMAGE_FILE_LARGE_ADDRESS_AWARE) {
        puts("Already Large Address Aware.");
    } else {
        ch |= IMAGE_FILE_LARGE_ADDRESS_AWARE;
        fseek(f, pos, SEEK_SET);
        fwrite(&ch, 2, 1, f);
        puts("Set Large Address Aware.");
    }

    fclose(f);
    return 0;
}
