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
#ifndef __SSDP_MEDIA_SERVER_HPP__
#define __SSDP_MEDIA_SERVER_HPP__

#include <ssdp/device.hpp>
#include <directory.hpp>
#include <manager.hpp>

namespace net { namespace ssdp { namespace import { namespace av {

	namespace items
	{
		struct sort_criterion
		{
			bool m_ascending;
			std::string m_term;
		};
		typedef std::vector<sort_criterion> sort_criteria;

		struct media_item;
		typedef std::shared_ptr<media_item> media_item_ptr;

		struct media_item
		{
			typedef av::items::media_item_ptr item_ptr;
			typedef std::vector<item_ptr> container_type;

			media_item() : m_id(0) {}
			virtual ~media_item() {}
			virtual container_type list(ulong start_from, ulong max_count, const sort_criteria& sort) = 0;
			virtual ulong predict_count(ulong served) const = 0;
			virtual void check_updates() {}
			virtual ulong update_id() const = 0;
			virtual media_item_ptr get_item(const std::string& id) = 0;
			virtual void set_id(uint id) { m_id = id; }
			virtual uint get_id() const { return m_id; }
			virtual bool is_folder() const = 0;
			virtual void output(std::ostream& o, const std::vector<std::string>& filter) const = 0;

			virtual void set_objectId_attr(const std::string& object_id) { m_object_id = object_id; }
			virtual std::string get_objectId_attr() const { return m_object_id; }
			virtual std::string get_parent_attr() const;

			virtual void set_title(const std::string& title) { m_title = title; }
			virtual std::string get_title() const { return m_title; }

		private:
			uint m_id;
			std::string m_object_id;
			std::string m_title;
		};

		static const char SEP = '.';
		static const ulong INVALID_ID = (ulong) - 1;

		struct root_item;
		typedef std::shared_ptr<root_item> root_item_ptr;

		std::pair<media_item_ptr, std::string> find_item(std::vector<media_item_ptr>& items, const std::string& id);

		struct common_props_item : media_item
		{
			virtual const char* get_upnp_class() const = 0;
			virtual time_t get_last_write_time() const { return 0; }
		protected:
			void output_open(std::ostream& o, const std::vector<std::string>& filter, ulong child_count) const;
			void output_close(std::ostream& o, const std::vector<std::string>& filter) const;
		};

		struct common_item : common_props_item
		{
			std::vector<media_item_ptr> list(ulong start_from, ulong max_count, const sort_criteria& sort) override { return std::vector<media_item_ptr>(); }
			ulong predict_count(ulong served) const override { return served; }
			ulong update_id() const override { return 0; }
			media_item_ptr get_item(const std::string& id) override { return nullptr; }
			bool is_folder() const override { return false; }
		};

		struct photo_item : common_item
		{
			const char* get_upnp_class() const override { return "object.item.imageItem.photo"; }
		};

		struct video_item : common_item
		{
			const char* get_upnp_class() const override { return "object.item.videoItem"; }
		};

		struct audio_item : common_item
		{
			const char* get_upnp_class() const override { return "object.item.audioItem.musicTrack"; }
		};

		struct container_item : common_props_item
		{
			container_item()
				: m_update_id(1)
				, m_current_max(0)
			{
			}

			std::vector<media_item_ptr> list(ulong start_from, ulong max_count, const sort_criteria& sort) override;
			ulong predict_count(ulong served) const override { return m_children.size(); }
			ulong update_id() const override { return m_update_id; }
			media_item_ptr get_item(const std::string& id) override;
			bool is_folder() const override { return true; }
			void output(std::ostream& o, const std::vector<std::string>& filter) const override;
			const char* get_upnp_class() const override { return "object.container.storageFolder"; }

			virtual void rescan_if_needed() {}
			virtual void folder_changed() { m_update_id++; /*notify?*/ }
			virtual void add_child(media_item_ptr);
			virtual void remove_child(media_item_ptr);

		private:
			ulong m_current_max;
			ulong m_update_id;
			std::vector<media_item_ptr> m_children;
		};
	}

	using namespace import::directory;
	using namespace import::manager;

	struct MediaServer;

	struct ContentDirectory: ContentDirectoryServerProxy
	{
		MediaServer* m_device;
		ContentDirectory(MediaServer* device) : m_device(device) {}

		error_code GetSystemUpdateID(const http::http_request& http_request, ui4& Id) override;

		error_code Search(const http::http_request& http_request, const std::string& ContainerID,
		                  const std::string& SearchCriteria, const std::string& Filter,
		                  ui4 StartingIndex, ui4 RequestedCount, const std::string& SortCriteria,
		                  std::string& Result, ui4& NumberReturned, ui4& TotalMatches,
		                  ui4& UpdateID) override;

		error_code GetSearchCapabilities(const http::http_request& http_request,
		                                 std::string& SearchCaps) override;

		error_code GetSortCapabilities(const http::http_request& http_request, std::string& SortCaps) override;

		error_code Browse(const http::http_request& http_request, const std::string& ObjectID,
		                  A_ARG_TYPE_BrowseFlag BrowseFlag, const std::string& Filter,
		                  ui4 StartingIndex, ui4 RequestedCount, const std::string& SortCriteria,
		                  std::string& Result, ui4& NumberReturned, ui4& TotalMatches,
		                  ui4& UpdateID) override;
	};

	struct ConnectionManager : ConnectionManagerServerProxy
	{
		MediaServer* m_device;
		ConnectionManager(MediaServer* device) : m_device(device) {}

		error_code GetCurrentConnectionInfo(const http::http_request& http_request,
			i4 ConnectionID, i4& RcsID, i4& AVTransportID,
			std::string& ProtocolInfo, std::string& PeerConnectionManager,
			i4& PeerConnectionID, A_ARG_TYPE_Direction& Direction,
			A_ARG_TYPE_ConnectionStatus& Status) override;

		error_code ConnectionComplete(const http::http_request& http_request, i4 ConnectionID) override;

		error_code PrepareForConnection(const http::http_request& http_request,
			const std::string& RemoteProtocolInfo, const std::string& PeerConnectionManager,
			i4 PeerConnectionID, A_ARG_TYPE_Direction Direction,
			i4& ConnectionID, i4& AVTransportID, i4& RcsID) override;

		error_code GetProtocolInfo(const http::http_request& http_request, std::string& Source,
			std::string& Sink) override;

		error_code GetCurrentConnectionIDs(const http::http_request& http_request,
			std::string& ConnectionIDs) override;
	};

	struct MediaServer : Device
	{
		MediaServer(const device_info& info, const config::config_ptr& config)
			: Device(info, config)
			, m_root_item(create_root_item())
			, m_directory(std::make_shared<ContentDirectory>(this))
			, m_manager(std::make_shared<ConnectionManager>(this))
			, m_system_update_id(1)
		{
			add(m_directory);
			add(m_manager);
		}

		const char* get_type() const override { return "urn:schemas-upnp-org:device:MediaServer:1"; }
		const char* get_description() const override { return "UPnP/AV 1.0 Compliant Media Server"; }

		ulong system_update_id() const { return m_system_update_id; }

		items::media_item_ptr get_item(const std::string& id);
		void add_root_element(items::media_item_ptr);
		void remove_root_element(items::media_item_ptr);

	private:
		items::root_item_ptr m_root_item;
		std::shared_ptr<ContentDirectory> m_directory;
		std::shared_ptr<ConnectionManager> m_manager;
		ulong m_system_update_id;

		items::root_item_ptr create_root_item();
	};

}}}} // net::ssdp::import::av

#endif //__SSDP_MEDIA_SERVER_HPP__
