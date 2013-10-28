package com.odap.verifyer;

public class OdapMetaBuilder {
	
	private String		property;
	private String		problem;
	private String		descript;
	private String		exam;
	private String		result;
	
	/**
	 * 
	 */
	public OdapMetaBuilder(){
		
	}
	
	public OdapMetaBuilder clear(){
		
		property = "";
		problem = "";
		descript = "";
		exam = "";
		
		return this;
	}
	
	public OdapMetaBuilder setProperty( String id, String value ){
		property = property + "\t<" + id + ">" + value  + "</" + id + ">\n";
		return this;
	}
	
	public OdapMetaBuilder addProblem( int x, int y, int width, int height ){
		
		problem += "\t<element x='"+x+"' y='"+y+"' w='"+width+"' h='"+height+"' />\n";
		
		return this;
	}
	
	public OdapMetaBuilder addDescript( int x, int y, int width, int height ){
		
		descript += "\t<element x='"+x+"' y='"+y+"' w='"+width+"' h='"+height+"' />\n";
		
		return this;
	}
	
	public OdapMetaBuilder addExam(int num, int x, int y, int width, int height ){
		
		exam  += "\t<element num='"+num+"' x='"+x+"' y='"+y+"' w='"+width+"' h='"+height+"' />\n";
		
		return this;
	}
	
	public String toXMLString(){
		
		result = "<odap>\n";
		result += "<property>\n" + property + "</property>\n";
		result += "<problem>\n" + problem + "</problem>\n";
		result += "<descript>\n" + descript + "</descript>\n";
		result += "<example>\n" + exam + "</example>\n";
		result += "</odap>";
		
		return result;
	}

}
