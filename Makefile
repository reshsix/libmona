#
#  This file is part of libmona
#
#  Libmona is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#
#  Libmona is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#  See the GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with libmona; if not, see <https://www.gnu.org/licenses/>.
#

CFLAGS += --std=c99 -Iinclude -Wall -Wextra

.PHONY: all debug clean install uninstall

TARGETS = build/libmaid.a build/libmaid.so build/maid
all: CFLAGS += -march=native -O3 -DNDEBUG=1
all: build/libmona.a
clean:
	rm -rf build

debug: CFLAGS += -Og -pg -ggdb3
debug: build/libmona.a

install: include/mona build/libmona.a
	cp -r include/mona    "$(DESTDIR)/usr/include/"
	cp    build/libmona.a "$(DESTDIR)/usr/local/lib/"
uninstall:
	rm -rf "$(DESTDIR)/usr/include/mona/"
	rm -rf "$(DESTDIR)/usr/local/lib/libmona.a"

build/libmona.a: build/core.o build/string.o | build
	ar ruv $@ $^
	ranlib $@

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@ -lm

build:
	mkdir -p build
