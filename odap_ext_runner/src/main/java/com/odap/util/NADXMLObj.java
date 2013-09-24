package com.odap.util;

import java.io.CharArrayReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

import com.sun.org.apache.xerces.internal.parsers.DOMParser;

/** 
 * 
 * XML �� �뚯떛 �섏뿬 e4x � 鍮꾩듂�섍쾶 Access �섍쾶 �댁＜�� �대옒��
 * 
 * @author Administrator
 *
 */
public class NADXMLObj {
		
	/**
	 * �앹꽦�� ( create 怨꾩뿴 �⑥닔濡쒕쭔 �앹꽦 �� �� �덈룄濡� �섍린 �꾪븿
	 */
	private NADXMLObj( ){
		
	}
	
	/**
	 * �대� 媛앹껜 蹂�� 由ъ뒪��
	 */
	private String tagName = "";
	private String content = "";
	private Map<String, String> attribute_list = new TreeMap<String, String>();
	private Map<String, List<NADXMLObj> > obj_list = new TreeMap<String, List<NADXMLObj> >();
	
	/** �몃� �명꽣 �섏씠�� **/
	
	public String getTagName(){
		return tagName;
	}
	public String getContent(){
		return content;
	}
	
	
	
	
	/**
	 * �몃뱶�� �띿꽦 媛믪쓣 由ы꽩 �쒕떎.
	 * @param id
	 * @return
	 */
	public String getAttribute( String id ){
		if( attribute_list.containsKey( id ) )
			return attribute_list.get( id );
		return "";
	}
	/**
	 * 
	 * �깅줉�� AttributeID 由ъ뒪�몃� 由ы꽩 �쒕떎.
	 * 
	 * @return
	 */
	public Set<String> getAttributeIdList(){
		return attribute_list.keySet();
	}
	
	/**
	 * 
	 * �대떦 ID瑜� 媛吏� �먯떇 �몃뱶瑜� List �뺥깭濡� 由ы꽩 �쒕떎.
	 * 
	 * @param id
	 * @return
	 */
	public List<NADXMLObj> getChilds( String id ){
		if( obj_list.containsKey( id ) )
			return obj_list.get( id );
		return null;
	}
	
	/**
	 * 
	 * �대떦 ID瑜� 媛吏� �먯떇�몃뱶以� 泥ル쾲吏� �몃뱶瑜� 由ы꽩 �쒕떎.
	 * 
	 * @param id
	 * @return
	 */
	public NADXMLObj getChild( String id ){
		List<NADXMLObj> list = getChilds( id);
		if( null != list && list.size() > 0 ) return list.get( 0 );
		return null;
	}
	
	/**
	 * Child�� ID 由ъ뒪�몃� 由ы꽩 �쒕떎.
	 * @return
	 */
	public Set<String> getChildIdList(){
		return obj_list.keySet();
	}
	
	/**
	 * 
	 * �붿냼瑜� 媛�몄삱��, path瑜� �댁슜�댁꽌 媛�몄삤�� �⑥닔
	 * 
	 * ex )
	 * <root ver="2" >
	 * 	<test a="1" >
	 * 		This is Sprata
	 *  </test>
	 *  <test a="2">
	 *  	This is Sprata 2
	 *  </test>
	 * </root>
	 * 
	 * �꾩쓽 XML�� �붿냼瑜� 媛�몄삤�� 諛⑸쾿
	 * 
	 * #root				-> root �몃뱶�� content瑜� 由ы꽩
	 * @ver					-> root �몃뱶�� ver �쇰뒗 �띿꽦 媛믪쓣 由ы꽩 ( 2 )
	 * root.test			-> test �몃뱶�� NADXMLObj瑜� 由ы꽩 ( �щ윭媛쒖쓽 �붿냼媛 �덉뼱�� �쒖씪 泥섏쓬 �붿냼瑜� 由ы꽩 �쒕떎 )
	 * root.test.#			-> test �몃뱶�� text 瑜� 由ы꽩 ( This is Sprata )
	 * root.test.@a			-> test �몃뱶�� a �쇰뒗 �띿꽦 媛믪쓣 由ы꽩 ( 1 )
	 * root.$test			-> 蹂듭닔媛쒖쓽 test �몃뱶瑜� 由ы꽩 ( List<NADXMLObj> )
	 * 
	 * @param path
	 * @return
	 */
	public Object getElement( String path ){
		
		String[] node = path.split( "\\." );
		Object ret = null;
		NADXMLObj temp = this;
		
		for( int i = 0; i < node.length ; ++ i ){
			String cur_node = node[i];
			switch( cur_node.charAt(0) ){
			case '@':
				// �띿꽦媛� 由ы꽩
				return temp.getAttribute(cur_node.substring(1));
			case '#':
				// 而⑦뀗痢� 由ы꽩
				return temp.getContent();
			case '$':
				// �먯떇 �꾩껜 由ы꽩
				return temp.getChilds(cur_node.substring(1));				
			default:
				// �먯깋 �뱀� 
				temp = temp.getChild( cur_node );
				if( null == temp ) return temp;
				break;
			}
		}		
		ret = temp;		
		return ret;
	}
	
