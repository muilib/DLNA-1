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

#include "pch.h"
#include <http/request_handler.hpp>
#include <http/response.hpp>
#include <regex>
#include <network/interface.hpp>
#include <expat.hpp>
#include <dom.hpp>

namespace net
{
	namespace http
	{
		class DOMParser : public xml::ExpatBase<DOMParser>
		{
			dom::XmlElementPtr elem;
			std::string text;

			void addText()
			{
				if (text.empty()) return;
				if (elem)
					elem->appendChild(doc->createTextNode(text));
				text.clear();
			}
		public:

			dom::XmlDocumentPtr doc;

			bool create(const char* cp)
			{
				doc = dom::XmlDocument::create();
				if (!doc) return false;
				return xml::ExpatBase<DOMParser>::create(cp);
			}

			void onStartElement(const XML_Char *name, const XML_Char **attrs)
			{
				addText();
				auto current = doc->createElement(name);
				if (!current) return;
				for (; *attrs; attrs += 2)
				{
					auto attr = doc->createAttribute(attrs[0], attrs[1]);
					if (!attr) continue;
					current->setAttribute(attr);
				}
				if (elem)
					elem->appendChild(current);
				else
					doc->setDocumentElement(current);
				elem = current;
			}

			void onEndElement(const XML_Char *name)
			{
				addText();
				if (!elem) return;
				dom::XmlNodePtr node = elem->parentNode();
				elem = std::static_pointer_cast<dom::XmlElement>(node);
			}

			void onCharacterData(const XML_Char *pszData, int nLength)
			{
				text += std::string(pszData, nLength);
			}
		};

		dom::XmlDocumentPtr create_from_socket(request_data_ptr data)
		{
			if (!data || !data->content_length())
				return nullptr;

			DOMParser parser;
			if (!parser.create(nullptr)) return nullptr;

			parser.enableElementHandler();
			parser.enableCharacterDataHandler();

			auto rest = data->content_length();
			char buffer[8192];
			while (rest)
			{
				auto chunk = sizeof(buffer);
				if (chunk > rest)
					chunk = rest;
				rest -= chunk;
				auto read = data->read(buffer, chunk);

				//std::cout.write(buffer, read);

				if (!parser.parse(buffer, read, false))
					return nullptr;
			}

			if (!parser.parse(buffer, 0))
				return nullptr;

			return parser.doc;
		}

		struct tmplt_chunk
		{
			template_vars::const_iterator m_var;
			const char* m_start;
			size_t m_size;
			tmplt_chunk(template_vars::const_iterator var, const char* start, size_t size)
				: m_var(var)
				, m_start(start)
				, m_size(size)
			{}
		};

		class template_content : public content
		{
			const char* m_tmplt;
			const template_vars& m_vars;
			std::vector<tmplt_chunk> m_chunks;
			std::vector<tmplt_chunk>::const_iterator m_cur;
			size_t m_ptr;
			template_vars::const_iterator find(const std::string& key)
			{
				auto cur = m_vars.begin(), end = m_vars.end();
				for (; cur != end; ++cur)
					if (cur->first == key)
						return cur;
				return cur;
			}
		public:
			template_content(const char* tmplt, const template_vars& vars)
				: m_tmplt(tmplt)
				, m_vars(vars)
			{
				while (*tmplt)
				{
					auto start = tmplt;
					while (*tmplt && *tmplt != '$') ++tmplt;

					auto var = *tmplt ? tmplt + 1 : tmplt;
					auto var_end = var;
					while (*var_end && std::isalpha((unsigned char) *var_end)) ++var_end;

					auto _var = find({var, var_end});

					m_chunks.emplace_back(_var, start, tmplt - start);

					tmplt = var_end;
				}

				m_cur = m_chunks.begin();
				m_ptr = 0;
			}

			bool size_known() override { return false; }
			std::size_t get_size() override { return 0; }
			std::size_t read(void* buffer, std::size_t size)
			{
				if (m_cur == m_chunks.end())
					return 0;

				size_t _read = 0;
				while (size > 0)
				{
					if (m_ptr < m_cur->m_size)
					{
						auto rest = m_cur->m_size - m_ptr;
						if (rest > size)
							rest = size;

						memcpy((char*) buffer + _read, m_cur->m_start + m_ptr, rest);
						_read += rest;
						size -= rest;
						m_ptr += rest;
					}

					if (m_ptr >= m_cur->m_size && size > 0)
					{
						if (m_cur->m_var != m_vars.end())
						{
							auto ptr = m_ptr - m_cur->m_size;
							auto rest = m_cur->m_var->second.length() - ptr;

							if (rest > size)
								rest = size;

							memcpy((char*) buffer + _read, m_cur->m_var->second.c_str() + ptr, rest);
							_read += rest;
							size -= rest;
							m_ptr += rest;
						}
					}

					size_t whole = m_cur->m_size;
					if (m_cur->m_var != m_vars.end())
						whole += m_cur->m_var->second.length();
					if (m_ptr >= whole)
					{
						++m_cur;
						m_ptr = 0;
						if (m_cur == m_chunks.end())
							break;
					}
				}
				return _read;
			}
		};

		request_handler::request_handler(const ssdp::device_ptr& device, net::ushort port)
			: m_device(device)
			, m_port(port)
		{
			m_vars.emplace_back("host", to_string(iface::get_default_interface()));
			m_vars.emplace_back("port", std::to_string(m_port));
			m_vars.emplace_back("uuid", m_device->usn());
		}

		static dom::XmlNodeListPtr env_body(const dom::XmlDocumentPtr& doc)
		{
			dom::NSData ns [] = { { "s", "http://schemas.xmlsoap.org/soap/envelope/" } };
			auto body = doc->find("/s:Envelope/s:Body", ns);
			return body ? body->childNodes() : nullptr;
		}

