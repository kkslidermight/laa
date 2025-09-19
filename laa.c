#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#define IMAGE_DOS_SIGNATURE 0x5A4D
#define IMAGE_NT_SIGNATURE  0x00004550
#define IMAGE_FILE_LARGE_ADDRESS_AWARE 0x0020

#pragma pack(push,1)
typedef struct {
    uint16_t e_magic;
    uint16_t e_cblp;
    uint16_t e_cp;
    uint16_t e_crlc;
    uint16_t e_cparhdr;
    uint16_t e_minalloc;
    uint16_t e_maxalloc;
    uint16_t e_ss;
    uint16_t e_sp;
    uint16_t e_csum;
    uint16_t e_ip;
    uint16_t e_cs;
    uint16_t e_lfarlc;
    uint16_t e_ovno;
    uint16_t e_res[4];
    uint16_t e_oemid;
    uint16_t e_oeminfo;
    uint16_t e_res2[10];
    int32_t  e_lfanew;
} IMAGE_DOS_HEADER;

typedef struct {
    uint32_t Signature;
    uint16_t Machine;
    uint16_t NumberOfSections;
    uint32_t TimeDateStamp;
    uint32_t PointerToSymbolTable;
    uint32_t NumberOfSymbols;
    uint16_t SizeOfOptionalHeader;
    uint16_t Characteristics;
} IMAGE_FILE_HEADER;

typedef struct {
    uint16_t Magic;
    uint8_t  MajorLinkerVersion;
    uint8_t  MinorLinkerVersion;
    uint32_t SizeOfCode;
    uint32_t SizeOfInitializedData;
    uint32_t SizeOfUninitializedData;
    uint32_t AddressOfEntryPoint;
    uint32_t BaseOfCode;
    uint32_t BaseOfData; 
    uint32_t ImageBaseLow; 
    uint32_t SectionAlignment;
    uint32_t FileAlignment;
    uint16_t MajorOSVersion;
    uint16_t MinorOSVersion;
    uint16_t MajorImageVersion;
    uint16_t MinorImageVersion;
    uint16_t MajorSubsystemVersion;
    uint16_t MinorSubsystemVersion;
    uint32_t Win32VersionValue;
    uint32_t SizeOfImage;
    uint32_t SizeOfHeaders;
    uint32_t CheckSum;
} IMAGE_OPTIONAL_HEADER32;
#pragma pack(pop)

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <pe_file>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb+");
    if (!f) { perror("fopen"); return 1; }

    IMAGE_DOS_HEADER dos;
    if (fread(&dos, sizeof(dos), 1, f) != 1 || dos.e_magic != IMAGE_DOS_SIGNATURE) {
        fprintf(stderr, "Not a valid MZ executable.\n");
        fclose(f); return 1;
    }

    if (fseek(f, dos.e_lfanew, SEEK_SET) != 0) { perror("fseek"); fclose(f); return 1; }

    uint32_t sig;
    if (fread(&sig, sizeof(sig), 1, f) != 1 || sig != IMAGE_NT_SIGNATURE) {
        fprintf(stderr, "Not a valid PE file.\n");
        fclose(f); return 1;
    }

    IMAGE_FILE_HEADER filehdr;
    long filehdr_pos = ftell(f);
    if (fread(&filehdr, sizeof(filehdr), 1, f) != 1) {
        fprintf(stderr, "Failed to read file header.\n");
        fclose(f); return 1;
    }

    if (filehdr.Characteristics & IMAGE_FILE_LARGE_ADDRESS_AWARE) {
        puts("Already Large Address Aware.");
        fclose(f);
        return 0;
    }

    filehdr.Characteristics |= IMAGE_FILE_LARGE_ADDRESS_AWARE;

    if (fseek(f, filehdr_pos, SEEK_SET) != 0) { perror("fseek"); fclose(f); return 1; }
    if (fwrite(&filehdr, sizeof(filehdr), 1, f) != 1) { perror("fwrite"); fclose(f); return 1; }

    long opt_pos = filehdr_pos + sizeof(IMAGE_FILE_HEADER);
    if (fseek(f, opt_pos + offsetof(IMAGE_OPTIONAL_HEADER32, CheckSum), SEEK_SET) != 0) { perror("fseek"); fclose(f); return 1; }
    uint32_t zero = 0;
    if (fwrite(&zero, sizeof(zero), 1, f) != 1) { perror("fwrite"); fclose(f); return 1; }

    fclose(f);
    puts("Set Large Address Aware and zeroed checksum.");
    return 0;
}
