#include "php.h"
#include <stdio.h>
#include <zend_hash.h>
#include <zend_errors.h>
#include <zend.h>
#include <csvtoarray.h>

const zend_function_entry csvtoarray_functions[] = {
	PHP_FE(get_array_from_csv, NULL)
	{NULL, NULL, NULL}
};

zend_module_entry csvtoarray_module_entry = {
	STANDARD_MODULE_HEADER,       // #if ZEND_MODULE_API_NO >= 20010901
	"csvtoarray",                 // name of module
	csvtoarray_functions,         // export functions
	NULL,                         // PHP_MINIT(csvtoarray), Module Initialization
	NULL,                         // PHP_MSHUTDOWN(csvtoarray), Module Shutdown
	NULL,                         // PHP_RINIT(csvtoarray), Request Initialization
	NULL,                         // PHP_RSHUTDOWN(csvtoarray), Request Shutdown
	NULL,                         // PHP_MINFO(csvtoarray), Module Info (for phpinfo())
	"0.0.1",                      // version
	STANDARD_MODULE_PROPERTIES
};

ZEND_GET_MODULE(csvtoarray)

	
	
PHP_FUNCTION(get_array_from_csv)
{
	// file path
	char *file_path;
	int file_path_len;
	
	//array with numbers of elements in string
	zval* zarr;
	zval **item;
	
	//delimiter
	char *delimiter;
	int delimiter_len;
	
	// check params
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sas", &file_path, &file_path_len, &zarr, &delimiter, &delimiter_len) == FAILURE) { 
   		return;
	}
	
	// count params on array param of numbers
	int count = zend_hash_num_elements(zarr->value.ht);
	int i = 0;
	int elements[count];
	zval *tmp;
	
	// reset zval array to head
	zend_hash_internal_pointer_reset(Z_ARRVAL_P(zarr));
	
	//copy params of zval array to C array
	while(i < count){
		zend_hash_get_current_data(Z_ARRVAL_P(zarr),(void **) &item);
		
		// check type element of array param  
		if(Z_TYPE_P(*item) != IS_LONG){
			zend_error(E_ERROR, "The some of array elements is not integer.\n");
		}
			
		elements[i] = (*item)->value.lval;                    
		zend_hash_move_forward(Z_ARRVAL_P(zarr));
		i++;
	}		

	// open file by file path param
	FILE *fp;
	if ((fp = fopen(file_path,"r")) == NULL)
		zend_error(E_ERROR, "Cannot read file. Please check file path.\n");
	
	// current number elemennt on line
	int numberOfElement, numberOfLine, is_match, current_key_of_ele, is_double_quotes, length_element_of_line, c;
	int size = sizeof(char);
	int step_buffer_length_update = 1;
	int MIN_BUFFER_SIZE = 500;
	char* BUFFER = malloc(size * MIN_BUFFER_SIZE * step_buffer_length_update);
	char quotes = '\"';
	
	// initialisation avariables
	numberOfLine = numberOfElement = is_match = current_key_of_ele = is_double_quotes = 0;
	array_init(return_value);
	
	// get each char element of filr
	while ((c = getc(fp)) != EOF){
		
		// if line of file is the end
		if(c == '\n' || numberOfLine == 0){
			
			// first line
			if(numberOfLine != 0)
				add_next_index_zval(return_value, tmp);
			
			// initialisation new array to the next linr
			MAKE_STD_ZVAL(tmp);
			array_init(tmp);
			
			// update index element of line
			numberOfElement = 0;
			
			// Increment to next line
			numberOfLine++;
			
		} else {
			
			// check if curren char element is delimiter
			if(c == *delimiter && is_double_quotes == 0){
				
				// add end of string element of line to array
				if(is_match == 1){
					//Update buffer
					BUFFER[current_key_of_ele] = '\0';
					//add to ARRAY NODE
					add_index_string(tmp, numberOfElement, BUFFER, 1);
					is_match = 0;
				}
				
				current_key_of_ele = 0;
				is_double_quotes = 0;
				numberOfElement++;
				BUFFER[0] = '\0';
				
				//check element number in possible array numbers
				if(1 == is_element_in_array_data(numberOfElement, elements, count)){
					is_match = 1;
				}
				continue;
			}
			
			//added char to current node of element
			if(is_match){
				
				//check quotes
				if(c == quotes){
					is_double_quotes = is_double_quotes == 1 ? 0 : 1;
					continue;
				}
				
				// check is buffer owerflow
				if(((MIN_BUFFER_SIZE * step_buffer_length_update) - current_key_of_ele) < 5){
					step_buffer_length_update++;
				    BUFFER = realloc(BUFFER, size * MIN_BUFFER_SIZE);
				}
				
				// add char element to buff
				BUFFER[current_key_of_ele] = c;
				current_key_of_ele++;
			}
		}
	}
	free(BUFFER);
	fclose(fp);
}

static int is_element_in_array_data(int number_of_element, int numbers[], int count)
{
	int i;
	if(count == 0)
		return 1;
	
	for(i = 0; i < count; i++){
		if(numbers[i] == number_of_element)
			return 1;
	}
	
	return 0;
}




