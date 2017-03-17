#ifndef STANDART_FH
#define STANDART_FH

//standart func IDS, hard coded, change at your own risk
#define STD_YAZ 1
#define STD_OXU 2
#define STD_KVK 3 // kvadrat kok

struct std_func_el{
	char name[1024];
	unsigned long hid;
	int id;
};

struct std_func_table{
	struct std_func_el std_funcs[MAX_STD_FUNC];
	int count;
};


struct std_func_table sft= {
	{
		{"yaz", 0, STD_YAZ},
		{"oxu", 0, STD_OXU},
		{"kvk", 0, STD_KVK}
	
	}, 3
};

#endif