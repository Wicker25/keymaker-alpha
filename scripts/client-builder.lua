#!/usr/bin/env lua

local LocalClient = require('clients/local-client')

-------------------------------------
-- Builds a local client.
-- @param options The parameters.
-------------------------------------
local function build_local_client(options)
  return LocalClient:new()
end

-------------------------------------
-- Builds a SSH client.
-- @param options The parameters.
-------------------------------------
local function build_ssh_client(options)

  local client = ssh.Client.new(options.hostname, options.port);
  client:connect(options.username, options.password)

  return client
end

-------------------------------------
-- Builds a SMB client.
-- @param options The parameters.
-------------------------------------
local function build_smb_client(options)
  print('ERROR!')
end

-------------------------------------
-- Builds the client.
-- @param options The parameters.
-------------------------------------
local function build_client(options)

  local type = tostring(options.type)

  if type == 'ssh' then return build_ssh_client(options) end
  if type == 'smb' then return build_smb_client(options) end

  return build_local_client(options)
end

-- Export the builder function
core.build_client = build_client