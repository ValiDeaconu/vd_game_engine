//
// Created by Vali on 11/10/2020.
//

#ifndef VD_GAME_ENGINE_PROPERTIES_HPP
#define VD_GAME_ENGINE_PROPERTIES_HPP

#include <memory>
#include <fstream>
#include <streambuf>
#include <unordered_map>
#include <vector>
#include <string>

#include <glm/glm.hpp>

namespace vd::property {
    class Properties;
    typedef std::shared_ptr<Properties> PropertiesPtr;

    class Properties {
    public:
        Properties();

        /**
         * Returns whether the properties were empty (only empty lines or comment lines)
         * @return true if there is no properties, false otherwise
         */
        [[nodiscard]] bool Empty() const;

        /**
         * Find if a value is assigned to a given key
         * @param key the given key
         * @return true if the value is assigned
         */
        [[nodiscard]] bool Has(const std::string& key) const;

        /**
         * Find the value assigned to a given key and cast it to the desired type
         * @tparam T return type: int, float, vec2, vec3, vec4, string
         * @param key the given key
         * @return the casted value from the property with the given key
         */
        template<typename T> T Get(const std::string& key) const;

        /**
         * Find the value assigned to a given key and returns the raw value
         * @param key the given key
         * @return the value from the property with the given key
         */
        [[nodiscard]] const std::vector<std::string>& Get(const std::string& key) const;

        /**
         * Set a new property with a given key and a list of values
         * @param key where the property is stored
         * @param values of the property
         * @return true if no other property already exists with the given key, false otherwise
         */
        bool Set(const std::string& key, const std::vector<std::string>& values);

        /**
         * Set a new property with a given key and a value
         * @param key where the property is stored
         * @param value of the property
         * @return true if no other property already exists with the given key, false otherwise
         */
        bool Set(const std::string& key, const std::string& value);

        /**
         * Change an old property located at a given key, removing the old one's values, setting new ones
         * If there was no property at the given key, the values are inserted using Properties::Set
         * @param key where the property is stored
         * @param values of the property
         */
        void Change(const std::string& key, const std::vector<std::string>& values);

        /**
         * Change an old property located at a given key, removing the old one's values, setting a new value
         * If there was no property at the given key, the value is inserted using Properties::Set
         * @param key where the property is stored
         * @param value of the property
         */
        void Change(const std::string& key, const std::string& value);
    private:
        std::unordered_map<std::string, std::vector<std::string>> m_Map;
    };
}


#endif //VD_GAME_ENGINE_PROPERTIES_HPP
