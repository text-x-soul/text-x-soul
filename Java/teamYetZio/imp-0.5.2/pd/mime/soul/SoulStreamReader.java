package pd.mime.soul;

import java.util.Arrays;
import java.util.Collection;
import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;

import java.util.Set;
import java.util.Iterator;

import java.util.regex.Pattern;
import java.util.regex.Matcher;

public class SoulStreamReader{
	
	public final String SOUL_READER_VERSION = "0.5.2";
	
	private String m_filename = "";
	private SoulParser m_SoulParser = new SoulParser();
	private Map<String, String> preprocessed_info = new HashMap<String, String>();
	private Map<String, String[]> groupMap = new HashMap<String, String[]>();
	private Map<List<String>, String> groupVarMap = new HashMap<List<String>, String>();
	private Map<String, String> varMap = new HashMap<String, String>(); 
	
	public enum SoulMaps{
		INFOMAP(0), 
		VARMAP(1);
		
		private final int number;
		SoulMaps(int number){
			this.number = number;
		}
		
		public int getValue() {
			return this.number;
		}
	}
	
	public SoulStreamReader(){
		
	}
	
	public void startStream(){
		
		String contents = "";
		try{
			String cont = new String(Files.readAllBytes(Paths.get(m_filename)), StandardCharsets.UTF_8);
			contents = cont;
		} catch(IOException e){
			e.printStackTrace();
		}
		
		contents = contents.replaceAll(m_SoulParser.commentMatch, "");
		
		String[] souldata = contents.split("\n");
		
		for(String line: souldata){
			Matcher InfoMatcher = this.m_SoulParser.preProMatch.matcher(line);
			if(InfoMatcher.find()){
				line = line.replaceAll("¡", "").replaceAll("Â", "");
				line = line.trim();
				Matcher varmatch = this.m_SoulParser.varMatch.matcher(line);
				if(varmatch.find()){
					this.preprocessed_info.put(varmatch.group(1).trim(), varmatch.group(2).trim());
				}
			}
		}
		
		for(String line: souldata){
			Matcher infmatch = this.m_SoulParser.preProMatch.matcher(line);
			Matcher groupmatch = this.m_SoulParser.groupMatch.matcher(line);
			Matcher groupvarmatch = this.m_SoulParser.groupVarMatch.matcher(line);
			Matcher varmatch = this.m_SoulParser.varMatch.matcher(line);
			
			if(infmatch.find()){
				continue;
			}
			else if(groupmatch.find()){
				continue;
			}
			else if(groupvarmatch.find()){
				continue;
			}
			else if(varmatch.find()){
				line = line.replaceAll("Â", "");
				
				String variable = varmatch.group(1);
				variable = variable.trim();
				
				String variable_value = varmatch.group(2);
				variable_value = variable_value.trim();
				
				this.varMap.put(variable, variable_value);
			}
		}
		
		for(String line: souldata){
			Matcher groupmatch = this.m_SoulParser.groupMatch.matcher(line);
			
			if(groupmatch.find()){
				line = line.replaceAll("Â", "");
				String group_value = groupmatch.group(2);
				group_value = group_value.replaceAll("\\{", "").replaceAll("\\}", "");
				group_value = group_value.trim();
				
				String[] group_value_list = group_value.split(",");
				
				for(int i = 0; i < group_value_list.length; i++){
					group_value_list[i] = group_value_list[i].trim();
				}
				
				String group_var = groupmatch.group(1);
				group_var = group_var.replaceAll("\\(", "").replaceAll("\\)", "");
				group_var = group_var.trim();
				
				this.groupMap.put(group_var, group_value_list);
				
				for(int i = 0; i < group_value_list.length; i++){
					this.groupVarMap.put(
					Collections.unmodifiableList(Arrays.asList(group_value_list[i], group_var)), 
					"NULL"
					);
				}
			}
		}
		
		for(String line: souldata){
			Matcher groupvarmatch = this.m_SoulParser.groupVarMatch.matcher(line);
			if(groupvarmatch.find()){
				line = line.replaceAll("Â", "");
				
				String group_var = groupvarmatch.group(1);
				group_var = group_var.replaceAll("(@\\(.*\\))", "");
				group_var = group_var.trim();
				
				String belongs_To = groupvarmatch.group(2);
				belongs_To = belongs_To.replaceAll("\\(", "").replaceAll("\\)", "");
				belongs_To = belongs_To.trim();
				
				String group_var_value = groupvarmatch.group(3);
				group_var_value = group_var_value.trim();
				
				this.groupVarMap.put(
				Collections.unmodifiableList(Arrays.asList(group_var, belongs_To)), 
				group_var_value
				);
			}
		}
	}
	
	public SoulStreamReader(String filename){
		this.setFileName(filename);
	}
	
	public void setFileName(String filename){
		this.m_filename = filename;
		this.startStream();
	}
	
	public String getFileName(){
		return this.m_filename;
	}
	
	public int typeOf(String key, SoulMaps map){
		int result = 0;
		if(map == SoulMaps.INFOMAP){
			if(this.preprocessed_info.containsKey(key)){
				String val = this.preprocessed_info.get(key);
				Matcher nummatch = this.m_SoulParser.isNumber.matcher(val);
				if(nummatch.find()){
					SoulParser.TypeIdentifiers t = SoulParser.TypeIdentifiers.NUMBER;
					result = t.getValue();
				}
				else{
					SoulParser.TypeIdentifiers t = SoulParser.TypeIdentifiers.INFO;
					result = t.getValue();
				}
			}
			else{
				result = -1;
			}
		}
		else if(map == SoulMaps.VARMAP){
			if(this.varMap.containsKey(key)){
				String val = this.varMap.get(key);
				Matcher nummatch = this.m_SoulParser.isNumber.matcher(val);
				if(nummatch.find()){
					SoulParser.TypeIdentifiers t = SoulParser.TypeIdentifiers.NUMBER;
					result = t.getValue();
				}
				else{
					SoulParser.TypeIdentifiers t = SoulParser.TypeIdentifiers.SOUL_STRING;
					result = t.getValue();
				}
			}
			else{
				result = -1;
			}
		}
		
		return result;
	}
	
	public String soulVersion(){
		if(this.preprocessed_info.containsKey("SOUL_VERSION")){
			return this.preprocessed_info.get("SOUL_VERSION");
		}
		else{
			return "NULL";
		}
	}
	
	public String getVarValue(String variable){
		if(this.varMap.containsKey(variable)){
			return this.varMap.get(variable);
		}
		else{
			return "NULL";
		}
	}
	
	public String[] groupDef(String group){
		if(this.groupMap.containsKey(group)){
			return this.groupMap.get(group);
		}
		else{
			String[] blankrsponse = {};
			return blankrsponse;
		}
	}
	
	public String groupParams(String group, int index){
		if(this.groupMap.containsKey(group)){
			return this.groupMap.get(group)[index];
		}
		else{
			return "NULL";
		}
	}
	
	public String getGrpValue(String group, String param){
		if(this.groupVarMap.containsKey(Arrays.asList(param, group))){
			return this.groupVarMap.get(Arrays.asList(param, group));
		}
		else{
			return "NULL";
		}
	}
	
	public Map<String, String> infoMap(){
		return this.preprocessed_info;
	}
	
	public Map<String, String> valueMap(){
		return this.varMap;
	}
	
	public Map<String, String[]> grpMap(){
		return this.groupMap;
	}
	
	public Map<List<String>, String> grpVarMap(){
		return this.groupVarMap;
	}
}
