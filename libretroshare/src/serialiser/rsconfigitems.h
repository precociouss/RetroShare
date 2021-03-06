#ifndef RS_CONFIG_ITEMS_SERIALISER_H
#define RS_CONFIG_ITEMS_SERIALISER_H

/*
 * libretroshare/src/serialiser: rsconfigitems.h
 *
 * RetroShare Serialiser.
 *
 * Copyright 2007-2008 by Robert Fernie.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 2 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * Please report all bugs and problems to "retroshare@lunamutt.com".
 *
 */

#include <map>
#include <vector>

#include "retroshare/rstypes.h"
#include "serialiser/rsserial.h"

#include "serialiser/rstlvidset.h"
#include "serialiser/rstlvfileitem.h"
#include "serialiser/rstlvkeyvalue.h"
#include "serialiser/rstlvaddrs.h"

class RsGroupInfo;

const uint8_t RS_PKT_TYPE_GENERAL_CONFIG = 0x01;
const uint8_t RS_PKT_TYPE_PEER_CONFIG    = 0x02;
const uint8_t RS_PKT_TYPE_CACHE_CONFIG   = 0x03;
const uint8_t RS_PKT_TYPE_FILE_CONFIG    = 0x04;
const uint8_t RS_PKT_TYPE_PLUGIN_CONFIG  = 0x05;
const uint8_t RS_PKT_TYPE_HISTORY_CONFIG = 0x06;

	/* GENERAL CONFIG SUBTYPES */
const uint8_t RS_PKT_SUBTYPE_KEY_VALUE = 0x01;

	/* PEER CONFIG SUBTYPES */
const uint8_t RS_PKT_SUBTYPE_PEER_STUN             = 0x02;
const uint8_t RS_PKT_SUBTYPE_PEER_NET              = 0x03;
const uint8_t RS_PKT_SUBTYPE_PEER_GROUP_deprecated = 0x04;
const uint8_t RS_PKT_SUBTYPE_PEER_PERMISSIONS      = 0x05;
const uint8_t RS_PKT_SUBTYPE_PEER_BANDLIMITS       = 0x06;
const uint8_t RS_PKT_SUBTYPE_NODE_GROUP            = 0x07;

	/* FILE CONFIG SUBTYPES */
const uint8_t RS_PKT_SUBTYPE_FILE_TRANSFER            = 0x01;
const uint8_t RS_PKT_SUBTYPE_FILE_ITEM_deprecated     = 0x02;
const uint8_t RS_PKT_SUBTYPE_FILE_ITEM                = 0x03;

/**************************************************************************/

// We should make these items use a clean serialise code, and all derive from the same item type.

class RsPeerNetItem: public RsItem
{
	public:
	RsPeerNetItem() 
	:RsItem(RS_PKT_VERSION1, RS_PKT_CLASS_CONFIG,
		RS_PKT_TYPE_PEER_CONFIG,
		RS_PKT_SUBTYPE_PEER_NET)
	{ return; }
virtual ~RsPeerNetItem();
virtual void clear();
std::ostream &print(std::ostream &out, uint16_t indent = 0);

	/* networking information */
	RsPeerId    peerId;                       /* Mandatory */
        RsPgpId     pgpId;                        /* Mandatory */
        std::string location;                     /* Mandatory */
        uint32_t    netMode;                      /* Mandatory */
	uint16_t    vs_disc;                      /* Mandatory */
	uint16_t    vs_dht;                       /* Mandatory */
	uint32_t    lastContact;                  /* Mandatory */

	RsTlvIpAddress localAddrV4;            	/* Mandatory */
	RsTlvIpAddress extAddrV4;           	/* Mandatory */
	RsTlvIpAddress localAddrV6;            	/* Mandatory */
	RsTlvIpAddress extAddrV6;            	/* Mandatory */

        std::string dyndns;

	RsTlvIpAddrSet localAddrList;
	RsTlvIpAddrSet extAddrList;

	// for proxy connection.
	std::string domain_addr;
	uint16_t    domain_port;  
};

