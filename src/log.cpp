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
#include <boost/log/sinks.hpp>
#include <boost/log/expressions.hpp>
// #include <boost/log/expressions/formatters/date_time.hpp>
// #include <boost/log/attributes.hpp>
// #include <boost/log/utility/setup/console.hpp>
#include <boost/utility/empty_deleter.hpp>
#include <iomanip>

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

            namespace sinks = boost::log::sinks;
            namespace expr = boost::log::expressions;
            namespace attrs = boost::log::attributes;

            using text_sink = sinks::synchronous_sink<sinks::text_ostream_backend>;
            auto sink = boost::make_shared<text_sink>();

            boost::shared_ptr<std::ostream> stream(&std::clog, boost::empty_deleter());
            sink->locked_backend()->add_stream(stream);

            // sink->set_formatter(
            //     expr::stream
            //         << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S")
            //         << expr::smessage
            //     );

            sink->set_formatter(
                expr::stream
                    << " :: " << std::left << std::setw(10) << boost::log::trivial::severity << " :: "
                    << expr::smessage
                );

            boost::log::core::get()->add_sink(sink);
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
