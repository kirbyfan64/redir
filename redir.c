#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

char* file = NULL;
unsigned long max = 0;
int append = 0;

#ifdef REDIRF
#define XARGS "[-t <tmp>] "
#define XOPT "t:"
char* tmp = NULL;
char tmps[32];
void gentmp() {
    sprintf(tmps, "/tmp/redir.%ld", (long)getpid());
    tmp = tmps;
}
#else
#define XARGS
#define XOPT
#endif

int process() {
    char tbuf[1024];
    unsigned long total = 0;
    size_t len;
    int ret = 0;
    FILE* out = NULL;
    #ifdef REDIRF
    FILE* tmpf;
    if ((tmpf = fopen(tmp, "w+")) == NULL) goto error;
    #else
    char* buf = NULL, *rebuf;
    unsigned long buflen = 0;
    #endif
    /* read all input */
    for (;;) {
        if (fgets(tbuf, sizeof(tbuf), stdin) == NULL) {
            if (ferror(stdin)) goto error;
            goto next;
        }
        len = strlen(tbuf);
        total += len;
        if (max && total > max) goto maxed_out;
        #ifdef REDIRF
        if (fwrite(tbuf, 1, len, tmpf) != len) goto error;
        #else
        if ((rebuf = realloc(buf, buflen+len+1)) == NULL) goto error;
        buf = rebuf;
        memcpy(buf+buflen, tbuf, len+1);
        buflen += len;
        #endif
    }
    next:
    /* write to file */
    if ((out = fopen(file, "w")) == NULL) goto error;
    #ifdef REDIRF
    rewind(tmpf);
    for (;;) {
        if (fgets(tbuf, sizeof(tbuf), tmpf) == NULL) {
            if (ferror(tmpf)) goto error;
            goto end;
        }
        len = strlen(tbuf);
        if (fwrite(tbuf, 1, len, out) != len) goto error;
    }
    #else
    if (fwrite(buf, 1, buflen, out) != buflen) goto error;
    #endif
    goto end;
    maxed_out:
    ret = 1;
    fputs("buffer maxed out\n", stderr);
    goto end;
    error:
    ret = 1;
    perror("redir");
    end:
    if (out) fclose(out);
    #ifdef REDIRF
    if (tmpf) {
        fclose(tmpf);
        remove(tmp);
    }
    #else
    if (buf) free(buf);
    #endif
    return ret;
}

int main(int argc, char** argv) {
    int ret = 0;
    char flag;
    char* end;
    append = 0;

    if (sizeof(pid_t) > sizeof(long)) {
        fputs("you have a WEIRD computer: sizeof(pid_t)>sizeof(long)\n", stderr);
        abort();
    }

    while ((flag = getopt(argc, argv, "has:" XOPT)) != -1) switch (flag) {
    case 'h':
        goto usage;
    case 'a':
        if (append) fputs("warning: -a specified multiple times\n", stderr);
        append = 1;
        break;
    case 's':
        if (max) {
            fputs("warning: -s specified multiple times\n", stderr);
            continue;
        }
        errno = 0;
        max = strtoul(optarg, &end, 10);
        if (errno || *end) {
            fputs("error parsing -s value\n", stderr);
            return 0;
        }
        break;
    #ifdef REDIRF
    case 't':
        if (tmp != NULL) fputs("warning: -t specified multiple times\n", stderr);
        tmp = optarg;
        break;
    #endif
    case '?': goto usage_error;
    }

    if (optind >= argc) {
        fputs("need file argument\n", stderr);
        goto usage_error;
    }

    file = argv[optind];

    #ifdef REDIRF
    if (!tmp) gentmp();
    #endif

    return process();

    usage_error:
    ret = 1;
    usage:
    printf("usage: %s [-s <max>] [-a] " XARGS "<file>\n", argv[0]);
    return ret;
}
