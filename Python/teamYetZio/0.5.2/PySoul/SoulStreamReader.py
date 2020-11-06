import re
import logging
import SoulParser

# The latest soul file format supported by this Python implementation of reader.
SOUL_READER_VERSION = "0.5.2"

class SoulMaps(SoulParser.Enum):
    '''
    This enum is used to help the user check up values present in a
    particular map.
    '''
    INFOMAP = 0
    VARMAP = 1

class SoulStreamReader:
    '''
    The Reader class for reading stream of a soul file and then
    parsing it with SoulParser class using regex to the current
    Soul File Format.
    '''

    # current filename
    m_filename = ""

    # SoulParser attribute
    m_SoulParser = SoulParser.SoulParser()

    '''
    Maps
    '''
    preprocessed_info = {} # This is used for SOUL_Registers.
    groupMap = {} # This is used for Group definitions.
    groupVarMap = {} # This is used for Group variables or GROUP_VARS.
    varMap = {} # This is used for Variables.
    
    def __init__(self, filename = ""):
        self.setFileName(filename)

    # This method runs everytime a soul file is configured with the current
    # SoulStreamReader object. This can occur on setting filenames or while
    # initializing one.
    def startStream(self):
        try:
            file = open(self.m_filename, "r")
            contents = file.read()
            file.close()
        except IOError:
            logging.error('SOUL: No Read Access')
            return

        ''' For ignoring comments, remove all of them while reading
            using regex provided by SoulParser '''
        souldata = re.sub(self.m_SoulParser.commentMatch, '', contents)

        for line in souldata.split('\n'):
            infmatch = re.search(self.m_SoulParser.preProMatch, line)
            if(infmatch):
                line = line.replace("¡", "")
                line = line.replace("Â", "")
                line = line.strip()
                varmatch = re.search(self.m_SoulParser.varMatch, line)
                if(varmatch):
                    infos = re.findall(self.m_SoulParser.varMatch, line)
                    self.preprocessed_info[infos[0][0]] = infos[0][1]

        for varline in souldata.split('\n'):
            infmatch = re.search(self.m_SoulParser.preProMatch, varline)
            groupmatch = re.search(self.m_SoulParser.groupMatch, varline)
            groupvarmatch = re.search(self.m_SoulParser.groupVarMatch, varline)
            varmatch = re.search(self.m_SoulParser.varMatch, varline)

            if(infmatch):
                continue
            
            elif(groupmatch):
                continue

            elif(groupvarmatch):
                continue

            elif(varmatch):
                varline = varline.replace("Â", "")
                VARS = re.findall(self.m_SoulParser.varMatch, varline)

                variable = VARS[0][0].strip()
                variable_value = VARS[0][1].strip()

                self.varMap[variable] = variable_value

        for grpline in souldata.split('\n'):
            groupmatch = re.search(self.m_SoulParser.groupMatch, grpline)
            grpline = grpline.replace("Â", "")
            if(groupmatch):
                grpmatch = re.findall(self.m_SoulParser.groupMatch, grpline)
                group_value = grpmatch[0][1].replace("{", "").replace("}", "")
                group_value = group_value.strip()

                group_value_list = group_value.split(',')

                # for stripping all group params
                for i in range(len(group_value_list)):
                    group_value_list[i] = group_value_list[i].strip()

                group_var = grpmatch[0][0].replace("(", "").replace(")", "")
                group_var = group_var.strip()
                self.groupMap[group_var] = group_value_list

                for i in range(len(group_value_list)):
                    groupIdentity = []
                    groupIdentity.insert(0, group_value_list[0])
                    groupIdentity.insert(1, group_value_list[1])
                    self.groupVarMap[tuple(groupIdentity)] = "NULL"

        for grpVarLine in souldata.split('\n'):
            groupvarmatch = re.search(self.m_SoulParser.groupVarMatch, grpVarLine)
            grpVarLine = grpVarLine.replace("Â", "")
            if(groupvarmatch):
                grpvarmatch = re.findall(self.m_SoulParser.groupVarMatch, grpVarLine)
                group_var = grpvarmatch[0][0]
                group_var = re.sub(r'(@\(.*\))', "", group_var).strip()

                belongs_To = grpvarmatch[0][1].replace("(", "").replace(")", "")
                belongs_To = belongs_To.strip()

                group_var_value = grpvarmatch[0][2].strip()

                groupIdentity = []
                groupIdentity.insert(0, group_var)
                groupIdentity.insert(1, belongs_To)
                self.groupVarMap[tuple(groupIdentity)] = group_var_value

    # Sets the current filename
    def setFileName(self, filename):
        self.m_filename = filename
        self.startStream()

    # Returns the current filename
    def getFileName(self):
        return self.m_filename

    # Returns the type of value in Map
    def typeOf(self, key, mapvalue):
        result = 0
        if(mapvalue == SoulMaps.INFOMAP):
            if(key in self.preprocessed_info.keys()):
                val = self.preprocessed_info[key]
                if(re.search(self.m_SoulParser.isNumber, val)):
                    result = SoulParser.TypeIdentifiers.NUMBER
                else:
                    result = SoulParser.TypeIdentifiers.INFO
            else:
                result = -1
        elif(mapvalue == SoulMaps.VARMAP):
            if(key in self.varMap.keys()):
                val = self.varMap[key]
                if(re.search(self.m_SoulParser.isNumber, val)):
                    result = SoulParser.TypeIdentifiers.NUMBER
                else:
                    result = SoulParser.TypeIdentifiers.SOUL_STRING
            else:
                result = -1
        else:
            result = -1
        
        return result

    # Returns the Soul Version of the current soul document
    def soulVersion(self):
        if("SOUL_VERSION" in self.preprocessed_info.keys()):
            return self.preprocessed_info["SOUL_VERSION"]
        else:
            return "NULL"

    # Returns the variable value
    def getVarValue(self, key):
        if(key in self.varMap.keys()):
            return self.varMap[key]
        else:
            return "NULL"

    # Returns the group Definition
    def groupDef(self, key):
        if(key in self.groupMap.keys()):
            return self.groupMap[key]
        else:
            dumplist = []
            return dumplist

    # Returns the group params/parameters
    def groupParams(self, key, index):
        if(key in self.groupMap.keys()):
            return self.groupMap[key][index]
        else:
            return "NULL"

    # Returns the group value of a particular param
    def getGrpValue(self, group, param):
        groupKey = []
        groupKey.insert(0, param)
        groupKey.insert(1, group)

        if(tuple(groupKey) in self.groupVarMap.keys()):
            return self.groupVarMap[tuple(groupKey)]
        else:
            return "NULL"

    # Returns the SOUL_Registers map
    def infoMap(self):
        return self.preprocessed_info

    # Returns the variable map
    def valueMap(self):
        return self.varMap

    # Returns the Group map
    def grpMap(self):
        return self.groupMap

    # Returns the Group-Variable map
    def grpVarMap(self):
        return self.groupVarMap

