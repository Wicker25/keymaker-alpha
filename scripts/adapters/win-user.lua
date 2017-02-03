#!/usr/bin/env lua

local WinUser = {}

-------------------------------------
-- Constructor method.
-------------------------------------
function WinUser:new(client)
  local o = {}
  setmetatable(o, self)
  self.__index = self

  self.client = client
  return o
end

-------------------------------------
-- Tests the credentials.
-------------------------------------
function WinUser:test_credentials(username, password)
  print('test_password')
end

-------------------------------------
-- Changes the credentials.
-------------------------------------
function WinUser:change_credentials(username, current_password, new_password)
  print('change_password')
end

return WinUser