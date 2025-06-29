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

#ifndef MONA_CORE_H
#define MONA_CORE_H

#include <time.h>
#include <stdint.h>

enum mona_sign
{
    MONA_SIGN_ARIES,
    MONA_SIGN_TAURUS,
    MONA_SIGN_GEMINI,
    MONA_SIGN_CANCER,
    MONA_SIGN_LEO,
    MONA_SIGN_VIRGO,
    MONA_SIGN_LIBRA,
    MONA_SIGN_SCORPIO,
    MONA_SIGN_SAGITTARIUS,
    MONA_SIGN_CAPRICORN,
    MONA_SIGN_AQUARIUS,
    MONA_SIGN_PISCES
};

enum mona_decan
{
    MONA_DECAN_ASCENDANT,
    MONA_DECAN_SUCCEDENT,
    MONA_DECAN_CADENT
};

enum mona_day
{
    MONA_DAY_SUN,
    MONA_DAY_MOON,
    MONA_DAY_MARS,
    MONA_DAY_MERCURY,
    MONA_DAY_JUPITER,
    MONA_DAY_VENUS,
    MONA_DAY_SATURN
};

enum mona_hour
{
    MONA_HOUR_SATURN,
    MONA_HOUR_JUPITER,
    MONA_HOUR_MARS,
    MONA_HOUR_SUN,
    MONA_HOUR_VENUS,
    MONA_HOUR_MERCURY,
    MONA_HOUR_MOON
};

enum mona_season
{
    MONA_SEASON_SPRING,
    MONA_SEASON_SUMMER,
    MONA_SEASON_AUTUMN,
    MONA_SEASON_WINTER
};

enum mona_moon
{
    MONA_MOON_NEW,
    MONA_MOON_WAXING_CRESCENT,
    MONA_MOON_FIRST_QUARTER,
    MONA_MOON_WAXING_GIBBOUS,
    MONA_MOON_FULL,
    MONA_MOON_WANING_GIBBOUS,
    MONA_MOON_THIRD_QUARTER,
    MONA_MOON_WANING_CRESCENT
};

enum mona_object
{
    MONA_OBJECT_MOON,
    MONA_OBJECT_MERCURY,
    MONA_OBJECT_VENUS,
    MONA_OBJECT_SUN,
    MONA_OBJECT_MARS,
    MONA_OBJECT_JUPITER,
    MONA_OBJECT_SATURN,

    MONA_OBJECT_ASCENDANT,
    MONA_OBJECT_DESCENDANT,
    MONA_OBJECT_MEDIUM_COELI,
    MONA_OBJECT_IMUM_COELI,

    MONA_OBJECT_COUNT
};

struct mona_waypoint
{
    double j2k, latitude, longitude;
    double X, Y, T;
};

struct mona_waypoint mona_waypoint(time_t utc,
                                   double latitude, double longitude);

struct mona_zodiac
{
    double position;
    enum mona_sign sign;
    enum mona_decan decan;
    uint8_t degrees, seconds;
};

struct mona_lunar
{
    double degree, age;
    enum mona_moon phase;
    time_t beginning, ending;
};

struct mona_solar
{
    double degree, age;
    enum mona_season season;
    time_t beginning, ending;
    time_t sunrise, sunset;
    enum mona_day day;
    enum mona_hour hour;
};

struct mona_zodiac mona_zodiac(struct mona_waypoint p, enum mona_object o);
struct mona_lunar mona_lunar(struct mona_waypoint p);
struct mona_solar mona_solar(struct mona_waypoint p);

#endif
