#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
// getopt()を使うための宣言
#define _GNU_SOURCE
#include <getopt.h>
#include <unistd.h>

#define BUFFER_SIZE 2048

static void do_grep(regex_t *pat, FILE *f);

int main(int argc, char *argv[]) {
  regex_t pat;
  int err;
  int i;
  int opt;

  /* オプションを解析するループ */
  while ((opt = getopt(argc, argv, "iv") != -1)) {
    switch (opt) {
      case 'i': /* 文字の大文字小文字を区別せずに検索するとき */
        err = regcomp(&pat, argv[1],
                      REG_EXTENDED | REG_NOSUB | REG_NEWLINE | REG_ICASE);
        break;
      case 'v': /* 一致しないものを検索したいとき*/
                //何を入れればいいのかまだわからん
        // fprintf(stdout, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
        break;
      case '?': /* 関係ない文字が入力されているとき*/
        fprintf(stderr, "Usage: %s [-iv] [FILE ...]\n", argv[0]);
        exit(1);
    }
  }

  argc -= optind;
  argv += optind;

  if (argc < 2) {
    fputs("no pattern\n", stderr);
    exit(1);
  }
  err = regcomp(&pat, argv[1], REG_EXTENDED | REG_NOSUB | REG_NEWLINE);

  if (err != 0) {
    char buf[1024];

    regerror(err, &pat, buf, sizeof buf);
    puts(buf);
    exit(1);
  }
  if (argc == 2) {
    do_grep(&pat, stdin);
  } else {
    for (i = 2; i < argc; i++) {
      FILE *f;

      f = fopen(argv[i], "r");
      if (!f) {
        perror(argv[i]);
        exit(1);
      }
      do_grep(&pat, f);
      fclose(f);
    }
  }
  regfree(&pat);
  exit(0);
}

static void do_grep(regex_t *pat, FILE *src) {
  char buf[4096];

  while (fgets(buf, sizeof buf, src)) {
    if (regexec(pat, buf, 0, NULL, 0) == 0) {
      fputs(buf, stdout);
    }
  }
}