// This item should be merged with the next item, but that is not backward compatible.
class RsPeerServicePermissionItem : public RsItem
{
	public:
		RsPeerServicePermissionItem() : RsItem(RS_PKT_VERSION1, RS_PKT_CLASS_CONFIG, RS_PKT_TYPE_PEER_CONFIG, RS_PKT_SUBTYPE_PEER_PERMISSIONS) {}
		virtual ~RsPeerServicePermissionItem() {}

		virtual void clear()
		{
			pgp_ids.clear() ;
			service_flags.clear() ;
		}
		std::ostream &print(std::ostream &out, uint16_t indent = 0);

		/* Mandatory */
		std::vector<RsPgpId> pgp_ids ;
		std::vector<ServicePermissionFlags> service_flags ;
};
class RsPeerBandwidthLimitsItem : public RsItem
{
	public:
		RsPeerBandwidthLimitsItem() : RsItem(RS_PKT_VERSION1, RS_PKT_CLASS_CONFIG, RS_PKT_TYPE_PEER_CONFIG, RS_PKT_SUBTYPE_PEER_BANDLIMITS) {}
		virtual ~RsPeerBandwidthLimitsItem() {}

		virtual void clear()
		{
			peers.clear() ;
		}
		std::ostream &print(std::ostream &out, uint16_t indent = 0);

		/* Mandatory */
		std::map<RsPgpId,PeerBandwidthLimits> peers ;
};

class RsPeerGroupItem_deprecated : public RsItem
{
public:
    RsPeerGroupItem_deprecated();
    virtual ~RsPeerGroupItem_deprecated();

	virtual void clear();
	std::ostream &print(std::ostream &out, uint16_t indent = 0);

	/* set data from RsGroupInfo to RsPeerGroupItem */
	void set(RsGroupInfo &groupInfo);
	/* get data from RsGroupInfo to RsPeerGroupItem */
	void get(RsGroupInfo &groupInfo);

	/* Mandatory */
	std::string id;
	std::string name;
	uint32_t    flag;

	RsTlvPgpIdSet pgpList;
};

class RsNodeGroupItem: public RsItem
{
public:
    RsNodeGroupItem();
    virtual ~RsNodeGroupItem() {}

    virtual void clear();
    std::ostream &print(std::ostream &out, uint16_t indent = 0);

    explicit RsNodeGroupItem(const RsGroupInfo&) ;

   // /* set data from RsGroupInfo to RsPeerGroupItem */
   // void set(RsGroupInfo &groupInfo);
   // /* get data from RsGroupInfo to RsPeerGroupItem */
   // void get(RsGroupInfo &groupInfo);

    /* Mandatory */
    RsNodeGroupId id;
    std::string name;
    uint32_t    flag;

    RsTlvPgpIdSet pgpList;
};

class RsPeerStunItem: public RsItem
{
	public:
	RsPeerStunItem() 
	:RsItem(RS_PKT_VERSION1, RS_PKT_CLASS_CONFIG,
		RS_PKT_TYPE_PEER_CONFIG,
		RS_PKT_SUBTYPE_PEER_STUN)
	{ return; }
virtual ~RsPeerStunItem();
virtual void clear();
std::ostream &print(std::ostream &out, uint16_t indent = 0);

	RsTlvPeerIdSet stunList;		  /* Mandatory */
};

class RsPeerConfigSerialiser: public RsSerialType
{
	public:
	RsPeerConfigSerialiser()
        :RsSerialType(RS_PKT_VERSION1, RS_PKT_CLASS_CONFIG,
	                RS_PKT_TYPE_PEER_CONFIG)
	{ return; }

virtual     ~RsPeerConfigSerialiser();
	
virtual	uint32_t    size(RsItem *);
virtual	bool        serialise  (RsItem *item, void *data, uint32_t *size);
virtual	RsItem *    deserialise(void *data, uint32_t *size);

