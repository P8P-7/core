#pragma once

#include <string>
#include <boost/log/core.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/trivial.hpp>
#include <boost/format.hpp>

/**
 * @file colored_console.h
 * @author Group 7 - Informatica
 */

/**
 * @namespace goliath::util
 * @brief Contains utilities
 */

#define CONSOLE_COLOR_DEFAULT "\033[39m"
#define CONSOLE_COLOR_BLACK "\033[30m"
#define CONSOLE_COLOR_WHITE "\033[31m"
#define CONSOLE_COLOR_RED "\033[31m"
#define CONSOLE_COLOR_GREEN "\033[32m"
#define CONSOLE_COLOR_YELLOW "\033[33m"
#define CONSOLE_COLOR_BLUE "\033[34m"
#define CONSOLE_COLOR_MAGENTA "\033[35m"
#define CONSOLE_COLOR_CYAN "\033[36m"
#define CONSOLE_COLOR_LIGHT_GREY "\033[37m"
#define CONSOLE_COLOR_DARK_GREY "\033[90m"
#define CONSOLE_COLOR_LIGHT_RED "\033[91m"
#define CONSOLE_COLOR_LIGHT_GREEN "\033[92m"
#define CONSOLE_COLOR_LIGHT_YELLOW "\033[93m"
#define CONSOLE_COLOR_LIGHT_BLUE "\033[94m"
#define CONSOLE_COLOR_LIGHT_MAGENTA "\033[95m"

#define CONSOLE_STYLE_RESET "\033[0m"
#define CONSOLE_STYLE_BOLD "\033[1m"
#define CONSOLE_STYLE_UNDERLINE "\033[4m"

const std::string LOG_COLOR_TRACE = CONSOLE_COLOR_MAGENTA;
const std::string LOG_COLOR_DEBUG = (boost::format("%1%%2%") % CONSOLE_STYLE_BOLD % CONSOLE_COLOR_YELLOW).str();
const std::string LOG_COLOR_INFO = CONSOLE_COLOR_BLUE;
const std::string LOG_COLOR_WARNING = CONSOLE_COLOR_LIGHT_RED;
const std::string LOG_COLOR_ERROR = (boost::format("%1%%2%") % CONSOLE_STYLE_BOLD % CONSOLE_COLOR_RED).str();
const std::string LOG_COLOR_FATAL = (boost::format("%1%%2%%3%") % CONSOLE_STYLE_BOLD % CONSOLE_STYLE_UNDERLINE %
                                         CONSOLE_COLOR_RED).str();
const std::string LOG_COLOR_DEFAULT = CONSOLE_COLOR_DEFAULT;

namespace goliath::util {
    namespace blog = boost::log;

    /**
     * @fn std::string getColor(const boost::log::trivial::severity_level& severityLevel)
     * @brief Returns a specific color based on the severity of the log
     * @param severityLevel Level of severity to display
     * @return Linux terminal color string
     */
    std::string getColor(const blog::trivial::severity_level& severityLevel);

    /**
     * @fn void colorConsoleFormatter(const boost::log::record_view& recordView, boost::log::formatting_ostream& formatStream)
     * @brief Formatter that takes in a message, applies data, thread, severity and a color
     */
    void colorConsoleFormatter(const blog::record_view& recordView, blog::formatting_ostream& formatStream);

    /**
     * @brief Initialize a sink with the formatter and sets it as default
     */
    void init();
}
