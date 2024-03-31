/*
 * Dumb S3M Header Editor
 * by RepellantMold (2023)
 *
 * Usage: DumbS3MHeaderEditor <filename.s3m>
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_s3m_header(unsigned char* header);
void check_s3m_tracker_version(unsigned char* header);
void handle_s3m_channels(unsigned char* header);
void handle_s3m_flags(unsigned char* header);
void handle_stereo_toggle(unsigned char* header);

int
main(int argc, char* argv[]) {
    unsigned char header[96] = {0};
    FILE* s3m = NULL;

    puts("Dumb S3M Header Editor\nby RepellantMold (2023, 2024)\n\n");

    switch (argc) {
        case 2:
            s3m = fopen(argv[1], "rb+");
            if (s3m == NULL) {
                puts("Failed to open the file.");
                return 1;
            }

            fread(header, sizeof(char), sizeof(header), s3m);

            if (check_s3m_header(header)) {
                puts("Not a valid S3M file.");
                return 2;
            }

            /* Null terminated string */
            printf("Song title: %.28s\n", header);

            check_s3m_tracker_version(header);

            handle_s3m_flags(header);

            handle_stereo_toggle(header);

            handle_s3m_channels(header);

            rewind(s3m);

            fwrite(header, sizeof(char), sizeof(header), s3m);

            fclose(s3m);

            puts("Done!");

            return 0;
            break;

        case 0:
        case 1:
            puts("Expected usage: DumbS3MHeaderEditor <filename.s3m>");
            return 1;
            break;

        default:
            puts("Too many arguments.");
            return 1;
            break;
    }
}

int
check_s3m_header(unsigned char* header) {
    if (!header) {
        return 2;
    }
    if (header[44] != 'S' || header[45] != 'C' || header[46] != 'R' || header[47] != 'M') {
        return 1;
    }
    return 0;
}

void
check_s3m_tracker_version(unsigned char* header) {
    unsigned short trackerinfo = 0;

    if (!header) {
        return;
    }

    trackerinfo = ((header[41] << 8) + header[40]);

    printf("Tracker info: %04X, which translates to...\n", trackerinfo);

    /* (not really going to be sophisticated with this) */
    switch (header[41] >> 4) {
        default:
            if (trackerinfo == 0xCA00) {
                puts("Camoto / libgamemusic");
            } else if (trackerinfo == 0x0208) {
                puts("Polish localized Squeak Tracker");
            } else if (trackerinfo == 0x5447) {
                puts("Graoumf Tracker");
            } else {
                puts("Unknown");
            }
            break;

        case 1:
            printf("Scream Tracker 3.%02X\n", header[40]);
            puts("(could be disguised...)");
            break;

        case 2: printf("Imago Orpheus %1X.%02X\n", header[41] & 0x0F, header[40]); break;

        case 3: printf("Impulse Tracker %1X.%02X\n", header[41] & 0x0F, header[40]); break;

        case 4: printf("Schism Tracker %1X.%02X\n", header[41] & 0x0F, header[40]); break;

        case 5:
            if (header[54] == 0 && header[55] == 0) {
                printf("OpenMPT %1X.%02X\n", header[41] & 0x0F, header[40]);
            } else {
                printf("OpenMPT %1X.%02X.%1X.%1X\n", header[41] & 0x0F, header[40], header[54], header[55]);
            }
            break;

        case 6: printf("BeRo Tracker %1X.%02X\n", header[41] & 0x0F, header[40]); break;

        case 7: printf("CreamTracker %1X.%02X\n", header[41] & 0x0F, header[40]); break;
    }
}

void
handle_s3m_channels(unsigned char* header) {
    size_t i = 0;

    if (!header) {
        return;
    }

    puts("Channel values (decimal):\n"
         "0-7: Left 1 - 8\n"
         "8-15: Right 1 - 8\n"
         "16-24: Adlib Melody 1 - 9\n"
         "25-29: Adlib Percussion\n"
         "30-127: Invalid/Garbage\n"
         "all values above + 128 = disabled\n"
         "255: Unused channel");

    for (i = 0; i < 32; i++) {
        printf("Enter the value for channel %02d (decimal):", (unsigned char)i + 1);
        scanf("%hhu", &header[64 + i]);
    }
}

void
handle_s3m_flags(unsigned char* header) {
    unsigned int flaggos = 0;

    if (!header) {
        return;
    }

    puts("\nThe bit meanings for the song flags (hex):\n"
         "0 (+1): ST2 vibrato (deprecated)\n"
         "1 (+2): ST2 tempo (deprecated)\n"
         "2 (+4): Amiga slides (deprecated)\n"
         "3 (+8): 0-vol optimizations\n"
         "4 (+10): Enforce Amiga limits\n"
         "5 (+20): Enable SoundBlaster filter/FX (deprecated)\n"
         "6 (+40): Fast volume slides\n"
         "7 (+80): Pointer to special data is valid\n\n"
         "Enter your new value (hexadecimal):");

    scanf("%2X", &flaggos);

    header[38] = flaggos;
}

void
handle_stereo_toggle(unsigned char* header) {
    unsigned int stereotoggle = 1;

    if (!header) {
        return;
    }

    puts("Would you like the song to be in stereo (1) or mono (0)?");
    scanf("%1u", &stereotoggle);

    header[51] |= stereotoggle << 7;
}
