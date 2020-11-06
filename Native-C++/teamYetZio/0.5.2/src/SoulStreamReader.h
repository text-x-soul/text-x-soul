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
 * SoulStreamReader.h@libsoul
 */

#ifndef SOULSTREAMREADER_H_
#define SOULSTREAMREADER_H_

#include "SoulParser.h"
#include <map>
#include <fstream>
#include <sstream>

#define SOUL_READER_VERSION = "0.5.2"

namespace Soul{

class SoulStreamReader{
public:
	SoulStreamReader();
	SoulStreamReader(const std::string &filename);

    enum SoulMaps{
        INFOMAP,
        VARMAP
    };

    /// @brief Function that runs on constructing or reading a soul configuration file.
    void startStream();

    /// @brief Returns a QSoulParser::TypeIdentifier int value for a value identified by its key.
    int typeOf(std::string key, SoulMaps map);

    /// @brief Sets the current Soul File Document name.
    void setFileName(std::string filename);

    /// @brief Returns the current Soul File Document name.
    std::string getFileName();

    /// @brief Returns the Soul File Document version.
    std::string soulVersion() const;

    /// @brief Returns the value in variable map identified by its key. The key is the variable.
    std::string getVarValue(std::string key);

    /// @brief Returns the group definition identified by its key. The key is the group name.
    std::vector<std::string> groupDef(std::string key);

    /// @brief Returns the group parameters identified by its key at index. The key is the group name.
    std::string groupParams(std::string key, int index);

    /// @brief Returns the Soul File Document Pre-Processed Information map.
    std::map<std::string, std::string> infoMap();

    /// @brief Returns the Soul File Document value or variable map.
    std::map<std::string, std::string> valueMap();

    /// @brief Returns the Soul File Document group map.
    std::map<std::string, std::vector<std::string>> grpMap();

    /// @brief Returns the Soul File Document group Variable map.
    std::map<std::vector<std::string>, std::string> grpVarMap();

    /// @brief Returns the Soul File Document group values.
    std::string getGrpValue(std::string group, std::string param);
private:
    std::string m_filename;
    SoulParser *m_SoulParser;

    std::map<std::string, std::string> preprocessed_info;
    std::map<std::string, std::vector<std::string>> groupMap;
    std::map<std::vector<std::string>, std::string> groupVarMap;
    std::map<std::string, std::string> varMap;
};

}

#endif /* SOULSTREAMREADER_H_ */
