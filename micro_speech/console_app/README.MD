### Simple console application ###

## Requirements
The console application shall handle simple ASCII commands that are introduced by the user on keyboard.
Each command have to start with '/' and it ends with 'CR' (Carriage Return, enter) as in the following examples: 
```
/command1<CR>
/command2<CR>
```
The application shall send back the character that was introduced by the user to imitate a standard terminal. On each CR, the application will add LF (line feed) followed by '>>' to indicate that the cursor moved to a new line.
Commands that shall be implemented:
1. /get_sdata  -> it returns the current statistics of the micro_speech application, how many times it labeled the input as "yes", "no" and "unknown".
A valid answer will be: 
``` 
>>/get_sdata
>>get_sdata: Heard yes: 5, heard no: 4, heard unknown: 10, heard silence: 2.
```
2. /set_led[LEDNO] -> it turns LEDNO ON e.g. set_led[1] will turn D1 LED ON.
```
>>/set_led[1]
>>set_led: LED1 OK
```
3. /clear_led[LEDNO] -> it turns LEDNO OFF e.g. clear_led[1] will turn D1 LED OFF
```
>>/clear_led[2]
>>clear_led: LED2 OK
```