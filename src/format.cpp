#include <string>
#include <sstream>

#include "format.h"

using std::string;

std::string PadString(char pad, int charNum, string origStr) {
    return std::string(charNum - origStr.length(), pad) + origStr;
}

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    // 3600 seconds in an hour
    std::string hours = PadString('0', 2, std::to_string(seconds / 3600));
    seconds = seconds % 3600;

    // 60 seconds in a minute
    std::string minutes = PadString('0', 2, std::to_string(seconds / 60));
    seconds = seconds % 60;

    std::string secondsFormat = PadString('0', 2, std::to_string(seconds));

    return hours + ":" + minutes + ":" + secondsFormat;
}