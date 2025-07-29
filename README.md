# Ephemeris of the Moon

## 🌙 Overview
Version: **1.0 alpha**

A small ephemeris library, suited for embedded systems

Focused on providing accurate astrology charts

## 🌌 Ritual

### Requirements
- A C99 compiler

### Crafting
Files are crafted in `build`
```sh
make
```

### Inscribing
Files are inscribed in `/usr/local`
```sh
sudo make install
sudo ldconfig
```

### Example
Set LATITUDE and LONGITUDE values
```c
#include <time.h>
#include <stdio.h>

#include <mona/core.h>
#include <mona/string.h>

static void
print_solar(struct mona_solar s)
{
    char rise[8], set[8];
    strftime(rise,  8,  "%H:%M",    localtime(&(s.sunrise)));
    strftime(set,   8,  "%H:%M",    localtime(&(s.sunset)));

    const char *season = mona_string(MONA_STRING_SEASON, s.season);

    char start[16], end[16];
    strftime(start, 16, "%d/%m/%y", localtime(&(s.beginning)));
    strftime(end,   16, "%d/%m/%y", localtime(&(s.ending)));

    const char *day  = mona_string(MONA_STRING_DAY,  s.day);
    const char *hour = mona_string(MONA_STRING_HOUR, s.hour);

    printf("| %-12s | %3.0f deg  |\n", "Degree",     s.degree);
    printf("| %-12s | %3.0f days |\n", "Age",        s.age);
    printf("| %-12s | %-8s |\n",       "Sunrise",    rise);
    printf("| %-12s | %-8s |\n",       "Sunset",     set);
    printf("| %-12s | %-8s |\n",       "Season",     season);
    printf("| %-12s | %-8s |\n",       "Beginning",  start);
    printf("| %-12s | %-8s |\n",       "Ending",     end);
    printf("| %-12s | %-8s |\n",       "Day ruler",  day);
    printf("| %-12s | %-8s |\n",       "Hour ruler", hour);
}

static void
print_lunar(struct mona_lunar l)
{
    const char *moon = mona_string(MONA_STRING_MOON, l.phase);

    char start[16], end[16];
    strftime(start, 16, "%d/%m/%y", localtime(&(l.beginning)));
    strftime(end,   16, "%d/%m/%y", localtime(&(l.ending)));

    printf("| %-12s | %3.0f deg         |\n", "Degree",    l.degree);
    printf("| %-12s | %3.0f days        |\n", "Age",       l.age);
    printf("| %-12s | %-15s |\n",             "Phase",     moon);
    printf("| %-12s | %-15s |\n",             "Beginning", start);
    printf("| %-12s | %-15s |\n",             "Ending",    end);
}

static void
print_zodiac(enum mona_object i, struct mona_zodiac z)
{
    printf("| %-12s ",        mona_string(MONA_STRING_OBJECT, i));
    printf("| %-11s ",        mona_string(MONA_STRING_SIGN, z.sign));
    printf("| %-9s ",         mona_string(MONA_STRING_DECAN, z.decan));
    printf("| %02d' %02d\" ", z.degrees, z.seconds);
    printf("| %3.0f deg |\n", z.position);
}

extern int
main(void)
{
    struct mona_waypoint w = mona_waypoint(time(0), LATITUDE, LONGITUDE);

    printf("[ Solar cycle ]\n");
    struct mona_solar s = mona_solar(w);
    print_solar(s);

    printf("\n[ Lunar cycle ]\n");
    struct mona_lunar l = mona_lunar(w);
    print_lunar(l);

    printf("\n[ Seven planets ]\n");
    for (int i = 0; i < MONA_OBJECT_COUNT; i++)
    {
        struct mona_zodiac z = mona_zodiac(w, i);
        print_zodiac(i, z);

        if (i == MONA_OBJECT_SATURN)
            printf("\n[ Midpoints ]\n");
    }

    return 0;
}
```

```sh
gcc test.c -lmona -lm
```
