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
	"0.0.2",                      // version
	STANDARD_MODULE_PROPERTIES
};

ZEND_GET_MODULE(csvtoarray)

PHP_FUNCTION(get_array_from_csv)
{
	char *file_path;
	int file_path_len;
	char *delimiter;
	int delimiter_len;
	zval* zarr;
	
	// check params
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sas", &file_path, &file_path_len, &zarr, &delimiter, &delimiter_len) == FAILURE) { 
   		return;
	}
	
	// count params on array param of numbers
	int count = zend_hash_num_elements(zarr->value.ht);
	int elements[count];
	FILE *fp;
	zval *tmp;

	//copy params of zval array to C array
	parse_array_with_field_numbers(zarr, &elements, count);

	// open file by file path param
	if ((fp = fopen(file_path,"r")) == NULL) zend_error(E_ERROR, "Cannot read file. Please check file path.\n");
	
	// current number elemennt on line
	int numberOfElement, numberOfLine, is_match, j, is_double_quotes, length_element_of_line, c;
	int size = sizeof(char);
	int step_buffer_length_update = 1;
	char* BUFFER = (char*)malloc(size * MIN_BUFFER_SIZE * step_buffer_length_update);

	// initialisation avariables
	numberOfLine = numberOfElement = is_match = j = is_double_quotes = 0;
	array_init(return_value);
	
	// get each char element of filr
	while ((c = getc(fp)) != EOF){
		// if line of file is the end
		if(c == LINE_END || numberOfLine == 0) {
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
					BUFFER[j] = '\0';
					//add to ARRAY NODE
					add_index_string(tmp, numberOfElement, BUFFER, 1);
					is_match = 0;
				}

				is_double_quotes = 0;
				numberOfElement++;
				BUFFER[0] = '\0';
				j = 0;

				is_element_in_array_data(numberOfElement, elements, count, &is_match);

			//added char to current node of element
			} else if(is_match) {
				//check quotes
				if(c == QUOTES){
					is_double_quotes = is_double_quotes == 1 ? 0 : 1;
					continue;
				}
				
				// check is buffer owerflow
				if(((MIN_BUFFER_SIZE * step_buffer_length_update) - j) < 5){
					step_buffer_length_update++;
				    BUFFER = realloc(BUFFER, size * MIN_BUFFER_SIZE);
				}
				
				// add char element to buff
				BUFFER[j] = c;
				j++;

			// check first element of line
			} else if(numberOfElement == 0) {
            	is_element_in_array_data(numberOfElement, elements, count, &is_match);
			}
		}
	}
	free(BUFFER);
	fclose(fp);
}

static int is_element_in_array_data(int number_of_element, int numbers[], int count, int *is_match)
{
	if(count == 0){
		*is_match = 1;
		return;
	}

	int i = 0;
	for(; i < count; i++){
		if(numbers[i] == number_of_element){
			*is_match = 1;
			return;
		}
	}
}

static void parse_array_with_field_numbers(zval* zarr, int* elements, int count)
{
	int i = 0;
	zval **item;

	// reset zval array to head
    zend_hash_internal_pointer_reset(Z_ARRVAL_P(zarr));

	while(i < count){
		zend_hash_get_current_data(Z_ARRVAL_P(zarr),(void **) &item);

		// check type element of array param
		if(Z_TYPE_P(*item) != IS_LONG){
			zend_error(E_ERROR, "The some of array elements is not integer.\n");
		}

		*elements = (*item)->value.lval;
		zend_hash_move_forward(Z_ARRVAL_P(zarr));
		elements++;
		i++;
	}
}
