
# Agenda

A much more limited reminder service

## Description

Like calendar(1) except ...

- Does not check the current directory for calendarfile
- Does not preprocess calendarfile with cpp(1);
- Does not use tab as separator;
- Does not allow other date formats than ISO8601;
- Does not jump over weekends

## Usage

    $ agenda

It will read a file named `CALENDAR` in user home directory
(if not overriden with `CALENDAR_FILE` env or `-f` option switch)
and filter lines that starts with an iso date (YYYY-mm-dd) that
is within the next 14 days span.

If the date omits the year (mm-dd), the current year is used.
This is useful for anniversary events.

See `man agenda` for options.

## Todo

- Relative dates

## See Also

- [When](http://www.lightandmatter.com/when/when.html)
- [remind](https://www.linuxjournal.com/article/3529)

