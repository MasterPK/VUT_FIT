#ifndef ERRORS_H
#define ERRORS_H

// Call EXIT_WITH_ERROR(error_code) instead of "return SYNTAX_ERR;" etc.
#define MAKE_STRING_(val) #val
#define MAKE_STRING(val) MAKE_STRING_(val)
#define RETURN_ERROR(error_code) \
	do {							\
		if (error_code != NO_ERR)	\
			fprintf(stderr, "Chyba v souboru " MAKE_STRING(__FILE__) " (%d) na radku:" \
			MAKE_STRING(__LINE__) ", typ tokenu=%d\n", error_code, token_tmp.type ); \
		return error_code;			\
	} while(0)
#define EXIT_ERROR(error_code) \
	do {							\
		if (error_code != NO_ERR)	\
			fprintf(stderr, "Chyba v souboru " MAKE_STRING(__FILE__) " (%d) na radku:" \
			MAKE_STRING(__LINE__) "\n", error_code ); \
		exit(error_code);			\
	} while(0)

#define	NO_ERR 0
#define	LEX_ERR 1
#define	SYNTAX_ERR 2
#define	SEM_DEF_ERR 3
#define	SEM_KOMP_ERR 4
#define	SEM_PAR_ERR 5
#define	SEM_ELSE_ERR 6
#define	ZERO_DIV_ERR 9
#define	PROG_ERR 	99

#endif
