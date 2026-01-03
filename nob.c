#define NOBUILD_IMPLEMENTATION
#include "./nob.h"

#define SOURCE "main.c"
#define BIN "twm"
#define CC "clang"
#define LIB "-lX11"
#define PREFIX "/usr/local/bin/"
#define OLD "c.old"
#define FLAGS "-std=c23", "-Wall", "-Wextra", "-Werror" 

void Compile(void) {
    CMD(CC, "-o", BIN, SOURCE, FLAGS, LIB);
}

void Install(void) {
    CMD("sudo", "cp", "-f", BIN, PREFIX);
}

void Wipe(void) {
    CMD("sudo", "rm", "-v", PREFIX""BIN);
    CMD("rm", BIN, "c.old");
}

int main(int argc, char *argv[]) {
    GO_REBUILD_URSELF(argc, argv);

    if (argc < 2) {
        printf("Usage: %s [-c compile] [-i install] [-w wipe]\n", argv[0]);
        return EXIT_SUCCESS;
    }

    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];

        if (arg[0] == '-') {
            for (unsigned long int j = 1; j < strlen(arg); j++) {

                switch (arg[j]) {
                    case 'c': Compile();  break;
                    case 'i': Install();	break;
                    case 'w': Wipe();     break;
                    default: printf("Unknown option: %c\n", arg[j]);
                        break;
                }
            }
        }
    }
    return EXIT_SUCCESS;
}
