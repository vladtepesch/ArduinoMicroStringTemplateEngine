# ArduinoMicroStringTemplateEngine
Micro String Template Engine for Arduino.  
A very simplistic string template engine that helps preventing the usage of string function to reduce heap fragmentation.  

## Purpose/The problem to solve
This "library" (actual it is a single cpp and header file) is supposed to clean string output code in Arduino environment.

While it seems to be easy to construct complex output string like `Serial.println("hello "+myString+". It is a nice day");` this pattern has a number of hidden implications:

- additional string objects are created in this steps
   - each of them uses dynamic memory
   - lot of different size memory allocations fragement the heap
   - more details take a reading here: [majen1] 
- strings created this way use doubled space
   - one instance in flash
   - one instance is copied to memory during startup

There are some ways how to circumvent this. Using as few operators as possible will prevent string object construction:

    Serial.print("hello ");
    Serial.print(myString);
    Serial.println(". It is a nice day");

For larger texts this drastically reduces your program readability and makes it difficult to copy-past text/code from other places.

## Solution
The idea by that this little helper tries to solve this, is by providing an easy way to provide a template string with some placeholders that are replaced by values from a provided array.

The `"%0"` to `"%9"` are the placeholder for the runtime supplied values.
If the character after the `'%'` sign is no decimal digit or another  `'%'` then just 
a single `'%'` is printed.

##Example

Instead of creating a String with concatination (or '+') that results in many 
temporary objects write a template string and use this class:


    String name ("Dave");
    String game ("TicTacToe");
    
    ////// instead of 
    String a = "Hello, " + name + ".\n"
               "I want to play a game. "
               "Would you like to play \"" + game + "\" with me?" ;
    Serial.print(a);
   
    ////// use that
    //  array with template values:
    const char* values[] = {name->c_str(), game->c_str()};
    
    // (1) output to serial - template parameter is C string literal from RAM (initially copied from flash on startup)
    TemplatePrinter::printTo(Serial, "Hello, %0. I want to play a game. Would you like to play \"%1\" with me?", values);
    ////// or
    // (2) output to serial - template parameter is inline C string literal from FLASH (without being duplicated to RAM)
    TemplatePrinter::printTo(Serial, F("Hello, %0. I want to play a game. Would you like to play \"%1\" with me?"), values);
    // (3) output to serial - same as above but uses a pointer to a FLASH string literal
    static const char PROGMEM s_greeting[] = R"(Hello, %0. I want to play a game. Would you like to play "%1" with me?)";
    TemplatePrinter::printTo(Serial, FPSTR(s_greeting), values);


The above second method also makes it more easy to put whole strings into flash (see 3rd variant);

While in this little example it would be as easy to simply use `Serial.printf(FPSTR(s_greeting), name.c_str());` (after replacing `%0` by `%s`) this would be very cumbersome if the template is quite long and contains the same variables mutliple times.   
However: there is a gcc-printf-extension that allows specifying the number of argument to place in (format string `%2$s`: use the second additional parsed argumend as string) but this extension is not supported everywhere.  
On Arduino ESP8266 Core it seems not to work with the `Print::printf` functions

## Limitations

 - currently only `char*` values are supported by the API
 - currently only tested on Arduino ESP8266 Core

[majen1]: https://hackingmajenkoblog.wordpress.com/2016/02/04/the-evils-of-arduino-strings/

