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

#pragma once

#include <map>
#include <vector>
#include <variant>

namespace moba::json {
    struct JsonValue;

    using JsonObject = std::map<std::string, JsonValue>;

    using JsonArray = std::vector<JsonValue>;

    using JsonNull = std::nullptr_t;

    using JsonBase = std::variant<double, std::string, long, bool, JsonArray, JsonObject, JsonNull>;

    struct JsonValue : public JsonBase {
        template <typename T>
        JsonValue(const T& t) : JsonBase(t) {
        }

        JsonValue() : JsonBase() {
        }
    };
}

