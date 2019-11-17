/**
 * \file
 *
 * \copyright This is free and unencumbered software released into the public domain.
 *             for more information look at LICENSE file or <https://unlicense.org>
 *
 */

#include "TemplatePrinter.h"


void TemplatePrinter::printTo(Print& pr, const char* i_template, const char* i_values[])
{
  const char* ps=i_template;
  while (const char* pn = strchr(ps, '%')) {
    pr.write(ps, pn-ps);
    pn++;
    char next = *pn;
    if (next == '\0') {
      return;
    }
    if ( next == '%') { // escaped %
      pr.write('%');
    } else {
      int idx = next - '0';
      if (idx >= 0 && idx <= 9) {
        pr.write(i_values[idx]);
      } else {
        pr.write('%');
        pr.write(next);
      }
    }
    ps = pn + 1;
  }
  // write rest of str
  pr.write(ps);
}

void TemplatePrinter::printTo(Print& pr, const __FlashStringHelper* i_template, const char* i_values[])
{
  PGM_P ps = reinterpret_cast<PGM_P>(i_template);
  while (1) {
    uint8_t c = pgm_read_byte(ps++);
    if (!c) {
      break;
    }
    if (c== '%') { // escaped %
      uint8_t c = pgm_read_byte(ps++);
      if (!c) {
        break;
      }
      if (c == '%') { // escaped %
        pr.write('%');
      } else {
        int idx = c - '0';
        if (idx >= 0 && idx <= 9) {
          pr.write(i_values[idx]);
        } else {
          pr.write('%');
          pr.write(c);
        }
      }
    } else {
      pr.write(c);
    }

  }
}

