/*
 *  This file is part of libmona
 *
 *  Libmona is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  Libmona is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with libmona; if not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>

#include <mona/core.h>
#include <mona/string.h>

static const char *SIGNS[] = {
    [MONA_SIGN_ARIES]       = "Aries",
    [MONA_SIGN_TAURUS]      = "Taurus",
    [MONA_SIGN_GEMINI]      = "Gemini",
    [MONA_SIGN_CANCER]      = "Cancer",
    [MONA_SIGN_LEO]         = "Leo",
    [MONA_SIGN_VIRGO]       = "Virgo",
    [MONA_SIGN_LIBRA]       = "Libra",
    [MONA_SIGN_SCORPIO]     = "Scorpio",
    [MONA_SIGN_SAGITTARIUS] = "Sagittarius",
    [MONA_SIGN_CAPRICORN]   = "Capricorn",
    [MONA_SIGN_AQUARIUS]    = "Aquarius",
    [MONA_SIGN_PISCES]      = "Pisces",
};

static const char *DECANS[] = {
    [MONA_DECAN_ASCENDANT] = "Ascendant",
    [MONA_DECAN_SUCCEDENT] = "Succedent",
    [MONA_DECAN_CADENT]    = "Cadent"
};

static const char *DAYS[] = {
    [MONA_DAY_SUN]  =    "Sun",     [MONA_DAY_MOON] =    "Moon",
    [MONA_DAY_MARS] =    "Mars",    [MONA_DAY_MERCURY] = "Mercury",
    [MONA_DAY_JUPITER] = "Jupiter", [MONA_DAY_VENUS]   = "Venus",
    [MONA_DAY_SATURN]  = "Saturn"
};

static const char *HOURS[] = {
    [MONA_HOUR_SUN]  =    "Sun",     [MONA_HOUR_MOON] =    "Moon",
    [MONA_HOUR_MARS] =    "Mars",    [MONA_HOUR_MERCURY] = "Mercury",
    [MONA_HOUR_JUPITER] = "Jupiter", [MONA_HOUR_VENUS]   = "Venus",
    [MONA_HOUR_SATURN]  = "Saturn"
};

static const char *SEASONS[] = {
    [MONA_SEASON_SUMMER] = "Summer", [MONA_SEASON_AUTUMN] = "Autumn",
    [MONA_SEASON_WINTER] = "Winter", [MONA_SEASON_SPRING] = "Spring"
};

static const char *MOONS[] = {
    [MONA_MOON_NEW]             = "New Moon",
    [MONA_MOON_WAXING_CRESCENT] = "Waxing Crescent",
    [MONA_MOON_FIRST_QUARTER]   = "First Quarter",
    [MONA_MOON_WAXING_GIBBOUS]  = "Waxing Gibbous",
    [MONA_MOON_FULL]            = "Full Moon",
    [MONA_MOON_WANING_GIBBOUS]  = "Waning Gibbous",
    [MONA_MOON_THIRD_QUARTER]   = "Third Quarter",
    [MONA_MOON_WANING_CRESCENT] = "Waning Crescent"
};

static const char *OBJECTS[] = {
    [MONA_OBJECT_MOON]    = "Moon",  [MONA_OBJECT_MERCURY] = "Mercury",
    [MONA_OBJECT_VENUS]   = "Venus", [MONA_OBJECT_SUN]     = "Sun",
    [MONA_OBJECT_MARS]    = "Mars",  [MONA_OBJECT_JUPITER] = "Jupiter",
    [MONA_OBJECT_SATURN]  = "Saturn",

    [MONA_OBJECT_ASCENDANT]    = "Ascendant",
    [MONA_OBJECT_DESCENDANT]   = "Descendant",
    [MONA_OBJECT_MEDIUM_COELI] = "Medium Coeli",
    [MONA_OBJECT_IMUM_COELI]   = "Imum Coeli"
};

extern const char *
mona_string(enum mona_string type, uint8_t value)
{
    char *ret = NULL;

    switch (type)
    {
        #define MONA_STRING_CASE(name) \
            case MONA_STRING_##name: \
                if (value < (sizeof(name##S) / sizeof(const char *))) \
                    ret = (char*)name##S[value]; \
                break;

        MONA_STRING_CASE(SIGN)
        MONA_STRING_CASE(DECAN)
        MONA_STRING_CASE(DAY)
        MONA_STRING_CASE(HOUR)
        MONA_STRING_CASE(SEASON)
        MONA_STRING_CASE(MOON)
        MONA_STRING_CASE(OBJECT)
    }

    return ret;
}
