%{
void parsererror(char const *);
int parserlex(void);
%}


%%
input:    /* empty */
     ;
%%

int parserlex(void)
{
    return 0;
}

void parsererror(char const *)
{
}