	private:


virtual	uint32_t    sizeNet(RsPeerNetItem *);
virtual	bool        serialiseNet  (RsPeerNetItem *item, void *data, uint32_t *size);
virtual	RsPeerNetItem *deserialiseNet(void *data, uint32_t *size);

virtual	uint32_t    sizeStun(RsPeerStunItem *);
virtual	bool        serialiseStun  (RsPeerStunItem *item, void *data, uint32_t *size);
virtual	RsPeerStunItem *    deserialiseStun(void *data, uint32_t *size);

virtual	uint32_t    sizeGroup(RsNodeGroupItem *);
virtual	bool        serialiseGroup  (RsNodeGroupItem *item, void *data, uint32_t *size);
virtual	RsNodeGroupItem *deserialiseGroup(void *data, uint32_t *size);
virtual	RsPeerGroupItem_deprecated *    deserialiseGroup_deprecated(void *data, uint32_t *size);

virtual	uint32_t    sizePeerBandwidthLimits(RsPeerBandwidthLimitsItem *);
virtual	bool        serialisePeerBandwidthLimits  (RsPeerBandwidthLimitsItem *item, void *data, uint32_t *size);
virtual	RsPeerBandwidthLimitsItem *deserialisePeerBandwidthLimits(void *data, uint32_t *size);

virtual	uint32_t    sizePermissions(RsPeerServicePermissionItem *);
virtual	bool        serialisePermissions  (RsPeerServicePermissionItem *item, void *data, uint32_t *size);
virtual	RsPeerServicePermissionItem *    deserialisePermissions(void *data, uint32_t *size);
};

/**************************************************************************/
/**************************************************************************/

/**************************************************************************/

class RsCacheConfig: public RsItem
{
	public:
	RsCacheConfig() 
	:RsItem(RS_PKT_VERSION1, RS_PKT_CLASS_CONFIG,
		RS_PKT_TYPE_CACHE_CONFIG,
		RS_PKT_SUBTYPE_DEFAULT)
	{ return; }
virtual ~RsCacheConfig();
virtual void clear();
std::ostream &print(std::ostream &out, uint16_t indent = 0);

        RsPeerId        pid;                 /* Mandatory */
	uint16_t        cachetypeid;            /* Mandatory */
	uint16_t        cachesubid;             /* Mandatory */

	std::string     path;    	        /* Mandatory */
	std::string     name;    	        /* Mandatory */
    RsFileHash      hash;    	        /* Mandatory */
	uint64_t	size;			/* Mandatory */

	uint32_t	recvd;                  /* Mandatory */
};


class RsCacheConfigSerialiser: public RsSerialType
{
	public:
	RsCacheConfigSerialiser()
        :RsSerialType(RS_PKT_VERSION1, RS_PKT_CLASS_CONFIG,
	                RS_PKT_TYPE_CACHE_CONFIG)
	{ return; }

virtual     ~RsCacheConfigSerialiser();
	
virtual	uint32_t    size(RsItem *);
virtual	bool        serialise  (RsItem *item, void *data, uint32_t *size);
virtual	RsItem *    deserialise(void *data, uint32_t *size);

};

/**************************************************************************/

class RsFileTransfer: public RsItem
{
	public:
		RsFileTransfer() :RsItem(RS_PKT_VERSION1, RS_PKT_CLASS_CONFIG, RS_PKT_TYPE_FILE_CONFIG, RS_PKT_SUBTYPE_FILE_TRANSFER)
		{ 
			state = 0;
			in = 0;
			transferred = 0;
			crate = 0;
			trate = 0;
			lrate = 0;
			ltransfer = 0;
			flags = 0;
			chunk_strategy = 0;
		}
		virtual ~RsFileTransfer();
		virtual void clear();
		std::ostream &print(std::ostream &out, uint16_t indent = 0);

		RsTlvFileItem file;
		RsTlvPeerIdSet allPeerIds;

        	RsPeerId cPeerId;

		uint16_t state;
		uint16_t in;

		uint64_t transferred;
		uint32_t crate;
		uint32_t trate;

		uint32_t lrate;
		uint32_t ltransfer;

		// chunk information
		uint32_t flags ;
		uint32_t chunk_strategy ;				// strategy flags for chunks
		CompressedChunkMap compressed_chunk_map ;	// chunk availability (bitwise)
};

/**************************************************************************/

const uint32_t RS_FILE_CONFIG_CLEANUP_DELETE = 0x0001;

