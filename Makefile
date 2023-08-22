# This file is part of MonaEphemeris.

# MonaEphemeris is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published
# by the Free Software Foundation, version 3.

# MonaEphemeris is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with MonaEphemeris. If not, see <https://www.gnu.org/licenses/>.

CFLAGS += -O2 -Iinclude -Wall -Wextra

.PHONY: all clean

all: build/libmona.a
clean:
	rm -rf build

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
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build
