#include "php.h"

PHP_FUNCTION(view);

const zend_function_entry viewer_functions[] = {
	PHP_FE(view, NULL)
	{NULL, NULL, NULL}
};

zend_module_entry viewer_module_entry = {
	STANDARD_MODULE_HEADER,       // #if ZEND_MODULE_API_NO >= 20010901
	"viewer",                       // название модуля
	viewer_functions,               // указываем экспортируемые функции
	NULL,                         // PHP_MINIT(test), Module Initialization
	NULL,                         // PHP_MSHUTDOWN(test), Module Shutdown
	NULL,                         // PHP_RINIT(test), Request Initialization
	NULL,                         // PHP_RSHUTDOWN(test), Request Shutdown
	NULL,                         // PHP_MINFO(test), Module Info (для phpinfo())
	"0.1",                        // версия нашего модуля
	STANDARD_MODULE_PROPERTIES
};

ZEND_GET_MODULE(viewer)

PHP_FUNCTION(view)
{
	char *text;
	int text_length;
	zval *data;
	
	// check params
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sa", &text, &text_length, &data) == FAILURE) { 
   		return;
	}
	
	// check file
	FILE *fp;
	if ((fp = fopen("/var/www/view/index.cphp", "r")) == NULL)
		printf ("Error with reading file. please check path to file\n");
	
	// get
	char str[300000] = " ";
	char *str_uk = str;
	int c;
	while ((c = getc(fp)) != EOF){
		*str_uk = c;
		str_uk++;
	}	
	
	fclose (fp);
	*str_uk = '\0';
	RETURN_STRING(str, 1);  
}


//php -d extension=viewer.so -r "var_dump(view('hgfgh',[]));"
