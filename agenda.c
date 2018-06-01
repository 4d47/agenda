
#define _XOPEN_SOURCE

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

#define DAYS(n) 60*60*24*n
#define DAYS_MAX INT_MAX/60/60/24
#define PATH_SIZE 1024
#define LINE_LEN 1024

const char* env(const char *, const char *);
time_t midnight();
struct tm* now();

int
main(int argc, char *argv[])
{
  int ch;
  time_t today = midnight();
  int backward = 0;
  int advance = DAYS(14);

  char path[PATH_SIZE];
  const char *editor = env("EDITOR", "vi");
  const char *agendafile = "/var/AGENDA";

  FILE *stream;
  char line[LINE_LEN];
  struct tm day = { 0 };
  int thisyear = now()->tm_year;
  time_t date;
  int n;

  if (snprintf(path, PATH_SIZE, "%s/%s", getenv("HOME"), "AGENDA")) {
    agendafile = env("AGENDA_FILE", path);
  }

  while ((ch = getopt(argc, argv, "A:B:t:f:eh")) != -1) {
    switch (ch) {
      case 't':
        if (strptime(optarg, "%F", &day)) {
          today = mktime(&day);
        }
        break;
      case 'A':
        advance = DAYS(atoi(optarg));
        break;
      case 'B':
        backward = DAYS(atoi(optarg));
        break;
      case 'f':
        agendafile = optarg;
        break;
      case 'e':
        execlp(editor, editor, agendafile, NULL);
        break;
      default:
        fputs("usage: agenda [-e] [-f file] [-t date] [-A num] [-B num]\n", stderr);
        return EXIT_FAILURE;
    }
  }

  stream = fopen(agendafile, "r");

  if (errno != 0) {
    perror("error");
    return EXIT_FAILURE;
  }

  while (fgets(line, LINE_LEN, stream) != NULL) {
    if ((n = strptime(line, "%F", &day))) {
      ;
    } else if ((n = strptime(line, "%m-%d", &day))) {
      day.tm_year = thisyear;
    }
    if (n) {
      date = mktime(&day);
      if (today - backward < date && date < today + advance) {
        fputs(line, stdout);
      }
    }
  }

  fclose(stream);
  return EXIT_SUCCESS;
}


const char*
env(const char *name, const char *alt)
{
  const char *value = getenv(name);
  return value ? value : alt;
}


time_t
midnight()
{
  struct tm *t = now();
  t->tm_hour = 0;
  t->tm_min = 0;
  t->tm_sec = 0;
  return mktime(t);
}


struct tm*
now()
{
  time_t t = time(NULL);
  return localtime(&t);
}