	/**
	 * DOMParser �� Element �붿냼濡� 遺�� OBJ瑜� 援ъ꽦 �⑸땲��.
	 * @param curr_e
	 * @return
	 */
	private boolean parseDomElement( String tagName, Node curr_e ){
		boolean ret = false;
		
		// 湲곕낯 �뺣낫 �뗮똿
		this.tagName = tagName;
		//content = curr_e.getNodeValue();
			
		// Attribute 異붿텧
		NamedNodeMap cur_attr = curr_e.getAttributes();
		if( null != cur_attr ){
			for( int i = 0; i < cur_attr.getLength() ; ++ i ){
				attribute_list.put( cur_attr.item( i ).getNodeName() , cur_attr.item( i ).getNodeValue() );
			}
		}
		
		// Node蹂� �쒕툕 NADXMLObj �앹꽦
		Node curr_n = curr_e.getFirstChild();
		while( null != curr_n ){			
				
			String sub_tagName = curr_n.getNodeName();
			
			if( curr_n.getNodeType() != Node.COMMENT_NODE ){
				
				short node_type = curr_n.getNodeType();
				
				 
				if( node_type == Node.TEXT_NODE ){
					// #text �몃뱶�� 援됱옣�� �먯＜ �ㅼ뼱 �⑤떎...
					if(  content.equals( "" ) )
						content = curr_n.getNodeValue().trim();					
				}else if (  node_type == Node.CDATA_SECTION_NODE ){
					content = curr_n.getNodeValue();
				}else{
					List<NADXMLObj> curr_obj_list 	= null;
					NADXMLObj		curr_obj		= new NADXMLObj();
					if( obj_list.containsKey( sub_tagName ) ){
						curr_obj_list = obj_list.get(sub_tagName);
					}else{
						curr_obj_list = new LinkedList<NADXMLObj>();
						obj_list.put( sub_tagName , curr_obj_list );
					}
					
					curr_obj_list.add(curr_obj);
					if( !curr_obj.parseDomElement(sub_tagName, curr_n) ){
						return false;
					}
				}
			}
			
			curr_n = curr_n.getNextSibling();

		}
		
		ret = true;
		
		return ret;
	}
	
	/**
	 * �꾩옱 �곹깭瑜� TEXT濡� 由ы꽩 �섎뒗 �⑥닔
	 * @param indent
	 * @param buff
	 */
	private void dump( String indent, StringBuffer buff )
	{
		buff.append(indent).append("TAG : ").append(tagName).append("\n")
			.append(indent).append("Content : ").append( content ).append("\n");
		
		// attribute
		Iterator<String> att_i = attribute_list.keySet().iterator();
		while( att_i.hasNext() ){
			String cur = att_i.next();
			buff.append(indent).append("ATTR : ").append(cur).append( " = " ).append( attribute_list.get( cur ) ).append("\n");
		}
		
		// node
		Iterator<String> node_key_i = obj_list.keySet().iterator();
		while( node_key_i.hasNext() ){
			String cur = node_key_i.next();
			Iterator<NADXMLObj> obj_i = obj_list.get( cur ).iterator();
			while( obj_i.hasNext() ){
				obj_i.next().dump("\t"+indent, buff );
			}
		}
		
	}
	
	/**
	 * toString();
	 */
	public String toString(){
		StringBuffer ret = new StringBuffer();		
		dump( "", ret );
		return ret.toString();		
	}
		
	/**
	 * �꾩옱 �곹깭瑜� XML String�쇰줈 由ы꽩 �섎뒗 �⑥닔
	 * @param indent
	 * @param buff
	 */
	private void toXmlString( String indent, StringBuffer buff )
	{
		buff.append(indent).append("<").append(tagName);
				
		// attribute
		Iterator<String> att_i = attribute_list.keySet().iterator();
		while( att_i.hasNext() ){
			String cur = att_i.next();
			buff.append(" ").append(cur).append( "='" ).append( attribute_list.get( cur ) ).append("' ");
		}
		buff.append(">");
		// node
		Iterator<String> node_key_i = obj_list.keySet().iterator();
		if( node_key_i.hasNext() ){			
			buff.append("\r");
			// �먯떇�� �덈떎.
			do{
				
				String cur = node_key_i.next();
				Iterator<NADXMLObj> obj_i = obj_list.get( cur ).iterator();
				
				while( obj_i.hasNext() ){
					obj_i.next().toXmlString("\t"+indent, buff );
				}
				
				buff.append("\r");
				
			}while( node_key_i.hasNext() );
		
			buff.append( content );
			buff.append(indent).append("</").append(tagName).append(">");
		}else{
			buff.append( content ).append("</").append(tagName).append(">");
		}	
	}
	
