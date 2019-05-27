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

#include "jsondecoder.h"
#include "json.h"

#include <boost/algorithm/string.hpp>
#include <exception>
#include <sstream>
#include <iostream>

namespace moba::json {

    JsonDecoder::JsonDecoder(JsonStreamReaderPtr reader, bool strict) : lastChar(0), reader(reader), strict(strict) {
    }

    JsonDecoder::~JsonDecoder() {

    }

    JsonValue JsonDecoder::decode()  {
        return nextObject();
    }

    std::string JsonDecoder::nextKey() {
        std::stringstream sb;

        for(int i = 0; i < JsonDecoder::MAX_STRING_LENGTH; ++i) {
            char c = next();

            if(isspace(c) || !(isalnum(c) || c == '_' || c == '"')) {
                throw JsonException("key contains invalide char!");
            }

            if(c == '"') {
                std::string s = sb.str();
                boost::algorithm::trim(s);
                if(s.length() == 0) {
                    throw JsonException("key is empty");
                }
                return s;
            }
            sb << c;
        }
        throw JsonException("maximum string-length reached!");
    }

    JsonValue JsonDecoder::nextObject() {
        JsonObject object;
        std::string key;
        char c;

        for(int i = 0; i < JsonDecoder::MAX_STRING_LENGTH; ++i) {
            c = next(!strict);
            switch(c) {
                case '}':
                    return object;

                case '"':
                    key = nextKey();
                    break;

                default:
                    throw JsonException("invalid key");
            }
            checkNext(':');

            if(object.find(key) != object.end()) {
                throw JsonException(std::string("duplicate key <") + key + ">");
            }
            object[key] = nextValue();

            switch(next(!strict)) {
                case ',':
                    break;

                case '}':
                    return object;

                default:
                    throw JsonException("expected a ',' or '}'");
            }
        }
        throw JsonException("maximum string-length reached!");
    }

    JsonValue JsonDecoder::nextValue() {
        char c = reader->peek(!strict);
        switch(c) {
            case 'n':
                return nextNull();

            case 't':
                return nextTrue();

            case 'f':
                return nextFalse();

            case '"':
                return nextString();

            case '{':
                return nextObject();

            case '[':
                return nextArray();

            default:
                return nextNumber();
        }
    }

    JsonValue JsonDecoder::nextNull() {
        reader->checkNext("null", !strict);
        return nullptr;
    }

    JsonValue JsonDecoder::nextTrue() {
        reader->checkNext("true", !strict);
        return true;
    }

    JsonValue JsonDecoder::nextFalse() {
        reader->checkNext("false", !strict);
        return false;
    }

    JsonValue JsonDecoder::nextString() {
        char c;
        std::stringstream sb;
        for(int i = 0; i < JsonDecoder::MAX_STRING_LENGTH; ++i) {
            c = next();
            switch(c) {
                case '\n':
                case '\r':
                    throw JsonException("invalid char");

                case '\\':
                    c = this->next();
                    switch (c) {
                        case 'b':
                            sb << '\b';
                            break;

                        case 't':
                            sb << '\t';
                            break;

                        case 'n':
                            sb << '\n';
                            break;

                        case 'f':
                            sb << '\f';
                            break;

                        case 'r':
                            sb << '\r';
                            break;

                        case 'u':
                            //FIXME sb.append((char)Integer.parseInt(this->next(4), 16));
                            break;

                        case '"':
                        case '\\':
                        case '/':
                            sb << c;
                            break;

                        default:
                            throw JsonException("invalid escape-sequence");
                    }
                    break;

                case '"':
                    return sb.str();

                default:
                    sb << c;
                    break;
            }
        }
        throw JsonException("maximum string-length reached!");
    }

    JsonValue JsonDecoder::nextArray() {
        JsonArray array;

        reader->checkNext('[', !strict);
        char c = reader->peek(!strict);

        if(c == ']') {
            return array;
        }
        lastChar = c;
        array.emplace_back(nextValue());

        while(true) {
            c = reader->next();

            switch(c) {
                case ',':
                    array.emplace_back(nextValue());
                    break;

                case ']':
                    return array;

                default:
                    throw JsonException("expected ',' or ']'");
            }
        }
    }

    JsonValue JsonDecoder::nextNumber() {
        char c;
        std::stringstream sb;
        for(int i = 0; i < JsonDecoder::MAX_STRING_LENGTH; ++i) {
            c = reader->peek(!strict);

            if(c == ',' || c == ']' || c == '}') {
                lastChar = c;
                return parseNumber(sb.str());
            }

            reader->next();

            if((c >= '0' && c <= '9') || c == '-' || c == 'e' || c == 'E' || c == '.' || c == 'x' || c == 'X') {
                sb << c;
                continue;
            }
            throw JsonException("parsing error");
        }
        throw JsonException("maximum string-length reached!");
    }

    JsonValue JsonDecoder::parseNumber(std::string s) {
        boost::algorithm::trim(s);

        if(!s.length()) {
            throw JsonException("empty value");
        }

        char b = s[0];
        if(!(b >= '0' && b <= '9') && b != '-') {
            throw JsonException("parsing error, number starts not with digit or -");
        }

        try {
            if(b == '0' && s.length() > 2 && (s[1] == 'x' || s[1] == 'X')) {
                return strtol(&(s.c_str()[2]), NULL, 16);
            }

            if(s.find('.') != std::string::npos || s.find('e') != std::string::npos || s.find('E') != std::string::npos) {
                std::stringstream buffer;
                double output;
                buffer << s;
                buffer >> output;
                return output;
            }

            return atol(s.c_str());
        } catch(std::exception &e) {
            throw JsonException(std::string("parsing, error could not determine value: <") + std::string(e.what()) + ">");
        }
    }
}
