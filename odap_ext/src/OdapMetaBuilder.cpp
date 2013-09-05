/*
 * OdapMetaBuilder.cpp
 *
 *  Created on: 2013. 9. 5.
 *      Author: Administrator
 */

#include "OdapMetaBuilder.h"

#include <stdio.h>

OdapMetaBuilder::OdapMetaBuilder()
: exam_cnt(0)
{


}

OdapMetaBuilder::~OdapMetaBuilder() {
}

	/**
	 * 문제 결과 만들기
	 */
OdapMetaBuilder* OdapMetaBuilder::setProperty( const char* id, const char* value ){
	property = property + "\t<" + id + ">" + value  + "</" + id + ">\n";
	return this;
}
OdapMetaBuilder* OdapMetaBuilder::addProblem( int x, int y, int width, int height ){
	char temp[512] = {0,};
	sprintf( temp, "\t<element x='%d' y='%d' w='%d' h='%d' />\n", x, y, width, height );
	problem += temp;
	return this;
}
OdapMetaBuilder* OdapMetaBuilder::addDescript( int x, int y, int width, int height ){
	char temp[512] = {0,};
	sprintf( temp, "\t<element x='%d' y='%d' w='%d' h='%d' />\n", x, y, width, height );
	descript += temp;
	return this;
}
OdapMetaBuilder* OdapMetaBuilder::addExam(int num, int x, int y, int width, int height ){
	char temp[512] = {0,};
	sprintf( temp, "\t<element num='%d' x='%d' y='%d' w='%d' h='%d' />\n", num , x, y, width, height );
	exam += temp;
	++ exam_cnt;
	return this;
}

OdapMetaBuilder* OdapMetaBuilder::clear(){
	property = "";
	problem = "";
	descript = "";
	exam = "";
	exam_cnt = 0;
	return this;
}


const char*		OdapMetaBuilder::toXMLString(){
	result = "<odap>\n";
	result += "<property>\n" + property + "</property>\n";
	result += "<problem>\n" + problem + "</problem>\n";
	result += "<descript>\n" + descript + "</descript>\n";
	result += "<example>\n" + exam + "</example>\n";
	result += "</odap>";
	return result.c_str();
}


bool			OdapMetaBuilder::checkIsCorrect(){
	if( exam_cnt == 5 ) return true;
	else				return false;
}
