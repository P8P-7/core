#include "colored_console.h"

#include <thread>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <boost/core/null_deleter.hpp>

std::string goliath::util::getColor(const blog::trivial::severity_level &severityLevel) {
    switch(severityLevel) {
        case boost::log::trivial::trace:
            return LOG_COLOR_TRACE;
        case boost::log::trivial::debug:
            return LOG_COLOR_DEBUG;
        case boost::log::trivial::info:
            return LOG_COLOR_INFO;
        case boost::log::trivial::warning:
            return LOG_COLOR_WARNING;
        case boost::log::trivial::error:
            return LOG_COLOR_ERROR;
        case boost::log::trivial::fatal:
            return LOG_COLOR_FATAL;
        default:
            return LOG_COLOR_DEFAULT;
    }
}

void goliath::util::colorConsoleFormatter(const blog::record_view &recordView, blog::formatting_ostream &formatStream) {
    auto now = std::chrono::high_resolution_clock::now();
    auto mil = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    auto mic = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000;
    auto nowC = std::chrono::high_resolution_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&nowC), "%F %T") << ':' << mil.count() << ':' << mic.count();

    formatStream << '[' << CONSOLE_COLOR_GREEN << ss.str() << LOG_COLOR_DEFAULT << "] ";
    formatStream << '<' << CONSOLE_COLOR_CYAN << std::this_thread::get_id() << LOG_COLOR_DEFAULT << "> ";

    auto severity = recordView.attribute_values()["Severity"].extract<boost::log::trivial::severity_level>();
    if (severity) {
        formatStream << '(' << severity << ")\t├ ";
        formatStream << getColor(severity.get());
    }

    formatStream  << recordView[boost::log::expressions::smessage];

    if(severity) {
        formatStream << LOG_COLOR_DEFAULT;
    }
}

void goliath::util::init() {
    auto sink = boost::make_shared<boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend>>();
    sink->locked_backend()->add_stream(boost::shared_ptr<std::ostream>(&std::cout, boost::null_deleter()));

    sink->set_formatter(&goliath::util::colorConsoleFormatter);

    boost::log::core::get()->add_sink(sink);
}
