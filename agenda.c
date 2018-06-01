#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

#define DAYS(n) 60*60*24*n
#define DAYS_MAX INT_MAX/60/60/24
#define PATH_SIZE 1024

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
  const char *calendarfile = "/var/CALENDAR";

  FILE *stream;
  const char *errstr;
  char *line = NULL;
  size_t linelen = 0;
  ssize_t nread;
  struct tm day = { 0 };
  int thisyear = now()->tm_year;
  time_t date;
  char *n;

  if (snprintf(path, PATH_SIZE, "%s/%s", getenv("HOME"), "CALENDAR")) {
    calendarfile = env("CALENDAR_FILE", path);
  }

  while ((ch = getopt(argc, argv, "A:B:t:f:eh")) != -1) {
    switch (ch) {
      case 't':
        if (strptime(optarg, "%Y-%m-%d", &day) != NULL) {
          today = mktime(&day);
        }
        break;
      case 'A':
        advance = DAYS(strtonum(optarg, 0, DAYS_MAX, &errstr));
        break;
      case 'B':
        backward = DAYS(strtonum(optarg, 0, DAYS_MAX, &errstr));
        break;
      case 'f':
        calendarfile = optarg;
        break;
      case 'e':
        execlp(editor, editor, calendarfile, NULL);
        break;
      default:
        fputs("usage: agenda [-e] [-f file] [-t date] [-A num] [-B num]\n", stderr);
        return EXIT_FAILURE;
    }
  }

  stream = fopen(calendarfile, "r");

  if (errno != 0) {
    perror("error");
    return EXIT_FAILURE;
  }

  while ((nread = getline(&line, &linelen, stream)) != -1) {
    if ((n = strptime(line, "%F", &day)) != NULL) {
    } else if ((n = strptime(line, "%m-%d", &day)) != NULL) {
      day.tm_year = thisyear;
    }
    if (n) {
      date = mktime(&day);
      if (today - backward < date && date < today + advance) {
        fputs(line, stdout);
      }
    }
  }

  free(line);
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