		static void print_debug(const http_request& header, const std::string& SOAPAction, dom::XmlDocumentPtr& doc)
		{
			return;
			std::ostringstream o;
			o << header.m_method << " ";
			if (header.m_resource != "*")
			{
				auto it = header.find("host");
				if (it != header.end())
					o << it->value();
			}
			o << header.m_resource << " " << header.m_protocol;
			o << " [" << to_string(header.m_remote_address) << ":" << header.m_remote_port << "]";

			auto ua = header.user_agent();
			if (!ua.empty())
			{
				o << " " << ua;
				auto pui = header.simple("x-av-physical-unit-info");
				auto ci = header.simple("x-av-client-info");
				if (!pui.empty() || !ci.empty())
				{
					o << " | " << pui;
					if (!pui.empty() && !ci.empty())
						o << " ";
					o << ci;
				}
			}
			o << "\n";

			if (!SOAPAction.empty())
			{
				if (doc)
				{
					o << "\n[" << SOAPAction << "]:\n";
					auto children = env_body(doc);
					if (children)
						dom::Print(o, children, false, 1);
					else
						dom::Print(o, doc->documentElement(), false, 1);
					o << "\n";
				}
				else
					o << "    [" << SOAPAction << "]\n";
			}

			std::cout << o.str();
		}

		std::pair<fs::path, fs::path> pop(const fs::path& p)
		{
			if (p.empty())
				return std::make_pair(p, p);
			auto it = p.begin();
			auto root = *it++;
			fs::path rest;
			while (it != p.end())
				rest /= *it++;

			return std::make_pair(root, rest);
		}

		std::pair<std::string, std::string> break_action(const std::string& action)
		{
			auto hash = action.find('#');

			// no hash - no function; no function - no sense
			if (hash == std::string::npos)
				return std::make_pair(std::string(), std::string());

			return std::make_pair(action.substr(0, hash), action.substr(hash + 1));
		}

		void request_handler::handle(const http_request& req, response& resp)
		{
			auto SOAPAction = req.SOAPAction();
			auto res = req.resource();
			auto method = req.method();

			dom::XmlDocumentPtr doc;

			if (method == http_method::post && !SOAPAction.empty())
			{
				doc = create_from_socket(req.request_data());
			}

			print_debug(req, SOAPAction, doc);

			fs::path root, rest;
			std::tie(root, rest) = pop(res); // pop leading slash
			std::tie(root, rest) = pop(rest);

			if (method == http_method::get)
			{
				if (root == "config")
				{
					if (rest == "device.xml")
						return make_device_xml(resp);

					return make_file(boost::filesystem::path("data") / root / rest, resp);
				}
				if (root == "images")
					return make_file(boost::filesystem::path("data") / root / rest, resp);
			}

			if (method == http_method::post)
			{
				if (root == "upnp")
				{
					std::string soap_type, soap_method;
					std::tie(soap_type, soap_method) = break_action(SOAPAction);

					ssdp::service_ptr service;
					for (auto&& candidate: ssdp::services(m_device))
					{
						if (soap_type == candidate->get_type())
						{
							service = candidate;
							break;
						}
					}

					if (!service)
						return make_404(resp);

					std::tie(root, rest) = pop(rest);
					if (root == "control")
					{
						if (rest == service->get_uri()) try
						{
							if (service->control_call_by_name(soap_method, req, doc, resp))
								return;
						}
						catch (...) { return make_500(resp); }
					}

					if (root == "event")
					{
						if (rest == service->get_uri()) try
						{
							if (service->event_call_by_name(soap_method, req, doc, resp))
								return;
						}
						catch (...) { return make_500(resp); }
					}
				}
			}

			make_404(resp);
		}

		void request_handler::make_templated(const char* tmplt, const char* content_type, response& resp)
		{
			auto & header = resp.header();
			header.clear(m_device->server());
			header.append("content-type", content_type);
			resp.content(std::make_shared<template_content>(tmplt, std::ref(m_vars)));
		}

		void request_handler::make_device_xml(response& resp)
		{
			auto & header = resp.header();
			header.clear(m_device->server());
			header.append("content-type", "text/xml; charset=\"utf-8\"");
			resp.content(content::from_string(m_device->get_configuration(to_string(iface::get_default_interface()) + ":" + std::to_string(m_port))));
		}

		static struct
		{
			std::string ext;
			const char* mime_type;
		} s_extensions [] = {
			{ ".xml", "text/xml" },
			{ ".png", "image/png" }
		};

		void request_handler::make_file(const fs::path& path, response& resp)
		{
			if (!fs::exists(path))
				return make_404(resp);

			const char* content_type = "text/html";
			if (path.has_extension())
			{
				std::string cmp = path.extension().string();
				for (auto&& c : cmp) c = std::tolower((unsigned char) c);

				for (auto&& ext : s_extensions)
					if (ext.ext == cmp)
					{
						content_type = ext.mime_type;
						break;
					}
			}

			auto & header = resp.header();
			header.clear(m_device->server());
			header.append("content-type", content_type);
			header.append("last-modified")->out() << to_string(time::last_write(path));
			resp.content(content::from_file(path));
		}

		void request_handler::make_404(response& resp)
		{
			auto & header = resp.header();
			header.clear(m_device->server());
			header.m_status = 404;
			header.append("content-type", "text/plain");
			resp.content(content::from_string("File not found...\n"));
		}

		void request_handler::make_500(response& resp)
		{
			auto & header = resp.header();
			header.clear(m_device->server());
			header.m_status = 500;
		}
	}
}