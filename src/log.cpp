// -*- mode: C++; coding: utf-8 -*-
/* *******************************************************************
* File: raven.cpp                               Part of Raven Server *
*                                                                    *
* Copyright (C) 2012, 2013, Joachim Pileborg and individual          *
* contributors. All rights reserved.                                 *
*                                                                    *
* Redistribution and use in source and binary forms, with or without *
* modification, are permitted provided that the following conditions *
* are met:                                                           *
*                                                                    *
*   o Redistributions of source code must retain the above copyright *
*     notice, this list of conditions and the following disclaimer.  *
*   o Redistributions in binary form must reproduce the above        *
*     copyright notice, this list of conditions and the following    *
*     disclaimer in the documentation and/or other materials         *
*     provided with the distribution.                                *
*   o Neither the name of The Raven nor the names of its             *
*     contributors may be used to endorse or promote products        *
*     derived from this software without specific prior written      *
*     permission.                                                    *
*                                                                    *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND             *
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,        *
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF           *
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS  *
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,*
* OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,           *
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR *
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY       *
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR     *
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF *
* THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF    *
* SUCH DAMAGE.                                                       *
*                                                                    *
******************************************************************* */

#include "raven.h"

// #include <memory>
// #include <boost/log/sinks.hpp>
// #include <boost/log/expressions.hpp>
// #include <boost/log/attributes.hpp>
#include <boost/log/utility/setup/console.hpp>

namespace raven {
namespace log {

/* **************************************************************** */

namespace
{
    struct log_initializer
    {
        log_initializer()
        {
            std::cout << "log_initializer::log_initializer\n";

            // boost::log::add_console_log(
            //     std::clog, boost::log::keywords::format = ">> %Message%");

            // namespace sinks = boost::log::sinks;
            // namespace expr = boost::log::expressions;
            // namespace attrs = boost::log::attributes;

            // using text_sink = sinks::synchronous_sink<sinks::text_ostream_backend>;
            // auto sink = std::make_shared<text_sink>();

            // sink->locked_backend()->add_stream(std::make_shared<std::ostream>(std::clog));

            // // sink->set_formatter(
            // //     expr::stream
            // //         << expr::attr<attrs::timer::value_type>("Timeline")
            // //         << expr::smessage
            // //     );

            // // boost::log::core::get()->add_sink(sink);

  // typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;
  //   boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();

  //   sink->locked_backend()->add_stream(
  //       boost::make_shared< std::ofstream >("sample.log"));

  //   sink->set_formatter
  //   (
  //       expr::stream
  //              // line id will be written in hex, 8-digits, zero-filled
  //           << std::hex << std::setw(8) << std::setfill('0') << expr::attr< unsigned int >("LineID")
  //           << ": <" << logging::trivial::severity
  //           << "> " << expr::smessage
  //   );

  //   logging::core::get()->add_sink(sink);
        }

        ~log_initializer()
        {
            std::cout << "log_initializer::~log_initializer\n";
        }
    } log_initializer;
}

/* **************************************************************** */

void init()
{
    BOOST_LOG_TRIVIAL(info) << "hello world";
}

void clean()
{
    BOOST_LOG_TRIVIAL(info) << "goodbye cruel world";
}

/* **************************************************************** */

} // namespace log
} // namespace raven
