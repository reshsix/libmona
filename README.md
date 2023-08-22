# MonaEphemeris
Astrology library for embedded devices

# Build
Static library is created in build/libmona.a. Can be installed with the headers
if wanted
```sh
make
sudo make install
```

# Example
Set LATITUDE and LONGITUDE values
```c
#include <time.h>
#include <stdio.h>

#include <mona/core.h>
#include <mona/string.h>

static void
print_solar(struct mona_chart *c)
{
    char rise[8], set[8];
    strftime(rise,  8,  "%H:%M",    localtime(&(c->solar.sunrise)));
    strftime(set,   8,  "%H:%M",    localtime(&(c->solar.sunset)));

    const char *season = mona_string(c->solar.phase, MONA_STRING_SEASON);

    char start[16], end[16];
    strftime(start, 16, "%d/%m/%y", localtime(&(c->solar.beginning)));
    strftime(end,   16, "%d/%m/%y", localtime(&(c->solar.ending)));

    const char *day  = mona_string(c->solar.day,  MONA_STRING_DAY);
    const char *hour = mona_string(c->solar.hour, MONA_STRING_HOUR);

    printf("[ Solar cycle ]\n");
    printf("| %-12s | %3.0f deg  |\n", "Degree",     c->solar.degree);
    printf("| %-12s | %3.0f days |\n", "Age",        c->solar.age);
    printf("| %-12s | %-8s |\n",       "Sunrise",    rise);
    printf("| %-12s | %-8s |\n",       "Sunset",     set);
    printf("| %-12s | %-8s |\n",       "Season",     season);
    printf("| %-12s | %-8s |\n",       "Beginning",  start);
    printf("| %-12s | %-8s |\n",       "Ending",     end);
    printf("| %-12s | %-8s |\n",       "Day ruler",  day);
    printf("| %-12s | %-8s |\n",       "Hour ruler", hour);
}

static void
print_lunar(struct mona_chart *c)
{
    const char *moon = mona_string(c->lunar.phase, MONA_STRING_MOON);

    char start[16], end[16];
    strftime(start, 16, "%d/%m/%y", localtime(&(c->lunar.beginning)));
    strftime(end,   16, "%d/%m/%y", localtime(&(c->lunar.ending)));

    printf("\n[ Lunar cycle ]\n");
    printf("| %-12s | %3.0f deg         |\n", "Degree",    c->lunar.degree);
    printf("| %-12s | %3.0f days        |\n", "Age",       c->lunar.age);
    printf("| %-12s | %-15s |\n",             "Phase",     moon);
    printf("| %-12s | %-15s |\n",             "Beginning", start);
    printf("| %-12s | %-15s |\n",             "Ending",    end);
}

static void
print_signs(struct mona_chart *c)
{
    printf("\n[ Seven planets ]\n");
    for (int i = 0; i < MONA_OBJECT_COUNT; i++)
    {
        struct mona_object *o = &(c->objects[i]);
        printf("| %-12s ",        mona_string(o->id, MONA_STRING_OBJECT));
        printf("| %-11s ",        mona_string(o->sign, MONA_STRING_SIGN));
        printf("| %-9s ",         mona_string(o->decan, MONA_STRING_DECAN));
        printf("| %02d' %02d\" ", o->degrees, o->seconds);
        printf("| %3.0f deg |\n", o->position);

        if (o->id == MONA_OBJECT_SATURN)
            printf("\n[ Midpoints ]\n");
    }
}

extern int
main(void)
{
    struct mona_chart c = {0};
    mona_chart(&c, time(0), LATITUDE, LONGITUDE);

    print_solar(&c);
    print_lunar(&c);
    print_signs(&c);
}
```

```sh
gcc test.c -lmona -lm
```
