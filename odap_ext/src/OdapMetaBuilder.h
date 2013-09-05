/*
 * OdapMetaBuilder.h
 *
 *  Created on: 2013. 9. 5.
 *      Author: Administrator
 */

#ifndef ODAPMETABUILDER_H_
#define ODAPMETABUILDER_H_

#include <map>
#include <vector>
#include <string>

/**
 * XML 예제
 *
 * <odap>
 * 	<property>
 * 		<filename>12321312</filename>
 * 		<ptype></ptype>
 * 	</property>
 * 	<problem>
 * 		<element x='' y='' w='' h='' />
 * 	</problem>
 * 	<descript>
 * 		<element x='' y='' w='' h='' />
 * 	</descript>
 * 	<example>
 * 		<element num='1' x='' y='' w='' h='' />
 * 		<element num='2' x='' y='' w='' h='' />
 * 		<element num='3' x='' y='' w='' h='' />
 * 		<element num='4' x='' y='' w='' h='' />
 * 		<element num='5' x='' y='' w='' h='' />
 * 	</example>
 *
 */
class OdapMetaBuilder {

private:
	std::string		property;
	std::string		problem;
	std::string		descript;
	std::string		exam;
	std::string		result;

private:
	int				exam_cnt;

public:
	OdapMetaBuilder();
	virtual ~OdapMetaBuilder();

	/**
	 * 문제 결과 만들기
	 */
public:
	OdapMetaBuilder* clear();
	OdapMetaBuilder* setProperty( const char* id, const char* value );
	OdapMetaBuilder* addProblem( int x, int y, int width, int height );
	OdapMetaBuilder* addDescript( int x, int y, int width, int height );
	OdapMetaBuilder* addExam(int num, int x, int y, int width, int height );

public:
	const char*		toXMLString();

public:
	/**
	 *현재 문제가 정상인지 판단
	 */
	bool			checkIsCorrect();

};

#endif /* ODAPMETABUILDER_H_ */
