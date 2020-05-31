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
 * SoulParser.h@libsoul
 */

#ifndef SOULPARSER_H_
#define SOULPARSER_H_

#include <string>
#include "srell.h"

namespace Soul{

#define MEDIA_TYPE_SOUL = "text/soul"
#define SOUL_PARSER_VERSION = "0.5.2"
#define SOUL_UTI = "public.soul"

inline std::vector<std::string> soul_confextensions {"soul", "SOUL", "sOUL", "SoUL", "SOuL", "SOUl"};

class SoulParser{
public:
	SoulParser();

	enum TypeIdentifiers{
        UNKNOWN = 0,
        SOUL_STRING = 1,
        NUMBER = 2,
        GROUP = 3,
        COMMENTS = 4,
        INFO = 5
	};

	srell::regex isNumber = srell::regex("\\d*");
	srell::regex preProMatch = srell::regex("(?<=¡)(.*)(?=¡)");
	srell::regex varMatch = srell::regex("([^\\s]+)\\s*=\\s*(.*)");
	srell::regex groupMatch = srell::regex("([^\\s]+[()])\\s*=\\s*(\\{.*\\})");
	srell::regex groupVarMatch = srell::regex("([^\\s]+@(\\(.*\\)))\\s*=\\s*(.*)");
	srell::regex commentMatch = srell::regex("¿([\\S\\s]*?)\\?");
};

}

#endif /* SOULPARSER_H_ */
