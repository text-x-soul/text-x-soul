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
 * SoulStreamReader.cpp@libsoul
 */

#include "SoulStreamReader.h"
#include "string_manipulators.h"
#include <iostream>

namespace Soul{

SoulStreamReader::SoulStreamReader(){
	m_SoulParser = new SoulParser;
}

SoulStreamReader::SoulStreamReader(const std::string &filename){
	setFileName(filename);
}

void SoulStreamReader::startStream(){
	m_SoulParser = new SoulParser;
	std::ifstream f(m_filename);
	std::string str;

	if(f){
		std::ostringstream ss;
		ss << f.rdbuf();
		str = ss.str();
	}
	else{
		std::cout << "SOUL: No Read Access" << std::endl;
		return;
	}
	f.close();

	std::string souldata = srell::regex_replace(str, srell::regex("¿([\\S\\s]*?)\\?"), "");
	std::istringstream lines( souldata );
	std::istringstream varlines( souldata );
	std::istringstream grouplines( souldata );
	std::istringstream groupvarlines( souldata );
	std::string line;

	// Pre-processed information or SOUL_REGISTERS
	while(getline(lines, line)){
		bool infmatch = srell::regex_search(line, m_SoulParser->preProMatch);
		if(infmatch){
			srell::cmatch infoMatch;
			remove(line, "¡");
			remove(line, "Â");
			line = trim(line);
			bool varmatch = srell::regex_search(line.c_str(), infoMatch, m_SoulParser->varMatch);
			if(varmatch){
				preprocessed_info[infoMatch.str(1)] = infoMatch.str(2);
			}
		}
	}

	std::string varline;
	// Variables
	while(getline(varlines, varline)){
		srell::cmatch varMatch;
		bool infmatch = srell::regex_search(varline, m_SoulParser->preProMatch);
		bool groupmatch = srell::regex_search(varline, m_SoulParser->groupMatch);
		bool groupvarmatch = srell::regex_search(varline, m_SoulParser->groupVarMatch);
		bool varmatch = srell::regex_search(varline, m_SoulParser->varMatch);

		if(infmatch){
			continue;
		}
		else if(groupmatch){
			continue;
		}
		else if(groupvarmatch){
			continue;
		}
		else if(varmatch){
			remove(varline, "Â");
			srell::regex_search(varline.c_str(), varMatch, m_SoulParser->varMatch);
			std::string variable = varMatch.str(1);
			variable = trim(variable);

			std::string variable_value = varMatch.str(2);
			variable_value = trim(variable_value);
			varMap[variable] = variable_value;
		}
	}

	std::string grpline;
	// Groups
	while(getline(grouplines, grpline)){
		srell::cmatch groupMatch;
		bool groupmatch = srell::regex_search(grpline.c_str(), groupMatch, m_SoulParser->groupMatch);
		if(groupmatch){
			remove(grpline, "Â");
			std::string group_value = groupMatch.str(2);
			remove(group_value, "{");
			remove(group_value, "}");
			group_value = trim(group_value);

			std::vector<std::string> group_value_list = split(group_value, ',');
			for(int i = 0; i < group_value_list.size(); i++){
				group_value_list[i] = trim(group_value_list[i]);
			}

			std::string group_var = groupMatch.str(1);
			remove(group_var, "(");
			remove(group_var, ")");
			group_var = trim(group_var);
			groupMap[group_var] = group_value_list;

			for(int i = 0; i < group_value_list.size(); i++){
				std::vector<std::string> groupIdentity;
				groupIdentity.insert(groupIdentity.begin(), group_value_list.at(i));
				groupIdentity.insert(groupIdentity.begin() + 1, group_var);
				groupVarMap[groupIdentity] = "NULL";
			}
		}
	}

	std::string grpVarline;
	// Group Variables or param values
	while(getline(groupvarlines, grpVarline)){
		srell::cmatch groupVarMatch;
		bool groupvarmatch = srell::regex_search(grpVarline.c_str(), groupVarMatch, m_SoulParser->groupVarMatch);
		if(groupvarmatch){
			remove(grpVarline, "Â");
			std::string group_var = groupVarMatch.str(1);
			group_var = srell::regex_replace(group_var, srell::regex("(@\\(.*\\))"), "");
			group_var = trim(group_var);

			std::string belongs_To = groupVarMatch.str(2);
			remove(belongs_To, "(");
			remove(belongs_To, ")");
			belongs_To = trim(belongs_To);

			std::string group_var_value = groupVarMatch.str(3);
			group_var_value = trim(group_var_value);

			std::vector<std::string> groupIdentity;
			groupIdentity.insert(groupIdentity.begin(), group_var);
			groupIdentity.insert(groupIdentity.begin() + 1, belongs_To);
			groupVarMap[groupIdentity] = group_var_value;
		}
	}

}

void SoulStreamReader::setFileName(std::string filename){
    m_filename = filename;
    startStream();
}

std::string SoulStreamReader::getFileName(){
	return m_filename;
}

int SoulStreamReader::typeOf(std::string key, SoulMaps map){
	int result = 0;
	if(map == INFOMAP){
		if(!(preprocessed_info.find(key) == preprocessed_info.end())){
			std::string val = preprocessed_info.at(key);
			if(srell::regex_search(val, m_SoulParser->isNumber)){
				result = m_SoulParser->NUMBER;
			}
			else{
				result = m_SoulParser->INFO;
			}
		}
		else{
			return -1;
		}
	}
	else if(map == VARMAP){
		if(!(varMap.find(key) == varMap.end())){
			std::string val = varMap.at(key);
			if(srell::regex_search(val, m_SoulParser->isNumber)){
				result = m_SoulParser->NUMBER;
			}
			else{
				result = m_SoulParser->SOUL_STRING;
			}
		}
		else{
			return -1;
		}
	}
	else{
		return -1;
	}

	return result;
}

std::string SoulStreamReader::soulVersion() const {
	if(!(preprocessed_info.find("SOUL_VERSION") == preprocessed_info.end())){
		return preprocessed_info.at("SOUL_VERSION");
	}
	else{
		return std::string("NULL");
	}
}

std::string SoulStreamReader::getVarValue(std::string key){
	if(!(varMap.find(key) == varMap.end())){
		return varMap.at(key);
	}
	else{
		return std::string("NULL");
	}
}

std::vector<std::string> SoulStreamReader::groupDef(std::string key){
	if(!(groupMap.find(key) == groupMap.end())){
		return groupMap.at(key);
	}
	else{
		return std::vector<std::string>();
	}
}

std::string SoulStreamReader::groupParams(std::string key, int index){
	if(!(groupMap.find(key) == groupMap.end())){
		return groupMap.at(key).at(index);
	}
	else{
		return std::string("NULL");
	}
}

std::string SoulStreamReader::getGrpValue(std::string group, std::string param){
	std::vector<std::string> groupKey;
	groupKey.push_back(param);
	groupKey.push_back(group);

	if(!(groupVarMap.find(groupKey) == groupVarMap.end())){
		return groupVarMap.at(groupKey);
	}
	else{
		return std::string("NULL");
	}
}

std::map<std::string, std::string> SoulStreamReader::infoMap(){
	return preprocessed_info;
}

std::map<std::string, std::string> SoulStreamReader::valueMap(){
	return varMap;
}

std::map<std::string, std::vector<std::string>> SoulStreamReader::grpMap(){
	return groupMap;
}

std::map<std::vector<std::string>, std::string> SoulStreamReader::grpVarMap(){
	return groupVarMap;
}


}
