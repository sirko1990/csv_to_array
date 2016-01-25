The extension is being tested

## Why do we need it?

Let's say there is a test file with many columns and you need it to be converted into an array. Although you need only a few columns from this file. So try to use this PHP extension

## Example
- we have test file
![alt tag] (http://i.imgur.com/4HY41KR.png)
- after the test file has beеn installed you can test it
```
$ php -d extension=csvtoarray.so -r "print_r(get_array_from_csv('/var/www/viewer/test_file/test.csv', array(2,5,7), ','));"
```
- 'get_array_from_csv' - it is name of function
- first param - it is path to file
- second param - it is an array with column's numbers which should be in array
- third param - it is delimiter

Result

![alt tag] (http://i.imgur.com/AGJL24U.png)

## How to install
- Go to repository directory
- Run next command
```
$ phpize
$ ./configure
$ sudo make
$ sudo make install
```
