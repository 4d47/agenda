
# Agenda

[![Build Status](https://travis-ci.org/4d47/agenda.svg?branch=master)](https://travis-ci.org/4d47/agenda)

A much more limited reminder service

## Description

Like calendar(1) except ...

- Does not check the current directory for calendarfile
- Does not preprocess calendarfile with cpp(1);
- Does not use tab as separator;
- Does not allow other date formats than ISO 8601;
- Does not jump over weekends

## Usage

    $ agenda

It will read a file named `AGENDA` in user home directory
(if not overriden with `AGENDA_FILE` env or `-f` option switch)
and filter lines that starts with an iso date (YYYY-mm-dd) that
is within the next 14 days span.

If the date omits the year (mm-dd), the current year is used.
This is useful for anniversary events.

See `man agenda` for options.

## To Think

- Relative dates

## See Also

- [When](http://www.lightandmatter.com/when/when.html)
- [remind](https://www.linuxjournal.com/article/3529)

