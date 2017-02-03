/*!
 * Title ---- km/config.hpp
 * Author --- Giacomo Trudu aka `Wicker25` - wicker25[at]gmail[dot]com
 *
 * Copyright (C) 2017 by Giacomo Trudu.
 * All rights reserved.
 *
 * This file is part of KeyMaker software.
 */

#ifndef __KM_CONFIG_HPP__
#define __KM_CONFIG_HPP__

#include <km.hpp>
#include <km/Buffer.hpp>

#include <vector>
#include <map>

namespace km { // Begin main namespace

/*!
 * The list of owner configs.
 */
static const std::map<const Buffer, Buffer> OwnerConfigs =
{
    { "name",        "Name"        },
    { "email",       "Email"       },
    { "company",     "Company"     },
    { "private_key", "Private Key" }
};


/*!
 * The list of standard entry fields.
 */
static const std::map<const Buffer, Buffer> StandardEntryFields =
{
    { "name",     "Name"      },
    { "username", "Username"  },
    { "password", "Password"  },
    { "host",     "Host"      },
    { "type",     "Type"      },
    { "shell",    "Shell"     },
    { "tunnel0",  "Tunnel #0" },
    { "tunnel1",  "Tunnel #1" },
    { "tunnel2",  "Tunnel #2" },
    { "tunnel3",  "Tunnel #3" },
    { "tunnel4",  "Tunnel #4" },
    { "vpn0",     "VPN #0"    },
    { "vpn1",     "VPN #1"    },
    { "vpn2",     "VPN #2"    },
    { "vpn3",     "VPN #3"    },
    { "vpn4",     "VPN #4"    }
};

} // End of main namespace

#endif /* __KM_CONFIG_HPP__ */