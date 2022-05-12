/*
    grep2.c -- grep command with -i and -v options
*/

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

static void grep_file(regex_t *re, char *path);  //これは何と何で分けた？
static void grep_stream(regex_t *re, FILE *f);  //これは何と何で分けた？

static int opt_invert = 0;  // invertは「反転」という意味（含まないときに使う）
static int opt_ignorecase = 0;  // ignorecaseは大文字小文字区別しない方で使う

int main(int argc, char *argv[]) {
  int opt;
  while ((opt = getopt(argc, argv, "iv")) !=
         -1) {  // getoptでオプションの場合分け
    switch (opt) {
      case 'i':  //大文字小文字を区別しないとき
        opt_ignorecase = 1;
        break;
      case 'v':  //一致しない文字列を出力するとき
        opt_invert = 1;
        break;
      case '?':  //関係ないものが入力されたとき
        fprintf(stderr, "Usage: %s [-iv] [<file>...]\n", argv[0]);
        exit(1);
    }
  }
  /*argcとargvの位置の移動*/
  argc -= optind;
  argv += optind;

  if (argc < 1) {  //指定されていないとき
    fputs("no pattern\n", stderr);
    exit(1);
  }
  char *pattern = argv[0];
  argc--;  //ここ2行がよくわからん、argcはどんどん減っていく
  argv++;  // argvはどんどん増えていく

  /* re は「正規表現 (Regular Expression)」の略語。
     「regexp」「regex」などもよく使われます */
  int re_mode = REG_EXTENDED | REG_NOSUB | REG_NEWLINE;  //通常時の正規表現
  if (opt_ignorecase)
    re_mode |= REG_ICASE;  //大文字小文字を区別しない時は追加する
  regex_t re;
  int err = regcomp(&re, pattern, re_mode);
  if (err != 0) {
    char buf[1024];

    regerror(err, &re, buf, sizeof buf);
    puts(buf);
    exit(1);
  }
  if (argc == 0) {
    grep_stream(&re, stdin);
  } else {
    int i;
    for (i = 0; i < argc; i++) {
      grep_file(&re, argv[i]);
    }
  }
  regfree(&re);
  exit(0);
}

/* path で示されるファイルを grep する */
static void grep_file(regex_t *re, char *path) {
  FILE *f;

  f = fopen(path, "r");  //"r"は「読み込み専用」
  if (!f) {
    perror(path);
    exit(1);
  }
  grep_stream(re, f);
  fclose(f);
}

/* f で示されるストリームを grep する。
   書籍では「FILE* はストリームなんだ！」と散々言ったくせに、
   変数名を f (file の頭文字) にするあたりが弱い。 */
static void grep_stream(regex_t *re, FILE *f) {
  char buf[4096];
  int matched;  //一致するかどうかの関数

  while (fgets(buf, sizeof buf, f)) {
    matched = (regexec(re, buf, 0, NULL, 0) == 0);
    if (opt_invert) {
      matched = !matched;  // matchedに一致しないものを代入
    }
    if (matched) {
      fputs(buf, stdout);  //そのまま代入(?)
    }
  }
}
