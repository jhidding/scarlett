%{
	/* prologue */
%}

/* declarations */
string 		\"(\\[^ \t\n]|[^\\"])*\"
symbol 

%%
/* rules */

expression:
  numeric
| string
| symbol
| special
| list
;

string:
      {
		std::cout << "[string] " << yytext << std::endl;
	}
%%

/* epilogue */

