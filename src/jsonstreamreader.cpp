/*
 *  Project:    moba-lib-json
 *
 *  Copyright (C) 2019 Stefan Paproth <pappi-@gmx.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/agpl.txt>.
 *
 */

#include "jsonstreamreader.h"
#include <sstream>

namespace moba::json {

    JsonStreamReader::~JsonStreamReader() {
    }

    char JsonStreamReader::peek() {
        return peek(false);
    }

    char JsonStreamReader::peek(bool ignoreWhitespace) {
        auto c = next(ignoreWhitespace);
        lastChar = c;
        return c;
    }

    void JsonStreamReader::checkNext(const std::string &s) {
        checkNext(s, false);
    }

    void JsonStreamReader::checkNext(const std::string &s, bool ignoreWhitespace) {
        for (auto const &c : s) {
            checkNext(c, ignoreWhitespace);
        }
    }

    void JsonStreamReader::checkNext(char x) {
        checkNext(x, false);
    }

    void checkNext(char x, bool ignoreWhitespace) {
        auto c = next(ignoreWhitespace);
        if(c != x) {
            throw JsonStreamReaderException(std::string("expected '") + std::string(1, x) + "' found '" + std::string(1, c) + "'!");
        }
    }

    char JsonStreamReader::next() {
        return next(false);
    }

    char JsonStreamReader::next(bool ignoreWhitespace) {
        if(lastChar != 0) {
            auto t = lastChar;
            lastChar = 0;
            return t;
        }
        char c;
        do {
            c = read();
        } while(::isspace(c) && ignoreWhitespace);

        if(c == -1 || c == 0) {
            throw JsonStreamReaderException("input stream corrupted!");
        }
        return (char)c;
    }

    std::string next(int n, bool ignoreWhitespace) {
        std::stringstream ss;

        if(n == 0) {
            return "";
        }

        for(int i = 0; i < n; ++i) {
            ss << next(ignoreWhitespace);
        }
        return ss.str();
    }

    std::string  next(int n) {
        return next(n, false);
    }
}

