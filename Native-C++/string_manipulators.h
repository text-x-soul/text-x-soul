/*
 * This file is a part of the Soul Configuration File Format and is in the public domain.
 *
 * Implemented for the Soul Format: 0.5.2
 *
 * The person who associated a work with this deed has dedicated the work to the public domain by waiving all of his or her rights
 * to the work worldwide under copyright law, including all related and neighboring rights, to the extent allowed by law.
 *
 * You can copy, modify, distribute and perform the work, even for commercial purposes, all without asking permission.
 * More Information: https://text-x-soul.tk/LICENSE
 *
 * string_manipulators.h@libsoul
 */

#ifndef STRING_MANIPULATORS_H_
#define STRING_MANIPULATORS_H_

#include "srell.h"
#include <sstream>

inline bool in_array(const std::string &value, const std::vector<std::string> &array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
}

// trim from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

void remove(std::string &str, std::string value){
	if(value == "(" or value == ")" or value == "{" or value == "}" or value == "?"){
		str = srell::regex_replace(str, srell::regex("\\" + value), "");
	}
	else{
		str = srell::regex_replace(str, srell::regex(value), "");
	}
}

std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

#endif /* STRING_MANIPULATORS_H_ */
