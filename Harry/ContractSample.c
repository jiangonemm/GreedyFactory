//system apis
int arrayLen(void *a);

//utility apis
int strcmp(char *a, char *b);
int Atoi(char *s);
char *Itoa(int a);

//parameter apis
void ZPT_JsonUnmashalInput(void *addr, int size, char *arg);
char *ZPT_JsonMashalResult(void *val, char *types, int succeed);

//Runtime apis
void ZPT_Runtime_Notify(char *address);

char *Add(char *a, char *b)
{
	return Itoa(Atoi(a) + Atoi(b));
}

char *invoke(char *method, char *args)
{

	if (strcmp(method, "init") == 0)
	{
		return "Hello World!";
	}

	if (strcmp(method, "add") == 0)
	{

		struct Params
		{
			char *a;
			char *b;
		};
		struct Params *p = (struct Params *)malloc(sizeof(struct Params));
		ZPT_JsonUnmashalInput(p, sizeof(struct Params), args);
		char *value = Add(p->a, p -> b);
		char *result = ZPT_JsonMashalResult(value, "string", 1);
		ZPT_Runtime_Notify(result);
		return result;
	}
}