/* Used by ft / extralist / configdirs / anyone who wants a basic file */ 
class RsFileConfigItem_deprecated: public RsItem
{
public:
    RsFileConfigItem_deprecated()
        :RsItem(RS_PKT_VERSION1, RS_PKT_CLASS_CONFIG,
                RS_PKT_TYPE_FILE_CONFIG,
                RS_PKT_SUBTYPE_FILE_ITEM_deprecated)
    {}
    virtual ~RsFileConfigItem_deprecated() {}
    virtual void clear();
    std::ostream &print(std::ostream &out, uint16_t indent = 0);

    RsTlvFileItem file;
    uint32_t flags;
    std::list<std::string> parent_groups ;
};

class RsFileConfigItem: public RsItem
{
public:
    RsFileConfigItem()
        :RsItem(RS_PKT_VERSION1, RS_PKT_CLASS_CONFIG,
                RS_PKT_TYPE_FILE_CONFIG,
                RS_PKT_SUBTYPE_FILE_ITEM)
    {}
    virtual ~RsFileConfigItem() {}
    virtual void clear();
    std::ostream &print(std::ostream &out, uint16_t indent = 0);

    RsTlvFileItem file;
    uint32_t flags;
    RsTlvNodeGroupIdSet parent_groups ;
};
/**************************************************************************/

class RsFileConfigSerialiser: public RsSerialType
{
	public:
	RsFileConfigSerialiser()
	:RsSerialType(RS_PKT_VERSION1, RS_PKT_CLASS_CONFIG, 
		RS_PKT_TYPE_FILE_CONFIG)
	{ return; }
virtual     ~RsFileConfigSerialiser() { return; }
	
virtual	uint32_t    size(RsItem *);
virtual	bool        serialise  (RsItem *item, void *data, uint32_t *size);
virtual	RsItem *    deserialise(void *data, uint32_t *size);

	private:

virtual	uint32_t    sizeTransfer(RsFileTransfer *);
virtual	bool        serialiseTransfer(RsFileTransfer *item, void *data, uint32_t *size);
virtual	RsFileTransfer *  deserialiseTransfer(void *data, uint32_t *size);

virtual	RsFileConfigItem_deprecated *  deserialiseFileItem_deprecated(void *data, uint32_t *size);

virtual	uint32_t    sizeFileItem(RsFileConfigItem *);
virtual	bool        serialiseFileItem(RsFileConfigItem *item, void *data, uint32_t *size);
virtual	RsFileConfigItem *deserialiseFileItem(void *data, uint32_t *size);

};

/**************************************************************************/

/* Config items that are used generally */

class RsConfigKeyValueSet: public RsItem
{
	public:
	RsConfigKeyValueSet() 
	:RsItem(RS_PKT_VERSION1, RS_PKT_CLASS_CONFIG,
		RS_PKT_TYPE_GENERAL_CONFIG,
		RS_PKT_SUBTYPE_KEY_VALUE)
	{ return; }
virtual ~RsConfigKeyValueSet();
virtual void clear();
std::ostream &print(std::ostream &out, uint16_t indent = 0);

	RsTlvKeyValueSet tlvkvs;
};


class RsGeneralConfigSerialiser: public RsSerialType
{
	public:
	RsGeneralConfigSerialiser()
        :RsSerialType(RS_PKT_VERSION1, RS_PKT_CLASS_CONFIG,
	                RS_PKT_TYPE_GENERAL_CONFIG)
	{ return; }

virtual     ~RsGeneralConfigSerialiser();
	
virtual	uint32_t    size(RsItem *);
virtual	bool        serialise  (RsItem *item, void *data, uint32_t *size);
virtual	RsItem *    deserialise(void *data, uint32_t *size);

	private:
uint32_t    sizeKeyValueSet(RsConfigKeyValueSet *item);
bool     serialiseKeyValueSet(RsConfigKeyValueSet *item, void *data, uint32_t *pktsize);
RsConfigKeyValueSet *deserialiseKeyValueSet(void *data, uint32_t *pktsize);

};

#endif /* RS_CONFIG_ITEMS_SERIALISER_H */
