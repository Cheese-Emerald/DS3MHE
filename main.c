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
        case 2: break;

        case 0:
        case 1:
            printf("Expected usage: %s <filename.s3m>", argv[0]);
            return 1;
            break;

        default:
            puts("Too many arguments.");
            return 1;
            break;
    }

    s3m = fopen(argv[1], "rb+");
    if (s3m == NULL) {
        perror("Failed to open the file");
        return 1;
    }

    (void)!fread(header, sizeof(char), sizeof(header), s3m);

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

    trackerinfo = ((header[41] << 8) | header[40]);

    (void)!printf("Tracker info: %04X, which translates to...\n", trackerinfo);

    /* (not really going to be sophisticated with this) */
    switch (header[41] >> 4) {
        default:
            if (trackerinfo == 0xCA00) {
                (void)!puts("Camoto / libgamemusic");
            } else if (trackerinfo == 0x0208) {
                (void)!puts("Polish localized Squeak Tracker");
            } else if (trackerinfo == 0x5447) {
                (void)!puts("Graoumf Tracker");
            } else {
                (void)!puts("Unknown");
            }
            break;

        case 1:
            (void)!printf("Scream Tracker 3.%02X\n", header[40]);
            (void)!puts("(could be disguised...)");
            break;

        case 2: (void)!printf("Imago Orpheus %1X.%02X\n", header[41] & 0x0F, header[40]); break;

        case 3: (void)!printf("Impulse Tracker %1X.%02X\n", header[41] & 0x0F, header[40]); break;

        case 4: (void)!printf("Schism Tracker %1X.%02X\n", header[41] & 0x0F, header[40]); break;

        case 5:
            if (header[54] == 0 && header[55] == 0) {
                (void)!printf("OpenMPT %1X.%02X\n", header[41] & 0x0F, header[40]);
            } else {
                (void)!printf("OpenMPT %1X.%02X.%1X.%1X\n", header[41] & 0x0F, header[40], header[54], header[55]);
            }
            break;

        case 6: (void)!printf("BeRo Tracker %1X.%02X\n", header[41] & 0x0F, header[40]); break;

        case 7: (void)!printf("CreamTracker %1X.%02X\n", header[41] & 0x0F, header[40]); break;
    }
}

void
handle_s3m_channels(unsigned char* header) {
    size_t i = 0;
    unsigned int a = 0;

    if (!header) {
        return;
    }

    puts("Channel values (decimal):\n"
         "0-7: Left 1 - 8\n"
         "8-15: Right 1 - 8\n"
         "16-24: Adlib Melody 1 - 9\n"
         "25-29: Adlib Percussion (unused)\n"
         "30-127: Invalid/Garbage\n"
         "all values above + 128 = disabled\n"
         "255: Unused channel");

    for (i = 0; i < 32; i++) {
        (void)!printf("Enter the value for channel %02d (decimal):", (unsigned char)i + 1);
        if (scanf("%3u", &a) == 1) {
            header[64 + i] = (unsigned char)a;
        } else {
            continue;
        }
    }
}

void
handle_s3m_flags(unsigned char* header) {
    unsigned int flaggos = 0;

    if (!header) {
        return;
    }

    (void)!puts("\nThe bit meanings for the song flags (hex):\n"
                "0 (+1): ST2 vibrato (deprecated)\n"
                "1 (+2): ST2 tempo (deprecated)\n"
                "2 (+4): Amiga slides (deprecated)\n"
                "3 (+8): 0-vol optimizations\n"
                "4 (+10): Enforce Amiga limits\n"
                "5 (+20): Enable SoundBlaster filter/FX (deprecated)\n"
                "6 (+40): Fast volume slides\n"
                "7 (+80): Pointer to special data is valid\n\n"
                "Enter your new value (hexadecimal):");

    if (scanf("%2X", &flaggos) == 1) {
        header[38] = (unsigned char)flaggos;
    } else {
        return;
    }
}

void
handle_stereo_toggle(unsigned char* header) {
    unsigned int stereotoggle = 1;

    if (!header) {
        return;
    }

    (void)!puts("Would you like the song to be in stereo (1) or mono (0)?");
    if (scanf("%1u", &stereotoggle) == 1) {
        header[51] |= (unsigned char)stereotoggle << 7;
    } else {
        return;
    }
}