	/**
	 * XML String
	 * @return
	 */
	public String toXmlString(){
		StringBuffer ret = new StringBuffer();		
		toXmlString( "", ret );
		return ret.toString();	
	}
	
	
	/**
	 * XML 臾몄옄�댁쓣 �댁슜�섏뿬 OBJECT瑜� 留뚮뱶�� �⑥닔
	 * 
	 * @param string
	 * @return
	 */
	public static NADXMLObj createObjectFromString( String xml ){
		NADXMLObj obj = null;
		
		DOMParser parser = new DOMParser();
		
		
		
        try
        {
            parser.parse(new InputSource(new CharArrayReader( xml.toCharArray() )));
        	 //parser.parse(new InputSource(new CharArrayReader( tempXml.toCharArray() )));
            Document doc = parser.getDocument();
            Element root = doc.getDocumentElement();
            
            obj = new NADXMLObj();
            if( !obj.parseDomElement( root.getTagName(), root ) ){
            	obj = null;
            }            
        }
        catch(SAXException e)
        {
            e.printStackTrace();
            obj = null;
        }
        catch(IOException e)
        {
            e.printStackTrace();
            obj = null;
        }
		
		
		return obj;
	}

	/**
	 * XML �뚯씪�� �댁슜�섏뿬 OBJECT瑜� 留뚮뱶�� �⑥닔
	 * 
	 * @param string
	 * @return
	 */
	public static NADXMLObj createObjectFromFile(String string)throws FileNotFoundException, IOException {
		
		String content = "";
		String result = "";
		String encodingType = "";

		// Step 1. File Open
		File fp = new File( string );

		//encoding 泥댄겕
		encodingType = getFileEncoding(string);
		System.out.println("encodingType:" + encodingType);
		
		if( fp.exists() && fp.canRead() ){		
		
			// Step 2. File Load
			long maxsize = fp.length();
			byte[] temp = new byte[ (int) maxsize ];
			
			InputStream input = new FileInputStream( fp );			
			int read_len = input.read( temp );
			int start_index = 0;
			
			// BOM 臾몄옄 �쒓굅
			for( int i = 0 ; i < read_len ; ++ i ){
				if( temp[i] == '<' ){
					start_index = i;
					break;
				}
			}	
			content = new String( temp, start_index, read_len - start_index);		
			
			result = content;
			
			/*
			if(encodingType.equals("UTF-8")){
				//utf-8�� 寃쎌슦 BOM �쒓굅
				System.out.println("bom�쒓굅");
				result = content.substring(1);	
			}
			*/
		}
		
		
			
			
		return  createObjectFromString( result );
	}
	
	
	/*
	 * �뚯씪 encoding �뺣낫 �뺤씤
	 * @param String
	 * @return String
	 * @
	 * */
	public static String getFileEncoding(String configPath){
		byte[] BOM = new byte[4];
		String encodingType = "";
		
		try{
		FileInputStream in = new FileInputStream(configPath);
		
		in.read(BOM, 0, 4);
		
		//�뚯씪 �몄퐫�� �뺤씤
				// 3. �뚯씪 �몄퐫�� �뺤씤�섍린
		if( (BOM[0] & 0xFF) == 0xEF && (BOM[1] & 0xFF) == 0xBB && (BOM[2] & 0xFF) == 0xBF ){
			System.out.println("UTF-8");
			encodingType = "UTF-8";
		}
		else if( (BOM[0] & 0xFF) == 0xFE && (BOM[1] & 0xFF) == 0xFF ){
			System.out.println("UTF-16BE");
			encodingType = "UTF-16BE";
		}
		else if( (BOM[0] & 0xFF) == 0xFF && (BOM[1] & 0xFF) == 0xFE ){
			System.out.println("UTF-16LE");
			encodingType = "UTF-16LE";
		}
		else if( (BOM[0] & 0xFF) == 0x00 && (BOM[1] & 0xFF) == 0x00 && 
		         (BOM[0] & 0xFF) == 0xFE && (BOM[1] & 0xFF) == 0xFF ){
			System.out.println("UTF-32BE");
			encodingType = "UTF-32BE";
		}
		else if( (BOM[0] & 0xFF) == 0xFF && (BOM[1] & 0xFF) == 0xFE && 
		         (BOM[0] & 0xFF) == 0x00 && (BOM[1] & 0xFF) == 0x00 ){
			System.out.println("UTF-32LE");
			 encodingType = "UTF-32LE";
		}
		else{
			System.out.println("EUC-KR");
			encodingType = "EUC-KR";
		}
					
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return encodingType;
	}
	
}