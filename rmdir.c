#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>

static int rmdir_ex(const char *name);
static int _rmdir_ex(int dirfd);

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Syntax: %s <dir>\n", argv[0]);
        return EXIT_FAILURE;
    }

    return rmdir_ex(argv[1]) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

int rmdir_ex(const char *name) {
    int result;
    int fd = open(name, O_RDONLY | O_DIRECTORY);

    if (fd < 0) {
        perror("open()");
        return -1;
    }

    result = _rmdir_ex(fd);

    if (result == 0) {
        result = rmdir(name);
    }

    /*if (close(fd) < 0) {
        perror("rmdir_ex.close()");
    }*/

    return result;
}

int _rmdir_ex(int dirfd) {
    int result = 0;
    int fd;
    struct stat buf;
    struct dirent *dirent;
    DIR *dir = fdopendir(dirfd);

    if (!dir) {
        perror("fdopendir()");
        return -1;
    }

    while (dirent = readdir(dir), dirent && result == 0) {

        // If d_name == "." or d_name == "..", skip

        if (dirent->d_name[0] == '.' && (dirent->d_name[1] == '\0' || (dirent->d_name[1] == '.' && dirent->d_name[2] == '\0'))) {
            continue;
        }

        fd = openat(dirfd, dirent->d_name, O_RDONLY);

        if (fd < 0) {
            perror("openat()");
            result = -1;
            break;
        }

        result = fstat(fd, &buf);

        if (result == 0) {
            if (S_ISDIR(buf.st_mode)) {
                result = _rmdir_ex(fd);

                if (result == 0) {
                    result = unlinkat(dirfd, dirent->d_name, AT_REMOVEDIR);

                    if (result < 0) {
                        perror("unlinkat(dir)");
                    }
                }
            } else {
                if (close(fd) < 0) {
                    perror("_rmdir_ex.close()");
                }

                if (S_ISREG(buf.st_mode)) {
                    result = unlinkat(dirfd, dirent->d_name, 0);

                    if (result < 0) {
                        perror("unlinkat(file)");
                    }
                } else {
                    fprintf(stderr, "Entry '%s' is neither file nor directory.\n", dirent->d_name);
                    result = -1;
                }
            }
        }
    }

    closedir(dir);
}
