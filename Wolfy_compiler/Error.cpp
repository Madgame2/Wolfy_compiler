#include"Error.h"

namespace Error {

	/*	error types:	0 -  99 - system errors
	*				  100 - 109 - params errors
	*				  110 - 119 - open adn reading file errors
	*/	


	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0,"invalid error code"),
		ERROR_ENTRY(1,"System error"),
		ERROR_ENTRY(2,"Lexem table overcrowded"),
		ERROR_ENTRY(3,"Error in lexem id"),
		ERROR_ENTRY(4,"Id Table error sizze overcrowded"),
		ERROR_ENTRY(5,"Error, unknown lexem in: "),
		ERROR_ENTRY(6,"Unknown indevicztor in: "),
		ERROR_ENTRY(7,"Wrong indevicator"),
		ERROR_ENTRY_NODEF(8),ERROR_ENTRY_NODEF(9),ERROR_ENTRY_NODEF10(10),ERROR_ENTRY_NODEF10(20),ERROR_ENTRY_NODEF10(30),
		ERROR_ENTRY_NODEF10(40),ERROR_ENTRY_NODEF10(50),ERROR_ENTRY_NODEF10(60),ERROR_ENTRY_NODEF10(70),ERROR_ENTRY_NODEF10(80),ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100,"param -in must be specified"),
		ERROR_ENTRY(101,"param -in alredy defined"),
		ERROR_ENTRY(102,"param -in is empty"),
		ERROR_ENTRY(103,"error in param struct (example: -in:main.cpp)"),
		ERROR_ENTRY(104,"input parameter length exceeded"),
		ERROR_ENTRY(105,"<-uncknow param"),
		ERROR_ENTRY(106,"param -out alredy defined"),
		ERROR_ENTRY(107,"param -out is empty"),
		ERROR_ENTRY(108,"param -log is empty"),
		ERROR_ENTRY(109,"param -log alredy defined"),
		ERROR_ENTRY(110,"open file error (-in)"),
		ERROR_ENTRY(111,"invalid character in source file (-in)"),
		ERROR_ENTRY(112,"open file error (-log)"),
		ERROR_ENTRY(113,"open file error (-out)"),
		ERROR_ENTRY(114,"invalid bracket in: "),
		ERROR_ENTRY(115,"ERROR: variable alredy difende in: "),ERROR_ENTRY_NODEF(116),ERROR_ENTRY_NODEF(117),ERROR_ENTRY_NODEF(118),
		ERROR_ENTRY_NODEF(119),ERROR_ENTRY_NODEF10(120),ERROR_ENTRY_NODEF10(130),ERROR_ENTRY_NODEF10(140),ERROR_ENTRY_NODEF10(150),ERROR_ENTRY_NODEF10(160),
		ERROR_ENTRY_NODEF10(170),ERROR_ENTRY_NODEF10(180),ERROR_ENTRY_NODEF10(190),ERROR_ENTRY_NODEF100(200),ERROR_ENTRY_NODEF100(300),ERROR_ENTRY_NODEF100(400),
		ERROR_ENTRY_NODEF100(500),ERROR_ENTRY_NODEF100(600),ERROR_ENTRY_NODEF100(700),ERROR_ENTRY_NODEF100(800),ERROR_ENTRY_NODEF100(900)
	};

	ERROR get_error(int id) {
		if (id<0 || id>ERROR_MAX_ENTRY) {
			return errors[0];
		}

		return errors[id];
	}

	ERROR get_error_in(int id, int line=-1, int pos=-1) {
		if (id<0 || id>ERROR_MAX_ENTRY) {
			return errors[0];
		}

		ERROR temp = errors[id];
		temp.data.line = line;
		temp.data.position = pos;

		return temp;
	}
}