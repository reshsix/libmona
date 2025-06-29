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

#ifndef MONA_STRING_H
#define MONA_STRING_H

#include <stdint.h>

enum mona_string
{
    MONA_STRING_SIGN,
    MONA_STRING_DECAN,
    MONA_STRING_DAY,
    MONA_STRING_HOUR,
    MONA_STRING_SEASON,
    MONA_STRING_MOON,
    MONA_STRING_OBJECT
};

const char *mona_string(enum mona_string type, uint8_t value);

#endif
