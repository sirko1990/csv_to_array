#!/usr/bin/env bash
 php -d extension=csvtoarray.so -r "print_r(get_array_from_csv('/var/my/csv_to_array/test_file/test.csv', array(0,1,2,3,4), ','));"
