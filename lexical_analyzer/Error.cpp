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
		ERROR_ENTRY(8,"invalid literal entry"),
		ERROR_ENTRY_NODEF(9),ERROR_ENTRY_NODEF10(10),ERROR_ENTRY_NODEF10(20),ERROR_ENTRY_NODEF10(30),
		ERROR_ENTRY_NODEF10(40),ERROR_ENTRY_NODEF10(50),ERROR_ENTRY_NODEF10(60),ERROR_ENTRY_NODEF10(70),ERROR_ENTRY_NODEF10(80),ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100,"param -in must be specified"),
		ERROR_ENTRY(101,"param -in alredy defined"),
		ERROR_ENTRY(102,"param -in is empty"),
		ERROR_ENTRY(103,"file main must be unique"),
		ERROR_ENTRY(104,"file main must be difended"),
		ERROR_ENTRY(105,"<-uncknow param"),
		ERROR_ENTRY(106,"param -out alredy defined"),
		ERROR_ENTRY(107,"param -out is empty"),
		ERROR_ENTRY(108,"param -log is empty"),
		ERROR_ENTRY(109,"param -log alredy defined"),
		ERROR_ENTRY(110,"open file error (-in)"),
		ERROR_ENTRY(111,"invalid character in source file (-in)"),
		ERROR_ENTRY(112,"open file error"),
		ERROR_ENTRY(113,"open prefabs file error"),
		ERROR_ENTRY(114,"invalid bracket in: "),
		ERROR_ENTRY(115,"ERROR: variable alredy difende in: "),ERROR_ENTRY_NODEF(116),ERROR_ENTRY_NODEF(117),ERROR_ENTRY_NODEF(118),
		ERROR_ENTRY_NODEF(119),ERROR_ENTRY_NODEF10(120),ERROR_ENTRY_NODEF10(130),ERROR_ENTRY_NODEF10(140),ERROR_ENTRY_NODEF10(150),ERROR_ENTRY_NODEF10(160),
		ERROR_ENTRY_NODEF10(170),ERROR_ENTRY_NODEF10(180),ERROR_ENTRY_NODEF10(190),ERROR_ENTRY_NODEF100(200),ERROR_ENTRY_NODEF10(210),ERROR_ENTRY_NODEF10(220),
		ERROR_ENTRY_NODEF10(230),ERROR_ENTRY_NODEF10(240),ERROR_ENTRY_NODEF10(250),ERROR_ENTRY_NODEF10(260),ERROR_ENTRY_NODEF10(270),ERROR_ENTRY_NODEF10(280),ERROR_ENTRY_NODEF10(290),
		ERROR_ENTRY_NODEF(291),ERROR_ENTRY_NODEF(292),ERROR_ENTRY_NODEF(293),ERROR_ENTRY_NODEF(294),ERROR_ENTRY_NODEF(295),ERROR_ENTRY_NODEF(296),ERROR_ENTRY_NODEF(297),ERROR_ENTRY_NODEF(298),
		ERROR_ENTRY_NODEF(299),
		ERROR_ENTRY(300,"this variable is alredy definded"),
		ERROR_ENTRY(301,"cannot overload function"),
		ERROR_ENTRY(302,"unknow indeficator"),
		ERROR_ENTRY(303,"not all branches of a function return a result"),
		ERROR_ENTRY(304,"incompatible value types"),
		ERROR_ENTRY(305,"innvalid return data type"),
		ERROR_ENTRY(306,"Uncknow function referens"),
		ERROR_ENTRY(307,"this global functin alredy definded"),
		ERROR_ENTRY(308,"undefinded global function"),ERROR_ENTRY_NODEF(309),
		ERROR_ENTRY_NODEF10(310),ERROR_ENTRY_NODEF10(320),ERROR_ENTRY_NODEF10(330),ERROR_ENTRY_NODEF10(340),ERROR_ENTRY_NODEF10(350),ERROR_ENTRY_NODEF10(360),ERROR_ENTRY_NODEF10(370),
		ERROR_ENTRY_NODEF10(380),ERROR_ENTRY_NODEF10(390),ERROR_ENTRY_NODEF10(400),
		/*ERROR_ENTRY_NODEF100(400),*/
		ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600,"Wrong programm struct"),
		ERROR_ENTRY(601,"Operator error"),
		ERROR_ENTRY(602,"error in expression"),
		ERROR_ENTRY(603,"function params error"),
		ERROR_ENTRY(604,"params error in caled function"),
		ERROR_ENTRY(605,"wrong value in this notation"),
		ERROR_ENTRY_NODEF100(700),ERROR_ENTRY_NODEF100(800),ERROR_ENTRY_NODEF100(900)
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