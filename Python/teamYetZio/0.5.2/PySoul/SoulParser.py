from enum import Enum

MEDIA_TYPE_SOUL = "text/soul"
SOUL_PARSER_VERSION = "0.5.2"
SOUL_UTI = "public.soul"

soul_confextensions = ["soul", "SOUL", "sOUL", "SoUL", "SOuL", "SOUl"]

class TypeIdentifiers(Enum):
    UNKNOWN = 0
    SOUL_STRING = 1
    NUMBER = 2
    GROUP = 3
    COMMENTS = 4
    INFO = 5

class SoulParser:

    '''
    SoulParser class provides regular expressions for searching and parsing
    quantities.
    '''
    
    isNumber = "\\d+"
    preProMatch = "(?<=¡)(.*)(?=¡)"
    varMatch = "([^\\s]+)\\s*=\\s*(.*)"
    groupMatch = "([^\\s]+[()])\\s*=\\s*(\\{.*\\})"
    groupVarMatch = "([^\\s]+@(\\(.*\\)))\\s*=\\s*(.*)"
    commentMatch = "¿([\\S\\s]*?)\\?"
