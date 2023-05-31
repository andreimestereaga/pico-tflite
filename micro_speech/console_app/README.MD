### Simple console application ###

## Requirements
The console application shall handle simple ASCII commands that are introduced by the user on keyboard.
Each command have to start with '/' and it ends with 'CR' (Carriage Return, enter) as in the following examples: 
```
/command1<CR>
/command2<CR>
```
The application shall send back the character that introduced by the user to imitate a standard terminal. The CR will be handled on the host's terminal (Putty) so that will provide LF as well (the cursor should move to the next line). On each CR the application will print '>>' to indicate that the cursor moved to a new line.
Commands that shall be implemented:
1. /get_sdata  -> it returns the current statistics of the micro_speech application, how many times it labeled the input as "yes", "no" and "unknown".
A valid answer will be: 
``` 
>>/get_sdata
>>get_sdata: Heard yes: 5, heard no: 4, heard unknown: 10, heard silence: 2.
```
2. /set_led[LEDNO] -> it turns LEDNO ON e.g. set_led[1] will turn D1 LED ON
3. /clear_led[LEDNO] -> it turns LEDNO OFF e.g. clear_led[1] will turn D1 LED OFF
