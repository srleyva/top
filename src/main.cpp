#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"

int main() {
  OSParser *parser = new LinuxParser::Parser;
  System system(parser);
  NCursesDisplay::Display(system);
}