package pd.mime.soul;

import java.util.regex.*;

public class SoulParser{
	
	public final String MEDIA_TYPE_SOUL = "text/soul";
	public final String SOUL_PARSER_VERSION = "0.5.2";
	public final String SOUL_UTI = "public.soul";
	
	public String[] soul_confextensions = new String[]{"soul", "SOUL", "sOUL", "SoUL", "SOuL", "SOUl"};
	
	public enum TypeIdentifiers{
		UNKNOWN(0), 
		SOUL_STRING(1), 
		NUMBER(2), 
		GROUP(3), 
		COMMENTS(4), 
		INFO(5);
		
		private final int number;
		TypeIdentifiers(int number){
			this.number = number;
		}
		
		public int getValue() {
			return this.number;
		}
	}
	
	public SoulParser(){
		
	}
	
	public Pattern isNumber = Pattern.compile("\\d+");
	public Pattern preProMatch = Pattern.compile("(?<=¡)(.*)(?=¡)");
	public Pattern varMatch = Pattern.compile("([^\\s]+)\\s*=\\s*(.*)");
	public Pattern groupMatch = Pattern.compile("([^\\s]+[()])\\s*=\\s*(\\{.*\\})");
	public Pattern groupVarMatch = Pattern.compile("([^\\s]+@(\\(.*\\)))\\s*=\\s*(.*)");
	public String commentMatch = "¿([^?]+)\\?";
	
}
