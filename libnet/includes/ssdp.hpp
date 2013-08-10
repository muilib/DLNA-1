/*
 * Copyright (C) 2013 midnightBITS
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __SSDP_HPP__
#define __SSDP_HPP__

#include <iostream>
#include <http.hpp>
#include <http_server.hpp>
#include <udp.hpp>
#include <interface.hpp>
#include <ssdp_device.hpp>

namespace net
{
	namespace ssdp
	{
		enum notification_type
		{
			ssdp_other,
			ALIVE,
			BYEBYE,
			DISCOVER
		};

		static const net::ushort PORT = 1900;
		static const long INTERVAL = 1800;

		inline std::ostream& operator << (std::ostream& o, notification_type type)
		{
			switch (type)
			{
			case ALIVE: return o << "ssdp:alive";
			case BYEBYE: return o << "ssdp:byebye";
			case DISCOVER: return o << "ssdp:discover";
			}
			return o << "ssdp:unknown:" << (int) type;
		}

		inline notification_type from_string(const std::string& header)
		{
			if (header.substr(0, 5) == "ssdp:")
			{
				auto type = header.substr(5);
				if (type == "discover") return DISCOVER;
				if (type == "alive") return ALIVE;
				if (type == "byebye") return BYEBYE;
			}
			return ssdp_other;
		}

		const boost::asio::ip::address & ipv4_multicast();

		struct ticker
		{
			ticker(boost::asio::io_service& io_service, const device_ptr& device, long seconds, net::ushort port);
			void start();
			void stop();
		private:
			device_ptr                  m_device;
			boost::asio::io_service&    m_service;
			boost::asio::deadline_timer m_timer;
			long                        m_interval;
			boost::asio::ip::address    m_local;
			net::ushort                 m_port;

			std::string build_msg(const std::string& nt, notification_type nts) const;

			void notify(notification_type nts) const;
			void stillAlive();
		};

		struct receiver
		{
			receiver(boost::asio::io_service& io_service, const device_ptr& device, net::ushort port);
			void start();
			void stop();
		private:
			udp::multicast_receiver m_impl;

			typedef boost::asio::io_service        service_t;
			typedef boost::asio::ip::address_v4    address_t;
			typedef boost::asio::ip::udp::endpoint endpoint_t;
			typedef std::array<char, 1024>         buffer_t;

			device_ptr  m_device;
			service_t&  m_service;
			address_t   m_local;
			net::ushort m_port;

			void discovery(const std::string& st);
			std::string build_discovery_msg(const std::string& st) const;
		};

		struct server
		{
			server(boost::asio::io_service& service, const device_ptr& device, net::ushort port)
				: m_http(service, device, port)
				, m_alive_ticker(service, device, INTERVAL, port)
				, m_listener(service, device, port)
			{
			}

			void start()
			{
				m_http.start();
				m_alive_ticker.start();
				m_listener.start();
			}

			void stop()
			{
				m_listener.stop();
				m_alive_ticker.stop();
				m_http.stop();
			}

		private:
			http::server m_http;
			ticker       m_alive_ticker;
			receiver     m_listener;
		};
	}
}

#endif //__SSDP_HPP__
