
#define QUOTES '\"'
#define LINE_END '\n'
#define MIN_BUFFER_SIZE 500

static int is_element_in_array_data(int number_of_element, int numbers[], int count, int* is_match);
static void parse_array_with_field_numbers(zval* zarr, int* elements, int count);

PHP_FUNCTION(get_array_from_csv);