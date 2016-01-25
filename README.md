The extension is being tested

## Why it is need 

Let's say you have csv file wich has many elements on line which  need to convert it whole on array
But you need only few columns from this file
So you can try to use this PHP extension

## Example
- we have test file
![alt tag] (http://i.imgur.com/4HY41KR.png)
- after that we had installed it, make test
```
$ php -d extension=csvtoarray.so -r "print_r(get_array_from_csv('/var/www/viewer/test_file/test.csv', array(2,5,7), ','));"
```
- 'get_array_from_csv' - it is name of function
- first param - it is path to file
- second param - it is array which has column's numbers which we need to have in array, 
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
