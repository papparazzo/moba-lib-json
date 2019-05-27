/*
 *  Project:    moba-common
 *
 *  Copyright (C) 2016 Stefan Paproth <pappi-@gmx.de>
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

#pragma once

#include <memory>

#include "json.h"
#include "jsonstreamreader.h"

namespace moba::json {

    class JsonException : public std::exception {

        public:
            virtual ~JsonException() throw() {

            }

            JsonException() {

            }

            JsonException(const std::string &what) {
                this->what__ = what;
            }

            virtual const char* what() const throw() {
                return this->what__.c_str();
            }

        private:
            std::string what__;
    };

    class JsonDecoder {

        public:
            JsonDecoder(JsonStreamReaderPtr reader, bool strict = false);
            virtual ~JsonDecoder();

            JsonValue decode();

        protected:
            void checkNext(const char x);
            char next(bool ignoreWhitespace = false);
            std::string next(int n);
            std::string nextKey();

            JsonValue nextValue();

            JsonValue nextNull();
            JsonValue nextTrue();
            JsonValue nextFalse();

            JsonValue nextString();
            JsonValue nextObject();
            JsonValue nextArray();

            JsonValue nextNumber();
            JsonValue parseNumber(std::string s);

            char lastChar;
            JsonStreamReaderPtr reader;
            bool strict;
            static const int  MAX_STRING_LENGTH = 1024;
    };
}
