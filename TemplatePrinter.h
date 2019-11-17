/**
 * \file
 * 
 * \copyright This is free and unencumbered software released into the public domain.
 *             for more information look at LICENSE file or <https://unlicense.org>
 *
 */

#pragma once


#include <Print.h>
#include <WString.h>
#include <avr/pgmspace.h>


/**
 * A very simplistic string template engine that helps preventing the usage of string function 
 * to reduce heap fragmentation.  
 * Idea is: give it a Print - object that is the output target and a template strig along 
 * with the values to fill in the place holder.
 *
 * The "%0" to "%9" are the placeholder for the runtime supplied values.
 * If the character after the '%' sign is no decimal digit or another '% then just 
 * a single '%' is printed.
 *
 * Example:
 *
 * Instead of creating a String with concatination (or '+') that results in many 
 * temporary objects write a template string and use this class:
 *
 * \code{.cpp}

   String name ("Dave");
   String game ("TicTacToe");
   ////// instead of 
   String a = "Hello, " + name + ". I want to play a game. Would you like to play \"" + game + "\" with me?" ;
   Serial.print(a);

   ////// use that
   //  array with template values:
   const char* values[] = {name->c_str(), game->c_str()};
   
   //  output to serial - template parameter is C string literal from RAM (initially copied from flash on startup)
   TemplatePrinter::printTo(Serial, "Hello, %0. I want to play a game. Would you like to play \"%1\" with me?", values);
   ////// or
   //  output to serial - template parameter is inline C string literal from FLASH (without being duplicated to RAM)
   TemplatePrinter::printTo(Serial, F("Hello, %0. I want to play a game. Would you like to play \"%1\" with me?"), values);
   //  output to serial - same as above but uses a pointer to a FLASH string literal
   static const char PROGMEM s_greeting[] = R"(Hello, %0. I want to play a game. Would you like to play "%1" with me?)";
   TemplatePrinter::printTo(Serial, FPSTR(s_greeting), values);

   \endcode

 * The above second method also makes it more easy to put whole strings into flash (see 3rd variant);
 *
 */
class TemplatePrinter
{
public:
  /** 
   *  Prints the i_template string (RAM) to pr and replaces occurences of "%0" - "%9" by thy strings from the i_values table.
   *  '%' signs can be excaped by "%%" but will printed on there own if the second character is not a digit
   */
  static void printTo(Print& pr, const char* i_template, const char* i_values[]);

  /**
   *  Prints the i_template string (FLASH) to pr and replaces occurences of "%0" - "%9" by thy strings from the i_values table.
   *  '%' signs can be excaped by "%%" but will printed on there own if the second character is not a digit
   */
  static void printTo(Print& pr, const __FlashStringHelper* i_template, const char* i_values[]);

private:

};

